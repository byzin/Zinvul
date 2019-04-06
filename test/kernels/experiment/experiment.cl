/*!
  \file experiment.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_EXPERIMENT_TEST_CL
#define ZINVUL_EXPERIMENT_TEST_CL

// Zinvul
#include "zinvul/cl/atomic.cl"
#include "zinvul/cl/math.cl"
#include "zinvul/cl/matrix.cl"
#include "zinvul/cl/rng.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

/*!
  */
//__kernel void experiment(__global float* buffer1,
//    __global float2* buffer2,
//    __global float3* buffer3,
//    __global float4* buffer4)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    float x = buffer1[index];
//    x = 2.0f * x;
//    buffer1[index] = x;
//  }
//}

/*!
  */
__kernel void experiment(__global ZMatrix2x2* inputs,
    __global ZMatrix2x2* outputs)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    {
      const ZMatrix2x2 lhs = inputs[0];
      const ZMatrix2x2 rhs = inputs[1];
      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
                                      0.0f, 0.0f);
      zAddMat2x2(&lhs, &rhs, &result);
      outputs[0] = result;
    }
    {
      const __global ZMatrix2x2* lhs = &inputs[0];
      const __global ZMatrix2x2* rhs = &inputs[1];
      zAddGMat2x2G(lhs, rhs, &outputs[1]);
    }
  }
}

/*!
  */
//__kernel void experiment(__global float* buffer1,
//    __global float2* buffer2,
//    __global float3* buffer3,
//    __global float4* buffer4)
//{
//  const uint32b index = zGetGlobalIdX();
//  const uint32b id = zGetLocalIdX();
//  local float data[64];
//  if (index == 0) {
//    float x = buffer1[index];
//    data[id] = 2.0f * x;
//    buffer1[index] = data[id];
//  }
//}

//uint32b increment(const uint32b value)
//{
//  return value + 1u;
//}

/*!
  */
//__kernel void experiment(global uint32b* results, global int32b* table, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    uint32b i = 0;
//    zAtomicExpressionU(results, increment, i);
//    table[i] = 1;
//  }
//}

/*!
  CMJ 1d
  */
//__kernel void experiment(__global float* buffer, const uint32b seed, const uint32b root_n, const uint32b num_of_samples)
//{
//  const uint32b index = zGetGlobalIdX();
//  const uint32b n = root_n * root_n;
//  const uint32b s = index / n;
//  const uint32b p = seed + index % n;
//  buffer[index] = zCmjDraw1D(s, p, root_n);
//}

/*!
  CMJ 2d
  */
//__kernel void experiment(__global float2* buffer, const uint32b seed, const uint32b root_n, const uint32b num_of_samples)
//{
//  const uint32b index = zGetGlobalIdX();
//  const uint32b n = root_n * root_n;
//  const uint32b s = index / n;
//  const uint32b p = seed + index % n;
//  buffer[index] = zCmjDraw2D(s, p, root_n);
//}

/*!
  Rsqrt
  */
//__kernel void experiment(__global float4* buffer)
//{
//  const uint32b index = zGetGlobalIdX();
//  float4 x = buffer[index];
//  x = zRsqrt4Impl(x);
//  buffer[index] = x;
//}

/*!
  Log 
  */
//__kernel void experiment(__global float4* buffer)
//{
//  const uint32b index = zGetGlobalIdX();
//  float4 x = buffer[index];
//  x = zLogF4Impl(x);
//  buffer[index] = x;
//}

#endif /* ZINVUL_EXPERIMENT_TEST_CL */
