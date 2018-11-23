/*!
  \file work_item.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL
#define ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL

// Zinvul
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

/*!
  */
kernel void test1dWorkSize(global uint32b* work_size_table, const uint32b work_size)
{
  const uint index = getGlobalIdX();
  if (index < work_size)
    work_size_table[index] = index;
  float3 a = makeFloat3(1.0f, 2.0f, 3.0f);
  float3 b = makeFloat3(3.0f, 4.0f, 5.0f);
  const float3 c = a + b;
}

/*!
  */
kernel void getGlobalId2dTest()
{
}

#endif /* ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL */
