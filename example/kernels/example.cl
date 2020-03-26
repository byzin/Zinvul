/*!
  \file example.cl
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_EXAMPLE_CL
#define ZINVUL_EXAMPLE_CL

// Zinvul
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using zinvul::int32b;
using zinvul::uint32b;

// Forward declaration
__kernel void testKernel(zinvul::ConstGlobalPtr<int32b> inputs,
    zinvul::GlobalPtr<float> outputs,
    const uint32b resolution);

/*!
  \details No detailed description

  \param [in] inputs No description.
  \param [out] outputs No description.
  \param [in] resolution No description.
  */
__kernel void testKernel(zinvul::ConstGlobalPtr<int32b> inputs,
    zinvul::GlobalPtr<float> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    outputs[index] = zinvul::cast<float>(inputs[index]);
  }
}

#endif // ZINVUL_EXAMPLE_CL
