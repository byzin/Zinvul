/*!
  \file geometric.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_GEOMETRIC_CL
#define ZINVUL_GEOMETRIC_CL

#include "types.cl"
#include "utility.cl"

//! Return the cross product of p0.xyz and p1.xyz
float3 zCrossF3(const float3 p0, const float3 p1)
{
  return zMakeFloat3(p0.y * p1.z - p0.z * p1.y,
                     p0.z * p1.x - p0.x * p1.z,
                     p0.x * p1.y - p0.y * p1.x);
}

//! Return the cross product of p0.xyz and p1.xyz
float4 zCrossF4(const float4 p0, const float4 p1)
{
  return zMakeFloat4(p0.y * p1.z - p0.z * p1.y,
                     p0.z * p1.x - p0.x * p1.z,
                     p0.x * p1.y - p0.y * p1.x,
                     0.0f);
}

//! Compute dot product
float zDotF2(const float2 p0, const float2 p1)
{
  return (p0.x * p1.x) + (p0.y * p1.y);
}

//! Compute dot product
float zDotF3(const float3 p0, const float3 p1)
{
  return (p0.x * p1.x) + (p0.y * p1.y) + (p0.z * p1.z);
}

//! Compute dot product
float zDot3F4(const float4 p0, const float4 p1)
{
  return (p0.x * p1.x) + (p0.y * p1.y) + (p0.z * p1.z);
}

//! Compute dot product
float zDotF4(const float4 p0, const float4 p1)
{
  return (p0.x * p1.x) + (p0.y * p1.y) + (p0.z * p1.z) + (p0.w * p1.w);
}

#endif /* ZINVUL_GEOMETRIC_CL */
