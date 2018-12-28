/*!
  \file math.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_TEST_MATH_CL
#define ZINVUL_MATH_TEST_MATH_CL

// Zinvul
#include "zinvul/cl/types.cl"
#include "zinvul/cl/math.cl"
#include "zinvul/cl/utility.cl"

/*!
  */
kernel void testCommon(global uint32b* abs_result1, global float* abs_result2,
    global uint2* abs_result3, global float2* abs_result4,
    global uint3* abs_result5, global float3* abs_result6,
    global uint4* abs_result7, global float4* abs_result8,
    global int32b* clamp_result1, global float* clamp_result2,
    global int2* clamp_result3, global float2* clamp_result4,
    global int3* clamp_result5, global float3* clamp_result6,
    global int4* clamp_result7, global float4* clamp_result8)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    // scalar abs
    {
      abs_result1[0] = abs(1);
      abs_result1[1] = abs(-1);
      abs_result1[2] = abs(1u);
      abs_result2[0] = fabs(1.0f);
      abs_result2[1] = fabs(-1.0f);
    }
    // vector2 abs
    {
      abs_result3[0] = abs(zMakeInt2(1, -2));
      abs_result3[1] = abs(zMakeUInt2(1, 2));
      abs_result4[0] = fabs(zMakeFloat2(1.0f, -2.0f));
    }
    // vector3 abs
    {
      abs_result5[0] = abs(zMakeInt3(1, -2, 3));
      abs_result5[1] = abs(zMakeUInt3(1, 2, 3));
      abs_result6[0] = fabs(zMakeFloat3(1.0f, -2.0f, 3.0f));
    }
    // vector4 abs
    {
      abs_result7[0] = abs(zMakeInt4(1, -2, 3, -4));
      abs_result7[1] = abs(zMakeUInt4(1, 2, 3, 4));
      abs_result8[0] = fabs(zMakeFloat4(1.0f, -2.0f, 3.0f, -4.0f));
    }
    // scalar clamp
    {
      clamp_result1[0] = max(0, -1);
      clamp_result1[1] = min(0, 1);
      clamp_result1[2] = clamp(0, -1, 1);
      clamp_result1[3] = max(10, -1);
      clamp_result1[4] = min(10, 1);
      clamp_result1[5] = clamp(10, -1, 1);
      clamp_result1[6] = max(-10, -1);
      clamp_result1[7] = min(-10, 1);
      clamp_result1[8] = clamp(-10, -1, 1);
      clamp_result2[0] = max(0.0f, -1.0f);
      clamp_result2[1] = min(0.0f, 1.0f);
      clamp_result2[2] = clamp(0.0f, -1.0f, 1.0f);
      clamp_result2[3] = max(10.0f, -1.0f);
      clamp_result2[4] = min(10.0f, 1.0f);
      clamp_result2[5] = clamp(10.0f, -1.0f, 1.0f);
      clamp_result2[6] = max(-10.0f, -1.0f);
      clamp_result2[7] = min(-10.0f, 1.0f);
      clamp_result2[8] = clamp(-10.0f, -1.0f, 1.0f);
    }
    // vector2 clamp
    {
      clamp_result3[0] = max(zMakeInt2(0, 10), zMakeInt2(-1, -2));
      clamp_result3[1] = min(zMakeInt2(0, 10), zMakeInt2(1, 2));
      clamp_result3[2] = clamp(zMakeInt2(0, 10),
                               zMakeInt2(-1, -2), 
                               zMakeInt2(1, 2));
      clamp_result3[3] = max(zMakeInt2(0, -10), zMakeInt2(-1, -2));
      clamp_result3[4] = min(zMakeInt2(0, -2), zMakeInt2(1, 2));
      clamp_result3[5] = clamp(zMakeInt2(0, -10),
                               zMakeInt2(-1, -2), 
                               zMakeInt2(1, 2));
      clamp_result4[0] = max(zMakeFloat2(0.0f, 10.0f), zMakeFloat2(-1.0f, -2.0f));
      clamp_result4[1] = min(zMakeFloat2(0.0f, 10.0f), zMakeFloat2(1.0f, 2.0f));
      clamp_result4[2] = clamp(zMakeFloat2(0.0f, 10.0f),
                               zMakeFloat2(-1.0f, -2.0f), 
                               zMakeFloat2(1.0f, 2.0f));
      clamp_result4[3] = max(zMakeFloat2(0.0f, -10.0f), zMakeFloat2(-1.0f, -2.0f));
      clamp_result4[4] = min(zMakeFloat2(0.0f, -2.0f), zMakeFloat2(1.0f, 2.0f));
      clamp_result4[5] = clamp(zMakeFloat2(0.0f, -10.0f),
                               zMakeFloat2(-1.0f, -2.0f), 
                               zMakeFloat2(1.0f, 2.0f));
    }
    // vector3 clamp
    {
      clamp_result5[0] = max(zMakeInt3(0, 10, -10), zMakeInt3(-1, -2, -3));
      clamp_result5[1] = min(zMakeInt3(0, 10, -3), zMakeInt3(1, 2, 3));
      clamp_result5[2] = clamp(zMakeInt3(0, 10, -10),
                               zMakeInt3(-1, -2, -3), 
                               zMakeInt3(1, 2, 3));
      clamp_result6[0] = max(zMakeFloat3(0.0f, 10.0f, -10.0f),
                             zMakeFloat3(-1.0f, -2.0f, -3.0f));
      clamp_result6[1] = min(zMakeFloat3(0.0f, 10.0f, -3.0f),
                             zMakeFloat3(1.0f, 2.0f, 3.0f));
      clamp_result6[2] = clamp(zMakeFloat3(0.0f, 10.0f, -10.0f),
                               zMakeFloat3(-1.0f, -2.0f, -3.0f), 
                               zMakeFloat3(1.0f, 2.0f, 3.0f));
    }
    // vector4 clamp
    {
      clamp_result7[0] = max(zMakeInt4(0, 10, -10, 100), zMakeInt4(-1, -2, -3, -4));
      clamp_result7[1] = min(zMakeInt4(0, 10, -3, 100), zMakeInt4(1, 2, 3, 4));
      clamp_result7[2] = clamp(zMakeInt4(0, 10, -10, 100),
                               zMakeInt4(-1, -2, -3, -4), 
                               zMakeInt4(1, 2, 3, 4));
      clamp_result8[0] = max(zMakeFloat4(0.0f, 10.0f, -10.0f, 100.0f),
                             zMakeFloat4(-1.0f, -2.0f, -3.0f, -4.0f));
      clamp_result8[1] = min(zMakeFloat4(0.0f, 10.0f, -3.0f, 100.0f),
                             zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f));
      clamp_result8[2] = clamp(zMakeFloat4(0.0f, 10.0f, -10.0f, 100.0f),
                               zMakeFloat4(-1.0f, -2.0f, -3.0f, -4.0f), 
                               zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f));
    }
  }
}

