/*!
  \file work_item.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL
#define ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL

// Zinvul
#include "zinvul/cl/atomic.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

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
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    work_size_table[0] = zinvul::getNumGroupsX();
    work_size_table[1] = zinvul::getNumGroupsY();
    work_size_table[2] = zinvul::getNumGroupsZ();
    work_size_table[3] = zinvul::getLocalSizeX();
    work_size_table[4] = zinvul::getLocalSizeY();
    work_size_table[5] = zinvul::getLocalSizeZ();
    work_size_table[6] = zinvul::getGlobalSizeX();
    work_size_table[7] = zinvul::getGlobalSizeY();
    work_size_table[8] = zinvul::getGlobalSizeZ();
  }
  if (index < resolution) {
    const uint32b id = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + zinvul::getLocalIdX();
    if (index == id)
      work_item_table[id] = 1u;
  }
  else {
    zinvul::atomic_inc(out_of_range);
  }
}

/*!
  */
__kernel void testWorkItem2D(GlobalPtr<uint32b> work_size_table,
    GlobalPtr<uint32b> work_item_table,
    GlobalPtr<uint32b> out_of_range,
    const uint2 resolution)
{
  const uint32b index = zinvul::getGlobalIdY() * zinvul::getGlobalSizeX() + zinvul::getGlobalIdX();
  if (index == 0) {
    work_size_table[0] = zinvul::getNumGroupsX();
    work_size_table[1] = zinvul::getNumGroupsY();
    work_size_table[2] = zinvul::getNumGroupsZ();
    work_size_table[3] = zinvul::getLocalSizeX();
    work_size_table[4] = zinvul::getLocalSizeY();
    work_size_table[5] = zinvul::getLocalSizeZ();
    work_size_table[6] = zinvul::getGlobalSizeX();
    work_size_table[7] = zinvul::getGlobalSizeY();
    work_size_table[8] = zinvul::getGlobalSizeZ();
  }
  const uint32b res = resolution.x * resolution.y;
  if (index < res) {
    const uint32b x = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + zinvul::getLocalIdX();
    const uint32b y = zinvul::getGroupIdY() * zinvul::getLocalSizeY() + zinvul::getLocalIdY();
    const uint32b id = y * zinvul::getGlobalSizeX() + x;
    if (index == id)
      work_item_table[id] = 1u;
  }
  else {
    zinvul::atomic_inc(out_of_range);
  }
}

/*!
  */
__kernel void testWorkItem3D(GlobalPtr<uint32b> work_size_table,
    GlobalPtr<uint32b> work_item_table,
    GlobalPtr<uint32b> out_of_range,
    const uint3 resolution)
{
  const uint32b index = zinvul::getGlobalIdZ() * (zinvul::getGlobalSizeX() * zinvul::getGlobalSizeY()) + zinvul::getGlobalIdY() * zinvul::getGlobalSizeX() + zinvul::getGlobalIdX();
  if (index == 0) {
    work_size_table[0] = zinvul::getNumGroupsX();
    work_size_table[1] = zinvul::getNumGroupsY();
    work_size_table[2] = zinvul::getNumGroupsZ();
    work_size_table[3] = zinvul::getLocalSizeX();
    work_size_table[4] = zinvul::getLocalSizeY();
    work_size_table[5] = zinvul::getLocalSizeZ();
    work_size_table[6] = zinvul::getGlobalSizeX();
    work_size_table[7] = zinvul::getGlobalSizeY();
    work_size_table[8] = zinvul::getGlobalSizeZ();
  }
  const uint32b res = resolution.x * resolution.y * resolution.z;
  if (index < res) {
    const uint32b x = zinvul::getGroupIdX() * zinvul::getLocalSizeX() + zinvul::getLocalIdX();
    const uint32b y = zinvul::getGroupIdY() * zinvul::getLocalSizeY() + zinvul::getLocalIdY();
    const uint32b z = zinvul::getGroupIdZ() * zinvul::getLocalSizeZ() + zinvul::getLocalIdZ();
    const uint32b id = z * (zinvul::getGlobalSizeX() * zinvul::getGlobalSizeY()) + y * zinvul::getGlobalSizeX() + x;
    if (index == id)
      work_item_table[id] = 1u;
  }
  else {
    zinvul::atomic_inc(out_of_range);
  }
}

#endif /* ZINVUL_WORK_ITEM_TEST_WORK_ITEM_CL */
