/*!
  \file atomic.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ATOMIC_TEST_ATOMIC_CL
#define ZINVUL_ATOMIC_TEST_ATOMIC_CL

// Zinvul
#include "zinvul/cl/array.cl"
#include "zinvul/cl/atomic.cl"
#include "zinvul/cl/limits.cl"
#include "zinvul/cl/synchronization.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconditional-uninitialized"

using zinvul::int8b;
using zinvul::int16b;
using zinvul::int32b;
using zinvul::int64b;
using zinvul::uint8b;
using zinvul::uint16b;
using zinvul::uint32b;
using zinvul::uint64b;
using zinvul::GlobalPtr;
using zinvul::ConstGlobalPtr;
using zinvul::ConstantPtr;
using zinvul::ConstConstantPtr;
using zinvul::Local;
using zinvul::LocalPtr;
using zinvul::ConstLocalPtr;
using zinvul::cast;
using zinvul::treatAs;

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
    const uint32b resolution);
__kernel void testAtomicMaxLocalUint(GlobalPtr<uint32b> result,
    const uint32b resolution);
__kernel void testAtomicAndGlobal(GlobalPtr<int32b> result);
__kernel void testAtomicAndGlobalUint(GlobalPtr<uint32b> result);
__kernel void testAtomicOrGlobal(GlobalPtr<int32b> result);
__kernel void testAtomicOrGlobalUint(GlobalPtr<uint32b> result);
__kernel void testAtomicFloatIncGlobal(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution);


/*!
  */
__kernel void testLocalMemoryFence(GlobalPtr<uint32b> table,
    GlobalPtr<uint32b> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();

  constexpr size_t cache_size = 128;
  Local<zinvul::Array<uint32b, cache_size>> cache;

  // Initialize the local cache
  const uint32b local_index = zinvul::getLocalIdX();
  {
    cache[local_index] = local_index + 1;
    zinvul::Synchronization::barrierLocal();
  }

  if (index < resolution) {
    table[index] = cache[local_index];

    uint32b sum = 0;
    for (size_t i = 0; i < zinvul::getLocalSizeX(); ++i)
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
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(zinvul::atomic_add(result, 1));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicAddLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(zinvul::atomic_add(p, 1));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_add(result, c);
}

/*!
  */
__kernel void testAtomicAddGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(-zinvul::atomic_add(result, -1));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicAddLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(-zinvul::atomic_add(p, -1));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_add(result, c);
}

/*!
  */
__kernel void testAtomicAddGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(zinvul::atomic_add(result, 1u));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicAddLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const size_t local_id = cast<size_t>(zinvul::atomic_add(p, 1u));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_add(result, c);
}

/*!
  */
__kernel void testAtomicSubGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(-zinvul::atomic_sub(result, 1));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicSubLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(-zinvul::atomic_sub(p, 1));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_sub(result, -c);
}

/*!
  */
__kernel void testAtomicSubGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(zinvul::atomic_sub(result, -1));
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicSubLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(zinvul::atomic_sub(p, -1));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_sub(result, -c);
}

/*!
  */
__kernel void testAtomicSubGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(zinvul::atomic_sub(result, 1u)) - 1u;
    table[i] = 1;
  }
}

/*!
  */
__kernel void testAtomicSubLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = zinvul::getLocalSizeX();
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const size_t local_id = zinvul::getLocalSizeX() - cast<size_t>(zinvul::atomic_sub(p, 1u));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_sub(result, zinvul::getLocalSizeX() - c);
}

/*!
  */
__kernel void testAtomicXchgGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const int32b v = cast<int32b>(index + 1);
    const size_t i = cast<size_t>(zinvul::atomic_xchg(result, v));
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
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const int32b v = -cast<int32b>(index + 1);
    const size_t i = cast<size_t>(-zinvul::atomic_xchg(result, v));
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
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const uint32b v = index + 1;
    const size_t i = cast<size_t>(zinvul::atomic_xchg(result, v));
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
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(zinvul::atomic_inc(result));
    table[i] = 1;
  }
}

