/*!
  \file kernel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_HPP
#define ZINVUL_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <std::size_t kDimension, typename ...BufferArgs>
class Kernel : private zisc::NonCopyable<Kernel<kDimension, BufferArgs...>>
{
  static_assert(zisc::Algorithm::isInBounds(kDimension, 1u, 4u),
                "The kDimension should be 1, 2 or 3.");

 public:
  //!
  virtual ~Kernel() noexcept;


  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Execute a kernel
  virtual void run(std::add_lvalue_reference_t<BufferArgs>... args,
                   const std::array<uint32b, kDimension> works,
                   const uint32b queue_index) noexcept = 0;

  //! Return the number of a kernel arguments
  static constexpr std::size_t numOfArguments() noexcept;

  //! Return the workgroup dimension
  static constexpr std::size_t workgroupDimension() noexcept;
};

// Type aliases
template <std::size_t kDimension, typename ...BufferArgs>
using UniqueKernel = zisc::UniqueMemoryPointer<Kernel<kDimension, BufferArgs...>>;

} // namespace zinvul

#include "kernel-inl.hpp"

#endif // ZINVUL_KERNEL_HPP
