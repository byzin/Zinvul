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
    const uint32b resolution)
{
  const uint index = getGlobalIdX();
  if (index == 0) {
    add_result[0] = 0;
    sub_result[0] = 0;
    xchg_result[0] = 0;
    // atomic_inc
    for (uint32b i = 0; i < resolution; ++i)
      inc_table[i] = 0;
    inc_result[0] = 0;
    dec_result[0] = 0;
  }
}

/*!
  */
kernel void testAtomic(global int32b* add_table, global int32b* add_result,
    global int32b* sub_table, global int32b* sub_result,
    global int32b* xchg_table, global int32b* xchg_result,
    global int32b* inc_table, global int32b* inc_result,
    global int32b* dec_table, global int32b* dec_result,
    const uint32b resolution)
{
  const uint index = getGlobalIdX();
  if (index < resolution) {
    // atomic_add
    {
//      const int32b i = atomic_add(&buffer1[0], 1);
    }
    // atomic_sub
    {
//      const int32b i = atomic_sub(&buffer1[1], 1);
    }
    // atomic_xchg
    {
    }
    // atomic_inc
    {
      const int i = atomic_inc(inc_result);
      inc_table[i] = 1;
    }
    // atomic_dec
    {
    }
  }
}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_ATOMIC_CL */
