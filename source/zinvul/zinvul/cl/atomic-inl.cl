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
#include "utility.cl"

namespace zinvul {

/*!
  */
template <typename Integer> inline
Integer Atomic::add(LocalPtr<Integer> p, const Integer value) noexcept
{
  return addImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::add(GlobalPtr<Integer> p, const Integer value) noexcept
{
  return addImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::sub(LocalPtr<Integer> p, const Integer value) noexcept
{
  return subImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::sub(GlobalPtr<Integer> p, const Integer value) noexcept
{
  return subImpl(p, value);
}

/*!
  */
template <typename Type> inline
Type Atomic::swap(LocalPtr<Type> p, const Type value) noexcept
{
  return swapImpl(p, value);
}

/*!
  */
template <typename Type> inline
Type Atomic::swap(GlobalPtr<Type> p, const Type value) noexcept
{
  return swapImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::increment(LocalPtr<Integer> p) noexcept
{
  return incrementImpl<LocalPtr<Integer>, Integer>(p);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::increment(GlobalPtr<Integer> p) noexcept
{
  return incrementImpl<GlobalPtr<Integer>, Integer>(p);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::decrement(LocalPtr<Integer> p) noexcept
{
  return decrementImpl<LocalPtr<Integer>, Integer>(p);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::decrement(GlobalPtr<Integer> p) noexcept
{
  return decrementImpl<GlobalPtr<Integer>, Integer>(p);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::compareAndSwap(LocalPtr<Integer> p,
                               const Integer comp,
                               const Integer value) noexcept
{
  return compareAndSwapImpl(p, comp, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::compareAndSwap(GlobalPtr<Integer> p,
                               const Integer comp,
                               const Integer value) noexcept
{
  return compareAndSwapImpl(p, comp, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::min(LocalPtr<Integer> p, const Integer value) noexcept
{
  return minImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::min(GlobalPtr<Integer> p, const Integer value) noexcept
{
  return minImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::max(LocalPtr<Integer> p, const Integer value) noexcept
{
  return maxImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::max(GlobalPtr<Integer> p, const Integer value) noexcept
{
  return maxImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitAnd(LocalPtr<Integer> p, const Integer value) noexcept
{
  return bitAndImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitAnd(GlobalPtr<Integer> p, const Integer value) noexcept
{
  return bitAndImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitOr(LocalPtr<Integer> p, const Integer value) noexcept
{
  return bitOrImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitOr(GlobalPtr<Integer> p, const Integer value) noexcept
{
  return bitOrImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitXor(LocalPtr<Integer> p, const Integer value) noexcept
{
  return bitXorImpl(p, value);
}

/*!
  */
template <typename Integer> inline
Integer Atomic::bitXor(GlobalPtr<Integer> p, const Integer value) noexcept
{
  return bitXorImpl(p, value);
}

/*!
  */
template <typename Type, typename Function, typename ...Types> inline
Type Atomic::perform(LocalPtr<Type> p,
                     Function expression,
                     Types&&... arguments) noexcept
{
  return performImpl<LocalPtr<Type>, Type>(p,
                                           expression,
                                           forward<Types>(arguments)...);
}

/*!
  */
template <typename Type, typename Function, typename ...Types> inline
Type Atomic::perform(GlobalPtr<Type> p,
                     Function expression,
                     Types&&... arguments) noexcept
{
  return performImpl<GlobalPtr<Type>, Type>(p,
                                            expression,
                                            forward<Types>(arguments)...);
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::addImpl(BufferPtr p, const Integer value) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_add(p, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::subImpl(BufferPtr p, const Integer value) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_sub(p, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Type> inline
Type Atomic::swapImpl(BufferPtr p, const Type value) noexcept
{
  using T = RemoveCvrefType<Type>;
  static_assert(kIsSame<T, int32b> || kIsSame<T, uint32b> || kIsSame<T, float>,
                "The Type isn't int, unsigned int or float.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_xchg(p, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::incrementImpl(BufferPtr p) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_inc(p);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::decrementImpl(BufferPtr p) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_dec(p);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::compareAndSwapImpl(BufferPtr p,
                                   const Integer comp,
                                   const Integer value) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_cmpxchg(p, comp, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::minImpl(BufferPtr p, const Integer value) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_min(p, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::maxImpl(BufferPtr p, const Integer value) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_max(p, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::bitAndImpl(BufferPtr p, const Integer value) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_and(p, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::bitOrImpl(BufferPtr p, const Integer value) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_or(p, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer> inline
Integer Atomic::bitXorImpl(BufferPtr p, const Integer value) noexcept
{
  using I = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I, int32b> || kIsSame<I, uint32b>,
                "The Integer isn't int or unsigned int.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_xor(p, value);
  return old;
}

/*!
  */
template <typename BufferPtr, typename Integer, typename Function, typename ...Types>
inline
Integer Atomic::performImpl(BufferPtr p,
                            Function expression,
                            Types&&... arguments) noexcept
{
  // Type check
  using I1 = RemoveCvrefType<Integer>;
  static_assert(kIsSame<I1, int32b> || kIsSame<I1, uint32b>,
                "The Integer isn't int or unsigned int.");
  using I2 = RemoveCvrefType<decltype(expression(*p, arguments...))>;
  static_assert(kIsSame<I1, I2>,
      "The buffer type and the return type of the expression isn't same.");
  // Perform an expression atomically
  auto old = *p;
  auto cmp = old;
#if !(defined(Z_MAC) && defined(ZINVUL_VULKAN))
  do {
#endif
    cmp = old;
    const auto value = expression(cmp, forward<Types>(arguments)...);
    old = compareAndSwap(p, cmp, value);
#if !(defined(Z_MAC) && defined(ZINVUL_VULKAN))
  } while (old != cmp);
#endif
  return old;
}

} // namespace zinvul

#endif /* ZINVUL_ATOMIC_INL_CL */
