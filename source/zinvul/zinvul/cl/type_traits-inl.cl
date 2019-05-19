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

template <typename T>
struct IsLValueReference
{
  static constexpr int32b value_ = 0;
};

template <typename T>
struct IsLValueReference<T&>
{
  static constexpr int32b value_ = 1;
};

template <typename T>
struct IsRValueReference
{
  static constexpr int32b value_ = 0;
};

template <typename T>
struct IsRValueReference<T&&>
{
  static constexpr int32b value_ = 1;
};

template <typename T1, typename T2>
struct IsSame
{
  static constexpr int32b value_ = 0;
};

template <typename T>
struct IsSame<T, T>
{
  static constexpr int32b value_ = 1;
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
