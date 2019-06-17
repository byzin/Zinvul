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

__kernel void testCmj64(GlobalPtr<float> result_1d, GlobalPtr<float2> result_2d, const uint32b seed, const uint32b num_of_samples);
#if !defined(Z_MAC)
__kernel void testCmj64D(GlobalPtr<double> result_1d, GlobalPtr<double2> result_2d, const uint32b seed, const uint32b num_of_samples);
#endif // Z_MAC
__kernel void testCmj64Image(GlobalPtr<float> color_buffer, const uint32b sample, const uint32b resolution);
__kernel void testCmj64Performance(GlobalPtr<float4> color_buffer,
    GlobalPtr<float4> color_comp_buffer,
    const uint32b sample,
    const uint32b iterations,
    const uint32b resolution);
__kernel void testCmj64PerformanceNormalization(GlobalPtr<float4> color_buffer,
    ConstGlobalPtr<float4> color_comp_buffer,
    const uint32b resolution);


/*!
  */
__kernel void testCmj64(GlobalPtr<float> result_1d, GlobalPtr<float2> result_2d, const uint32b seed, const uint32b num_of_samples)
{
  const uint32b index = getGlobalIdX();
  using Cmj = CmjN64;
  if (index < num_of_samples) {
    constexpr uint32b root_n = Cmj::getRootPeriod();
    constexpr uint32b n = root_n * root_n;
    // 1D random
    {
      const uint32b s = index / n;
      const uint32b p = seed + index % n;
      result_1d[index] = Cmj::generate1D<float>(s, p);
    }
    // 2D random
    {
      const uint32b s = index / n;
      const uint32b p = seed + index % n;
      result_2d[index] = Cmj::generate2D<float2>(s, p);
    }
  }
}

#if !defined(Z_MAC)

/*!
  */
__kernel void testCmj64D(GlobalPtr<double> result_1d, GlobalPtr<double2> result_2d, const uint32b seed, const uint32b num_of_samples)
{
  const uint32b index = getGlobalIdX();
  using Cmj = CmjN64;
  if (index < num_of_samples) {
    constexpr uint32b root_n = Cmj::getRootPeriod();
    constexpr uint32b n = root_n * root_n;
    // 1D random
    {
      const uint32b s = index / n;
      const uint32b p = seed + index % n;
      result_1d[index] = Cmj::generate1D<double>(s, p);
    }
    // 2D random
    {
      const uint32b s = index / n;
      const uint32b p = seed + index % n;
      result_2d[index] = Cmj::generate2D<double2>(s, p);
    }
  }
}

#endif // Z_MAC

/*!
  */
__kernel void testCmj64Image(GlobalPtr<float> color_buffer, const uint32b sample, const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  using Cmj = CmjN64;
  if (index < resolution) {
    const uint32b seed = index;
    const float c = Cmj::generate1D<float>(sample, seed);
    color_buffer[index] = c;
  }
}

/*!
  */
__kernel void testCmj64Performance(GlobalPtr<float4> color_buffer,
    GlobalPtr<float4> color_comp_buffer,
    const uint32b sample,
    const uint32b iterations,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();

  using Cmj = CmjN256;
  constexpr uint32b root_n = Cmj::getRootPeriod();
  constexpr uint32b n = root_n * root_n;
  constexpr uint32b seed = 0u;

  if (index < resolution) {
    float4 sum{0.0f, 0.0f, 0.0f, 0.0f};
    for (uint32b i = 0; i < iterations; ++i) {
      const uint32b s = (sample + i) % n;
      const uint32b p = seed + index + (sample + i) / n;
      const float r = Cmj::generate1D<float>(s, p);
      const float2 gb = Cmj::generate2D<float2>(s, p);

      sum = sum + float4{r, gb.x, gb.y, 0.0f};
      //! \todo Compensation
    }
    sum = color_buffer[index] + sum;
    {
      //! \todo using the w component as uint32b doesn't work
//      const uint32b count = treatAs<uint32b>(sum.w) + iterations;
//      sum.w = treatAs<float>(count);

      const float count = sum.w + cast<float>(iterations);
      sum.w = count;
    }
    color_buffer[index] = sum;
  }
}

/*!
  */
__kernel void testCmj64PerformanceNormalization(GlobalPtr<float4> color_buffer,
    ConstGlobalPtr<float4> color_comp_buffer,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    float4 color = color_buffer[index] + color_comp_buffer[index];

    //! \todo using the w component as uint32b doesn't work
//    const uint32b n = treatAs<uint32b>(color.w);
//    const float inv_n = (0u < n) ? 1.0f / cast<float>(n) : 0.0f;

    const float inv_n = (0.0f < color.w) ? 1.0f / color.w : 0.0f;

    color = inv_n * color;
    color.w = 1.0f;

    color_buffer[index] = color;
  }
}

#endif /* ZINVUL_RNG_TEST_RNG_CL */
