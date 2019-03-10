/*!
  \file math_function.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_TEST_MATH_FUNCTION_CL
#define ZINVUL_MATH_TEST_MATH_FUNCTION_CL

// Zinvul
#include "zinvul/cl/types.cl"
#include "zinvul/cl/math.cl"
#include "zinvul/cl/utility.cl"

//! Make a normal value
float makeNormal(const float x)
{
  const int bias = zExponentBiasF;
  const float k = 0.98f * (2.0f * zAbsF(x) - 1.0f);
  const int e = zClamp((int)(k * (float)bias), -bias + 1, bias);
  const float s = (x < 0.0f) ? -1.0f : 1.0f;
  const float m = 0.499f * zAbsF(x) + 0.5f;
  float f = zLdexp(m, e);
  f = (x == 0.0f) ? 0.0f : s * zClampF(f, zFloatMin, zFloatMax);
  return f;
}

//! Make a subnormal value
float makeSubnormal(const float x)
{
  const float s = (x < 0.0f) ? -1.0f : 1.0f;
  float f = (x * x * x * x) * zFloatDenormMax;
  f = s * zClampF(f, zFloatDenormMin, zFloatDenormMax);
  return f;
}

/*!
  */
__kernel void testMakeNormal(__global float* results1, const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    {
      const float x = (float)(2 * index) / (float)resolution - 1.0f;
      results1[4 * index] = x;
      const float z = makeNormal(x);
      results1[4 * index + 1] = z;
    }
    {
      const float x = (float)(index + 1) / (float)resolution;
      results1[4 * index + 2] = x;
      const float z = makeNormal(x);
      results1[4 * index + 3] = z;
    }
  }
}

/*!
  */
#define ZINVUL_TEST_FRLDEXP(frexp1, ldexp1, frexp2, ldexp2, frexp3, ldexp3, frexp4, ldexp4, results1, results_exp1, results2, results_exp2, results3, results_exp3, results4, results_exp4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    const float x = (float)(2 * index) / (float)resolution - 1.0f; \
    { \
      const float z = makeNormal(x); \
      results1[3 * index] = z; \
      int32b e = 0; \
      const float m = frexp1(z, &e); \
      results1[3 * index + 1] = m; \
      results_exp1[index] = e; \
      const float f = ldexp1(m, e); \
      results1[3 * index + 2] = f; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[3 * i] = z; \
        int32b e = 0; \
        const float m = frexp1(z, &e); \
        results1[3 * i + 1] = m; \
        results_exp1[i] = e; \
        const float f = ldexp1(m, e); \
        results1[3 * i + 2] = f; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[3 * i] = z; \
        int32b e = 0; \
        const float m = frexp1(z, &e); \
        results1[3 * i + 1] = m; \
        results_exp1[i] = e; \
        const float f = ldexp1(m, e); \
        results1[3 * i + 2] = f; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x)); \
      results2[3 * index] = z; \
      int2 e = zMakeInt2(0, 0); \
      const float2 m = frexp2(z, &e); \
      results2[3 * index + 1] = m; \
      results_exp2[index] = e; \
      const float2 f = ldexp2(m, e); \
      results2[3 * index + 2] = f; \
    } \
    { \
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x)); \
      results3[3 * index] = z; \
      int3 e = zMakeInt3(0, 0, 0); \
      const float3 m = frexp3(z, &e); \
      results3[3 * index + 1] = m; \
      results_exp3[index] = e; \
      const float3 f = ldexp3(m, e); \
      results3[3 * index + 2] = f; \
    } \
    { \
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x)); \
      results4[3 * index] = z; \
      int4 e = zMakeInt4(0, 0, 0, 0); \
      const float4 m = frexp4(z, &e); \
      results4[3 * index + 1] = m; \
      results_exp4[index] = e; \
      const float4 f = ldexp4(m, e); \
      results4[3 * index + 2] = f; \
    } \
  }

/*!
  */
__kernel void testFrLdexp(global float* results1,
    global int32b* results_exp1,
    global float2* results2,
    global int2* results_exp2,
    global float3* results3,
    global int3* results_exp3,
    global float4* results4,
    global int4* results_exp4,
    const uint32b resolution)
{
  ZINVUL_TEST_FRLDEXP(frexp, ldexp, frexp, ldexp, frexp, ldexp, frexp, ldexp,
      results1, results_exp1, results2, results_exp2, results3, results_exp3, results4, results_exp4, resolution);
}

/*!
  */
__kernel void testzFrLdexpImpl(global float* results1,
    global int32b* results_exp1,
    global float2* results2,
    global int2* results_exp2,
    global float3* results3,
    global int3* results_exp3,
    global float4* results4,
    global int4* results_exp4,
    const uint32b resolution)
{
  ZINVUL_TEST_FRLDEXP(zFrexpImpl, zLdexpImpl, zFrexp2Impl, zLdexp2Impl, zFrexp3Impl, zLdexp3Impl, zFrexp4Impl, zLdexp4Impl,
      results1, results_exp1, results2, results_exp2, results3, results_exp3, results4, results_exp4, resolution);
}

/*!
  */
__kernel void testzFrLdexp(global float* results1,
    global int32b* results_exp1,
    global float2* results2,
    global int2* results_exp2,
    global float3* results3,
    global int3* results_exp3,
    global float4* results4,
    global int4* results_exp4,
    const uint32b resolution)
{
  ZINVUL_TEST_FRLDEXP(zFrexp, zLdexp, zFrexp2, zLdexp2, zFrexp3, zLdexp3, zFrexp4, zLdexp4,
      results1, results_exp1, results2, results_exp2, results3, results_exp3, results4, results_exp4, resolution);
}

/*!
  */
#define ZINVUL_TEST_EXP(expo1, expo2, expo3, expo4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    float x = (float)(2 * index) / (float)resolution - 1.0f; \
    x = x * 100.0f; \
    x = (x < 0.0f) ? -zLog2(zAbsF(x)) : zLog2(zAbsF(x)); \
    { \
      const float z = x; \
      results1[2 * index] = z; \
      const float y = expo1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = expo1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = -INFINITY; \
        results1[2 * i] = z; \
        const float y = expo1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 2; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = expo1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(x, 0.85f * x); \
      results2[2 * index] = z; \
      const float2 y = expo2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x); \
      results3[2 * index] = z; \
      const float3 y = expo3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x); \
      results4[2 * index] = z; \
      const float4 y = expo4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testExp(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_EXP(exp, exp, exp, exp,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzExpImpl(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_EXP(zExpImpl, zExp2Impl, zExp3Impl, zExp4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzExp(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_EXP(zExp, zExp2, zExp3, zExp4,
      results1, results2, results3, results4, resolution);
}

/*!
  */
#define ZINVUL_TEST_LOG(loga1, loga2, loga3, loga4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    const float x = (float)(index + 1) / (float)resolution; \
    { \
      const float z = makeNormal(x); \
      results1[2 * index] = z; \
      const float y = loga1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = loga1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = loga1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x)); \
      results2[2 * index] = z; \
      const float2 y = loga2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x)); \
      results3[2 * index] = z; \
      const float3 y = loga3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x)); \
      results4[2 * index] = z; \
      const float4 y = loga4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testLog(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_LOG(log, log, log, log,
    results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzLogImpl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_LOG(zLogImpl, zLogF2Impl, zLogF3Impl, zLogF4Impl,
    results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzLog(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_LOG(zLog, zLogF2, zLogF3, zLogF4,
    results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testLog2(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_LOG(log2, log2, log2, log2,
    results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzLog2Impl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_LOG(zLog2Impl, zLog2F2Impl, zLog2F3Impl, zLog2F4Impl,
    results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzLog2(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_LOG(zLog2, zLog2F2, zLog2F3, zLog2F4,
    results1, results2, results3, results4, resolution);
}

/*!
  */
#define ZINVUL_TEST_POW(pow1, pow2, pow3, pow4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    float x = (float)(2 * index) / (float)resolution - 1.0f; \
    x = x * 100.0f; \
    x = (x < 0.0f) ? -zLog2(zAbsF(x)) : zLog2(zAbsF(x)); \
    { \
      const float z = x; \
      results1[9 * index] = z; \
      { \
        const float y = pow1(0.0f, z); \
        results1[9 * index + 1] = y; \
      } \
      { \
        const float y = pow1(1.0f, z); \
        results1[9 * index + 2] = y; \
      } \
      { \
        const float y = pow1(-1.0f, z); \
        results1[9 * index + 3] = y; \
      } \
      { \
        const float y = pow1(2.0f, z); \
        results1[9 * index + 4] = y; \
      } \
      { \
        const float y = pow1(0.5f, z); \
        results1[9 * index + 5] = y; \
      } \
      { \
        const float y = pow1(INFINITY, z); \
        results1[9 * index + 6] = y; \
      } \
      { \
        const float y = pow1(-INFINITY, z); \
        results1[9 * index + 7] = y; \
      } \
      { \
        const float y = pow1(NAN, z); \
        results1[9 * index + 8] = y; \
      } \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = -INFINITY; \
        results1[9 * i] = z; \
        { \
          const float y = pow1(0.0f, z); \
          results1[9 * i + 1] = y; \
        } \
        { \
          const float y = pow1(1.0f, z); \
          results1[9 * i + 2] = y; \
        } \
        { \
          const float y = pow1(-1.0f, z); \
          results1[9 * i + 3] = y; \
        } \
        { \
          const float y = pow1(2.0f, z); \
          results1[9 * i + 4] = y; \
        } \
        { \
          const float y = pow1(0.5f, z); \
          results1[9 * i + 5] = y; \
        } \
        { \
          const float y = pow1(INFINITY, z); \
          results1[9 * i + 6] = y; \
        } \
        { \
          const float y = pow1(-INFINITY, z); \
          results1[9 * i + 7] = y; \
        } \
        { \
          const float y = pow1(NAN, z); \
          results1[9 * i + 8] = y; \
        } \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = INFINITY; \
        results1[9 * i] = z; \
        { \
          const float y = pow1(0.0f, z); \
          results1[9 * i + 1] = y; \
        } \
        { \
          const float y = pow1(1.0f, z); \
          results1[9 * i + 2] = y; \
        } \
        { \
          const float y = pow1(-1.0f, z); \
          results1[9 * i + 3] = y; \
        } \
        { \
          const float y = pow1(2.0f, z); \
          results1[9 * i + 4] = y; \
        } \
        { \
          const float y = pow1(0.5f, z); \
          results1[9 * i + 5] = y; \
        } \
        { \
          const float y = pow1(INFINITY, z); \
          results1[9 * i + 6] = y; \
        } \
        { \
          const float y = pow1(-INFINITY, z); \
          results1[9 * i + 7] = y; \
        } \
        { \
          const float y = pow1(NAN, z); \
          results1[9 * i + 8] = y; \
        } \
      } \
      { \
        const uint i = resolution + 2; \
        const float z = NAN; \
        results1[9 * i] = z; \
        { \
          const float y = pow1(0.0f, z); \
          results1[9 * i + 1] = y; \
        } \
        { \
          const float y = pow1(1.0f, z); \
          results1[9 * i + 2] = y; \
        } \
        { \
          const float y = pow1(-1.0f, z); \
          results1[9 * i + 3] = y; \
        } \
        { \
          const float y = pow1(2.0f, z); \
          results1[9 * i + 4] = y; \
        } \
        { \
          const float y = pow1(0.5f, z); \
          results1[9 * i + 5] = y; \
        } \
        { \
          const float y = pow1(INFINITY, z); \
          results1[9 * i + 6] = y; \
        } \
        { \
          const float y = pow1(-INFINITY, z); \
          results1[9 * i + 7] = y; \
        } \
        { \
          const float y = pow1(NAN, z); \
          results1[9 * i + 8] = y; \
        } \
      } \
    } \
    { \
      const float2 b = zMakeFloat2(2.0f, 2.0f); \
      const float2 z = zMakeFloat2(x, 0.85f * x); \
      results2[2 * index] = z; \
      const float2 y = pow2(b, z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 b = zMakeFloat3(2.0f, 2.0f, 2.0f); \
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x); \
      results3[2 * index] = z; \
      const float3 y = pow3(b, z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 b = zMakeFloat4(2.0f, 2.0f, 2.0f, 2.0f); \
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x); \
      results4[2 * index] = z; \
      const float4 y = pow4(b, z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testPow(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_POW(pow, pow, pow, pow,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzPowImpl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_POW(zPowImpl, zPow2Impl, zPow3Impl, zPow4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzPow(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_POW(zPow, zPow2, zPow3, zPow4,
      results1, results2, results3, results4, resolution);
}

/*!
  */
kernel void testzPown(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    //! \todo conversion from uint to int doesn't work on macOS
    // const int32b x = (int32b)index - (int32b)resolution / 2;
    const int32b x = (int32b)((float)index - (float)(resolution >> 1));
    // Scalar
    {
      const int32b z = x;
      result1[9 * index] = (float)z;
      {
        const float y = zPown(0.0f, z);
        result1[9 * index + 1] = y;
      }
      {
        const float y = zPown(1.0f, z);
        result1[9 * index + 2] = y;
      }
      {
        const float y = zPown(-1.0f, z);
        result1[9 * index + 3] = y;
      }
      {
        const float y = zPown(2.0f, z);
        result1[9 * index + 4] = y;
      }
      {
        const float y = zPown(0.5f, z);
        result1[9 * index + 5] = y;
      }
      {
        const float y = zPown(INFINITY, z);
        result1[9 * index + 6] = y;
      }
      {
        const float y = zPown(-INFINITY, z);
        result1[9 * index + 7] = y;
      }
      {
        const float y = zPown(NAN, z);
        result1[9 * index + 8] = y;
      }
    }
    // Vector2
    {
      const float2 b = zMakeFloat2(2.0f, 2.0f);
      const int2 z = zBroadcast2(x);
      result2[2 * index] = zI2ToF2(z);
      const float2 y = zPown2(b, z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 b = zMakeFloat3(2.0f, 2.0f, 2.0f);
      const int3 z = zBroadcast3(x);
      result3[2 * index] = zI3ToF3(z);
      const float3 y = zPown3(b, z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 b = zMakeFloat4(2.0f, 2.0f, 2.0f, 2.0f);
      const int4 z = zBroadcast4(x);
      result4[2 * index] = zI4ToF4(z);
      const float4 y = zPown4(b, z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
#define ZINVUL_TEST_RSQRT(rsqrt1, rsqrt2, rsqrt3, rsqrt4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    const float x = (float)(index + 1) / (float)resolution; \
    { \
      const float z = makeNormal(x); \
      results1[2 * index] = z; \
      const float y = rsqrt1(z); \
      results1[2 * index + 1] = y; \
    } \
    { \
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x)); \
      results2[2 * index] = z; \
      const float2 y = rsqrt2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x)); \
      results3[2 * index] = z; \
      const float3 y = rsqrt3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x)); \
      results4[2 * index] = z; \
      const float4 y = rsqrt4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testRsqrt(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_RSQRT(rsqrt, rsqrt, rsqrt, rsqrt,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzRsqrtImpl(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_RSQRT(zRsqrtImpl, zRsqrt2Impl, zRsqrt3Impl, zRsqrt4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzRsqrt(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_RSQRT(zRsqrt, zRsqrt2, zRsqrt3, zRsqrt4,
      results1, results2, results3, results4, resolution);
}

/*!
  */
#define ZINVUL_TEST_SQRT(sqrt1, sqrt2, sqrt3, sqrt4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    const float x = (float)(index + 1) / (float)resolution; \
    { \
      const float z = makeNormal(x); \
      results1[2 * index] = z; \
      const float y = sqrt1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = sqrt1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = sqrt1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x)); \
      results2[2 * index] = z; \
      const float2 y = sqrt2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x)); \
      results3[2 * index] = z; \
      const float3 y = sqrt3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x)); \
      results4[2 * index] = z; \
      const float4 y = sqrt4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testSqrt(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_SQRT(sqrt, sqrt, sqrt, sqrt,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzSqrtImpl(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_SQRT(zSqrtImpl, zSqrt2Impl, zSqrt3Impl, zSqrt4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzSqrt(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_SQRT(zSqrt, zSqrt2, zSqrt3, zSqrt4,
      results1, results2, results3, results4, resolution);
}

#define ZINVUL_TEST_CBRT(cbrt1, cbrt2, cbrt3, cbrt4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    const float x = (float)(2 * index) / (float)resolution - 1.0f; \
    { \
      const float z = makeNormal(x); \
      results1[2 * index] = z; \
      const float y = cbrt1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = cbrt1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = cbrt1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x)); \
      results2[2 * index] = z; \
      const float2 y = cbrt2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x)); \
      results3[2 * index] = z; \
      const float3 y = cbrt3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x)); \
      results4[2 * index] = z; \
      const float4 y = cbrt4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testzCbrt(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_CBRT(zCbrt, zCbrt2, zCbrt3, zCbrt4,
      results1, results2, results3, results4, resolution);
}

#define ZINVUL_TEST_SIN(sin1, sin2, sin3, sin4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); \
    x = (4.0f * zPiF) * x; \
    { \
      const float z = x; \
      results1[2 * index] = z; \
      const float y = sin1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = sin1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = sin1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(x, 0.85f * x); \
      results2[2 * index] = z; \
      const float2 y = sin2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x); \
      results3[2 * index] = z; \
      const float3 y = sin3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x); \
      results4[2 * index] = z; \
      const float4 y = sin4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testSin(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_SIN(sin, sin, sin, sin,
    results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzSinImpl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_SIN(zSinImpl, zSin2Impl, zSin3Impl, zSin4Impl,
    results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzSin(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_SIN(zSin, zSin2, zSin3, zSin4,
    results1, results2, results3, results4, resolution);
}

#define ZINVUL_TEST_COS(cos1, cos2, cos3, cos4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); \
    x = (4.0f * zPiF) * x; \
    { \
      const float z = x; \
      results1[2 * index] = z; \
      const float y = cos1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = cos1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = cos1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(x, 0.85f * x); \
      results2[2 * index] = z; \
      const float2 y = cos2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x); \
      results3[2 * index] = z; \
      const float3 y = cos3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x); \
      results4[2 * index] = z; \
      const float4 y = cos4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testCos(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_COS(cos, cos, cos, cos,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzCosImpl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_COS(zCosImpl, zCos2Impl, zCos3Impl, zCos4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzCos(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_COS(zCos, zCos2, zCos3, zCos4,
      results1, results2, results3, results4, resolution);
}

#define ZINVUL_TEST_SINCOS(sincos1, sincos2, sincos3, sincos4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); \
    x = (4.0f * zPiF) * x; \
    { \
      const float z = x; \
      results1[3 * index] = z; \
      float c = 0.0f; \
      const float y = sincos1(z, &c); \
      results1[3 * index + 1] = y; \
      results1[3 * index + 2] = c; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[3 * i] = z; \
        float c = 0.0f; \
        const float y = sincos1(z, &c); \
        results1[3 * i + 1] = y; \
        results1[3 * i + 2] = c; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[3 * i] = z; \
        float c = 0.0f; \
        const float y = sincos1(z, &c); \
        results1[3 * i + 1] = y; \
        results1[3 * i + 2] = c; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(x, 0.85f * x); \
      results2[3 * index] = z; \
      float2 c = zMakeFloat2(0.0f, 0.0f); \
      const float2 y = sincos2(z, &c); \
      results2[3 * index + 1] = y; \
      results2[3 * index + 2] = c; \
    } \
    { \
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x); \
      results3[3 * index] = z; \
      float3 c = zMakeFloat3(0.0f, 0.0f, 0.0f); \
      const float3 y = sincos3(z, &c); \
      results3[3 * index + 1] = y; \
      results3[3 * index + 2] = c; \
    } \
    { \
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x); \
      results4[3 * index] = z; \
      float4 c = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f); \
      const float4 y = sincos4(z, &c); \
      results4[3 * index + 1] = y; \
      results4[3 * index + 2] = c; \
    } \
  }

/*!
  */
__kernel void testzSincosImpl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_SINCOS(zSincosImpl, zSincos2Impl, zSincos3Impl, zSincos4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzSincos(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_SINCOS(zSincos, zSincos2, zSincos3, zSincos4,
      results1, results2, results3, results4, resolution);
}

#define ZINVUL_TEST_TAN(tan1, tan2, tan3, tan4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); \
    x = (4.0f * zPiF) * x; \
    { \
      const float z = x; \
      results1[2 * index] = z; \
      const float y = tan1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = tan1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = tan1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(x, 0.85f * x); \
      results2[2 * index] = z; \
      const float2 y = tan2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x); \
      results3[2 * index] = z; \
      const float3 y = tan3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x); \
      results4[2 * index] = z; \
      const float4 y = tan4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testTan(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_TAN(tan, tan, tan, tan,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzTanImpl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_TAN(zTanImpl, zTan2Impl, zTan3Impl, zTan4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzTan(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_TAN(zTan, zTan2, zTan3, zTan4,
      results1, results2, results3, results4, resolution);
}

#define ZINVUL_TEST_ASIN(asin1, asin2, asin3, asin4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); \
    { \
      const float z = x; \
      results1[2 * index] = z; \
      const float y = asin1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = asin1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = asin1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(x, 0.85f * x); \
      results2[2 * index] = z; \
      const float2 y = asin2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x); \
      results3[2 * index] = z; \
      const float3 y = asin3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x); \
      results4[2 * index] = z; \
      const float4 y = asin4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testAsin(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ASIN(asin, asin, asin, asin,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzAsinImpl(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ASIN(zAsinImpl, zAsin2Impl, zAsin3Impl, zAsin4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzAsin(global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ASIN(zAsin, zAsin2, zAsin3, zAsin4,
      results1, results2, results3, results4, resolution);
}

#define ZINVUL_TEST_ACOS(acos1, acos2, acos3, acos4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); \
    { \
      const float z = x; \
      results1[2 * index] = z; \
      const float y = acos1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = acos1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = acos1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(x, 0.85f * x); \
      results2[2 * index] = z; \
      const float2 y = acos2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x); \
      results3[2 * index] = z; \
      const float3 y = acos3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x); \
      results4[2 * index] = z; \
      const float4 y = acos4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testAcos(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ACOS(acos, acos, acos, acos,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzAcosImpl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ACOS(zAcosImpl, zAcos2Impl, zAcos3Impl, zAcos4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzAcos(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ACOS(zAcos, zAcos2, zAcos3, zAcos4,
      results1, results2, results3, results4, resolution);
}

#define ZINVUL_TEST_ATAN(arctan1, arctan2, arctan3, arctan4, results1, results2, results3, results4, resolution) \
  const uint32b index = zGetGlobalIdX(); \
  if (index < resolution) { \
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); \
    { \
      const float z = makeNormal(x); \
      results1[2 * index] = z; \
      const float y = arctan1(z); \
      results1[2 * index + 1] = y; \
    } \
    if (index == 0) { \
      { \
        const uint i = resolution; \
        const float z = -INFINITY; \
        results1[2 * i] = z; \
        const float y = arctan1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 1; \
        const float z = INFINITY; \
        results1[2 * i] = z; \
        const float y = arctan1(z); \
        results1[2 * i + 1] = y; \
      } \
      { \
        const uint i = resolution + 2; \
        const float z = NAN; \
        results1[2 * i] = z; \
        const float y = arctan1(z); \
        results1[2 * i + 1] = y; \
      } \
    } \
    { \
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x)); \
      results2[2 * index] = z; \
      const float2 y = arctan2(z); \
      results2[2 * index + 1] = y; \
    } \
    { \
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x)); \
      results3[2 * index] = z; \
      const float3 y = arctan3(z); \
      results3[2 * index + 1] = y; \
    } \
    { \
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x)); \
      results4[2 * index] = z; \
      const float4 y = arctan4(z); \
      results4[2 * index + 1] = y; \
    } \
  }

/*!
  */
__kernel void testAtan(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ATAN(atan, atan, atan, atan,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzAtanImpl(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ATAN(zAtanImpl, zAtan2Impl, zAtan3Impl, zAtan4Impl,
      results1, results2, results3, results4, resolution);
}

/*!
  */
__kernel void testzAtan(
    global float* results1,
    global float2* results2,
    global float3* results3,
    global float4* results4,
    const uint32b resolution)
{
  ZINVUL_TEST_ATAN(zAtan, zAtan2, zAtan3, zAtan4,
      results1, results2, results3, results4, resolution);
}

#endif /* ZINVUL_MATH_TEST_MATH_FUNCTION_CL */
