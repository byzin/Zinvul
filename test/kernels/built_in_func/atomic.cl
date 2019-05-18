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

///*!
//  */
//kernel void initAtomicTest(global int32b* add_table, global int32b* add_result,
//    global int32b* sub_table, global int32b* sub_result,
//    global int32b* xchg_table, global int32b* xchg_result,
//    global int32b* inc_table, global int32b* inc_result,
//    global int32b* dec_table, global int32b* dec_result,
//    global int32b* cmpxchg_table, global int32b* cmpxchg_result,
//    global int32b* min_table, global int32b* min_result,
//    global int32b* max_table, global int32b* max_result,
//    global int32b* and_table, global int32b* and_result,
//    global int32b* or_table, global int32b* or_result,
//    global int32b* xor_table, global int32b* xor_result,
//    const uint32b resolution)
//{
//  const uint index = zGetGlobalIdX();
//  if (index == 0) {
//    for (uint32b i = 0; i < resolution; ++i) {
//      add_table[i] = 0;
//      sub_table[i] = 0;
//      xchg_table[i] = 0;
//      inc_table[i] = 0;
//      dec_table[i] = 0;
//      cmpxchg_table[i] = 0;
//      min_table[i] = 0;
//      max_table[i] = 0;
//      and_table[i] = 0;
//      or_table[i] = 0;
//      xor_table[i] = 0;
//    }
//    add_result[0] = 0;
//    sub_result[0] = 0;
//    xchg_result[0] = -1;
//    inc_result[0] = 0;
//    dec_result[0] = 0;
//    cmpxchg_result[0] = 0;
//    min_result[0] = 1;
//    max_result[0] = 0;
//    and_result[0] = 0;
//    or_result[0] = 0;
//    xor_result[0] = 0;
//  }
//}
//
///*!
//  */
//kernel void testAtomic(global int32b* add_table, global int32b* add_result,
//    global int32b* sub_table, global int32b* sub_result,
//    global int32b* xchg_table, global int32b* xchg_result,
//    global int32b* inc_table, global int32b* inc_result,
//    global int32b* dec_table, global int32b* dec_result,
//    global int32b* cmpxchg_table, global int32b* cmpxchg_result,
//    global int32b* min_table, global int32b* min_result,
//    global int32b* max_table, global int32b* max_result,
//    global int32b* and_table, global int32b* and_result,
//    global int32b* or_table, global int32b* or_result,
//    global int32b* xor_table, global int32b* xor_result,
//    const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    // atomic_add
//    {
//      const int32b i = atomic_add(add_result, 1);
//      add_table[i] = 1;
//    }
//    // atomic_sub
//    {
//      const int32b i = atomic_sub(sub_result, 1);
//      sub_table[-i] = 1;
//    }
//    // atomic_xchg
//    {
////      atomic_xchg(xchg_result, (int32b)index);
////      xchg_table[index] = 1;
//      (void)xchg_table;
//      (void)xchg_result;
//    }
//    // atomic_inc
//    {
//      const int i = atomic_inc(inc_result);
//      inc_table[i] = 1;
//    }
//    // atomic_dec
//    {
//      const int i = atomic_dec(dec_result);
//      dec_table[-i] = 1;
//    }
//    // cmpxchg
//    {
////      atomic_cmpxchg(cmpxchg_result, 0, (int32b)index);
////      cmpxchg_table[index] = 1;
//      (void)cmpxchg_table;
//      (void)cmpxchg_result;
//    }
//    // atomic_min
//    {
////      const int32b v = (int32b)(index + 1) - (int32b)(resolution >> 1);
////      atomic_min(min_result, v);
////      min_table[index] = 1;
//      (void)min_table;
//      (void)min_result;
//    }
//    // atomic_max
//    {
////      const int32b v = (int32b)(index + 1) - (int32b)(resolution >> 1);
////      atomic_max(max_result, v);
////      max_table[index] = 1;
//      (void)max_table;
//      (void)max_result;
//    }
//    // atomic_and
//    {
////      atomic_and(and_result, (int32b)index);
////      and_table[index] = 1;
//      (void)and_table;
//      (void)and_result;
//    }
//    // atomic_or
//    {
////      atomic_or(or_result, (int32b)index);
////      or_table[index] = 1;
//      (void)or_table;
//      (void)or_result;
//    }
//    // atomic_xor
//    {
////      atomic_xor(xor_result, (int32b)index);
////      xor_table[index] = 1;
//      (void)xor_table;
//      (void)xor_result;
//    }
//  }
//}

/*!
  */
__kernel void testAtomicAddGlobalPositive(GlobalPtr<int32b> result,
    GlobalPtr<int32b> table,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const int32b old = Atomic::add(result, 1);
    const size_t i = cast<size_t>(old);
    table[i] = 1;
  }
}

