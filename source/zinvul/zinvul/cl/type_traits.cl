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

//! Vector types
template <typename Type>
struct VectorType
{
  using ElementType = int32b;
  //! Return the number of elements of the type
  static constexpr size_t size() noexcept {return 0u;}
  static_assert(sizeof(Type) == 0, "The 'Type' is unsupported type.");
};

//! Integer vector type
template <size_t kBytes, size_t kN>
struct IntegerVector
{
  using Type = int32b;
  using ElementType = int32b;
  //! Return the number of elements of the type
  static constexpr size_t size() noexcept {return 0u;}
  static_assert(kN == 0, "The size of vector is unsupported.");
};

template <size_t kN>
using Integer8Vector = IntegerVector<1, kN>;

template <size_t kN>
using Integer8VectorType = typename Integer8Vector<kN>::Type;

template <size_t kN>
using Integer16Vector = IntegerVector<2, kN>;

template <size_t kN>
using Integer16VectorType = typename Integer16Vector<kN>::Type;

template <size_t kN>
using Integer32Vector = IntegerVector<4, kN>;

template <size_t kN>
using Integer32VectorType = typename Integer32Vector<kN>::Type;

template <size_t kN>
using Integer64Vector = IntegerVector<8, kN>;

template <size_t kN>
using Integer64VectorType = typename Integer64Vector<kN>::Type;

//! Unsigned integer vector type
template <size_t kBytes, size_t kN>
struct UIntegerVector
{
  using Type = uint32b;
  using ElementType = uint32b;
  //! Return the number of elements of the type
  static constexpr size_t size() noexcept {return 0u;}
  static_assert(kN == 0, "The size of vector is unsupported.");
};

template <size_t kN>
using UInteger8Vector = UIntegerVector<1, kN>;

template <size_t kN>
using UInteger8VectorType = typename UInteger8Vector<kN>::Type;

template <size_t kN>
using UInteger16Vector = UIntegerVector<2, kN>;

template <size_t kN>
using UInteger16VectorType = typename UInteger16Vector<kN>::Type;

template <size_t kN>
using UInteger32Vector = UIntegerVector<4, kN>;

template <size_t kN>
using UInteger32VectorType = typename UInteger32Vector<kN>::Type;

template <size_t kN>
using UInteger64Vector = UIntegerVector<8, kN>;

template <size_t kN>
using UInteger64VectorType = typename UInteger64Vector<kN>::Type;

//! Unsigned integer vector type
template <size_t kBytes, size_t kN>
struct FloatVector 
{
  using Type = float;
  using ElementType = float;
  //! Return the number of elements of the type
  static constexpr size_t size() noexcept {return 0u;}
  static_assert(kN == 0, "The size of vector is unsupported.");
};

template <size_t kN>
using Float16Vector = FloatVector<2, kN>;

template <size_t kN>
using Float16VectorType = typename Float16Vector<kN>::Type;

template <size_t kN>
using Float32Vector = FloatVector<4, kN>;

template <size_t kN>
using Float32VectorType = typename Float32Vector<kN>::Type;

template <size_t kN>
using Float64Vector = FloatVector<8, kN>;

template <size_t kN>
using Float64VectorType = typename Float64Vector<kN>::Type;

//! Integer type that is same element size and alignment as the given type
template <typename T>
using IntegerFrom = IntegerVector<sizeof(typename VectorType<T>::ElementType),
                                  VectorType<T>::size()>;

//! Integer type that is same element size and alignment as the given type
template <typename T>
using IntegerTypeFrom = typename IntegerFrom<T>::Type;

//! Unsigned integer type that is same element size and alignment as the given type
template <typename T>
using UIntegerFrom = UIntegerVector<sizeof(typename VectorType<T>::ElementType),
                                    VectorType<T>::size()>;

//! Unsigned integer type that is same element size and alignment as the given type
template <typename T>
using UIntegerTypeFrom = typename UIntegerFrom<T>::Type;

//! Floating point type that is same element size and alignment as the given type
template <typename T>
using FloatFrom = FloatVector<sizeof(typename VectorType<T>::ElementType),
                              VectorType<T>::size()>;

//! Floating point type that is same element size and alignment as the given type
template <typename T>
using FloatTypeFrom = typename FloatFrom<T>::Type;

// Type categories

//! Check if a type is integer scalar or vector type
template <typename T> struct IsInteger;

template <typename T>
constexpr int32b kIsInteger = IsInteger<T>::kValue;

//! Check if a type is signed integer scalar or vector type
template <typename T> struct IsSignedInteger;

template <typename T>
constexpr int32b kIsSignedInteger = IsSignedInteger<T>::kValue;

//! Check if a type is unsigned integer scalar or vector type
template <typename T> struct IsUnsignedInteger;

template <typename T>
constexpr int32b kIsUnsignedInteger = IsUnsignedInteger<T>::kValue;

//! Check if a type is floating point scalar or vector type
template <typename T> struct IsFloatingPoint;

template <typename T>
constexpr int32b kIsFloatingPoint = IsFloatingPoint<T>::kValue;

//! Check if a type is half scalar or vector type
template <typename T> struct IsHalf;

template <typename T>
constexpr int32b kIsHalf = IsHalf<T>::kValue;

//! Check if a type is float scalar or vector type
template <typename T> struct IsSingleFloat;

template <typename T>
constexpr int32b kIsSingleFloat = IsSingleFloat<T>::kValue;

//! Check if a type is double scalar or vector type
template <typename T> struct IsDouble;

template <typename T>
constexpr int32b kIsDouble = IsDouble<T>::kValue;

//! Check if a type is an arithmetic type
template <typename T> struct IsArithmetic;

template <typename T>
constexpr int32b kIsArithmetic = IsArithmetic<T>::kValue;

//! Check if a type is a signed arithmetic type
template <typename T> struct IsSigned;

template <typename T>
constexpr int32b kIsSigned = IsSigned<T>::kValue;

// Type properties

//! Check if a type is lvalue reference
template <typename T> struct IsLValueReference;

template <typename T>
constexpr int32b kIsLValueReference = IsLValueReference<T>::kValue;

//! Check if a type is rvalue reference
template <typename T> struct IsRValueReference;

template <typename T>
constexpr int32b kIsRValueReference = IsRValueReference<T>::kValue;

//! Check if two types are same
template <typename T1, typename T2> struct IsSame;

template <typename T1, typename T2>
constexpr int32b kIsSame = IsSame<T1, T2>::kValue;

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

constexpr int32b kScalarResultFalse = 0b0;
constexpr int32b kScalarResultTrue = 0b1;

} // namespace zinvul

#include "type_traits-inl.cl"

namespace zinvul {

// Comparison type

//! Integer type that is used as a comparison result of the given type
template <typename T>
using ComparisonResult = IntegerFrom<T>;

//! Integer type that is used as a comparison result of the given type
template <typename T>
using ComparisonResultType = typename ComparisonResult<T>::Type;

//! Integer value that represents 'false' of a comparison result of the given type
template  <typename T>
constexpr Constant<typename ComparisonResult<T>::ElementType> kResultFalse{0b0};

//! Integer value that represents 'true' of a comparison result of the given type
template  <typename T>
constexpr Constant<typename ComparisonResult<T>::ElementType> kResultTrue =
    (VectorType<T>::size() == 1)
        ? typename ComparisonResult<T>::ElementType{0b1}
        : ~kResultFalse<T>;

} // namespace zinvul

#endif /* ZINVUL_TYPE_TRAITS_CL */
