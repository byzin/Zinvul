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
__kernel void testCastUint32bToUint2(__global uint32b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(uint2) / sizeof(uint32b);
    __global uint2* data = (__global uint2*)(buffer + offset * index);
    const uint32b v = 2 * index;
    *data = zMakeUInt2(v, v + 1);
  }
}

/*!
  */
__kernel void testCastUint32bToUint4(__global uint32b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(uint4) / sizeof(uint32b);
    __global uint4* data = (__global uint4*)(buffer + offset * index);
    const uint32b v = 4 * index;
    *data = zMakeUInt4(v, v + 1, v + 2, v + 3);
  }
}

/*!
  */
__kernel void testCastUint32bToFloat(__global uint32b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float) / sizeof(uint32b);
    __global float* data = (__global float*)(buffer + offset * index);
    *data = (float)index;
  }
}

/*!
  */
__kernel void testCastUint32bToFloat2(__global uint32b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float2) / sizeof(uint32b);
    __global float2* data = (__global float2*)(buffer + offset * index);
    const float v = (float)(2 * index);
    *data = zMakeFloat2(v, v + 1.0f);
  }
}

/*!
  */
__kernel void testCastUint32bToFloat4(__global uint32b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float4) / sizeof(uint32b);
    __global float4* data = (__global float4*)(buffer + offset * index);
    const float v = (float)(4 * index);
    *data = zMakeFloat4(v, v + 1.0f, v + 2.0f, v + 3.0f);
  }
}

/*!
  */
__kernel void testCastUint32bToRay(__global uint32b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(Ray1) / sizeof(uint32b);
    __global Ray1* data = (__global Ray1*)(buffer + offset * index);
    initRay1G(data);
  }
}

typedef struct __TestData1
{
  float4 v1_;

  int2 v2_;
  short4 v3_;

  float v4_;
  int v5_;
  char4 v6_;
  short2 v7_;
} TestData1;

void initTestData1G(__global TestData1* data)
{
  data->v1_ = zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f);
  data->v2_ = zMakeInt2(1, 2);
#if !defined(Z_MAC)
  //! \todo Crashes on macOS
  data->v3_ = zMakeShort4(1, 2, 3, 4);
#endif // Z_MAC
  data->v4_ = 10.0f;
  data->v5_ = 10;
  data->v6_ = zMakeChar4(1, 2, 3, 4);
  data->v7_ = zMakeShort2(-10, 10);
}

/*!
  */
__kernel void testCastUint32bToTestData1(__global uint32b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(TestData1) / sizeof(uint32b);
    __global TestData1* data = (__global TestData1*)(buffer + offset * index);
    initTestData1G(data);
  }
}

#if defined(ZINVUL_TEST_INT8_POINTER)

/*!
  */
__kernel void testInt8Pointer(__global int8b* input, __global int8b* output)
{
  const uint32b index = zGetGlobalIdX();
  if (index < 128) {
    {
      int8b v = input[index];
      v = -v;
      output[index] = v;
    }
  }
}

/*!
  */
__kernel void testUint8Pointer(__global uint8b* input, __global uint8b* output)
{
  const uint32b index = zGetGlobalIdX();
  if (index < 128) {
    {
      uint8b v = input[index];
      v = 2 * v;
      output[index] = v;
    }
  }
}

/*!
  */
__kernel void testCastUint8bToUint32b(__global uint8b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(uint32b) / sizeof(uint8b);
    __global uint32b* data = (__global uint32b*)(buffer + offset * index);
    *data = index;
  }
}

/*!
  */
__kernel void testCastUint16bToUint32b(__global uint16b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(uint32b) / sizeof(uint16b);
    __global uint32b* data = (__global uint32b*)(buffer + offset * index);
    *data = index;
  }
}

/*!
  */
