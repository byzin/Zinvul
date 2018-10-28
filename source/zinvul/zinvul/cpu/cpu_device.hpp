/*!
  \file cpu_device.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_DEVICE_HPP
#define ZINVUL_CPU_DEVICE_HPP

// Zisc
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
class CpuDevice : public Device
{
 public:
  //! Initialize a cpu device
  CpuDevice(DeviceOptions& options) noexcept;


  //! Return cpu type
  DeviceType deviceType() const noexcept override;

  //! Make a buffer
  template <typename Type>
  UniqueBuffer<Type> makeBuffer(const int usage_flags) noexcept;

  //! Make a kernel
  template <typename GroupType, typename ...ArgumentTypes>
  UniqueKernel<GroupType, ArgumentTypes...> makeKernel(
      const typename Kernel<GroupType, ArgumentTypes...>::KernelFunction func) noexcept;
};

} // namespace zinvul

#include "cpu_device-inl.hpp"

#endif // ZINVUL_CPU_DEVICE_HPP
