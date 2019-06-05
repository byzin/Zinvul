/*!
  \file zinvul-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_INL_HPP
#define ZINVUL_ZINVUL_INL_HPP

#include "zinvul.hpp"
// Standard C++ library
#include <cstddef>
#include <string_view>
#include <type_traits>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "kernel.hpp"
#include "kernel_set.hpp"
#include "kernel_arg_parser.hpp"
#include "cpu/cpu_buffer.hpp"
#include "cpu/cpu_device.hpp"
#include "cpu/cpu_kernel.hpp"
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
#include "vulkan/vulkan_buffer.hpp"
#include "vulkan/vulkan_device.hpp"
#include "vulkan/vulkan_kernel.hpp"
#endif // ZINVUL_ENABLE_VULKAN_BACKEND
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <BufferType kBufferType, typename Type> inline
UniqueBuffer<kBufferType, Type> makeBuffer(
    Device* device,
    const BufferUsage usage_flag) noexcept
{
  UniqueBuffer<kBufferType, Type> buffer;
  switch (device->deviceType()) {
   case DeviceType::kCpu: {
    auto d = zisc::cast<CpuDevice*>(device);
    buffer = d->makeBuffer<kBufferType, Type>(usage_flag);
    break;
   }
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
   case DeviceType::kVulkan: {
    auto d = zisc::cast<VulkanDevice*>(device);
    buffer = d->makeBuffer<kBufferType, Type>(usage_flag);
    break;
   }
#endif // ZINVUL_ENABLE_VULKAN_BACKEND
   default: {
    ZISC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return buffer;
}

/*!
  */
template <typename Type> inline
UniqueBuffer<BufferType::kUniform, Type> makeUniformBuffer(
    Device* device,
    const BufferUsage usage_flag) noexcept
{
  auto buffer = makeBuffer<BufferType::kUniform, Type>(device, usage_flag);
  return buffer;
}

/*!
  */
template <typename Type> inline
UniqueBuffer<BufferType::kStorage, Type> makeStorageBuffer(
    Device* device,
    const BufferUsage usage_flag) noexcept
{
  auto buffer = makeBuffer<BufferType::kStorage, Type>(device, usage_flag);
  return buffer;
}

/*!
  */
inline
UniqueDevice makeDevice(DeviceOptions& options) noexcept
{

  UniqueDevice device;
  switch (options.device_type_) {
   case DeviceType::kCpu: {
    using UniqueCpuDevice = zisc::UniqueMemoryPointer<CpuDevice>;
    device = UniqueCpuDevice::make(options.mem_resource_, options);
    break;
   }
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
   case DeviceType::kVulkan: {
    using UniqueVulkanDevice = zisc::UniqueMemoryPointer<VulkanDevice>;
    device = UniqueVulkanDevice::make(options.mem_resource_, options);
    break;
   }
#endif // ZINVUL_ENABLE_VULKAN_BACKEND
   default: {
    ZISC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return device;
}

namespace cppinner {

template <std::size_t kDimension, typename Function, typename ...BufferArgs>
struct KernelMaker;

template <std::size_t kDimension, typename Function, typename ...BufferArgs>
struct KernelMaker<kDimension, Function, Kernel<kDimension, BufferArgs...>>
{
  static auto makeKernel(CpuDevice* d, Function func) noexcept
  {
    auto kernel = d->makeKernel<kDimension, Function, BufferArgs...>(func);
    return kernel;
  }

#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
  static auto makeKernel(VulkanDevice* d,
                  const uint32b module_index,
                  const std::string_view kernel_name) noexcept
  {
    auto kernel = d->makeKernel<kDimension, BufferArgs...>(module_index,
                                                           kernel_name);
    return kernel;
  }
#endif // ZINVUL_ENABLE_VULKAN_BACKEND
};

} // namespace cppinner

/*!
  */
template <std::size_t kDimension, typename SetType, typename ...ArgumentTypes>
inline
UniqueKernelWithoutLocal<kDimension, ArgumentTypes...> makeKernel(
    Device* device,
    const KernelSet<SetType> /* kernel_set */,
    void (*cpu_kernel)(ArgumentTypes...),
    const std::string_view vulkan_kernel) noexcept
{
  using UniqueKernel = UniqueKernelWithoutLocal<kDimension, ArgumentTypes...>;
  using Function = void (*)(ArgumentTypes...);
  using KernelMaker = cppinner::KernelMaker<kDimension,
                                            Function,
                                            typename UniqueKernel::value_type>;

  UniqueKernel kernel;
  switch (device->deviceType()) {
   case DeviceType::kCpu: {
    auto d = zisc::cast<CpuDevice*>(device);
    kernel = KernelMaker::makeKernel(d, cpu_kernel);
    break;
   }
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
   case DeviceType::kVulkan: {
    auto d = zisc::cast<VulkanDevice*>(device);
    constexpr uint32b module_index = SetType::getId();
    if (!d->hasShaderModule(module_index)) {
      const auto spirv_code = SetType::getKernelSpirvCode(d->workResource());
      d->setShaderModule(spirv_code, module_index);
    }
    kernel = KernelMaker::makeKernel(d, module_index, vulkan_kernel);
    break;
   }
#endif // ZINVUL_ENABLE_VULKAN_BACKEND
   default: {
    ZISC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return kernel;
}

#undef ZINVUL_MAKE_KERNEL_ARGS
#define ZINVUL_MAKE_KERNEL_ARGS(kernel_set, kernel) \
  zinvul:: kernel_set :: __KernelSet{}, \
  &zinvul::cl:: kernel_set :: kernel , \
  #kernel

} // namespace zinvul

#endif // ZINVUL_ZINVUL_INL_HPP
