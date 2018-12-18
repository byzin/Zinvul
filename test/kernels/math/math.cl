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
      clamp_result1[0] = clamp(0, -1, 1);
      clamp_result1[1] = clamp(10, -1, 1);
      clamp_result1[2] = clamp(-10, -1, 1);
      clamp_result2[0] = clamp(0.0f, -1.0f, 1.0f);
      clamp_result2[1] = clamp(10.0f, -1.0f, 1.0f);
      clamp_result2[2] = clamp(-10.0f, -1.0f, 1.0f);
      // radian
      clamp_result2[3] = degrees(zPiF);
      clamp_result2[4] = radians(180.0f);
    }
    // vector2 clamp
    {
      clamp_result3[0] = clamp(zMakeInt2(0, 10),
                               zMakeInt2(-1, -2), 
                               zMakeInt2(1, 2));
      clamp_result3[1] = clamp(zMakeInt2(0, -10),
                               zMakeInt2(-1, -2), 
                               zMakeInt2(1, 2));
      clamp_result4[0] = clamp(zMakeFloat2(0.0f, 10.0f),
                               zMakeFloat2(-1.0f, -2.0f), 
                               zMakeFloat2(1.0f, 2.0f));
      clamp_result4[1] = clamp(zMakeFloat2(0.0f, -10.0f),
                               zMakeFloat2(-1.0f, -2.0f), 
                               zMakeFloat2(1.0f, 2.0f));
      // radian
      clamp_result4[2] = degrees(zMakeFloat2(zPiF, 0.5f * zPiF));
      clamp_result4[3] = radians(zMakeFloat2(180.0f, 90.0f));
    }
    // vector3 clamp
    {
      clamp_result5[0] = clamp(zMakeInt3(0, 10, -10),
                               zMakeInt3(-1, -2, -3), 
                               zMakeInt3(1, 2, 3));
      clamp_result6[0] = clamp(zMakeFloat3(0.0f, 10.0f, -10.0f),
                               zMakeFloat3(-1.0f, -2.0f, -3.0f), 
                               zMakeFloat3(1.0f, 2.0f, 3.0f));
      clamp_result6[1] = degrees(zMakeFloat3(zPiF, 0.5f * zPiF, 0.25f * zPiF));
      clamp_result6[2] = radians(zMakeFloat3(180.0f, 90.0f, 45.0f));
    }
    // vector4 clamp
    {
      clamp_result7[0] = clamp(zMakeInt4(0, 10, -10, 100),
                               zMakeInt4(-1, -2, -3, -4), 
                               zMakeInt4(1, 2, 3, 4));
      clamp_result8[0] = clamp(zMakeFloat4(0.0f, 10.0f, -10.0f, 100.0f),
                               zMakeFloat4(-1.0f, -2.0f, -3.0f, -4.0f), 
                               zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f));
      clamp_result8[1] = degrees(zMakeFloat4(zPiF, 0.5f * zPiF, 0.25f * zPiF, 0.0f));
      clamp_result8[2] = radians(zMakeFloat4(180.0f, 90.0f, 45.0f, 0.0f));
    }
  }
}

/*!
  */
kernel void testCommon2(global uint32b* abs_result1, global float* abs_result2,
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
      clamp_result1[0] = zClamp(0, -1, 1);
      clamp_result1[1] = zClamp(10, -1, 1);
      clamp_result1[2] = zClamp(-10, -1, 1);
      clamp_result2[0] = zClampF(0.0f, -1.0f, 1.0f);
      clamp_result2[1] = zClampF(10.0f, -1.0f, 1.0f);
      clamp_result2[2] = zClampF(-10.0f, -1.0f, 1.0f);
      // radian
      clamp_result2[3] = zDegreesF(zPiF);
      clamp_result2[4] = zRadiansF(180.0f);
    }
    // vector2 clamp
    {
      clamp_result3[0] = zClamp2(zMakeInt2(0, 10),
                                 zMakeInt2(-1, -2), 
                                 zMakeInt2(1, 2));
      clamp_result3[1] = zClamp2(zMakeInt2(0, -10),
                                 zMakeInt2(-1, -2), 
                                 zMakeInt2(1, 2));
      clamp_result4[0] = zClampF2(zMakeFloat2(0.0f, 10.0f),
                                  zMakeFloat2(-1.0f, -2.0f), 
                                  zMakeFloat2(1.0f, 2.0f));
      clamp_result4[1] = zClampF2(zMakeFloat2(0.0f, -10.0f),
                                  zMakeFloat2(-1.0f, -2.0f), 
                                  zMakeFloat2(1.0f, 2.0f));
      // radian
      clamp_result4[2] = zDegreesF2(zMakeFloat2(zPiF, 0.5f * zPiF));
      clamp_result4[3] = zRadiansF2(zMakeFloat2(180.0f, 90.0f));
    }
    // vector3 clamp
    {
      clamp_result5[0] = zClamp3(zMakeInt3(0, 10, -10),
                                 zMakeInt3(-1, -2, -3), 
                                 zMakeInt3(1, 2, 3));
      clamp_result6[0] = zClampF3(zMakeFloat3(0.0f, 10.0f, -10.0f),
                                  zMakeFloat3(-1.0f, -2.0f, -3.0f), 
                                  zMakeFloat3(1.0f, 2.0f, 3.0f));
      clamp_result6[1] = zDegreesF3(zMakeFloat3(zPiF, 0.5f * zPiF, 0.25f * zPiF));
      clamp_result6[2] = zRadiansF3(zMakeFloat3(180.0f, 90.0f, 45.0f));
    }
    // vector4 clamp
    {
      clamp_result7[0] = zClamp4(zMakeInt4(0, 10, -10, 100),
                                 zMakeInt4(-1, -2, -3, -4), 
                                 zMakeInt4(1, 2, 3, 4));
      clamp_result8[0] = zClampF4(zMakeFloat4(0.0f, 10.0f, -10.0f, 100.0f),
                                  zMakeFloat4(-1.0f, -2.0f, -3.0f, -4.0f), 
                                  zMakeFloat4(1.0f, 2.0f, 3.0f, 4.0f));
      clamp_result8[1] = zDegreesF4(zMakeFloat4(zPiF, 0.5f * zPiF, 0.25f * zPiF, 0.0f));
      clamp_result8[2] = zRadiansF4(zMakeFloat4(180.0f, 90.0f, 45.0f, 0.0f));
    }
  }
}

/*!
  */
kernel void testConstantValues(global float* pi_value)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    pi_value[0] = zPiF;
  }
}

#endif /* ZINVUL_MATH_TEST_MATH_CL */
