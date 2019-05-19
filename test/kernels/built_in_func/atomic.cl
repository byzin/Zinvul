/*!
  \file atomic.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUILT_IN_FUNC_TEST_ATOMIC_CL
#define ZINVUL_BUILT_IN_FUNC_TEST_ATOMIC_CL

// Zinvul
#include "zinvul/cl/array.cl"
#include "zinvul/cl/atomic.cl"
#include "zinvul/cl/limits.cl"
#include "zinvul/cl/memory_fence.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using namespace zinvul;

// Forward declaration
__kernel void testLocalMemoryFence(GlobalPtr<uint32b> table,
    GlobalPtr<uint32b> results,
    const uint32b resolution);
__kernel void testAtomicAddGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicAddLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicAddGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicAddLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicAddGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicAddLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicSubGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicSubLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicSubGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicSubLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicSubGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicSubLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicXchgGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicXchgGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicXchgGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicIncGlobal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicIncLocal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicIncGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicIncLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicDecGlobal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicDecLocal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicDecGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicDecLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicCmpxchgGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicCmpxchgLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicCmpxchgGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicCmpxchgLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicCmpxchgGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicCmpxchgLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);
__kernel void testAtomicMinGlobalPositive(GlobalPtr<int32b> result,
    const uint32b resolution);
__kernel void testAtomicMinLocalPositive(GlobalPtr<int32b> result,
    const uint32b resolution);
__kernel void testAtomicMinGlobalNegative(GlobalPtr<int32b> result,
    const uint32b resolution);
__kernel void testAtomicMinLocalNegative(GlobalPtr<int32b> result,
    const uint32b resolution);
__kernel void testAtomicMinGlobalUint(GlobalPtr<uint32b> result,
    const uint32b resolution);
__kernel void testAtomicMinLocalUint(GlobalPtr<uint32b> result,
    const uint32b resolution);
__kernel void testAtomicMaxGlobalPositive(GlobalPtr<int32b> result,
    const uint32b resolution);
__kernel void testAtomicMaxLocalPositive(GlobalPtr<int32b> result,
    const uint32b resolution);
__kernel void testAtomicMaxGlobalNegative(GlobalPtr<int32b> result,
    const uint32b resolution);
__kernel void testAtomicMaxLocalNegative(GlobalPtr<int32b> result,
    const uint32b resolution);
__kernel void testAtomicMaxGlobalUint(GlobalPtr<uint32b> result,
    const uint32b resolution);;
__kernel void testAtomicMaxLocalUint(GlobalPtr<uint32b> result,
    const uint32b resolution);
__kernel void testAtomicAndGlobal(GlobalPtr<int32b> result);
__kernel void testAtomicAndGlobalUint(GlobalPtr<uint32b> result);
__kernel void testAtomicOrGlobal(GlobalPtr<int32b> result);
__kernel void testAtomicOrGlobalUint(GlobalPtr<uint32b> result);


/*!
  */
__kernel void testLocalMemoryFence(GlobalPtr<uint32b> table,
    GlobalPtr<uint32b> results,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();

  constexpr size_t cache_size = 128;
  Local<Array<uint32b, cache_size>> cache;

  // Initialize the local cache
  const uint32b local_index = getLocalIdX();
  {
    cache[local_index] = local_index + 1;
    MemoryFence::performLocalBarrier();
  }

  if (index < resolution) {
    table[index] = cache[local_index];

    uint32b sum = 0;
    for (size_t i = 0; i < getLocalSizeX(); ++i)
      sum += cache[i];
    results[index] = sum;
  }
}

/*!
  */
__kernel void testAtomicAddGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(Atomic::add(result, 1));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicAddLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(Atomic::add(p, 1));
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::add(result, c);
}

/*!
  */
__kernel void testAtomicAddGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(-Atomic::add(result, -1));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicAddLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(-Atomic::add(p, -1));
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::add(result, c);
}

/*!
  */
__kernel void testAtomicAddGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(Atomic::add(result, 1u));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicAddLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const size_t local_id = cast<size_t>(Atomic::add(p, 1u));
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::add(result, c);
}

/*!
  */
__kernel void testAtomicSubGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(-Atomic::sub(result, 1));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicSubLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(-Atomic::sub(p, 1));
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::sub(result, -c);
}

/*!
  */
__kernel void testAtomicSubGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(Atomic::sub(result, -1));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicSubLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(Atomic::sub(p, -1));
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::sub(result, -c);
}

