/*!
  \file zinvul.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_HPP
#define ZINVUL_ZINVUL_HPP

// Zisc
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "cpu/cpu_buffer.hpp"
#include "cpu/cpu_device.hpp"
#include "cpu/cpu_kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

//! Make a buffer
template <typename Type>
UniqueBuffer<Type> makeBuffer(Device* device, const int usage_flags) noexcept;

//! Make a device
UniqueDevice makeDevice(DeviceOptions& options) noexcept;

//! Make a kernel
template <typename GroupType, typename ...ArgumentTypes>
UniqueKernel<GroupType, ArgumentTypes...> makeKernel(
    Device* device,
    const typename Kernel<GroupType, ArgumentTypes...>::KernelFunction func) noexcept;

//! Make a kernel
#define makeZinvulKernel(device, kernel_group, kernel) \
    inner::KernelFunction<decltype(&zinvul:: kernel_group ::KernelGroup:: kernel )>\
        ::make(device, &zinvul:: kernel_group ::KernelGroup:: kernel );

} // namespace zinvul

#include "zinvul-inl.hpp"

#endif // ZINVUL_ZINVUL_HPP
