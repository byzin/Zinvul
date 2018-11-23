/*!
  \file utility.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_UTILITY_CL
#define ZINVUL_UTILITY_CL

#include "types.cl"

/*!
  */
uint getGlobalId(const uint dimension)
{
  const uint id = get_global_id(dimension);
  return id;
}

/*!
  */
uint getGlobalIdX()
{
  const uint id = get_global_id(0);
  return id;
}

/*!
  */
uint getGlobalIdY()
{
  const uint id = get_global_id(1);
  return id;
}

/*!
  */
uint getGlobalIdZ()
{
  const uint id = get_global_id(2);
  return id;
}

/*!
  */
float2 makeFloat2(const float x, const float y)
{
#ifdef ZINVUL_CPP_CL
  return float2{x, y};
#else // ZINVUL_CPP_CL
  return (float2)(x, y);
#endif // ZINVUL_CPP_CL
}

/*!
  */
float3 makeFloat3(const float x, const float y, const float z)
{
#ifdef ZINVUL_CPP_CL
  return float3{x, y, z};
#else // ZINVUL_CPP_CL
  return (float3)(x, y, z);
#endif // ZINVUL_CPP_CL
}

/*!
  */
float4 makeFloat4(const float x, const float y, const float z, const float w)
{
#ifdef ZINVUL_CPP_CL
  return float4{x, y, z, w};
#else // ZINVUL_CPP_CL
  return (float4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
}

#endif /* ZINVUL_UTILITY_CL */
