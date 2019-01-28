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
int32b zAtomicAdd(__global int32b* p, const int32b value)
{
  const int32b old = atomic_add(p, value);
  return old;
}

//! Perform atomic addition
uint32b zAtomicAddU(__global uint32b* p, const uint32b value)
{
  const uint32b old = atomic_add(p, value);
  return old;
}

//! Perform atomic subtraction
int32b zAtomicSub(__global int32b* p, const int32b value)
{
  const int32b old = atomic_sub(p, value);
  return old;
}

//! Perform atomic subtraction
uint32b zAtomicSubU(__global uint32b* p, const uint32b value)
{
  const uint32b old = atomic_sub(p, value);
  return old;
}

//! Atomically replace the value
int32b zAtomicXchg(__global int32b* p, const int32b value)
{
  const int32b old = atomic_xchg(p, value);
  return old;
}

//! Atomically replace the value
uint32b zAtomicXchgU(__global uint32b* p, const uint32b value)
{
  const uint32b old = atomic_xchg(p, value);
  return old;
}

//! Perform atomic post-increment
int32b zAtomicInc(__global int32b* p)
{
  const int32b old = atomic_inc(p);
  return old;
}

//! Perform atomic post-increment
uint32b zAtomicIncU(__global uint32b* p)
{
  const uint32b old = atomic_inc(p);
  return old;
}

//! Perform atomic post-decrement
int32b zAtomicDec(__global int32b* p)
{
  const int32b old = atomic_dec(p);
  return old;
}

//! Perform atomic post-decrement
uint32b zAtomicDecU(__global uint32b* p)
{
  const uint32b old = atomic_dec(p);
  return old;
}

//! Atomically compare the values and exchange if equal
int32b zAtomicCmpxchg(__global int32b* p, const int32b cmp, const int32b value)
{
  const int32b old = atomic_cmpxchg(p, cmp, value);
  return old;
}

//! Atomically compare the values and exchange if equal
uint32b zAtomicCmpxchgU(__global uint32b* p, const uint32b cmp, const uint32b value)
{
  const uint32b old = atomic_cmpxchg(p, cmp, value);
  return old;
}

//! Atomically compute min(old, value)
int32b zAtomicMin(__global int32b* p, const int32b value)
{
  const int32b old = atomic_min(p, value);
  return old;
}

//! Atomically compute min(old, value)
uint32b zAtomicMinU(__global uint32b* p, const uint32b value)
{
  const uint32b old = atomic_min(p, value);
  return old;
}

//! Atomically compute max(old, value)
int32b zAtomicMax(__global int32b* p, const int32b value)
{
  const int32b old = atomic_max(p, value);
  return old;
}

//! Atomically compute max(old, value)
uint32b zAtomicMaxU(__global uint32b* p, const uint32b value)
{
  const uint32b old = atomic_max(p, value);
  return old;
}

//! Atomically compute (old & value)
int32b zAtomicAnd(__global int32b* p, const int32b value)
{
  const int32b old = atomic_and(p, value);
  return old;
}

//! Atomically compute (old & value)
uint32b zAtomicAndU(__global uint32b* p, const uint32b value)
{
  const uint32b old = atomic_and(p, value);
  return old;
}

//! Atomically compute (old | value)
int32b zAtomicOr(__global int32b* p, const int32b value)
{
  const int32b old = atomic_or(p, value);
  return old;
}

//! Atomically compute (old | value)
uint32b zAtomicOrU(__global uint32b* p, const uint32b value)
{
  const uint32b old = atomic_or(p, value);
  return old;
}

//! Atomically compute (old ^ value)
int32b zAtomicXor(__global int32b* p, const int32b value)
{
  const int32b old = atomic_xor(p, value);
  return old;
}

//! Atomically compute (old ^ value)
uint32b zAtomicXorU(__global uint32b* p, const uint32b value)
{
  const uint32b old = atomic_xor(p, value);
  return old;
}

#endif /* ZINVUL_ATOMIC_CL */
