/*!
  \file kernel_arg_parser-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_ARG_PARSER_INL_HPP
#define ZINVUL_KERNEL_ARG_PARSER_INL_HPP

#include "kernel_arg_parser.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/zisc_config.hpp"
// Zinvul
#include "buffer.hpp"
#include "kernel.hpp"
#include "cppcl/address_space_pointer.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \brief Global or Local type info
  */
template <cl::AddressSpaceType kAddressType, typename Type>
class AddressSpaceInfo<cl::AddressSpacePointer<kAddressType, Type>>
{
 public:
  using ElementType = std::remove_cv_t<Type>;

  static constexpr bool kIsGlobal = (kAddressType == cl::AddressSpaceType::kGlobal);
  static constexpr bool kIsLocal = (kAddressType == cl::AddressSpaceType::kLocal);
  static constexpr bool kIsPod = false;
  static constexpr bool kIsConst = std::is_const_v<Type>;

  static_assert(kIsGlobal || kIsLocal, "The address space is wrong.");

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The element type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The element type is reference.");
};

/*!
  */
template <std::size_t kDimension, typename T, typename ...ArgumentTypes>
class KernelArgParser<kDimension, T, ArgumentTypes...>
{
  static_assert(!std::is_pointer_v<T> &&
                (!std::is_pointer_v<ArgumentTypes> && ...),
                "The kernel has any pointer argument.");
  static_assert(!std::is_reference_v<T> &&
                (!std::is_reference_v<ArgumentTypes> && ...),
                "The kernel has any reference argument.");

  using Parent = KernelArgParser<kDimension, ArgumentTypes...>;
  using ArgInfo = KernelArgInfo<T>;

  template <typename ...Types> struct ExtendedKernel;

  template <typename ...Types>
  struct ExtendedKernel<Kernel<kDimension, Types...>>
  {
    using BufferArg = Buffer<ArgInfo::kBufferType, typename ArgInfo::ElementType>;
    using KernelType = Kernel<kDimension, BufferArg, Types...>;
  };

 public:
  using KernelWithoutLocal = std::conditional_t<ArgInfo::kIsLocal,
      typename Parent::KernelWithoutLocal,
      typename ExtendedKernel<typename Parent::KernelWithoutLocal>::KernelType>;
};

} // namespace zinvul

#endif // ZINVUL_KERNEL_ARG_PARSER_INL_HPP
