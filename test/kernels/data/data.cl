/*!
  \file data.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DATA_TEST_DATA_CL
#define ZINVUL_DATA_TEST_DATA_CL

#include "zinvul/cl/array.cl"
#include "zinvul/cl/fnv_1a_hash_engine.cl"
#include "zinvul/cl/limits.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using namespace zinvul;

namespace test {
class OptionTest;
} // namespace test

// Prototypes
__kernel void testPointer(ConstGlobalPtr<int32b> src, GlobalPtr<int32b> dst);
__kernel void testGlobalInstance(GlobalPtr<uint32b> results,
    GlobalPtr<test::OptionTest> options);
__kernel void testLocalInstance(GlobalPtr<uint32b> results,
    ConstGlobalPtr<test::OptionTest> options);
__kernel void copyBufferTest(ConstGlobalPtr<uint32b> src, GlobalPtr<uint32b> dst);
__kernel void multiplyBufferTest(GlobalPtr<int32b> table, const uint32b resolution);
__kernel void testInt8bBuffer(GlobalPtr<int8b> buffer);
__kernel void testUint8bBuffer(GlobalPtr<uint8b> buffer);
__kernel void testInt16bBuffer(GlobalPtr<int16b> buffer);
__kernel void testUint16bBuffer(GlobalPtr<uint16b> buffer);
__kernel void testTypeCast(
    ConstGlobalPtr<int32b> iinputs,
    ConstGlobalPtr<uint32b> uinputs,
    ConstGlobalPtr<float> finputs,
    GlobalPtr<int8b> ibuffer1,
    GlobalPtr<int32b> ibuffer2,
    GlobalPtr<int4> ibuffer3,
    GlobalPtr<uint8b> ubuffer1,
    GlobalPtr<uint32b> ubuffer2,
    GlobalPtr<uint4> ubuffer3,
    GlobalPtr<float> fbuffer1,
    GlobalPtr<float4> fbuffer2);
__kernel void testTypeReinterpreting(
    ConstGlobalPtr<uint32b> uinputs1,
    ConstGlobalPtr<float> finputs1,
    GlobalPtr<uint32b> ubuffer1,
    GlobalPtr<uint4> ubuffer2,
    GlobalPtr<float> fbuffer1,
    GlobalPtr<float4> fbuffer2);
__kernel void testCastUint8bToFloat(GlobalPtr<uint8b> buffer1,
    ConstGlobalPtr<uint8b> buffer2,
    GlobalPtr<float> buffer3,
    const uint32b resolution);
__kernel void testCastUint16bToFloat(GlobalPtr<uint16b> buffer1,
    ConstGlobalPtr<uint16b> buffer2,
    GlobalPtr<float> buffer3,
    const uint32b resolution);
__kernel void testCastUint32bToFloat(GlobalPtr<uint32b> buffer1,
    ConstGlobalPtr<uint32b> buffer2,
    GlobalPtr<float> buffer3,
    const uint32b resolution);
__kernel void testNumericLimits(GlobalPtr<int8b> int8_buffer,
    GlobalPtr<uint8b> uint8_buffer,
    GlobalPtr<int16b> int16_buffer,
    GlobalPtr<uint16b> uint16_buffer,
    GlobalPtr<int32b> int32_buffer,
    GlobalPtr<uint32b> uint32_buffer,
    GlobalPtr<float> float_buffer);
#if !defined(Z_MAC)
__kernel void testNumericLimits64(GlobalPtr<int64b> int64_buffer,
    GlobalPtr<uint64b> uint64_buffer,
    GlobalPtr<double> double_buffer);
#endif // !Z_MAC
__kernel void testArray(ConstGlobalPtr<uint32b> src,
    GlobalPtr<uint32b> dst);
__kernel void testFnv1AHash32(GlobalPtr<uint32b> hash32_buffer);
#if !defined(Z_MAC)
__kernel void testFnv1AHash64(GlobalPtr<uint64b> hash64_buffer);
#endif // !Z_MAC


namespace test {

struct PointerTest
{
  void init()
  {
    v0_ = 5;
    v1_ = 10;
    v2_ = 15;
    v3_ = 20;
  }

  int32b v0_;
  int32b v1_;
  int32b v2_;
  int32b v3_;
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
    for (size_t i = 0; i < storage_size; ++i)
      (storage + i)->init();
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
      (storage + 3)->init();
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

namespace test {

class OptionTest
{
 public:
  void init()
  {
    data_x_ = 0;
    data_y_ = 0;
    data_z_ = 0;
    data_w_ = 0;
  }

  uint32b getValue1() const
  {
    const uint32b v = data_y_ >> 16u;
    return v;
  }

  uint32b getValue2() const
  {
    const uint32b v = data_z_ >> 8u;
    return v;
  }

  uint32b getValue3() const
  {
    const uint32b v = data_w_ >> 24u;
    return v;
  }

  void setValue1(const uint32b v)
  {
    data_y_ = v << 16u;
  }

  void setValue2(const uint32b v)
  {
    data_z_ = v << 8u;
  }

  void setValue3(const uint32b v)
  {
    data_w_ = v << 24u;
  }

  uint32b data_x_;
  uint32b data_y_;
  uint32b data_z_;
  uint32b data_w_;
};

}

/*!
  */
