/*!
  \file types-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPES_INL_CL
#define ZINVUL_TYPES_INL_CL

namespace zinvul {

namespace inner {

// Type aliases
#if defined(ZINVUL_CPU)
// Integer types
typedef ::zinvul::int8b int8b;
typedef ::zinvul::int16b int16b;
typedef ::zinvul::int32b int32b;
typedef ::zinvul::int64b int64b;
// Unsigned integer types
typedef ::zinvul::uint8b uint8b;
typedef ::zinvul::uint16b uint16b;
typedef ::zinvul::uint32b uint32b;
typedef ::zinvul::uint64b uint64b;
// Address space types
// Global
template <typename Type>
using Global = Type;
template <typename Type>
using GlobalPtr = AddressSpacePointer<Type, AddressSpaceType::kGlobal>;
template <typename Type>
using ConstGlobalPtr = AddressSpacePointer<const Type, AddressSpaceType::kGlobal>;
// Local
template <typename Type>
using Local = Type;
template <typename Type>
using LocalPtr = AddressSpacePointer<Type, AddressSpaceType::kLocal>;
template <typename Type>
using ConstLocalPtr = AddressSpacePointer<const Type, AddressSpaceType::kLocal>;
// Constant
template <typename Type>
using Constant = const Type;
template <typename Type>
using ConstantPtr = AddressSpacePointer<const Type, AddressSpaceType::kConstant>;
// Private
template <typename Type>
using Private = Type;
template <typename Type>
using PrivatePtr = AddressSpacePointer<Type, AddressSpaceType::kPrivate>;
template <typename Type>
using ConstPrivatePtr = AddressSpacePointer<const Type, AddressSpaceType::kPrivate>;
#else // Vulkan
// Integer types
typedef char int8b;
typedef short int16b;
typedef int int32b;
typedef long int64b;
// Unsigned integer types
typedef unsigned char uint8b;
typedef unsigned short uint16b;
typedef unsigned int uint32b;
typedef unsigned long uint64b;
// Address space types
// Global
template <typename Type>
using Global = __global Type;
template <typename Type>
using GlobalPtr = Global<Type>*;
template <typename Type>
using ConstGlobalPtr = const Global<Type>*;
// Local
template <typename Type>
using Local = __local Type;
template <typename Type>
using LocalPtr = Local<Type>*;
template <typename Type>
using ConstLocalPtr = const Local<Type>*;
// Constant
template <typename Type>
using Constant = __constant Type;
template <typename Type>
using ConstantPtr = Constant<Type>*;
// Private
template <typename Type>
using Private = __private Type;
template <typename Type>
using PrivatePtr = Private<Type>*;
template <typename Type>
using ConstPrivatePtr = const Private<Type>*;
#endif

} // namespace inner

} // namespace zinvul

#endif /* ZINVUL_TYPES_INL_CL */