///*!
//  */
//kernel void testAtomicAddNegative(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = atomic_add(result, -1);
//    table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicAddUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = atomic_add(result, 1u);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicAddPositive(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = zAtomicAdd(result, 1);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicAddNegative(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = zAtomicAdd(result, -1);
//    table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicAddUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = zAtomicAddU(result, 1u);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicSubPositive(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = atomic_sub(result, 1);
//    table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicSubNegative(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = atomic_sub(result, -1);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicSubUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = atomic_sub(result, 1u);
//    table[i - 1] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicSubPositive(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = zAtomicSub(result, 1);
//    table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicSubNegative(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = zAtomicSub(result, -1);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicSubUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = zAtomicSubU(result, 1u);
//    table[i - 1] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicXchgPositive(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = (int)(index + 1);
//    const int32b i = atomic_xchg(result, v);
//    if (i < (int)resolution)
//      table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicXchgNegative(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = -(int)(index + 1);
//    const int32b i = atomic_xchg(result, v);
//    if (-(int)resolution < i)
//      table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicXchgUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b v = index + 1;
//    const uint32b i = atomic_xchg(result, v);
//    if (i < resolution)
//      table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicXchgPositive(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = (int)(index + 1);
//    const int32b i = zAtomicXchg(result, v);
//    if (i < (int)resolution)
//      table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicXchgNegative(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = -(int)(index + 1);
//    const int32b i = zAtomicXchg(result, v);
//    if (-(int)resolution < i)
//      table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicXchgUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b v = index + 1;
//    const uint32b i = zAtomicXchgU(result, v);
//    if (i < resolution)
//      table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicInc(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = atomic_inc(result);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicIncUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = atomic_inc(result);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicInc(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = zAtomicInc(result);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicIncUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = zAtomicIncU(result);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicDec(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = atomic_dec(result);
//    table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicDecUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = atomic_dec(result);
//    table[i - 1] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicDec(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b i = zAtomicDec(result);
//    table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicDecUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = zAtomicDecU(result);
//    table[i - 1] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicCmpxchgPositive(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    int32b i = *result;
//    int32b c = 0;
//    do {
//      c = i;
//      const int32b v = c + 1;
//      i = atomic_cmpxchg(result, c, v);
//    } while (i != c);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicCmpxchgNegative(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    int32b i = *result;
//    int32b c = 0;
//    do {
//      c = i;
//      const int32b v = c - 1;
//      i = atomic_cmpxchg(result, c, v);
//    } while (i != c);
//    table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicCmpxchgUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    uint32b i = *result;
//    uint32b c = 0;
//    do {
//      c = i;
//      const uint32b v = c + 1;
//      i = atomic_cmpxchg(result, c, v);
//    } while (i != c);
//    table[i] = 1;
//  }
//}
//
//#define ZINVUL_TEST_INCREMENT(value) (value + 1)
//
///*!
//  */
//kernel void testzAtomicCmpxchgPositive(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    int32b i = 0;
//    zAtomicExpression(result, ZINVUL_TEST_INCREMENT, i);
//    table[i] = 1;
//  }
//}
//
//#define ZINVUL_TEST_DECREMENT(value) (value - 1)
//
///*!
//  */
//kernel void testzAtomicCmpxchgNegative(global int32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    int32b i = 0;
//    zAtomicExpression(result, ZINVUL_TEST_DECREMENT, i);
//    table[-i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testzAtomicCmpxchgUint(global uint32b* result, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    uint32b i = 0;
//    zAtomicExpressionU(result, ZINVUL_TEST_INCREMENT, i);
//    table[i] = 1;
//  }
//}
//
///*!
//  */
//kernel void testAtomicMinPositive(global int32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = (int32b)index;
//    atomic_min(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicMinNegative(global int32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = -(int32b)index;
//    atomic_min(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicMinUint(global uint32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b v = index;
//    atomic_min(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicMinPositive(global int32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = (int32b)index;
//    zAtomicMin(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicMinNegative(global int32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = -(int32b)index;
//    zAtomicMin(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicMinUint(global uint32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b v = index;
//    zAtomicMinU(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicMaxPositive(global int32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = (int32b)index;
//    atomic_max(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicMaxNegative(global int32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = -(int32b)index;
//    atomic_max(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicMaxUint(global uint32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b v = index;
//    atomic_max(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicMaxPositive(global int32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = (int32b)index;
//    zAtomicMax(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicMaxNegative(global int32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const int32b v = -(int32b)index;
//    zAtomicMax(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicMaxUint(global uint32b* result, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b v = index;
//    zAtomicMaxU(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicAnd(global int32b* result)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < 32) {
//    const int32b v = ~(1 << (int32b)index);
//    atomic_and(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicAndUint(global uint32b* result)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < 32) {
//    const uint32b v = ~(1u << index);
//    atomic_and(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicAnd(global int32b* result)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < 32) {
//    const int32b v = ~(1 << (int32b)index);
//    zAtomicAnd(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicAndUint(global uint32b* result)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < 32) {
//    const uint32b v = ~(1u << index);
//    zAtomicAndU(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicOr(global int32b* result)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < 32) {
//    const int32b v = 1 << (int32b)index;
//    atomic_or(result, v);
//  }
//}
//
///*!
//  */
//kernel void testAtomicOrUint(global uint32b* result)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < 32) {
//    const uint32b v = 1 << index;
//    atomic_or(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicOr(global int32b* result)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < 32) {
//    const int32b v = 1 << (int32b)index;
//    zAtomicOr(result, v);
//  }
//}
//
///*!
//  */
//kernel void testzAtomicOrUint(global uint32b* result)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < 32) {
//    const uint32b v = 1 << index;
//    zAtomicOrU(result, v);
//  }
//}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_ATOMIC_CL */
