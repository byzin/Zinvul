/*!
  \file work_item.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL
#define ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL

// Zinvul
#include "zinvul/cl/atomic.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using namespace zinvul;

// Forward declaration
__kernel void testWorkItem1D(GlobalPtr<uint32b> work_size_table,
    GlobalPtr<uint32b> work_item_table,
    GlobalPtr<uint32b> out_of_range,
    const uint32b resolution);
__kernel void testWorkItem2D(GlobalPtr<uint32b> work_size_table,
    GlobalPtr<uint32b> work_item_table,
    GlobalPtr<uint32b> out_of_range,
    const uint2 resolution);
__kernel void testWorkItem3D(GlobalPtr<uint32b> work_size_table,
    GlobalPtr<uint32b> work_item_table,
    GlobalPtr<uint32b> out_of_range,
    const uint3 resolution);

/*!
  */
__kernel void testWorkItem1D(GlobalPtr<uint32b> work_size_table,
    GlobalPtr<uint32b> work_item_table,
    GlobalPtr<uint32b> out_of_range,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    work_size_table[0] = getNumGroupsX();
    work_size_table[1] = getNumGroupsY();
    work_size_table[2] = getNumGroupsZ();
    work_size_table[3] = getLocalSizeX();
    work_size_table[4] = getLocalSizeY();
    work_size_table[5] = getLocalSizeZ();
    work_size_table[6] = getGlobalSizeX();
    work_size_table[7] = getGlobalSizeY();
    work_size_table[8] = getGlobalSizeZ();
  }
  if (index < resolution) {
    const uint32b id = getGroupIdX() * getLocalSizeX() + getLocalIdX();
    if (index == id)
      work_item_table[id] = 1u;
  }
  else {
    Atomic::increment(out_of_range);
  }
}

/*!
  */
__kernel void testWorkItem2D(GlobalPtr<uint32b> work_size_table,
    GlobalPtr<uint32b> work_item_table,
    GlobalPtr<uint32b> out_of_range,
    const uint2 resolution)
{
  const uint32b index = getGlobalIdY() * getGlobalSizeX() + getGlobalIdX();
  if (index == 0) {
    work_size_table[0] = getNumGroupsX();
    work_size_table[1] = getNumGroupsY();
    work_size_table[2] = getNumGroupsZ();
    work_size_table[3] = getLocalSizeX();
    work_size_table[4] = getLocalSizeY();
    work_size_table[5] = getLocalSizeZ();
    work_size_table[6] = getGlobalSizeX();
    work_size_table[7] = getGlobalSizeY();
    work_size_table[8] = getGlobalSizeZ();
  }
  const uint32b res = resolution.x * resolution.y;
  if (index < res) {
    const uint32b x = getGroupIdX() * getLocalSizeX() + getLocalIdX();
    const uint32b y = getGroupIdY() * getLocalSizeY() + getLocalIdY();
    const uint32b id = y * getGlobalSizeX() + x;
    if (index == id)
      work_item_table[id] = 1u;
  }
  else {
    Atomic::increment(out_of_range);
  }
}

/*!
  */
__kernel void testWorkItem3D(GlobalPtr<uint32b> work_size_table,
    GlobalPtr<uint32b> work_item_table,
    GlobalPtr<uint32b> out_of_range,
    const uint3 resolution)
{
  const uint32b index = getGlobalIdZ() * (getGlobalSizeX() * getGlobalSizeY()) + getGlobalIdY() * getGlobalSizeX() + getGlobalIdX();
  if (index == 0) {
    work_size_table[0] = getNumGroupsX();
    work_size_table[1] = getNumGroupsY();
    work_size_table[2] = getNumGroupsZ();
    work_size_table[3] = getLocalSizeX();
    work_size_table[4] = getLocalSizeY();
    work_size_table[5] = getLocalSizeZ();
    work_size_table[6] = getGlobalSizeX();
    work_size_table[7] = getGlobalSizeY();
    work_size_table[8] = getGlobalSizeZ();
  }
  const uint32b res = resolution.x * resolution.y * resolution.z;
  if (index < res) {
    const uint32b x = getGroupIdX() * getLocalSizeX() + getLocalIdX();
    const uint32b y = getGroupIdY() * getLocalSizeY() + getLocalIdY();
    const uint32b z = getGroupIdZ() * getLocalSizeZ() + getLocalIdZ();
    const uint32b id = z * (getGlobalSizeX() * getGlobalSizeY()) + y * getGlobalSizeX() + x;
    if (index == id)
      work_item_table[id] = 1u;
  }
  else {
    Atomic::increment(out_of_range);
  }
}

#endif /* ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL */
