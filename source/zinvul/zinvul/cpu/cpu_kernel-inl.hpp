/*!
  \file cpu_kernel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_KERNEL_INL_HPP
#define ZINVUL_CPU_KERNEL_INL_HPP

#include "cpu_kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/function_reference.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_buffer.hpp"
#include "cpu_device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/kernel_group.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
CpuKernel<GroupType, kDimension, ArgumentTypes...>::CpuKernel(
    CpuDevice* device,
    const KernelFunction kernel_func) noexcept :
        device_{device}
{
  setKernel(kernel_func);
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
CpuDevice* CpuKernel<GroupType, kDimension, ArgumentTypes...>::device() noexcept
{
  ZISC_ASSERT(device_ != nullptr, "The assigned device is null.");
  return device_;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
const CpuDevice* CpuKernel<GroupType, kDimension, ArgumentTypes...>::device()
    const noexcept
{
  ZISC_ASSERT(device_ != nullptr, "The assigned device is null.");
  return device_;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
DeviceType CpuKernel<GroupType, kDimension, ArgumentTypes...>::deviceType()
    const noexcept
{
  return DeviceType::kCpu;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
auto CpuKernel<GroupType, kDimension, ArgumentTypes...>::kernel() const noexcept
    -> KernelFunction
{
  ZISC_ASSERT(kernel_ != nullptr, "The kernel function is null.");
  return kernel_;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
bool CpuKernel<GroupType, kDimension, ArgumentTypes...>::hasKernel() const noexcept
{
  const bool result = kernel_ != nullptr;
  return result;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
void CpuKernel<GroupType, kDimension, ArgumentTypes...>::setKernel(
    const KernelFunction kernel_func) noexcept
{
  kernel_ = kernel_func;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
template <typename Type> inline
Type CpuKernel<GroupType, kDimension, ArgumentTypes...>::refer(
    BufferRef<Type> buffer) const noexcept
{
  ZISC_ASSERT(buffer.deviceType() == DeviceType::kCpu,
              "The device type of the buffer isn't cpu.");
  ZISC_ASSERT(0 < buffer.size(), "The buffer is empty.");
  using CpuBufferP = std::add_pointer_t<CpuBuffer<std::remove_pointer_t<Type>>>;
  auto cpu_buffer = zisc::cast<CpuBufferP>(&buffer);
  if constexpr (std::is_pointer_v<Type>)
    return cpu_buffer->data();
  else
    return *(cpu_buffer->data());
}

} // namespace zinvul

#endif // ZINVUL_CPU_KERNEL_INL_HPP
