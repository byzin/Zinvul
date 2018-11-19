/*!
  \file kernel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "kernel_group.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
template <typename> class Buffer;

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
class Kernel : private zisc::NonCopyable<Kernel<GroupType, kDimension, ArgumentTypes...>>
{
  static_assert(std::is_base_of_v<KernelGroup, GroupType>,
                "The GroupType isn't derived from zinvul::KernelGroup.");
  static_assert(std::is_default_constructible_v<GroupType>,
                "The GroupType isn't default constructable.");
  static_assert(zisc::isInBounds(kDimension, 1u, 4u),
                "The kDimension should be 1, 2 or 3.");

 protected:
  template <typename Type>
  using BufferRef = std::add_lvalue_reference_t<Buffer<std::remove_cv_t<std::remove_pointer_t<Type>>>>;

 public:
  using KernelGroupType = GroupType;
  using KernelFunction = void (KernelGroupType::*)(ArgumentTypes...);


  //!
  virtual ~Kernel() noexcept;


  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Execute a kernel
  virtual void run(BufferRef<ArgumentTypes>... args,
                   const std::array<uint32b, kDimension> works,
                   const uint32b queue_index = 0) noexcept = 0;

  //! Return the number of a kernel arguments
  static constexpr std::size_t numOfArguments() noexcept;

  //! Return the workgroup dimension
  static constexpr std::size_t workgroupDimension() noexcept;

 private:
};

// Type aliases
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
using UniqueKernel = zisc::UniqueMemoryPointer<Kernel<GroupType, kDimension, ArgumentTypes...>>;

} // namespace zinvul

#include "kernel-inl.hpp"

#endif // ZINVUL_KERNEL_HPP
