/*!
  \file type_traits-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPE_TRAITS_INL_CL
#define ZINVUL_TYPE_TRAITS_INL_CL

#include "type_traits.cl"
// Zinvul
#include "types.cl"

namespace zinvul {

#define ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(VecType, ElemType, n) \
  template <> \
  struct VectorType< VecType > \
  { \
    using ElementType = ElemType; \
    static constexpr size_t size() noexcept {return n;} \
    static_assert(n <= sizeof( VecType ) / sizeof( ElemType ), \
                  "The number of elements is wrong."); \
  }

ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int8b, int8b, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char2, int8b, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char3, int8b, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(char4, int8b, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int16b, int16b, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short2, int16b, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short3, int16b, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(short4, int16b, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int32b, int32b, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int2, int32b, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int3, int32b, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int4, int32b, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(int64b, int64b, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long2, int64b, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long3, int64b, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(long4, int64b, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint8b, uint8b, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar2, uint8b, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar3, uint8b, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uchar4, uint8b, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint16b, uint16b, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort2, uint16b, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort3, uint16b, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ushort4, uint16b, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint32b, uint32b, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint2, uint32b, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint3, uint32b, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint4, uint32b, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(uint64b, uint64b, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong2, uint64b, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong3, uint64b, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(ulong4, uint64b, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half, half, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half2, half, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half3, half, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(half4, half, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float, float, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float2, float, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float3, float, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(float4, float, 4);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double, double, 1);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double2, double, 2);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double3, double, 3);
ZINVUL_VECTOR_TYPE_TEMPLATE_SPECIALIZATION_IMPL(double4, double, 4);

#define ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(T, ElemT, n) \
  template <> \
  struct IntegerVector< sizeof( ElemT ), n > \
  { \
    using Type = T; \
    using ElementType = ElemT; \
    static constexpr size_t size() noexcept {return n ;} \
  }

ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(int8b, int8b, 1);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(char2, int8b, 2);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(char3, int8b, 3);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(char4, int8b, 4);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(int16b, int16b, 1);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(short2, int16b, 2);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(short3, int16b, 3);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(short4, int16b, 4);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(int32b, int32b, 1);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(int2, int32b, 2);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(int3, int32b, 3);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(int4, int32b, 4);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(int64b, int64b, 1);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(long2, int64b, 2);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(long3, int64b, 3);
ZINVUL_INTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(long4, int64b, 4);

#define ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(T, ElemT, n) \
  template <> \
  struct UIntegerVector< sizeof( ElemT ), n > \
  { \
    using Type = T; \
    using ElementType = ElemT; \
    static constexpr size_t size() noexcept {return n ;} \
  }

ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uint8b, uint8b, 1);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uchar2, uint8b, 2);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uchar3, uint8b, 3);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uchar4, uint8b, 4);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uint16b, uint16b, 1);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(ushort2, uint16b, 2);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(ushort3, uint16b, 3);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(ushort4, uint16b, 4);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uint32b, uint32b, 1);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uint2, uint32b, 2);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uint3, uint32b, 3);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uint4, uint32b, 4);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(uint64b, uint64b, 1);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(ulong2, uint64b, 2);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(ulong3, uint64b, 3);
ZINVUL_UINTEGER_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(ulong4, uint64b, 4);

#define ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(T, ElemT, n) \
  template <> \
  struct FloatVector< sizeof( ElemT ), n > \
  { \
    using fType = T; \
    using ElementType = ElemT; \
    static constexpr size_t size() noexcept {return n ;} \
  }

ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(half, half, 1);
ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(half2, half, 2);
ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(half3, half, 3);
ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(half4, half, 4);
ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(float, float, 1);
ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(float2, float, 2);
ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(float3, float, 3);
ZINVUL_FLOAT_VECTOR_TEMPLATE_SPECIALIZATION_IMPL(float4, float, 4);

template <typename T>
struct IsSignedInteger
{
  static constexpr int32b kValue = kScalarResultFalse;
};

#define ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsSignedInteger< Type > \
  { \
    static constexpr int32b kValue = kScalarResultTrue; \
  }

ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int8b);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(char2);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(char3);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(char4);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int16b);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(short2);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(short3);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(short4);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int32b);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int2);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int3);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int4);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(int64b);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(long2);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(long3);
ZINVUL_IS_SIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(long4);

template <typename T>
struct IsUnsignedInteger
{
  static constexpr int32b kValue = kScalarResultFalse;
};

#define ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsUnsignedInteger< Type > \
  { \
    static constexpr int32b kValue = kScalarResultTrue; \
  }

ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint8b);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uchar2);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uchar3);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uchar4);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint16b);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ushort2);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ushort3);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ushort4);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint32b);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint2);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint3);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint4);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(uint64b);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ulong2);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ulong3);
ZINVUL_IS_UNSIGNED_INTEGER_TEMPLATE_SPECIALIZATION_IMPL(ulong4);

template <typename T>
struct IsInteger
{
  static constexpr int32b kValue = kIsSignedInteger<T> | kIsUnsignedInteger<T>;
};

template <typename T>
struct IsHalf
{
  static constexpr int32b kValue = kScalarResultFalse;
};

#define ZINVUL_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsHalf< Type > \
  { \
    static constexpr int32b kValue = kScalarResultTrue; \
  }

ZINVUL_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half);
ZINVUL_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half2);
ZINVUL_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half3);
ZINVUL_IS_HALF_TEMPLATE_SPECIALIZATION_IMPL(half4);

template <typename T>
struct IsSingleFloat
{
  static constexpr int32b kValue = kScalarResultFalse;
};

#define ZINVUL_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsSingleFloat< Type > \
  { \
    static constexpr int32b kValue = kScalarResultTrue; \
  }

ZINVUL_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float);
ZINVUL_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float2);
ZINVUL_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float3);
ZINVUL_IS_SINGLE_FLOAT_TEMPLATE_SPECIALIZATION_IMPL(float4);

template <typename T>
struct IsDouble
{
  static constexpr int32b kValue = kScalarResultFalse;
};

#define ZINVUL_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(Type) \
  template <> \
  struct IsDouble< Type > \
  { \
    static constexpr int32b kValue = kScalarResultTrue; \
  }

ZINVUL_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double);
ZINVUL_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double2);
ZINVUL_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double3);
ZINVUL_IS_DOUBLE_TEMPLATE_SPECIALIZATION_IMPL(double4);

template <typename T>
struct IsFloatingPoint
{
  static constexpr int32b kValue = kIsHalf<T> | kIsSingleFloat<T> | kIsDouble<T>;
};

template <typename T>
struct IsLValueReference
{
  static constexpr int32b kValue = kScalarResultFalse;
};

template <typename T>
struct IsLValueReference<T&>
{
  static constexpr int32b kValue = kScalarResultTrue;
};

template <typename T>
struct IsRValueReference
{
  static constexpr int32b kValue = kScalarResultFalse;
};

template <typename T>
struct IsRValueReference<T&&>
{
  static constexpr int32b kValue = kScalarResultTrue;
};

template <typename T1, typename T2>
struct IsSame
{
  static constexpr int32b kValue = kScalarResultFalse;
};

template <typename T>
struct IsSame<T, T>
{
  static constexpr int32b kValue = kScalarResultTrue;
};

template <typename T>
struct RemoveConst
{
  using Type = T;
};

template <typename T>
struct AddConst
{
  using Type = const T;
};

template <typename T>
struct AddVolatile
{
  using Type = volatile T;
};

template <typename T>
struct AddCv
{
  using Type = AddConstType<AddVolatileType<T>>;
};

template <typename T>
struct RemoveConst<const T>
{
  using Type = T;
};

template <typename T>
struct RemoveVolatile
{
  using Type = T;
};

template <typename T>
struct RemoveVolatile<volatile T>
{
  using Type = T;
};

template <typename T>
struct RemoveCv
{
  using Type = RemoveConstType<RemoveVolatileType<T>>;
};

template <typename T>
struct AddLValueReference
{
  using NoRefType = RemoveReferenceType<T>;
  using Type = NoRefType&;
};

template <typename T>
struct AddRValueReference
{
  using NoRefType = RemoveReferenceType<T>;
  using Type = NoRefType&&;
};

template <typename T>
struct RemoveReference
{
  using Type = T;
};

template <typename T>
struct RemoveReference<T&>
{
  using Type = T;
};

template <typename T>
struct RemoveReference<T&&>
{
  using Type = T;
};

template <typename T>
struct RemoveCvref
{
  using Type = RemoveCvType<RemoveReferenceType<T>>;
};

template <typename T>
struct AddPointer
{
  using NoPointerType = RemovePointerType<T>;
  using Type = NoPointerType*;
};

template <typename T>
struct RemovePointer
{
  using Type = T;
};

template <typename T>
struct RemovePointer<T*>
{
  using Type = T;
};

template <typename T>
struct RemovePointer<T* const>
{
  using Type = T;
};

template <typename T>
struct RemovePointer<T* volatile>
{
  using Type = T;
};

template <typename T>
struct RemovePointer<T* const volatile>
{
  using Type = T;
};

template <typename T, typename F>
struct Conditional<true, T, F>
{
  using Type = T;
};

template <typename T, typename F>
struct Conditional<false, T, F>
{
  using Type = F;
};

template <typename T>
struct TypeIdentity
{
  using Type = T;
};

} // namespace zinvul

#endif /* ZINVUL_TYPE_TRAITS_INL_CL */
