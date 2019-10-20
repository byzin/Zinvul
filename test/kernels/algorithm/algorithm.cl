/*!
  \file algorithm.cl
  \author Sho Ikeda
  */

#ifndef ZINVUL_ALGORITHM_TEST_ALGORITHM_CL
#define ZINVUL_ALGORITHM_TEST_ALGORITHM_CL

// Zinvul
#include "zinvul/cl/algorithm.cl"
#include "zinvul/cl/limits.cl"
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
using zinvul::Private;
using zinvul::PrivatePtr;
using zinvul::ConstPrivatePtr;
using zinvul::cast;
using zinvul::treatAs;

// Forward declaration
__kernel void testAbsInt8(GlobalPtr<uint8b> results1,
    GlobalPtr<uchar2> results2,
    GlobalPtr<uchar3> results3,
    GlobalPtr<uchar4> results4);
__kernel void testAbsInt16(GlobalPtr<uint16b> results1,
    GlobalPtr<ushort2> results2,
    GlobalPtr<ushort3> results3,
    GlobalPtr<ushort4> results4);
__kernel void testAbsInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4);
__kernel void testAbsUInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4);
__kernel void testAbsFloat(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4);
__kernel void testClampInt8(GlobalPtr<int8b> results1,
    GlobalPtr<char2> results2,
    GlobalPtr<char3> results3,
    GlobalPtr<char4> results4);
__kernel void testClampInt16(GlobalPtr<int16b> results1,
    GlobalPtr<short2> results2,
    GlobalPtr<short3> results3,
    GlobalPtr<short4> results4);
__kernel void testClampInt32(GlobalPtr<int32b> results1,
    GlobalPtr<int2> results2,
    GlobalPtr<int3> results3,
    GlobalPtr<int4> results4);
__kernel void testClampUInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4);
__kernel void testClampFloat(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4);
__kernel void testMaxInt8(GlobalPtr<int8b> results1,
    GlobalPtr<char2> results2,
    GlobalPtr<char3> results3,
    GlobalPtr<char4> results4);
__kernel void testMaxInt16(GlobalPtr<int16b> results1,
    GlobalPtr<short2> results2,
    GlobalPtr<short3> results3,
    GlobalPtr<short4> results4);
__kernel void testMaxInt32(GlobalPtr<int32b> results1,
    GlobalPtr<int2> results2,
    GlobalPtr<int3> results3,
    GlobalPtr<int4> results4);
__kernel void testMaxUInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4);
__kernel void testMaxFloat(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4);
__kernel void testMinInt8(GlobalPtr<int8b> results1,
    GlobalPtr<char2> results2,
    GlobalPtr<char3> results3,
    GlobalPtr<char4> results4);
__kernel void testMinInt16(GlobalPtr<int16b> results1,
    GlobalPtr<short2> results2,
    GlobalPtr<short3> results3,
    GlobalPtr<short4> results4);
__kernel void testMinInt32(GlobalPtr<int32b> results1,
    GlobalPtr<int2> results2,
    GlobalPtr<int3> results3,
    GlobalPtr<int4> results4);
__kernel void testMinUInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4);
__kernel void testMinFloat(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4);
__kernel void testMixScalar(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4);
__kernel void testMix(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4);
__kernel void testSign(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4);
__kernel void testPopcount8(ConstGlobalPtr<uint8b> inputs,
    GlobalPtr<uint8b> results1,
    GlobalPtr<uchar2> results2,
    GlobalPtr<uchar3> results3,
    GlobalPtr<uchar4> results4,
    const uint32b resolution);
__kernel void testPopcount16(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<uint16b> results1,
    GlobalPtr<ushort2> results2,
    GlobalPtr<ushort3> results3,
    GlobalPtr<ushort4> results4,
    const uint32b resolution);
__kernel void testPopcount32(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4,
    const uint32b resolution);
#if !defined(Z_MAC)
__kernel void testPopcount64(ConstGlobalPtr<uint64b> inputs,
    GlobalPtr<uint64b> results1,
    GlobalPtr<ulong2> results2,
    GlobalPtr<ulong3> results3,
    GlobalPtr<ulong4> results4,
    const uint32b resolution);
#endif // Z_MAC

