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
using GlobalPtr = AddressSpacePointer<AddressSpaceType::kGlobal, Type>;
template <typename Type>
using ConstGlobalPtr = AddressSpacePointer<AddressSpaceType::kGlobal, const Type>;
// Local
template <typename Type>
using Local = Type;
template <typename Type>
using LocalPtr = AddressSpacePointer<AddressSpaceType::kLocal, Type>;
template <typename Type>
using ConstLocalPtr = AddressSpacePointer<AddressSpaceType::kLocal, const Type>;
// Constant
template <typename Type>
using Constant = const Type;
template <typename Type>
using ConstantPtr = AddressSpacePointer<AddressSpaceType::kConstant, const Type>;
// Private
template <typename Type>
using Private = Type;
template <typename Type>
using PrivatePtr = 
    typename AddressSpacePointer<AddressSpaceType::kPrivate, Type>::Pointer;
template <typename Type>
using ConstPrivatePtr =
    typename AddressSpacePointer<AddressSpaceType::kPrivate, const Type>::ConstPointer;
// Generic
template <typename Type>
using Generic = Type;
template <typename Type>
using GenericPtr = PrivatePtr<Type>;
template <typename Type>
using ConstGenericPtr = ConstPrivatePtr<Type>;
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
// Generic
template <typename Type>
using Generic = __generic Type;
template <typename Type>
using GenericPtr = Generic<Type>*;
template <typename Type>
using ConstGenericPtr = const Generic<Type>*;
#endif

} // namespace inner

// Type size check
#define ZINVUL_TYPE_SIZE_CHECK_IMPL(type, size) \
  static_assert(sizeof( type ) == size , \
                "The size of " #type " is wrong."); \
  static_assert(alignof( type ) == size , \
                "The align of " #type " is wrong.")

ZINVUL_TYPE_SIZE_CHECK_IMPL(inner::int8b, 1);
ZINVUL_TYPE_SIZE_CHECK_IMPL(char2, 2);
ZINVUL_TYPE_SIZE_CHECK_IMPL(char3, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(char4, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(inner::uint8b, 1);
ZINVUL_TYPE_SIZE_CHECK_IMPL(uchar2, 2);
ZINVUL_TYPE_SIZE_CHECK_IMPL(uchar3, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(uchar4, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(inner::int16b, 2);
ZINVUL_TYPE_SIZE_CHECK_IMPL(short2, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(short3, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(short4, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(inner::uint16b, 2);
ZINVUL_TYPE_SIZE_CHECK_IMPL(ushort2, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(ushort3, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(ushort4, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(inner::int32b, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(int2, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(int3, 16);
ZINVUL_TYPE_SIZE_CHECK_IMPL(int4, 16);
ZINVUL_TYPE_SIZE_CHECK_IMPL(inner::uint32b, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(uint2, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(uint3, 16);
ZINVUL_TYPE_SIZE_CHECK_IMPL(uint4, 16);
ZINVUL_TYPE_SIZE_CHECK_IMPL(inner::int64b, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(long2, 16);
ZINVUL_TYPE_SIZE_CHECK_IMPL(long3, 32);
ZINVUL_TYPE_SIZE_CHECK_IMPL(long4, 32);
ZINVUL_TYPE_SIZE_CHECK_IMPL(half, 2);
ZINVUL_TYPE_SIZE_CHECK_IMPL(half2, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(half3, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(half4, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(float, 4);
ZINVUL_TYPE_SIZE_CHECK_IMPL(float2, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(float3, 16);
ZINVUL_TYPE_SIZE_CHECK_IMPL(float4, 16);
ZINVUL_TYPE_SIZE_CHECK_IMPL(double, 8);
ZINVUL_TYPE_SIZE_CHECK_IMPL(double2, 16);
ZINVUL_TYPE_SIZE_CHECK_IMPL(double3, 32);
ZINVUL_TYPE_SIZE_CHECK_IMPL(double4, 32);

static_assert(sizeof(inner::GlobalPtr<int>) == sizeof(void*),
              "The size of GlobalPtr is wrong.");
static_assert(sizeof(inner::ConstGlobalPtr<int>) == sizeof(const void*),
              "The size of ConstGlobalPtr is wrong.");
static_assert(sizeof(inner::LocalPtr<int>) == sizeof(void*),
              "The size of LocalPtr is wrong.");
static_assert(sizeof(inner::ConstLocalPtr<int>) == sizeof(const void*),
              "The size of LocalPtr is wrong.");
static_assert(sizeof(inner::ConstantPtr<int>) == sizeof(void*),
              "The size of ConstantPtr is wrong.");
static_assert(sizeof(inner::PrivatePtr<int>) == sizeof(void*),
              "The size of PrivatePtr is wrong.");
static_assert(sizeof(inner::ConstPrivatePtr<int>) == sizeof(const void*),
              "The size of PrivatePtr is wrong.");
static_assert(sizeof(inner::GenericPtr<int>) == sizeof(void*),
              "The size of GenericPtr is wrong.");
static_assert(sizeof(inner::ConstGenericPtr<int>) == sizeof(const void*),
              "The size of ConstGenericPtr is wrong.");

} // namespace zinvul

#endif /* ZINVUL_TYPES_INL_CL */
