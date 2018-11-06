/*!
  \file cpu_device.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_DEVICE_HPP
#define ZINVUL_CPU_DEVICE_HPP

// Standard C++ library
#include <cstddef>
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
  UniqueBuffer<Type> makeBuffer(const BufferUsage usage_flag) noexcept;

  //! Make a kernel
  template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
  UniqueKernel<GroupType, kDimension, ArgumentTypes...> makeKernel(
      const typename Kernel<GroupType, kDimension, ArgumentTypes...>::KernelFunction func) noexcept;

  //! Wait this thread until all commands in the queue are completed
  void waitForCompletion() noexcept override;
};

} // namespace zinvul

#include "cpu_device-inl.hpp"

#endif // ZINVUL_CPU_DEVICE_HPP
