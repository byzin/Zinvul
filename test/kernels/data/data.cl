/*!
  \file data.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DATA_TEST_DATA_CL
#define ZINVUL_DATA_TEST_DATA_CL

#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using namespace zinvul;

// Prototypes
__kernel void testPointer(ConstGlobalPtr<int32b> src, GlobalPtr<int32b> dst);
__kernel void copyBufferTest(ConstGlobalPtr<uint32b> src, GlobalPtr<uint32b> dst);
__kernel void multiplyBufferTest(GlobalPtr<int32b> table, const uint32b resolution);
__kernel void getTypeSize(GlobalPtr<uint32b> size_table);
__kernel void testInt8bBuffer(GlobalPtr<int8b> buffer);
__kernel void testUint8bBuffer(GlobalPtr<uint8b> buffer);
__kernel void testInt16bBuffer(GlobalPtr<int16b> buffer);
__kernel void testUint16bBuffer(GlobalPtr<uint16b> buffer);
__kernel void testInt64bBuffer(GlobalPtr<int64b> buffer);
__kernel void testUint64bBuffer(GlobalPtr<uint64b> buffer);
__kernel void testTypeCast(
    ConstGlobalPtr<int32b> iinputs,
    ConstGlobalPtr<uint32b> uinputs,
    ConstGlobalPtr<float> finputs,
    GlobalPtr<int8b> ibuffer1,
    GlobalPtr<int32b> ibuffer2,
    GlobalPtr<int64b> ibuffer3,
    GlobalPtr<int4> ibuffer4,
    GlobalPtr<long4> ibuffer5,
    GlobalPtr<uint8b> ubuffer1,
    GlobalPtr<uint32b> ubuffer2,
    GlobalPtr<uint64b> ubuffer3,
    GlobalPtr<uint4> ubuffer4,
    GlobalPtr<ulong4> ubuffer5,
    GlobalPtr<float> fbuffer1,
    GlobalPtr<float4> fbuffer2,
    GlobalPtr<double> dbuffer1,
    GlobalPtr<double4> dbuffer2);
/*!
  */
__kernel void testTypeReinterpreting(
    ConstGlobalPtr<uint32b> uinputs1,
    ConstGlobalPtr<uint64b> uinputs2,
    ConstGlobalPtr<float> finputs1,
    ConstGlobalPtr<double> finputs2,
    GlobalPtr<uint32b> ubuffer1,
    GlobalPtr<uint64b> ubuffer2,
    GlobalPtr<uint4> ubuffer3,
    GlobalPtr<ulong4> ubuffer4,
    GlobalPtr<float> fbuffer1,
    GlobalPtr<double> fbuffer2,
    GlobalPtr<float4> fbuffer3,
    GlobalPtr<double4> fbuffer4);

namespace test {

struct PointerTest
{
  int32b v0_ = 5;
  int32b v1_ = 10;
  int32b v2_ = 15;
  int32b v3_ = 20;
};

}

/*!
  */
__kernel void testPointer(ConstGlobalPtr<int32b> src, GlobalPtr<int32b> dst)
{
  constexpr size_t storage_size = 10;
  Local<test::PointerTest> storage[storage_size];
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      for (i = 0; i < 5; ++i)
        dst[i] = src[i];
    }
    {
      ConstGlobalPtr<int32b> s = src + 2;
      dst[i++] = *s;
      dst[i++] = *(++s);
      dst[i++] = *(s++);
      dst[i++] = *s;
      s -= 2;
      dst[i++] = *s;
    }
    {
      ConstGlobalPtr<int32b> s = 4 + src;
      dst[i++] = *s;
      dst[i++] = *(--s);
      dst[i++] = *(s--);
      dst[i++] = *s;
      s = s - 1;
      dst[i++] = *s;
      s += 2;
      dst[i++] = *s;
    }
    {
      ConstLocalPtr<test::PointerTest> s{&storage[3]};
      dst[i++] = s->v2_;
      dst[i++] = s->v0_;

      ConstLocalPtr<test::PointerTest> t{nullptr};
      if (t)
        dst[i++] = s->v1_;
      else
        dst[i++] = s->v3_;
      if (s)
        dst[i++] = s->v1_;
      else
        dst[i++] = s->v3_;
    }
    {
      ConstLocalPtr<test::PointerTest> begin = storage;
      ConstLocalPtr<test::PointerTest> end = storage + storage_size;
      dst[i++] = begin - end;
      dst[i++] = end - begin;

      dst[i++] = (begin == end) ? 1 : 0;
      dst[i++] = (begin != end) ? 1 : 0;
      dst[i++] = (begin > end) ? 1 : 0;
      dst[i++] = (begin >= end) ? 1 : 0;
      dst[i++] = (begin < end) ? 1 : 0;
      dst[i++] = (begin <= end) ? 1 : 0;

      ConstLocalPtr<test::PointerTest> t{nullptr};
      dst[i++] = (begin == nullptr) ? 1 : 0;
      dst[i++] = (t == nullptr) ? 1 : 0;
    }
  }
}

