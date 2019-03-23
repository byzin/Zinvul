/*!
  \file data.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DATA_TEST_DATA_CL
#define ZINVUL_DATA_TEST_DATA_CL

#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

/*!
  */
__kernel void copyBufferTest(__global const uint32b* src, __global uint32b* dst)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    for (size_t i = 0; i < 16; ++i) {
      dst[i] = src[i];
    }
  }
}

/*!
  */
__kernel void multiplyBufferTest(__global int32b* table, const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    table[index] = 2 * table[index];
  }
}

/*!
  */
__kernel void getTypeSize(__global uint32b* size_table)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    uint32b table_index = 0;
    size_table[table_index++] = sizeof(char);
    size_table[table_index++] = sizeof(int8b);
    size_table[table_index++] = sizeof(short);
    size_table[table_index++] = sizeof(int16b);
    size_table[table_index++] = sizeof(int);
    size_table[table_index++] = sizeof(int32b);
    size_table[table_index++] = sizeof(long);
    size_table[table_index++] = sizeof(int64b);
    size_table[table_index++] = sizeof(unsigned char);
    size_table[table_index++] = sizeof(uchar);
    size_table[table_index++] = sizeof(uint8b);
    size_table[table_index++] = sizeof(unsigned short);
    size_table[table_index++] = sizeof(ushort);
    size_table[table_index++] = sizeof(uint16b);
    size_table[table_index++] = sizeof(unsigned int);
    size_table[table_index++] = sizeof(uint);
    size_table[table_index++] = sizeof(uint32b);
    size_table[table_index++] = sizeof(unsigned long);
    size_table[table_index++] = sizeof(ulong);
    size_table[table_index++] = sizeof(uint64b);
    size_table[table_index++] = sizeof(half);
    size_table[table_index++] = sizeof(float);
    size_table[table_index++] = sizeof(char2);
    size_table[table_index++] = sizeof(char3);
    size_table[table_index++] = sizeof(char4);
    size_table[table_index++] = sizeof(short2);
    size_table[table_index++] = sizeof(short3);
    size_table[table_index++] = sizeof(short4);
    size_table[table_index++] = sizeof(int2);
    size_table[table_index++] = sizeof(int3);
    size_table[table_index++] = sizeof(int4);
    size_table[table_index++] = sizeof(long2);
    size_table[table_index++] = sizeof(long3);
    size_table[table_index++] = sizeof(long4);
    size_table[table_index++] = sizeof(half2);
    size_table[table_index++] = sizeof(half3);
    size_table[table_index++] = sizeof(half4);
    size_table[table_index++] = sizeof(float2);
    size_table[table_index++] = sizeof(float3);
    size_table[table_index++] = sizeof(float4);
    size_table[table_index] = table_index;
  }
}

/*!
  */
__kernel void testDataConversion(
    __global int16b* buffer1, __global float* buffer2, __global uint32b* buffer3,
    __global float4* buffer4, __global int4* buffer5)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    (void)buffer1[0];
//    {
//      const uint32b v = 3;
//      buffer1[0] = convert_short(v);
//    }
    (void)buffer2[0];
//    {
//      const int32b v = 3;
//      buffer2[0] = convert_float(v);
//    }
    (void)buffer3[0];
//    {
//      const float v = 5.0f;
//      buffer3[0] = convert_uint(v);
//    }
    (void)buffer4[0];
//    {
//      const int4 v = makeInt4(1, 2, 3, 4);
//      buffer4[0] = convert_float4(v);
//    }
    (void)buffer5[0];
//    {
//      const float4 v = makeFloat4(1.0f, 2.0f, 3.0f, 4.0f);
//      buffer5[0] = convert_int4(v);
//    }
  }
}

/*!
  */
__kernel void testDataReinterpreting(
    __global uint32b* buffer1, __global float* buffer2,
    __global uint4* buffer3, __global float4* buffer4)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    {
      const float v = 1.0f;
      buffer1[0] = as_uint(v);
    }
    {
      const uint32b v = 0x3f800000u;
      buffer2[0] = as_float(v);
    }
    {
      const float4 v = zMakeFloat4(1.0f, 2.0f, 4.0f, 8.0f);
      buffer3[0] = as_uint4(v);
    }
    {
      const uint4 v = zMakeUInt4(0x3f800000u, 0x40000000u, 0x40800000u, 0x41000000u);
      buffer4[0] = as_float4(v);
    }
  }
}

typedef struct __Ray1
{
  float4 origin_;
  float4 dir_;
} Ray1;

typedef struct __Ray2
{
  float origin_[3];
  float dir_[3];
} Ray2;

