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
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu/cpu_buffer.hpp"
#include "cpu/cpu_device.hpp"
#include "cpu/cpu_kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename Type> inline
UniqueBuffer<Type> makeBuffer(Device* device, const int usage_flags) noexcept
{
  UniqueBuffer<Type> buffer;
  switch (device->deviceType()) {
   case DeviceType::kCpu: {
    auto d = zisc::cast<CpuDevice*>(device);
    buffer = d->makeBuffer<Type>(usage_flags);
    break;
   }
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
   default: {
    ZISC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return device;
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes>
UniqueKernel<GroupType, ArgumentTypes...> makeKernel(
    Device* device,
    const typename Kernel<GroupType, ArgumentTypes...>::KernelFunction func) noexcept
{
  UniqueKernel<GroupType, ArgumentTypes...> kernel;
  switch (device->deviceType()) {
   case DeviceType::kCpu: {
    auto d = zisc::cast<CpuDevice*>(device);
    kernel = d->makeKernel<GroupType, ArgumentTypes...>(func);
    break;
   }
   default: {
    ZISC_ASSERT(false, "Error: Unsupported device type is specified.");
    break;
   }
  }
  return kernel;
}

namespace inner {

template <typename GroupType, typename ...ArgumentTypes> struct KernelFunction;
template <typename GroupType, typename ...ArgumentTypes>
struct KernelFunction<void (GroupType::*)(ArgumentTypes...)>
{
  using Function = typename Kernel<GroupType, ArgumentTypes...>::KernelFunction;
  static UniqueKernel<GroupType, ArgumentTypes...> make(
      Device* device,
      const Function func) noexcept
  {
    return makeKernel<GroupType, ArgumentTypes...>(device, func);
  }
};

} // namespace inner

} // namespace zinvul

#endif // ZINVUL_ZINVUL_INL_HPP
