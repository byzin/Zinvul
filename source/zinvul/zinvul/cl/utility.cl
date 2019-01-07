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

//! Check a condition has false
int32b zHasFalse2(const int2 c)
{
  const int32b result = (c.x & c.y) == 0;
  return result;
}

//! Check a condition has false
int32b zHasFalse3(const int3 c)
{
  const int32b result = (c.x & c.y & c.z) == 0;
  return result;
}

//! Check a condition has false
int32b zHasFalse4(const int4 c)
{
  const int32b result = (c.x & c.y & c.z & c.w) == 0;
  return result;
}

//! Check a condition has true
int32b zHasTrue2(const int2 c)
{
  const int32b result = (c.x | c.y) != 0;
  return result;
}

//! Check a condition has true
int32b zHasTrue3(const int3 c)
{
  const int32b result = (c.x | c.y | c.z) != 0;
  return result;
}

//! Check a condition has true
int32b zHasTrue4(const int4 c)
{
  const int32b result = (c.x | c.y | c.z | c.w) != 0;
  return result;
}

#if defined(ZINVUL_CPU)
//! Write the result of the format to stdout
#define zPrint(format, value) printf(format, value)
#else
#define zPrint(format, value)
#endif

#if defined(Z_DEBUG_MODE) && defined(ZINVUL_CPU)
#define zAssert(condition, message) assertError(condition, message)
#define zAssert2(c, message) assertError(!zHasFalse2(c), message)
#define zAssert3(c, message) assertError(!zHasFalse3(c), message)
#define zAssert4(c, message) assertError(!zHasFalse4(c), message)
#else
#define zAssert(condition, message)
#define zAssert2(c, message)
#define zAssert3(c, message)
#define zAssert4(c, message)
#endif

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
#if defined(ZINVUL_CPU)
  const char2 value{x, y};
