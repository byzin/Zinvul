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
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
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
template <typename GroupType, typename ...ArgumentTypes> inline
CpuKernel<GroupType, ArgumentTypes...>::CpuKernel(CpuDevice* device,
                                                  const KernelFunction kernel_func)
    noexcept :
        device_{device}
{
  setKernel(kernel_func);
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> inline
CpuDevice* CpuKernel<GroupType, ArgumentTypes...>::device() noexcept
{
  ZISC_ASSERT(device_ != nullptr, "The assigned device is null.");
  return device_;
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> inline
const CpuDevice* CpuKernel<GroupType, ArgumentTypes...>::device() const noexcept
{
  ZISC_ASSERT(device_ != nullptr, "The assigned device is null.");
  return device_;
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> inline
DeviceType CpuKernel<GroupType, ArgumentTypes...>::deviceType() const noexcept
{
  return DeviceType::kCpu;
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> inline
auto CpuKernel<GroupType, ArgumentTypes...>::kernel() const noexcept
    -> KernelFunction
{
  return kernel_;
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> inline
void CpuKernel<GroupType, ArgumentTypes...>::run(
    BufferRef<ArgumentTypes>... args,
    const uint32b work_x_size,
    const uint32b work_y_size,
    const uint32b work_z_size) noexcept
{
  ZISC_ASSERT(kernel_ != nullptr, "The kernel function is null.");
  ZISC_ASSERT(0 < work_x_size, "The work size x is zero.");
  ZISC_ASSERT(0 < work_y_size, "The work size y is zero.");
  ZISC_ASSERT(0 < work_z_size, "The work size z is zero.");
  KernelGroupType instance;
  for (uint32b z = 0; z < work_z_size; ++z) {
    for (uint32b y = 0; y < work_y_size; ++y) {
      for (uint32b x = 0; x < work_x_size; ++x) {
        instance.setGlobalWorkId(x, y, z);
        (instance.*kernel_)(refer<ArgumentTypes>(args)...);
      }
    }
  }
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> inline
void CpuKernel<GroupType, ArgumentTypes...>::setKernel(
    const KernelFunction kernel_func) noexcept
{
  kernel_ = kernel_func;
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> template <typename Type> 
inline
Type CpuKernel<GroupType, ArgumentTypes...>::refer(BufferRef<Type> buffer)
    const noexcept
{
  ZISC_ASSERT(buffer.deviceType() == DeviceType::kCpu,
              "The device type of the buffer isn't cpu.");
  ZISC_ASSERT(0 < buffer.size(), "The buffer is empty.");
  using CpuBufferP = std::add_pointer_t<CpuBuffer<std::remove_cv_t<std::remove_pointer_t<Type>>>>;
  auto cpu_buffer = zisc::cast<CpuBufferP>(&buffer);
  if constexpr (std::is_pointer_v<Type>)
    return cpu_buffer->data();
  else
    return *(cpu_buffer->data());
}

} // namespace zinvul

#endif // ZINVUL_CPU_KERNEL_INL_HPP
