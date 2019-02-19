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
#include "utility.cl"

//! Perform atomic addition
#define zAtomicAdd(p, value) atomic_add(p, value)

//! Perform atomic addition
#define zAtomicAddU(p, value) atomic_add(p, value)

//! Perform atomic subtraction
#define zAtomicSub(p, value) atomic_sub(p, value)

//! Perform atomic subtraction
#define zAtomicSubU(p, value) atomic_sub(p, value)

//! Atomically replace the value
#define zAtomicXchg(p, value) atomic_xchg(p, value)

//! Atomically replace the value
#define zAtomicXchgU(p, value) atomic_xchg(p, value)

//! Perform atomic post-increment
#define zAtomicInc(p) atomic_inc(p)

//! Perform atomic post-increment
#define zAtomicIncU(p) atomic_inc(p)

//! Perform atomic post-decrement
#define zAtomicDec(p) atomic_dec(p)

//! Perform atomic post-decrement
#define zAtomicDecU(p) atomic_dec(p)

//! Atomically compare the values and exchange if equal
#define zAtomicCmpxchg(p, cmp, value) atomic_cmpxchg(p, cmp, value)

//! Atomically compare the values and exchange if equal
#define zAtomicCmpxchgU(p, cmp, value) atomic_cmpxchg(p, cmp, value)

//! Atomically compute min(old, value)
#define zAtomicMin(p, value) atomic_min(p, value)

//! Atomically compute min(old, value)
#define zAtomicMinU(p, value) atomic_min(p, value)

//! Atomically compute max(old, value)
#define zAtomicMax(p, value) atomic_max(p, value)

//! Atomically compute max(old, value)
#define zAtomicMaxU(p, value) atomic_max(p, value)

//! Atomically compute (old & value)
#define zAtomicAnd(p, value) atomic_and(p, value)

//! Atomically compute (old & value)
#define zAtomicAndU(p, value) atomic_and(p, value)

//! Atomically compute (old | value)
#define zAtomicOr(p, value) atomic_or(p, value)

//! Atomically compute (old | value)
#define zAtomicOrU(p, value) atomic_or(p, value)

//! Atomically compute (old ^ value)
#define zAtomicXor(p, value) atomic_xor(p, value)

//! Atomically compute (old ^ value)
#define zAtomicXorU(p, value) atomic_xor(p, value)

#endif /* ZINVUL_ATOMIC_CL */