#else // ZINVUL_CPU
  const char2 value = (char2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
char3 zMakeChar3(const int8b x, const int8b y, const int8b z)
{
#if defined(ZINVUL_CPU)
  const char3 value{x, y, z};
#else // ZINVUL_CPU
  const char3 value = (char3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
char4 zMakeChar4(const int8b x, const int8b y, const int8b z, const int8b w)
{
#if defined(ZINVUL_CPU)
  const char4 value{x, y, z, w};
#else // ZINVUL_CPU
  const char4 value = (char4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
uchar2 zMakeUChar2(const uint8b x, const uint8b y)
{
#if defined(ZINVUL_CPU)
  const uchar2 value{x, y};
#else // ZINVUL_CPU
  const uchar2 value = (uchar2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
uchar3 zMakeUChar3(const uint8b x, const uint8b y, const uint8b z)
{
#if defined(ZINVUL_CPU)
  const uchar3 value{x, y, z};
#else // ZINVUL_CPU
  const uchar3 value = (uchar3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
uchar4 zMakeUChar4(const uint8b x, const uint8b y, const uint8b z, const uint8b w)
{
#if defined(ZINVUL_CPU)
  const uchar4 value{x, y, z, w};
#else // ZINVUL_CPU
  const uchar4 value = (uchar4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
short2 zMakeShort2(const int16b x, const int16b y)
{
#if defined(ZINVUL_CPU)
  const short2 value{x, y};
#else // ZINVUL_CPU
  const short2 value = (short2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
short3 zMakeShort3(const int16b x, const int16b y, const int16b z)
{
#if defined(ZINVUL_CPU)
  const short3 value{x, y, z};
#else // ZINVUL_CPU
  const short3 value = (short3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
short4 zMakeShort4(const int16b x, const int16b y, const int16b z, const int16b w)
{
#if defined(ZINVUL_CPU)
  const short4 value{x, y, z, w};
#else // ZINVUL_CPU
  const short4 value = (short4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
ushort2 zMakeUShort2(const uint16b x, const uint16b y)
{
#if defined(ZINVUL_CPU)
  const ushort2 value{x, y};
#else // ZINVUL_CPU
  const ushort2 value = (ushort2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
ushort3 zMakeUShort3(const uint16b x, const uint16b y, const uint16b z)
{
#if defined(ZINVUL_CPU)
  const ushort3 value{x, y, z};
#else // ZINVUL_CPU
  const ushort3 value = (ushort3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
ushort4 zMakeUShort4(const uint16b x, const uint16b y, const uint16b z, const uint16b w)
{
#if defined(ZINVUL_CPU)
  const ushort4 value{x, y, z, w};
#else // ZINVUL_CPU
  const ushort4 value = (ushort4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
int2 zMakeInt2(const int32b x, const int32b y)
{
#if defined(ZINVUL_CPU)
  const int2 value{x, y};
#else // ZINVUL_CPU
  const int2 value = (int2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
int3 zMakeInt3(const int32b x, const int32b y, const int32b z)
{
#if defined(ZINVUL_CPU)
  const int3 value{x, y, z};
#else // ZINVUL_CPU
  const int3 value = (int3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
int4 zMakeInt4(const int32b x, const int32b y, const int32b z, const int32b w)
{
#if defined(ZINVUL_CPU)
  const int4 value{x, y, z, w};
#else // ZINVUL_CPU
  const int4 value = (int4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
uint2 zMakeUInt2(const uint32b x, const uint32b y)
{
#if defined(ZINVUL_CPU)
  const uint2 value{x, y};
#else // ZINVUL_CPU
  const uint2 value = (uint2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
uint3 zMakeUInt3(const uint32b x, const uint32b y, const uint32b z)
{
#if defined(ZINVUL_CPU)
  const uint3 value{x, y, z};
#else // ZINVUL_CPU
  const uint3 value = (uint3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
uint4 zMakeUInt4(const uint32b x, const uint32b y, const uint32b z, const uint32b w)
{
#if defined(ZINVUL_CPU)
  const uint4 value{x, y, z, w};
#else // ZINVUL_CPU
  const uint4 value = (uint4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
long2 zMakeLong2(const int64b x, const int64b y)
{
#if defined(ZINVUL_CPU)
  const long2 value{x, y};
#else // ZINVUL_CPU
  const long2 value = (long2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
long3 zMakeLong3(const int64b x, const int64b y, const int64b z)
{
#if defined(ZINVUL_CPU)
  const long3 value{x, y, z};
#else // ZINVUL_CPU
  const long3 value = (long3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
long4 zMakeLong4(const int64b x, const int64b y, const int64b z, const int64b w)
{
#if defined(ZINVUL_CPU)
  const long4 value{x, y, z, w};
#else // ZINVUL_CPU
  const long4 value = (long4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
ulong2 zMakeULong2(const uint64b x, const uint64b y)
{
#if defined(ZINVUL_CPU)
  const ulong2 value{x, y};
#else // ZINVUL_CPU
  const ulong2 value = (ulong2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
ulong3 zMakeULong3(const uint64b x, const uint64b y, const uint64b z)
{
#if defined(ZINVUL_CPU)
  const ulong3 value{x, y, z};
#else // ZINVUL_CPU
  const ulong3 value = (ulong3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
ulong4 zMakeULong4(const uint64b x, const uint64b y, const uint64b z, const uint64b w)
{
#if defined(ZINVUL_CPU)
  const ulong4 value{x, y, z, w};
#else // ZINVUL_CPU
  const ulong4 value = (ulong4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
float2 zMakeFloat2(const float x, const float y)
{
#if defined(ZINVUL_CPU)
  const float2 value{x, y};
#else // ZINVUL_CPU
  const float2 value = (float2)(x, y);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
float3 zMakeFloat3(const float x, const float y, const float z)
{
#if defined(ZINVUL_CPU)
  const float3 value{x, y, z};
#else // ZINVUL_CPU
  const float3 value = (float3)(x, y, z);
#endif // ZINVUL_CPU
  return value;
}

/*!
  */
float4 zMakeFloat4(const float x, const float y, const float z, const float w)
{
#if defined(ZINVUL_CPU)
  const float4 value{x, y, z, w};
#else // ZINVUL_CPU
  const float4 value = (float4)(x, y, z, w);
#endif // ZINVUL_CPU
  return value;
}

//! Convert int2 to uint2
uint2 zI2ToU2(const int2 value)
{
  const uint2 result = zMakeUInt2((uint32b)value.x, (uint32b)value.y);
  return result;
}

//! Convert int3 to uint3
uint3 zI3ToU3(const int3 value)
{
  const uint3 result = zMakeUInt3((uint32b)value.x, (uint32b)value.y,
                                  (uint32b)value.z);
  return result;
}

//! Convert int4 to uint4
uint4 zI4ToU4(const int4 value)
{
  const uint4 result = zMakeUInt4((uint32b)value.x, (uint32b)value.y,
                                  (uint32b)value.z, (uint32b)value.w);
  return result;
}

//! Convert int2 to float2
float2 zI2ToF2(const int2 value)
{
  const float2 result = zMakeFloat2((float)value.x, (float)value.y);
  return result;
}

//! Convert int3 to float3
float3 zI3ToF3(const int3 value)
{
  const float3 result = zMakeFloat3((float)value.x, (float)value.y,
                                    (float)value.z);
  return result;
}

//! Convert int4 to float4
float4 zI4ToF4(const int4 value)
{
  const float4 result = zMakeFloat4((float)value.x, (float)value.y,
                                    (float)value.z, (float)value.w);
  return result;
}

//! Convert uint2 to int2
int2 zU2ToI2(const uint2 value)
{
  const int2 result = zMakeInt2((int32b)value.x, (int32b)value.y);
  return result;
}

//! Convert uint3 to int3
int3 zU3ToI3(const uint3 value)
{
  const int3 result = zMakeInt3((int32b)value.x, (int32b)value.y,
                                (int32b)value.z);
  return result;
}

//! Convert uint4 to int4
int4 zU4ToI4(const uint4 value)
{
  const int4 result = zMakeInt4((int32b)value.x, (int32b)value.y,
                                (int32b)value.z, (int32b)value.w);
  return result;
}

//! Convert uint2 to float2
float2 zU2ToF2(const uint2 value)
{
  const float2 result = zMakeFloat2((float)value.x, (float)value.y);
  return result;
}

//! Convert uint3 to float3
float3 zU3ToF3(const uint3 value)
{
  const float3 result = zMakeFloat3((float)value.x, (float)value.y,
                                    (float)value.z);
  return result;
}

//! Convert uint4 to float4
float4 zU4ToF4(const uint4 value)
{
  const float4 result = zMakeFloat4((float)value.x, (float)value.y,
                                    (float)value.z, (float)value.w);
  return result;
}

//! Convert float2 to int2
int2 zF2ToI2(const float2 value)
{
  const int2 result = zMakeInt2((int32b)value.x, (int32b)value.y);
  return result;
}

//! Convert float3 to int3
int3 zF3ToI3(const float3 value)
{
  const int3 result = zMakeInt3((int32b)value.x, (int32b)value.y,
                                (int32b)value.z);
  return result;
}

//! Convert float4 to int4
int4 zF4ToI4(const float4 value)
{
  const int4 result = zMakeInt4((int32b)value.x, (int32b)value.y,
                                (int32b)value.z, (int32b)value.w);
  return result;
}

//! Convert float2 to uint2
uint2 zF2ToU2(const float2 value)
{
  const uint2 result = zMakeUInt2((uint32b)value.x, (uint32b)value.y);
  return result;
}

//! Convert float3 to uint3
uint3 zF3ToU3(const float3 value)
{
  const uint3 result = zMakeUInt3((uint32b)value.x, (uint32b)value.y,
                                  (uint32b)value.z);
  return result;
}

//! Convert float4 to uint4
uint4 zF4ToU4(const float4 value)
{
  const uint4 result = zMakeUInt4((uint32b)value.x, (uint32b)value.y,
                                  (uint32b)value.z, (uint32b)value.w);
  return result;
}

//! Broadcast a int value
int2 zBroadcast2(const int32b value)
{
  const int2 result = zMakeInt2(value, value);
  return result;
}

//! Broadcast a int value
int3 zBroadcast3(const int32b value)
{
  const int3 result = zMakeInt3(value, value, value);
  return result;
}

//! Broadcast a int value
int4 zBroadcast4(const int32b value)
{
  const int4 result = zMakeInt4(value, value, value, value);
  return result;
}

//! Broadcast a uint value
uint2 zBroadcastU2(const uint32b value)
{
  const uint2 result = zMakeUInt2(value, value);
  return result;
}

//! Broadcast a uint value
uint3 zBroadcastU3(const uint32b value)
{
  const uint3 result = zMakeUInt3(value, value, value);
  return result;
}

//! Broadcast a uint value
uint4 zBroadcastU4(const uint32b value)
{
  const uint4 result = zMakeUInt4(value, value, value, value);
  return result;
}

//! Broadcast a float value
float2 zBroadcastF2(const float value)
{
  const float2 result = zMakeFloat2(value, value);
  return result;
}

//! Broadcast a float value
float3 zBroadcastF3(const float value)
{
  const float3 result = zMakeFloat3(value, value, value);
  return result;
}

//! Broadcast a float value
float4 zBroadcastF4(const float value)
{
  const float4 result = zMakeFloat4(value, value, value, value);
  return result;
}

#endif /* ZINVUL_UTILITY_CL */