__kernel void testAtomicIncLocal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(zinvul::atomic_inc(p));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_add(result, c);
}

/*!
  */
__kernel void testAtomicIncGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(zinvul::atomic_inc(result));
    table[i] = 1;
  }
}

__kernel void testAtomicIncLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const size_t local_id = cast<size_t>(zinvul::atomic_inc(p));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_add(result, c);
}

__kernel void testAtomicDecGlobal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(-zinvul::atomic_dec(result));
    table[i] = 1;
  }
}

__kernel void testAtomicDecLocal(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(-zinvul::atomic_dec(p));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_sub(result, -c);
}

__kernel void testAtomicDecGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const size_t i = cast<size_t>(zinvul::atomic_dec(result)) - 1u;
    table[i] = 1;
  }
}

__kernel void testAtomicDecLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = zinvul::getLocalSizeX();
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const size_t local_id = zinvul::getLocalSizeX() - cast<size_t>(zinvul::atomic_dec(p));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_sub(result, zinvul::getLocalSizeX() - c);
}

__kernel void testAtomicCmpxchgGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const auto inc = [](const int32b p)
    {
      return p + 1;
    };
    const size_t i = cast<size_t>(zinvul::Atomic::perform(result, inc));
    table[i] = 1;
  }
}

__kernel void testAtomicCmpxchgLocalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    const auto inc = [](const int32b p)
    {
      const int32b r = p + 1;
      return r;
    };
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(zinvul::Atomic::perform(p, inc));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_add(result, c);
}

__kernel void testAtomicCmpxchgGlobalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const auto sub = [](const int32b p, const int32b value)
    {
      const int32b r = p - value;
      return r;
    };
    const size_t i = cast<size_t>(-zinvul::Atomic::perform(result, sub, 1));
    table[i] = 1;
  }
}

__kernel void testAtomicCmpxchgLocalNegative(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    const auto sub = [](const int32b p, const int32b value)
    {
      const int32b r = p - value;
      return r;
    };
    LocalPtr<int32b> p = &c;
    const size_t local_id = cast<size_t>(-zinvul::Atomic::perform(p, sub, 1));
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_sub(result, c);
}

__kernel void testAtomicCmpxchgGlobalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const auto add = [](const uint32b p, const uint32b value)
    {
      const uint32b r = p + value;
      return r;
    };
    const size_t i = zinvul::Atomic::perform(result, add, 1u);
    table[i] = 1;
  }
}

__kernel void testAtomicCmpxchgLocalUint(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    const auto add = [](const uint32b p, const uint32b value)
    {
      const uint32b r = p + value;
      return r;
    };
    LocalPtr<uint32b> p = &c;
    const size_t local_id = zinvul::Atomic::perform(p, add, 1u);
    const size_t i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_id;
    table[i] = 1;
  }
  zinvul::Synchronization::barrierAll();

  if (local_index == 0)
    zinvul::atomic_add(result, c);
}

/*!
  */
__kernel void testAtomicMinGlobalPositive(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const int32b v = cast<int32b>(index);
    zinvul::atomic_min(result, v);
  }
}

__kernel void testAtomicMinLocalPositive(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = zinvul::NumericLimits<int32b>::max();
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const int32b i = cast<int32b>(zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_index);
    zinvul::atomic_min(p, i);
  }
  zinvul::Synchronization::barrierAll();

  if ((local_index == 0) && (c != zinvul::NumericLimits<int32b>::max()))
    zinvul::atomic_min(result, c);
}

__kernel void testAtomicMinGlobalNegative(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const int32b v = -cast<int32b>(index);
    zinvul::atomic_min(result, v);
  }
}

__kernel void testAtomicMinLocalNegative(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 1;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const int32b i = -cast<int32b>(zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_index);
    zinvul::atomic_min(p, i);
  }
  zinvul::Synchronization::barrierAll();

  if ((local_index == 0) && (c != 1))
    zinvul::atomic_min(result, c);
}

