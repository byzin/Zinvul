/*!
  \file relational.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL
#define ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL

// Zinvul
#include "zinvul/cl/limits.cl"
#include "zinvul/cl/relational.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/type_traits.cl"
#include "zinvul/cl/utility.cl"

using namespace zinvul;

// Forward declaration
__kernel void testRelationalF1(GlobalPtr<int32b> results);
__kernel void testRelationalF4(GlobalPtr<int4> results);
__kernel void testBitselectU(GlobalPtr<uint32b> results1,
    GlobalPtr<uchar2> results2,
    GlobalPtr<ushort3> results3,
    GlobalPtr<uint4> results4);
__kernel void testSelectI(GlobalPtr<int32b> results1,
    GlobalPtr<char2> results2,
    GlobalPtr<short3> results3,
    GlobalPtr<int4> results4);
__kernel void testSelectU(GlobalPtr<uint32b> results1,
    GlobalPtr<uchar2> results2,
    GlobalPtr<ushort3> results3,
    GlobalPtr<uint4> results4);
__kernel void testSelectF(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4);

namespace test {

template <typename FloatN, typename ResultN> inline
void testRelationalImpl(const FloatN a,
    const FloatN b,
    const FloatN c,
    const FloatN d,
    GlobalPtr<ResultN> results) noexcept;

/*!
  */
template <typename FloatN, typename ResultN> inline
void testRelationalImpl(const FloatN a,
    const FloatN b,
    const FloatN c,
    const FloatN d,
    GlobalPtr<ResultN> results) noexcept
{
  uint32b index = 0;
  results[index++] = isEqual(a, b);
  results[index++] = isNotEqual(a, b);
  results[index++] = isGreater(a, b);
  results[index++] = isGreater(a, d);
  results[index++] = isGreaterEqual(a, b);
  results[index++] = isLess(a, b);
  results[index++] = isLess(a, d);
  results[index++] = isLessEqual(a, b);
  results[index++] = isInf(d);
  results[index++] = isNan(c);
  results[index++] = isSignBitSet(c);
  results[index++] = isSignBitSet(d);
}

} // namespace test

/*!
  */
__kernel void testRelationalF1(GlobalPtr<int32b> results)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    using Limits = zinvul::NumericLimits<float>;
    {
      const float a = 1.0f;
      const float b = 1.0f;
      const float c = -1.0f;
      const float d = Limits::infinity();
      test::testRelationalImpl(a, b, c, d, results);
    }
    {
      const float a = 0.0f;
      const float b = Limits::infinity();
      const float c = -Limits::infinity();
      const float d = Limits::quietNan();
      test::testRelationalImpl(a, b, c, d, results + 12);
    }
  }
}

/*!
  */
__kernel void testRelationalF4(GlobalPtr<int4> results)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    using Limits = zinvul::NumericLimits<float>;
    const float4 a{1.0f, 2.0f, 3.0f, 4.0f};
    const float4 b{3.0f, 2.0f, 1.0f, 0.0f};
    const float4 c{1.0f, 0.0f, -1.0f, Limits::quietNan()};
    const float4 d{0.0f, -1.0f, Limits::infinity(), -Limits::infinity()};
    test::testRelationalImpl(a, b, c, d, results);
  }
}

/*!
  */
