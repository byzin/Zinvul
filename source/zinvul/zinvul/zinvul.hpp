/*!
  \file zinvul.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_HPP
#define ZINVUL_ZINVUL_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/unique_memory_pointer.hpp"
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
#include "zinvul/cppcl/types.hpp"
#include "zinvul/cppcl/vector.hpp"

namespace zinvul {

//! Make a buffer
template <typename Type>
UniqueBuffer<Type> makeBuffer(Device* device,
                              const BufferUsage usage_flag) noexcept;

//! Make a device
UniqueDevice makeDevice(DeviceOptions& options) noexcept;

//! Make a kernel
#define makeZinvulKernel(device, kernel_group, kernel, dimension)

} // namespace zinvul

#include "zinvul-inl.hpp"

#endif // ZINVUL_ZINVUL_HPP
