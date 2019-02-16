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
kernel void testFrLdexp(
    global float* result1,
    global int32b* result_exp1,
    global float2* result2,
    global int2* result_exp2,
    global float3* result3,
    global int3* result_exp3,
    global float4* result4,
    global int4* result_exp4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(2 * index) / (float)resolution - 1.0f; // [-1, 1)
    // Scalar
    {
      const float z = makeNormal(x);
      result1[3 * index] = z;
      int32b e = 0;
      const float m = frexp(z, &e);
      result1[3 * index + 1] = m;
      result_exp1[index] = e;
      const float f = ldexp(m, e);
      result1[3 * index + 2] = f;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[3 * i] = z;
        int32b e = 0;
        const float m = frexp(z, &e);
        result1[3 * i + 1] = m;
        result_exp1[i] = e;
        const float f = ldexp(m, e);
        result1[3 * i + 2] = f;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[3 * i] = z;
        int32b e = 0;
        const float m = frexp(z, &e);
        result1[3 * i + 1] = m;
        result_exp1[i] = e;
        const float f = ldexp(m, e);
        result1[3 * i + 2] = f;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[3 * index] = z;
      int2 e = zMakeInt2(0, 0);
      const float2 m = frexp(z, &e);
      result2[3 * index + 1] = m;
      result_exp2[index] = e;
      const float2 f = ldexp(m, e);
      result2[3 * index + 2] = f;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[3 * index] = z;
      int3 e = zMakeInt3(0, 0, 0);
      const float3 m = frexp(z, &e);
      result3[3 * index + 1] = m;
      result_exp3[index] = e;
      const float3 f = ldexp(m, e);
      result3[3 * index + 2] = f;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[3 * index] = z;
      int4 e = zMakeInt4(0, 0, 0, 0);
      const float4 m = frexp(z, &e);
      result4[3 * index + 1] = m;
      result_exp4[index] = e;
      const float4 f = ldexp(m, e);
      result4[3 * index + 2] = f;
    }
  }
}

/*!
  */
kernel void testZFrLdexp(
    global float* result1,
    global int32b* result_exp1,
    global float2* result2,
    global int2* result_exp2,
    global float3* result3,
    global int3* result_exp3,
    global float4* result4,
    global int4* result_exp4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(2 * index) / (float)resolution - 1.0f; // [-1, 1)
    // Scalar
    {
      const float z = makeNormal(x);
      result1[3 * index] = z;
      int32b e = 0;
      const float m = zFrexp(z, &e);
      result1[3 * index + 1] = m;
      result_exp1[index] = e;
      const float f = zLdexp(m, e);
      result1[3 * index + 2] = f;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[3 * i] = z;
        int32b e = 0;
        const float m = zFrexp(z, &e);
        result1[3 * i + 1] = m;
        result_exp1[i] = e;
        const float f = zLdexp(m, e);
        result1[3 * i + 2] = f;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[3 * i] = z;
        int32b e = 0;
        const float m = zFrexp(z, &e);
        result1[3 * i + 1] = m;
        result_exp1[i] = e;
        const float f = zLdexp(m, e);
        result1[3 * i + 2] = f;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[3 * index] = z;
      int2 e = zMakeInt2(0, 0);
      const float2 m = zFrexp2(z, &e);
      result2[3 * index + 1] = m;
      result_exp2[index] = e;
      const float2 f = zLdexp2(m, e);
      result2[3 * index + 2] = f;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[3 * index] = z;
      int3 e = zMakeInt3(0, 0, 0);
      const float3 m = zFrexp3(z, &e);
      result3[3 * index + 1] = m;
      result_exp3[index] = e;
      const float3 f = zLdexp3(m, e);
      result3[3 * index + 2] = f;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[3 * index] = z;
      int4 e = zMakeInt4(0, 0, 0, 0);
      const float4 m = zFrexp4(z, &e);
      result4[3 * index + 1] = m;
      result_exp4[index] = e;
      const float4 f = zLdexp4(m, e);
      result4[3 * index + 2] = f;
    }
  }
}

/*!
  */
