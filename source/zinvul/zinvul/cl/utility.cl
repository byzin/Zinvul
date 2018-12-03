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
uint32b zGetGlobalId(const uint32b dimension)
{
  const uint32b id = get_global_id(dimension);
  return id;
}

/*!
  */
uint32b zGetGlobalIdX()
{
  const uint32b id = get_global_id(0);
  return id;
}

/*!
  */
uint32b zGetGlobalIdY()
{
  const uint32b id = get_global_id(1);
  return id;
}

/*!
  */
uint32b zGetGlobalIdZ()
{
  const uint32b id = get_global_id(2);
  return id;
}

/*!
  */
char2 zMakeChar2(const int8b x, const int8b y)
{
#ifdef ZINVUL_CPP_CL
  const char2 value{x, y};
#else // ZINVUL_CPP_CL
  const char2 value = (char2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
char3 zMakeChar3(const int8b x, const int8b y, const int8b z)
{
#ifdef ZINVUL_CPP_CL
  const char3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const char3 value = (char3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
char4 zMakeChar4(const int8b x, const int8b y, const int8b z, const int8b w)
{
#ifdef ZINVUL_CPP_CL
  const char4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const char4 value = (char4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
uchar2 zMakeUChar2(const uint8b x, const uint8b y)
{
#ifdef ZINVUL_CPP_CL
  const uchar2 value{x, y};
#else // ZINVUL_CPP_CL
  const uchar2 value = (uchar2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
uchar3 zMakeUChar3(const uint8b x, const uint8b y, const uint8b z)
{
#ifdef ZINVUL_CPP_CL
  const uchar3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const uchar3 value = (uchar3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
uchar4 zMakeUChar4(const uint8b x, const uint8b y, const uint8b z, const uint8b w)
{
#ifdef ZINVUL_CPP_CL
  const uchar4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const uchar4 value = (uchar4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
short2 zMakeShort2(const int16b x, const int16b y)
{
#ifdef ZINVUL_CPP_CL
  const short2 value{x, y};
#else // ZINVUL_CPP_CL
  const short2 value = (short2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
short3 zMakeShort3(const int16b x, const int16b y, const int16b z)
{
#ifdef ZINVUL_CPP_CL
  const short3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const short3 value = (short3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
short4 zMakeShort4(const int16b x, const int16b y, const int16b z, const int16b w)
{
#ifdef ZINVUL_CPP_CL
  const short4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const short4 value = (short4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
ushort2 zMakeUShort2(const uint16b x, const uint16b y)
{
#ifdef ZINVUL_CPP_CL
  const ushort2 value{x, y};
#else // ZINVUL_CPP_CL
  const ushort2 value = (ushort2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
ushort3 zMakeUShort3(const uint16b x, const uint16b y, const uint16b z)
{
#ifdef ZINVUL_CPP_CL
  const ushort3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const ushort3 value = (ushort3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
ushort4 zMakeUShort4(const uint16b x, const uint16b y, const uint16b z, const uint16b w)
{
#ifdef ZINVUL_CPP_CL
  const ushort4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const ushort4 value = (ushort4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
int2 zMakeInt2(const int32b x, const int32b y)
{
#ifdef ZINVUL_CPP_CL
  const int2 value{x, y};
#else // ZINVUL_CPP_CL
  const int2 value = (int2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
int3 zMakeInt3(const int32b x, const int32b y, const int32b z)
{
#ifdef ZINVUL_CPP_CL
  const int3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const int3 value = (int3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
int4 zMakeInt4(const int32b x, const int32b y, const int32b z, const int32b w)
{
#ifdef ZINVUL_CPP_CL
  const int4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const int4 value = (int4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
uint2 zMakeUInt2(const uint32b x, const uint32b y)
{
#ifdef ZINVUL_CPP_CL
  const uint2 value{x, y};
#else // ZINVUL_CPP_CL
  const uint2 value = (uint2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
uint3 zMakeUInt3(const uint32b x, const uint32b y, const uint32b z)
{
#ifdef ZINVUL_CPP_CL
  const uint3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const uint3 value = (uint3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
uint4 zMakeUInt4(const uint32b x, const uint32b y, const uint32b z, const uint32b w)
{
#ifdef ZINVUL_CPP_CL
  const uint4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const uint4 value = (uint4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
long2 zMakeLong2(const int64b x, const int64b y)
{
#ifdef ZINVUL_CPP_CL
  const long2 value{x, y};
#else // ZINVUL_CPP_CL
  const long2 value = (long2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
long3 zMakeLong3(const int64b x, const int64b y, const int64b z)
{
#ifdef ZINVUL_CPP_CL
  const long3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const long3 value = (long3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
long4 zMakeLong4(const int64b x, const int64b y, const int64b z, const int64b w)
{
#ifdef ZINVUL_CPP_CL
  const long4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const long4 value = (long4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
ulong2 zMakeULong2(const uint64b x, const uint64b y)
{
#ifdef ZINVUL_CPP_CL
  const ulong2 value{x, y};
#else // ZINVUL_CPP_CL
  const ulong2 value = (ulong2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
ulong3 zMakeULong3(const uint64b x, const uint64b y, const uint64b z)
{
#ifdef ZINVUL_CPP_CL
  const ulong3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const ulong3 value = (ulong3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
ulong4 zMakeULong4(const uint64b x, const uint64b y, const uint64b z, const uint64b w)
{
#ifdef ZINVUL_CPP_CL
  const ulong4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const ulong4 value = (ulong4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
float2 zMakeFloat2(const float x, const float y)
{
#ifdef ZINVUL_CPP_CL
  const float2 value{x, y};
#else // ZINVUL_CPP_CL
  const float2 value = (float2)(x, y);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
float3 zMakeFloat3(const float x, const float y, const float z)
{
#ifdef ZINVUL_CPP_CL
  const float3 value{x, y, z};
#else // ZINVUL_CPP_CL
  const float3 value = (float3)(x, y, z);
#endif // ZINVUL_CPP_CL
  return value;
}

/*!
  */
float4 zMakeFloat4(const float x, const float y, const float z, const float w)
{
#ifdef ZINVUL_CPP_CL
  const float4 value{x, y, z, w};
#else // ZINVUL_CPP_CL
  const float4 value = (float4)(x, y, z, w);
#endif // ZINVUL_CPP_CL
  return value;
}

#endif /* ZINVUL_UTILITY_CL */