/*!
  */
__kernel void copyBufferTest(ConstGlobalPtr<uint32b> src, GlobalPtr<uint32b> dst)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    for (size_t i = 0; i < 16; ++i) {
      dst[i] = src[i];
    }
  }
}

/*!
  */
__kernel void multiplyBufferTest(GlobalPtr<int32b> table, const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    table[index] = 2 * table[index];
  }
}

/*!
  */
__kernel void getTypeSize(GlobalPtr<uint32b> size_table)
{
  const uint32b index = getGlobalIdX();
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
    size_table[table_index++] = sizeof(double);
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
    size_table[table_index++] = sizeof(uchar2);
    size_table[table_index++] = sizeof(uchar3);
    size_table[table_index++] = sizeof(uchar4);
    size_table[table_index++] = sizeof(ushort2);
    size_table[table_index++] = sizeof(ushort3);
    size_table[table_index++] = sizeof(ushort4);
    size_table[table_index++] = sizeof(uint2);
    size_table[table_index++] = sizeof(uint3);
    size_table[table_index++] = sizeof(uint4);
    size_table[table_index++] = sizeof(ulong2);
    size_table[table_index++] = sizeof(ulong3);
    size_table[table_index++] = sizeof(ulong4);
    size_table[table_index++] = sizeof(half2);
    size_table[table_index++] = sizeof(half3);
    size_table[table_index++] = sizeof(half4);
    size_table[table_index++] = sizeof(float2);
    size_table[table_index++] = sizeof(float3);
    size_table[table_index++] = sizeof(float4);
    size_table[table_index++] = sizeof(double2);
    size_table[table_index++] = sizeof(double3);
    size_table[table_index++] = sizeof(double4);
    size_table[table_index] = table_index;
  }
}

/*!
  */