kernel void testSqrt(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = sqrt(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = sqrt(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = sqrt(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = sqrt(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = sqrt(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = sqrt(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZpow(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = (float)(2 * index) / (float)resolution - 1.0f; // [-1, 1)
    x = x * 100.0f;
    x = (x < 0.0f) ? -zLog2(zAbsF(x)) : zLog2(zAbsF(x));
    // Scalar
    {
      const float z = x;
      result1[9 * index] = z;
      {
        const float y = zPow(0.0f, z);
        result1[9 * index + 1] = y;
      }
      {
        const float y = zPow(1.0f, z);
        result1[9 * index + 2] = y;
      }
      {
        const float y = zPow(-1.0f, z);
        result1[9 * index + 3] = y;
      }
      {
        const float y = zPow(2.0f, z);
        result1[9 * index + 4] = y;
      }
      {
        const float y = zPow(0.5f, z);
        result1[9 * index + 5] = y;
      }
      {
        const float y = zPow(INFINITY, z);
        result1[9 * index + 6] = y;
      }
      {
        const float y = zPow(-INFINITY, z);
        result1[9 * index + 7] = y;
      }
      {
        const float y = zPow(NAN, z);
        result1[9 * index + 8] = y;
      }
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = -INFINITY;
        result1[9 * i] = z;
        {
          const float y = zPow(0.0f, z);
          result1[9 * i + 1] = y;
        }
        {
          const float y = zPow(1.0f, z);
          result1[9 * i + 2] = y;
        }
        {
          const float y = zPow(-1.0f, z);
          result1[9 * i + 3] = y;
        }
        {
          const float y = zPow(2.0f, z);
          result1[9 * i + 4] = y;
        }
        {
          const float y = zPow(0.5f, z);
          result1[9 * i + 5] = y;
        }
        {
          const float y = zPow(INFINITY, z);
          result1[9 * i + 6] = y;
        }
        {
          const float y = zPow(-INFINITY, z);
          result1[9 * i + 7] = y;
        }
        {
          const float y = zPow(NAN, z);
          result1[9 * i + 8] = y;
        }
      }
      {
        const uint i = resolution + 1;
        const float z = INFINITY;
        result1[9 * i] = z;
        {
          const float y = zPow(0.0f, z);
          result1[9 * i + 1] = y;
        }
        {
          const float y = zPow(1.0f, z);
          result1[9 * i + 2] = y;
        }
        {
          const float y = zPow(-1.0f, z);
          result1[9 * i + 3] = y;
        }
        {
          const float y = zPow(2.0f, z);
          result1[9 * i + 4] = y;
        }
        {
          const float y = zPow(0.5f, z);
          result1[9 * i + 5] = y;
        }
        {
          const float y = zPow(INFINITY, z);
          result1[9 * i + 6] = y;
        }
        {
          const float y = zPow(-INFINITY, z);
          result1[9 * i + 7] = y;
        }
        {
          const float y = zPow(NAN, z);
          result1[9 * i + 8] = y;
        }
      }
      {
        const uint i = resolution + 2;
        const float z = NAN;
        result1[9 * i] = z;
        {
          const float y = zPow(0.0f, z);
          result1[9 * i + 1] = y;
        }
        {
          const float y = zPow(1.0f, z);
          result1[9 * i + 2] = y;
        }
        {
          const float y = zPow(-1.0f, z);
          result1[9 * i + 3] = y;
        }
        {
          const float y = zPow(2.0f, z);
          result1[9 * i + 4] = y;
        }
        {
          const float y = zPow(0.5f, z);
          result1[9 * i + 5] = y;
        }
        {
          const float y = zPow(INFINITY, z);
          result1[9 * i + 6] = y;
        }
        {
          const float y = zPow(-INFINITY, z);
          result1[9 * i + 7] = y;
        }
        {
          const float y = zPow(NAN, z);
          result1[9 * i + 8] = y;
        }
      }
    }
    // Vector2
    {
      const float2 b = zMakeFloat2(2.0f, 2.0f);
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = zPow2(b, z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 b = zMakeFloat3(2.0f, 2.0f, 2.0f);
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = zPow3(b, z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 b = zMakeFloat4(2.0f, 2.0f, 2.0f, 2.0f);
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = zPow4(b, z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testPow(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = (float)(2 * index) / (float)resolution - 1.0f; // [-1, 1)
    x = x * 100.0f;
    x = (x < 0.0f) ? -zLog2(zAbsF(x)) : zLog2(zAbsF(x));
    // Scalar
    {
      const float z = x;
      result1[9 * index] = z;
      {
        const float y = pow(0.0f, z);
        result1[9 * index + 1] = y;
      }
      {
        const float y = pow(1.0f, z);
        result1[9 * index + 2] = y;
      }
      {
        const float y = pow(-1.0f, z);
        result1[9 * index + 3] = y;
      }
      {
        const float y = pow(2.0f, z);
        result1[9 * index + 4] = y;
      }
      {
        const float y = pow(0.5f, z);
        result1[9 * index + 5] = y;
      }
      {
        const float y = pow(INFINITY, z);
        result1[9 * index + 6] = y;
      }
      {
        const float y = pow(-INFINITY, z);
        result1[9 * index + 7] = y;
      }
      {
        const float y = pow(NAN, z);
        result1[9 * index + 8] = y;
      }
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = -INFINITY;
        result1[9 * i] = z;
        {
          const float y = pow(0.0f, z);
          result1[9 * i + 1] = y;
        }
        {
          const float y = pow(1.0f, z);
          result1[9 * i + 2] = y;
        }
        {
          const float y = pow(-1.0f, z);
          result1[9 * i + 3] = y;
        }
        {
          const float y = pow(2.0f, z);
          result1[9 * i + 4] = y;
        }
        {
          const float y = pow(0.5f, z);
          result1[9 * i + 5] = y;
        }
        {
          const float y = pow(INFINITY, z);
          result1[9 * i + 6] = y;
        }
        {
          const float y = pow(-INFINITY, z);
          result1[9 * i + 7] = y;
        }
        {
          const float y = pow(NAN, z);
          result1[9 * i + 8] = y;
        }
      }
      {
        const uint i = resolution + 1;
        const float z = INFINITY;
        result1[9 * i] = z;
        {
          const float y = pow(0.0f, z);
          result1[9 * i + 1] = y;
        }
        {
          const float y = pow(1.0f, z);
          result1[9 * i + 2] = y;
        }
        {
          const float y = pow(-1.0f, z);
          result1[9 * i + 3] = y;
        }
        {
          const float y = pow(2.0f, z);
          result1[9 * i + 4] = y;
        }
        {
          const float y = pow(0.5f, z);
          result1[9 * i + 5] = y;
        }
        {
          const float y = pow(INFINITY, z);
          result1[9 * i + 6] = y;
        }
        {
          const float y = pow(-INFINITY, z);
          result1[9 * i + 7] = y;
        }
        {
          const float y = pow(NAN, z);
          result1[9 * i + 8] = y;
        }
      }
      {
        const uint i = resolution + 2;
        const float z = NAN;
        result1[9 * i] = z;
        {
          const float y = pow(0.0f, z);
          result1[9 * i + 1] = y;
        }
        {
          const float y = pow(1.0f, z);
          result1[9 * i + 2] = y;
        }
        {
          const float y = pow(-1.0f, z);
          result1[9 * i + 3] = y;
        }
        {
          const float y = pow(2.0f, z);
          result1[9 * i + 4] = y;
        }
        {
          const float y = pow(0.5f, z);
          result1[9 * i + 5] = y;
        }
        {
          const float y = pow(INFINITY, z);
          result1[9 * i + 6] = y;
        }
        {
          const float y = pow(-INFINITY, z);
          result1[9 * i + 7] = y;
        }
        {
          const float y = pow(NAN, z);
          result1[9 * i + 8] = y;
        }
      }
    }
    // Vector2
    {
      const float2 b = zMakeFloat2(2.0f, 2.0f);
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = pow(b, z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 b = zMakeFloat3(2.0f, 2.0f, 2.0f);
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = pow(b, z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 b = zMakeFloat4(2.0f, 2.0f, 2.0f, 2.0f);
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = pow(b, z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZpown(
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
kernel void testZsqrt(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = zSqrt(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zSqrt(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zSqrt(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = zSqrt2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = zSqrt3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = zSqrt4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testRsqrt(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = rsqrt(z);
      result1[2 * index + 1] = y;
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = rsqrt(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = rsqrt(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = rsqrt(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testRsqrtSubnormal(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeSubnormal(x);
      result1[2 * index] = z;
      const float y = rsqrt(z);
      result1[2 * index + 1] = y;
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeSubnormal(x), makeSubnormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = rsqrt(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeSubnormal(x), makeSubnormal(0.85f * x), makeSubnormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = rsqrt(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeSubnormal(x), makeSubnormal(0.85f * x), makeSubnormal(0.5f * x), makeSubnormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = rsqrt(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZrsqrt(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = zRsqrt(z);
      result1[2 * index + 1] = y;
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = zRsqrt2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = zRsqrt3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = zRsqrt4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZrsqrtSubnormal(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeSubnormal(x);
      result1[2 * index] = z;
      const float y = zRsqrt(z);
      result1[2 * index + 1] = y;
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeSubnormal(x), makeSubnormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = zRsqrt2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeSubnormal(x), makeSubnormal(0.85f * x), makeSubnormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = zRsqrt3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeSubnormal(x), makeSubnormal(0.85f * x), makeSubnormal(0.5f * x), makeSubnormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = zRsqrt4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZcbrt(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(2 * index) / (float)resolution - 1.0f; // [-1, 1)
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = zCbrt(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zCbrt(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zCbrt(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = zCbrt2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = zCbrt3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = zCbrt4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testExp(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = (float)(2 * index) / (float)resolution - 1.0f; // [-1, 1)
    x = x * 100.0f;
    x = (x < 0.0f) ? -zLog2(zAbsF(x)) : zLog2(zAbsF(x));
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = exp(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = exp(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = -INFINITY;
        result1[2 * i] = z;
        const float y = exp(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 2;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = exp(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = exp(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = exp(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = exp(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZexp(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = (float)(2 * index) / (float)resolution - 1.0f; // [-1, 1)
    x = x * 100.0f;
    x = (x == 0.0f) ? x : ((x < 0.0f) ? -zLog2(zAbsF(x)) : zLog2(zAbsF(x)));
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = zExp(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zExp(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = -INFINITY;
        result1[2 * i] = z;
        const float y = zExp(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 2;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zExp(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = zExp2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = zExp3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = zExp4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testLog(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = log(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = log(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = log(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = log(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = log(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = log(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZlog(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = zLog(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zLog(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zLog(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = zLogF2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = zLogF3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = zLogF4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testLog2(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = log2(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = log2(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = log2(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = log2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = log2(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = log2(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZlog2(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = (float)(index + 1) / (float)resolution; // (0, 1]
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = zLog2(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zLog2(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zLog2(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = zLog2F2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = zLog2F3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = zLog2F4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testSin(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    x = (4.0f * zPiF) * x;
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = sin(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = sin(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = sin(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = sin(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = sin(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = sin(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZsin(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    x = (4.0f * zPiF) * x;
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = zSin(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zSin(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zSin(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = zSin2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = zSin3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = zSin4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testCos(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    x = (4.0f * zPiF) * x;
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = cos(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = cos(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = cos(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = cos(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = cos(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = cos(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZcos(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    x = (4.0f * zPiF) * x;
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = zCos(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zCos(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zCos(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = zCos2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = zCos3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = zCos4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZsincos(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    x = (4.0f * zPiF) * x;
    // Scalar
    {
      const float z = x;
      result1[3 * index] = z;
      float c = 0.0f;
      const float y = zSincos(z, &c);
      result1[3 * index + 1] = y;
      result1[3 * index + 2] = c;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[3 * i] = z;
        float c = 0.0f;
        const float y = zSincos(z, &c);
        result1[3 * i + 1] = y;
        result1[3 * i + 2] = c;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[3 * i] = z;
        float c = 0.0f;
        const float y = zSincos(z, &c);
        result1[3 * i + 1] = y;
        result1[3 * i + 2] = c;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[3 * index] = z;
      float2 c = zMakeFloat2(0.0f, 0.0f);
      const float2 y = zSincos2(z, &c);
      result2[3 * index + 1] = y;
      result2[3 * index + 2] = c;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[3 * index] = z;
      float3 c = zMakeFloat3(0.0f, 0.0f, 0.0f);
      const float3 y = zSincos3(z, &c);
      result3[3 * index + 1] = y;
      result3[3 * index + 2] = c;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[3 * index] = z;
      float4 c = zMakeFloat4(0.0f, 0.0f, 0.0f, 0.0f);
      const float4 y = zSincos4(z, &c);
      result4[3 * index + 1] = y;
      result4[3 * index + 2] = c;
    }
  }
}

/*!
  */
kernel void testTan(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    x = (4.0f * zPiF) * x;
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = tan(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = tan(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = tan(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = tan(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = tan(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = tan(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZtan(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    x = (4.0f * zPiF) * x;
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = zTan(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zTan(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zTan(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = zTan2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = zTan3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = zTan4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testAsin(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = asin(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = asin(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = asin(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = asin(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = asin(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = asin(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZasin(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = zAsin(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zAsin(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zAsin(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = zAsin2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = zAsin3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = zAsin4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testAcos(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = acos(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = acos(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = acos(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = acos(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = acos(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = acos(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZacos(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    // Scalar
    {
      const float z = x;
      result1[2 * index] = z;
      const float y = zAcos(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zAcos(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zAcos(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(x, 0.85f * x);
      result2[2 * index] = z;
      const float2 y = zAcos2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(x, 0.85f * x, 0.5f * x);
      result3[2 * index] = z;
      const float3 y = zAcos3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(x, 0.85f * x, 0.5f * x, 0.35f * x);
      result4[2 * index] = z;
      const float4 y = zAcos4(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testAtan(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = atan(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = -INFINITY;
        result1[2 * i] = z;
        const float y = atan(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = atan(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 2;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = atan(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = atan(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = atan(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = atan(z);
      result4[2 * index + 1] = y;
    }
  }
}

/*!
  */
kernel void testZatan(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const float x = -1.0f + 2.0f * ((float)(index + 1) / (float)resolution); // [-1, 1)
    // Scalar
    {
      const float z = makeNormal(x);
      result1[2 * index] = z;
      const float y = zAtan(z);
      result1[2 * index + 1] = y;
    }
    if (index == 0) {
      {
        const uint i = resolution;
        const float z = -INFINITY;
        result1[2 * i] = z;
        const float y = zAtan(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 1;
        const float z = INFINITY;
        result1[2 * i] = z;
        const float y = zAtan(z);
        result1[2 * i + 1] = y;
      }
      {
        const uint i = resolution + 2;
        const float z = NAN;
        result1[2 * i] = z;
        const float y = zAtan(z);
        result1[2 * i + 1] = y;
      }
    }
    // Vector2
    {
      const float2 z = zMakeFloat2(makeNormal(x), makeNormal(0.85f * x));
      result2[2 * index] = z;
      const float2 y = zAtan2(z);
      result2[2 * index + 1] = y;
    }
    // Vector3
    {
      const float3 z = zMakeFloat3(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x));
      result3[2 * index] = z;
      const float3 y = zAtan3(z);
      result3[2 * index + 1] = y;
    }
    // Vector4
    {
      const float4 z = zMakeFloat4(makeNormal(x), makeNormal(0.85f * x), makeNormal(0.5f * x), makeNormal(0.35f * x));
      result4[2 * index] = z;
      const float4 y = zAtan4(z);
      result4[2 * index + 1] = y;
    }
  }
}

#endif /* ZINVUL_MATH_TEST_MATH_FUNCTION_CL */
