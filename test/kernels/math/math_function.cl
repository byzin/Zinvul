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
    const float n = x * x * FLT_MAX;
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
    const float n = x * x * FLT_MAX;
    // Scalar
    {
      const float z = n;
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
      const float2 z = zMakeFloat2(n, n);
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
      const float3 z = zMakeFloat3(n, n, n);
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
      const float4 z = zMakeFloat4(n, n, n, n);
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

#endif /* ZINVUL_MATH_TEST_MATH_FUNCTION_CL */