__kernel void testAtomicMinGlobalUint(GlobalPtr<uint32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const uint32b v = index;
    zinvul::atomic_min(result, v);
  }
}

__kernel void testAtomicMinLocalUint(GlobalPtr<uint32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = zinvul::NumericLimits<uint32b>::max();
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const uint32b i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_index;
    zinvul::atomic_min(p, i);
  }
  zinvul::Synchronization::barrierAll();

  if ((local_index == 0) && (c != zinvul::NumericLimits<uint32b>::max()))
    zinvul::atomic_min(result, c);
}

__kernel void testAtomicMaxGlobalPositive(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const int32b v = cast<int32b>(index);
    zinvul::atomic_max(result, v);
  }
}

__kernel void testAtomicMaxLocalPositive(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = -1;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const int32b i = cast<int32b>(zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_index);
    zinvul::atomic_max(p, i);
  }
  zinvul::Synchronization::barrierAll();

  if ((local_index == 0) && (c != -1))
    zinvul::atomic_max(result, c);
}

__kernel void testAtomicMaxGlobalNegative(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const int32b v = -cast<int32b>(index);
    zinvul::atomic_max(result, v);
  }
}

__kernel void testAtomicMaxLocalNegative(GlobalPtr<int32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<int32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = zinvul::NumericLimits<int32b>::min();
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<int32b> p = &c;
    const int32b i = -cast<int32b>(zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_index);
    zinvul::atomic_max(p, i);
  }
  zinvul::Synchronization::barrierAll();

  if ((local_index == 0) && (c != zinvul::NumericLimits<int32b>::min()))
    zinvul::atomic_max(result, c);
}

__kernel void testAtomicMaxGlobalUint(GlobalPtr<uint32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const uint32b v = index;
    zinvul::atomic_max(result, v);
  }
}

__kernel void testAtomicMaxLocalUint(GlobalPtr<uint32b> result,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  Local<uint32b> c;

  // Initialize a local variable
  const uint32b local_index = zinvul::getLocalIdX();
  if (local_index == 0)
    c = 0;
  zinvul::Synchronization::barrierLocal();

  if (index < resolution) {
    LocalPtr<uint32b> p = &c;
    const uint32b i = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + local_index;
    zinvul::atomic_max(p, i);
  }
  zinvul::Synchronization::barrierAll();

  if ((local_index == 0) && (c != 0))
    zinvul::atomic_max(result, c);
}

__kernel void testAtomicAndGlobal(GlobalPtr<int32b> result)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < 32) {
    const int32b v = ~(1 << cast<int32b>(index));
    zinvul::atomic_and(result, v);
  }
}

__kernel void testAtomicAndGlobalUint(GlobalPtr<uint32b> result)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < 32) {
    const uint32b v = ~(1u << index);
    zinvul::atomic_and(result, v);
  }
}

__kernel void testAtomicOrGlobal(GlobalPtr<int32b> result)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < 32) {
    const int32b v = 1 << cast<int32b>(index);
    zinvul::atomic_or(result, v);
  }
}

__kernel void testAtomicOrGlobalUint(GlobalPtr<uint32b> result)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < 32) {
    const uint32b v = 1 << index;
    zinvul::atomic_or(result, v);
  }
}

__kernel void testAtomicFloatIncGlobal(GlobalPtr<uint32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const auto add = [](const uint32b p, const float value)
    {
      const float r = treatAs<float>(p) + value;
      return treatAs<uint32b>(r);
    };
    const float old = treatAs<float>(zinvul::Atomic::perform(result, add, 1.0f));
    const size_t i = cast<size_t>(old);
    table[i] = 1;
  }
}

#pragma clang diagnostic pop

#endif /* ZINVUL_ATOMIC_TEST_ATOMIC_CL */