__kernel void testTypeCast(
    ConstGlobalPtr<int32b> iinputs,
    ConstGlobalPtr<uint32b> uinputs,
    ConstGlobalPtr<float> finputs,

    GlobalPtr<int8b> ibuffer1,
    GlobalPtr<int32b> ibuffer2,
    GlobalPtr<int64b> ibuffer3,
    GlobalPtr<int4> ibuffer4,
    GlobalPtr<long4> ibuffer5,

    GlobalPtr<uint8b> ubuffer1,
    GlobalPtr<uint32b> ubuffer2,
    GlobalPtr<uint64b> ubuffer3,
    GlobalPtr<uint4> ubuffer4,
    GlobalPtr<ulong4> ubuffer5,

    GlobalPtr<float> fbuffer1,
    GlobalPtr<float4> fbuffer2,

    GlobalPtr<double> dbuffer1,
    GlobalPtr<double4> dbuffer2)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    // int8b
    {
      for (size_t i = 0; i < 4; ++i) {
        const uint32b v = uinputs[i];
        const auto r = cast<int8b>(v);
        ibuffer1[i] = r;
      }
    }
    {
      for (size_t i = 0; i < 4; ++i) {
        const float v = -finputs[i];
        const auto r = cast<int8b>(v);
        ibuffer1[i + 4] = r;
      }
    }
    // int32b
    {
      for (size_t i = 0; i < 4; ++i) {
        const uint32b v = uinputs[i];
        const auto r = cast<int32b>(v);
        ibuffer2[i] = r;
      }
    }
    {
      for (size_t i = 0; i < 4; ++i) {
        const float v = -finputs[i];
        const auto r = cast<int32b>(v);
        ibuffer2[i + 4] = r;
      }
    }
    // int64b
    {
      for (size_t i = 0; i < 4; ++i) {
        const uint32b v = uinputs[i];
        const auto r = cast<int64b>(v);
        ibuffer3[i] = r;
      }
    }
    {
      for (size_t i = 0; i < 4; ++i) {
        const float v = -finputs[i];
        const auto r = cast<int64b>(v);
        ibuffer3[i + 4] = r;
      }
    }
    // int4
    {
      const uint4 v{uinputs[0], uinputs[1], uinputs[2], uinputs[3]};
      const auto r = cast<int4>(v);
      ibuffer4[0] = r;
    }
    {
      const float4 v{-finputs[0], -finputs[1], -finputs[2], -finputs[3]};
      const auto r = cast<int4>(v);
      ibuffer4[1] = r;
    }
    // long4
    {
      const uint4 v{uinputs[0], uinputs[1], uinputs[2], uinputs[3]};
      const auto r = cast<long4>(v);
      ibuffer5[0] = r;
    }
    {
      const float4 v{-finputs[0], -finputs[1], -finputs[2], -finputs[3]};
      const auto r = cast<long4>(v);
      ibuffer5[1] = r;
    }

    // uint8b
    {
      for (size_t i = 0; i < 4; ++i) {
        const float v = finputs[i];
        const auto r = cast<uint8b>(v);
        ubuffer1[i] = r;
      }
    }
    // uint32b
    {
      for (size_t i = 0; i < 4; ++i) {
        const float v = finputs[i];
        const auto r = cast<uint32b>(v);
        ubuffer2[i] = r;
      }
    }
    // uint64b
    {
      for (size_t i = 0; i < 4; ++i) {
        const float v = finputs[i];
        const auto r = cast<uint64b>(v);
        ubuffer3[i] = r;
      }
    }
    // uint4
    {
      const int4 v{iinputs[0], iinputs[1], iinputs[2], iinputs[3]};
      const auto r = cast<uint4>(v);
      ubuffer4[0] = r;
    }
    {
      const float4 v{finputs[0], finputs[1], finputs[2], finputs[3]};
      const auto r = cast<uint4>(v);
      ubuffer4[1] = r;
    }
    // ulong4
    {
      const int4 v{iinputs[0], iinputs[1], iinputs[2], iinputs[3]};
      const auto r = cast<ulong4>(v);
      ubuffer5[0] = r;
    }
    {
      const float4 v{finputs[0], finputs[1], finputs[2], finputs[3]};
      const auto r = cast<ulong4>(v);
      ubuffer5[1] = r;
    }

    // float
    {
      for (size_t i = 0; i < 4; ++i) {
        const uint32b v = uinputs[i];
        const auto r = cast<float>(v);
        fbuffer1[i] = r;
      }
      for (size_t i = 0; i < 4; ++i) {
        const int32b v = -iinputs[i];
        const auto r = cast<float>(v);
        fbuffer1[i + 4] = r;
      }
    }
    // float4
    {
      const uint4 v{uinputs[0], uinputs[1], uinputs[2], uinputs[3]};
      const auto r = cast<float4>(v);
      fbuffer2[0] = r;
    }
    {
      const int4 v{-iinputs[0], -iinputs[1], -iinputs[2], -iinputs[3]};
      const auto r = cast<float4>(v);
      fbuffer2[1] = r;
    }

    // double
    {
      for (size_t i = 0; i < 4; ++i) {
        const uint32b v = uinputs[i];
        const auto r = cast<double>(v);
        dbuffer1[i] = r;
      }
      for (size_t i = 0; i < 4; ++i) {
        const int32b v = -iinputs[i];
        const auto r = cast<double>(v);
        dbuffer1[i + 4] = r;
      }
    }
    // double4
    {
      const uint4 v{uinputs[0], uinputs[1], uinputs[2], uinputs[3]};
      const auto r = cast<double4>(v);
      dbuffer2[0] = r;
    }
    {
      const int4 v{-iinputs[0], -iinputs[1], -iinputs[2], -iinputs[3]};
      const auto r = cast<double4>(v);
      dbuffer2[1] = r;
    }
  }
}

/*!
  */
