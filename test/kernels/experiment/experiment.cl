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

using zinvul::int8b;
using zinvul::int16b;
using zinvul::int32b;
using zinvul::int64b;
using zinvul::uint8b;
using zinvul::uint16b;
using zinvul::uint32b;
using zinvul::uint64b;
using zinvul::GlobalPtr;
using zinvul::ConstGlobalPtr;
using zinvul::ConstantPtr;
using zinvul::ConstConstantPtr;
using zinvul::Local;
using zinvul::LocalPtr;
using zinvul::ConstLocalPtr;
using zinvul::cast;
using zinvul::treatAs;

// Forward declaration
__kernel void experiment(GlobalPtr<float4> buffer0, const uint32b resolution);

/*!
  \brief The code doesn't work
  */
__kernel void experiment(GlobalPtr<float4> buffer0, const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    const float4 value{1.0f, 2.0f, 3.0f, 4.0f};
    zinvul::print("## PrintMessage\n");
    zinvul::print("## PrintValue: %g, %g, %g\n", value.x, value.y, value.z);
    zinvul::assertIfFalse(value.w == 4.0f, "## Assertion\n");
  }
}

#endif /* ZINVUL_EXPERIMENT_TEST_CL */
