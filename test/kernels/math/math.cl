/*!
  \file math.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_TEST_MATH_CL
#define ZINVUL_MATH_TEST_MATH_CL

// Zinvul
#include "zinvul/cl/types.cl"
#include "zinvul/cl/math.cl"
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
__kernel void testConstantValue(GlobalPtr<int32b> iresults,
    GlobalPtr<float> fresults);
#if !defined(Z_MAC)
__kernel void testConstantValue64(GlobalPtr<int32b> iresults,
    GlobalPtr<double> fresults);
#endif // !Z_MAC

__kernel void testConstantValue(GlobalPtr<int32b> iresults,
    GlobalPtr<float> fresults)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr int32b k = FLT_DIG;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MANT_DIG;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MAX_10_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MAX_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MIN_10_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MIN_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_RADIX;
      iresults[i++] = k;
    }

    i = 0;
    {
      constexpr float k = FLT_MAX;
      fresults[i++] = k;
    }
    {
      constexpr float k = FLT_MIN;
      fresults[i++] = k;
    }
    {
      constexpr float k = FLT_EPSILON;
      fresults[i++] = k;
    }

    // Math constant values
    {
      constexpr float k = M_E_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_LOG2E_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_LOG10E_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_LN2_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_LN10_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_PI_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_PI_2_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_PI_4_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_1_PI_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_2_PI_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_2_SQRTPI_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_SQRT2_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_SQRT1_2_F;
      fresults[i++] = k;
    }

    {
      constexpr float pi = zinvul::kPi<float>;
      fresults[i++] = pi;
    }
    {
      constexpr float pi = zinvul::kPi<float>;
      constexpr float s = zinvul::mathconst::sqrt(pi);
      fresults[i++] = s;
    }
    {
      constexpr float s = zinvul::mathconst::sqrt(2.0f);
      fresults[i++] = s;
    }
    {
      constexpr float s = zinvul::mathconst::sqrt(3.0f);
      fresults[i++] = s;
    }
    {
      constexpr float pi = zinvul::kPi<float>;
      constexpr float c = zinvul::mathconst::cbrt(pi);
      fresults[i++] = c;
    }
    {
      constexpr float c = zinvul::mathconst::cbrt(2.0f);
      fresults[i++] = c;
    }
    {
      constexpr float c = zinvul::mathconst::cbrt(3.0f);
      fresults[i++] = c;
    }
  }
}

#if !defined(Z_MAC)

// Forward declaration
__kernel void testConstantValue64(GlobalPtr<int32b> iresults,
    GlobalPtr<double> fresults)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr int32b k = DBL_DIG;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MANT_DIG;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MAX_10_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MAX_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MIN_10_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MIN_EXP;
      iresults[i++] = k;
    }

    i = 0;
    {
      constexpr double k = DBL_MAX;
      fresults[i++] = k;
    }
    {
      constexpr double k = DBL_MIN;
      fresults[i++] = k;
    }
    {
      constexpr double k = DBL_EPSILON;
      fresults[i++] = k;
    }

    // Math constant values
    {
      constexpr double k = M_E;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_LOG2E;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_LOG10E;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_LN2;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_LN10;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_PI;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_PI_2;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_PI_4;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_1_PI;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_2_PI;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_2_SQRTPI;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_SQRT2;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_SQRT1_2;
      fresults[i++] = k;
    }

    {
      constexpr double pi = zinvul::kPi<double>;
      fresults[i++] = pi;
    }
    {
      constexpr double pi = zinvul::kPi<double>;
      constexpr double s = zinvul::mathconst::sqrt(pi);
      fresults[i++] = s;
    }
    {
      constexpr double s = zinvul::mathconst::sqrt(2.0);
      fresults[i++] = s;
    }
    {
      constexpr double s = zinvul::mathconst::sqrt(3.0);
      fresults[i++] = s;
    }
    {
      constexpr double pi = zinvul::kPi<double>;
      constexpr double c = zinvul::mathconst::cbrt(pi);
      fresults[i++] = c;
    }
    {
      constexpr double c = zinvul::mathconst::cbrt(2.0);
      fresults[i++] = c;
    }
    {
      constexpr double c = zinvul::mathconst::cbrt(3.0);
      fresults[i++] = c;
    }
  }
}

#endif // !Z_MAC

//#define ZINVUL_TEST_ABS(abs1, abs2, abs3, abs4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      results1[0] = abs1(5); \
//      results1[1] = abs1(-5); \
//    } \
//    { \
//      results2[0] = abs2(zMakeInt2(5, -2)); \
//    } \
//    { \
//      results3[0] = abs3(zMakeInt3(5, -2, 9)); \
//    } \
//    { \
//      results4[0] = abs4(zMakeInt4(5, -2, 9, -15)); \
//    } \
//  } \
//
///*!
//  */
//__kernel void testAbs(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_ABS(abs, abs, abs, abs,
//                  results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzAbsImpl(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_ABS(zAbsImpl, zAbs2Impl, zAbs3Impl, zAbs4Impl,
//                  results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzAbs(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_ABS(zAbs, zAbs2, zAbs3, zAbs4,
//                  results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_ABSF(abs1, abs2, abs3, abs4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      results1[0] = abs1(5.0f); \
//      results1[1] = abs1(-5.0f); \
//    } \
//    { \
//      results2[0] = abs2(zMakeFloat2(5.0f, -2.0f)); \
//    } \
//    { \
//      results3[0] = abs3(zMakeFloat3(5.0f, -2.0f, 9.0f)); \
//    } \
//    { \
//      results4[0] = abs4(zMakeFloat4(5.0f, -2.0f, 9.0f, -15.0f)); \
//    } \
//  } \
//
///*!
//  */
//__kernel void testAbsF(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_ABSF(fabs, fabs, fabs, fabs,
//                  results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzAbsFImpl(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_ABSF(zAbsFImpl, zAbsF2Impl, zAbsF3Impl, zAbsF4Impl,
//                  results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzAbsF(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_ABSF(zAbsF, zAbsF2, zAbsF3, zAbsF4,
//                  results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_IS_ALMOST_SAME(same1, same2, same3, same4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      const float x = 5.0f; \
//      results1[0] = same1(x, x); \
//    } \
//    { \
//      const float x = -5.0f; \
//      results1[1] = same1(x, x); \
//    } \
//    { \
//      const float x = 5.0f; \
//      const float y = 5.00001f; \
//      results1[2] = same1(x, y); \
//    } \
//    { \
//      const float x = -5.0f; \
//      const float y = -5.00001f; \
//      results1[3] = same1(x, y); \
//    } \
//    { \
//      const float x = 1.0f + zFloatEpsilon; \
//      const float y = 1.0f + 4.0f * zFloatEpsilon; \
//      results1[4] = same1(x, y); \
//    } \
//    { \
//      const float x = 1.0f + zFloatEpsilon; \
//      const float y = 1.0f + 10.0f * zFloatEpsilon; \
//      results1[5] = same1(x, y); \
//    } \
//    { \
//      const float2 x = zMakeFloat2(5.0f, -2.0f); \
//      results2[0] = same2(x, x); \
//    } \
//    { \
//      const float3 x = zMakeFloat3(5.0f, -2.0f, 9.0f); \
//      results3[0] = same3(x, x); \
//    } \
//    { \
//      const float4 x = zMakeFloat4(5.0f, -2.0f, 9.0f, -15.0f); \
//      results4[0] = same4(x, x); \
//    } \
//  } \
//
///*!
//  */
//__kernel void testzIsAlmostSame(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_IS_ALMOST_SAME(
//      zIsAlmostSame, zIsAlmostSame2, zIsAlmostSame3, zIsAlmostSame4,
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_IS_ODD(isOdd1, isOdd2, isOdd3, isOdd4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      results1[0] = isOdd1(-2); \
//      results1[1] = isOdd1(-1); \
//      results1[2] = isOdd1(0); \
//      results1[3] = isOdd1(1); \
//      results1[4] = isOdd1(2); \
//    } \
//    { \
//      results2[0] = isOdd2(zMakeInt2(1, -2)); \
//    } \
//    { \
//      results3[0] = isOdd3(zMakeInt3(0, -1, -2)); \
//    } \
//    { \
//      results4[0] = isOdd4(zMakeInt4(-1, -2, 1, 2)); \
//    } \
//  } \
//
//__kernel void testzIsOdd(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_IS_ODD(zIsOdd, zIsOdd2, zIsOdd3, zIsOdd4,
//                     results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_IS_ODDU(isOdd1, isOdd2, isOdd3, isOdd4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      results1[0] = isOdd1(0u); \
//      results1[1] = isOdd1(1u); \
//      results1[2] = isOdd1(2u); \
//    } \
//    { \
//      results2[0] = isOdd2(zMakeUInt2(0, 1)); \
//    } \
//    { \
//      results3[0] = isOdd3(zMakeUInt3(0, 1, 2)); \
//    } \
//    { \
//      results4[0] = isOdd4(zMakeUInt4(0, 1, 2, 3)); \
//    } \
//  } \
//
//__kernel void testzIsOddU(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_IS_ODDU(zIsOddU, zIsOddU2, zIsOddU3, zIsOddU4,
//                     results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_CEIL(ceil1, ceil2, ceil3, ceil4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      uint i = 0; \
//      { \
//        const float x = 0.0f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = 1.0f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = 3.1f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = 3.5f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = 3.9f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = 4.1f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = 4.5f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = 4.9f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = -1.0f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = -3.1f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = -3.5f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = -3.9f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = -4.1f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = -4.5f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = -4.9f; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = -INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//      { \
//        const float x = NAN; \
//        results1[i++] = x; \
//        results1[i++] = ceil1(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float2 x = zMakeFloat2(0.0f, 0.0f); \
//        results2[i++] = x; \
//        results2[i++] = ceil2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(1.0f, -1.0f); \
//        results2[i++] = x; \
//        results2[i++] = ceil2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.1f, -3.1f); \
//        results2[i++] = x; \
//        results2[i++] = ceil2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.5f, -3.5f); \
//        results2[i++] = x; \
//        results2[i++] = ceil2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.9f, -3.9f); \
//        results2[i++] = x; \
//        results2[i++] = ceil2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.1f, -4.1f); \
//        results2[i++] = x; \
//        results2[i++] = ceil2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.5f, -4.5f); \
//        results2[i++] = x; \
//        results2[i++] = ceil2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.9f, -4.9f); \
//        results2[i++] = x; \
//        results2[i++] = ceil2(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float3 x = zMakeFloat3(0.0f, 0.0f, 0.0f); \
//        results3[i++] = x; \
//        results3[i++] = ceil3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(1.0f, -1.0f, 1.0f); \
//        results3[i++] = x; \
//        results3[i++] = ceil3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.1f, -3.1f, 3.1f); \
//        results3[i++] = x; \
//        results3[i++] = ceil3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.5f, -3.5f, 3.5f); \
//        results3[i++] = x; \
//        results3[i++] = ceil3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.9f, -3.9f, 3.9f); \
//        results3[i++] = x; \
//        results3[i++] = ceil3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.1f, -4.1f, 4.1f); \
//        results3[i++] = x; \
//        results3[i++] = ceil3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.5f, -4.5f, 4.5f); \
//        results3[i++] = x; \
//        results3[i++] = ceil3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.9f, -4.9f, 4.9f); \
//        results3[i++] = x; \
//        results3[i++] = ceil3(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float4 x = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f); \
//        results4[i++] = x; \
//        results4[i++] = ceil4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(1.0f, -1.0f, 5.0f, -5.0f); \
//        results4[i++] = x; \
//        results4[i++] = ceil4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.1f, -3.1f, 13.1f, -13.1f); \
//        results4[i++] = x; \
//        results4[i++] = ceil4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.5f, -3.5f, 13.5f, -13.5f); \
//        results4[i++] = x; \
//        results4[i++] = ceil4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.9f, -3.9f, 13.9f, -13.9f); \
//        results4[i++] = x; \
//        results4[i++] = ceil4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.1f, -4.1f, 14.1f, -14.1f); \
//        results4[i++] = x; \
//        results4[i++] = ceil4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.5f, -4.5f, 14.5f, -14.5f); \
//        results4[i++] = x; \
//        results4[i++] = ceil4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.9f, -4.9f, 14.9f, -14.9f); \
//        results4[i++] = x; \
//        results4[i++] = ceil4(x); \
//      } \
//    } \
//  } \
//
///*!
//  */
//__kernel void testCeil(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_CEIL(ceil, ceil, ceil, ceil,
//                   results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzCeilImpl(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_CEIL(zCeilImpl, zCeil2Impl, zCeil3Impl, zCeil4Impl,
//                   results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzCeil(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_CEIL(zCeil, zCeil2, zCeil3, zCeil4,
//                   results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_FLOOR(floor1, floor2, floor3, floor4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      uint i = 0; \
//      { \
//        const float x = 0.0f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = 1.0f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = 3.1f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = 3.5f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = 3.9f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = 4.1f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = 4.5f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = 4.9f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = -1.0f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = -3.1f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = -3.5f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = -3.9f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = -4.1f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = -4.5f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = -4.9f; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = -INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//      { \
//        const float x = NAN; \
//        results1[i++] = x; \
//        results1[i++] = floor1(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float2 x = zMakeFloat2(0.0f, 0.0f); \
//        results2[i++] = x; \
//        results2[i++] = floor2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(1.0f, -1.0f); \
//        results2[i++] = x; \
//        results2[i++] = floor2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.1f, -3.1f); \
//        results2[i++] = x; \
//        results2[i++] = floor2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.5f, -3.5f); \
//        results2[i++] = x; \
//        results2[i++] = floor2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.9f, -3.9f); \
//        results2[i++] = x; \
//        results2[i++] = floor2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.1f, -4.1f); \
//        results2[i++] = x; \
//        results2[i++] = floor2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.5f, -4.5f); \
//        results2[i++] = x; \
//        results2[i++] = floor2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.9f, -4.9f); \
//        results2[i++] = x; \
//        results2[i++] = floor2(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float3 x = zMakeFloat3(0.0f, 0.0f, 0.0f); \
//        results3[i++] = x; \
//        results3[i++] = floor3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(1.0f, -1.0f, 1.0f); \
//        results3[i++] = x; \
//        results3[i++] = floor3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.1f, -3.1f, 3.1f); \
//        results3[i++] = x; \
//        results3[i++] = floor3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.5f, -3.5f, 3.5f); \
//        results3[i++] = x; \
//        results3[i++] = floor3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.9f, -3.9f, 3.9f); \
//        results3[i++] = x; \
//        results3[i++] = floor3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.1f, -4.1f, 4.1f); \
//        results3[i++] = x; \
//        results3[i++] = floor3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.5f, -4.5f, 4.5f); \
//        results3[i++] = x; \
//        results3[i++] = floor3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.9f, -4.9f, 4.9f); \
//        results3[i++] = x; \
//        results3[i++] = floor3(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float4 x = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f); \
//        results4[i++] = x; \
//        results4[i++] = floor4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(1.0f, -1.0f, 5.0f, -5.0f); \
//        results4[i++] = x; \
//        results4[i++] = floor4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.1f, -3.1f, 13.1f, -13.1f); \
//        results4[i++] = x; \
//        results4[i++] = floor4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.5f, -3.5f, 13.5f, -13.5f); \
//        results4[i++] = x; \
//        results4[i++] = floor4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.9f, -3.9f, 13.9f, -13.9f); \
//        results4[i++] = x; \
//        results4[i++] = floor4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.1f, -4.1f, 14.1f, -14.1f); \
//        results4[i++] = x; \
//        results4[i++] = floor4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.5f, -4.5f, 14.5f, -14.5f); \
//        results4[i++] = x; \
//        results4[i++] = floor4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.9f, -4.9f, 14.9f, -14.9f); \
//        results4[i++] = x; \
//        results4[i++] = floor4(x); \
//      } \
//    } \
//  } \
//
///*!
//  */
//__kernel void testFloor(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_FLOOR(floor, floor, floor, floor,
//                    results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzFloorImpl(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_FLOOR(zFloorImpl, zFloor2Impl, zFloor3Impl, zFloor4Impl,
//                    results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzFloor(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_FLOOR(zFloor, zFloor2, zFloor3, zFloor4,
//                    results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_TRUNC(trunc1, trunc2, trunc3, trunc4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      uint i = 0; \
//      { \
//        const float x = 0.0f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = 1.0f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = 3.1f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = 3.5f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = 3.9f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = 4.1f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = 4.5f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = 4.9f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = -1.0f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = -3.1f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = -3.5f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = -3.9f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = -4.1f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = -4.5f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = -4.9f; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = -INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//      { \
//        const float x = NAN; \
//        results1[i++] = x; \
//        results1[i++] = trunc1(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float2 x = zMakeFloat2(0.0f, 0.0f); \
//        results2[i++] = x; \
//        results2[i++] = trunc2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(1.0f, -1.0f); \
//        results2[i++] = x; \
//        results2[i++] = trunc2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.1f, -3.1f); \
//        results2[i++] = x; \
//        results2[i++] = trunc2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.5f, -3.5f); \
//        results2[i++] = x; \
//        results2[i++] = trunc2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.9f, -3.9f); \
//        results2[i++] = x; \
//        results2[i++] = trunc2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.1f, -4.1f); \
//        results2[i++] = x; \
//        results2[i++] = trunc2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.5f, -4.5f); \
//        results2[i++] = x; \
//        results2[i++] = trunc2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.9f, -4.9f); \
//        results2[i++] = x; \
//        results2[i++] = trunc2(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float3 x = zMakeFloat3(0.0f, 0.0f, 0.0f); \
//        results3[i++] = x; \
//        results3[i++] = trunc3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(1.0f, -1.0f, 1.0f); \
//        results3[i++] = x; \
//        results3[i++] = trunc3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.1f, -3.1f, 3.1f); \
//        results3[i++] = x; \
//        results3[i++] = trunc3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.5f, -3.5f, 3.5f); \
//        results3[i++] = x; \
//        results3[i++] = trunc3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.9f, -3.9f, 3.9f); \
//        results3[i++] = x; \
//        results3[i++] = trunc3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.1f, -4.1f, 4.1f); \
//        results3[i++] = x; \
//        results3[i++] = trunc3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.5f, -4.5f, 4.5f); \
//        results3[i++] = x; \
//        results3[i++] = trunc3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.9f, -4.9f, 4.9f); \
//        results3[i++] = x; \
//        results3[i++] = trunc3(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float4 x = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f); \
//        results4[i++] = x; \
//        results4[i++] = trunc4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(1.0f, -1.0f, 5.0f, -5.0f); \
//        results4[i++] = x; \
//        results4[i++] = trunc4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.1f, -3.1f, 13.1f, -13.1f); \
//        results4[i++] = x; \
//        results4[i++] = trunc4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.5f, -3.5f, 13.5f, -13.5f); \
//        results4[i++] = x; \
//        results4[i++] = trunc4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.9f, -3.9f, 13.9f, -13.9f); \
//        results4[i++] = x; \
//        results4[i++] = trunc4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.1f, -4.1f, 14.1f, -14.1f); \
//        results4[i++] = x; \
//        results4[i++] = trunc4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.5f, -4.5f, 14.5f, -14.5f); \
//        results4[i++] = x; \
//        results4[i++] = trunc4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.9f, -4.9f, 14.9f, -14.9f); \
//        results4[i++] = x; \
//        results4[i++] = trunc4(x); \
//      } \
//    } \
//  } \
//
///*!
//  */
//__kernel void testTrunc(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_TRUNC(trunc, trunc, trunc, trunc,
//                    results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzTruncImpl(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_TRUNC(zTruncImpl, zTrunc2Impl, zTrunc3Impl, zTrunc4Impl,
//                    results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzTrunc(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_TRUNC(zTrunc, zTrunc2, zTrunc3, zTrunc4,
//                    results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_ROUND(round1, round2, round3, round4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      uint i = 0; \
//      { \
//        const float x = 0.0f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = 1.0f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = 3.1f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = 3.5f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = 3.9f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = 4.1f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = 4.5f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = 4.9f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = -1.0f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = -3.1f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = -3.5f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = -3.9f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = -4.1f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = -4.5f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = -4.9f; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = -INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//      { \
//        const float x = NAN; \
//        results1[i++] = x; \
//        results1[i++] = round1(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float2 x = zMakeFloat2(0.0f, 0.0f); \
//        results2[i++] = x; \
//        results2[i++] = round2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(1.0f, -1.0f); \
//        results2[i++] = x; \
//        results2[i++] = round2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.1f, -3.1f); \
//        results2[i++] = x; \
//        results2[i++] = round2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.5f, -3.5f); \
//        results2[i++] = x; \
//        results2[i++] = round2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.9f, -3.9f); \
//        results2[i++] = x; \
//        results2[i++] = round2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.1f, -4.1f); \
//        results2[i++] = x; \
//        results2[i++] = round2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.5f, -4.5f); \
//        results2[i++] = x; \
//        results2[i++] = round2(x); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.9f, -4.9f); \
//        results2[i++] = x; \
//        results2[i++] = round2(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float3 x = zMakeFloat3(0.0f, 0.0f, 0.0f); \
//        results3[i++] = x; \
//        results3[i++] = round3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(1.0f, -1.0f, 1.0f); \
//        results3[i++] = x; \
//        results3[i++] = round3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.1f, -3.1f, 3.1f); \
//        results3[i++] = x; \
//        results3[i++] = round3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.5f, -3.5f, 3.5f); \
//        results3[i++] = x; \
//        results3[i++] = round3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.9f, -3.9f, 3.9f); \
//        results3[i++] = x; \
//        results3[i++] = round3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.1f, -4.1f, 4.1f); \
//        results3[i++] = x; \
//        results3[i++] = round3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.5f, -4.5f, 4.5f); \
//        results3[i++] = x; \
//        results3[i++] = round3(x); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.9f, -4.9f, 4.9f); \
//        results3[i++] = x; \
//        results3[i++] = round3(x); \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      { \
//        const float4 x = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f); \
//        results4[i++] = x; \
//        results4[i++] = round4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(1.0f, -1.0f, 5.0f, -5.0f); \
//        results4[i++] = x; \
//        results4[i++] = round4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.1f, -3.1f, 13.1f, -13.1f); \
//        results4[i++] = x; \
//        results4[i++] = round4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.5f, -3.5f, 13.5f, -13.5f); \
//        results4[i++] = x; \
//        results4[i++] = round4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.9f, -3.9f, 13.9f, -13.9f); \
//        results4[i++] = x; \
//        results4[i++] = round4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.1f, -4.1f, 14.1f, -14.1f); \
//        results4[i++] = x; \
//        results4[i++] = round4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.5f, -4.5f, 14.5f, -14.5f); \
//        results4[i++] = x; \
//        results4[i++] = round4(x); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.9f, -4.9f, 14.9f, -14.9f); \
//        results4[i++] = x; \
//        results4[i++] = round4(x); \
//      } \
//    } \
//  } \
//
///*!
//  */
//__kernel void testRound(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_ROUND(round, round, round, round,
//                    results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzRoundImpl(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_ROUND(zRoundImpl, zRound2Impl, zRound3Impl, zRound4Impl,
//                    results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzRound(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_ROUND(zRound, zRound2, zRound3, zRound4,
//                    results1, results2, results3, results4);
//}
//
///*!
//  */
//kernel void testConstantValues(global int32b* int_values,
//    global int32b* max_values,
//    global uint32b* umax_values,
//    global float* float_values)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t int_index = 0;
//    size_t float_index = 0;
//    // Integer
//    int_values[int_index++] = FLT_DIG;
//    int_values[int_index++] = FLT_MANT_DIG;
//    int_values[int_index++] = FLT_MAX_10_EXP;
//    int_values[int_index++] = FLT_MAX_EXP;
//    int_values[int_index++] = FLT_MIN_10_EXP;
//    int_values[int_index++] = FLT_MIN_EXP;
//    int_values[int_index++] = FLT_RADIX;
//    int_values[int_index++] = CHAR_BIT;
//    int_values[int_index++] = SCHAR_MAX;
//    int_values[int_index++] = SCHAR_MIN;
//    int_values[int_index++] = CHAR_MAX;
//    int_values[int_index++] = CHAR_MIN;
//    int_values[int_index++] = SHRT_MAX;
//    int_values[int_index++] = SHRT_MIN;
//    int_values[int_index++] = INT_MAX;
//    int_values[int_index++] = INT_MIN;
//    // Max values
//    int_index = 0;
//    max_values[int_index++] = zCharMax;
//    max_values[int_index++] = zCharMin;
//    max_values[int_index++] = zShortMax;
//    max_values[int_index++] = zShortMin;
//    max_values[int_index++] = zIntMax;
//    max_values[int_index++] = zIntMin;
////    max_values[int_index++] = zLongMax;
////    max_values[int_index++] = zLongMin;
//    int_index = 0;
//    umax_values[int_index++] = zUCharMax;
//    umax_values[int_index++] = zUShortMax;
//    umax_values[int_index++] = zUIntMax;
////    umax_values[int_index++] = zULongMax;
//    // Float
//    float_values[float_index++] = MAXFLOAT;
//    float_values[float_index++] = INFINITY;
//    float_values[float_index++] = NAN;
//    float_values[float_index++] = FLT_MAX;
//    float_values[float_index++] = FLT_MIN;
//    float_values[float_index++] = FLT_EPSILON;
//    float_values[float_index++] = M_E_F;
//    float_values[float_index++] = M_LOG2E_F;
//    float_values[float_index++] = M_LOG10E_F;
//    float_values[float_index++] = M_LN2_F;
//    float_values[float_index++] = M_LN10_F;
//    float_values[float_index++] = M_PI_F;
//    float_values[float_index++] = M_PI_2_F;
//    float_values[float_index++] = M_PI_4_F;
//    float_values[float_index++] = M_1_PI_F;
//    float_values[float_index++] = M_2_PI_F;
//    float_values[float_index++] = M_2_SQRTPI_F;
//    float_values[float_index++] = M_SQRT2_F;
//    float_values[float_index++] = M_SQRT1_2_F;
//    // Zinvul
//    float_values[float_index++] = zFloatMax;
//    float_values[float_index++] = zFloatMin;
//    float_values[float_index++] = zFloatEpsilon;
//    float_values[float_index++] = zFloatDenormMax;
//    float_values[float_index++] = zFloatDenormMin;
//    float_values[float_index++] = zEF;
//    float_values[float_index++] = zInvEF;
//    float_values[float_index++] = zLog2EF;
//    float_values[float_index++] = zInvLog2EF;
//    float_values[float_index++] = zLog10EF;
//    float_values[float_index++] = zInvLog10EF;
//    float_values[float_index++] = zLn2F;
//    float_values[float_index++] = zInvLn2F;
//    float_values[float_index++] = zLn10F;
//    float_values[float_index++] = zInvLn10F;
//    float_values[float_index++] = zPiF;
//    float_values[float_index++] = zPi2F;
//    float_values[float_index++] = zPi4F;
//    float_values[float_index++] = zInvPiF;
//    float_values[float_index++] = z2InvPiF;
//    float_values[float_index++] = z2InvSqrtPiF;
//    float_values[float_index++] = zSqrt2F;
//    float_values[float_index++] = zInvSqrt2F;
//    float_values[float_index++] = zSqrt3F;
//    float_values[float_index++] = zInvSqrt3F;
//  }
//}
//
//#define ZINVUL_TEST_MAX(max1, max2, max3, max4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    size_t i = 0; \
//    { \
//      const int32b lhs = INT_MIN; \
//      const int32b rhs = INT_MAX; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int32b lhs = INT_MAX; \
//      const int32b rhs = INT_MIN; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int32b lhs = -1; \
//      const int32b rhs = 0; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int32b lhs = 0; \
//      const int32b rhs = -1; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs,rhs); \
//    } \
//    { \
//      const int32b lhs = -1; \
//      const int32b rhs = 1; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int32b lhs = 1; \
//      const int32b rhs = -1; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int32b lhs = -10; \
//      const int32b rhs = -1; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int32b lhs = -1; \
//      const int32b rhs = -10; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int32b lhs = 10; \
//      const int32b rhs = 1; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int32b lhs = 1; \
//      const int32b rhs = 10; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const int2 lhs = zMakeInt2(0, 1); \
//      const int2 rhs = zMakeInt2(0, -1); \
//      results2[0] = lhs; \
//      results2[1] = rhs; \
//      results2[2] = max2(lhs, rhs); \
//    } \
//    { \
//      const int3 lhs = zMakeInt3(0, 1, -10); \
//      const int3 rhs = zMakeInt3(0, -1, 10); \
//      results3[0] = lhs; \
//      results3[1] = rhs; \
//      results3[2] = max3(lhs, rhs); \
//    } \
//    { \
//      const int4 lhs = zMakeInt4(0, 1, -10, INT_MAX); \
//      const int4 rhs = zMakeInt4(0, -1, 10, INT_MIN); \
//      results4[0] = lhs; \
//      results4[1] = rhs; \
//      results4[2] = max4(lhs, rhs); \
//    } \
//  } \
//
//__kernel void testMax(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_MAX(max, max, max, max,
//                  results1, results2, results3, results4);
//}
//
//__kernel void testzMaxImpl(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_MAX(zMaxImpl, zMax2Impl, zMax3Impl, zMax4Impl,
//                  results1, results2, results3, results4);
//}
//
//__kernel void testzMax(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_MAX(zMax, zMax2, zMax3, zMax4,
//                  results1, results2, results3, results4);
//}
//
//__kernel void testMin(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_MAX(min, min, min, min,
//                  results1, results2, results3, results4);
//}
//
//__kernel void testzMinImpl(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_MAX(zMinImpl, zMin2Impl, zMin3Impl, zMin4Impl,
//                  results1, results2, results3, results4);
//}
//
//__kernel void testzMin(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_MAX(zMin, zMin2, zMin3, zMin4,
//                  results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_MAXU(max1, max2, max3, max4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    size_t i = 0; \
//    { \
//      const uint32b lhs = 0u; \
//      const uint32b rhs = UINT_MAX; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const uint32b lhs = UINT_MAX; \
//      const uint32b rhs = 0u; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const uint32b lhs = 1u; \
//      const uint32b rhs = 0u; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const uint32b lhs = 0u; \
//      const uint32b rhs = 1u; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs,rhs); \
//    } \
//    { \
//      const uint32b lhs = 10u; \
//      const uint32b rhs = 1u; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const uint32b lhs = 1u; \
//      const uint32b rhs = 10u; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const uint2 lhs = zMakeUInt2(0u, 1u); \
//      const uint2 rhs = zMakeUInt2(1u, 0u); \
//      results2[0] = lhs; \
//      results2[1] = rhs; \
//      results2[2] = max2(lhs, rhs); \
//    } \
//    { \
//      const uint3 lhs = zMakeUInt3(0u, 1u, 10u); \
//      const uint3 rhs = zMakeUInt3(1u, 10u, 0u); \
//      results3[0] = lhs; \
//      results3[1] = rhs; \
//      results3[2] = max3(lhs, rhs); \
//    } \
//    { \
//      const uint4 lhs = zMakeUInt4(0u, 1u, 10u, UINT_MAX); \
//      const uint4 rhs = zMakeUInt4(1u, 10u, 0u, 0u); \
//      results4[0] = lhs; \
//      results4[1] = rhs; \
//      results4[2] = max4(lhs, rhs); \
//    } \
//  } \
//
//__kernel void testMaxU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_MAXU(max, max, max, max,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testzMaxUImpl(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_MAXU(zMaxUImpl, zMaxU2Impl, zMaxU3Impl, zMaxU4Impl,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testzMaxU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_MAXU(zMaxU, zMaxU2, zMaxU3, zMaxU4,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testMinU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_MAXU(min, min, min, min,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testzMinUImpl(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_MAXU(zMinUImpl, zMinU2Impl, zMinU3Impl, zMinU4Impl,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testzMinU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_MAXU(zMinU, zMinU2, zMinU3, zMinU4,
//                   results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_MAXF(max1, max2, max3, max4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    size_t i = 0; \
//    { \
//      const float lhs = FLT_MAX; \
//      const float rhs = FLT_MIN; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = FLT_MIN; \
//      const float rhs = FLT_MAX; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = FLT_MAX; \
//      const float rhs = -FLT_MAX; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = -FLT_MAX; \
//      const float rhs = FLT_MAX; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = -1.0f; \
//      const float rhs = 0.0f; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = 0.0f; \
//      const float rhs = -1.0f; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs,rhs); \
//    } \
//    { \
//      const float lhs = -1.0f; \
//      const float rhs = 1.0f; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = 1.0f; \
//      const float rhs = -1.0f; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = 1.0f; \
//      const float rhs = 1.0f + FLT_EPSILON; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = 1.0f + FLT_EPSILON; \
//      const float rhs = 1.0f; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = -1.0f; \
//      const float rhs = -1.0f - FLT_EPSILON; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = -1.0f - FLT_EPSILON; \
//      const float rhs = -1.0f; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = 1.0f; \
//      const float rhs = 1.0f; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = -1.0f; \
//      const float rhs = -1.0f; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = INFINITY; \
//      const float rhs = -INFINITY; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float lhs = -INFINITY; \
//      const float rhs = INFINITY; \
//      results1[i++] = lhs; \
//      results1[i++] = rhs; \
//      results1[i++] = max1(lhs, rhs); \
//    } \
//    { \
//      const float2 lhs = zMakeFloat2(0.0f, 1.0f); \
//      const float2 rhs = zMakeFloat2(0.0f, -1.0f); \
//      results2[0] = lhs; \
//      results2[1] = rhs; \
//      results2[2] = max2(lhs, rhs); \
//    } \
//    { \
//      const float3 lhs = zMakeFloat3(0.0f, 1.0f, -10.0f); \
//      const float3 rhs = zMakeFloat3(0.0f, -1.0f, 10.0f); \
//      results3[0] = lhs; \
//      results3[1] = rhs; \
//      results3[2] = max3(lhs, rhs); \
//    } \
//    { \
//      const float4 lhs = zMakeFloat4(0.0f, 1.0f, -10.0f, FLT_MAX); \
//      const float4 rhs = zMakeFloat4(0.0f, -1.0f, 10.0f, FLT_MIN); \
//      results4[0] = lhs; \
//      results4[1] = rhs; \
//      results4[2] = max4(lhs, rhs); \
//    } \
//  } \
//
//__kernel void testMaxF(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MAXF(max, max, max, max,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testzMaxFImpl(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MAXF(zMaxFImpl, zMaxF2Impl, zMaxF3Impl, zMaxF4Impl,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testzMaxF(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MAXF(zMaxF, zMaxF2, zMaxF3, zMaxF4,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testMinF(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MAXF(min, min, min, min,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testzMinFImpl(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MAXF(zMinFImpl, zMinF2Impl, zMinF3Impl, zMinF4Impl,
//                   results1, results2, results3, results4);
//}
//
//__kernel void testzMinF(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MAXF(zMinF, zMinF2, zMinF3, zMinF4,
//                   results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_MIX(mix1, mix2f, mix2, mix3f, mix3, mix4f, mix4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      const float x = -1.0f; \
//      const float y = 1.0f; \
//      { \
//        const float a = 0.0f; \
//        results1[0] = mix1(x, y, a); \
//      } \
//      { \
//        const float a = 0.5f; \
//        results1[1] = mix1(x, y, a); \
//      } \
//      { \
//        const float a = 1.0f; \
//        results1[2] = mix1(x, y, a); \
//      } \
//    } \
//    { \
//      const float2 x = zMakeFloat2(-1.0f, -2.0f); \
//      const float2 y = zMakeFloat2(1.0f, 2.0f); \
//      { \
//        const float a = 0.0f; \
//        results2[0] = mix2f(x, y, a); \
//      } \
//      { \
//        const float a = 0.5f; \
//        results2[1] = mix2f(x, y, a); \
//      } \
//      { \
//        const float a = 1.0f; \
//        results2[2] = mix2f(x, y, a); \
//      } \
//      { \
//        const float2 a = zMakeFloat2(0.0f, 1.0f); \
//        results2[3] = mix2(x, y, a); \
//      } \
//      { \
//        const float2 a = zMakeFloat2(0.5f, 1.0f); \
//        results2[4] = mix2(x, y, a); \
//      } \
//      { \
//        const float2 a = zMakeFloat2(1.0f, 0.0f); \
//        results2[5] = mix2(x, y, a); \
//      } \
//    } \
//    { \
//      const float3 x = zMakeFloat3(-1.0f, -2.0f, -3.0f); \
//      const float3 y = zMakeFloat3(1.0f, 2.0f, 3.0f); \
//      { \
//        const float a = 0.0f; \
//        results3[0] = mix3f(x, y, a); \
//      } \
//      { \
//        const float a = 0.5f; \
//        results3[1] = mix3f(x, y, a); \
//      } \
//      { \
//        const float a = 1.0f; \
//        results3[2] = mix3f(x, y, a); \
//      } \
//      { \
//        const float3 a = zMakeFloat3(0.0f, 0.5f, 1.0f); \
//        results3[3] = mix3(x, y, a); \
//      } \
//      { \
//        const float3 a = zMakeFloat3(0.5f, 1.0f, 0.0f); \
//        results3[4] = mix3(x, y, a); \
//      } \
//      { \
//        const float3 a = zMakeFloat3(1.0f, 0.0f, 0.5f); \
//        results3[5] = mix3(x, y, a); \
//      } \
//    } \
//    { \
//      const float4 x = zMakeFloat4(-1.0f, -2.0f, -3.0f, -4.0f); \
//      const float4 y = zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f); \
//      { \
//        const float a = 0.0f; \
//        results4[0] = mix4f(x, y, a); \
//      } \
//      { \
//        const float a = 0.5f; \
//        results4[1] = mix4f(x, y, a); \
//      } \
//      { \
//        const float a = 1.0f; \
//        results4[2] = mix4f(x, y, a); \
//      } \
//      { \
//        const float4 a = zMakeFloat4(0.0f, 0.5f, 1.0f, 0.0f); \
//        results4[3] = mix4(x, y, a); \
//      } \
//      { \
//        const float4 a = zMakeFloat4(0.5f, 1.0f, 0.0f, 0.5f); \
//        results4[4] = mix4(x, y, a); \
//      } \
//      { \
//        const float4 a = zMakeFloat4(1.0f, 0.0f, 0.5f, 1.0f); \
//        results4[5] = mix4(x, y, a); \
//      } \
//    } \
//  }
//
///*!
//  */
//__kernel void testMix(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_MIX(mix, mix, mix, mix, mix, mix, mix,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzMixImpl(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_MIX(zMixImpl, zMix2FImpl, zMix2Impl, zMix3FImpl, zMix3Impl, zMix4FImpl, zMix4Impl,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzMix(
//    __global float* results1, __global float2* results2,
//    __global float3* results3, __global float4* results4)
//{
//  ZINVUL_TEST_MIX(zMix, zMix2F, zMix2, zMix3F, zMix3, zMix4F, zMix4,
//      results1, results2, results3, results4);
//}
//
//int32b getClampInput(const size_t i)
//{
//  const int32b x_list[] = {0, 1, INT_MAX, -1, INT_MIN};
//  return x_list[i];
//}
//
//#define ZINVUL_TEST_CLAMP(clamp1, clamp2, clamp3, clamp4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    size_t i = 0; \
//    for (size_t j = 0; j < 5; ++j) { \
//      const int32b minval = 0; \
//      const int32b maxval = 0; \
//      const int32b x = getClampInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    for (size_t j = 0; j < 5; ++j) { \
//      const int32b minval = -1; \
//      const int32b maxval = 1; \
//      const int32b x = getClampInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    for (size_t j = 0; j < 5; ++j) { \
//      const int32b minval = INT_MIN; \
//      const int32b maxval = INT_MAX; \
//      const int32b x = getClampInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const int2 x = zMakeInt2(0, INT_MAX); \
//      const int2 minval = zBroadcast2(-1); \
//      const int2 maxval = zBroadcast2(1); \
//      results2[i++] = x; \
//      results2[i++] = minval; \
//      results2[i++] = maxval; \
//      results2[i++] = clamp2(x, minval, maxval); \
//    } \
//    { \
//      const int2 x = zMakeInt2(-1, INT_MIN); \
//      const int2 minval = zBroadcast2(INT_MIN); \
//      const int2 maxval = zBroadcast2(INT_MAX); \
//      results2[i++] = x; \
//      results2[i++] = minval; \
//      results2[i++] = maxval; \
//      results2[i++] = clamp2(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const int3 x = zMakeInt3(0, 1, INT_MAX); \
//      const int3 minval = zBroadcast3(-1); \
//      const int3 maxval = zBroadcast3(1); \
//      results3[i++] = x; \
//      results3[i++] = minval; \
//      results3[i++] = maxval; \
//      results3[i++] = clamp3(x, minval, maxval); \
//    } \
//    { \
//      const int3 x = zMakeInt3(0, -1, INT_MIN); \
//      const int3 minval = zBroadcast3(INT_MIN); \
//      const int3 maxval = zBroadcast3(INT_MAX); \
//      results3[i++] = x; \
//      results3[i++] = minval; \
//      results3[i++] = maxval; \
//      results3[i++] = clamp3(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const int4 x = zMakeInt4(0, 1, 10, INT_MAX); \
//      const int4 minval = zBroadcast4(-1); \
//      const int4 maxval = zBroadcast4(1); \
//      results4[i++] = x; \
//      results4[i++] = minval; \
//      results4[i++] = maxval; \
//      results4[i++] = clamp4(x, minval, maxval); \
//    } \
//    { \
//      const int4 x = zMakeInt4(0, -1, -10, INT_MIN); \
//      const int4 minval = zBroadcast4(INT_MIN); \
//      const int4 maxval = zBroadcast4(INT_MAX); \
//      results4[i++] = x; \
//      results4[i++] = minval; \
//      results4[i++] = maxval; \
//      results4[i++] = clamp4(x, minval, maxval); \
//    } \
//  }
//
//__kernel void testClamp(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_CLAMP(clamp, clamp, clamp, clamp,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClampImpl(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_CLAMP(zClampImpl, zClamp2Impl, zClamp3Impl, zClamp4Impl,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClamp(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_CLAMP(zClamp, zClamp2, zClamp3, zClamp4,
//      results1, results2, results3, results4);
//}
//
//uint32b getClampUInput(const size_t i)
//{
//  const uint32b x_list[] = {0u, 1u, 2u, UINT_MAX};
//  return x_list[i];
//}
//
//#define ZINVUL_TEST_CLAMPU(clamp1, clamp2, clamp3, clamp4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    size_t i = 0; \
//    for (size_t j = 0; j < 4; ++j) { \
//      const uint32b minval = 0u; \
//      const uint32b maxval = 0u; \
//      const uint32b x = getClampUInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    for (size_t j = 0; j < 4; ++j) { \
//      const uint32b minval = 1u; \
//      const uint32b maxval = 3u; \
//      const uint32b x = getClampUInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    for (size_t j = 0; j < 4; ++j) { \
//      const uint32b minval = 0u; \
//      const uint32b maxval = UINT_MAX; \
//      const uint32b x = getClampUInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const uint2 x = zMakeUInt2(0u, UINT_MAX); \
//      const uint2 minval = zBroadcastU2(1u); \
//      const uint2 maxval = zBroadcastU2(3u); \
//      results2[i++] = x; \
//      results2[i++] = minval; \
//      results2[i++] = maxval; \
//      results2[i++] = clamp2(x, minval, maxval); \
//    } \
//    { \
//      const uint2 x = zMakeUInt2(1u, UINT_MAX); \
//      const uint2 minval = zBroadcastU2(0u); \
//      const uint2 maxval = zBroadcastU2(INT_MAX); \
//      results2[i++] = x; \
//      results2[i++] = minval; \
//      results2[i++] = maxval; \
//      results2[i++] = clamp2(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const uint3 x = zMakeUInt3(0u, 1u, UINT_MAX); \
//      const uint3 minval = zBroadcastU3(1u); \
//      const uint3 maxval = zBroadcastU3(3u); \
//      results3[i++] = x; \
//      results3[i++] = minval; \
//      results3[i++] = maxval; \
//      results3[i++] = clamp3(x, minval, maxval); \
//    } \
//    { \
//      const uint3 x = zMakeUInt3(0u, 1u, UINT_MAX); \
//      const uint3 minval = zBroadcastU3(0u); \
//      const uint3 maxval = zBroadcastU3(UINT_MAX); \
//      results3[i++] = x; \
//      results3[i++] = minval; \
//      results3[i++] = maxval; \
//      results3[i++] = clamp3(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const uint4 x = zMakeUInt4(0u, 1u, 10u, UINT_MAX); \
//      const uint4 minval = zBroadcastU4(1u); \
//      const uint4 maxval = zBroadcastU4(3u); \
//      results4[i++] = x; \
//      results4[i++] = minval; \
//      results4[i++] = maxval; \
//      results4[i++] = clamp4(x, minval, maxval); \
//    } \
//    { \
//      const uint4 x = zMakeUInt4(0u, 1u, 10u, UINT_MAX); \
//      const uint4 minval = zBroadcastU4(0u); \
//      const uint4 maxval = zBroadcastU4(UINT_MAX); \
//      results4[i++] = x; \
//      results4[i++] = minval; \
//      results4[i++] = maxval; \
//      results4[i++] = clamp4(x, minval, maxval); \
//    } \
//  }
//
//__kernel void testClampU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_CLAMPU(clamp, clamp, clamp, clamp,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClampUImpl(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_CLAMPU(zClampUImpl, zClampU2Impl, zClampU3Impl, zClampU4Impl,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClampU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_CLAMPU(zClampU, zClampU2, zClampU3, zClampU4,
//      results1, results2, results3, results4);
//}
//
//float getClampFInput(const size_t i)
//{
//  const float x_list[] = {0.0f, 1.0f, FLT_MAX, -1.0f, -FLT_MAX, INFINITY, -INFINITY};
//  return x_list[i];
//}
//
//#define ZINVUL_TEST_CLAMPF(clamp1, clamp2f, clamp2, clamp3f, clamp3, clamp4f, clamp4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    size_t i = 0; \
//    for (size_t j = 0; j < 7; ++j) { \
//      const float minval = 0.0f; \
//      const float maxval = 0.0f; \
//      const float x = getClampFInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    for (size_t j = 0; j < 7; ++j) { \
//      const float minval = -1.0f; \
//      const float maxval = 1.0f; \
//      const float x = getClampFInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    for (size_t j = 0; j < 7; ++j) { \
//      const float minval = -INFINITY; \
//      const float maxval = INFINITY; \
//      const float x = getClampFInput(j); \
//      results1[i++] = x; \
//      results1[i++] = minval; \
//      results1[i++] = maxval; \
//      results1[i++] = clamp1(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const float2 x = zMakeFloat2(0.0f, FLT_MAX); \
//      const float minval = -1.0f; \
//      const float maxval = 1.0f; \
//      results2[i++] = x; \
//      results2[i++] = zBroadcastF2(minval); \
//      results2[i++] = zBroadcastF2(maxval); \
//      results2[i++] = clamp2f(x, minval, maxval); \
//    } \
//    { \
//      const float2 x = zMakeFloat2(-1.0f, -FLT_MAX); \
//      const float minval = -INFINITY; \
//      const float maxval = INFINITY; \
//      results2[i++] = x; \
//      results2[i++] = zBroadcastF2(minval); \
//      results2[i++] = zBroadcastF2(maxval); \
//      results2[i++] = clamp2f(x, minval, maxval); \
//    } \
//    { \
//      const float2 x = zMakeFloat2(0.0f, FLT_MAX); \
//      const float2 minval = zBroadcastF2(-1.0f); \
//      const float2 maxval = zBroadcastF2(1.0f); \
//      results2[i++] = x; \
//      results2[i++] = minval; \
//      results2[i++] = maxval; \
//      results2[i++] = clamp2(x, minval, maxval); \
//    } \
//    { \
//      const float2 x = zMakeFloat2(-1.0f, -FLT_MAX); \
//      const float2 minval = zBroadcastF2(-INFINITY); \
//      const float2 maxval = zBroadcastF2(INFINITY); \
//      results2[i++] = x; \
//      results2[i++] = minval; \
//      results2[i++] = maxval; \
//      results2[i++] = clamp2(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const float3 x = zMakeFloat3(0.0f, 1.0f, FLT_MAX); \
//      const float minval = -1.0f; \
//      const float maxval = 1.0f; \
//      results3[i++] = x; \
//      results3[i++] = zBroadcastF3(minval); \
//      results3[i++] = zBroadcastF3(maxval); \
//      results3[i++] = clamp3f(x, minval, maxval); \
//    } \
//    { \
//      const float3 x = zMakeFloat3(-1.0f, -FLT_MAX, -INFINITY); \
//      const float minval = -INFINITY; \
//      const float maxval = INFINITY; \
//      results3[i++] = x; \
//      results3[i++] = zBroadcastF3(minval); \
//      results3[i++] = zBroadcastF3(maxval); \
//      results3[i++] = clamp3f(x, minval, maxval); \
//    } \
//    { \
//      const float3 x = zMakeFloat3(0.0f, 1.0f, FLT_MAX); \
//      const float3 minval = zBroadcastF3(-1.0f); \
//      const float3 maxval = zBroadcastF3(1.0f); \
//      results3[i++] = x; \
//      results3[i++] = minval; \
//      results3[i++] = maxval; \
//      results3[i++] = clamp3(x, minval, maxval); \
//    } \
//    { \
//      const float3 x = zMakeFloat3(-1.0f, -FLT_MAX, -INFINITY); \
//      const float3 minval = zBroadcastF3(-INFINITY); \
//      const float3 maxval = zBroadcastF3(INFINITY); \
//      results3[i++] = x; \
//      results3[i++] = minval; \
//      results3[i++] = maxval; \
//      results3[i++] = clamp3(x, minval, maxval); \
//    } \
//    i = 0; \
//    { \
//      const float4 x = zMakeFloat4(0.0f, 1.0f, FLT_MAX, INFINITY); \
//      const float minval = -1.0f; \
//      const float maxval = 1.0f; \
//      results4[i++] = x; \
//      results4[i++] = zBroadcastF4(minval); \
//      results4[i++] = zBroadcastF4(maxval); \
//      results4[i++] = clamp4f(x, minval, maxval); \
//    } \
//    { \
//      const float4 x = zMakeFloat4(0.0f, -1.0f, -FLT_MAX, -INFINITY); \
//      const float minval = -INFINITY; \
//      const float maxval = INFINITY; \
//      results4[i++] = x; \
//      results4[i++] = zBroadcastF4(minval); \
//      results4[i++] = zBroadcastF4(maxval); \
//      results4[i++] = clamp4f(x, minval, maxval); \
//    } \
//    { \
//      const float4 x = zMakeFloat4(0.0f, 1.0f, FLT_MAX, INFINITY); \
//      const float4 minval = zBroadcastF4(-1.0f); \
//      const float4 maxval = zBroadcastF4(1.0f); \
//      results4[i++] = x; \
//      results4[i++] = minval; \
//      results4[i++] = maxval; \
//      results4[i++] = clamp4(x, minval, maxval); \
//    } \
//    { \
//      const float4 x = zMakeFloat4(0.0f, -1.0f, -FLT_MAX, -INFINITY); \
//      const float4 minval = zBroadcastF4(-INFINITY); \
//      const float4 maxval = zBroadcastF4(INFINITY); \
//      results4[i++] = x; \
//      results4[i++] = minval; \
//      results4[i++] = maxval; \
//      results4[i++] = clamp4(x, minval, maxval); \
//    } \
//  }
//
//__kernel void testClampF(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_CLAMPF(clamp, clamp, clamp, clamp, clamp, clamp, clamp,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClampFImpl(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_CLAMPF(zClampFImpl, zClampF2FImpl, zClampF2Impl, zClampF3FImpl, zClampF3Impl, zClampF4FImpl, zClampF4Impl,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClampF(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_CLAMPF(zClampF, zClampF2F, zClampF2, zClampF3F, zClampF3, zClampF4F, zClampF4, 
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_FRACT(fract1, fract2, fract3, fract4, results1, results1i, results2, results2i, results3, results3i, results4, results4i) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      uint i = 0; \
//      float iptr = 0.0f; \
//      { \
//        const float x = 0.0f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = 1.0f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = 3.1f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = 3.5f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = 3.9f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = 4.1f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = 4.5f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = 4.9f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = -1.0f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = -3.1f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = -3.5f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = -3.9f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = -4.1f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = -4.5f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = -4.9f; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = -INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//      { \
//        const float x = NAN; \
//        results1[i++] = x; \
//        results1[i++] = fract1(x, &iptr); \
//        results1i[i/2-1] = iptr; \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      float2 iptr = zMakeFloat2(0.0f, 0.0f); \
//      { \
//        const float2 x = zMakeFloat2(0.0f, 0.0f); \
//        results2[i++] = x; \
//        results2[i++] = fract2(x, &iptr); \
//        results2i[i/2-1] = iptr; \
//      } \
//      { \
//        const float2 x = zMakeFloat2(1.0f, -1.0f); \
//        results2[i++] = x; \
//        results2[i++] = fract2(x, &iptr); \
//        results2i[i/2-1] = iptr; \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.1f, -3.1f); \
//        results2[i++] = x; \
//        results2[i++] = fract2(x, &iptr); \
//        results2i[i/2-1] = iptr; \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.5f, -3.5f); \
//        results2[i++] = x; \
//        results2[i++] = fract2(x, &iptr); \
//        results2i[i/2-1] = iptr; \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.9f, -3.9f); \
//        results2[i++] = x; \
//        results2[i++] = fract2(x, &iptr); \
//        results2i[i/2-1] = iptr; \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.1f, -4.1f); \
//        results2[i++] = x; \
//        results2[i++] = fract2(x, &iptr); \
//        results2i[i/2-1] = iptr; \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.5f, -4.5f); \
//        results2[i++] = x; \
//        results2[i++] = fract2(x, &iptr); \
//        results2i[i/2-1] = iptr; \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.9f, -4.9f); \
//        results2[i++] = x; \
//        results2[i++] = fract2(x, &iptr); \
//        results2i[i/2-1] = iptr; \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      float3 iptr = zMakeFloat3(0.0f, 0.0f, 0.0f); \
//      { \
//        const float3 x = zMakeFloat3(0.0f, 0.0f, 0.0f); \
//        results3[i++] = x; \
//        results3[i++] = fract3(x, &iptr); \
//        results3i[i/2-1] = iptr; \
//      } \
//      { \
//        const float3 x = zMakeFloat3(1.0f, -1.0f, 1.0f); \
//        results3[i++] = x; \
//        results3[i++] = fract3(x, &iptr); \
//        results3i[i/2-1] = iptr; \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.1f, -3.1f, 3.1f); \
//        results3[i++] = x; \
//        results3[i++] = fract3(x, &iptr); \
//        results3i[i/2-1] = iptr; \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.5f, -3.5f, 3.5f); \
//        results3[i++] = x; \
//        results3[i++] = fract3(x, &iptr); \
//        results3i[i/2-1] = iptr; \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.9f, -3.9f, 3.9f); \
//        results3[i++] = x; \
//        results3[i++] = fract3(x, &iptr); \
//        results3i[i/2-1] = iptr; \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.1f, -4.1f, 4.1f); \
//        results3[i++] = x; \
//        results3[i++] = fract3(x, &iptr); \
//        results3i[i/2-1] = iptr; \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.5f, -4.5f, 4.5f); \
//        results3[i++] = x; \
//        results3[i++] = fract3(x, &iptr); \
//        results3i[i/2-1] = iptr; \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.9f, -4.9f, 4.9f); \
//        results3[i++] = x; \
//        results3[i++] = fract3(x, &iptr); \
//        results3i[i/2-1] = iptr; \
//      } \
//    } \
//    { \
//      uint i = 0; \
//      float4 iptr = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f); \
//      { \
//        const float4 x = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f); \
//        results4[i++] = x; \
//        results4[i++] = fract4(x, &iptr); \
//        results4i[i/2-1] = iptr; \
//      } \
//      { \
//        const float4 x = zMakeFloat4(1.0f, -1.0f, 5.0f, -5.0f); \
//        results4[i++] = x; \
//        results4[i++] = fract4(x, &iptr); \
//        results4i[i/2-1] = iptr; \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.1f, -3.1f, 13.1f, -13.1f); \
//        results4[i++] = x; \
//        results4[i++] = fract4(x, &iptr); \
//        results4i[i/2-1] = iptr; \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.5f, -3.5f, 13.5f, -13.5f); \
//        results4[i++] = x; \
//        results4[i++] = fract4(x, &iptr); \
//        results4i[i/2-1] = iptr; \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.9f, -3.9f, 13.9f, -13.9f); \
//        results4[i++] = x; \
//        results4[i++] = fract4(x, &iptr); \
//        results4i[i/2-1] = iptr; \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.1f, -4.1f, 14.1f, -14.1f); \
//        results4[i++] = x; \
//        results4[i++] = fract4(x, &iptr); \
//        results4i[i/2-1] = iptr; \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.5f, -4.5f, 14.5f, -14.5f); \
//        results4[i++] = x; \
//        results4[i++] = fract4(x, &iptr); \
//        results4i[i/2-1] = iptr; \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.9f, -4.9f, 14.9f, -14.9f); \
//        results4[i++] = x; \
//        results4[i++] = fract4(x, &iptr); \
//        results4i[i/2-1] = iptr; \
//      } \
//    } \
//  } \
//
///*!
//  */
//__kernel void testFract(
//    __global float* results1, __global float* results1i,
//    __global float2* results2, __global float2* results2i,
//    __global float3* results3, __global float3* results3i,
//    __global float4* results4, __global float4* results4i)
//{
//  ZINVUL_TEST_FRACT(fract, fract, fract, fract,
//      results1, results1i, results2, results2i, results3, results3i, results4, results4i);
//}
//
///*!
//  */
//__kernel void testzFractImpl(
//    __global float* results1, __global float* results1i,
//    __global float2* results2, __global float2* results2i,
//    __global float3* results3, __global float3* results3i,
//    __global float4* results4, __global float4* results4i)
//{
//  ZINVUL_TEST_FRACT(zFractImpl, zFract2Impl, zFract3Impl, zFract4Impl,
//      results1, results1i, results2, results2i, results3, results3i, results4, results4i);
//}
//
///*!
//  */
//__kernel void testzFract(
//    __global float* results1, __global float* results1i,
//    __global float2* results2, __global float2* results2i,
//    __global float3* results3, __global float3* results3i,
//    __global float4* results4, __global float4* results4i)
//{
//  ZINVUL_TEST_FRACT(zFract, zFract2, zFract3, zFract4,
//      results1, results1i, results2, results2i, results3, results3i, results4, results4i);
//}
//
//#define ZINVUL_TEST_MOD(mod1, mod2, mod3, mod4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      const float y = 1.0f; \
//      uint i = 0; \
//      { \
//        const float x = 0.0f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = 1.0f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = 3.1f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = 3.5f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = 3.9f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = 4.1f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = 4.5f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = 4.9f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = -1.0f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = -3.1f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = -3.5f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = -3.9f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = -4.1f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = -4.5f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = -4.9f; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = -INFINITY; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//      { \
//        const float x = NAN; \
//        results1[i++] = x; \
//        results1[i++] = mod1(x, y); \
//      } \
//    } \
//    { \
//      const float2 y = zMakeFloat2(1.0f, 2.0f); \
//      uint i = 0; \
//      { \
//        const float2 x = zMakeFloat2(0.0f, 0.0f); \
//        results2[i++] = x; \
//        results2[i++] = mod2(x, y); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(1.0f, -1.0f); \
//        results2[i++] = x; \
//        results2[i++] = mod2(x, y); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.1f, -3.1f); \
//        results2[i++] = x; \
//        results2[i++] = mod2(x, y); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.5f, -3.5f); \
//        results2[i++] = x; \
//        results2[i++] = mod2(x, y); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(3.9f, -3.9f); \
//        results2[i++] = x; \
//        results2[i++] = mod2(x, y); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.1f, -4.1f); \
//        results2[i++] = x; \
//        results2[i++] = mod2(x, y); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.5f, -4.5f); \
//        results2[i++] = x; \
//        results2[i++] = mod2(x, y); \
//      } \
//      { \
//        const float2 x = zMakeFloat2(4.9f, -4.9f); \
//        results2[i++] = x; \
//        results2[i++] = mod2(x, y); \
//      } \
//    } \
//    { \
//      const float3 y = zMakeFloat3(1.0f, 2.0f, 3.0f); \
//      uint i = 0; \
//      { \
//        const float3 x = zMakeFloat3(0.0f, 0.0f, 0.0f); \
//        results3[i++] = x; \
//        results3[i++] = mod3(x, y); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(1.0f, -1.0f, 1.0f); \
//        results3[i++] = x; \
//        results3[i++] = mod3(x, y); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.1f, -3.1f, 3.1f); \
//        results3[i++] = x; \
//        results3[i++] = mod3(x, y); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.5f, -3.5f, 3.5f); \
//        results3[i++] = x; \
//        results3[i++] = mod3(x, y); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(3.9f, -3.9f, 3.9f); \
//        results3[i++] = x; \
//        results3[i++] = mod3(x, y); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.1f, -4.1f, 4.1f); \
//        results3[i++] = x; \
//        results3[i++] = mod3(x, y); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.5f, -4.5f, 4.5f); \
//        results3[i++] = x; \
//        results3[i++] = mod3(x, y); \
//      } \
//      { \
//        const float3 x = zMakeFloat3(4.9f, -4.9f, 4.9f); \
//        results3[i++] = x; \
//        results3[i++] = mod3(x, y); \
//      } \
//    } \
//    { \
//      const float4 y = zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f); \
//      uint i = 0; \
//      { \
//        const float4 x = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f); \
//        results4[i++] = x; \
//        results4[i++] = mod4(x, y); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(1.0f, -1.0f, 5.0f, -5.0f); \
//        results4[i++] = x; \
//        results4[i++] = mod4(x, y); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.1f, -3.1f, 13.1f, -13.1f); \
//        results4[i++] = x; \
//        results4[i++] = mod4(x, y); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.5f, -3.5f, 13.5f, -13.5f); \
//        results4[i++] = x; \
//        results4[i++] = mod4(x, y); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(3.9f, -3.9f, 13.9f, -13.9f); \
//        results4[i++] = x; \
//        results4[i++] = mod4(x, y); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.1f, -4.1f, 14.1f, -14.1f); \
//        results4[i++] = x; \
//        results4[i++] = mod4(x, y); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.5f, -4.5f, 14.5f, -14.5f); \
//        results4[i++] = x; \
//        results4[i++] = mod4(x, y); \
//      } \
//      { \
//        const float4 x = zMakeFloat4(4.9f, -4.9f, 14.9f, -14.9f); \
//        results4[i++] = x; \
//        results4[i++] = mod4(x, y); \
//      } \
//    } \
//  }
//
///*!
//  */
//__kernel void testMod(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MOD(fmod, fmod, fmod, fmod,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzModImpl(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MOD(zModImpl, zMod2Impl, zMod3Impl, zMod4Impl,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzMod(__global float* results1,
//    __global float2* results2,
//    __global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_MOD(zMod, zMod2, zMod3, zMod4,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//#define ZINVUL_TEST_RADIAN(degrees1, degrees2, degrees3, degrees4, radians1, radians2, radians3, radians4,  results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      results1[0] = degrees1(zPiF); \
//      results1[1] = radians1(180.0f); \
//    } \
//    { \
//      results2[0] = degrees2(zMakeFloat2(zPiF, 0.5f * zPiF)); \
//      results2[1] = radians2(zMakeFloat2(180.0f, 90.0f)); \
//    } \
//    { \
//      results3[0] = degrees3(zMakeFloat3(zPiF, 0.5f * zPiF, 0.25f * zPiF)); \
//      results3[1] = radians3(zMakeFloat3(180.0f, 90.0f, 45.0f)); \
//    } \
//    { \
//      results4[0] = degrees4(zMakeFloat4(zPiF, 0.5f * zPiF, 0.25f * zPiF, 0.0f)); \
//      results4[1] = radians4(zMakeFloat4(180.0f, 90.0f, 45.0f, 0.0f)); \
//    } \
//  }
//
///*!
//  */
//kernel void testRadian(global float* results1,
//    global float2* results2,
//    global float3* results3,
//    global float4* results4)
//{
//  ZINVUL_TEST_RADIAN(degrees, degrees, degrees, degrees,
//      radians, radians, radians, radians,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//kernel void testzRadianImpl(global float* results1,
//    global float2* results2,
//    global float3* results3,
//    global float4* results4)
//{
//  ZINVUL_TEST_RADIAN(zDegreesImpl, zDegrees2Impl, zDegrees3Impl, zDegrees4Impl,
//      zRadiansImpl, zRadians2Impl, zRadians3Impl, zRadians4Impl,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//kernel void testzRadian(global float* results1,
//    global float2* results2,
//    global float3* results3,
//    global float4* results4)
//{
//  ZINVUL_TEST_RADIAN(zDegrees, zDegrees2, zDegrees3, zDegrees4,
//      zRadians, zRadians2, zRadians3, zRadians4,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//#define ZINVUL_TEST_CLZ(clz1, clz2, clz3, clz4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      int x = 0; \
//      for (int i = 0; i < 32; ++i) { \
//        results1[i] = clz1(x); \
//        x = (x << 1) | 1; \
//      } \
//    } \
//    { \
//      int2 x = zMakeInt2(0, 0); \
//      for (int i = 0; i < 32; ++i) { \
//        const int2 y = x / zMakeInt2(1, 2); \
//        results2[i] = clz2(y); \
//        x = (x << 1) | 1; \
//      } \
//    } \
//    { \
//      int3 x = zMakeInt3(0, 0, 0); \
//      for (int i = 0; i < 32; ++i) { \
//        const int3 y = x / zMakeInt3(1, 2, 3); \
//        results3[i] = clz3(y); \
//        x = (x << 1) | 1; \
//      } \
//    } \
//    { \
//      int4 x = zMakeInt4(0, 0, 0, 0); \
//      for (int i = 0; i < 32; ++i) { \
//        const int4 y = x / zMakeInt4(1, 2, 3, 5); \
//        results4[i] = clz4(y); \
//        x = (x << 1) | 1; \
//      } \
//    } \
//  }
//
//__kernel void testClz(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_CLZ(clz, clz ,clz, clz,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClzImpl(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_CLZ(zClzImpl, zClz2Impl, zClz3Impl, zClz4Impl,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClz(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_CLZ(zClz, zClz2, zClz3, zClz4,
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_CLZU(clz1, clz2, clz3, clz4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      uint x = 0u; \
//      for (uint i = 0u; i < 32u; ++i) { \
//        results1[i] = clz1(x); \
//        x = (x << 1u) | 1u; \
//      } \
//    } \
//    { \
//      uint2 x = zMakeUInt2(0u, 0u); \
//      for (uint i = 0u; i < 32u; ++i) { \
//        const uint2 y = x / zMakeUInt2(1u, 2u); \
//        results2[i] = clz2(y); \
//        x = (x << 1u) | 1u; \
//      } \
//    } \
//    { \
//      uint3 x = zMakeUInt3(0u, 0u, 0u); \
//      for (uint i = 0u; i < 32u; ++i) { \
//        const uint3 y = x / zMakeUInt3(1u, 2u, 3u); \
//        results3[i] = clz3(y); \
//        x = (x << 1u) | 1u; \
//      } \
//    } \
//    { \
//      uint4 x = zMakeUInt4(0u, 0u, 0u, 0u); \
//      for (uint i = 0u; i < 32u; ++i) { \
//        const uint4 y = x / zMakeUInt4(1u, 2u, 3u, 5u); \
//        results4[i] = clz4(y); \
//        x = (x << 1u) | 1u; \
//      } \
//    } \
//  }
//
//__kernel void testClzU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_CLZU(clz, clz ,clz, clz,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClzUImpl(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_CLZU(zClzUImpl, zClzU2Impl, zClzU3Impl, zClzU4Impl,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzClzU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_CLZU(zClzU, zClzU2, zClzU3, zClzU4,
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_POPCOUNT(popcount1, popcount2, popcount3, popcount4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      int x = 0; \
//      for (int i = 0; i < 32; ++i) { \
//        results1[i] = popcount1(x); \
//        x = (x << 1) | 1; \
//      } \
//    } \
//    { \
//      int2 x = zMakeInt2(0, 0); \
//      for (int i = 0; i < 32; ++i) { \
//        const int2 y = x / zMakeInt2(1, 2); \
//        results2[i] = popcount2(y); \
//        x = (x << 1) | 1; \
//      } \
//    } \
//    { \
//      int3 x = zMakeInt3(0, 0, 0); \
//      for (int i = 0; i < 32; ++i) { \
//        const int3 y = x / zMakeInt3(1, 2, 3); \
//        results3[i] = popcount3(y); \
//        x = (x << 1) | 1; \
//      } \
//    } \
//    { \
//      int4 x = zMakeInt4(0, 0, 0, 0); \
//      for (int i = 0; i < 32; ++i) { \
//        const int4 y = x / zMakeInt4(1, 2, 3, 5); \
//        results4[i] = popcount4(y); \
//        x = (x << 1) | 1; \
//      } \
//    } \
//  }
//
///*!
//  */
//__kernel void testPopcount(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_POPCOUNT(popcount, popcount, popcount, popcount,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzPopcountImpl(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_POPCOUNT(zPopcountImpl, zPopcount2Impl, zPopcount3Impl, zPopcount4Impl,
//      results1, results2, results3, results4);
//}
//
///*!
//  */
//__kernel void testzPopcount(__global int32b* results1,
//    __global int2* results2,
//    __global int3* results3,
//    __global int4* results4)
//{
//  ZINVUL_TEST_POPCOUNT(zPopcount, zPopcount2, zPopcount3, zPopcount4,
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_POPCOUNTU(popcount1, popcount2, popcount3, popcount4, results1, results2, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    { \
//      uint x = 0u; \
//      for (uint i = 0u; i < 32u; ++i) { \
//        results1[i] = popcount1(x); \
//        x = (x << 1u) | 1u; \
//      } \
//    } \
//    { \
//      uint2 x = zMakeUInt2(0u, 0u); \
//      for (uint i = 0u; i < 32u; ++i) { \
//        const uint2 y = x / zMakeUInt2(1u, 2u); \
//        results2[i] = popcount2(y); \
//        x = (x << 1u) | 1u; \
//      } \
//    } \
//    { \
//      uint3 x = zMakeUInt3(0u, 0u, 0u); \
//      for (uint i = 0u; i < 32u; ++i) { \
//        const uint3 y = x / zMakeUInt3(1u, 2u, 3u); \
//        results3[i] = popcount3(y); \
//        x = (x << 1u) | 1u; \
//      } \
//    } \
//    { \
//      uint4 x = zMakeUInt4(0u, 0u, 0u, 0u); \
//      for (uint i = 0u; i < 32u; ++i) { \
//        const uint4 y = x / zMakeUInt4(1u, 2u, 3u, 5u); \
//        results4[i] = popcount4(y); \
//        x = (x << 1u) | 1u; \
//      } \
//    } \
//  }
//
//__kernel void testPopcountU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_POPCOUNTU(popcount, popcount, popcount, popcount,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzPopcountUImpl(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_POPCOUNTU(zPopcountUImpl, zPopcountU2Impl, zPopcountU3Impl, zPopcountU4Impl,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzPopcountU(__global uint32b* results1,
//    __global uint2* results2,
//    __global uint3* results3,
//    __global uint4* results4)
//{
//  ZINVUL_TEST_POPCOUNTU(zPopcountU, zPopcountU2, zPopcountU3, zPopcountU4,
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_DOT(dot2, dot3, dot3f4, dot4, k, results2, results3, results3f4, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    uint i = 0; \
//    { \
//      const float2 p0 = zMakeFloat2(1.0f, 0.0f); \
//      const float2 p1 = zMakeFloat2(0.0f, 1.0f); \
//      results2[i++] = dot2(p0, p1); \
//    } \
//    { \
//      const float2 p0 = zMakeFloat2(-4.0f, -9.0f); \
//      const float2 p1 = zMakeFloat2(-1.0f, 2.0f); \
//      results2[i++] = dot2(p0, p1); \
//    } \
//    { \
//      const float2 p0 = zMakeFloat2(1.0f, 1.0f); \
//      const float2 p1 = zMakeFloat2(5.0f, 5.0f); \
//      results2[i++] = dot2(p0, p1); \
//    } \
//    i = 0; \
//    { \
//      const float3 p0 = zMakeFloat3(1.0f, 2.0f, 3.0f); \
//      const float3 p1 = zMakeFloat3(4.0f, -5.0f, 6.0f); \
//      results3[i++] = dot3(p0, p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(6.0f, -1.0f, 3.0f); \
//      const float3 p1 = zMakeFloat3(4.0f, 18.0f, -2.0f); \
//      results3[i++] = dot3(p0, p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(-7.0f, 4.0f, -3.0f); \
//      const float3 p1 = zMakeFloat3(8.0f, 11.0f, -12.0f); \
//      results3[i++] = dot3(p0, p1); \
//    } \
//    i = 0; \
//    { \
//      const float4 p0 = zMakeFloat4(1.0f, 2.0f, 3.0f, k); \
//      const float4 p1 = zMakeFloat4(4.0f, -5.0f, 6.0f, k); \
//      results3f4[i++] = dot3f4(p0, p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(6.0f, -1.0f, 3.0f, k); \
//      const float4 p1 = zMakeFloat4(4.0f, 18.0f, -2.0f, k); \
//      results3f4[i++] = dot3f4(p0, p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(-7.0f, 4.0f, -3.0f, k); \
//      const float4 p1 = zMakeFloat4(8.0f, 11.0f, -12.0f, k); \
//      results3f4[i++] = dot3f4(p0, p1); \
//    } \
//    i = 0; \
//    { \
//      const float4 p0 = zMakeFloat4(1.0f, 2.0f, -5.0f, 2.0f); \
//      const float4 p1 = zMakeFloat4(4.0f, 8.0f, 1.0f, -2.0f); \
//      results4[i++] = dot4(p0, p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(1.0f, 0.0f, 1.0f, 0.0f); \
//      const float4 p1 = zMakeFloat4(-3.0f, -3.0f, -3.0f, -3.0f); \
//      results4[i++] = dot4(p0, p1); \
//    } \
//  }
//
//__kernel void testDot(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_DOT(dot, dot, dot, dot, 0.0f,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzDotImpl(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_DOT(zDot2Impl, zDot3Impl, zDot3F4Impl, zDot4Impl, 1.0f,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzDot(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_DOT(zDot2, zDot3, zDot3F4, zDot4, 1.0f,
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_CROSS(cross3, cross4, results3, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    uint i = 0; \
//    { \
//      const float3 p0 = zMakeFloat3(1.0f, 0.0f, 0.0f); \
//      const float3 p1 = zMakeFloat3(0.0f, 1.0f, 0.0f); \
//      results3[i++] = cross3(p0, p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(1.0f, 2.0f, 3.0f); \
//      const float3 p1 = zMakeFloat3(4.0f, 5.0f, 6.0f); \
//      results3[i++] = cross3(p0, p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(2.0f, 3.0f, 4.0f); \
//      const float3 p1 = zMakeFloat3(5.0f, 6.0f, 7.0f); \
//      results3[i++] = cross3(p0, p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(2.0f, -3.0f, 1.0f); \
//      const float3 p1 = zMakeFloat3(4.0f, -1.0f, 5.0f); \
//      results3[i++] = cross3(p0, p1); \
//    } \
//    i = 0; \
//    { \
//      const float4 p0 = zMakeFloat4(1.0f, 0.0f, 0.0f, 1.0f); \
//      const float4 p1 = zMakeFloat4(0.0f, 1.0f, 0.0f, 1.0f); \
//      results4[i++] = cross4(p0, p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(1.0f, 2.0f, 3.0f, 1.0f); \
//      const float4 p1 = zMakeFloat4(4.0f, 5.0f, 6.0f, 1.0f); \
//      results4[i++] = cross4(p0, p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(2.0f, 3.0f, 4.0f, 1.0f); \
//      const float4 p1 = zMakeFloat4(5.0f, 6.0f, 7.0f, 1.0f); \
//      results4[i++] = cross4(p0, p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(2.0f, -3.0f, 1.0f, 1.0f); \
//      const float4 p1 = zMakeFloat4(4.0f, -1.0f, 5.0f, 1.0f); \
//      results4[i++] = cross4(p0, p1); \
//    } \
//  }
//
//#if !defined(Z_MAC)
//
//__kernel void testCross(__global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_CROSS(cross, cross, results3, results4);
//}
//
//#endif // Z_MAC
//
//__kernel void testzCrossImpl(__global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_CROSS(zCross3Impl, zCross4Impl, results3, results4);
//}
//
//__kernel void testzCross(__global float3* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_CROSS(zCross3, zCross4, results3, results4);
//}
//
//#define ZINVUL_TEST_LENGTH(length2, length3, length3f4, length4, k, results2, results3, results3f4, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    uint i = 0; \
//    { \
//      const float2 p0 = zMakeFloat2(zSqrt2F, zSqrt2F); \
//      const float2 p1 = zMakeFloat2(0.0f, 0.0f); \
//      results2[i++] = length2(p0 - p1); \
//    } \
//    i = 0; \
//    { \
//      const float3 p0 = zMakeFloat3(5.0f, 3.0f, 0.0f); \
//      const float3 p1 = zMakeFloat3(2.0f, -2.0f, zSqrt2F); \
//      results3[i++] = length3(p0 - p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(1.0f, 0.0f, -5.0f); \
//      const float3 p1 = zMakeFloat3(-3.0f, 2.0f, -1.0f); \
//      results3[i++] = length3(p0 - p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(2.0f, 3.0f, 5.0f); \
//      const float3 p1 = zMakeFloat3(2.0f, 0.0f, 9.0f); \
//      results3[i++] = length3(p0 - p1); \
//    } \
//    i = 0; \
//    { \
//      const float4 p0 = zMakeFloat4(5.0f, 3.0f, 0.0f, k); \
//      const float4 p1 = zMakeFloat4(2.0f, -2.0f, zSqrt2F, k); \
//      results3f4[i++] = length3f4(p0 - p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(1.0f, 0.0f, -5.0f, k); \
//      const float4 p1 = zMakeFloat4(-3.0f, 2.0f, -1.0f, k); \
//      results3f4[i++] = length3f4(p0 - p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(2.0f, 3.0f, 5.0f, k); \
//      const float4 p1 = zMakeFloat4(2.0f, 0.0f, 9.0f, k); \
//      results3f4[i++] = length3f4(p0 - p1); \
//    } \
//    i = 0; \
//    { \
//      const float4 p0 = zMakeFloat4(2.0f, 3.0f, 4.0f, 2.0f); \
//      const float4 p1 = zMakeFloat4(1.0f, -2.0f, 1.0f, 3.0f); \
//      results4[i++] = length4(p0 - p1); \
//    } \
//  } \
//
//__kernel void testLength(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_LENGTH(length, length, length, length, 0.0f,
//      results1, results2, results3, results4);
//}
// 
//__kernel void testzLengthImpl(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_LENGTH(zLength2Impl, zLength3Impl, zLength3F4Impl, zLength4Impl, 1.0f,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzLength(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_LENGTH(zLength2, zLength3, zLength3F4, zLength4, 1.0f,
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_DISTANCE(distance2, distance3, distance3f4, distance4, k, results2, results3, results3f4, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    uint i = 0; \
//    { \
//      const float2 p0 = zMakeFloat2(zSqrt2F, zSqrt2F); \
//      const float2 p1 = zMakeFloat2(0.0f, 0.0f); \
//      results2[i++] = distance2(p0, p1); \
//    } \
//    i = 0; \
//    { \
//      const float3 p0 = zMakeFloat3(5.0f, 3.0f, 0.0f); \
//      const float3 p1 = zMakeFloat3(2.0f, -2.0f, zSqrt2F); \
//      results3[i++] = distance3(p0, p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(1.0f, 0.0f, -5.0f); \
//      const float3 p1 = zMakeFloat3(-3.0f, 2.0f, -1.0f); \
//      results3[i++] = distance3(p0, p1); \
//    } \
//    { \
//      const float3 p0 = zMakeFloat3(2.0f, 3.0f, 5.0f); \
//      const float3 p1 = zMakeFloat3(2.0f, 0.0f, 9.0f); \
//      results3[i++] = distance3(p0, p1); \
//    } \
//    i = 0; \
//    { \
//      const float4 p0 = zMakeFloat4(5.0f, 3.0f, 0.0f, k); \
//      const float4 p1 = zMakeFloat4(2.0f, -2.0f, zSqrt2F, k); \
//      results3f4[i++] = distance3f4(p0, p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(1.0f, 0.0f, -5.0f, k); \
//      const float4 p1 = zMakeFloat4(-3.0f, 2.0f, -1.0f, k); \
//      results3f4[i++] = distance3f4(p0, p1); \
//    } \
//    { \
//      const float4 p0 = zMakeFloat4(2.0f, 3.0f, 5.0f, k); \
//      const float4 p1 = zMakeFloat4(2.0f, 0.0f, 9.0f, k); \
//      results3f4[i++] = distance3f4(p0, p1); \
//    } \
//    i = 0; \
//    { \
//      const float4 p0 = zMakeFloat4(2.0f, 3.0f, 4.0f, 2.0f); \
//      const float4 p1 = zMakeFloat4(1.0f, -2.0f, 1.0f, 3.0f); \
//      results4[i++] = distance4(p0, p1); \
//    } \
//  } \
//
//__kernel void testDistance(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_DISTANCE(distance, distance, distance, distance, 0.0f,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzDistanceImpl(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_DISTANCE(zDistance2Impl, zDistance3Impl, zDistance3F4Impl, zDistance4Impl, 1.0f,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzDistance(__global float* results1,
//    __global float* results2,
//    __global float* results3,
//    __global float* results4)
//{
//  ZINVUL_TEST_DISTANCE(zDistance2, zDistance3, zDistance3F4, zDistance4, 1.0f,
//      results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_NORMALIZE(norm2, norm3, norm3f4, norm4, k, results2, results3, results3f4, results4) \
//  const uint32b index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    uint i = 0; \
//    { \
//      const float2 p = zMakeFloat2(zSqrt2F, zSqrt3F); \
//      results2[i++] = p; \
//      results2[i++] = norm2(p); \
//    } \
//    i = 0; \
//    { \
//      const float3 p = zMakeFloat3(2.0f, 3.0f, 5.0f); \
//      results3[i++] = p; \
//      results3[i++] = norm3(p); \
//    } \
//    i = 0; \
//    { \
//      const float4 p = zMakeFloat4(2.0f, 3.0f, 5.0f, k); \
//      results3f4[i++] = p; \
//      results3f4[i++] = norm3f4(p); \
//    } \
//    i = 0; \
//    { \
//      const float4 p = zMakeFloat4(2.0f, 3.0f, 4.0f, 2.0f); \
//      results4[i++] = p; \
//      results4[i++] = norm4(p); \
//    } \
//  } \
//
//__kernel void testNormalize(__global float2* results1,
//    __global float3* results2,
//    __global float4* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_NORMALIZE(normalize, normalize, normalize, normalize, 0.0f,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzNormalizeImpl(__global float2* results1,
//    __global float3* results2,
//    __global float4* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_NORMALIZE(zNormalize2Impl, zNormalize3Impl, zNormalize3F4Impl, zNormalize4Impl, 1.0f,
//      results1, results2, results3, results4);
//}
//
//__kernel void testzNormalize(__global float2* results1,
//    __global float3* results2,
//    __global float4* results3,
//    __global float4* results4)
//{
//  ZINVUL_TEST_NORMALIZE(zNormalize2, zNormalize3, zNormalize3F4, zNormalize4, 1.0f,
//      results1, results2, results3, results4);
//}

#endif /* ZINVUL_MATH_TEST_MATH_CL */