__kernel void testGlobalInstance(GlobalPtr<uint32b> results,
    GlobalPtr<test::OptionTest> options)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    results[0] = options->getValue1();
    options->setValue2(10);
    results[1] = options[0].getValue3();
  }
}

/*!
  */
__kernel void testLocalInstance(GlobalPtr<uint32b> results,
    ConstGlobalPtr<test::OptionTest> o)
{
  constexpr size_t size = 2;
  Local<test::OptionTest> options[size];
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    options->init();
    // options[1].init(); //! \todo clspv crashes

    options[0] = o[0];
    // options[1] = o[0]; //! \todo clspv crashes
    options[1].setValue2(10);
    results[0] = options->getValue1();
    results[1] = (options + 1)->getValue2();
    results[2] = options[0].getValue3();
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
__kernel void testTypeCast(
    ConstGlobalPtr<int32b> iinputs,
    ConstGlobalPtr<uint32b> uinputs,
    ConstGlobalPtr<float> finputs,

    GlobalPtr<int8b> ibuffer1,
    GlobalPtr<int32b> ibuffer2,
    GlobalPtr<int4> ibuffer3,

    GlobalPtr<uint8b> ubuffer1,
    GlobalPtr<uint32b> ubuffer2,
    GlobalPtr<uint4> ubuffer3,

    GlobalPtr<float> fbuffer1,
    GlobalPtr<float4> fbuffer2)
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
    // int4
    {
      const uint4 v{uinputs[0], uinputs[1], uinputs[2], uinputs[3]};
      const auto r = cast<int4>(v);
      ibuffer3[0] = r;
    }
    {
      const float4 v{-finputs[0], -finputs[1], -finputs[2], -finputs[3]};
      const auto r = cast<int4>(v);
      ibuffer3[1] = r;
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
    // uint4
    {
      const int4 v{iinputs[0], iinputs[1], iinputs[2], iinputs[3]};
      const auto r = cast<uint4>(v);
      ubuffer3[0] = r;
    }
    {
      const float4 v{finputs[0], finputs[1], finputs[2], finputs[3]};
      const auto r = cast<uint4>(v);
      ubuffer3[1] = r;
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
  }
}

/*!
  */
__kernel void testTypeReinterpreting(
    ConstGlobalPtr<uint32b> uinputs1,
    ConstGlobalPtr<float> finputs1,
    GlobalPtr<uint32b> ubuffer1,
    GlobalPtr<uint4> ubuffer2,
    GlobalPtr<float> fbuffer1,
    GlobalPtr<float4> fbuffer2)
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
    // uint4
    {
      const float4 v{finputs1[0], finputs1[1], finputs1[2], finputs1[3]};
      const uint4 r = treatAs<uint4>(v);
      ubuffer2[0] = r;
    }
    {
      const float4 v{finputs1[0], finputs1[1], finputs1[2], finputs1[3]};
      const uint4 r = *treatAs<const uint4*>(&v);
      ubuffer2[1] = r;
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
    // float4
    {
      const uint4 v{uinputs1[0], uinputs1[1], uinputs1[2], uinputs1[3]};
      const float4 r = treatAs<float4>(v);
      fbuffer2[0] = r;
    }
    {
      const uint4 v{uinputs1[0], uinputs1[1], uinputs1[2], uinputs1[3]};
      const float4 r = *treatAs<const float4*>(&v);
      fbuffer2[1] = r;
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
__kernel void testCastUint8bToFloat(GlobalPtr<uint8b> buffer1,
    ConstGlobalPtr<uint8b> buffer2,
    GlobalPtr<float> buffer3,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t offset = (sizeof(float) / sizeof(uint8b)) * index;
    // Writing test
    {
      GlobalPtr<float> data = treatAs<GlobalPtr<float>>(buffer1 + offset);
      const float v = cast<float>(index);
      *data = v;
    }
    // Reading test
    {
      ConstGlobalPtr<float> data = treatAs<ConstGlobalPtr<float>>(buffer2 + offset);
      const float v = *data;
      buffer3[index] = v;
    }
  }
}

/*!
  */
__kernel void testCastUint16bToFloat(GlobalPtr<uint16b> buffer1,
    ConstGlobalPtr<uint16b> buffer2,
    GlobalPtr<float> buffer3,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t offset = (sizeof(float) / sizeof(uint16b)) * index;
    // Writing test
    {
      GlobalPtr<float> data = treatAs<GlobalPtr<float>>(buffer1 + offset);
      const float v = cast<float>(index);
      *data = v;
    }
    // Reading test
    {
      ConstGlobalPtr<float> data = treatAs<ConstGlobalPtr<float>>(buffer2 + offset);
      const float v = *data;
      buffer3[index] = v;
    }
  }
}

/*!
  */
__kernel void testCastUint32bToFloat(GlobalPtr<uint32b> buffer1,
    ConstGlobalPtr<uint32b> buffer2,
    GlobalPtr<float> buffer3,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const size_t offset = (sizeof(float) / sizeof(uint32b)) * index;
    // Writing test
    {
      GlobalPtr<float> data = treatAs<GlobalPtr<float>>(buffer1 + offset);
      const float v = cast<float>(index);
      *data = v;
    }
    // Reading test
    {
      ConstGlobalPtr<float> data = treatAs<ConstGlobalPtr<float>>(buffer2 + offset);
      const float v = *data;
      buffer3[index] = v;
    }
  }
}

