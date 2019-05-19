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

// Type properties

//! Check if a type is lvalue reference
template <typename T> struct IsLValueReference;

template <typename T>
constexpr int32b kIsLValueReference = IsLValueReference<T>::value_;

//! Check if a type is rvalue reference
template <typename T> struct IsRValueReference;

template <typename T>
constexpr int32b kIsRValueReference = IsRValueReference<T>::value_;

//! Check if two types are same
template <typename T1, typename T2> struct IsSame;

template <typename T1, typename T2>
constexpr int32b kIsSame = IsSame<T1, T2>::value_;

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

} // namespace zinvul

#include "type_traits-inl.cl"

#endif /* ZINVUL_TYPE_TRAITS_CL */