__kernel void testCastUint8bToUint2(__global uint8b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(uint2) / sizeof(uint8b);
    __global uint2* data = (__global uint2*)(buffer + offset * index);
    const uint32b v = 2 * index;
    *data = zMakeUInt2(v, v + 1);
  }
}

/*!
  */
__kernel void testCastUint16bToUint2(__global uint16b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(uint2) / sizeof(uint16b);
    __global uint2* data = (__global uint2*)(buffer + offset * index);
    const uint32b v = 2 * index;
    *data = zMakeUInt2(v, v + 1);
  }
}

/*!
  */
__kernel void testCastUint8bToUint4(__global uint8b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(uint4) / sizeof(uint8b);
    __global uint4* data = (__global uint4*)(buffer + offset * index);
    const uint32b v = 4 * index;
    *data = zMakeUInt4(v, v + 1, v + 2, v + 3);
  }
}

/*!
  */
__kernel void testCastUint16bToUint4(__global uint16b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(uint4) / sizeof(uint16b);
    __global uint4* data = (__global uint4*)(buffer + offset * index);
    const uint32b v = 4 * index;
    *data = zMakeUInt4(v, v + 1, v + 2, v + 3);
  }
}

/*!
  */
__kernel void testCastUint8bToFloat(__global uint8b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float) / sizeof(uint8b);
    __global float* data = (__global float*)(buffer + offset * index);
    *data = (float)index;
  }
}

/*!
  */
__kernel void testCastUint16bToFloat(__global uint16b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float) / sizeof(uint16b);
    __global float* data = (__global float*)(buffer + offset * index);
    *data = (float)index;
  }
}

/*!
  */
__kernel void testCastUint8bToFloat2(__global uint8b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float2) / sizeof(uint8b);
    __global float2* data = (__global float2*)(buffer + offset * index);
    const float v = (float)(2 * index);
    *data = zMakeFloat2(v, v + 1.0f);
  }
}

/*!
  */
__kernel void testCastUint16bToFloat2(__global uint16b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float2) / sizeof(uint16b);
    __global float2* data = (__global float2*)(buffer + offset * index);
    const float v = (float)(2 * index);
    *data = zMakeFloat2(v, v + 1.0f);
  }
}

/*!
  */
__kernel void testCastUint8bToFloat4(__global uint8b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float4) / sizeof(uint8b);
    __global float4* data = (__global float4*)(buffer + offset * index);
    const float v = (float)(4 * index);
    *data = zMakeFloat4(v, v + 1.0f, v + 2.0f, v + 3.0f);
  }
}

/*!
  */
__kernel void testCastUint16bToFloat4(__global uint16b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(float4) / sizeof(uint16b);
    __global float4* data = (__global float4*)(buffer + offset * index);
    const float v = (float)(4 * index);
    *data = zMakeFloat4(v, v + 1.0f, v + 2.0f, v + 3.0f);
  }
}

/*!
  */
__kernel void testCastUint8bToRay(__global uint8b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(Ray1) / sizeof(uint8b);
    __global Ray1* data = (__global Ray1*)(buffer + offset * index);
    initRay1G(data);
  }
}

/*!
  */
__kernel void testCastUint16bToRay(__global uint16b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(Ray1) / sizeof(uint16b);
    __global Ray1* data = (__global Ray1*)(buffer + offset * index);
    initRay1G(data);
  }
}

/*!
  */
__kernel void testCastUint8bToTestData1(__global uint8b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(TestData1) / sizeof(uint8b);
    __global TestData1* data = (__global TestData1*)(buffer + offset * index);
    initTestData1G(data);
  }
}

/*!
  */
__kernel void testCastUint16bToTestData1(__global uint16b* buffer,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const size_t offset = sizeof(TestData1) / sizeof(uint16b);
    __global TestData1* data = (__global TestData1*)(buffer + offset * index);
    initTestData1G(data);
  }
}

#endif // ZINVUL_TEST_INT8_POINTER

#endif /* ZINVUL_DATA_TEST_DATA_CL */
