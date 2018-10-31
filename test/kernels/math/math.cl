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
kernel void testConstantValues(global float* pi_value)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    pi_value[0] = kPiF;
  }
}

#endif /* ZINVUL_MATH_TEST_MATH_CL */