__kernel void testTypeReinterpreting(
    ConstGlobalPtr<uint32b> uinputs1,
    ConstGlobalPtr<uint64b> uinputs2,
    ConstGlobalPtr<float> finputs1,
    ConstGlobalPtr<double> finputs2,
    GlobalPtr<uint32b> ubuffer1,
    GlobalPtr<uint64b> ubuffer2,
    GlobalPtr<uint4> ubuffer3,
    GlobalPtr<ulong4> ubuffer4,
    GlobalPtr<float> fbuffer1,
    GlobalPtr<double> fbuffer2,
    GlobalPtr<float4> fbuffer3,
    GlobalPtr<double4> fbuffer4)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    // uint32b
    {
      for (size_t i = 0; i < 4; ++i) {
        const float v = finputs1[i];
        const uint32b r = treatAs<uint32b>(v);
        ubuffer1[i] = r;
      }
      for (size_t i = 0; i < 4; ++i) {
        const float v = finputs1[i];
        const uint32b r = *treatAs<const uint32b*>(&v);
        ubuffer1[i + 4] = r;
      }
    }
    // uint64b
    {
      for (size_t i = 0; i < 4; ++i) {
        const double v = finputs2[i];
        const uint64b r = treatAs<uint64b>(v);
        ubuffer2[i] = r;
      }
      for (size_t i = 0; i < 4; ++i) {
        const double v = finputs2[i];
        const uint64b r = *treatAs<const uint64b*>(&v);
        ubuffer2[i + 4] = r;
      }
    }
    // uint4
    {
      const float4 v{finputs1[0], finputs1[1], finputs1[2], finputs1[3]};
      const uint4 r = treatAs<uint4>(v);
      ubuffer3[0] = r;
    }
    {
      const float4 v{finputs1[0], finputs1[1], finputs1[2], finputs1[3]};
      const uint4 r = *treatAs<const uint4*>(&v);
      ubuffer3[1] = r;
    }
    // ulong4
    {
      const double4 v{finputs2[0], finputs2[1], finputs2[2], finputs2[3]};
      const ulong4 r = treatAs<ulong4>(v);
      ubuffer4[0] = r;
    }
    {
      const double4 v{finputs2[0], finputs2[1], finputs2[2], finputs2[3]};
      const ulong4 r = *treatAs<const ulong4*>(&v);
      ubuffer4[1] = r;
    }

    // float
    {
      for (size_t i = 0; i < 4; ++i) {
        const uint32b v = uinputs1[i];
        const float r = treatAs<float>(v);
        fbuffer1[i] = r;
      }
      for (size_t i = 0; i < 4; ++i) {
        const uint32b v = uinputs1[i];
        const float r = *treatAs<const float*>(&v);
        fbuffer1[i + 4] = r;
      }
    }
    // double
    {
      for (size_t i = 0; i < 4; ++i) {
        const uint64b v = uinputs2[i];
        const double r = treatAs<double>(v);
        fbuffer2[i] = r;
      }
      for (size_t i = 0; i < 4; ++i) {
        const uint64b v = uinputs2[i];
        const double r = *treatAs<const double*>(&v);
        fbuffer2[i + 4] = r;
      }
    }
    // float4
    {
      const uint4 v{uinputs1[0], uinputs1[1], uinputs1[2], uinputs1[3]};
      const float4 r = treatAs<float4>(v);
      fbuffer3[0] = r;
    }
    {
      const uint4 v{uinputs1[0], uinputs1[1], uinputs1[2], uinputs1[3]};
      const float4 r = *treatAs<const float4*>(&v);
      fbuffer3[1] = r;
    }
    // double4 
    {
      const ulong4 v{uinputs2[0], uinputs2[1], uinputs2[2], uinputs2[3]};
      const double4 r = treatAs<double4>(v);
      fbuffer4[0] = r;
    }
    {
      const ulong4 v{uinputs2[0], uinputs2[1], uinputs2[2], uinputs2[3]};
      const double4 r = *treatAs<const double4*>(&v);
      fbuffer4[1] = r;
    }
  }
}

//typedef struct __Ray1
//{
//  float4 origin_;
//  float4 dir_;
//} Ray1;
//
//typedef struct __Ray2
//{
//  float origin_[3];
//  float dir_[3];
//} Ray2;
//
//void initRay1G(global Ray1* ray)
//{
//  ray->origin_ = zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f);
//  ray->dir_ = zMakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
//}
//
//void initRay1(Ray1* ray)
//{
//  ray->origin_ = zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f);
//  ray->dir_ = zMakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
//}
//
//void initRay2G(global Ray2* ray)
//{
//  for (int index = 0; index < 3; ++index) {
//    ray->origin_[index] = (float)(index + 1);
//    ray->dir_[index] = 1.0f;
//  }
//}
//
//void initRay2(Ray2* ray)
//{
//  for (int index = 0; index < 3; ++index) {
//    ray->origin_[index] = (float)(index + 1);
//    ray->dir_[index] = 1.0f;
//  }
//}
//
///*!
//  */
//__kernel void testVariablePointer(global Ray1* ray1_table, global Ray2* ray2_table, const uint32b resolution)
//{
//  const uint32b index = getGlobalIdX();
//  if (index < resolution) {
//    const uint32b i = 2 * index;
//    {
//      initRay1G(&ray1_table[i]);
//      Ray1 ray = ray1_table[i + 1];
//      initRay1(&ray);
//      ray1_table[i + 1] = ray;
//    }
//    {
//      initRay2G(&ray2_table[i]);
//      //! \todo Remove commentout. The initRay2 crashes on AMD
////      Ray2 ray = ray2_table[i + 1];
////      initRay2(&ray);
////      ray2_table[i + 1] = ray;
//    }
//  }
//}

