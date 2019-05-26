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
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"
#include "zinvul/cppcl/address_space_pointer.hpp"

namespace zinvul {

// Forward declaration
template <typename> class Buffer;

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes>
class Kernel : private zisc::NonCopyable<Kernel<kDimension, ArgumentTypes...>>
{
  static_assert(zisc::isInBounds(kDimension, 1u, 4u),
                "The kDimension should be 1, 2 or 3.");

  template <typename T, cl::AddressSpaceType kASpaceType = cl::AddressSpaceType::kPrivate>
  struct ArgType
  {
    using Type = std::remove_pointer_t<std::remove_cv_t<T>>;
    static constexpr cl::AddressSpaceType kAddressSpaceType = kASpaceType;
    static constexpr bool kIsPointer = std::is_pointer_v<T>;
  };

  template <typename T, cl::AddressSpaceType kASpaceType>
  struct ArgType<cl::AddressSpacePointer<T, kASpaceType>>
  {
    using Type = std::remove_cv_t<T>;
    static constexpr cl::AddressSpaceType kAddressSpaceType = kASpaceType;
    static constexpr bool kIsPointer = true;
  };

 protected:
  template <typename Type>
  using ClArgType = ArgType<Type>;
  template <typename Type>
  using BufferRef = std::add_lvalue_reference_t<Buffer<typename ClArgType<Type>::Type>>;

 public:
  using Function = void (*)(ArgumentTypes...);


  //!
  virtual ~Kernel() noexcept;


  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Execute a kernel
  virtual void run(BufferRef<ArgumentTypes>... args,
                   const std::array<uint32b, kDimension> works,
                   const uint32b queue_index) noexcept = 0;

  //! Return the number of a kernel arguments
  static constexpr std::size_t numOfArguments() noexcept;

  //! Return the workgroup dimension
  static constexpr std::size_t workgroupDimension() noexcept;
};

// Type aliases
template <std::size_t kDimension, typename ...ArgumentTypes>
using UniqueKernel = zisc::UniqueMemoryPointer<Kernel<kDimension, ArgumentTypes...>>;

} // namespace zinvul

#include "kernel-inl.hpp"

#endif // ZINVUL_KERNEL_HPP
