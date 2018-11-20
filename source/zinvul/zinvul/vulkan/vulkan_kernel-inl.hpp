/*!
  \file vulkan_kernel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_KERNEL_INL_HPP
#define ZINVUL_VULKAN_KERNEL_INL_HPP

#include "vulkan_kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Vulkan
#include <vulkan/vulkan.hpp>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_group.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
VulkanKernel<GroupType, kDimension, ArgumentTypes...>::VulkanKernel(
    VulkanDevice* device,
    const uint32b module_index,
    const char* kernel_name) noexcept :
        device_{device}
{
  ZISC_ASSERT(device_ != nullptr, "The device is null.");
  initialize(module_index, kernel_name);
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
VulkanKernel<GroupType, kDimension, ArgumentTypes...>::~VulkanKernel() noexcept
{
  destroy();
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::destroy() noexcept
{
  const auto& device = device_->device();
  if (compute_pipeline_) {
    device.destroyPipeline(compute_pipeline_, nullptr);
    compute_pipeline_ = nullptr;
  }
  if (pipeline_layout_) {
    device.destroyPipelineLayout(pipeline_layout_, nullptr);
    pipeline_layout_ = nullptr;
  }
  if (descriptor_pool_) {
    device.destroyDescriptorPool(descriptor_pool_, nullptr);
    descriptor_pool_ = nullptr;
  }
  if (descriptor_set_layout_) {
    device.destroyDescriptorSetLayout(descriptor_set_layout_, nullptr);
    descriptor_set_layout_ = nullptr;
  }
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
VulkanDevice* VulkanKernel<GroupType, kDimension, ArgumentTypes...>::device()
    noexcept
{
  return device_;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
const VulkanDevice* VulkanKernel<GroupType, kDimension, ArgumentTypes...>::device()
    const noexcept
{
  return device_;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
DeviceType VulkanKernel<GroupType, kDimension, ArgumentTypes...>::deviceType()
    const noexcept
{
  return DeviceType::kVulkan;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
template <typename ...Buffers> inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::bindBuffers(
    std::add_lvalue_reference_t<Buffers>... args) noexcept
{
  constexpr std::size_t n = KernelBase::numOfArguments();
  std::array<vk::Buffer, n> buffer_list{refer<ArgumentTypes>(args)...};
  std::array<vk::DescriptorBufferInfo, n> descriptor_info_list;
  std::array<vk::WriteDescriptorSet, n> descriptor_set_list;
  setBufferInfo(buffer_list.data(),
                descriptor_info_list.data(),
                descriptor_set_list.data());

  const auto& device = device_->device();
  device.updateDescriptorSets(zisc::cast<uint32b>(n),
                              descriptor_set_list.data(),
                              0,
                              nullptr);
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::dispatch(
    std::array<uint32b, kDimension> works) noexcept
{
  const auto workgroup_size = device_->getWorkgroupSize(this->workgroupDimension());
  auto num_of_works = this->getNumOfWorks(works);
  for (std::size_t i = 0; i < this->workgroupDimension(); ++i) {
    num_of_works[i] = ((num_of_works[i] % workgroup_size[i]) == 0)
        ? num_of_works[i] / workgroup_size[i]
        : num_of_works[i] / workgroup_size[i] + 1;
  }

  const vk::CommandBufferBeginInfo begin_info{};
  command_buffer_.begin(begin_info);

  command_buffer_.bindPipeline(vk::PipelineBindPoint::eCompute, compute_pipeline_);
  command_buffer_.bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                     pipeline_layout_,
                                     0,
                                     1,
                                     &descriptor_set_,
                                     0,
                                     nullptr);
  command_buffer_.dispatch(num_of_works[0], num_of_works[1], num_of_works[2]);

  command_buffer_.end();
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::initCommandBuffer()
    noexcept
{
  const vk::CommandBufferAllocateInfo alloc_info{
      device_->commandPool(),
      vk::CommandBufferLevel::ePrimary,
      1};
  const auto& device = device_->device();
  auto [result, command_buffers] = device.allocateCommandBuffers(alloc_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Command buffer allocation failed.");
  command_buffer_ = command_buffers[0];
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::initComputePipeline(
    const uint32b module_index,
    const char* kernel_name) noexcept
{
  // Set work group size
  const auto workgroup_size = device_->getWorkgroupSize(this->workgroupDimension());
  const vk::SpecializationMapEntry entries[] = {
    {0, 0 * sizeof(uint32b), sizeof(uint32b)},
    {1, 1 * sizeof(uint32b), sizeof(uint32b)},
    {2, 2 * sizeof(uint32b), sizeof(uint32b)}};
  const vk::SpecializationInfo info{3,  
                                    entries,
                                    3 * sizeof(uint32b),
                                    workgroup_size.data()};
  // Shader stage create info
  const auto& shader_module = device_->getShaderModule(module_index);
  const vk::PipelineShaderStageCreateInfo shader_stage_create_info{
      vk::PipelineShaderStageCreateFlags{},
      vk::ShaderStageFlagBits::eCompute,
      shader_module,
      kernel_name,
      &info};
  // Pipeline create info
  const vk::ComputePipelineCreateInfo create_info{
      vk::PipelineCreateFlags{},
      shader_stage_create_info,
      pipeline_layout_};

  const auto& device = device_->device();
  auto [result, pipelines] = device.createComputePipelines(vk::PipelineCache{},
                                                           create_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Compute pipeline creation failed.");
  compute_pipeline_ = pipelines[0];
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::initDescriptorPool()
    noexcept
{
  const vk::DescriptorPoolSize pool_size{
      vk::DescriptorType::eStorageBuffer,
      zisc::cast<uint32b>(this->numOfArguments())};
  const vk::DescriptorPoolCreateInfo create_info{vk::DescriptorPoolCreateFlags{},
                                                 1,
                                                 1,
                                                 &pool_size};
  const auto& device = device_->device();
  auto [result, descriptor_pool] = device.createDescriptorPool(create_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Descriptor pool creation failed.");
  descriptor_pool_ = descriptor_pool;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::initDescriptorSet()
    noexcept
{
  const vk::DescriptorSetAllocateInfo alloc_info{descriptor_pool_,
                                                 1,
                                                 &descriptor_set_layout_};
  const auto& device = device_->device();
  auto [result, descriptor_sets] = device.allocateDescriptorSets(alloc_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Descriptor set allocation failed.");
  descriptor_set_ = descriptor_sets[0];
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::initDescriptorSetLayout()   noexcept
{
  constexpr std::size_t n = KernelBase::numOfArguments();
  std::array<vk::DescriptorSetLayoutBinding, n> layout_bindings;
  for (uint32b i = 0; i < n; ++i) {
    layout_bindings[i] = vk::DescriptorSetLayoutBinding{
        i,
        vk::DescriptorType::eStorageBuffer,
        1,
        vk::ShaderStageFlagBits::eCompute};
  }
  const vk::DescriptorSetLayoutCreateInfo create_info{
      vk::DescriptorSetLayoutCreateFlags{},
      zisc::cast<uint32b>(layout_bindings.size()),
      layout_bindings.data()};
  const auto& device = device_->device();
  auto [result, descriptor_set_layout] = device.createDescriptorSetLayout(create_info);
  ZISC_ASSERT(result == vk::Result::eSuccess,
              "Descriptor set layout creation failed.");
  descriptor_set_layout_ = descriptor_set_layout;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::initialize(
    const uint32b module_index, const char* kernel_name) noexcept
{
  initDescriptorSetLayout();
  initDescriptorPool();
  initDescriptorSet();
  initPipelineLayout();
  initComputePipeline(module_index, kernel_name);
  initCommandBuffer();
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::initPipelineLayout()
    noexcept
{
  const vk::PipelineLayoutCreateInfo create_info{
      vk::PipelineLayoutCreateFlags{},
      1,
      &descriptor_set_layout_};
  const auto& device = device_->device();
  auto [result, pipeline_layout] = device.createPipelineLayout(create_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Pipeline layout creation failed.");
  pipeline_layout_ = pipeline_layout;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
template <typename Type> inline
vk::Buffer& VulkanKernel<GroupType, kDimension, ArgumentTypes...>::refer(
    BufferRef<Type> buffer) const noexcept
{
  ZISC_ASSERT(buffer.deviceType() == DeviceType::kVulkan,
              "The device type of the buffer isn't vulkan.");
  using VulkanBufferP =
      std::add_pointer_t<VulkanBuffer<std::remove_pointer_t<Type>>>;
  auto vulkan_buffer = zisc::cast<VulkanBufferP>(&buffer);
  return vulkan_buffer->buffer();
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void VulkanKernel<GroupType, kDimension, ArgumentTypes...>::setBufferInfo(
    vk::Buffer* buffer_list,
    vk::DescriptorBufferInfo* descriptor_info_list,
    vk::WriteDescriptorSet* descriptor_set_list) const noexcept
{
  constexpr std::size_t n = KernelBase::numOfArguments();
  for (std::size_t index = 0; index < n; ++index) {
    auto& descriptor_info = descriptor_info_list[index];
    descriptor_info.buffer = buffer_list[index];
    descriptor_info.offset = 0;
    descriptor_info.range = VK_WHOLE_SIZE;

    auto& descriptor_set = descriptor_set_list[index];
    descriptor_set.dstSet = descriptor_set_;
    descriptor_set.dstBinding = zisc::cast<uint32b>(index);
    descriptor_set.dstArrayElement = 0;
    descriptor_set.descriptorCount = 1;
    descriptor_set.descriptorType = vk::DescriptorType::eStorageBuffer;
    descriptor_set.pImageInfo = nullptr;
    descriptor_set.pBufferInfo = &descriptor_info;
    descriptor_set.pTexelBufferView = nullptr;
  }
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_KERNEL_INL_HPP