/*!
  */
__kernel void testNumericLimits(GlobalPtr<int8b> int8_buffer,
    GlobalPtr<uint8b> uint8_buffer,
    GlobalPtr<int16b> int16_buffer,
    GlobalPtr<uint16b> uint16_buffer,
    GlobalPtr<int32b> int32_buffer,
    GlobalPtr<uint32b> uint32_buffer,
    GlobalPtr<float> float_buffer)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    // int8
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<int8b>::min();
      int8_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<int8b>::lowest();
      int8_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<int8b>::max();
      int8_buffer[i++] = vmax;
    }
    // uint8
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<uint8b>::min();
      uint8_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<uint8b>::lowest();
      uint8_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<uint8b>::max();
      uint8_buffer[i++] = vmax;
    }
    // int16
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<int16b>::min();
      int16_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<int16b>::lowest();
      int16_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<int16b>::max();
      int16_buffer[i++] = vmax;
    }
    // uint16
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<uint16b>::min();
      uint16_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<uint16b>::lowest();
      uint16_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<uint16b>::max();
      uint16_buffer[i++] = vmax;
    }
    // int32
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<int32b>::min();
      int32_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<int32b>::lowest();
      int32_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<int32b>::max();
      int32_buffer[i++] = vmax;
    }
    // uint32
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<uint32b>::min();
      uint32_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<uint32b>::lowest();
      uint32_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<uint32b>::max();
      uint32_buffer[i++] = vmax;
    }
    // float 
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<float>::min();
      float_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<float>::lowest();
      float_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<float>::max();
      float_buffer[i++] = vmax;
      constexpr auto vepsilon = NumericLimits<float>::epsilon();
      float_buffer[i++] = vepsilon;
      constexpr auto vinfinity = NumericLimits<float>::infinity();
      float_buffer[i++] = vinfinity;
      auto vnan = NumericLimits<float>::quietNan();
      float_buffer[i++] = vnan;
    }
  }
}

#if !defined(Z_MAC)

/*!
  */
__kernel void testNumericLimits64(GlobalPtr<int64b> int64_buffer,
    GlobalPtr<uint64b> uint64_buffer,
    GlobalPtr<double> double_buffer)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    // int64
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<int64b>::min();
      int64_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<int64b>::lowest();
      int64_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<int64b>::max();
      int64_buffer[i++] = vmax;
    }
    // uint64
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<uint64b>::min();
      uint64_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<uint64b>::lowest();
      uint64_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<uint64b>::max();
      uint64_buffer[i++] = vmax;
    }
    // double
    {
      size_t i = 0;
      constexpr auto vmin = NumericLimits<double>::min();
      double_buffer[i++] = vmin;
      constexpr auto vlow = NumericLimits<double>::lowest();
      double_buffer[i++] = vlow;
      constexpr auto vmax = NumericLimits<double>::max();
      double_buffer[i++] = vmax;
      constexpr auto vepsilon = NumericLimits<double>::epsilon();
      double_buffer[i++] = vepsilon;
      constexpr auto vinfinity = NumericLimits<double>::infinity();
      double_buffer[i++] = vinfinity;
      auto vnan = NumericLimits<double>::quietNan();
      double_buffer[i++] = vnan;
    }
  }
}

#endif // !Z_MAC

/*!
  */
__kernel void testArray(ConstGlobalPtr<uint32b> src,
    GlobalPtr<uint32b> dst)
{
  constexpr size_t n = 5;
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    size_t idx = 0;
    // Construct
    Private<Array<uint32b, n>> array;
    for (size_t i = 0; i < array.size(); ++i)
      array.set(i, i + 1);
    for (const auto value : array)
      dst[idx++] = value;
    array.fill(2);
    for (const auto value : array)
      dst[idx++] = value;
    // Access
    for (size_t i = 0; i < array.size(); ++i) {
      array[i] = src[i];
      dst[idx++] = array[i];
    }
    // Iterator
    uint32b sum = 0;
    for (auto ite = array.begin(); ite != array.end(); ++ite)
      sum += *ite;
    // min max
    dst[idx++] = array.getMinIndex();
    dst[idx++] = array.getMaxIndex();
  }
}

/*!
  */
__kernel void testFnv1AHash32(GlobalPtr<uint32b> hash32_buffer)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      const char seed[] = "";
      constexpr size_t n = sizeof(seed) / sizeof(seed[0]) - 1;
      hash32_buffer[i++] = Fnv1aHash32::hash(seed);
      hash32_buffer[i++] = Fnv1aHash32::hash(treatAs<const int8b*>(&seed[0]), n);
    }
    {
      const char seed[] = "a";
      constexpr size_t n = sizeof(seed) / sizeof(seed[0]) - 1;
      hash32_buffer[i++] = Fnv1aHash32::hash(seed);
      hash32_buffer[i++] = Fnv1aHash32::hash(treatAs<const int8b*>(&seed[0]), n);
    }
    {
      const char seed[] = "b";
      constexpr size_t n = sizeof(seed) / sizeof(seed[0]) - 1;
      hash32_buffer[i++] = Fnv1aHash32::hash(seed);
      hash32_buffer[i++] = Fnv1aHash32::hash(treatAs<const int8b*>(&seed[0]), n);
    }
    {
      const char seed[] = "foobar";
      constexpr size_t n = sizeof(seed) / sizeof(seed[0]) - 1;
      hash32_buffer[i++] = Fnv1aHash32::hash(seed);
      hash32_buffer[i++] = Fnv1aHash32::hash(treatAs<const int8b*>(&seed[0]), n);
    }
    {
      const uint8b seed = 'a';
      hash32_buffer[i++] = Fnv1aHash32::hash(seed);
    }
    {
      const uint8b seed = 'b';
      hash32_buffer[i++] = Fnv1aHash32::hash(seed);
    }
    {
      const uint32b seed = 123'456'789u;
      hash32_buffer[i++] = Fnv1aHash32::hash(seed);
    }
  }
}

#if !defined(Z_MAC)

/*!
  */
__kernel void testFnv1AHash64(GlobalPtr<uint64b> hash64_buffer)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      const char seed[] = "";
      constexpr size_t n = sizeof(seed) / sizeof(seed[0]) - 1;
      hash64_buffer[i++] = Fnv1aHash64::hash(seed);
      hash64_buffer[i++] = Fnv1aHash64::hash(treatAs<const int8b*>(&seed[0]), n);
    }
    {
      const char seed[] = "a";
      constexpr size_t n = sizeof(seed) / sizeof(seed[0]) - 1;
      hash64_buffer[i++] = Fnv1aHash64::hash(seed);
      hash64_buffer[i++] = Fnv1aHash64::hash(treatAs<const int8b*>(&seed[0]), n);
    }
    {
      const char seed[] = "b";
      constexpr size_t n = sizeof(seed) / sizeof(seed[0]) - 1;
      hash64_buffer[i++] = Fnv1aHash64::hash(seed);
      hash64_buffer[i++] = Fnv1aHash64::hash(treatAs<const int8b*>(&seed[0]), n);
    }
    {
      const char seed[] = "foobar";
      constexpr size_t n = sizeof(seed) / sizeof(seed[0]) - 1;
      hash64_buffer[i++] = Fnv1aHash64::hash(seed);
      hash64_buffer[i++] = Fnv1aHash64::hash(treatAs<const int8b*>(&seed[0]), n);
    }
    {
      const uint8b seed = 'a';
      hash64_buffer[i++] = Fnv1aHash64::hash(seed);
    }
    {
      const uint8b seed = 'b';
      hash64_buffer[i++] = Fnv1aHash64::hash(seed);
    }
    {
      const uint64b seed = 123'456'789u;
      hash64_buffer[i++] = Fnv1aHash64::hash(seed);
    }
  }
}

#endif // !Z_MAC

#endif /* ZINVUL_DATA_TEST_DATA_CL */
