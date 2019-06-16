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
__kernel void experiment(GlobalPtr<uint32b> buffer0, const uint32b resolution);

/*!
  */
__kernel void experiment(GlobalPtr<uint32b> buffer0, const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    buffer0[index] = index;
  }
}

#endif /* ZINVUL_EXPERIMENT_TEST_CL */
