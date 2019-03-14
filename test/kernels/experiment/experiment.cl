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
#include "zinvul/cl/geometric.cl"
#include "zinvul/cl/math.cl"
#include "zinvul/cl/rng.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

/*!
  */
__kernel void experiment(__global float* buffer)
{
  const uint32b index = zGetGlobalIdX();
  float x = buffer[index];
  x = 2.0f * x;
  buffer[index] = x;
}

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
  Rsqrt
  */
//__kernel void experiment(__global float4* buffer)
//{
//  const uint32b index = zGetGlobalIdX();
//  float4 x = buffer[index];
//  x = zLogF4Impl(x);
//  buffer[index] = x;
//}

#endif /* ZINVUL_EXPERIMENT_TEST_CL */
