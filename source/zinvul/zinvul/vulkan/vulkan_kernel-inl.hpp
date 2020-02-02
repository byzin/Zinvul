/*!
  \file vulkan_kernel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_KERNEL_INL_HPP
#define ZINVUL_VULKAN_KERNEL_INL_HPP

#include "vulkan_kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
// Vulkan
#include <vulkan/vulkan.hpp>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"
#include "zinvul/kernel_arg_parser.hpp"
#include "zinvul/zinvul.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::VulkanKernel(
    VulkanDevice* device,
    const uint32b module_index,
    const std::string_view kernel_name) noexcept :
        device_{device}
{
  ZISC_ASSERT(device_ != nullptr, "The device is null.");
  initialize(module_index, kernel_name);
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::~VulkanKernel()
    noexcept
{
  destroy();
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::destroy()
    noexcept
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
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
auto VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::device()
    noexcept -> VulkanDevice*
{
  return device_;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
auto VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::device()
    const noexcept -> const VulkanDevice*
{
  return device_;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
auto VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::SubPlatformType()
    const noexcept -> SubPlatformType
{
  return SubPlatformType::kVulkan;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::run(
    std::add_lvalue_reference_t<BufferArgs>... args,
    const std::array<uint32b, kDimension> works,
    const uint32b queue_index) noexcept
{
  if (!isSameArgs(args...))
    bindBuffers(args...);
  dispatch(works);
  device()->submit(QueueType::kCompute, queue_index, command_buffer_);
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::bindBuffers(
    std::add_lvalue_reference_t<BufferArgs>... args) noexcept
{
  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;

  if ((0 == kNumOfBuffers) || isSameArgs(args...))
    return;

  std::array<vk::Buffer, kNumOfBuffers> buffer_list{getVkBuffer(args)...};
  std::array<vk::DescriptorBufferInfo, kNumOfBuffers> descriptor_info_list;
  std::array<vk::WriteDescriptorSet, kNumOfBuffers> descriptor_set_list;

  constexpr auto buffer_info_list = ParseResult::getGlobalArgInfoList();
  for (std::size_t index = 0; index < kNumOfBuffers; ++index) {
    auto& descriptor_info = descriptor_info_list[index];
    descriptor_info.buffer = buffer_list[index];
    descriptor_info.offset = 0;
    descriptor_info.range = VK_WHOLE_SIZE;

    auto& buffer_info = buffer_info_list[index];
    auto& descriptor_set = descriptor_set_list[index];
    const bool is_uniform_buffer = Config::isUniformBufferEnabled() &&
        (buffer_info.isConstant() || buffer_info.isPod());
    descriptor_set.dstSet = descriptor_set_;
    descriptor_set.dstBinding = zisc::cast<uint32b>(buffer_info.index());
    descriptor_set.dstArrayElement = 0;
    descriptor_set.descriptorCount = 1;
    descriptor_set.descriptorType = is_uniform_buffer
        ? vk::DescriptorType::eUniformBuffer
        : vk::DescriptorType::eStorageBuffer;
    descriptor_set.pImageInfo = nullptr;
    descriptor_set.pBufferInfo = &descriptor_info;
    descriptor_set.pTexelBufferView = nullptr;
  }

  const auto& device = device_->device();
  device.updateDescriptorSets(zisc::cast<uint32b>(kNumOfBuffers),
                              descriptor_set_list.data(),
                              0,
                              nullptr);
  buffer_list_ = std::move(buffer_list);
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::dispatch(
    std::array<uint32b, kDimension> works) noexcept
{
  const auto group_size = device_->calcWorkGroupSize(works);
  vk::CommandBufferBeginInfo begin_info{};
  begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
  command_buffer_.begin(begin_info);

  command_buffer_.bindPipeline(vk::PipelineBindPoint::eCompute, compute_pipeline_);
  command_buffer_.bindDescriptorSets(vk::PipelineBindPoint::eCompute,
                                     pipeline_layout_,
                                     0,
                                     1,
                                     &descriptor_set_,
                                     0,
                                     nullptr);
  command_buffer_.dispatch(group_size[0], group_size[1], group_size[2]);

  command_buffer_.end();
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
template <typename Type> inline
auto VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::getVkBuffer(
    Type&& buffer) const noexcept -> vk::Buffer&
{
  ZISC_ASSERT(buffer.SubPlatformType() == SubPlatformType::kVulkan,
              "The device type of the buffer isn't vulkan.");

  using ElementType = typename std::remove_reference_t<Type>::Type;
  constexpr DescriptorType descriptor =
      std::remove_reference_t<Type>::descriptorType();
  using VulkanBufferT = VulkanBuffer<descriptor, ElementType>;
  using VulkanBufferPtr = std::add_pointer_t<VulkanBufferT>;

  auto b = zisc::cast<VulkanBufferPtr>(std::addressof(buffer));
  return b->buffer();
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initCommandBuffer()
    noexcept
{
  const vk::CommandBufferAllocateInfo alloc_info{
      device_->commandPool(QueueType::kCompute),
      vk::CommandBufferLevel::ePrimary,
      1};
  const auto& device = device_->device();
  auto [result, command_buffers] = device.allocateCommandBuffers(alloc_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Command buffer allocation failed.");
  command_buffer_ = command_buffers[0];
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initComputePipeline(
    const uint32b module_index,
    const std::string_view kernel_name) noexcept
{
  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;

  // Set constant IDs
  constexpr std::size_t num_of_entries = 3u + ParseResult::kNumOfLocalArgs;
  std::array<uint32b, num_of_entries> constant_data;
  {
    const auto& local_work_size = device_->localWorkSize<kDimension>();
    for (std::size_t i = 0; i < local_work_size.size(); ++i)
      constant_data[i] = local_work_size[i];
    for (std::size_t i = local_work_size.size(); i < num_of_entries; ++i)
      constant_data[i] = device_->subgroupSize();
  }
  std::array<vk::SpecializationMapEntry, num_of_entries> entries;
  for (std::size_t i = 0; i < entries.size(); ++i) {
    entries[i].constantID = zisc::cast<uint32b>(i);
    entries[i].offset = zisc::cast<uint32b>(i * sizeof(uint32b));
    entries[i].size = sizeof(uint32b);
  }
  const vk::SpecializationInfo info{zisc::cast<uint32b>(num_of_entries),  
                                    entries.data(),
                                    num_of_entries * sizeof(uint32b),
                                    constant_data.data()};
  // Shader stage create info
  const auto& shader_module = device_->getShaderModule(module_index);
  const vk::PipelineShaderStageCreateInfo shader_stage_create_info{
      vk::PipelineShaderStageCreateFlags{},
      vk::ShaderStageFlagBits::eCompute,
      shader_module,
      kernel_name.data(),
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
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initDescriptorPool()
    noexcept
{
  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
  static_assert(0 < ParseResult::kNumOfStorageBuffer,
                "The kernel doesn't have any storage buffer argument.");

  constexpr bool has_uniform_buffer = Config::isUniformBufferEnabled() &&
                                      (0 < ParseResult::kNumOfUniformBuffer);
  constexpr std::size_t num_of_info = has_uniform_buffer ? 2 : 1;
  std::array<vk::DescriptorPoolSize, num_of_info> pool_size_list;
  if constexpr (has_uniform_buffer) {
    pool_size_list[0].type = vk::DescriptorType::eUniformBuffer;
    pool_size_list[0].descriptorCount =
        zisc::cast<uint32b>(ParseResult::kNumOfUniformBuffer);
    pool_size_list[1].type = vk::DescriptorType::eStorageBuffer;
    pool_size_list[1].descriptorCount =
        zisc::cast<uint32b>(ParseResult::kNumOfStorageBuffer);
    static_assert(kNumOfBuffers ==
        (ParseResult::kNumOfStorageBuffer + ParseResult::kNumOfUniformBuffer),
        "The number of arguments is wrong.");
  }
  else {
    pool_size_list[0].type = vk::DescriptorType::eStorageBuffer;
    pool_size_list[0].descriptorCount = zisc::cast<uint32b>(kNumOfBuffers);
  }
  const vk::DescriptorPoolCreateInfo create_info{vk::DescriptorPoolCreateFlags{},
                                                 1,
                                                 zisc::cast<uint32b>(num_of_info),
                                                 pool_size_list.data()};
  const auto& device = device_->device();
  auto [result, descriptor_pool] = device.createDescriptorPool(create_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Descriptor pool creation failed.");
  descriptor_pool_ = descriptor_pool;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initDescriptorSet()
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
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initDescriptorSetLayout()
    noexcept
{
  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
  constexpr auto buffer_info_list = ParseResult::getGlobalArgInfoList();

  std::array<vk::DescriptorSetLayoutBinding, kNumOfBuffers> layout_bindings;
  for (std::size_t index = 0; index < kNumOfBuffers; ++index) {
    auto& buffer_info = buffer_info_list[index];
    const bool is_uniform_buffer = Config::isUniformBufferEnabled() &&
        (buffer_info.isConstant() || buffer_info.isPod());

    layout_bindings[index] = vk::DescriptorSetLayoutBinding{
      zisc::cast<uint32b>(index),
        is_uniform_buffer ? vk::DescriptorType::eUniformBuffer
                          : vk::DescriptorType::eStorageBuffer,
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
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initialize(
    const uint32b module_index, const std::string_view kernel_name) noexcept
{
  using ParseResult = KernelArgParser<kDimension, ArgumentTypes...>;
  static_assert(kNumOfBuffers == ParseResult::kNumOfGlobalArgs,
                "The number of buffers is wrong.");

  initDescriptorSetLayout();
  initDescriptorPool();
  initDescriptorSet();
  initPipelineLayout();
  initComputePipeline(module_index, kernel_name);
  initCommandBuffer();
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::initPipelineLayout()
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
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
bool VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::isSameArgs(
    std::add_lvalue_reference_t<BufferArgs>... args) const noexcept
{
  std::array<vk::Buffer, kNumOfBuffers> buffer_list{{getVkBuffer(args)...}};
  bool result = true;
  for (std::size_t i = 0; (i < buffer_list.size()) && result; ++i)
    result = buffer_list_[i] == buffer_list[i];
  return result;
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_KERNEL_INL_HPP
