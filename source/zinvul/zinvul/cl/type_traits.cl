/*!
  \file type_traits.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPE_TRAITS_CL
#define ZINVUL_TYPE_TRAITS_CL

// Zinvul
#include "types.cl"

namespace zinvul {

//! Make a signed integer type from byte size
template <size_t kBytes>
struct IntegerFromBytes
{
  using Type = int32b;
  static_assert(kBytes == 0, "The specified size is unsupported.");
};

template <size_t kBytes>
using IntegerTypeFromBytes = typename IntegerFromBytes<kBytes>::Type;

//! Make an unsigned integer type from byte size
template <size_t kBytes>
struct UIntegerFromBytes
{
  using Type = int32b;
  static_assert(kBytes == 0, "The specified size is unsupported.");
};

template <size_t kBytes>
using UIntegerTypeFromBytes = typename UIntegerFromBytes<kBytes>::Type;

//! Make a floating point type from byte size
template <size_t kBytes>
struct FloatFromBytes
{
  using Type = int32b;
  static_assert(kBytes == 0, "The specified size is unsupported.");
};

template <size_t kBytes>
using FloatTypeFromBytes = typename FloatFromBytes<kBytes>::Type;

//! Information of a vector type
template <typename Type>
struct VectorTypeInfo
{
  using ElementType = int32b;
  //! Return the number of elements of the type
  static constexpr size_t size() noexcept {return 0u;}
  static_assert(sizeof(Type) == 0, "The 'Type' is unsupported type.");
};

//! Make a vector type
template <typename ElementType, size_t kN>
struct VectorFromElems
{
  using Type = int32b;
  static_assert(sizeof(ElementType) == 0, "The vector type is unsupported type.");
};

template <typename ElementType, size_t kN>
using VectorTypeFromElems = typename VectorFromElems<ElementType, kN>::Type;

template <size_t kN>
using Integer8VecType = VectorTypeFromElems<int8b, kN>;

template <size_t kN>
using Integer16VecType = VectorTypeFromElems<int16b, kN>;

template <size_t kN>
using Integer32VecType = VectorTypeFromElems<int32b, kN>;

template <size_t kN>
using Integer64VecType = VectorTypeFromElems<int64b, kN>;

template <size_t kN>
using UInteger8VecType = VectorTypeFromElems<uint8b, kN>;

template <size_t kN>
using UInteger16VecType = VectorTypeFromElems<uint16b, kN>;

template <size_t kN>
using UInteger32VecType = VectorTypeFromElems<uint32b, kN>;

template <size_t kN>
using UInteger64VecType = VectorTypeFromElems<uint64b, kN>;

template <size_t kN>
using Float16VecType = VectorTypeFromElems<half, kN>;

template <size_t kN>
using Float32VecType = VectorTypeFromElems<float, kN>;

template <size_t kN>
using Float64VecType = VectorTypeFromElems<double, kN>;

//! Integer vector type that has same element size and alignment as the given type
template <typename T>
using IntegerTypeFromVec = VectorTypeFromElems<
    IntegerTypeFromBytes<sizeof(typename VectorTypeInfo<T>::ElementType)>,
    VectorTypeInfo<T>::size()>;

//! Unsigned integer type that has same element size and alignment as the given type
template <typename T>
using UIntegerTypeFromVec = VectorTypeFromElems<
    UIntegerTypeFromBytes<sizeof(typename VectorTypeInfo<T>::ElementType)>,
    VectorTypeInfo<T>::size()>;

//! Floating point type that has same element size and alignment as the given type
template <typename T>
using FloatTypeFrom = VectorTypeFromElems<
    FloatTypeFromBytes<sizeof(typename VectorTypeInfo<T>::ElementType)>,
    VectorTypeInfo<T>::size()>;

//! Information of an address space type
template <typename T>
class AddressSpaceInfo;
//{
//  using Type = typename ASpaceInfo::Type;
//  static constexpr int32b isGlobal() noexcept;
//  static constexpr int32b isLocal() noexcept;
//  static constexpr int32b isConstant() noexcept;
//  static constexpr int32b isPrivate() noexcept;
//};

// Type categories

//! Check if a type is integer scalar or vector type
template <typename T> struct IsInteger;

template <typename T>
constexpr Constant<int32b> kIsInteger = IsInteger<T>::kValue;

//! Check if a type is signed integer scalar or vector type
template <typename T> struct IsSignedInteger;

template <typename T>
constexpr Constant<int32b> kIsSignedInteger = IsSignedInteger<T>::kValue;

//! Check if a type is unsigned integer scalar or vector type
template <typename T> struct IsUnsignedInteger;

template <typename T>
constexpr Constant<int32b> kIsUnsignedInteger = IsUnsignedInteger<T>::kValue;

//! Check if a type is floating point scalar or vector type
template <typename T> struct IsFloatingPoint;

template <typename T>
constexpr Constant<int32b> kIsFloatingPoint = IsFloatingPoint<T>::kValue;

//! Check if a type is half scalar or vector type
template <typename T> struct IsHalf;

template <typename T>
constexpr Constant<int32b> kIsHalf = IsHalf<T>::kValue;

//! Check if a type is float scalar or vector type
template <typename T> struct IsSingleFloat;

template <typename T>
constexpr Constant<int32b> kIsSingleFloat = IsSingleFloat<T>::kValue;

//! Check if a type is double scalar or vector type
template <typename T> struct IsDouble;

template <typename T>
constexpr Constant<int32b> kIsDouble = IsDouble<T>::kValue;

//! Check if a type is an arithmetic type
template <typename T> struct IsArithmetic;

template <typename T>
constexpr Constant<int32b> kIsArithmetic = IsArithmetic<T>::kValue;

//! Check if a type is a signed arithmetic type
template <typename T> struct IsSigned;

template <typename T>
constexpr Constant<int32b> kIsSigned = IsSigned<T>::kValue;

// Type properties

//! Check if a type is a pointer type
template <typename T> struct IsPointer;

template <typename T>
constexpr Constant<int32b> kIsPointer = IsPointer<T>::kValue;

//! Check if a type is lvalue reference
template <typename T> struct IsLValueReference;

template <typename T>
constexpr Constant<int32b> kIsLValueReference = IsLValueReference<T>::kValue;

//! Check if a type is rvalue reference
template <typename T> struct IsRValueReference;

template <typename T>
constexpr Constant<int32b> kIsRValueReference = IsRValueReference<T>::kValue;

//! Check if two types are same
template <typename T1, typename T2> struct IsSame;

template <typename T1, typename T2>
constexpr Constant<int32b> kIsSame = IsSame<T1, T2>::kValue;

// Miscellaneous transformations

//! Add 'const' and/or 'volatile' specifiers to the given type
template <typename T> struct AddCv;

template <typename T>
using AddCvType = typename AddCv<T>::Type;

//! Add 'const' specifiers to the given type
template <typename T> struct AddConst;

template <typename T>
using AddConstType = typename AddConst<T>::Type;

//! Add 'volatile' specifiers to the given type
template <typename T> struct AddVolatile;

template <typename T>
using AddVolatileType = typename AddVolatile<T>::Type;

//! Remove 'const' and/or 'volatile' specifiers from the given type
template <typename T> struct RemoveCv;

template <typename T>
using RemoveCvType = typename RemoveCv<T>::Type;

//! Remove 'const' specifiers from the given type
template <typename T> struct RemoveConst;

template <typename T>
using RemoveConstType = typename RemoveConst<T>::Type;

//! Remove 'volatile' specifiers from the given type
template <typename T> struct RemoveVolatile;

template <typename T>
using RemoveVolatileType = typename RemoveVolatile<T>::Type;

//! Add 'lvalue' reference to the given type
template <typename T> struct AddLValueReference;

template <typename T>
using AddLValueReferenceType = typename AddLValueReference<T>::Type;

//! Add 'rvalue' reference to the given type
template <typename T> struct AddRValueReference;

template <typename T>
using AddRValueReferenceType = typename AddRValueReference<T>::Type;

//! Remove a reference from the given type
template <typename T> struct RemoveReference;

template <typename T>
using RemoveReferenceType = typename RemoveReference<T>::Type;

//! Combine RemoveCv and RemoveReference
template <typename T> struct RemoveCvref;

template <typename T>
using RemoveCvrefType = typename RemoveCvref<T>::Type;

//! Add a pointer to the given type
template <typename T> struct AddPointer;

template <typename T>
using AddPointerType = typename AddPointer<T>::Type;

//! Remove a pointer from the given type
template <typename T> struct RemovePointer;

template <typename T>
using RemovePointerType = typename RemovePointer<T>::Type;

//! Choose one type or another based on compile-time boolean
template <bool kCondition, typename T, typename F> struct Conditional;

template <bool kCondition, typename T, typename F>
using ConditionalType = typename Conditional<kCondition, T, F>::Type;

//! Return the type argument unchanged
template <typename T> struct TypeIdentity;

template <typename T>
using TypeIdentityType = typename TypeIdentity<T>::Type;

// Comparison type

constexpr Constant<int32b> kResultFalse = 0b0;

template <typename T>
constexpr Constant<int32b> kResultTrue = (VectorTypeInfo<T>::size() == 1)
    ? 0b1
    : ~0b0;

//! Integer type that is used as a comparison result of the given type
template <typename T>
using ComparisonResultType = IntegerTypeFromVec<RemoveCvType<T>>;

} // namespace zinvul

#include "type_traits-inl.cl"

#endif /* ZINVUL_TYPE_TRAITS_CL */
