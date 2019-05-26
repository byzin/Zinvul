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
#include "kernel_set.hpp"
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
template <typename Type> inline
UniqueBuffer<Type> makeBuffer(Device* device,
                              const BufferUsage usage_flag) noexcept
{
  UniqueBuffer<Type> buffer;
  switch (device->deviceType()) {
   case DeviceType::kCpu: {
    auto d = zisc::cast<CpuDevice*>(device);
    buffer = d->makeBuffer<Type>(usage_flag);
    break;
   }
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
   case DeviceType::kVulkan: {
    auto d = zisc::cast<VulkanDevice*>(device);
    buffer = d->makeBuffer<Type>(usage_flag);
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

/*!
  */
template <std::size_t kDimension, typename SetType, typename ...ArgumentTypes>
inline
UniqueKernel<kDimension, ArgumentTypes...> makeKernel(
    Device* device,
    const KernelSet<SetType> /* kernel_set */,
    void (*cpu_kernel)(ArgumentTypes...),
    const std::string_view vulkan_kernel) noexcept
{
  UniqueKernel<kDimension, ArgumentTypes...> kernel;
  switch (device->deviceType()) {
   case DeviceType::kCpu: {
    auto d = zisc::cast<CpuDevice*>(device);
    kernel = d->makeKernel<kDimension, ArgumentTypes...>(cpu_kernel);
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
    kernel = d->makeKernel<kDimension, ArgumentTypes...>(module_index,
                                                         vulkan_kernel);
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
