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
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::add(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = addImpl(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::sub(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = subImpl(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceType, typename Type> inline
Type Atomic::swap(AddressSpaceType p, const Type value) noexcept
{
  const auto old = swapImpl(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger> inline
auto Atomic::increment(AddressSpaceInteger p) noexcept
{
  const auto old = incrementImpl(p);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger> inline
auto Atomic::decrement(AddressSpaceInteger p) noexcept
{
  const auto old = decrementImpl(p);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::compareAndSwap(AddressSpaceInteger p,
                               const Integer comp,
                               const Integer value) noexcept
{
  const auto old = compareAndSwapImpl(p, comp, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::min(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = minImpl(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::max(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = maxImpl(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::bitAnd(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = bitAndImpl(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::bitOr(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = bitOrImpl(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::bitXor(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = bitXorImpl(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Function, typename ...Types> inline
auto Atomic::perform(AddressSpaceInteger p,
                     Function expression,
                     Types&&... arguments) noexcept
{
  const auto old = performImpl(p, expression, forward<Types>(arguments)...);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::addImpl(AddressSpaceInteger p, const Integer value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Integer>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_add(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::subImpl(AddressSpaceInteger p, const Integer value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Integer>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_sub(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceType, typename Type> inline
Type Atomic::swapImpl(AddressSpaceType p, const Type value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceType>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Type>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_xchg(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger> inline
auto Atomic::incrementImpl(AddressSpaceInteger p) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_inc(p);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger> inline
auto Atomic::decrementImpl(AddressSpaceInteger p) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_dec(p);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::compareAndSwapImpl(AddressSpaceInteger p,
                                   const Integer comp,
                                   const Integer value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Integer>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_cmpxchg(p, comp, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::minImpl(AddressSpaceInteger p, const Integer value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Integer>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_min(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::maxImpl(AddressSpaceInteger p, const Integer value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Integer>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_max(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::bitAndImpl(AddressSpaceInteger p, const Integer value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Integer>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_and(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::bitOrImpl(AddressSpaceInteger p, const Integer value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Integer>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_or(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer Atomic::bitXorImpl(AddressSpaceInteger p, const Integer value) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  static_assert(isInteger32<Integer>(),
                "The Integer isn't 32bit integer type.");
  const auto old = ZINVUL_GLOBAL_NAMESPACE::atomic_xor(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Function, typename ...Types> inline
auto Atomic::performImpl(AddressSpaceInteger p,
                         Function expression,
                         Types&&... arguments) noexcept
{
  static_assert(isGlobalOrLocalInteger32Ptr<AddressSpaceInteger>(),
                "The given p isn't global or local 32bit integer.");
  using Integer = RemoveCvrefType<decltype(expression(*p, arguments...))>;
  static_assert(isInteger32<Integer>(),
                "The return type of the expression isn't 32bit integer type.");
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

/*!
  */
template <typename Type> inline
constexpr bool Atomic::isGlobalOrLocalPtr() noexcept
{
  using ASpaceInfo = AddressSpaceInfo<Type>;
  const bool is_global_or_local = (ASpaceInfo::isGlobal() != 0) ||
                                  (ASpaceInfo::isLocal() != 0);
  const bool is_pointer = ASpaceInfo::isPointer() != 0;
  return is_global_or_local && is_pointer;
}

/*!
  */
template <typename Type> inline
constexpr bool Atomic::isGlobalOrLocalInteger32Ptr() noexcept
{
  using ASpaceInfo = AddressSpaceInfo<Type>;
  using DataType = typename ASpaceInfo::DataType;
  return isGlobalOrLocalPtr<Type>() && isInteger32<DataType>();
}

/*!
  */
template <typename Type> inline
constexpr bool Atomic::isInteger32() noexcept
{
  const bool is_integer32 = (kIsInteger<Type> != 0) && (sizeof(Type) == 4);
  return is_integer32;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer atomic_add(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = Atomic::add(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer atomic_sub(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = Atomic::sub(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceType, typename Type> inline
Type atomic_xchg(AddressSpaceType p, const Type value) noexcept
{
  const auto old = Atomic::swap(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger> inline
auto atomic_inc(AddressSpaceInteger p) noexcept
{
  const auto old = Atomic::increment(p);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger> inline
auto atomic_dec(AddressSpaceInteger p) noexcept
{
  const auto old = Atomic::decrement(p);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer atomic_cmpxchg(AddressSpaceInteger p,
                       const Integer comp,
                       const Integer value) noexcept
{
  const auto old = Atomic::compareAndSwap(p, comp, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer atomic_min(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = Atomic::min(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer atomic_max(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = Atomic::max(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer atomic_and(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = Atomic::bitAnd(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer atomic_or(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = Atomic::bitOr(p, value);
  return old;
}

/*!
  */
template <typename AddressSpaceInteger, typename Integer> inline
Integer atoic_xor(AddressSpaceInteger p, const Integer value) noexcept
{
  const auto old = Atomic::bitXor(p, value);
  return old;
}

} // namespace zinvul

#endif /* ZINVUL_ATOMIC_INL_CL */
