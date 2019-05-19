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
//#include "utility.cl"

////! Perform atomic addition
//#define zAtomicAdd(p, value) atomic_add(p, value)
//
////! Perform atomic addition
//#define zAtomicAddU(p, value) atomic_add(p, value)
//
////! Perform atomic subtraction
//#define zAtomicSub(p, value) atomic_sub(p, value)
//
////! Perform atomic subtraction
//#define zAtomicSubU(p, value) atomic_sub(p, value)
//
////! Atomically replace the value
//#define zAtomicXchg(p, value) atomic_xchg(p, value)
//
////! Atomically replace the value
//#define zAtomicXchgU(p, value) atomic_xchg(p, value)
//
////! Perform atomic post-increment
//#define zAtomicInc(p) atomic_inc(p)
//
////! Perform atomic post-increment
//#define zAtomicIncU(p) atomic_inc(p)
//
////! Perform atomic post-decrement
//#define zAtomicDec(p) atomic_dec(p)
//
////! Perform atomic post-decrement
//#define zAtomicDecU(p) atomic_dec(p)
//
////! Atomically compare the values and exchange if equal
//#define zAtomicCmpxchg(p, cmp, value) atomic_cmpxchg(p, cmp, value)
//
////! Atomically compare the values and exchange if equal
//#define zAtomicCmpxchgU(p, cmp, value) atomic_cmpxchg(p, cmp, value)
//
////! Atomically compute min(old, value)
//#define zAtomicMin(p, value) atomic_min(p, value)
//
////! Atomically compute min(old, value)
//#define zAtomicMinU(p, value) atomic_min(p, value)
//
////! Atomically compute max(old, value)
//#define zAtomicMax(p, value) atomic_max(p, value)
//
////! Atomically compute max(old, value)
//#define zAtomicMaxU(p, value) atomic_max(p, value)
//
////! Atomically compute (old & value)
//#define zAtomicAnd(p, value) atomic_and(p, value)
//
////! Atomically compute (old & value)
//#define zAtomicAndU(p, value) atomic_and(p, value)
//
////! Atomically compute (old | value)
//#define zAtomicOr(p, value) atomic_or(p, value)
//
////! Atomically compute (old | value)
//#define zAtomicOrU(p, value) atomic_or(p, value)
//
////! Atomically compute (old ^ value)
//#define zAtomicXor(p, value) atomic_xor(p, value)
//
////! Atomically compute (old ^ value)
//#define zAtomicXorU(p, value) atomic_xor(p, value)
//
//#if defined(Z_MAC) && defined(ZINVUL_VULKAN)
//
////! Atomically do an expression
//#define zAtomicExpression(p, expression, old) \
//  { \
//    old = p[0]; \
//    const int32b c = old; \
//    const int32b v = expression(c); \
//    old = zAtomicCmpxchg(p, c, v); \
//  }
//
////! Atomically do an expression
//#define zAtomicExpressionU(p, expression, old) \
//  { \
//    old = p[0]; \
//    const uint32b c = old; \
//    const uint32b v = expression(c); \
//    old = zAtomicCmpxchgU(p, c, v); \
//  }
//
//#else
//
////! Atomically do an expression
//#define zAtomicExpression(p, expression, old) \
//  { \
//    old = p[0]; \
//    int32b c = 0u; \
//    do { \
//      c = old; \
//      const int32b v = expression(c); \
//      old = zAtomicCmpxchg(p, c, v); \
//    } while (old != c); \
//  }
//
////! Atomically do an expression
//#define zAtomicExpressionU(p, expression, old) \
//  { \
//    old = p[0]; \
//    uint32b c = 0u; \
//    do { \
//      c = old; \
//      const uint32b v = expression(c); \
//      old = zAtomicCmpxchgU(p, c, v); \
//    } while (old != c); \
//  }
//
//#endif

namespace zinvul {

/*!
  */
class Atomic
{
 public:
  //! Perform atomic addition
  template <typename Integer>
  static Integer add(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic addition
  template <typename Integer>
  static Integer add(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic subtraction
  template <typename Integer>
  static Integer sub(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic subtraction
  template <typename Integer>
  static Integer sub(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic swapping
  template <typename Type>
  static Type swap(LocalPtr<Type> p, const Type value) noexcept;

  //! Perform atomic swapping
  template <typename Type>
  static Type swap(GlobalPtr<Type> p, const Type value) noexcept;

  //! Perform atomic post-increment
  template <typename Integer>
  static Integer increment(LocalPtr<Integer> p) noexcept;

  //! Perform atomic post-increment
  template <typename Integer>
  static Integer increment(GlobalPtr<Integer> p) noexcept;

  //! Perform atomic post-decrement
  template <typename Integer>
  static Integer decrement(LocalPtr<Integer> p) noexcept;

  //! Perform atomic post-decrement
  template <typename Integer>
  static Integer decrement(GlobalPtr<Integer> p) noexcept;

  //! Perform atomic comparison and swapping
  template <typename Integer>
  static Integer compareAndSwap(LocalPtr<Integer> p,
                                const Integer comp,
                                const Integer value) noexcept;

  //! Perform atomic comparison and swapping
  template <typename Integer>
  static Integer compareAndSwap(GlobalPtr<Integer> p,
                                const Integer comp,
                                const Integer value) noexcept;

  //! Perform atomic min
  template <typename Integer>
  static Integer min(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic min
  template <typename Integer>
  static Integer min(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic max
  template <typename Integer>
  static Integer max(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic max
  template <typename Integer>
  static Integer max(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit and
  template <typename Integer>
  static Integer bitAnd(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit and
  template <typename Integer>
  static Integer bitAnd(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit or
  template <typename Integer>
  static Integer bitOr(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit or
  template <typename Integer>
  static Integer bitOr(GlobalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit xor
  template <typename Integer>
  static Integer bitXor(LocalPtr<Integer> p, const Integer value) noexcept;

  //! Perform atomic bit xor
  template <typename Integer>
  static Integer bitXor(GlobalPtr<Integer> p, const Integer value) noexcept;
};

} // namespace zinvul

#include "atomic-inl.cl"

#endif /* ZINVUL_ATOMIC_CL */