__kernel void testBitselectU(GlobalPtr<uint32b> results1,
    GlobalPtr<uchar2> results2,
    GlobalPtr<ushort3> results3,
    GlobalPtr<uint4> results4)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    {
      const uint32b a = 0b01010101'01010101'01010101'01010101u;
      const uint32b b = 0b10101010'10101011'10101010'10101010u;
      const uint32b c = 0b00001111'00001111'00001111'00001111u;
      results1[0] = selectBit(a, b, c);
    }
    {
      const uchar2 a = makeUChar2(0b01010101u);
      const uchar2 b = makeUChar2(0b10101010u);
      const uchar2 c{0b00001111u, 0b11110000u};
      results2[0] = selectBit(a, b, c);
    }
    {
      const ushort3 a = makeUShort3(0b01010101'01010101u);
      const ushort3 b = makeUShort3(0b10101010'10101010u);
      const ushort3 c{0b00001111'00001111u, 0b11110000'11110000u, 0b00110011'00110011u};
      results3[0] = selectBit(a, b, c);
    }
    {
      const uint4 a = makeUInt4(0b01010101'01010101'01010101'01010101u);
      const uint4 b = makeUInt4(0b10101010'10101011'10101010'10101010u);
      const uint4 c{0b00001111'00001111'00001111'00001111u,
                   0b11110000'11110000'11110000'11110000u,
                   0b00110011'00110011'00110011'00110011u,
                   0b11001100'11001100'11001100'11001100u};
      results4[0] = selectBit(a, b, c);
    }
  }
}

/*!
  */
__kernel void testSelectI(GlobalPtr<int32b> results1,
    GlobalPtr<char2> results2,
    GlobalPtr<short3> results3,
    GlobalPtr<int4> results4)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    constexpr int32b sfalse = kResultFalse<int32b>;
    constexpr int32b strue = kResultTrue<int32b>;
    constexpr int32b vfalse = kResultFalse<int4>;
    constexpr int32b vtrue = kResultTrue<int4>;

    {
      const int32b a = 1;
      const int32b b = -1;
      const int32b c = sfalse;
      results1[0] = selectValue(a, b, c);
    }
    {
      const int32b a = 1;
      const int32b b = -1;
      const int32b c = strue;
      results1[1] = selectValue(a, b, c);
    }

    {
      const char2 a{1, 2};
      const char2 b{-1, -2};
      const char2 c{kResultFalse<char2>, kResultTrue<char2>};
      results2[0] = selectValue(a, b, c);
    }
    {
      const char2 a{1, 2};
      const char2 b{-1, -2};
      const char2 c{kResultTrue<char2>, kResultFalse<char2>};
      results2[1] = selectValue(a, b, c);
    }

    {
      const short3 a{1, 2, 3};
      const short3 b{-1, -2, -3};
      const short3 c{kResultFalse<short3>, kResultTrue<short3>, kResultFalse<short3>};
      results3[0] = selectValue(a, b, c);
    }
    {
      const short3 a{1, 2, 3};
      const short3 b{-1, -2, -3};
      const short3 c{kResultTrue<short3>, kResultFalse<short3>, kResultTrue<short3>};
      results3[1] = selectValue(a, b, c);
    }

    {
      const int4 a{1, 2, 3, 4};
      const int4 b{-1, -2, -3, -4};
      const int4 c{vfalse, vtrue, vfalse, vtrue};
      results4[0] = selectValue(a, b, c);
    }
    {
      const int4 a{1, 2, 3, 4};
      const int4 b{-1, -2, -3, -4};
      const int4 c{vtrue, vfalse, vtrue, vfalse};
      results4[1] = selectValue(a, b, c);
    }
  }
}

/*!
  */
__kernel void testSelectU(GlobalPtr<uint32b> results1,
    GlobalPtr<uchar2> results2,
    GlobalPtr<ushort3> results3,
    GlobalPtr<uint4> results4)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    constexpr int32b sfalse = kResultFalse<uint32b>;
    constexpr int32b strue = kResultTrue<uint32b>;
    constexpr int32b vfalse = kResultFalse<uint4>;
    constexpr int32b vtrue = kResultTrue<uint4>;

    {
      const uint32b a = 1u;
      const uint32b b = 2u;
      const int32b c = sfalse;
      results1[0] = selectValue(a, b, c);
    }
    {
      const uint32b a = 1u;
      const uint32b b = 2u;
      const int32b c = strue;
      results1[1] = selectValue(a, b, c);
    }

    {
      const uchar2 a{1u, 2u};
      const uchar2 b{3u, 4u};
      const char2 c{kResultFalse<char2>, kResultTrue<char2>};
      results2[0] = selectValue(a, b, c);
    }
    {
      const uchar2 a{1u, 2u};
      const uchar2 b{3u, 4u};
      const char2 c{kResultTrue<char2>, kResultFalse<char2>};
      results2[1] = selectValue(a, b, c);
    }

    {
      const ushort3 a{1u, 2u, 3u};
      const ushort3 b{4u, 5u, 6u};
      const short3 c{kResultFalse<short3>, kResultTrue<short3>, kResultFalse<short3>};
      results3[0] = selectValue(a, b, c);
    }
    {
      const ushort3 a{1u, 2u, 3u};
      const ushort3 b{4u, 5u, 6u};
      const short3 c{kResultTrue<short3>, kResultFalse<short3>, kResultTrue<short3>};
      results3[1] = selectValue(a, b, c);
    }

    {
      const uint4 a{1u, 2u, 3u, 4u};
      const uint4 b{5u, 6u, 7u, 8u};
      const int4 c{vfalse, vtrue, vfalse, vtrue};
      results4[0] = selectValue(a, b, c);
    }
    {
      const uint4 a{1u, 2u, 3u, 4u};
      const uint4 b{5u, 6u, 7u, 8u};
      const int4 c{vtrue, vfalse, vtrue, vfalse};
      results4[1] = selectValue(a, b, c);
    }
  }
}

/*!
  */
__kernel void testSelectF(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    constexpr int32b sfalse = kResultFalse<float>;
    constexpr int32b strue = kResultTrue<float>;
    constexpr int32b vfalse = kResultFalse<float4>;
    constexpr int32b vtrue = kResultTrue<float4>;

    {
      const float a = 1.0f;
      const float b =-1.0f;
      const int32b c = sfalse;
      results1[0] = selectValue(a, b, c);
    }
    {
      const float a = 1.0f;
      const float b = -1.0f;
      const int32b c = strue;
      results1[1] = selectValue(a, b, c);
    }

    {
      const float2 a{1.0f, 2.0f};
      const float2 b{-1.0f, -2.0f};
      const int2 c{vfalse, vtrue};
      results2[0] = selectValue(a, b, c);
    }
    {
      const float2 a{1.0f, 2.0f};
      const float2 b{-1.0f, -2.0f};
      const int2 c{vtrue, vfalse};
      results2[1] = selectValue(a, b, c);
    }

    {
      const float3 a{1.0f, 2.0f, 3.0f};
      const float3 b{-1.0f, -2.0f, -3.0f};
      const int3 c{vfalse, vtrue, vfalse};
      results3[0] = selectValue(a, b, c);
    }
    {
      const float3 a{1.0f, 2.0f, 3.0f};
      const float3 b{-1.0f, -2.0f, -3.0f};
      const int3 c{vtrue, vfalse, vtrue};
      results3[1] = selectValue(a, b, c);
    }

    {
      const float4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const float4 b{-1.0f, -2.0f, -3.0f, -4.0f};
      const int4 c{vfalse, vtrue, vfalse, vtrue};
      results4[0] = selectValue(a, b, c);
    }
    {
      const float4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const float4 b{-1.0f, -2.0f, -3.0f, -4.0f};
      const int4 c{vtrue, vfalse, vtrue, vfalse};
      results4[1] = selectValue(a, b, c);
    }
  }
}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL */
