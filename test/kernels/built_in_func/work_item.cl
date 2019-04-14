/*!
  \file work_item.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUILT_IN_FUNC_TEST_WORK_ITEM_CL
#define ZINVUL_BUILT_IN_FUNC_TEST_WORK_ITEM_CL

// Zinvul
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

///*!
//  */
//__kernel void testWorkItem1D(__global uint32b* work_size_table,
//    __global uint32b* work_item_table,
//    const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    work_size_table[0] = zGetNumGroupsX();
//    work_size_table[1] = zGetNumGroupsY();
//    work_size_table[2] = zGetNumGroupsZ();
//    work_size_table[3] = zGetLocalSizeX();
//    work_size_table[4] = zGetLocalSizeY();
//    work_size_table[5] = zGetLocalSizeZ();
//    work_size_table[6] = zGetGlobalSizeX();
//    work_size_table[7] = zGetGlobalSizeY();
//    work_size_table[8] = zGetGlobalSizeZ();
//  }
//  if (index < resolution) {
//    const uint32b i = zGetGroupIdX() * zGetLocalSizeX() + zGetLocalIdX();
//    work_item_table[i] = 1u;
//  }
//}
//
///*!
//  */
//__kernel void testWorkItem2D(__global uint32b* work_size_table,
//    __global uint32b* work_item_table,
//    const uint2 resolution)
//{
//  const uint32b index = zGetGlobalIdY() * zGetGlobalSizeX() + zGetGlobalIdX();
//  if (index == 0) {
//    work_size_table[0] = zGetNumGroupsX();
//    work_size_table[1] = zGetNumGroupsY();
//    work_size_table[2] = zGetNumGroupsZ();
//    work_size_table[3] = zGetLocalSizeX();
//    work_size_table[4] = zGetLocalSizeY();
//    work_size_table[5] = zGetLocalSizeZ();
//    work_size_table[6] = zGetGlobalSizeX();
//    work_size_table[7] = zGetGlobalSizeY();
//    work_size_table[8] = zGetGlobalSizeZ();
//  }
//  const uint32b res = resolution.x * resolution.y;
//  if (index < res) {
//    const uint32b x = zGetGroupIdX() * zGetLocalSizeX() + zGetLocalIdX();
//    const uint32b y = zGetGroupIdY() * zGetLocalSizeY() + zGetLocalIdY();
//    const uint32b i = y * zGetGlobalSizeX() + x;
//    work_item_table[i] = 1u;
//  }
//}
//
///*!
//  */
//__kernel void testWorkItem3D(__global uint32b* work_size_table,
//    __global uint32b* work_item_table,
//    const uint3 resolution)
//{
//  const uint32b index = zGetGlobalIdZ() * (zGetGlobalSizeX() * zGetGlobalSizeY()) + zGetGlobalIdY() * zGetGlobalSizeX() + zGetGlobalIdX();
//  if (index == 0) {
//    work_size_table[0] = zGetNumGroupsX();
//    work_size_table[1] = zGetNumGroupsY();
//    work_size_table[2] = zGetNumGroupsZ();
//    work_size_table[3] = zGetLocalSizeX();
//    work_size_table[4] = zGetLocalSizeY();
//    work_size_table[5] = zGetLocalSizeZ();
//    work_size_table[6] = zGetGlobalSizeX();
//    work_size_table[7] = zGetGlobalSizeY();
//    work_size_table[8] = zGetGlobalSizeZ();
//  }
//  const uint32b res = resolution.x * resolution.y * resolution.z;
//  if (index < res) {
//    const uint32b x = zGetGroupIdX() * zGetLocalSizeX() + zGetLocalIdX();
//    const uint32b y = zGetGroupIdY() * zGetLocalSizeY() + zGetLocalIdY();
//    const uint32b z = zGetGroupIdZ() * zGetLocalSizeZ() + zGetLocalIdZ();
//    const uint32b i = z * (zGetGlobalSizeX() * zGetGlobalSizeY()) + y * zGetGlobalSizeX() + x;
//    work_item_table[i] = 1u;
//  }
//}
//
//#if defined(ZINVUL_CPU)
//#define TEST_MAX_SUBGROUP_SIZE 1u
//#else // ZINVUL_CPU
//#define TEST_MAX_SUBGROUP_SIZE 64u
//#endif
//
//#define TEST_ITEM_CACHE_SIZE 32u
//
///*!
//  */
//__kernel void testLocalDataShare(__global uint32b* results1,
//    const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  local uint2 cache[TEST_MAX_SUBGROUP_SIZE * TEST_ITEM_CACHE_SIZE];
//  if (index < resolution) {
//    const size_t offset = TEST_ITEM_CACHE_SIZE * zGetLocalIdX();
//    for (size_t i = 0; i < TEST_ITEM_CACHE_SIZE; ++i) {
//      cache[offset + i] = zMakeUInt2(0u, 0u);
//    }
//    for (size_t i = 0; i < TEST_ITEM_CACHE_SIZE; ++i) {
//      ++cache[offset + i].x;
//      ++cache[offset + i].y;
//    }
//    uint32b n = 0;
//    for (size_t i = 0; i < TEST_ITEM_CACHE_SIZE; ++i) {
//      const uint2 v = cache[offset + i];
//      n += v.x + v.y;
//    }
//    results1[index] = n;
//  }
//}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_WORK_ITEM_CL */