/*!
  */
__kernel void testAtomicSubGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(Atomic::sub(result, 1u)) - 1u;
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicSubLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = getLocalSizeX();
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const size_t local_id = getLocalSizeX() - cast<size_t>(Atomic::sub(p, 1u));
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::sub(result, getLocalSizeX() - c);
}

/*!
  */
__kernel void testAtomicXchgGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const int32b v = cast<int32b>(index + 1);
    const size_t i = cast<size_t>(Atomic::swap(result, v));
    if (i < resolution)
      table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicXchgGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const int32b v = -cast<int32b>(index + 1);
    const size_t i = cast<size_t>(-Atomic::swap(result, v));
    if (i < resolution)
      table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicXchgGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const uint32b v = index + 1;
    const size_t i = cast<size_t>(Atomic::swap(result, v));
    if (i < resolution)
      table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicIncGlobal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(Atomic::increment(result));
    table[i] = 1;
  }
}

__kernel void testAtomicIncLocal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    // \todo Segmentation fault happens with clspv
    // const size_t local_id = cast<size_t>(Atomic::increment(p));
    const size_t local_id = 0;
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::add(result, c);
}

/*!
  */
__kernel void testAtomicIncGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(Atomic::increment(result));
    table[i] = 1;
  }
}

__kernel void testAtomicIncLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    // \todo Segmentation fault happens with clspv
    // const size_t local_id = cast<size_t>(Atomic::increment(p));
    const size_t local_id = 0;
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::add(result, c);
}

__kernel void testAtomicDecGlobal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(-Atomic::decrement(result));
    table[i] = 1;
  }
}

__kernel void testAtomicDecLocal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    // \todo Segmentation fault happens with clspv
    // const size_t local_id = cast<size_t>(-Atomic::decrement(p));
    const size_t local_id = 0;
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::sub(result, -c);
}

__kernel void testAtomicDecGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(Atomic::decrement(result)) - 1u;
    table[i] = 1;
  }
}

__kernel void testAtomicDecLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = getLocalSizeX();
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    // \todo Segmentation fault happens with clspv
    // const size_t local_id = getLocalSizeX() - cast<size_t>(Atomic::decrement(p));
    const size_t local_id = 0;
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::sub(result, getLocalSizeX() - c);
}

__kernel void testAtomicCmpxchgGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    int32b i = *result;
    int32b c = 0;
    do {
      c = i;
      const int32b v = c + 1;
      i = Atomic::compareAndSwap(result, c, v);
    } while (i != c);
    table[cast<size_t>(i)] = 1;
  }
}

__kernel void testAtomicCmpxchgLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    int32b idx = *p;
    int32b cmp = 0;
    do {
      cmp = idx;
      const int32b v = cmp + 1;
      // \todo Segmentation fault happens with clspv
      // idx = Atomic::compareAndSwap(p, cmp, v);
    } while (idx != cmp);
    const size_t local_id = cast<size_t>(idx);
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::add(result, c);
}

__kernel void testAtomicCmpxchgGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    int32b i = *result;
    int32b c = 0;
    do {
      c = i;
      const int32b v = c - 1;
      i = Atomic::compareAndSwap(result, c, v);
    } while (i != c);
    table[cast<size_t>(-i)] = 1;
  }
}

__kernel void testAtomicCmpxchgLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    int32b idx = *p;
    int32b cmp = 0;
    do {
      cmp = idx;
      const int32b v = cmp - 1;
      // \todo Segmentation fault happens with clspv
      // idx = Atomic::compareAndSwap(p, cmp, v);
    } while (idx != cmp);
    const size_t local_id = cast<size_t>(-idx);
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::sub(result, c);
}

__kernel void testAtomicCmpxchgGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    uint32b i = *result;
    uint32b c = 0;
    do {
      c = i;
      const uint32b v = c + 1;
      i = Atomic::compareAndSwap(result, c, v);
    } while (i != c);
    table[i] = 1;
  }
}

__kernel void testAtomicCmpxchgLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    uint32b idx = *p;
    uint32b cmp = 0;
    do {
      cmp = idx;
      const uint32b v = cmp + 1;
      // \todo Segmentation fault happens with clspv
      // idx = Atomic::compareAndSwap(p, cmp, v);
    } while (idx != cmp);
    const size_t local_id = cast<size_t>(idx);
    const size_t i = getGroupIdX() * getLocalSizeX() + local_id;
    table[i] = 1;
  }
  MemoryFence::performBarrier();

  if (local_index == 0)
    Atomic::add(result, c);
}

/*!
  */
__kernel void testAtomicMinGlobalPositive(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const int32b v = cast<int32b>(index);
    Atomic::min(result, v);
  }
}

__kernel void testAtomicMinLocalPositive(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = NumericLimits<int32b>::max();
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const int32b i = cast<int32b>(getGroupIdX() * getLocalSizeX() + local_index);
    Atomic::min(p, i);
  }
  MemoryFence::performBarrier();

  if ((local_index == 0) && (c != NumericLimits<int32b>::max()))
    Atomic::min(result, c);
}

__kernel void testAtomicMinGlobalNegative(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const int32b v = -cast<int32b>(index);
    Atomic::min(result, v);
  }
}

__kernel void testAtomicMinLocalNegative(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 1;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const int32b i = -cast<int32b>(getGroupIdX() * getLocalSizeX() + local_index);
    Atomic::min(p, i);
  }
  MemoryFence::performBarrier();

  if ((local_index == 0) && (c != 1))
    Atomic::min(result, c);
}

__kernel void testAtomicMinGlobalUint(GlobalPtr<uint32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const uint32b v = index;
    Atomic::min(result, v);
  }
}

__kernel void testAtomicMinLocalUint(GlobalPtr<uint32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = NumericLimits<uint32b>::max();
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const uint32b i = getGroupIdX() * getLocalSizeX() + local_index;
    Atomic::min(p, i);
  }
  MemoryFence::performBarrier();

  if ((local_index == 0) && (c != NumericLimits<uint32b>::max()))
    Atomic::min(result, c);
}

__kernel void testAtomicMaxGlobalPositive(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const int32b v = cast<int32b>(index);
    Atomic::max(result, v);
  }
}

__kernel void testAtomicMaxLocalPositive(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = -1;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const int32b i = cast<int32b>(getGroupIdX() * getLocalSizeX() + local_index);
    Atomic::max(p, i);
  }
  MemoryFence::performBarrier();

  if ((local_index == 0) && (c != -1))
    Atomic::max(result, c);
}

__kernel void testAtomicMaxGlobalNegative(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const int32b v = -cast<int32b>(index);
    Atomic::max(result, v);
  }
}

__kernel void testAtomicMaxLocalNegative(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = NumericLimits<int32b>::min();
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const int32b i = -cast<int32b>(getGroupIdX() * getLocalSizeX() + local_index);
    Atomic::max(p, i);
  }
  MemoryFence::performBarrier();

  if ((local_index == 0) && (c != NumericLimits<int32b>::min()))
    Atomic::max(result, c);
}

__kernel void testAtomicMaxGlobalUint(GlobalPtr<uint32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const uint32b v = index;
    Atomic::max(result, v);
  }
}

__kernel void testAtomicMaxLocalUint(GlobalPtr<uint32b> result,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = getLocalIdX();
  if (local_index == 0)
    c = 0;
  MemoryFence::performLocalBarrier();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const uint32b i = getGroupIdX() * getLocalSizeX() + local_index;
    Atomic::max(p, i);
  }
  MemoryFence::performBarrier();

  if ((local_index == 0) && (c != 0))
    Atomic::max(result, c);
}

__kernel void testAtomicAndGlobal(GlobalPtr<int32b> result)
{
  const uint32b index = getGlobalIdX();
  if (index < 32) {
    const int32b v = ~(1 << cast<int32b>(index));
    Atomic::bitAnd(result, v);
  }
}

__kernel void testAtomicAndGlobalUint(GlobalPtr<uint32b> result)
{
  const uint32b index = getGlobalIdX();
  if (index < 32) {
    const uint32b v = ~(1u << index);
    Atomic::bitAnd(result, v);
  }
}

__kernel void testAtomicOrGlobal(GlobalPtr<int32b> result)
{
  const uint32b index = getGlobalIdX();
  if (index < 32) {
    const int32b v = 1 << cast<int32b>(index);
    Atomic::bitOr(result, v);
  }
}

__kernel void testAtomicOrGlobalUint(GlobalPtr<uint32b> result)
{
  const uint32b index = getGlobalIdX();
  if (index < 32) {
    const uint32b v = 1 << index;
    Atomic::bitOr(result, v);
  }
}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_ATOMIC_CL */