void initRay1G(global Ray1* ray)
{
  ray->origin_ = zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f);
  ray->dir_ = zMakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
}

void initRay1(Ray1* ray)
{
  ray->origin_ = zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f);
  ray->dir_ = zMakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
}

void initRay2G(global Ray2* ray)
{
  for (int index = 0; index < 3; ++index) {
    ray->origin_[index] = (float)(index + 1);
    ray->dir_[index] = 1.0f;
  }
}

void initRay2(Ray2* ray)
{
  for (int index = 0; index < 3; ++index) {
    ray->origin_[index] = (float)(index + 1);
    ray->dir_[index] = 1.0f;
  }
}

/*!
  */
__kernel void testVariablePointer(global Ray1* ray1_table, global Ray2* ray2_table, const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const uint32b i = 2 * index;
    {
      initRay1G(&ray1_table[i]);
      Ray1 ray = ray1_table[i + 1];
      initRay1(&ray);
      ray1_table[i + 1] = ray;
    }
    {
      initRay2G(&ray2_table[i]);
      //! \todo Remove commentout. The initRay2 crashes on AMD
//      Ray2 ray = ray2_table[i + 1];
//      initRay2(&ray);
//      ray2_table[i + 1] = ray;
    }
  }
}

/*!
  */
__kernel void testInt8bBuffer(__global int8b* buffer)
{
  const uint32b index = zGetGlobalIdX();
  if (index <= UCHAR_MAX) {
    const int32b v = (int32b)index + CHAR_MIN;
    buffer[index] = (int8b)v;
  }
}

/*!
  */
__kernel void testUint8bBuffer(__global uint8b* buffer)
{
  const uint32b index = zGetGlobalIdX();
  if (index <= UCHAR_MAX) {
    const uint8b v = (uint8b)index;
    buffer[index] = v;
  }
}

/*!
  */
__kernel void testInt16bBuffer(__global int16b* buffer)
{
  const uint32b index = zGetGlobalIdX();
  if (index <= USHRT_MAX) {
    const int32b v = (int32b)index + SHRT_MIN;
    buffer[index] = (int16b)v;
  }
}

/*!
  */
__kernel void testUint16bBuffer(__global uint16b* buffer)
{
  const uint32b index = zGetGlobalIdX();
  if (index <= USHRT_MAX) {
    const uint16b v = (uint16b)index;
    buffer[index] = v;
  }
}

/*!
  */
__kernel void testInt64bBuffer(__global int64b* buffer)
{
  const uint32b index = zGetGlobalIdX();
  if (index <= USHRT_MAX) {
    int64b v = (int64b)index + (int64b)SHRT_MIN;
    v = v << 32l;
    buffer[index] = v;
  }
}

/*!
  */
__kernel void testUint64bBuffer(__global uint64b* buffer)
{
  const uint32b index = zGetGlobalIdX();
  if (index <= USHRT_MAX) {
    uint64b v = (uint64b)index;
    v = v << 32ul;
    buffer[index] = v;
  }
}

/*!
  */
__kernel void testCastUint8bToFloat(__global uint8b* buffer1,
    const __global uint8b* buffer2,
    __global float* buffer3,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float) / sizeof(uint8b);
    // Writing test
    {
      __global float* data = (__global float*)(buffer1 + offset * index);
      const float v = (float)index;
      *data = v;
    }
    // Reading test
    {
      const __global float* data = (const __global float*)(buffer2 + offset * index);
      const float v = *data;
      buffer3[index] = v;
    }
  }
}

/*!
  */
__kernel void testCastUint16bToFloat(__global uint16b* buffer1,
    const __global uint16b* buffer2,
    __global float* buffer3,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float) / sizeof(uint16b);
    // Writing test
    {
      __global float* data = (__global float*)(buffer1 + offset * index);
      const float v = (float)index;
      *data = v;
    }
    // Reading test
    {
      const __global float* data = (const __global float*)(buffer2 + offset * index);
      const float v = *data;
      buffer3[index] = v;
    }
  }
}

/*!
  */
__kernel void testCastUint32bToFloat(__global uint32b* buffer1,
    const __global uint32b* buffer2,
    __global float* buffer3,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float) / sizeof(uint32b);
    // Writing test
    {
      __global float* data = (__global float*)(buffer1 + offset * index);
      const float v = (float)index;
      *data = v;
    }
    // Reading test
    {
      const __global float* data = (const __global float*)(buffer2 + offset * index);
      const float v = *data;
      buffer3[index] = v;
    }
  }
}


#endif /* ZINVUL_DATA_TEST_DATA_CL */
