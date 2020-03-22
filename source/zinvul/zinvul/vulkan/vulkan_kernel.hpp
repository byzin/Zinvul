/*!
  \file vulkan_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_KERNEL_HPP
#define ZINVUL_VULKAN_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Vulkan
#include <vulkan/vulkan.h>
// Zinvul
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"
#include "zinvul/utility/kernel_init_parameters.hpp"

namespace zinvul {

// Forward declaration
template <typename Type> class Buffer;
class VulkanDevice;
template <std::size_t kDimension, typename FuncArgTypes, typename ...ArgTypes>
class VulkanKernel;

/*!
  \brief No brief description

  No detailed description.

  \tparam kDimension No description.
  \tparam FuncArgTypes No description.
  \tparam ArgTypes No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
class VulkanKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>
  : public Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>
{
 public:
  // Type aliases
  using BaseKernel =
      Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>;
  using InitParameters = typename BaseKernel::InitParameters;
  template <typename Type>
  using BufferRef = typename BaseKernel::template BufferRef<Type>;
  using LaunchOptions = typename BaseKernel::LaunchOptions;


  //! Initialize the kernel
  VulkanKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~VulkanKernel() noexcept override;


  //! Execute a kernel
  void run(BufferRef<ArgTypes>... args,
           const LaunchOptions& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const InitParameters& params) override;

 private:
//  //! Bind buffers
//  void bindBuffers(std::add_lvalue_reference_t<BufferArgs>... args) noexcept;
//
//  //! Dispatch
//  void dispatch(const std::array<uint32b, kDimension> works) noexcept;
//
//  //! Get the VkBuffer of the given buffer
//  template <typename Type>
//  vk::Buffer& getVkBuffer(Type&& buffer) const noexcept;
//
//  //! Initialize a command buffer
//  void initCommandBuffer() noexcept;
//
//  //! Initialize a compute pipeline
//  void initComputePipeline(const uint32b module_index,
//                           const std::string_view kernel_name) noexcept;
//
//  //! Initialize a descriptor pool
//  void initDescriptorPool() noexcept;
//
//  //! Initialize a descriptor set
//  void initDescriptorSet() noexcept;
//
//  //! Initialize a descriptor set layout
//  void initDescriptorSetLayout() noexcept;
//
//  //! Initialize a kernel
//  void initialize(const uint32b module_index,
//                  const std::string_view kernel_name) noexcept;
//
//  //! Initialize a pipeline layout
//  void initPipelineLayout() noexcept;
//
//  //! Check if the current buffers are same as previous buffers
//  bool isSameArgs(std::add_lvalue_reference_t<BufferArgs>... args) const noexcept;


//  VulkanDevice* device_;
//  vk::DescriptorSetLayout descriptor_set_layout_;
//  vk::DescriptorPool descriptor_pool_;
//  vk::DescriptorSet descriptor_set_;
//  vk::PipelineLayout pipeline_layout_;
//  vk::Pipeline compute_pipeline_;
//  vk::CommandBuffer command_buffer_;
//  std::array<vk::Buffer, kNumOfBuffers> buffer_list_;
};

} // namespace zinvul

#include "vulkan_kernel-inl.hpp"

#endif // ZINVUL_VULKAN_KERNEL_HPP
