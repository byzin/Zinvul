/*!
  \file atomic-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ATOMIC_INL_CL
#define ZINVUL_ATOMIC_INL_CL

#include "atomic.cl"
// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  */
template <typename Integer> inline
Integer Atomic::add(LocalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_add(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::add(GlobalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_add(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::sub(LocalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_sub(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::sub(GlobalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_sub(p, value);
  return old;
}

/*!
  */
template <typename Type> inline
Type Atomic::swap(LocalPtr<Type> p, const Type value) noexcept
{
  static_assert(kIsSameType<Type, int32b> || kIsSameType<Type, uint32b> ||
                kIsSameType<Type, float>,
                "The Type isn't int, unsigned int or float.");
  const Private<Type> old = atomic_xchg(p, value);
  return old;
}

/*!
  */
template <typename Type> inline
Type Atomic::swap(GlobalPtr<Type> p, const Type value) noexcept
{
  static_assert(kIsSameType<Type, int32b> || kIsSameType<Type, uint32b> ||
                kIsSameType<Type, float>,
                "The Type isn't int, unsigned int or float.");
  const Private<Type> old = atomic_xchg(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::increment(LocalPtr<Integer> p) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_inc(p);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::increment(GlobalPtr<Integer> p) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_inc(p);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::decrement(LocalPtr<Integer> p) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_dec(p);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::decrement(GlobalPtr<Integer> p) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_dec(p);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::compareAndSwap(LocalPtr<Integer> p,
                               const Integer comp,
                               const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_cmpxchg(p, comp, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::compareAndSwap(GlobalPtr<Integer> p,
                               const Integer comp,
                               const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_cmpxchg(p, comp, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::min(LocalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_min(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::min(GlobalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_min(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::max(LocalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_max(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::max(GlobalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_max(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitAnd(LocalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_and(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitAnd(GlobalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_and(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitOr(LocalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_or(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitOr(GlobalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_or(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitXor(LocalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_xor(p, value);
  return old;
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitXor(GlobalPtr<Integer> p, const Integer value) noexcept
{
  static_assert(kIsSameType<Integer, int32b> || kIsSameType<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  const Private<Integer> old = atomic_xor(p, value);
  return old;
}

} // namespace zinvul

#endif /* ZINVUL_ATOMIC_INL_CL */
