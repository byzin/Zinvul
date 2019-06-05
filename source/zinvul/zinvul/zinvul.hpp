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
#include <string_view>
// Zisc
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "kernel_arg_parser.hpp"
#include "kernel_set.hpp"
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
template <BufferType kBufferType, typename Type>
UniqueBuffer<kBufferType, Type> makeBuffer(
    Device* device,
    const BufferUsage usage_flag) noexcept;

//! Make a uniform buffer
template <typename Type>
UniqueBuffer<BufferType::kUniform, Type> makeUniformBuffer(
    Device* device,
    const BufferUsage usage_flag) noexcept;

//! Make a storage buffer
template <typename Type>
UniqueBuffer<BufferType::kStorage, Type> makeStorageBuffer(
    Device* device,
    const BufferUsage usage_flag) noexcept;

//! Make a device
UniqueDevice makeDevice(DeviceOptions& options) noexcept;

template <std::size_t kDimension, typename ...ArgumentTypes>
using UniqueKernelWithoutLocal = zisc::UniqueMemoryPointer<
    typename KernelArgParser<kDimension, ArgumentTypes...>::KernelWithoutLocal>;

//! Make a kernel
template <std::size_t kDimension, typename SetType, typename ...ArgumentTypes>
UniqueKernelWithoutLocal<kDimension, ArgumentTypes...>
makeKernel(Device* device,
           const KernelSet<SetType> kernel_set,
           void (*cpu_kernel)(ArgumentTypes...),
           const std::string_view vulkan_kernel) noexcept;

//! Create arguments of makeKernel
#define ZINVUL_MAKE_KERNEL_ARGS(kernel_set, kernel)

} // namespace zinvul

#include "zinvul-inl.hpp"

#endif // ZINVUL_ZINVUL_HPP
