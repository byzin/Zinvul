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
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using namespace zinvul;

// Forward declaration
__kernel void experiment(GlobalPtr<float4> buffer0, const uint32b resolution);

/*!
  \brief The code doesn't work
  */
__kernel void experiment(GlobalPtr<float4> buffer0, const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    const float4 t{1.0f, 1.0f, 1.0f, 0.0f};
    float4 v = buffer0[index] + t;
    {
      const uint32b c = treatAs<uint32b>(v.w) + 32u;
      v.w = treatAs<float>(c);
    }
    buffer0[index] = v;
  }
}

#endif /* ZINVUL_EXPERIMENT_TEST_CL */