/*!
  */
kernel void testRadian(
    global float* radian_result1, global float2* radian_result2,
    global float3* radian_result3, global float4* radian_result4)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    {
      // radian
      radian_result1[0] = degrees(zPiF);
      radian_result1[1] = radians(180.0f);
    }
    // vector2
    {
      // radian
      radian_result2[0] = degrees(zMakeFloat2(zPiF, 0.5f * zPiF));
      radian_result2[1] = radians(zMakeFloat2(180.0f, 90.0f));
    }
    // vector3
    {
      radian_result3[0] = degrees(zMakeFloat3(zPiF, 0.5f * zPiF, 0.25f * zPiF));
      radian_result3[1] = radians(zMakeFloat3(180.0f, 90.0f, 45.0f));
    }
    // vector4
    {
      radian_result4[0] = degrees(zMakeFloat4(zPiF, 0.5f * zPiF, 0.25f * zPiF, 0.0f));
      radian_result4[1] = radians(zMakeFloat4(180.0f, 90.0f, 45.0f, 0.0f));
    }
  }
}

/*!
  */
kernel void testZCommon(global uint32b* abs_result1, global float* abs_result2,
    global uint2* abs_result3, global float2* abs_result4,
    global uint3* abs_result5, global float3* abs_result6,
    global uint4* abs_result7, global float4* abs_result8,
    global int32b* clamp_result1, global float* clamp_result2,
    global int2* clamp_result3, global float2* clamp_result4,
    global int3* clamp_result5, global float3* clamp_result6,
    global int4* clamp_result7, global float4* clamp_result8)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    // scalar abs
    {
      abs_result1[0] = zAbs(1);
      abs_result1[1] = zAbs(-1);
      abs_result2[0] = zAbsF(1.0f);
      abs_result2[1] = zAbsF(-1.0f);
    }
    // vector2 abs
    {
      abs_result3[0] = zAbs2(zMakeInt2(1, -2));
      abs_result4[0] = zAbsF2(zMakeFloat2(1.0f, -2.0f));
    }
    // vector3 abs
    {
      abs_result5[0] = zAbs3(zMakeInt3(1, -2, 3));
      abs_result6[0] = zAbsF3(zMakeFloat3(1.0f, -2.0f, 3.0f));
    }
    // vector4 abs
    {
      abs_result7[0] = zAbs4(zMakeInt4(1, -2, 3, -4));
      abs_result8[0] = zAbsF4(zMakeFloat4(1.0f, -2.0f, 3.0f, -4.0f));
    }
    // scalar clamp
    {
      clamp_result1[0] = zMax(0, -1);
      clamp_result1[1] = zMin(0, 1);
      clamp_result1[2] = zClamp(0, -1, 1);
      clamp_result1[3] = zMax(10, -1);
      clamp_result1[4] = zMin(10, 1);
      clamp_result1[5] = zClamp(10, -1, 1);
      clamp_result1[6] = zMax(-10, -1);
      clamp_result1[7] = zMin(-10, 1);
      clamp_result1[8] = zClamp(-10, -1, 1);
      clamp_result2[0] = zMaxF(0.0f, -1.0f);
      clamp_result2[1] = zMinF(0.0f, 1.0f);
      clamp_result2[2] = zClampF(0.0f, -1.0f, 1.0f);
      clamp_result2[3] = zMaxF(10.0f, -1.0f);
      clamp_result2[4] = zMinF(10.0f, 1.0f);
      clamp_result2[5] = zClampF(10.0f, -1.0f, 1.0f);
      clamp_result2[6] = zMaxF(-10.0f, -1.0f);
      clamp_result2[7] = zMinF(-10.0f, 1.0f);
      clamp_result2[8] = zClampF(-10.0f, -1.0f, 1.0f);
    }
    // vector2 clamp
    {
      clamp_result3[0] = zMax2(zMakeInt2(0, 10), zMakeInt2(-1, -2));
      clamp_result3[1] = zMin2(zMakeInt2(0, 10), zMakeInt2(1, 2));
      clamp_result3[2] = zClamp2(zMakeInt2(0, 10),
                                 zMakeInt2(-1, -2), 
                                 zMakeInt2(1, 2));
      clamp_result3[3] = zMax2(zMakeInt2(0, -10), zMakeInt2(-1, -2));
      clamp_result3[4] = zMin2(zMakeInt2(0, -2), zMakeInt2(1, 2));
      clamp_result3[5] = zClamp2(zMakeInt2(0, -10),
                                 zMakeInt2(-1, -2), 
                                 zMakeInt2(1, 2));
      clamp_result4[0] = zMaxF2(zMakeFloat2(0.0f, 10.0f), zMakeFloat2(-1.0f, -2.0f));
      clamp_result4[1] = zMinF2(zMakeFloat2(0.0f, 10.0f), zMakeFloat2(1.0f, 2.0f));
      clamp_result4[2] = zClampF2(zMakeFloat2(0.0f, 10.0f),
                                  zMakeFloat2(-1.0f, -2.0f), 
                                  zMakeFloat2(1.0f, 2.0f));
      clamp_result4[3] = zMaxF2(zMakeFloat2(0.0f, -10.0f), zMakeFloat2(-1.0f, -2.0f));
      clamp_result4[4] = zMinF2(zMakeFloat2(0.0f, -2.0f), zMakeFloat2(1.0f, 2.0f));
      clamp_result4[5] = zClampF2(zMakeFloat2(0.0f, -10.0f),
                                  zMakeFloat2(-1.0f, -2.0f), 
                                  zMakeFloat2(1.0f, 2.0f));
    }
    // vector3 clamp
    {
      clamp_result5[0] = zMax3(zMakeInt3(0, 10, -10), zMakeInt3(-1, -2, -3));
      clamp_result5[1] = zMin3(zMakeInt3(0, 10, -3), zMakeInt3(1, 2, 3));
      clamp_result5[2] = zClamp3(zMakeInt3(0, 10, -10),
                                 zMakeInt3(-1, -2, -3), 
                                 zMakeInt3(1, 2, 3));
      clamp_result6[0] = zMaxF3(zMakeFloat3(0.0f, 10.0f, -10.0f),
                                zMakeFloat3(-1.0f, -2.0f, -3.0f));
      clamp_result6[1] = zMinF3(zMakeFloat3(0.0f, 10.0f, -3.0f),
                                zMakeFloat3(1.0f, 2.0f, 3.0f));
      clamp_result6[2] = zClampF3(zMakeFloat3(0.0f, 10.0f, -10.0f),
                                  zMakeFloat3(-1.0f, -2.0f, -3.0f), 
                                  zMakeFloat3(1.0f, 2.0f, 3.0f));
    }
    // vector4 clamp
    {
      clamp_result7[0] = zMax4(zMakeInt4(0, 10, -10, 100), zMakeInt4(-1, -2, -3, -4));
      clamp_result7[1] = zMin4(zMakeInt4(0, 10, -3, 100), zMakeInt4(1, 2, 3, 4));
      clamp_result7[2] = zClamp4(zMakeInt4(0, 10, -10, 100),
                                 zMakeInt4(-1, -2, -3, -4), 
                                 zMakeInt4(1, 2, 3, 4));
      clamp_result8[0] = zMaxF4(zMakeFloat4(0.0f, 10.0f, -10.0f, 100.0f),
                                zMakeFloat4(-1.0f, -2.0f, -3.0f, -4.0f));
      clamp_result8[1] = zMinF4(zMakeFloat4(0.0f, 10.0f, -3.0f, 100.0f),
                                zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f));
      clamp_result8[2] = zClampF4(zMakeFloat4(0.0f, 10.0f, -10.0f, 100.0f),
                                  zMakeFloat4(-1.0f, -2.0f, -3.0f, -4.0f), 
                                  zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f));
    }
  }
}

