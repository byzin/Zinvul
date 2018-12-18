/*!
  \file math.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_CL
#define ZINVUL_MATH_CL

#include "types.cl"
#include "utility.cl"

constant float zPiF = 3.14159274f;
constant float zInvPiF = 1.0f / 3.14159274f;

// Common functions

//! Return |x|
uint32b zAbs(const int32b x)
{
#ifdef ZINVUL_CPP_CL
  const int32b result = (x < 0) ? -x : x;
#else
  const int32b result = select(x, -x, x < 0);
#endif
  return (uint32b)result;
}

//! Return |x|
uint2 zAbs2(const int2 x)
{
#ifdef ZINVUL_CPP_CL
  const int2 result = zMakeInt2(0, 0);
#else
  const int2 result = select(x, -x, x < 0);
#endif
  return zMakeUInt2((uint32b)result.x, (uint32b)result.y);
}

//! Return |x|
uint3 zAbs3(const int3 x)
{
#ifdef ZINVUL_CPP_CL
  const int3 result = zMakeInt3(0, 0, 0);
#else
  const int3 result = select(x, -x, x < 0);
#endif
  return zMakeUInt3((uint32b)result.x, (uint32b)result.y, (uint32b)result.z);
}

//! Return |x|
uint4 zAbs4(const int4 x)
{
#ifdef ZINVUL_CPP_CL
  const int4 result = zMakeInt4(0, 0, 0, 0);
#else
  const int4 result = select(x, -x, x < 0);
#endif
  return zMakeUInt4((uint32b)result.x, (uint32b)result.y,
                    (uint32b)result.z, (uint32b)result.w);
}

#endif /* ZINVUL_MATH_CL */
