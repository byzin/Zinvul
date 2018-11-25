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
kernel void test1dWorkSize(global uint32b* work_size_table, const uint32b resolution)
{
  const uint num_of_funcs = 8;
  const uint index = getGlobalIdX();
  if (index < resolution) {
    work_size_table[num_of_funcs * index    ] = index;
    work_size_table[num_of_funcs * index + 1] = get_global_offset(0);
    work_size_table[num_of_funcs * index + 2] = get_global_size(0);
    work_size_table[num_of_funcs * index + 3] = get_group_id(0);
    work_size_table[num_of_funcs * index + 4] = get_local_id(0);
    work_size_table[num_of_funcs * index + 5] = get_local_size(0);
    work_size_table[num_of_funcs * index + 6] = get_num_groups(0);
    work_size_table[num_of_funcs * index + 7] = resolution;
  }
}

kernel void test2dWorkSize(global uint32b* work_size_table, const uint2 resolution)
{
  const uint num_of_funcs = 16;
  const uint x = getGlobalIdX();
  const uint y = getGlobalIdY();
  if ((x < resolution.x) && (y < resolution.y)) {
    const uint index = x + y * resolution.x;
    work_size_table[num_of_funcs * index    ] = x;
    work_size_table[num_of_funcs * index + 1] = y;
    work_size_table[num_of_funcs * index + 2] = get_global_offset(0);
    work_size_table[num_of_funcs * index + 3] = get_global_offset(1);
    work_size_table[num_of_funcs * index + 4] = get_global_size(0);
    work_size_table[num_of_funcs * index + 5] = get_global_size(1);
    work_size_table[num_of_funcs * index + 6] = get_group_id(0);
    work_size_table[num_of_funcs * index + 7] = get_group_id(1);
    work_size_table[num_of_funcs * index + 8] = get_local_id(0);
    work_size_table[num_of_funcs * index + 9] = get_local_id(1);
    work_size_table[num_of_funcs * index + 10] = get_local_size(0);
    work_size_table[num_of_funcs * index + 11] = get_local_size(1);
    work_size_table[num_of_funcs * index + 12] = get_num_groups(0);
    work_size_table[num_of_funcs * index + 13] = get_num_groups(1);
    work_size_table[num_of_funcs * index + 14] = resolution.x;
    work_size_table[num_of_funcs * index + 15] = resolution.y;
  }
}


#endif /* ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL */
