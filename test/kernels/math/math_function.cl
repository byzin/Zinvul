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
  f = (x == 0.0f) ? 0.0f : s * zClampF(f, FLT_MIN, FLT_MAX);
  return f;
}

//! Make a subnormal value
float makeSubnormal(const float x)
{
  const float subnormal_max = FLT_MIN - zDenormMinF;
  const float s = (x < 0.0f) ? -1.0f : 1.0f;
  float f = (x * x * x * x) * subnormal_max;
  f = s * zClampF(f, zDenormMinF, subnormal_max);
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
    const float n = makeNormal(x);
    // Scalar
    {
      const float z = n;
      result1[3 * index] = z;
      //! \todo Remove comment out. It crashes on AMD
//      int32b e = 0;
//      const float m = frexp(z, &e);
//      result1[3 * index + 1] = m;
//      result_exp1[index] = e;
//      const float f = ldexp(m, e);
//      result1[3 * index + 2] = f;
      (void)result1;
      (void)result_exp1;
    }
    if (index == 0) {
      {
//        const uint i = resolution;
//        const float z = INFINITY;
//        result1[3 * i] = z;
//        int32b e = 0;
//        const float m = frexp(z, &e);
//        result1[3 * i + 1] = m;
//        result_exp1[i] = e;
//        const float f = ldexp(m, e);
//        result1[3 * i + 2] = f;
      }
      {
//        const uint i = resolution + 1;
//        const float z = NAN;
//        result1[3 * i] = z;
//        int32b e = 0;
//        const float m = frexp(z, &e);
//        result1[3 * i + 1] = m;
//        result_exp1[i] = e;
//        const float f = ldexp(m, e);
//        result1[3 * i + 2] = f;
      }
    }
    // Vector2
    {
//      const float2 z = zMakeFloat2(n, n);
//      result2[3 * index] = z;
//      int2 e = zMakeInt2(0, 0);
//      const float2 m = frexp(z, &e);
//      result2[3 * index + 1] = m;
//      result_exp2[index] = e;
//      const float2 f = ldexp(m, e);
//      result2[3 * index + 2] = f;
      (void)result2;
      (void)result_exp2;
    }
    // Vector3
    {
//      const float3 z = zMakeFloat3(n, n, n);
//      result3[3 * index] = z;
//      int3 e = zMakeInt3(0, 0, 0);
//      const float3 m = frexp(z, &e);
//      result3[3 * index + 1] = m;
//      result_exp3[index] = e;
//      const float3 f = ldexp(m, e);
//      result3[3 * index + 2] = f;
      (void)result3;
      (void)result_exp3;
    }
    // Vector4
    {
//      const float4 z = zMakeFloat4(n, n, n, n);
//      result4[3 * index] = z;
//      int4 e = zMakeInt4(0, 0, 0, 0);
//      const float4 m = frexp(z, &e);
//      result4[3 * index + 1] = m;
//      result_exp4[index] = e;
//      const float4 f = ldexp(m, e);
//      result4[3 * index + 2] = f;
      (void)result4;
      (void)result_exp4;
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
      result1[7 * index] = z;
      {
        const float y = pow(0.0f, z);
        result1[7 * index + 1] = y;
      }
      {
        const float y = pow(1.0f, z);
        result1[7 * index + 2] = y;
      }
      {
        const float y = pow(2.0f, z);
        result1[7 * index + 3] = y;
      }
      {
        const float y = pow(0.5f, z);
        result1[7 * index + 4] = y;
      }
      {
        const float y = pow(INFINITY, z);
        result1[7 * index + 5] = y;
      }
      {
        const float y = pow(-INFINITY, z);
        result1[7 * index + 6] = y;
      }
    }
//    if (index == 0) {
//      {
//        const uint i = resolution;
//        const float z = INFINITY;
//        result1[2 * i] = z;
//        const float y = sqrt(z);
//        result1[2 * i + 1] = y;
//      }
//      {
//        const uint i = resolution + 1;
//        const float z = NAN;
//        result1[2 * i] = z;
//        const float y = sqrt(z);
//        result1[2 * i + 1] = y;
//      }
//    }
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
kernel void testZsqrt(
    global float* result1,
    global float2* result2,
    global float3* result3,
    global float4* result4,
    const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  const uint32b offset = 100u;
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

#endif /* ZINVUL_MATH_TEST_MATH_FUNCTION_CL */
