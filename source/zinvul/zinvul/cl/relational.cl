/*!
  \file relational.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_RELATIONAL_CL
#define ZINVUL_RELATIONAL_CL

#include "types.cl"
#include "utility.cl"

//! Return c ? b : a
int32b zSelect(const int32b a, const int32b b, const int32b c)
{
  return c ? b : a;
}

//! Return c ? b : a
int2 zSelect2(const int2 a, const int2 b, const int2 c)
{
  return zMakeInt2(c.x ? b.x : a.x,
                   c.y ? b.y : a.y);
}

//! Return c ? b : a
int3 zSelect3(const int3 a, const int3 b, const int3 c)
{
  return zMakeInt3(c.x ? b.x : a.x,
                   c.y ? b.y : a.y,
                   c.z ? b.z : a.z);
}

//! Return c ? b : a
int4 zSelect4(const int4 a, const int4 b, const int4 c)
{
  return zMakeInt4(c.x ? b.x : a.x,
                   c.y ? b.y : a.y,
                   c.z ? b.z : a.z,
                   c.w ? b.w : a.w);
}

//! Return c ? b : a
uint32b zSelectU(const uint32b a, const uint32b b, const int32b c)
{
  return c ? b : a;
}

//! Return c ? b : a
uint2 zSelectU2(const uint2 a, const uint2 b, const int2 c)
{
  return zMakeUInt2(c.x ? b.x : a.x,
                    c.y ? b.y : a.y);
}

//! Return c ? b : a
uint3 zSelectU3(const uint3 a, const uint3 b, const int3 c)
{
  return zMakeUInt3(c.x ? b.x : a.x,
                    c.y ? b.y : a.y,
                    c.z ? b.z : a.z);
}

//! Return c ? b : a
uint4 zSelectU4(const uint4 a, const uint4 b, const int4 c)
{
  return zMakeUInt4(c.x ? b.x : a.x,
                    c.y ? b.y : a.y,
                    c.z ? b.z : a.z,
                    c.w ? b.w : a.w);
}

//! Return c ? b : a
float zSelectF(const float a, const float b, const int32b c)
{
  return c ? b : a;
}

//! Return c ? b : a
float2 zSelectF2(const float2 a, const float2 b, const int2 c)
{
  return zMakeFloat2(c.x ? b.x : a.x,
                     c.y ? b.y : a.y);
}

//! Return c ? b : a
float3 zSelectF3(const float3 a, const float3 b, const int3 c)
{
  return zMakeFloat3(c.x ? b.x : a.x,
                     c.y ? b.y : a.y,
                     c.z ? b.z : a.z);
}

//! Return c ? b : a
float4 zSelectF4(const float4 a, const float4 b, const int4 c)
{
  return zMakeFloat4(c.x ? b.x : a.x,
                     c.y ? b.y : a.y,
                     c.z ? b.z : a.z,
                     c.w ? b.w : a.w);
}

#endif /* ZINVUL_RELATIONAL_CL */
