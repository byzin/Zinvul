/*!
  \file atomic.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
  //! Perform atomic addition
  template <typename Integer>
  static Integer add(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic addition
  template <typename Integer>
  static Integer add(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic subtraction
  template <typename Integer>
  static Integer sub(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic subtraction
  template <typename Integer>
  static Integer sub(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic swapping
  template <typename Type>
  static Type swap(GlobalPtr<Type> p, const Type value) noexcept;

  //! Perform atomic swapping
  template <typename Type>
  static Type swap(LocalPtr<Type> p, const Type value) noexcept;

  //! Perform atomic post-increment
  template <typename Integer>
  static Integer increment(GlobalPtr<Integer> p) noexcept;

  //! Perform atomic post-increment
  template <typename Integer>
  static Integer increment(LocalPtr<Integer> p) noexcept;

  //! Perform atomic post-decrement
  template <typename Integer>
  static Integer decrement(GlobalPtr<Integer> p) noexcept;

  //! Perform atomic post-decrement
  template <typename Integer>
  static Integer decrement(LocalPtr<Integer> p) noexcept;

  //! Perform atomic comparison and swapping
  template <typename Integer>
  static Integer compareAndSwap(GlobalPtr<Integer> p,
                                const Integer comp,
                                const Integer value) noexcept;

  //! Perform atomic comparison and swapping
  template <typename Integer>
  static Integer compareAndSwap(LocalPtr<Integer> p,
                                const Integer comp,
                                const Integer value) noexcept;

  //! Perform atomic min
  template <typename Integer>
  static Integer min(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic min
  template <typename Integer>
  static Integer min(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic max
  template <typename Integer>
  static Integer max(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic max
  template <typename Integer>
  static Integer max(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit and
  template <typename Integer>
  static Integer bitAnd(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit and
  template <typename Integer>
  static Integer bitAnd(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit or
  template <typename Integer>
  static Integer bitOr(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit or
  template <typename Integer>
  static Integer bitOr(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit xor
  template <typename Integer>
  static Integer bitXor(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit xor
  template <typename Integer>
  static Integer bitXor(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform an expression atomically
  template <typename Integer, typename Function, typename ...Types>
  static Integer perform(GlobalPtr<Integer> p,
                         Function expression,
                         Types&&... arguments) noexcept;

  //! Perform an expression atomically
  template <typename Integer, typename Function, typename ...Types>
  static Integer perform(LocalPtr<Integer> p,
                         Function expression,
                         Types&&... arguments) noexcept;
 private:
  //! Perform atomic addition
  template <typename BufferPtr, typename Integer>
  static Integer addImpl(BufferPtr p, const Integer value) noexcept;

  //! Perform atomic subtraction
  template <typename BufferPtr, typename Integer>
  static Integer subImpl(BufferPtr p, const Integer value) noexcept;

  //! Perform atomic swapping
  template <typename BufferPtr, typename Type>
  static Type swapImpl(BufferPtr p, const Type value) noexcept;

  //! Perform atomic post-increment
  template <typename BufferPtr, typename Integer>
  static Integer incrementImpl(BufferPtr p) noexcept;

  //! Perform atomic post-decrement
  template <typename BufferPtr, typename Integer>
  static Integer decrementImpl(BufferPtr p) noexcept;

  //! Perform atomic comparison and swapping
  template <typename BufferPtr, typename Integer>
  static Integer compareAndSwapImpl(BufferPtr p,
                                    const Integer comp,
                                    const Integer value) noexcept;

  //! Perform atomic min
  template <typename BufferPtr, typename Integer>
  static Integer minImpl(BufferPtr p, const Integer value) noexcept;

  //! Perform atomic max
  template <typename BufferPtr, typename Integer>
  static Integer maxImpl(BufferPtr p, const Integer value) noexcept;

  //! Perform atomic bit and
  template <typename BufferPtr, typename Integer>
  static Integer bitAndImpl(BufferPtr p, const Integer value) noexcept;

  //! Perform atomic bit or
  template <typename BufferPtr, typename Integer>
  static Integer bitOrImpl(BufferPtr p, const Integer value) noexcept;

  //! Perform atomic bit xor
  template <typename BufferPtr, typename Integer>
  static Integer bitXorImpl(BufferPtr p, const Integer value) noexcept;

  //! Perform an expression atomically
  template <typename BufferPtr, typename Integer, typename Function, typename ...Types>
  static Integer performImpl(BufferPtr p,
                             Function expression,
                             Types&&... arguments) noexcept;
};

// CL function aliases

//! Perform atomic addition
template <typename Integer>
Integer atomic_add(GlobalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic addition
template <typename Integer>
Integer atomic_add(LocalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic subtraction
template <typename Integer>
Integer atomic_sub(GlobalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic subtraction
template <typename Integer>
Integer atomic_sub(LocalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic swapping
template <typename Type>
Type atomic_xchg(GlobalPtr<Type> p, const Type value) noexcept;

//! Perform atomic swapping
template <typename Type>
Type atomic_xchg(LocalPtr<Type> p, const Type value) noexcept;

//! Perform atomic post-increment
template <typename Integer>
Integer atomic_inc(GlobalPtr<Integer> p) noexcept;

//! Perform atomic post-increment
template <typename Integer>
Integer atomic_inc(LocalPtr<Integer> p) noexcept;

//! Perform atomic post-decrement
template <typename Integer>
Integer atomic_dec(GlobalPtr<Integer> p) noexcept;

//! Perform atomic post-decrement
template <typename Integer>
Integer atomic_dec(LocalPtr<Integer> p) noexcept;

//! Perform atomic comparison and swapping
template <typename Integer>
Integer atomic_cmpxchg(GlobalPtr<Integer> p,
                       const Integer comp,
                       const Integer value) noexcept;

//! Perform atomic comparison and swapping
template <typename Integer>
Integer atomic_cmpxchg(LocalPtr<Integer> p,
                       const Integer comp,
                       const Integer value) noexcept;

//! Perform atomic min
template <typename Integer>
Integer atomic_min(GlobalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic min
template <typename Integer>
Integer atomic_min(LocalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic max
template <typename Integer>
Integer atomic_max(GlobalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic max
template <typename Integer>
Integer atomic_max(LocalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic bit and
template <typename Integer>
Integer atomic_and(GlobalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic bit and
template <typename Integer>
Integer atomic_and(LocalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic bit or
template <typename Integer>
Integer atomic_or(GlobalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic bit or
template <typename Integer>
Integer atomic_or(LocalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic bit xor
template <typename Integer>
Integer atoic_xor(GlobalPtr<Integer> p, const Integer value) noexcept;

//! Perform atomic bit xor
template <typename Integer>
Integer atomic_xor(LocalPtr<Integer> p, const Integer value) noexcept;

} // namespace zinvul

#include "atomic-inl.cl"

#endif /* ZINVUL_ATOMIC_CL */
