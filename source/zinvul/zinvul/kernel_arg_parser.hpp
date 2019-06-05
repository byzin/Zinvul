/*!
  \file kernel_arg_parser.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_ARG_PARSER_HPP
#define ZINVUL_KERNEL_ARG_PARSER_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Zinvul
#include "kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \brief POD type info
  */
template <typename Type>
class AddressSpaceInfo
{
 public:
  using ElementType = std::remove_cv_t<Type>;

  static constexpr bool kIsGlobal = true;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsPod = true;
  static constexpr bool kIsConst = std::is_const_v<Type>;

 private:
  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");
};

/*!
  \brief Type information of a kernel arg
  */
template <typename Type>
class KernelArgInfo
{
  using ASpaceInfo = AddressSpaceInfo<std::remove_cv_t<Type>>;

 public:
  using ElementType = typename ASpaceInfo::ElementType;

  static constexpr bool kIsGlobal = ASpaceInfo::kIsGlobal;
  static constexpr bool kIsLocal = ASpaceInfo::kIsLocal;
  static constexpr bool kIsPod = ASpaceInfo::kIsPod;
  static constexpr bool kIsConst = std::is_const_v<Type> || ASpaceInfo::kIsConst;
  static constexpr BufferType kBufferType = (kIsPod || kIsConst)
      ? BufferType::kUniform
      : BufferType::kStorage;

 private:
  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");
  static_assert(!(!kIsPod && std::is_const_v<Type>),
                "The address space pointer is const-qualified.");
};

/*!
  \brief Type information of kernel arguments
  */
template <std::size_t kDimension, typename ...ArgumentTypes>
class KernelArgParser
{
  static_assert((!std::is_pointer_v<ArgumentTypes> && ...),
                "The kernel has any pointer argument.");
  static_assert((!std::is_reference_v<ArgumentTypes> && ...),
                "The kernel has any reference argument.");

 public:
  using KernelWithoutLocal = Kernel<kDimension>;
};

} // namespace zinvul

#include "kernel_arg_parser-inl.hpp"

#endif // ZINVUL_KERNEL_ARG_PARSER_HPP