__kernel void testAbsInt8(GlobalPtr<uint8b> results1,
    GlobalPtr<uchar2> results2,
    GlobalPtr<uchar3> results3,
    GlobalPtr<uchar4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      const int8b v = 0;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int8b v = 1;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int8b v = -1;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int8b v = zinvul::NumericLimits<int8b>::max();
      results1[i++] = zinvul::abs(v);
    }
    {
      const int8b v = zinvul::NumericLimits<int8b>::min();
      results1[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const char2 v{0, 0};
      results2[i++] = zinvul::abs(v);
    }
    {
      const char2 v{1, -1};
      results2[i++] = zinvul::abs(v);
    }
    {
      const char2 v{zinvul::NumericLimits<int8b>::max(),
                    zinvul::NumericLimits<int8b>::min()};
      results2[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const char3 v{0, 0, 0};
      results3[i++] = zinvul::abs(v);
    }
    {
      const char3 v{1, -1, 0};
      results3[i++] = zinvul::abs(v);
    }
    {
      const char3 v{zinvul::NumericLimits<int8b>::max(),
                    zinvul::NumericLimits<int8b>::min(),
                    zinvul::NumericLimits<int8b>::lowest()};
      results3[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const char4 v{0, 0, 0, 0};
      results4[i++] = zinvul::abs(v);
    }
    {
      const char4 v{1, -1, 0, 0};
      results4[i++] = zinvul::abs(v);
    }
    {
      const char4 v{zinvul::NumericLimits<int8b>::max(),
                    zinvul::NumericLimits<int8b>::min(),
                    zinvul::NumericLimits<int8b>::lowest(),
                    0};
      results4[i++] = zinvul::abs(v);
    }
  }
}

__kernel void testAbsInt16(GlobalPtr<uint16b> results1,
    GlobalPtr<ushort2> results2,
    GlobalPtr<ushort3> results3,
    GlobalPtr<ushort4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      const int16b v = 0;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int16b v = 1;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int16b v = -1;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int16b v = zinvul::NumericLimits<int16b>::max();
      results1[i++] = zinvul::abs(v);
    }
    {
      const int16b v = zinvul::NumericLimits<int16b>::min();
      results1[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const short2 v{0, 0};
      results2[i++] = zinvul::abs(v);
    }
    {
      const short2 v{1, -1};
      results2[i++] = zinvul::abs(v);
    }
    {
      const short2 v{zinvul::NumericLimits<int16b>::max(),
                     zinvul::NumericLimits<int16b>::min()};
      results2[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const short3 v{0, 0, 0};
      results3[i++] = zinvul::abs(v);
    }
    {
      const short3 v{1, -1, 0};
      results3[i++] = zinvul::abs(v);
    }
    {
      const short3 v{zinvul::NumericLimits<int16b>::max(),
                     zinvul::NumericLimits<int16b>::min(),
                     zinvul::NumericLimits<int16b>::lowest()};
      results3[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const short4 v{0, 0, 0, 0};
      results4[i++] = zinvul::abs(v);
    }
    {
      const short4 v{1, -1, 0, 0};
      results4[i++] = zinvul::abs(v);
    }
    {
      const short4 v{zinvul::NumericLimits<int16b>::max(),
                     zinvul::NumericLimits<int16b>::min(),
                     zinvul::NumericLimits<int16b>::lowest(),
                     0};
      results4[i++] = zinvul::abs(v);
    }
  }
}

__kernel void testAbsInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      const int32b v = 0;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int32b v = 1;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int32b v = -1;
      results1[i++] = zinvul::abs(v);
    }
    {
      const int32b v = zinvul::NumericLimits<int32b>::max();
      results1[i++] = zinvul::abs(v);
    }
    {
      const int32b v = zinvul::NumericLimits<int32b>::min();
      results1[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const int2 v{0, 0};
      results2[i++] = zinvul::abs(v);
    }
    {
      const int2 v{1, -1};
      results2[i++] = zinvul::abs(v);
    }
    {
      const int2 v{zinvul::NumericLimits<int32b>::max(),
                   zinvul::NumericLimits<int32b>::min()};
      results2[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const int3 v{0, 0, 0};
      results3[i++] = zinvul::abs(v);
    }
    {
      const int3 v{1, -1, 0};
      results3[i++] = zinvul::abs(v);
    }
    {
      const int3 v{zinvul::NumericLimits<int32b>::max(),
                   zinvul::NumericLimits<int32b>::min(),
                   zinvul::NumericLimits<int32b>::lowest()};
      results3[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const int4 v{0, 0, 0, 0};
      results4[i++] = zinvul::abs(v);
    }
    {
      const int4 v{1, -1, 0, 0};
      results4[i++] = zinvul::abs(v);
    }
    {
      const int4 v{zinvul::NumericLimits<int32b>::max(),
                   zinvul::NumericLimits<int32b>::min(),
                   zinvul::NumericLimits<int32b>::lowest(),
                   0};
      results4[i++] = zinvul::abs(v);
    }
  }
}

__kernel void testAbsUInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      const uint32b v = 0;
      results1[i++] = zinvul::abs(v);
    }
    {
      const uint32b v = 1;
      results1[i++] = zinvul::abs(v);
    }
    {
      const uint32b v = zinvul::NumericLimits<uint32b>::max();
      results1[i++] = zinvul::abs(v);
    }
    {
      const uint32b v = zinvul::NumericLimits<uint32b>::min();
      results1[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const uint2 v{0u, 0u};
      results2[i++] = zinvul::abs(v);
    }
    {
      const uint2 v{1u, 1u};
      results2[i++] = zinvul::abs(v);
    }
    {
      const uint2 v{zinvul::NumericLimits<uint32b>::max(),
                    zinvul::NumericLimits<uint32b>::min()};
      results2[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const uint3 v{0u, 0u, 0u};
      results3[i++] = zinvul::abs(v);
    }
    {
      const uint3 v{1u, 0u, 0u};
      results3[i++] = zinvul::abs(v);
    }
    {
      const uint3 v{zinvul::NumericLimits<uint32b>::max(),
                    zinvul::NumericLimits<uint32b>::min(),
                    zinvul::NumericLimits<uint32b>::lowest()};
      results3[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const uint4 v{1u, 1u, 0u, 0u};
      results4[i++] = zinvul::abs(v);
    }
    {
      const uint4 v{zinvul::NumericLimits<uint32b>::max(),
                    zinvul::NumericLimits<uint32b>::min(),
                    zinvul::NumericLimits<uint32b>::lowest(),
                    0u};
      results4[i++] = zinvul::abs(v);
    }
  }
}

__kernel void testAbsFloat(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      const float v = 0.0f;
      results1[i++] = zinvul::abs(v);
    }
    {
      const float v = 1.0f;
      results1[i++] = zinvul::abs(v);
    }
    {
      const float v = -1.0f;
      results1[i++] = zinvul::abs(v);
    }
    {
      const float v = zinvul::NumericLimits<float>::max();
      results1[i++] = zinvul::abs(v);
    }
    {
      const float v = zinvul::NumericLimits<float>::lowest();
      results1[i++] = zinvul::abs(v);
    }
    {
      const float v = zinvul::NumericLimits<float>::min();
      results1[i++] = zinvul::abs(v);
    }
    {
      const float v = -zinvul::NumericLimits<float>::min();
      results1[i++] = zinvul::abs(v);
    }
    {
      const float v = zinvul::NumericLimits<float>::infinity();
      results1[i++] = zinvul::abs(v);
    }
    {
      const float v = -zinvul::NumericLimits<float>::infinity();
      results1[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const float2 v{0.0f, -0.0f};
      results2[i++] = zinvul::abs(v);
    }
    {
      const float2 v{1.0f, -1.0f};
      results2[i++] = zinvul::abs(v);
    }
    {
      const float2 v{zinvul::NumericLimits<float>::max(),
                     zinvul::NumericLimits<float>::lowest()};
      results2[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const float3 v{0.0f, -0.0f, 0.0f};
      results3[i++] = zinvul::abs(v);
    }
    {
      const float3 v{1.0f, -1.0f, zinvul::NumericLimits<float>::epsilon()};
      results3[i++] = zinvul::abs(v);
    }
    {
      const float3 v{zinvul::NumericLimits<float>::max(),
                     zinvul::NumericLimits<float>::min(),
                     zinvul::NumericLimits<float>::lowest()};
      results3[i++] = zinvul::abs(v);
    }

    i = 0;
    {
      const float4 v{0.0f, -0.0f, 1.0f, -1.0f};
      results4[i++] = zinvul::abs(v);
    }
    {
      const float4 v{zinvul::NumericLimits<float>::epsilon(),
                     -zinvul::NumericLimits<float>::epsilon(),
                     zinvul::NumericLimits<float>::infinity(),
                     -zinvul::NumericLimits<float>::infinity()};
      results4[i++] = zinvul::abs(v);
    }
    {
      const float4 v{zinvul::NumericLimits<float>::max(),
                     zinvul::NumericLimits<float>::min(),
                     zinvul::NumericLimits<float>::lowest(),
                     0.0f};
      results4[i++] = zinvul::abs(v);
    }
  }
}

__kernel void testClampInt8(GlobalPtr<int8b> results1,
    GlobalPtr<char2> results2,
    GlobalPtr<char3> results3,
    GlobalPtr<char4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr int8b minval = -1;
      constexpr int8b maxval = 1;
      int8b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int8b minval = 20;
      constexpr int8b maxval = 20;
      int8b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int8b minval = -20;
      constexpr int8b maxval = -20;
      int8b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int8b minval = zinvul::NumericLimits<int8b>::min();
      constexpr int8b maxval = zinvul::NumericLimits<int8b>::max();
      int8b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const auto minval = zinvul::makeChar2(-1);
      const auto maxval = zinvul::makeChar2(1);
      const char2 v{0, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar2(20);
      const auto maxval = zinvul::makeChar2(20);
      const char2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar2(-20);
      const auto maxval = zinvul::makeChar2(-20);
      const char2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar2(zinvul::NumericLimits<int8b>::min());
      const auto maxval = zinvul::makeChar2(zinvul::NumericLimits<int8b>::max());
      const char2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const auto minval = zinvul::makeChar3(-1);
      const auto maxval = zinvul::makeChar3(1);
      const char3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar3(20);
      const auto maxval = zinvul::makeChar3(20);
      const char3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar3(-20);
      const auto maxval = zinvul::makeChar3(-20);
      const char3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar3(zinvul::NumericLimits<int8b>::min());
      const auto maxval = zinvul::makeChar3(zinvul::NumericLimits<int8b>::max());
      const char3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const auto minval = zinvul::makeChar4(-1);
      const auto maxval = zinvul::makeChar4(1);
      const char4 v{0, 10, -10, zinvul::NumericLimits<int8b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar4(20);
      const auto maxval = zinvul::makeChar4(20);
      const char4 v{0, 10, -10, zinvul::NumericLimits<int8b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar4(-20);
      const auto maxval = zinvul::makeChar4(-20);
      const char4 v{0, 10, -10, zinvul::NumericLimits<int8b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeChar4(zinvul::NumericLimits<int8b>::min());
      const auto maxval = zinvul::makeChar4(zinvul::NumericLimits<int8b>::max());
      const char4 v{0, 10, -10, zinvul::NumericLimits<int8b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
  }
}

__kernel void testClampInt16(GlobalPtr<int16b> results1,
    GlobalPtr<short2> results2,
    GlobalPtr<short3> results3,
    GlobalPtr<short4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr int16b minval = -1;
      constexpr int16b maxval = 1;
      int16b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int16b minval = 20;
      constexpr int16b maxval = 20;
      int16b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int16b minval = -20;
      constexpr int16b maxval = -20;
      int16b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int16b minval = zinvul::NumericLimits<int16b>::min();
      constexpr int16b maxval = zinvul::NumericLimits<int16b>::max();
      int16b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const auto minval = zinvul::makeShort2(-1);
      const auto maxval = zinvul::makeShort2(1);
      const short2 v{0, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort2(20);
      const auto maxval = zinvul::makeShort2(20);
      const short2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort2(-20);
      const auto maxval = zinvul::makeShort2(-20);
      const short2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort2(zinvul::NumericLimits<int16b>::min());
      const auto maxval = zinvul::makeShort2(zinvul::NumericLimits<int16b>::max());
      const short2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const auto minval = zinvul::makeShort3(-1);
      const auto maxval = zinvul::makeShort3(1);
      const short3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort3(20);
      const auto maxval = zinvul::makeShort3(20);
      const short3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort3(-20);
      const auto maxval = zinvul::makeShort3(-20);
      const short3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort3(zinvul::NumericLimits<int16b>::min());
      const auto maxval = zinvul::makeShort3(zinvul::NumericLimits<int16b>::max());
      const short3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const auto minval = zinvul::makeShort4(-1);
      const auto maxval = zinvul::makeShort4(1);
      const short4 v{0, 10, -10, zinvul::NumericLimits<int16b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort4(20);
      const auto maxval = zinvul::makeShort4(20);
      const short4 v{0, 10, -10, zinvul::NumericLimits<int16b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort4(-20);
      const auto maxval = zinvul::makeShort4(-20);
      const short4 v{0, 10, -10, zinvul::NumericLimits<int16b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeShort4(zinvul::NumericLimits<int16b>::min());
      const auto maxval = zinvul::makeShort4(zinvul::NumericLimits<int16b>::max());
      const short4 v{0, 10, -10, zinvul::NumericLimits<int16b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
  }
}

__kernel void testClampInt32(GlobalPtr<int32b> results1,
    GlobalPtr<int2> results2,
    GlobalPtr<int3> results3,
    GlobalPtr<int4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr int32b minval = -1;
      constexpr int32b maxval = 1;
      int32b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = 20;
      constexpr int32b maxval = 20;
      int32b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = -20;
      constexpr int32b maxval = -20;
      int32b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = zinvul::NumericLimits<int32b>::min();
      constexpr int32b maxval = zinvul::NumericLimits<int32b>::max();
      int32b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      constexpr int32b minval = -1;
      constexpr int32b maxval = 1;
      const int2 v{0, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = 20;
      constexpr int32b maxval = 20;
      const int2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = -20;
      constexpr int32b maxval = -20;
      const int2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = zinvul::NumericLimits<int32b>::min();
      constexpr int32b maxval = zinvul::NumericLimits<int32b>::max();
      const int2 v{10, -10};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      constexpr int32b minval = -1;
      constexpr int32b maxval = 1;
      const int3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = 20;
      constexpr int32b maxval = 20;
      const int3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = -20;
      constexpr int32b maxval = -20;
      const int3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = zinvul::NumericLimits<int32b>::min();
      constexpr int32b maxval = zinvul::NumericLimits<int32b>::max();
      const int3 v{0, 10, -10};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      constexpr int32b minval = -1;
      constexpr int32b maxval = 1;
      const int4 v{0, 10, -10, zinvul::NumericLimits<int32b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = 20;
      constexpr int32b maxval = 20;
      const int4 v{0, 10, -10, zinvul::NumericLimits<int32b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = -20;
      constexpr int32b maxval = -20;
      const int4 v{0, 10, -10, zinvul::NumericLimits<int32b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr int32b minval = zinvul::NumericLimits<int32b>::min();
      constexpr int32b maxval = zinvul::NumericLimits<int32b>::max();
      const int4 v{0, 10, -10, zinvul::NumericLimits<int32b>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
  }
}

__kernel void testClampUInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr uint32b minval = 0u;
      constexpr uint32b maxval = 1u;
      uint32b v = 0u;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10u;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr uint32b minval = 20;
      constexpr uint32b maxval = 20;
      uint32b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr uint32b minval = 0u;
      constexpr uint32b maxval = zinvul::NumericLimits<uint32b>::max();
      uint32b v = 0;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      constexpr uint32b minval = 0u;
      constexpr uint32b maxval = 1u;
      const uint2 v{0u, 10u};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr uint32b minval = 20u;
      constexpr uint32b maxval = 20u;
      const uint2 v{0u, 10u};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr uint32b minval = 0u;
      constexpr uint32b maxval = zinvul::NumericLimits<uint32b>::max();
      const uint2 v{0u, 10u};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      constexpr uint32b minval = 0u;
      constexpr uint32b maxval = 10u;
      const uint3 v{0u, 10, zinvul::NumericLimits<uint32b>::max()};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr uint32b minval = 20u;
      constexpr uint32b maxval = 20u;
      const uint3 v{0u, 10, zinvul::NumericLimits<uint32b>::max()};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr uint32b minval = 0u;
      constexpr uint32b maxval = zinvul::NumericLimits<uint32b>::max();
      const uint3 v{0u, 10, zinvul::NumericLimits<uint32b>::max()};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      constexpr uint32b minval = 0u;
      constexpr uint32b maxval = 1u;
      const uint4 v{0u, 10u, zinvul::NumericLimits<uint32b>::max(), 100u};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr uint32b minval = 20u;
      constexpr uint32b maxval = 20u;
      const uint4 v{0u, 10u, zinvul::NumericLimits<uint32b>::max(), 100u};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr uint32b minval = 0u;
      constexpr uint32b maxval = zinvul::NumericLimits<uint32b>::max();
      const uint4 v{0u, 10u, zinvul::NumericLimits<uint32b>::max(), 100u};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
  }
}

__kernel void testClampFloat(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr float minval = -1.0f;
      constexpr float maxval = 1.0f;
      float v = 0.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr float minval = 20;
      constexpr float maxval = 20;
      float v = 0.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr float minval = -20.0f;
      constexpr float maxval = -20.0f;
      float v = 0.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      constexpr float minval = zinvul::NumericLimits<float>::lowest();
      constexpr float maxval = zinvul::NumericLimits<float>::max();
      float v = 0.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = -10.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
      v = 10.0f;
      results1[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const float2 minval{-1.0f, -1.0f};
      const float2 maxval{1.0f, 1.0f};
      const float2 v{0.0f, -10.0f};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const float2 minval{20.0f, 20.0f};
      const float2 maxval{20.0f, 20.0f};
      const float2 v{10.0f, -10.0f};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const float2 minval{-20.0f, -20.0f};
      const float2 maxval{-20.0f, -20.0f};
      const float2 v{10.0f, -10.0f};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeFloat2(zinvul::NumericLimits<float>::lowest());
      const auto maxval = zinvul::makeFloat2(zinvul::NumericLimits<float>::max());
      const float2 v{10.0f, -10.0f};
      results2[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const auto minval = zinvul::makeFloat3(-1.0f);
      const auto maxval = zinvul::makeFloat3(1.0f);
      const float3 v{0.0f, 10.0f, -10.0f};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeFloat3(20.0f);
      const auto maxval = zinvul::makeFloat3(20.0f);
      const float3 v{0.0f, 10.0f, -10.0f};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeFloat3(-20.0f);
      const auto maxval = zinvul::makeFloat3(-20.0f);
      const float3 v{0.0f, 10.0f, -10.0f};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeFloat3(zinvul::NumericLimits<float>::lowest());
      const auto maxval = zinvul::makeFloat3(zinvul::NumericLimits<float>::max());
      const float3 v{0.0f, 10.0f, -10.0f};
      results3[i++] = zinvul::clamp(v, minval, maxval);
    }

    i = 0;
    {
      const auto minval = zinvul::makeFloat4(-1.0f);
      const auto maxval = zinvul::makeFloat4(1.0f);
      const float4 v{0.0f, 10.0f, -10.0f, zinvul::NumericLimits<float>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeFloat4(20.0f);
      const auto maxval = zinvul::makeFloat4(20.0f);
      const float4 v{0.0f, 10.0f, -10.0f, zinvul::NumericLimits<float>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeFloat4(-20.0f);
      const auto maxval = zinvul::makeFloat4(-20.0f);
      const float4 v{0.0f, 10.0f, -10.0f, zinvul::NumericLimits<float>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = zinvul::makeFloat4(zinvul::NumericLimits<float>::lowest());
      const auto maxval = zinvul::makeFloat4(zinvul::NumericLimits<float>::max());
      const float4 v{0.0f, 10.0f, -10.0f, zinvul::NumericLimits<float>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
    {
      const auto minval = -zinvul::makeFloat4(zinvul::NumericLimits<float>::infinity());
      const auto maxval = zinvul::makeFloat4(zinvul::NumericLimits<float>::infinity());
      const float4 v{0.0f, 10.0f, -10.0f, zinvul::NumericLimits<float>::min()};
      results4[i++] = zinvul::clamp(v, minval, maxval);
    }
  }
}

__kernel void testMaxInt8(GlobalPtr<int8b> results1,
    GlobalPtr<char2> results2,
    GlobalPtr<char3> results3,
    GlobalPtr<char4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr int8b maxi = zinvul::NumericLimits<int8b>::max();
    constexpr int8b mini = zinvul::NumericLimits<int8b>::min();
    size_t i = 0;
    {
      const int8b lhs = -1;
      const int8b rhs = 1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int8b lhs = mini;
      const int8b rhs = -1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int8b lhs = maxi;
      const int8b rhs = 1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int8b lhs = maxi;
      const int8b rhs = mini;
      results1[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const char2 lhs = zinvul::makeChar2(-1, 1);
      const char2 rhs = zinvul::makeChar2(1, -1);
      results2[i++] = zinvul::max(lhs, rhs);
    }
    {
      const char2 lhs = zinvul::makeChar2(mini, maxi);
      const char2 rhs = zinvul::makeChar2(maxi, mini);
      results2[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const char3 lhs = zinvul::makeChar3(-1, 0, 1);
      const char3 rhs = zinvul::makeChar3(1, -1, 0);
      results3[i++] = zinvul::max(lhs, rhs);
    }
    {
      const char3 lhs = zinvul::makeChar3(mini, 0, maxi);
      const char3 rhs = zinvul::makeChar3(maxi, mini, 0);
      results3[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const char4 lhs = zinvul::makeChar4(-1, 0, 1, 0);
      const char4 rhs = zinvul::makeChar4(1, -1, 0, 0);
      results4[i++] = zinvul::max(lhs, rhs);
    }
    {
      const char4 lhs = zinvul::makeChar4(mini, 0, maxi, maxi);
      const char4 rhs = zinvul::makeChar4(maxi, mini, 0, mini);
      results4[i++] = zinvul::max(lhs, rhs);
    }
  }
}

__kernel void testMaxInt16(GlobalPtr<int16b> results1,
    GlobalPtr<short2> results2,
    GlobalPtr<short3> results3,
    GlobalPtr<short4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr int16b maxi = zinvul::NumericLimits<int16b>::max();
    constexpr int16b mini = zinvul::NumericLimits<int16b>::min();
    size_t i = 0;
    {
      const int16b lhs = -1;
      const int16b rhs = 1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int16b lhs = mini;
      const int16b rhs = -1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int16b lhs = maxi;
      const int16b rhs = 1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int16b lhs = maxi;
      const int16b rhs = mini;
      results1[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const short2 lhs = zinvul::makeShort2(-1, 1);
      const short2 rhs = zinvul::makeShort2(1, -1);
      results2[i++] = zinvul::max(lhs, rhs);
    }
    {
      const short2 lhs = zinvul::makeShort2(mini, maxi);
      const short2 rhs = zinvul::makeShort2(maxi, mini);
      results2[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const short3 lhs = zinvul::makeShort3(-1, 0, 1);
      const short3 rhs = zinvul::makeShort3(1, -1, 0);
      results3[i++] = zinvul::max(lhs, rhs);
    }
    {
      const short3 lhs = zinvul::makeShort3(mini, 0, maxi);
      const short3 rhs = zinvul::makeShort3(maxi, mini, 0);
      results3[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const short4 lhs = zinvul::makeShort4(-1, 0, 1, 0);
      const short4 rhs = zinvul::makeShort4(1, -1, 0, 0);
      results4[i++] = zinvul::max(lhs, rhs);
    }
    {
      const short4 lhs = zinvul::makeShort4(mini, 0, maxi, maxi);
      const short4 rhs = zinvul::makeShort4(maxi, mini, 0, mini);
      results4[i++] = zinvul::max(lhs, rhs);
    }
  }
}

__kernel void testMaxInt32(GlobalPtr<int32b> results1,
    GlobalPtr<int2> results2,
    GlobalPtr<int3> results3,
    GlobalPtr<int4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr int32b maxi = zinvul::NumericLimits<int32b>::max();
    constexpr int32b mini = zinvul::NumericLimits<int32b>::min();
    size_t i = 0;
    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int32b lhs = mini;
      const int32b rhs = -1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int32b lhs = maxi;
      const int32b rhs = 1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int32b lhs = maxi;
      const int32b rhs = mini;
      results1[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const int2 lhs = zinvul::makeInt2(-1, 1);
      const int2 rhs = zinvul::makeInt2(1, -1);
      results2[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int2 lhs = zinvul::makeInt2(mini, maxi);
      const int2 rhs = zinvul::makeInt2(maxi, mini);
      results2[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const int3 lhs = zinvul::makeInt3(-1, 0, 1);
      const int3 rhs = zinvul::makeInt3(1, -1, 0);
      results3[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int3 lhs = zinvul::makeInt3(mini, 0, maxi);
      const int3 rhs = zinvul::makeInt3(maxi, mini, 0);
      results3[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const int4 lhs = zinvul::makeInt4(-1, 0, 1, 0);
      const int4 rhs = zinvul::makeInt4(1, -1, 0, 0);
      results4[i++] = zinvul::max(lhs, rhs);
    }
    {
      const int4 lhs = zinvul::makeInt4(mini, 0, maxi, maxi);
      const int4 rhs = zinvul::makeInt4(maxi, mini, 0, mini);
      results4[i++] = zinvul::max(lhs, rhs);
    }
  }
}

__kernel void testMaxUInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr uint32b maxi = zinvul::NumericLimits<uint32b>::max();
    constexpr uint32b mini = zinvul::NumericLimits<uint32b>::min();
    size_t i = 0;
    {
      const uint32b lhs = 0;
      const uint32b rhs = 1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const uint32b lhs = mini;
      const uint32b rhs = 0;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const uint32b lhs = maxi;
      const uint32b rhs = 1;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const uint32b lhs = maxi;
      const uint32b rhs = mini;
      results1[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const uint2 lhs = zinvul::makeUInt2(1u, 0u);
      const uint2 rhs = zinvul::makeUInt2(0u, 1u);
      results2[i++] = zinvul::max(lhs, rhs);
    }
    {
      const uint2 lhs = zinvul::makeUInt2(mini, maxi);
      const uint2 rhs = zinvul::makeUInt2(maxi, mini);
      results2[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const uint3 lhs = zinvul::makeUInt3(0u, 0u, 1u);
      const uint3 rhs = zinvul::makeUInt3(1u, 1u, 0u);
      results3[i++] = zinvul::max(lhs, rhs);
    }
    {
      const uint3 lhs = zinvul::makeUInt3(mini, 0u, maxi);
      const uint3 rhs = zinvul::makeUInt3(maxi, mini, 0u);
      results3[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const uint4 lhs = zinvul::makeUInt4(0u, 0u, 1u, 1u);
      const uint4 rhs = zinvul::makeUInt4(1u, 0u, 0u, 1u);
      results4[i++] = zinvul::max(lhs, rhs);
    }
    {
      const uint4 lhs = zinvul::makeUInt4(mini, 0u, maxi, maxi);
      const uint4 rhs = zinvul::makeUInt4(maxi, mini, 0u, mini);
      results4[i++] = zinvul::max(lhs, rhs);
    }
  }
}

__kernel void testMaxFloat(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr float maxi = zinvul::NumericLimits<float>::max();
    constexpr float mini = zinvul::NumericLimits<float>::lowest();
    size_t i = 0;
    {
      const float lhs = -1.0f;
      const float rhs = 1.0f;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const float lhs = mini;
      const float rhs = -1.0f;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const float lhs = maxi;
      const float rhs = 1.0f;
      results1[i++] = zinvul::max(lhs, rhs);
    }
    {
      const float lhs = maxi;
      const float rhs = mini;
      results1[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const float2 lhs = zinvul::makeFloat2(-1.0f, 1.0f);
      const float2 rhs = zinvul::makeFloat2(1.0f, -1.0f);
      results2[i++] = zinvul::max(lhs, rhs);
    }
    {
      const float2 lhs = zinvul::makeFloat2(mini, maxi);
      const float2 rhs = zinvul::makeFloat2(maxi, mini);
      results2[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const float3 lhs = zinvul::makeFloat3(-1.0f, 0.0f, 1.0f);
      const float3 rhs = zinvul::makeFloat3(1.0f, -1.0f, 0.0f);
      results3[i++] = zinvul::max(lhs, rhs);
    }
    {
      const float3 lhs = zinvul::makeFloat3(mini, 0.0f, maxi);
      const float3 rhs = zinvul::makeFloat3(maxi, mini, 0.0f);
      results3[i++] = zinvul::max(lhs, rhs);
    }

    i = 0;
    {
      const float4 lhs = zinvul::makeFloat4(-1.0f, 0.0f, 1.0f, 0.0f);
      const float4 rhs = zinvul::makeFloat4(1.0f, -1.0f, 0.0f, 0.0f);
      results4[i++] = zinvul::max(lhs, rhs);
    }
    {
      const float4 lhs = zinvul::makeFloat4(mini, 0.0f, maxi, maxi);
      const float4 rhs = zinvul::makeFloat4(maxi, mini, 0.0f, mini);
      results4[i++] = zinvul::max(lhs, rhs);
    }
  }
}

__kernel void testMinInt8(GlobalPtr<int8b> results1,
    GlobalPtr<char2> results2,
    GlobalPtr<char3> results3,
    GlobalPtr<char4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr int8b maxi = zinvul::NumericLimits<int8b>::max();
    constexpr int8b mini = zinvul::NumericLimits<int8b>::min();
    size_t i = 0;
    {
      const int8b lhs = -1;
      const int8b rhs = 1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int8b lhs = mini;
      const int8b rhs = -1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int8b lhs = maxi;
      const int8b rhs = 1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int8b lhs = maxi;
      const int8b rhs = mini;
      results1[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const char2 lhs = zinvul::makeChar2(-1, 1);
      const char2 rhs = zinvul::makeChar2(1, -1);
      results2[i++] = zinvul::min(lhs, rhs);
    }
    {
      const char2 lhs = zinvul::makeChar2(mini, maxi);
      const char2 rhs = zinvul::makeChar2(maxi, mini);
      results2[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const char3 lhs = zinvul::makeChar3(-1, 0, 1);
      const char3 rhs = zinvul::makeChar3(1, -1, 0);
      results3[i++] = zinvul::min(lhs, rhs);
    }
    {
      const char3 lhs = zinvul::makeChar3(mini, 0, maxi);
      const char3 rhs = zinvul::makeChar3(maxi, mini, 0);
      results3[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const char4 lhs = zinvul::makeChar4(-1, 0, 1, 0);
      const char4 rhs = zinvul::makeChar4(1, -1, 0, 0);
      results4[i++] = zinvul::min(lhs, rhs);
    }
    {
      const char4 lhs = zinvul::makeChar4(mini, 0, maxi, maxi);
      const char4 rhs = zinvul::makeChar4(maxi, mini, 0, mini);
      results4[i++] = zinvul::min(lhs, rhs);
    }
  }
}

__kernel void testMinInt16(GlobalPtr<int16b> results1,
    GlobalPtr<short2> results2,
    GlobalPtr<short3> results3,
    GlobalPtr<short4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr int16b maxi = zinvul::NumericLimits<int16b>::max();
    constexpr int16b mini = zinvul::NumericLimits<int16b>::min();
    size_t i = 0;
    {
      const int16b lhs = -1;
      const int16b rhs = 1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int16b lhs = mini;
      const int16b rhs = -1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int16b lhs = maxi;
      const int16b rhs = 1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int16b lhs = maxi;
      const int16b rhs = mini;
      results1[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const short2 lhs = zinvul::makeShort2(-1, 1);
      const short2 rhs = zinvul::makeShort2(1, -1);
      results2[i++] = zinvul::min(lhs, rhs);
    }
    {
      const short2 lhs = zinvul::makeShort2(mini, maxi);
      const short2 rhs = zinvul::makeShort2(maxi, mini);
      results2[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const short3 lhs = zinvul::makeShort3(-1, 0, 1);
      const short3 rhs = zinvul::makeShort3(1, -1, 0);
      results3[i++] = zinvul::min(lhs, rhs);
    }
    {
      const short3 lhs = zinvul::makeShort3(mini, 0, maxi);
      const short3 rhs = zinvul::makeShort3(maxi, mini, 0);
      results3[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const short4 lhs = zinvul::makeShort4(-1, 0, 1, 0);
      const short4 rhs = zinvul::makeShort4(1, -1, 0, 0);
      results4[i++] = zinvul::min(lhs, rhs);
    }
    {
      const short4 lhs = zinvul::makeShort4(mini, 0, maxi, maxi);
      const short4 rhs = zinvul::makeShort4(maxi, mini, 0, mini);
      results4[i++] = zinvul::min(lhs, rhs);
    }
  }
}

__kernel void testMinInt32(GlobalPtr<int32b> results1,
    GlobalPtr<int2> results2,
    GlobalPtr<int3> results3,
    GlobalPtr<int4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr int32b maxi = zinvul::NumericLimits<int32b>::max();
    constexpr int32b mini = zinvul::NumericLimits<int32b>::min();
    size_t i = 0;
    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int32b lhs = mini;
      const int32b rhs = -1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int32b lhs = maxi;
      const int32b rhs = 1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int32b lhs = maxi;
      const int32b rhs = mini;
      results1[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const int2 lhs = zinvul::makeInt2(-1, 1);
      const int2 rhs = zinvul::makeInt2(1, -1);
      results2[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int2 lhs = zinvul::makeInt2(mini, maxi);
      const int2 rhs = zinvul::makeInt2(maxi, mini);
      results2[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const int3 lhs = zinvul::makeInt3(-1, 0, 1);
      const int3 rhs = zinvul::makeInt3(1, -1, 0);
      results3[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int3 lhs = zinvul::makeInt3(mini, 0, maxi);
      const int3 rhs = zinvul::makeInt3(maxi, mini, 0);
      results3[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const int4 lhs = zinvul::makeInt4(-1, 0, 1, 0);
      const int4 rhs = zinvul::makeInt4(1, -1, 0, 0);
      results4[i++] = zinvul::min(lhs, rhs);
    }
    {
      const int4 lhs = zinvul::makeInt4(mini, 0, maxi, maxi);
      const int4 rhs = zinvul::makeInt4(maxi, mini, 0, mini);
      results4[i++] = zinvul::min(lhs, rhs);
    }
  }
}

__kernel void testMinUInt32(GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr uint32b maxi = zinvul::NumericLimits<uint32b>::max();
    constexpr uint32b mini = zinvul::NumericLimits<uint32b>::min();
    size_t i = 0;
    {
      const uint32b lhs = 0;
      const uint32b rhs = 1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const uint32b lhs = mini;
      const uint32b rhs = 0;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const uint32b lhs = maxi;
      const uint32b rhs = 1;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const uint32b lhs = maxi;
      const uint32b rhs = mini;
      results1[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const uint2 lhs = zinvul::makeUInt2(1u, 0u);
      const uint2 rhs = zinvul::makeUInt2(0u, 1u);
      results2[i++] = zinvul::min(lhs, rhs);
    }
    {
      const uint2 lhs = zinvul::makeUInt2(mini, maxi);
      const uint2 rhs = zinvul::makeUInt2(maxi, mini);
      results2[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const uint3 lhs = zinvul::makeUInt3(0u, 0u, 1u);
      const uint3 rhs = zinvul::makeUInt3(1u, 1u, 0u);
      results3[i++] = zinvul::min(lhs, rhs);
    }
    {
      const uint3 lhs = zinvul::makeUInt3(mini, 0u, maxi);
      const uint3 rhs = zinvul::makeUInt3(maxi, mini, 0u);
      results3[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const uint4 lhs = zinvul::makeUInt4(0u, 0u, 1u, 1u);
      const uint4 rhs = zinvul::makeUInt4(1u, 0u, 0u, 1u);
      results4[i++] = zinvul::min(lhs, rhs);
    }
    {
      const uint4 lhs = zinvul::makeUInt4(mini, 0u, maxi, maxi);
      const uint4 rhs = zinvul::makeUInt4(maxi, mini, 0u, mini);
      results4[i++] = zinvul::min(lhs, rhs);
    }
  }
}

__kernel void testMinFloat(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr float maxi = zinvul::NumericLimits<float>::max();
    constexpr float mini = zinvul::NumericLimits<float>::lowest();
    size_t i = 0;
    {
      const float lhs = -1.0f;
      const float rhs = 1.0f;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const float lhs = mini;
      const float rhs = -1.0f;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const float lhs = maxi;
      const float rhs = 1.0f;
      results1[i++] = zinvul::min(lhs, rhs);
    }
    {
      const float lhs = maxi;
      const float rhs = mini;
      results1[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const float2 lhs = zinvul::makeFloat2(-1.0f, 1.0f);
      const float2 rhs = zinvul::makeFloat2(1.0f, -1.0f);
      results2[i++] = zinvul::min(lhs, rhs);
    }
    {
      const float2 lhs = zinvul::makeFloat2(mini, maxi);
      const float2 rhs = zinvul::makeFloat2(maxi, mini);
      results2[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const float3 lhs = zinvul::makeFloat3(-1.0f, 0.0f, 1.0f);
      const float3 rhs = zinvul::makeFloat3(1.0f, -1.0f, 0.0f);
      results3[i++] = zinvul::min(lhs, rhs);
    }
    {
      const float3 lhs = zinvul::makeFloat3(mini, 0.0f, maxi);
      const float3 rhs = zinvul::makeFloat3(maxi, mini, 0.0f);
      results3[i++] = zinvul::min(lhs, rhs);
    }

    i = 0;
    {
      const float4 lhs = zinvul::makeFloat4(-1.0f, 0.0f, 1.0f, 0.0f);
      const float4 rhs = zinvul::makeFloat4(1.0f, -1.0f, 0.0f, 0.0f);
      results4[i++] = zinvul::min(lhs, rhs);
    }
    {
      const float4 lhs = zinvul::makeFloat4(mini, 0.0f, maxi, maxi);
      const float4 rhs = zinvul::makeFloat4(maxi, mini, 0.0f, mini);
      results4[i++] = zinvul::min(lhs, rhs);
    }
  }
}

__kernel void testMixScalar(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr size_t a_size = 4;
  constexpr float a_list[a_size] = {0.0f, 0.5f, 0.7f, 1.0f};
  if (index == 0) {
    size_t i = 0;
    for (size_t j = 0; j < a_size; ++j) {
      {
        const float lhs = -1.0f;
        const float rhs = 1.0f;
        results1[i++] = zinvul::mix(lhs, rhs, a_list[j]);
      }
      {
        const float lhs = 1.0f;
        const float rhs = -1.0f;
        results1[i++] = zinvul::mix(lhs, rhs, a_list[j]);
      }
    }

    i = 0;
    for (size_t j = 0; j < a_size; ++j) {
      {
        const float2 lhs = zinvul::makeFloat2(-1.0f, 1.0f);
        const float2 rhs = zinvul::makeFloat2(1.0f, -1.0f);
        results2[i++] = zinvul::mix(lhs, rhs, a_list[j]);
      }
    }

    i = 0;
    for (size_t j = 0; j < a_size; ++j) {
      {
        const float3 lhs = zinvul::makeFloat3(-1.0f, 1.0f, -100.0f);
        const float3 rhs = zinvul::makeFloat3(1.0f, -1.0f, 100.0f);
        results3[i++] = zinvul::mix(lhs, rhs, a_list[j]);
      }
    }

    i = 0;
    for (size_t j = 0; j < a_size; ++j) {
      {
        const float4 lhs = zinvul::makeFloat4(-1.0f, 1.0f, -100.0f, 100.0f);
        const float4 rhs = zinvul::makeFloat4(1.0f, -1.0f, 100.0f, -100.0f);
        results4[i++] = zinvul::mix(lhs, rhs, a_list[j]);
      }
    }
  }
}

__kernel void testMix(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      const float lhs = -1.0f;
      const float rhs = 1.0f;
      const float a = 0.5f;
      results1[i++] = zinvul::mix(lhs, rhs, a);
    }
    {
      const float lhs = 1.0f;
      const float rhs = -1.0f;
      const float a = 0.5f;
      results1[i++] = zinvul::mix(lhs, rhs, a);
    }

    i = 0;
    {
      const float2 lhs = zinvul::makeFloat2(-1.0f, 1.0f);
      const float2 rhs = zinvul::makeFloat2(1.0f, -1.0f);
      const float2 a = zinvul::makeFloat2(0.5f, 0.7f);
      results2[i++] = zinvul::mix(lhs, rhs, a);
    }

    i = 0;
    {
      const float3 lhs = zinvul::makeFloat3(-1.0f, 1.0f, -100.0f);
      const float3 rhs = zinvul::makeFloat3(1.0f, -1.0f, 100.0f);
      const float3 a = zinvul::makeFloat3(0.5f, 0.7f, 1.0f);
      results3[i++] = zinvul::mix(lhs, rhs, a);
    }

    i = 0;
    {
      const float4 lhs = zinvul::makeFloat4(-1.0f, 1.0f, -100.0f, 100.0f);
      const float4 rhs = zinvul::makeFloat4(1.0f, -1.0f, 100.0f, -100.0f);
      const float4 a = zinvul::makeFloat4(0.0f, 0.5f, 0.7f, 1.0f);
      results4[i++] = zinvul::mix(lhs, rhs, a);
    }
  }
}

__kernel void testSign(GlobalPtr<float> results1,
    GlobalPtr<float2> results2,
    GlobalPtr<float3> results3,
    GlobalPtr<float4> results4)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    constexpr float maxi = zinvul::NumericLimits<float>::max();
    constexpr float mini = zinvul::NumericLimits<float>::lowest();
    constexpr float m = zinvul::NumericLimits<float>::min();
    size_t i = 0;
    {
      results1[i++] = zinvul::sign(mini);
    }
    {
      results1[i++] = zinvul::sign(maxi);
    }
    {
      results1[i++] = zinvul::sign(-1.0f);
    }
    {
      results1[i++] = zinvul::sign(1.0f);
    }
    {
      results1[i++] = zinvul::sign(-m);
    }
    {
      results1[i++] = zinvul::sign(m);
    }
    {
      results1[i++] = zinvul::sign(0.0f);
    }

    i = 0;
    {
      const auto x = zinvul::makeFloat2(mini, maxi);
      results2[i++] = zinvul::sign(x);
    }
    {
      const auto x = zinvul::makeFloat2(-1.0f, 1.0f);
      results2[i++] = zinvul::sign(x);
    }
    {
      const auto x = zinvul::makeFloat2(-m, m);
      results2[i++] = zinvul::sign(x);
    }
    {
      const auto x = zinvul::makeFloat2(0.0f);
      results2[i++] = zinvul::sign(x);
    }

    i = 0;
    {
      const auto x = zinvul::makeFloat3(mini, maxi, 0.0f);
      results3[i++] = zinvul::sign(x);
    }
    {
      const auto x = zinvul::makeFloat3(-1.0f, 1.0f, 0.0f);
      results3[i++] = zinvul::sign(x);
    }
    {
      const auto x = zinvul::makeFloat3(-m, m, 0.0f);
      results3[i++] = zinvul::sign(x);
    }

    i = 0;
    {
      const auto x = zinvul::makeFloat4(mini, maxi, -1.0f, 1.0f);
      results4[i++] = zinvul::sign(x);
    }
    {
      const auto x = zinvul::makeFloat4(-m, m, 0.0f, 0.0f);
      results4[i++] = zinvul::sign(x);
    }
  }
}

__kernel void testPopcount8(ConstGlobalPtr<uint8b> inputs,
    GlobalPtr<uint8b> results1,
    GlobalPtr<uchar2> results2,
    GlobalPtr<uchar3> results3,
    GlobalPtr<uchar4> results4,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const uint8b input = inputs[index];
    {
      results1[index] = zinvul::popcount(input);
    }
    {
      const auto v = zinvul::make<uchar2>(input);
      results2[index] = zinvul::popcount(v);
    }
    {
      const auto v = zinvul::make<uchar3>(input);
      results3[index] = zinvul::popcount(v);
    }
    {
      const auto v = zinvul::make<uchar4>(input);
      results4[index] = zinvul::popcount(v);
    }
  }
}

__kernel void testPopcount16(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<uint16b> results1,
    GlobalPtr<ushort2> results2,
    GlobalPtr<ushort3> results3,
    GlobalPtr<ushort4> results4,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const uint16b input = inputs[index];
    {
      results1[index] = zinvul::popcount(input);
    }
    {
      const auto v = zinvul::make<ushort2>(input);
      results2[index] = zinvul::popcount(v);
    }
    {
      const auto v = zinvul::make<ushort3>(input);
      results3[index] = zinvul::popcount(v);
    }
    {
      const auto v = zinvul::make<ushort4>(input);
      results4[index] = zinvul::popcount(v);
    }
  }
}

__kernel void testPopcount32(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> results1,
    GlobalPtr<uint2> results2,
    GlobalPtr<uint3> results3,
    GlobalPtr<uint4> results4,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const uint32b input = inputs[index];
    {
      results1[index] = zinvul::popcount(input);
    }
    {
      const auto v = zinvul::make<uint2>(input);
      results2[index] = zinvul::popcount(v);
    }
    {
      const auto v = zinvul::make<uint3>(input);
      results3[index] = zinvul::popcount(v);
    }
    {
      const auto v = zinvul::make<uint4>(input);
      results4[index] = zinvul::popcount(v);
    }
  }
}

#if !defined(Z_MAC)

__kernel void testPopcount64(ConstGlobalPtr<uint64b> inputs,
    GlobalPtr<uint64b> results1,
    GlobalPtr<ulong2> results2,
    GlobalPtr<ulong3> results3,
    GlobalPtr<ulong4> results4,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const uint64b input = inputs[index];
    {
      results1[index] = zinvul::popcount(input);
    }
    {
      const auto v = zinvul::make<ulong2>(input);
      results2[index] = zinvul::popcount(v);
    }
    {
      const auto v = zinvul::make<ulong3>(input);
      results3[index] = zinvul::popcount(v);
    }
    {
      const auto v = zinvul::make<ulong4>(input);
      results4[index] = zinvul::popcount(v);
    }
  }
}

#endif // Z_MAC

#endif /* ZINVUL_ALGORITHM_TEST_ALGORITHM_CL */
