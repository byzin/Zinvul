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
#include "kernel_group.hpp"
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

namespace cppinner {

template <typename ...Types> class KernelFunction;

template <typename GroupType, typename ...ArgumentTypes>
class KernelFunction<GroupType, void (*)(ArgumentTypes...)>
{
  static_assert(std::is_base_of_v<KernelGroup<GroupType>, GroupType>,
                "The GroupType isn't derived from zisc::KernelGroup.");
  using Function = void (*)(ArgumentTypes...);

 public:
  template <std::size_t kDimension>
  static auto make(Device* device,
                   Function kernel_func,
                   const std::string_view kernel_name) noexcept
  {
    UniqueKernel<kDimension, ArgumentTypes...> kernel;
    switch (device->deviceType()) {
     case DeviceType::kCpu: {
      auto d = zisc::cast<CpuDevice*>(device);
      kernel = d->makeKernel<kDimension, ArgumentTypes...>(kernel_func);
      break;
     }
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
     case DeviceType::kVulkan: {
      auto d = zisc::cast<VulkanDevice*>(device);
      constexpr uint32b module_index = GroupType::getId();
      if (!d->hasShaderModule(module_index)) {
        const auto spirv_code = GroupType::getKernelSpirvCode(d->workResource());
        d->setShaderModule(spirv_code, module_index);
      }
      kernel = d->makeKernel<kDimension, ArgumentTypes...>(module_index,
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

} // namespace cppinner

/*!
  */
#undef makeZinvulKernel
#define makeZinvulKernel(device, kernel_group, kernel, dimension) \
    cppinner::KernelFunction<zinvul:: kernel_group ::__KernelGroup, \
                             decltype(&zinvul::cl:: kernel )> \
        ::make< dimension >(device, &zinvul::cl:: kernel , #kernel )

} // namespace zinvul

#endif // ZINVUL_ZINVUL_INL_HPP
