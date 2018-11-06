/*!
  \file cpu_device-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_DEVICE_INL_HPP
#define ZINVUL_CPU_DEVICE_INL_HPP

#include "cpu_device.hpp"
// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/error.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "cpu_buffer.hpp"
#include "cpu_kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
inline
CpuDevice::CpuDevice(DeviceOptions& options) noexcept :
    Device(options)
{
}

/*!
  */
inline
DeviceType CpuDevice::deviceType() const noexcept
{
  return DeviceType::kCpu;
}

/*!
  */
template <typename Type> inline
UniqueBuffer<Type> CpuDevice::makeBuffer(const BufferUsage usage_flag) noexcept
{
  using UniqueCpuBuffer = zisc::UniqueMemoryPointer<CpuBuffer<Type>>;
  UniqueBuffer<Type> buffer =
      UniqueCpuBuffer::make(memoryResource(), this, usage_flag);
  return buffer;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
UniqueKernel<GroupType, kDimension, ArgumentTypes...> CpuDevice::makeKernel(
    const typename Kernel<GroupType, kDimension, ArgumentTypes...>::KernelFunction func) noexcept
{
  using UniqueCpuKernel = zisc::UniqueMemoryPointer<CpuKernel<GroupType,
                                                              kDimension,
                                                              ArgumentTypes...>>;
  UniqueKernel<GroupType, kDimension, ArgumentTypes...> kernel =
      UniqueCpuKernel::make(memoryResource(), this, func);
  return kernel;
}

/*!
  */
inline
void CpuDevice::waitForCompletion() noexcept
{
}

} // namespace zinvul

#endif // ZINVUL_CPU_DEVICE_INL_HPP
