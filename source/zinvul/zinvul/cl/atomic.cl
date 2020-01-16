/*!
  \file atomic.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ATOMIC_CL
#define ZINVUL_ATOMIC_CL

#include "types.cl"

namespace zinvul {

/*!
  */
class Atomic
{
 public:
  //! Perform atomic addition for global or local integer buffer
  template <typename AddressSpaceInteger, typename Integer>
  static Integer add(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic subtraction for global or local integer buffer
  template <typename AddressSpaceInteger, typename Integer>
  static Integer sub(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic swapping for global or local buffer
  template <typename AddressSpaceType, typename Type>
  static Type swap(AddressSpaceType p, const Type value) noexcept;

  //! Perform atomic post-increment for global or local integer buffer
  template <typename AddressSpaceInteger>
  static auto increment(AddressSpaceInteger p) noexcept;

  //! Perform atomic post-decrement for global or local integer buffer
  template <typename AddressSpaceInteger>
  static auto decrement(AddressSpaceInteger p) noexcept;

  //! Perform atomic comparison and swapping for global or local integer buffer
  template <typename AddressSpaceInteger, typename Integer>
  static Integer compareAndSwap(AddressSpaceInteger p,
                                const Integer comp,
                                const Integer value) noexcept;

  //! Perform atomic min
  template <typename AddressSpaceInteger, typename Integer>
  static Integer min(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic max
  template <typename AddressSpaceInteger, typename Integer>
  static Integer max(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic bit and
  template <typename AddressSpaceInteger, typename Integer>
  static Integer bitAnd(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic bit or
  template <typename AddressSpaceInteger, typename Integer>
  static Integer bitOr(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic bit xor
  template <typename AddressSpaceInteger, typename Integer>
  static Integer bitXor(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform an expression atomically
  template <typename AddressSpaceInteger, typename Function, typename ...Types>
  static auto perform(AddressSpaceInteger p,
                      Function expression,
                      Types&&... arguments) noexcept;
 private:
  //! Perform atomic addition
  template <typename AddressSpaceInteger, typename Integer>
  static Integer addImpl(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic subtraction
  template <typename AddressSpaceInteger, typename Integer>
  static Integer subImpl(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic swapping
  template <typename AddressSpaceType, typename Type>
  static Type swapImpl(AddressSpaceType p, const Type value) noexcept;

  //! Perform atomic post-increment
  template <typename AddressSpaceInteger>
  static auto incrementImpl(AddressSpaceInteger p) noexcept;

  //! Perform atomic post-decrement
  template <typename AddressSpaceInteger>
  static auto decrementImpl(AddressSpaceInteger p) noexcept;

  //! Perform atomic comparison and swapping
  template <typename AddressSpaceInteger, typename Integer>
  static Integer compareAndSwapImpl(AddressSpaceInteger p,
                                    const Integer comp,
                                    const Integer value) noexcept;

  //! Perform atomic min
  template <typename AddressSpaceInteger, typename Integer>
  static Integer minImpl(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic max
  template <typename AddressSpaceInteger, typename Integer>
  static Integer maxImpl(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic bit and
  template <typename AddressSpaceInteger, typename Integer>
  static Integer bitAndImpl(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic bit or
  template <typename AddressSpaceInteger, typename Integer>
  static Integer bitOrImpl(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform atomic bit xor
  template <typename AddressSpaceInteger, typename Integer>
  static Integer bitXorImpl(AddressSpaceInteger p, const Integer value) noexcept;

  //! Perform an expression atomically
  template <typename AddressSpaceInteger, typename Function, typename ...Types>
  static auto performImpl(AddressSpaceInteger p,
                          Function expression,
                          Types&&... arguments) noexcept;

  //! Check if the given type is global or local integer pointer
  template <typename Type>
  static constexpr bool isGlobalOrLocalPtr() noexcept;

  //! Check if the given type is global or local integer pointer
  template <typename Type>
  static constexpr bool isGlobalOrLocalInteger32Ptr() noexcept;

  //! Check if the given type is 32bit integer type
  template <typename Type>
  static constexpr bool isInteger32() noexcept;
};

// CL function aliases

//! Perform atomic addition
template <typename AddressSpaceInteger, typename Integer>
Integer atomic_add(AddressSpaceInteger p, const Integer value) noexcept;

//! Perform atomic subtraction
template <typename AddressSpaceInteger, typename Integer>
Integer atomic_sub(AddressSpaceInteger p, const Integer value) noexcept;

//! Perform atomic swapping
template <typename AddressSpaceType, typename Type>
Type atomic_xchg(AddressSpaceType p, const Type value) noexcept;

//! Perform atomic post-increment
template <typename AddressSpaceInteger>
auto atomic_inc(AddressSpaceInteger p) noexcept;

//! Perform atomic post-decrement
template <typename AddressSpaceInteger>
auto atomic_dec(AddressSpaceInteger p) noexcept;

//! Perform atomic comparison and swapping
template <typename AddressSpaceInteger, typename Integer>
Integer atomic_cmpxchg(AddressSpaceInteger p,
                       const Integer comp,
                       const Integer value) noexcept;

//! Perform atomic min
template <typename AddressSpaceInteger, typename Integer>
Integer atomic_min(AddressSpaceInteger p, const Integer value) noexcept;

//! Perform atomic max
template <typename AddressSpaceInteger, typename Integer>
Integer atomic_max(AddressSpaceInteger p, const Integer value) noexcept;

//! Perform atomic bit and
template <typename AddressSpaceInteger, typename Integer>
Integer atomic_and(AddressSpaceInteger p, const Integer value) noexcept;

//! Perform atomic bit or
template <typename AddressSpaceInteger, typename Integer>
Integer atomic_or(AddressSpaceInteger p, const Integer value) noexcept;

//! Perform atomic bit xor
template <typename AddressSpaceInteger, typename Integer>
Integer atoic_xor(AddressSpaceInteger p, const Integer value) noexcept;

} // namespace zinvul

#include "atomic-inl.cl"

#endif /* ZINVUL_ATOMIC_CL */