/*!
  */
kernel void testZRadian(
    global float* radian_result1, global float2* radian_result2,
    global float3* radian_result3, global float4* radian_result4)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    {
      // radian
      radian_result1[0] = zDegreesF(zPiF);
      radian_result1[1] = zRadiansF(180.0f);
    }
    // vector2
    {
      // radian
      radian_result2[0] = zDegreesF2(zMakeFloat2(zPiF, 0.5f * zPiF));
      radian_result2[1] = zRadiansF2(zMakeFloat2(180.0f, 90.0f));
    }
    // vector3
    {
      radian_result3[0] = zDegreesF3(zMakeFloat3(zPiF, 0.5f * zPiF, 0.25f * zPiF));
      radian_result3[1] = zRadiansF3(zMakeFloat3(180.0f, 90.0f, 45.0f));
    }
    // vector4
    {
      radian_result4[0] = zDegreesF4(zMakeFloat4(zPiF, 0.5f * zPiF, 0.25f * zPiF, 0.0f));
      radian_result4[1] = zRadiansF4(zMakeFloat4(180.0f, 90.0f, 45.0f, 0.0f));
    }
  }
}

/*!
  */
kernel void testConstantValues(global int32b* int_values, global float* float_values)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    size_t int_index = 0;
    size_t float_index = 0;
    // Integer
    int_values[int_index++] = FLT_DIG;
    int_values[int_index++] = FLT_MANT_DIG;
    int_values[int_index++] = FLT_MAX_10_EXP;
    int_values[int_index++] = FLT_MAX_EXP;
    int_values[int_index++] = FLT_MIN_10_EXP;
    int_values[int_index++] = FLT_MIN_EXP;
    int_values[int_index++] = FLT_RADIX;
    int_values[int_index++] = CHAR_BIT;
    int_values[int_index++] = SCHAR_MAX;
    int_values[int_index++] = SCHAR_MIN;
    int_values[int_index++] = CHAR_MAX;
    int_values[int_index++] = CHAR_MIN;
    int_values[int_index++] = SHRT_MAX;
    int_values[int_index++] = SHRT_MIN;
    int_values[int_index++] = INT_MAX;
    int_values[int_index++] = INT_MIN;
    // Float
    float_values[float_index++] = MAXFLOAT;
    float_values[float_index++] = INFINITY;
    float_values[float_index++] = NAN;
    float_values[float_index++] = FLT_MAX;
    float_values[float_index++] = FLT_MIN;
    float_values[float_index++] = FLT_EPSILON;
    float_values[float_index++] = M_E_F;
    float_values[float_index++] = M_LOG2E_F;
    float_values[float_index++] = M_LOG10E_F;
    float_values[float_index++] = M_LN2_F;
    float_values[float_index++] = M_LN10_F;
    float_values[float_index++] = M_PI_F;
    float_values[float_index++] = M_PI_2_F;
    float_values[float_index++] = M_PI_4_F;
    float_values[float_index++] = M_1_PI_F;
    float_values[float_index++] = M_2_PI_F;
    float_values[float_index++] = M_2_SQRTPI_F;
    float_values[float_index++] = M_SQRT2_F;
    float_values[float_index++] = M_SQRT1_2_F;
    // Zinvul
    float_values[float_index++] = zPiF;
  }
}

/*!
  */
kernel void testMaxFunction(global int32b* result1, global float* result2)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    // Scalar
    size_t scalar_index = 0;
    {
      const int32b lhs = INT_MIN;
      const int32b rhs = INT_MAX;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    {
      const int32b lhs = INT_MAX;
      const int32b rhs = INT_MIN;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 0;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    {
      const int32b lhs = 0;
      const int32b rhs = -1;
      result1[scalar_index++] = zMax(lhs,rhs);
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    {
      const int32b lhs = 1;
      const int32b rhs = -1;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    {
      const int32b lhs = -10;
      const int32b rhs = -1;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    {
      const int32b lhs = -1;
      const int32b rhs = -10;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    {
      const int32b lhs = 10;
      const int32b rhs = 1;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    {
      const int32b lhs = 1;
      const int32b rhs = 10;
      result1[scalar_index++] = zMax(lhs, rhs);
    }
    scalar_index = 0;
    {
      const float lhs = -FLT_MAX;
      const float rhs = FLT_MAX;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = FLT_MAX;
      const float rhs = -FLT_MAX;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = -1.0f;
      const float rhs = 0.0f;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = 0.0f;
      const float rhs = -1.0f;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = -1.0f;
      const float rhs = 1.0f;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = 1.0f;
      const float rhs = -1.0f;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = -10.0f;
      const float rhs = -1.0f;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = -1.0f;
      const float rhs = -10.0f;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = 10.0f;
      const float rhs = 1.0f;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
    {
      const float lhs = 1.0f;
      const float rhs = 10.0f;
      result2[scalar_index++] = zMaxF(lhs, rhs);
    }
  }
}

/*!
  */
kernel void testMinFunction(global int32b* result1, global float* result2)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    // Scalar
    size_t scalar_index = 0;
    {
      const int32b lhs = INT_MIN;
      const int32b rhs = INT_MAX;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    {
      const int32b lhs = INT_MAX;
      const int32b rhs = INT_MIN;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 0;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    {
      const int32b lhs = 0;
      const int32b rhs = -1;
      result1[scalar_index++] = zMin(lhs,rhs);
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    {
      const int32b lhs = 1;
      const int32b rhs = -1;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    {
      const int32b lhs = -10;
      const int32b rhs = -1;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    {
      const int32b lhs = -1;
      const int32b rhs = -10;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    {
      const int32b lhs = 10;
      const int32b rhs = 1;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    {
      const int32b lhs = 1;
      const int32b rhs = 10;
      result1[scalar_index++] = zMin(lhs, rhs);
    }
    scalar_index = 0;
    {
      const float lhs = -FLT_MAX;
      const float rhs = FLT_MAX;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = FLT_MAX;
      const float rhs = -FLT_MAX;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = -1.0f;
      const float rhs = 0.0f;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = 0.0f;
      const float rhs = -1.0f;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = -1.0f;
      const float rhs = 1.0f;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = 1.0f;
      const float rhs = -1.0f;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = -10.0f;
      const float rhs = -1.0f;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = -1.0f;
      const float rhs = -10.0f;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = 10.0f;
      const float rhs = 1.0f;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
    {
      const float lhs = 1.0f;
      const float rhs = 10.0f;
      result2[scalar_index++] = zMinF(lhs, rhs);
    }
  }
}

/*!
  */
kernel void testMathFunctions(global float* result1, global float2* result2,
    global float3* result3, global float4* result4)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
#if defined(ZINVUL_VULKAN)
    size_t result_index = 0;
    int32b expr = 0;
    result1[0] = frexp(5.14f, &expr);
    // acos
    {
      const float p_list[] = {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};
      size_t i = 0;
      {
        const float p = p_list[i++];
        result1[result_index] = acos(p);
        result2[result_index] = acos(zMakeFloat2(p, p));
        result3[result_index] = acos(zMakeFloat3(p, p, p));
        result4[result_index++] = acos(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = acos(p);
        result2[result_index] = acos(zMakeFloat2(p, p));
        result3[result_index] = acos(zMakeFloat3(p, p, p));
        result4[result_index++] = acos(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = acos(p);
        result2[result_index] = acos(zMakeFloat2(p, p));
        result3[result_index] = acos(zMakeFloat3(p, p, p));
        result4[result_index++] = acos(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = acos(p);
        result2[result_index] = acos(zMakeFloat2(p, p));
        result3[result_index] = acos(zMakeFloat3(p, p, p));
        result4[result_index++] = acos(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = acos(p);
        result2[result_index] = acos(zMakeFloat2(p, p));
        result3[result_index] = acos(zMakeFloat3(p, p, p));
        result4[result_index++] = acos(zMakeFloat4(p, p, p, p));
      }
    }
    // asin
    {
      const float p_list[] = {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};
      size_t i = 0;
      {
        const float p = p_list[i++];
        result1[result_index] = asin(p);
        result2[result_index] = asin(zMakeFloat2(p, p));
        result3[result_index] = asin(zMakeFloat3(p, p, p));
        result4[result_index++] = asin(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = asin(p);
        result2[result_index] = asin(zMakeFloat2(p, p));
        result3[result_index] = asin(zMakeFloat3(p, p, p));
        result4[result_index++] = asin(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = asin(p);
        result2[result_index] = asin(zMakeFloat2(p, p));
        result3[result_index] = asin(zMakeFloat3(p, p, p));
        result4[result_index++] = asin(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = asin(p);
        result2[result_index] = asin(zMakeFloat2(p, p));
        result3[result_index] = asin(zMakeFloat3(p, p, p));
        result4[result_index++] = asin(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = asin(p);
        result2[result_index] = asin(zMakeFloat2(p, p));
        result3[result_index] = asin(zMakeFloat3(p, p, p));
        result4[result_index++] = asin(zMakeFloat4(p, p, p, p));
      }
    }
    // asin
    {
      const float p_list[] = {-INFINITY, -1.0f, -0.5f, 0.0f, 0.5f, 1.0f, INFINITY};
      size_t i = 0;
      {
        const float p = p_list[i++];
        result1[result_index] = atan(p);
        result2[result_index] = atan(zMakeFloat2(p, p));
        result3[result_index] = atan(zMakeFloat3(p, p, p));
        result4[result_index++] = atan(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = atan(p);
        result2[result_index] = atan(zMakeFloat2(p, p));
        result3[result_index] = atan(zMakeFloat3(p, p, p));
        result4[result_index++] = atan(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = atan(p);
        result2[result_index] = atan(zMakeFloat2(p, p));
        result3[result_index] = atan(zMakeFloat3(p, p, p));
        result4[result_index++] = atan(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = atan(p);
        result2[result_index] = atan(zMakeFloat2(p, p));
        result3[result_index] = atan(zMakeFloat3(p, p, p));
        result4[result_index++] = atan(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = atan(p);
        result2[result_index] = atan(zMakeFloat2(p, p));
        result3[result_index] = atan(zMakeFloat3(p, p, p));
        result4[result_index++] = atan(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = atan(p);
        result2[result_index] = atan(zMakeFloat2(p, p));
        result3[result_index] = atan(zMakeFloat3(p, p, p));
        result4[result_index++] = atan(zMakeFloat4(p, p, p, p));
      }
      {
        const float p = p_list[i++];
        result1[result_index] = atan(p);
        result2[result_index] = atan(zMakeFloat2(p, p));
        result3[result_index] = atan(zMakeFloat3(p, p, p));
        result4[result_index++] = atan(zMakeFloat4(p, p, p, p));
      }
    }
#endif
  }
}

#endif /* ZINVUL_MATH_TEST_MATH_CL */
