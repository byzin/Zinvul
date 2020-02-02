/*!
  \file vulkan_kernel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_KERNEL_HPP
#define ZINVUL_VULKAN_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>
#include <type_traits>
// Vulkan
#include <vulkan/vulkan.hpp>
// Zinvul
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class VulkanDevice;

template <std::size_t kDimension, typename Function, typename ...BufferArgs>
class VulkanKernel;

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
class VulkanKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...> :
    public Kernel<kDimension, BufferArgs...>
{
  using KernelBase = Kernel<kDimension, BufferArgs...>;


  static constexpr std::size_t kNumOfBuffers = KernelBase::numOfArguments();

 public:
  //! Construct a kernel
  VulkanKernel(VulkanDevice* device,
               const uint32b module_index,
               const std::string_view kernel_name) noexcept;

  //! Destroy a kernel
  ~VulkanKernel() noexcept override;


  //! Destroy a kernel
  void destroy() noexcept;

  //! Return an assigned device
  VulkanDevice* device() noexcept;

  //! Return an assigned device
  const VulkanDevice* device() const noexcept;

  //! Return the device type
  SubPlatformType SubPlatformType() const noexcept override;

  //! Execute a kernel
  void run(std::add_lvalue_reference_t<BufferArgs>... args,
           const std::array<uint32b, kDimension> works,
           const uint32b queue_index) noexcept override;

 private:
  //! Bind buffers
  void bindBuffers(std::add_lvalue_reference_t<BufferArgs>... args) noexcept;

  //! Dispatch
  void dispatch(const std::array<uint32b, kDimension> works) noexcept;

  //! Get the VkBuffer of the given buffer
  template <typename Type>
  vk::Buffer& getVkBuffer(Type&& buffer) const noexcept;

  //! Initialize a command buffer
  void initCommandBuffer() noexcept;

  //! Initialize a compute pipeline
  void initComputePipeline(const uint32b module_index,
                           const std::string_view kernel_name) noexcept;

  //! Initialize a descriptor pool
  void initDescriptorPool() noexcept;

  //! Initialize a descriptor set
  void initDescriptorSet() noexcept;

  //! Initialize a descriptor set layout
  void initDescriptorSetLayout() noexcept;

  //! Initialize a kernel
  void initialize(const uint32b module_index,
                  const std::string_view kernel_name) noexcept;

  //! Initialize a pipeline layout
  void initPipelineLayout() noexcept;

  //! Check if the current buffers are same as previous buffers
  bool isSameArgs(std::add_lvalue_reference_t<BufferArgs>... args) const noexcept;


  VulkanDevice* device_;
  vk::DescriptorSetLayout descriptor_set_layout_;
  vk::DescriptorPool descriptor_pool_;
  vk::DescriptorSet descriptor_set_;
  vk::PipelineLayout pipeline_layout_;
  vk::Pipeline compute_pipeline_;
  vk::CommandBuffer command_buffer_;
  std::array<vk::Buffer, kNumOfBuffers> buffer_list_;
};

} // namespace zinvul

#include "vulkan_kernel-inl.hpp"

#endif // ZINVUL_VULKAN_KERNEL_HPP
