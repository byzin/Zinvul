/*!
  \file types.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPES_CL
#define ZINVUL_TYPES_CL

#include "types-inl.cl"

namespace zinvul {

// Type aliases
// Integer types
typedef inner::int8b int8b;
typedef inner::int16b int16b;
typedef inner::int32b int32b;
typedef inner::int64b int64b;
// Unsigned integer types
typedef inner::uint8b uint8b;
typedef inner::uint16b uint16b;
typedef inner::uint32b uint32b;
typedef inner::uint64b uint64b;
// Address space types
// Global
template <typename Type>
using Global = inner::Global<Type>;
template <typename Type>
using GlobalPtr = inner::GlobalPtr<Type>;
template <typename Type>
using ConstGlobalPtr = inner::ConstGlobalPtr<Type>;
// Local
template <typename Type>
using Local = inner::Local<Type>;
template <typename Type>
using LocalPtr = inner::LocalPtr<Type>;
template <typename Type>
using ConstLocalPtr = inner::ConstLocalPtr<Type>;
// Constant
template <typename Type>
using Constant = inner::Constant<Type>;
template <typename Type>
using ConstantPtr = inner::ConstantPtr<Type>;
// Private
template <typename Type>
using Private = inner::Private<Type>;
template <typename Type>
using PrivatePtr = inner::PrivatePtr<Type>;
template <typename Type>
using ConstPrivatePtr = inner::ConstPrivatePtr<Type>;

// Type size check
static_assert(sizeof(int8b) == 1, "The size of int8b is wrong.");
static_assert(sizeof(int16b) == 2, "The size of int16b is wrong.");
static_assert(sizeof(int32b) == 4, "The size of int32b is wrong.");
static_assert(sizeof(int64b) == 8, "The size of int64b is wrong.");
static_assert(sizeof(uint8b) == 1, "The size of uint8b is wrong.");
static_assert(sizeof(uint16b) == 2, "The size of uint16b is wrong.");
static_assert(sizeof(uint32b) == 4, "The size of uint32b is wrong.");
static_assert(sizeof(uint64b) == 8, "The size of uint64b is wrong.");
static_assert(sizeof(half) == 2, "The size of half is wrong.");
static_assert(sizeof(float) == 4, "The size of float is wrong.");
static_assert(sizeof(double) == 8, "The size of double is wrong.");
static_assert(sizeof(GlobalPtr<int>) == sizeof(void*),
              "The size of GlobalPtr is wrong.");
static_assert(sizeof(ConstGlobalPtr<int>) == sizeof(const void*),
              "The size of ConstGlobalPtr is wrong.");
static_assert(sizeof(LocalPtr<int>) == sizeof(void*),
              "The size of LocalPtr is wrong.");
static_assert(sizeof(ConstLocalPtr<int>) == sizeof(const void*),
              "The size of LocalPtr is wrong.");
static_assert(sizeof(ConstantPtr<int>) == sizeof(void*),
              "The size of ConstantPtr is wrong.");
static_assert(sizeof(PrivatePtr<int>) == sizeof(void*),
              "The size of PrivatePtr is wrong.");
static_assert(sizeof(ConstPrivatePtr<int>) == sizeof(const void*),
              "The size of PrivatePtr is wrong.");

} // namespace zinvul

#endif /* ZINVUL_TYPES_CL */
