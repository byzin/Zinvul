/*!
  \file zinvul-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_INL_HPP
#define ZINVUL_ZINVUL_INL_HPP

#include "zinvul.hpp"
// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Zinvul
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

namespace inner {

template <typename, typename ...ArgumentTypes> struct KernelFunction;

template <typename GroupType, typename ...ArgumentTypes>
struct KernelFunction<void (GroupType::*)(ArgumentTypes...)>
{
  template <std::size_t kDimension>
  using Function = typename Kernel<GroupType, kDimension, ArgumentTypes...>::KernelFunction;

  template <std::size_t kDimension>
  static auto make(Device* device,
                   const Function<kDimension> kernel_func,
                   const char* kernel_name) noexcept
  {
    UniqueKernel<GroupType, kDimension, ArgumentTypes...> kernel;
    switch (device->deviceType()) {
     case DeviceType::kCpu: {
      auto d = zisc::cast<CpuDevice*>(device);
      kernel = d->makeKernel<GroupType, kDimension, ArgumentTypes...>(kernel_func);
      break;
     }
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
     case DeviceType::kVulkan: {
      auto d = zisc::cast<VulkanDevice*>(device);
      const uint32b module_index = GroupType{}.__getKernelGroupNumber__();
      if (!d->hasShaderModule(module_index)) {
        const auto spirv_code =
            GroupType{}.__getKernelSpirvCode__(d->workResource());
        d->setShaderModule(spirv_code, module_index);
      }
      kernel = d->makeKernel<GroupType, kDimension, ArgumentTypes...>(module_index,
                                                                      kernel_name);
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
};

} // namespace inner

/*!
  */
#undef makeZinvulKernel
#define makeZinvulKernel(device, kernel_group, kernel, dimension) \
    inner::KernelFunction<decltype(&zinvul:: kernel_group ::KernelGroup:: kernel )>\
        ::make< dimension >(device, \
                            &zinvul:: kernel_group ::KernelGroup:: kernel , \
                            #kernel )

} // namespace zinvul

#endif // ZINVUL_ZINVUL_INL_HPP
