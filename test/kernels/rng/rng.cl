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
#include "zinvul/cl/correlated_multi_jittered_engine.cl"
#include "zinvul/cl/floating_point.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using namespace zinvul;

/*!
  */
__kernel void testCmj64(GlobalPtr<float> result_1d, GlobalPtr<float2> result_2d, const uint32b seed, const uint32b num_of_samples)
{
  const uint32b index = getGlobalIdX();
  if (index < num_of_samples) {
    constexpr uint32b root_n = 8;
    constexpr uint32b n = root_n * root_n;
    // 1D random
    {
      const uint32b s = index / n;
      const uint32b p = seed + index % n;
      result_1d[index] = CmjN64::generate1D<float>(s, p);
    }
    // 2D random
//    {
//      const uint32b s = index / n;
//      const uint32b p = seed + index % n;
//      result_2d[sample] = CmjN64::generate2D(s, p);
//    }
  }
}

///*!
//  */
//__kernel void testCmjImage(__global float* image_buffer, const uint32b sample, const uint32b root_n, const uint32b resolution)
//{
//  const uint32b index = zGetGlobalIdX();
//  if (index < resolution) {
//    const uint32b seed = index;
//    const float r = zCmjDraw1D(sample, seed, root_n);
//    const uint32b idx = 3u * index + sample;
//    image_buffer[idx] = r;
//  }
//}

#endif /* ZINVUL_RNG_TEST_RNG_CL */
