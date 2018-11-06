/*!
  \file vulkan_kernel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_KERNEL_HPP
#define ZINVUL_VULKAN_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
// Vulkan
#include <vulkan/vulkan.hpp>
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_group.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class VulkanDevice;
template <typename> class VulkanBuffer;

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
class VulkanKernel : public Kernel<GroupType, kDimension, ArgumentTypes...>
{
  using KernelBase = Kernel<GroupType, kDimension, ArgumentTypes...>;
  template <typename Type>
  using BufferRef = typename KernelBase::template BufferRef<Type>;

 public:
  //! Construct a kernel
  VulkanKernel(VulkanDevice* device,
               const uint32b module_index,
               const char* kernel_name) noexcept;

  //! Destroy a kernel
  ~VulkanKernel() noexcept override;


  //! Destroy a kernel
  void destroy() noexcept;

  //! Return an assigned device
  VulkanDevice* device() noexcept;

  //! Return an assigned device
  const VulkanDevice* device() const noexcept;

  //! Return the device type
  DeviceType deviceType() const noexcept override;

  //! Execute a kernel
  void run(BufferRef<ArgumentTypes>... args,
           const std::array<uint32b, kDimension> works) noexcept override;

 private:
  //! Bind buffers
  void bindBuffers(BufferRef<ArgumentTypes>... args) noexcept;

  //! Dispatch
  void dispatch(const std::array<uint32b, kDimension> works) noexcept;

  //! Initialize a command buffer
  void initCommandBuffer() noexcept;

  //! Initialize a compute pipeline
  void initComputePipeline(const uint32b module_index,
                           const char* kernel_name) noexcept;

  //! Initialize a descriptor pool
  void initDescriptorPool() noexcept;

  //! Initialize a descriptor set
  void initDescriptorSet() noexcept;

  //! Initialize a descriptor set layout
  void initDescriptorSetLayout() noexcept;

  //! Initialize a kernel
  void initialize(const uint32b module_index, const char* kernel_name) noexcept;

  //! Initialize a pipeline layout
  void initPipelineLayout() noexcept;

  //! Get the buffer body
  template <typename Type>
  vk::Buffer& refer(BufferRef<Type> buffer) const noexcept;

  //! Make a buffer write descriptor set
  void setBufferInfo(vk::Buffer* buffer_list,
                     vk::DescriptorBufferInfo* descriptor_info_list,
                     vk::WriteDescriptorSet* descriptor_set_list) const noexcept;


  VulkanDevice* device_;
  vk::DescriptorSetLayout descriptor_set_layout_;
  vk::DescriptorPool descriptor_pool_;
  vk::DescriptorSet descriptor_set_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline compute_pipeline_;
  vk::CommandBuffer command_buffer_;
};

} // namespace zinvul

#include "vulkan_kernel-inl.hpp"

#endif // ZINVUL_VULKAN_KERNEL_HPP