/*!
  */
__kernel void testInt8bBuffer(GlobalPtr<int8b> buffer)
{
  const uint32b index = getGlobalIdX();
  if (index <= UCHAR_MAX) {
    const int32b v = cast<int32b>(index) + CHAR_MIN;
    buffer[index] = cast<int8b>(v);
  }
}

/*!
  */
__kernel void testUint8bBuffer(GlobalPtr<uint8b> buffer)
{
  const uint32b index = getGlobalIdX();
  if (index <= UCHAR_MAX) {
    const uint8b v = cast<uint8b>(index);
    buffer[index] = v;
  }
}

/*!
  */
__kernel void testInt16bBuffer(GlobalPtr<int16b> buffer)
{
  const uint32b index = getGlobalIdX();
  if (index <= USHRT_MAX) {
    const int32b v = cast<int32b>(index) + SHRT_MIN;
    buffer[index] = cast<int16b>(v);
  }
}

/*!
  */
__kernel void testUint16bBuffer(GlobalPtr<uint16b> buffer)
{
  const uint32b index = getGlobalIdX();
  if (index <= USHRT_MAX) {
    const uint16b v = cast<uint16b>(index);
    buffer[index] = v;
  }
}

/*!
  */
__kernel void testInt64bBuffer(GlobalPtr<int64b> buffer)
{
  const uint32b index = getGlobalIdX();
  if (index <= USHRT_MAX) {
    int64b v = cast<int64b>(index) + cast<int64b>(SHRT_MIN);
    v = v << 32l;
    buffer[index] = v;
  }
}

/*!
  */
__kernel void testUint64bBuffer(GlobalPtr<uint64b> buffer)
{
  const uint32b index = getGlobalIdX();
  if (index <= USHRT_MAX) {
    uint64b v = cast<uint64b>(index);
    v = v << 32ul;
    buffer[index] = v;
  }
}

///*!
//  */
//__kernel void testCastUint8bToFloat(__global uint8b* buffer1,
//    const __global uint8b* buffer2,
//    __global float* buffer3,
//    const uint32b resolution)
//{
//  const uint32b index = getGlobalIdX();
//  if (index < resolution) {
//    const size_t offset = sizeof(float) / sizeof(uint8b);
//    // Writing test
//    {
//      __global float* data = (__global float*)(buffer1 + offset * index);
//      const float v = (float)index;
//      *data = v;
//    }
//    // Reading test
//    {
//      const __global float* data = (const __global float*)(buffer2 + offset * index);
//      const float v = *data;
//      buffer3[index] = v;
//    }
//  }
//}
//
///*!
//  */
//__kernel void testCastUint16bToFloat(__global uint16b* buffer1,
//    const __global uint16b* buffer2,
//    __global float* buffer3,
//    const uint32b resolution)
//{
//  const uint32b index = getGlobalIdX();
//  if (index < resolution) {
//    const size_t offset = sizeof(float) / sizeof(uint16b);
//    // Writing test
//    {
//      __global float* data = (__global float*)(buffer1 + offset * index);
//      const float v = (float)index;
//      *data = v;
//    }
//    // Reading test
//    {
//      const __global float* data = (const __global float*)(buffer2 + offset * index);
//      const float v = *data;
//      buffer3[index] = v;
//    }
//  }
//}
//
///*!
//  */
//__kernel void testCastUint32bToFloat(__global uint32b* buffer1,
//    const __global uint32b* buffer2,
//    __global float* buffer3,
//    const uint32b resolution)
//{
//  const uint32b index = getGlobalIdX();
//  if (index < resolution) {
//    const size_t offset = sizeof(float) / sizeof(uint32b);
//    // Writing test
//    {
//      __global float* data = (__global float*)(buffer1 + offset * index);
//      const float v = (float)index;
//      *data = v;
//    }
//    // Reading test
//    {
//      const __global float* data = (const __global float*)(buffer2 + offset * index);
//      const float v = *data;
//      buffer3[index] = v;
//    }
//  }
//}


#endif /* ZINVUL_DATA_TEST_DATA_CL */
