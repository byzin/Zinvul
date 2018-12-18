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
  const int32b result = c ? b : a;
  return result;
}

//! Return c ? b : a
int2 zSelect2(const int2 a, const int2 b, const int2 c)
{
  const int2 result = zMakeInt2(zSelect(a.x, b.x, c.x),
                                zSelect(a.y, b.y, c.y));
  return result;
}

//! Return c ? b : a
int3 zSelect3(const int3 a, const int3 b, const int3 c)
{
  const int3 result = zMakeInt3(zSelect(a.x, b.x, c.x),
                                zSelect(a.y, b.y, c.y),
                                zSelect(a.z, b.z, c.z));
  return result;
}

//! Return c ? b : a
int4 zSelect4(const int4 a, const int4 b, const int4 c)
{
  const int4 result = zMakeInt4(zSelect(a.x, b.x, c.x),
                                zSelect(a.y, b.y, c.y),
                                zSelect(a.z, b.z, c.z),
                                zSelect(a.w, b.w, c.w));
  return result;
}

//! Return c ? b : a
uint32b zSelectU(const uint32b a, const uint32b b, const int32b c)
{
  const uint32b result = c ? b : a;
  return result;
}

//! Return c ? b : a
uint2 zSelectU2(const uint2 a, const uint2 b, const int2 c)
{
  const uint2 result = zMakeUInt2(zSelectU(a.x, b.x, c.x),
                                  zSelectU(a.y, b.y, c.y));
  return result;
}

//! Return c ? b : a
uint3 zSelectU3(const uint3 a, const uint3 b, const int3 c)
{
  const uint3 result = zMakeUInt3(zSelectU(a.x, b.x, c.x),
                                  zSelectU(a.y, b.y, c.y),
                                  zSelectU(a.z, b.z, c.z));
  return result;
}

//! Return c ? b : a
uint4 zSelectU4(const uint4 a, const uint4 b, const int4 c)
{
  const uint4 result = zMakeUInt4(zSelectU(a.x, b.x, c.x),
                                  zSelectU(a.y, b.y, c.y),
                                  zSelectU(a.z, b.z, c.z),
                                  zSelectU(a.w, b.w, c.w));
  return result;
}

//! Return c ? b : a
float zSelectF(const float a, const float b, const int32b c)
{
  const float result = c ? b : a;
  return result;
}

//! Return c ? b : a
float2 zSelectF2(const float2 a, const float2 b, const int2 c)
{
  const float2 result = zMakeFloat2(zSelectF(a.x, b.x, c.x),
                                    zSelectF(a.y, b.y, c.y));
  return result;
}

//! Return c ? b : a
float3 zSelectF3(const float3 a, const float3 b, const int3 c)
{
  const float3 result = zMakeFloat3(zSelectF(a.x, b.x, c.x),
                                    zSelectF(a.y, b.y, c.y),
                                    zSelectF(a.z, b.z, c.z));
  return result;
}

//! Return c ? b : a
float4 zSelectF4(const float4 a, const float4 b, const int4 c)
{
  const float4 result = zMakeFloat4(zSelectF(a.x, b.x, c.x),
                                    zSelectF(a.y, b.y, c.y),
                                    zSelectF(a.z, b.z, c.z),
                                    zSelectF(a.w, b.w, c.w));
  return result;
}

#endif /* ZINVUL_RELATIONAL_CL */
