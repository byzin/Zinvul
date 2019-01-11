/*!
  \file atomic.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUILT_IN_FUNC_TEST_ATOMIC_CL
#define ZINVUL_BUILT_IN_FUNC_TEST_ATOMIC_CL

// Zinvul
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

/*!
  */
kernel void initAtomicTest(global int32b* add_table, global int32b* add_result,
    global int32b* sub_table, global int32b* sub_result,
    global int32b* xchg_table, global int32b* xchg_result,
    global int32b* inc_table, global int32b* inc_result,
    global int32b* dec_table, global int32b* dec_result,
    global int32b* cmpxchg_table, global int32b* cmpxchg_result,
    global int32b* min_table, global int32b* min_result,
    global int32b* max_table, global int32b* max_result,
    global int32b* and_table, global int32b* and_result,
    global int32b* or_table, global int32b* or_result,
    global int32b* xor_table, global int32b* xor_result,
    const uint32b resolution)
{
  const uint index = zGetGlobalIdX();
  if (index == 0) {
    for (uint32b i = 0; i < resolution; ++i) {
      add_table[i] = 0;
      sub_table[i] = 0;
      xchg_table[i] = 0;
      inc_table[i] = 0;
      dec_table[i] = 0;
      cmpxchg_table[i] = 0;
      min_table[i] = 0;
      max_table[i] = 0;
      and_table[i] = 0;
      or_table[i] = 0;
      xor_table[i] = 0;
    }
    add_result[0] = 0;
    sub_result[0] = 0;
    xchg_result[0] = -1;
    inc_result[0] = 0;
    dec_result[0] = 0;
    cmpxchg_result[0] = 0;
    min_result[0] = 1;
    max_result[0] = 0;
    and_result[0] = 0;
    or_result[0] = 0;
    xor_result[0] = 0;
  }
}

/*!
  */
kernel void testAtomic(global int32b* add_table, global int32b* add_result,
    global int32b* sub_table, global int32b* sub_result,
    global int32b* xchg_table, global int32b* xchg_result,
    global int32b* inc_table, global int32b* inc_result,
    global int32b* dec_table, global int32b* dec_result,
    global int32b* cmpxchg_table, global int32b* cmpxchg_result,
    global int32b* min_table, global int32b* min_result,
    global int32b* max_table, global int32b* max_result,
    global int32b* and_table, global int32b* and_result,
    global int32b* or_table, global int32b* or_result,
    global int32b* xor_table, global int32b* xor_result,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    // atomic_add
    {
      const int32b i = atomic_add(add_result, 1);
      add_table[i] = 1;
    }
    // atomic_sub
    {
      const int32b i = atomic_sub(sub_result, 1);
      sub_table[-i] = 1;
    }
    // atomic_xchg
    {
//      atomic_xchg(xchg_result, (int32b)index);
//      xchg_table[index] = 1;
      (void)xchg_table;
      (void)xchg_result;
    }
    // atomic_inc
    {
      const int i = atomic_inc(inc_result);
      inc_table[i] = 1;
    }
    // atomic_dec
    {
      const int i = atomic_dec(dec_result);
      dec_table[-i] = 1;
    }
    // cmpxchg
    {
//      atomic_cmpxchg(cmpxchg_result, 0, (int32b)index);
//      cmpxchg_table[index] = 1;
      (void)cmpxchg_table;
      (void)cmpxchg_result;
    }
    // atomic_min
    {
//      const int32b v = (int32b)(index + 1) - (int32b)(resolution >> 1);
//      atomic_min(min_result, v);
//      min_table[index] = 1;
      (void)min_table;
      (void)min_result;
    }
    // atomic_max
    {
//      const int32b v = (int32b)(index + 1) - (int32b)(resolution >> 1);
//      atomic_max(max_result, v);
//      max_table[index] = 1;
      (void)max_table;
      (void)max_result;
    }
    // atomic_and
    {
//      atomic_and(and_result, (int32b)index);
//      and_table[index] = 1;
      (void)and_table;
      (void)and_result;
    }
    // atomic_or
    {
//      atomic_or(or_result, (int32b)index);
//      or_table[index] = 1;
      (void)or_table;
      (void)or_result;
    }
    // atomic_xor
    {
//      atomic_xor(xor_result, (int32b)index);
//      xor_table[index] = 1;
      (void)xor_table;
      (void)xor_result;
    }
  }
}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_ATOMIC_CL */
