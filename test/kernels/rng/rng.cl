/*!
  \file rng.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_RNG_TEST_RNG_CL
#define ZINVUL_RNG_TEST_RNG_CL

// Zinvul
#include "zinvul/cl/rng.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

/*!
  */
__kernel void testCmj(__global float* result_1d, __global float2* result_2d, const uint32b seed, const uint32b root_n, const uint32b num_of_samples)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    const uint32b n = root_n * root_n;
    // 1D random
    for (size_t sample = 0; sample < num_of_samples; ++sample) {
      const uint32b s = sample / n;
      const uint32b p = seed + sample % n;
      result_1d[sample] = zCmjDraw1D(s, p, root_n);
    }
    // 2D random
    for (size_t sample = 0; sample < num_of_samples; ++sample) {
      const uint32b s = sample / n;
      const uint32b p = seed + sample % n;
      result_2d[sample] = zCmjDraw2D(s, p, root_n);
    }
  }
}

/*!
  */
__kernel void testCmjImage(__global float* image_buffer, const uint32b sample, const uint32b root_n, const uint32b resolution)
{
  const uint32b index = zGetGlobalIdX();
  if (index < resolution) {
    const uint32b seed = index;
    const float r = zCmjDraw1D(sample, seed, root_n);
    const uint32b idx = 3u * index + sample;
    image_buffer[idx] = r;
  }
}

#endif /* ZINVUL_RNG_TEST_RNG_CL */
