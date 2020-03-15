/*!
  \file zinvul.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_HPP
#define ZINVUL_ZINVUL_HPP

// Standard C++ library
#include <cstddef>
// Zinvul
#include "buffer.hpp"
#include "device.hpp"
#include "platform.hpp"
#include "platform_options.hpp"
//#include "kernel_arg_parser.hpp"
//#include "kernel_set.hpp"
#include "cpu/cpu_buffer.hpp"
#include "cpu/cpu_device.hpp"
//#include "cpu/cpu_kernel.hpp"
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
#include "vulkan/vulkan_buffer.hpp"
#include "vulkan/vulkan_device.hpp"
//#include "vulkan/vulkan_kernel.hpp"
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
#include "zinvul/zinvul_config.hpp"
//#include "zinvul/cppcl/types.hpp"
//#include "zinvul/cppcl/vector.hpp"

namespace zinvul {

//! Make a buffer
template <typename Type>
UniqueBuffer<Type> makeBuffer(Device* device, const BufferUsage flag);

////! Copy a src buffer to a dst buffer
//template <DescriptorType kDescriptor1, DescriptorType kDescriptor2, typename Type>
//void copy(const Buffer<kDescriptor1, Type>& src,
//          Buffer<kDescriptor2, Type>* dst,
//          const std::size_t count,
//          const std::size_t src_offset,
//          const std::size_t dst_offset,
//          const uint32b queue_index) noexcept;

//template <std::size_t kDimension, typename ...ArgumentTypes>
//using UniqueKernelWithoutLocal = zisc::UniqueMemoryPointer<
//    typename KernelArgParser<kDimension, ArgumentTypes...>::KernelWithoutLocal>;
//
////! Make a kernel
//template <std::size_t kDimension, typename SetType, typename ...ArgumentTypes>
//UniqueKernelWithoutLocal<kDimension, ArgumentTypes...>
//makeKernel(Device* device,
//           const KernelSet<SetType> kernel_set,
//           void (*cpu_kernel)(ArgumentTypes...),
//           const std::string_view vulkan_kernel) noexcept;

//! Create arguments of makeKernel
//#define ZINVUL_MAKE_KERNEL_ARGS(kernel_set, kernel)

} // namespace zinvul

#include "zinvul-inl.hpp"

#endif // ZINVUL_ZINVUL_HPP
