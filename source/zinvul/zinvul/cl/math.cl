/*!
  \file math.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_CL
#define ZINVUL_MATH_CL

// Zinvul
#include "math_const.cl"
#include "types.cl"

namespace zinvul {

// Constant values

//! Circular constant
template <typename Float>
constexpr Constant<Float> kPi = mathconst::Math::pi<Float>();

//// Constants
//#define zCharMax 127
//#define zCharMin (-zCharMax - 1)
//#define zUCharMax 255u
//#define zShortMax 32767
//#define zShortMin (-zShortMax - 1)
//#define zUShortMax 65535u
//#define zIntMax 2147483647
//#define zIntMin (-zIntMax - 1)
//#define zUIntMax 0xffffffffu
//#define zLongMax 0x7fffffffffffffffl
//#define zLongMin (-zLongMax - 1l)
//#define zULongMax 0xfffffffffffffffful
//
//#define zFloatMax 3.402823466e+38f
//#define zFloatMin 1.175494351e-38f
//#define zFloatEpsilon 1.192092896e-07f
//#define zFloatDenormMax 1.175494211e-38f
//#define zFloatDenormMin 1.401298464e-45f
//#define zEF 2.718281746e+00f //!< The value of 'e'
//#define zInvEF 3.678794503e-01f //!< The value of '1 / e'
//#define zLog2EF 1.442695022e+00f //!< The value of 'log2(e)'
//#define zInvLog2EF 6.931471825e-01f //!< The value of '1 / log2(e)'
//#define zLog10EF 4.342944920e-01f //!< The value of 'log10(e)'
//#define zInvLog10EF 2.302585125e+00f //!< The value of '1 / log10(e)'
//#define zLn2F 6.931471825e-01f //!< The value of 'log(2)'
//#define zInvLn2F 1.442695022e+00f //!< The value of '1 / log(2)'
//#define zLn10F 2.302585125e+00f //!< The value of 'log(10)'
//#define zInvLn10F 4.342944920e-01f //!< The value of '1 / log(10)'
//#define zPiF 3.141592741e+00f //!< The value of 'pi'
//#define zPi2F 1.570796371e+00f //!< The value of 'pi / 2'
//#define zPi4F 7.853981853e-01f //!< The value of 'pi / 4'
//#define zInvPiF 3.183098733e-01f //!< The value of '1 / pi'
//#define z2InvPiF 6.366197467e-01f //!< The value of '2 / pi'
//#define z2InvSqrtPiF 1.128379226e+00f //!< The value of '2 / sqrt(pi)'
//#define zSqrt2F 1.414213538e+00f //!< The value of 'sqrt(2)'
//#define zInvSqrt2F 7.071067691e-01f //!< The value of '1/sqrt(2)'
//#define zSqrt3F 1.732050776e+00f //!< The value of 'sqrt(3)'
//#define zInvSqrt3F 5.773502588e-01f //!< The value of '1/sqrt(3)'
//
//#define zSignBitMaskF 0x80000000u
//#define zExponentBitMaskF 0x7f800000u
//#define zExponentBitSizeF 8
//#define zExponentBiasF 127
//#define zSignificandBitMaskF 0x7fffffu
//#define zSignificandBitSizeF 23
//
////! Return c ? b : a
//#define ZINVUL_SELECT_IMPL(a, b, c) (c ? b : a)
//
//#if defined(ZINVUL_CPU)
//
////! Return c ? b : a
//#define ZINVUL_SELECT2_IMPL(makeVec2, a, b, c) \
//    makeVec2(ZINVUL_SELECT_IMPL(a.x, b.x, c.x), \
//             ZINVUL_SELECT_IMPL(a.y, b.y, c.y))
//
////! Return c ? b : a
//#define ZINVUL_SELECT3_IMPL(makeVec3, a, b, c) \
//    makeVec3(ZINVUL_SELECT_IMPL(a.x, b.x, c.x), \
//             ZINVUL_SELECT_IMPL(a.y, b.y, c.y), \
//             ZINVUL_SELECT_IMPL(a.z, b.z, c.z))
//
////! Return c ? b : a
//#define ZINVUL_SELECT4_IMPL(makeVec4, a, b, c) \
//    makeVec4(ZINVUL_SELECT_IMPL(a.x, b.x, c.x), \
//             ZINVUL_SELECT_IMPL(a.y, b.y, c.y), \
//             ZINVUL_SELECT_IMPL(a.z, b.z, c.z), \
//             ZINVUL_SELECT_IMPL(a.w, b.w, c.w))
//
//#else // ZINVUL_CPU
//
////! Return c ? b : a
//#define ZINVUL_SELECT2_IMPL(makeVec2, a, b, c) ZINVUL_SELECT_IMPL(a, b, c)
//
////! Return c ? b : a
//#define ZINVUL_SELECT3_IMPL(makeVec3, a, b, c) ZINVUL_SELECT_IMPL(a, b, c)
//
////! Return c ? b : a
//#define ZINVUL_SELECT4_IMPL(makeVec4, a, b, c) ZINVUL_SELECT_IMPL(a, b, c)
//
//#endif // ZINVUL_CPU
//
////! Return c ? b : a
//int32b zSelectImpl(const int32b a, const int32b b, const int32b c)
//{
//  const int32b result = ZINVUL_SELECT_IMPL(a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//int32b zSelect(const int32b a, const int32b b, const int32b c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const int32b result = select(a, b, c)
//#else 
//  const int32b result = zSelectImpl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//int2 zSelect2Impl(const int2 a, const int2 b, const int2 c)
//{
//  const int2 result = ZINVUL_SELECT2_IMPL(zMakeInt2, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//int2 zSelect2(const int2 a, const int2 b, const int2 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const int2 result = select(a, b, c);
//#else
//  const int2 result = zSelect2Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//int3 zSelect3Impl(const int3 a, const int3 b, const int3 c)
//{
//  const int3 result = ZINVUL_SELECT3_IMPL(zMakeInt3, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//int3 zSelect3(const int3 a, const int3 b, const int3 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const int3 result = select(a, b, c);
//#else
//  const int3 result = zSelect3Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//int4 zSelect4Impl(const int4 a, const int4 b, const int4 c)
//{
//  const int4 result = ZINVUL_SELECT4_IMPL(zMakeInt4, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//int4 zSelect4(const int4 a, const int4 b, const int4 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const int4 result = select(a, b, c);
//#else
//  const int4 result = zSelect4Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//uint32b zSelectUImpl(const uint32b a, const uint32b b, const int32b c)
//{
//  const uint32b result = ZINVUL_SELECT_IMPL(a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//uint32b zSelectU(const uint32b a, const uint32b b, const int32b c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const uint32b result = select(a, b, c);
//#else
//  const uint32b result = zSelectUImpl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//uint2 zSelectU2Impl(const uint2 a, const uint2 b, const int2 c)
//{
//  const uint2 result = ZINVUL_SELECT2_IMPL(zMakeUInt2, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//uint2 zSelectU2(const uint2 a, const uint2 b, const int2 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const uint2 result = select(a, b, c);
//#else
//  const uint2 result = zSelectU2Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//uint3 zSelectU3Impl(const uint3 a, const uint3 b, const int3 c)
//{
//  const uint3 result = ZINVUL_SELECT3_IMPL(zMakeUInt3, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//uint3 zSelectU3(const uint3 a, const uint3 b, const int3 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const uint3 result = select(a, b, c);
//#else
//  const uint3 result = zSelectU3Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//uint4 zSelectU4Impl(const uint4 a, const uint4 b, const int4 c)
//{
//  const uint4 result = ZINVUL_SELECT4_IMPL(zMakeUInt4, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//uint4 zSelectU4(const uint4 a, const uint4 b, const int4 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const uint4 result = select(a, b, c);
//#else
//  const uint4 result = zSelectU4Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//float zSelectFImpl(const float a, const float b, const int32b c)
//{
//  const float result = ZINVUL_SELECT_IMPL(a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//float2 zSelectF2Impl(const float2 a, const float2 b, const int2 c)
//{
//  const float2 result = ZINVUL_SELECT2_IMPL(zMakeFloat2, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//float3 zSelectF3Impl(const float3 a, const float3 b, const int3 c)
//{
//  const float3 result = ZINVUL_SELECT3_IMPL(zMakeFloat3, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//float4 zSelectF4Impl(const float4 a, const float4 b, const int4 c)
//{
//  const float4 result = ZINVUL_SELECT4_IMPL(zMakeFloat4, a, b, c);
//  return result;
//}
//
////! Return c ? b : a
//float zSelectF(const float a, const float b, const int32b c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const float result = select(a, b, c);
//#else
//  const float result = zSelectFImpl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//float2 zSelectF2(const float2 a, const float2 b, const int2 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const float2 result = select(a, b, c);
//#else
//  const float2 result = zSelectF2Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//float3 zSelectF3(const float3 a, const float3 b, const int3 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const float3 result = select(a, b, c);
//#else
//  const float3 result = zSelectF3Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return c ? b : a
//float4 zSelectF4(const float4 a, const float4 b, const int4 c)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SELECT)
//  const float4 result = select(a, b, c);
//#else
//  const float4 result = zSelectF4Impl(a, b, c);
//#endif
//  return result;
//}
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//#define ZINVUL_SIGN_IMPL(FType, selectF, broadcastF, x) \
//  FType y = selectF(broadcastF(0.0f), broadcastF(1.0f), 0.0f < x); \
//  y = selectF(y, broadcastF(-1.0f), x < 0.0f)
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//float zSignImpl(const float x)
//{
//  ZINVUL_SIGN_IMPL(float, zSelectF, , x);
//  return y;
//}
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//float2 zSign2Impl(const float2 x)
//{
//  ZINVUL_SIGN_IMPL(float2, zSelectF2, zBroadcastF2, x);
//  return y;
//}
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//float3 zSign3Impl(const float3 x)
//{
//  ZINVUL_SIGN_IMPL(float3, zSelectF3, zBroadcastF3, x);
//  return y;
//}
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//float4 zSign4Impl(const float4 x)
//{
//  ZINVUL_SIGN_IMPL(float4, zSelectF4, zBroadcastF4, x);
//  return y;
//}
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//float zSign(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SIGN)
//  const float y = sign(x);
//#else
//  const float y = zSignImpl(x);
//#endif
//  return y;
//}
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//float2 zSign2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SIGN)
//  const float2 y = sign(x);
//#else
//  const float2 y = zSign2Impl(x);
//#endif
//  return y;
//}
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//float3 zSign3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SIGN)
//  const float3 y = sign(x);
//#else
//  const float3 y = zSign3Impl(x);
//#endif
//  return y;
//}
//
////! Return 1 if x > 0, -1 if x < 0, otherwise 0
//float4 zSign4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SIGN)
//  const float4 y = sign(x);
//#else
//  const float4 y = zSign4Impl(x);
//#endif
//  return y;
//}
//
//// Nearest integer floating point operations
//
////! Return the nearest integer not less than the given value. \todo Implement
//#define ZINVUL_ICEIL_IMPL(x) ceil(x)
//
////! Return the nearest integer not less than the given value
//int32b zICeilImpl(const float x)
//{
//  const float f = ZINVUL_ICEIL_IMPL(x);
//  const int32b y = (int32b)f;
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//int2 zICeil2Impl(const float2 x)
//{
//  const float2 f = ZINVUL_ICEIL_IMPL(x);
//  const int2 y = zF2ToI2(f);
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//int3 zICeil3Impl(const float3 x)
//{
//  const float3 f = ZINVUL_ICEIL_IMPL(x);
//  const int3 y = zF3ToI3(f);
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//int4 zICeil4Impl(const float4 x)
//{
//  const float4 f = ZINVUL_ICEIL_IMPL(x);
//  const int4 y = zF4ToI4(f);
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//int32b zICeil(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float f = ceil(x);
//  const int32b y = (int32b)f;
//#else
//  const int32b y = zICeilImpl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//int2 zICeil2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float2 f = ceil(x);
//  const int2 y = zF2ToI2(f);
//#else
//  const int2 y = zICeil2Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//int3 zICeil3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float3 f = ceil(x);
//  const int3 y = zF3ToI3(f);
//#else
//  const int3 y = zICeil3Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//int4 zICeil4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float4 f = ceil(x);
//  const int4 y = zF4ToI4(f);
//#else
//  const int4 y = zICeil4Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not less than the given value. \todo Implement
//#define ZINVUL_CEIL_IMPL(x) ceil(x)
//
////! Return the nearest integer not less than the given value
//float zCeilImpl(const float x)
//{
//  const float y = ZINVUL_CEIL_IMPL(x);
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//float2 zCeil2Impl(const float2 x)
//{
//  const float2 y = ZINVUL_CEIL_IMPL(x);
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//float3 zCeil3Impl(const float3 x)
//{
//  const float3 y = ZINVUL_CEIL_IMPL(x);
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//float4 zCeil4Impl(const float4 x)
//{
//  const float4 y = ZINVUL_CEIL_IMPL(x);
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//float zCeil(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float y = ceil(x);
//#else
//  const float y = zCeilImpl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//float2 zCeil2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float2 y = ceil(x);
//#else
//  const float2 y = zCeil2Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//float3 zCeil3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float3 y = ceil(x);
//#else
//  const float3 y = zCeil3Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not less than the given value
//float4 zCeil4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float4 y = ceil(x);
//#else
//  const float4 y = zCeil4Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater than the given value. \todo Implement
//#define ZINVUL_IFLOOR_IMPL(x) floor(x)
//
////! Return the nearest integer not greater than the given value
//int32b zIFloorImpl(const float x)
//{
//  const float f = ZINVUL_IFLOOR_IMPL(x);
//  const int32b y = (int32b)f;
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//int2 zIFloor2Impl(const float2 x)
//{
//  const float2 f = ZINVUL_IFLOOR_IMPL(x);
//  const int2 y = zF2ToI2(f);
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//int3 zIFloor3Impl(const float3 x)
//{
//  const float3 f = ZINVUL_IFLOOR_IMPL(x);
//  const int3 y = zF3ToI3(f);
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//int4 zIFloor4Impl(const float4 x)
//{
//  const float4 f = ZINVUL_IFLOOR_IMPL(x);
//  const int4 y = zF4ToI4(f);
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//int32b zIFloor(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float f = floor(x);
//  const int32b y = (int32b)f;
//#else
//  const int32b y = zIFloorImpl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//int2 zIFloor2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float2 f = floor(x);
//  const int2 y = zF2ToI2(f);
//#else
//  const int2 y = zIFloor2Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//int3 zIFloor3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float3 f = floor(x);
//  const int3 y = zF3ToI3(f);
//#else
//  const int3 y = zIFloor3Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//int4 zIFloor4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float4 y = floor(x);
//#else
//  const float4 y = ZINVUL_IFLOOR_IMPL(x);
//#endif
//  return zF4ToI4(y);
//}
//
////! Return the nearest integer not greater than the given value. \todo Implement
//#define ZINVUL_FLOOR_IMPL(x) floor(x)
//
////! Return the nearest integer not greater than the given value
//float zFloorImpl(const float x)
//{
//  const float y = ZINVUL_FLOOR_IMPL(x);
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//float2 zFloor2Impl(const float2 x)
//{
//  const float2 y = ZINVUL_FLOOR_IMPL(x);
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//float3 zFloor3Impl(const float3 x)
//{
//  const float3 y = ZINVUL_FLOOR_IMPL(x);
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//float4 zFloor4Impl(const float4 x)
//{
//  const float4 y = ZINVUL_FLOOR_IMPL(x);
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//float zFloor(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float y = floor(x);
//#else
//  const float y = zFloorImpl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//float2 zFloor2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float2 y = floor(x);
//#else
//  const float2 y = zFloor2Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//float3 zFloor3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float3 y = floor(x);
//#else
//  const float3 y = zFloor3Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater than the given value
//float4 zFloor4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float4 y = floor(x);
//#else
//  const float4 y = zFloor4Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//int32b zITrunc(const float x)
//{
//  return (int32b)x;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//int2 zITrunc2(const float2 x)
//{
//  return zF2ToI2(x);
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//int3 zITrunc3(const float3 x)
//{
//  return zF3ToI3(x);
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//int4 zITrunc4(const float4 x)
//{
//  return zF4ToI4(x);
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//#define ZINVUL_TRUNC_IMPL(FType, iToF, selectF, iTrunc, x) \
//  FType y = iToF(iTrunc(x)); \
//  y = selectF(y, x, isinf(x) || isnan(x))
//
////! Return the nearest integer not greater in magnitude than the given value
//float zTruncImpl(const float x)
//{
//  ZINVUL_TRUNC_IMPL(float, (float), zSelectF, zITrunc, x);
//  return y;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//float2 zTrunc2Impl(const float2 x)
//{
//  ZINVUL_TRUNC_IMPL(float2, zI2ToF2, zSelectF2, zITrunc2, x);
//  return y;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//float3 zTrunc3Impl(const float3 x)
//{
//  ZINVUL_TRUNC_IMPL(float3, zI3ToF3, zSelectF3, zITrunc3, x);
//  return y;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//float4 zTrunc4Impl(const float4 x)
//{
//  ZINVUL_TRUNC_IMPL(float4, zI4ToF4, zSelectF4, zITrunc4, x);
//  return y;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//float zTrunc(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float y = trunc(x);
//#else
//  const float y = zTruncImpl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//float2 zTrunc2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float2 y = trunc(x);
//#else
//  const float2 y = zTrunc2Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//float3 zTrunc3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float3 y = trunc(x);
//#else
//  const float3 y = zTrunc3Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer not greater in magnitude than the given value
//float4 zTrunc4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float4 y = trunc(x);
//#else
//  const float4 y = zTrunc4Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//#define ZINVUL_IROUND_IMPL(getSign, x) (getSign(x) * 0.5f + x);
//
////! Return the nearest integer, rounding away from zero in halfway cases
//int32b zIRoundImpl(const float x)
//{
//  const float f = ZINVUL_IROUND_IMPL(zSign, x);
//  const int32b y = (int32b)f;
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//int2 zIRound2Impl(const float2 x)
//{
//  const float2 f = ZINVUL_IROUND_IMPL(zSign2, x);
//  const int2 y = zF2ToI2(f);
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//int3 zIRound3Impl(const float3 x)
//{
//  const float3 f = ZINVUL_IROUND_IMPL(zSign3, x);
//  const int3 y = zF3ToI3(f);
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//int4 zIRound4Impl(const float4 x)
//{
//  const float4 f = ZINVUL_IROUND_IMPL(zSign4, x);
//  const int4 y = zF4ToI4(f);
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//int32b zIRound(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float f = round(x);
//  const int32b y = (int32b)f;
//#else
//  const int32b y = zIRoundImpl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//int2 zIRound2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float2 f = round(x);
//  const int2 y = zF2ToI2(f);
//#else
//  const int2 y = zIRound2Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//int3 zIRound3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float3 f = round(x);
//  const int3 y = zF3ToI3(f);
//#else
//  const int3 y = zIRound3Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//int4 zIRound4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float4 f = round(x);
//  const int4 y = zF4ToI4(f);
//#else
//  const int4 y = zIRound4Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//#define ZINVUL_ROUND_IMPL(FType, iToF, selectF, iRound, x) \
//  FType y = iToF(iRound(x)); \
//  y = selectF(y, x, isinf(x) || isnan(x))
//
////! Return the nearest integer, rounding away from zero in halfway cases
//float zRoundImpl(const float x)
//{
//  ZINVUL_ROUND_IMPL(float, (float), zSelectF, zIRound, x);
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//float2 zRound2Impl(const float2 x)
//{
//  ZINVUL_ROUND_IMPL(float2, zI2ToF2, zSelectF2, zIRound2, x);
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//float3 zRound3Impl(const float3 x)
//{
//  ZINVUL_ROUND_IMPL(float3, zI3ToF3, zSelectF3, zIRound3, x);
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//float4 zRound4Impl(const float4 x)
//{
//  ZINVUL_ROUND_IMPL(float4, zI4ToF4, zSelectF4, zIRound4, x);
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//float zRound(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float y = round(x);
//#else
//  const float y = zRoundImpl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//float2 zRound2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float2 y = round(x);
//#else
//  const float2 y = zRound2Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//float3 zRound3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float3 y = round(x);
//#else
//  const float3 y = zRound3Impl(x);
//#endif
//  return y;
//}
//
////! Return the nearest integer, rounding away from zero in halfway cases
//float4 zRound4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
//  const float4 y = round(x);
//#else
//  const float4 y = zRound4Impl(x);
//#endif
//  return y;
//}
//
//// Basic operations
//
////! Return |x|
//uint32b zAbsImpl(const int32b x)
//{
//  const uint32b result = (uint32b)zSelect(x, -x, x < 0);
//  return result;
//}
//
////! Return |x|
//uint2 zAbs2Impl(const int2 x)
//{
//  const uint2 result = zI2ToU2(zSelect2(x, -x, x < 0));
//  return result;
//}
//
////! Return |x|
//uint3 zAbs3Impl(const int3 x)
//{
//  const uint3 result = zI3ToU3(zSelect3(x, -x, x < 0));
//  return result;
//}
//
////! Return |x|
//uint4 zAbs4Impl(const int4 x)
//{
//  const uint4 result = zI4ToU4(zSelect4(x, -x, x < 0));
//  return result;
//}
//
////! Return |x|
//uint32b zAbs(const int32b x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
//  const uint32b result = abs(x);
//#else
//  const uint32b result = zAbsImpl(x);
//#endif
//  return result;
//}
//
////! Return |x|
//uint2 zAbs2(const int2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
//  const uint2 result = abs(x);
//#else
//  const uint2 result = zAbs2Impl(x);
//#endif
//  return result;
//}
//
////! Return |x|
//uint3 zAbs3(const int3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
//  const uint3 result = abs(x);
//#else
//  const uint3 result = zAbs3Impl(x);
//#endif
//  return result;
//}
//
////! Return |x|
//uint4 zAbs4(const int4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
//  const uint4 result = abs(x);
//#else
//  const uint4 result = zAbs4Impl(x);
//#endif
//  return result;
//}
//
////! Return |x|
//float zAbsFImpl(const float x)
//{
//  const float result = zSelectF(x, -x, x < 0.0f);
//  return result;
//}
//
////! Return |x|
//float2 zAbsF2Impl(const float2 x)
//{
//  const float2 result = zSelectF2(x, -x, x < 0.0f);
//  return result;
//}
//
////! Return |x|
//float3 zAbsF3Impl(const float3 x)
//{
//  const float3 result = zSelectF3(x, -x, x < 0.0f);
//  return result;
//}
//
////! Return |x|
//float4 zAbsF4Impl(const float4 x)
//{
//  const float4 result = zSelectF4(x, -x, x < 0.0f);
//  return result;
//}
//
////! Return |x|
//float zAbsF(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
//  const float result = fabs(x);
//#else
//  const float result = zAbsFImpl(x);
//#endif
//  return result;
//}
//
////! Return |x|
//float2 zAbsF2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
//  const float2 result = fabs(x);
//#else
//  const float2 result = zAbsF2Impl(x);
//#endif
//  return result;
//}
//
////! Return |x|
//float3 zAbsF3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
//  const float3 result = fabs(x);
//#else
//  const float3 result = zAbsF3Impl(x);
//#endif
//  return result;
//}
//
////! Return |x|
//float4 zAbsF4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
//  const float4 result = fabs(x);
//#else
//  const float4 result = zAbsF4Impl(x);
//#endif
//  return result;
//}
//
////! Check if the given two values are almost same. \todo Support subnormal
//#define ZINVUL_IS_ALMOST_SAME_IMPL(FType, IType, absolute, lhs, rhs) \
//  const FType e = (4.0f * zFloatEpsilon) * absolute(lhs + rhs); \
//  const FType v = absolute(lhs - rhs); \
//  const IType result = (v <= e); \
//
////! Check if the given two values are almost same
//int32b zIsAlmostSame(const float lhs, const float rhs)
//{
//  ZINVUL_IS_ALMOST_SAME_IMPL(float, int32b, zAbsF, lhs, rhs);
//  return result;
//}
//
////! Check if the given two values are almost same
//int2 zIsAlmostSame2(const float2 lhs, const float2 rhs)
//{
//  ZINVUL_IS_ALMOST_SAME_IMPL(float2, int2, zAbsF2, lhs, rhs);
//  return result;
//}
//
////! Check if the given two values are almost same
//int3 zIsAlmostSame3(const float3 lhs, const float3 rhs)
//{
//  ZINVUL_IS_ALMOST_SAME_IMPL(float3, int3, zAbsF3, lhs, rhs);
//  return result;
//}
//
////! Check if the given two values are almost same
//int4 zIsAlmostSame4(const float4 lhs, const float4 rhs)
//{
//  ZINVUL_IS_ALMOST_SAME_IMPL(float4, int4, zAbsF4, lhs, rhs);
//  return result;
//}
//
////! Check if the given value is odd
//int32b zIsOdd(const int32b value)
//{
//  const int32b result = (value & 1) == 1;
//  return result;
//}
//
////! Check if the given value is odd
//int2 zIsOdd2(const int2 value)
//{
//  const int2 result = (value & 1) == 1;
//  return result;
//}
//
////! Check if the given value is odd
//int3 zIsOdd3(const int3 value)
//{
//  const int3 result = (value & 1) == 1;
//  return result;
//}
//
////! Check if the given value is odd
//int4 zIsOdd4(const int4 value)
//{
//  const int4 result = (value & 1) == 1;
//  return result;
//}

//! Check if the given value is odd
template <typename IntegerN>
auto isOdd(const IntegerN value) noexcept;

////! Check if the given value is odd
//int32b zIsOddU(const uint32b value)
//{
//  const int32b result = (value & 1u) == 1u;
//  return result;
//}
//
////! Check if the given value is odd
//int2 zIsOddU2(const uint2 value)
//{
//  const int2 result = (value & 1u) == 1u;
//  return result;
//}
//
////! Check if the given value is odd
//int3 zIsOddU3(const uint3 value)
//{
//  const int3 result = (value & 1u) == 1u;
//  return result;
//}
//
////! Check if the given value is odd
//int4 zIsOddU4(const uint4 value)
//{
//  const int4 result = (value & 1u) == 1u;
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//int32b zMaxImpl(const int32b x, const int32b y)
//{
//  const int32b result = zSelect(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//int2 zMax2Impl(const int2 x, const int2 y)
//{
//  const int2 result = zSelect2(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//int3 zMax3Impl(const int3 x, const int3 y)
//{
//  const int3 result = zSelect3(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//int4 zMax4Impl(const int4 x, const int4 y)
//{
//  const int4 result = zSelect4(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//int32b zMax(const int32b x, const int32b y)
//{
//  const int32b result = zMaxImpl(x, y);
//  return result;
//}
//
//
////! Return y if x < y, otherwise it returns x
//int2 zMax2(const int2 x, const int2 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const int2 result = max(x, y);
//#else
//  const int2 result = zMax2Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//int3 zMax3(const int3 x, const int3 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const int3 result = max(x, y);
//#else
//  const int3 result = zMax3Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//int4 zMax4(const int4 x, const int4 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const int4 result = max(x, y);
//#else
//  const int4 result = zMax4Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//uint32b zMaxUImpl(const uint32b x, const uint32b y)
//{
//  const uint32b result = zSelectU(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//uint2 zMaxU2Impl(const uint2 x, const uint2 y)
//{
//  const uint2 result = zSelectU2(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//uint3 zMaxU3Impl(const uint3 x, const uint3 y)
//{
//  const uint3 result = zSelectU3(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//uint4 zMaxU4Impl(const uint4 x, const uint4 y)
//{
//  const uint4 result = zSelectU4(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//uint32b zMaxU(const uint32b x, const uint32b y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const uint32b result = max(x, y);
//#else
//  const uint32b result = zMaxUImpl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//uint2 zMaxU2(const uint2 x, const uint2 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const uint2 result = max(x, y);
//#else
//  const uint2 result = zMaxU2Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//uint3 zMaxU3(const uint3 x, const uint3 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const uint3 result = max(x, y);
//#else
//  const uint3 result = zMaxU3Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//uint4 zMaxU4(const uint4 x, const uint4 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const uint4 result = max(x, y);
//#else
//  const uint4 result = zMaxU4Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//float zMaxFImpl(const float x, const float y)
//{
//  const float result = zSelectF(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//float2 zMaxF2Impl(const float2 x, const float2 y)
//{
//  const float2 result = zSelectF2(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//float3 zMaxF3Impl(const float3 x, const float3 y)
//{
//  const float3 result = zSelectF3(x, y, x < y);
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//float4 zMaxF4Impl(const float4 x, const float4 y)
//{
//  const float4 result = zSelectF4(x, y, x < y);
//  return result;
//}
////! Return y if x < y, otherwise it returns x
//float zMaxF(const float x, const float y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const float result = max(x, y);
//#else
//  const float result = zMaxFImpl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//float2 zMaxF2(const float2 x, const float2 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const float2 result = max(x, y);
//#else
//  const float2 result = zMaxF2Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//float3 zMaxF3(const float3 x, const float3 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const float3 result = max(x, y);
//#else
//  const float3 result = zMaxF3Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if x < y, otherwise it returns x
//float4 zMaxF4(const float4 x, const float4 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const float4 result = max(x, y);
//#else
//  const float4 result = zMaxF4Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//int32b zMinImpl(const int32b x, const int32b y)
//{
//  const int32b result = zSelect(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//int2 zMin2Impl(const int2 x, const int2 y)
//{
//  const int2 result = zSelect2(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//int3 zMin3Impl(const int3 x, const int3 y)
//{
//  const int3 result = zSelect3(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//int4 zMin4Impl(const int4 x, const int4 y)
//{
//  const int4 result = zSelect4(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//int32b zMin(const int32b x, const int32b y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const int32b result = min(x, y);
//#else
//  const int32b result = zMinImpl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//int2 zMin2(const int2 x, const int2 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const int2 result = min(x, y);
//#else
//  const int2 result = zMin2Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//int3 zMin3(const int3 x, const int3 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const int3 result = min(x, y);
//#else
//  const int3 result = zMin3Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//int4 zMin4(const int4 x, const int4 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const int4 result = min(x, y);
//#else
//  const int4 result = zMin4Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//uint32b zMinUImpl(const uint32b x, const uint32b y)
//{
//  const uint32b result = zSelectU(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//uint2 zMinU2Impl(const uint2 x, const uint2 y)
//{
//  const uint2 result = zSelectU2(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//uint3 zMinU3Impl(const uint3 x, const uint3 y)
//{
//  const uint3 result = zSelectU3(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//uint4 zMinU4Impl(const uint4 x, const uint4 y)
//{
//  const uint4 result = zSelectU4(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//uint32b zMinU(const uint32b x, const uint32b y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const uint32b result = min(x, y);
//#else
//  const uint32b result = zMinUImpl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//uint2 zMinU2(const uint2 x, const uint2 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const uint2 result = min(x, y);
//#else
//  const uint2 result = zMinU2Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//uint3 zMinU3(const uint3 x, const uint3 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const uint3 result = min(x, y);
//#else
//  const uint3 result = zMinU3Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//uint4 zMinU4(const uint4 x, const uint4 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const uint4 result = min(x, y);
//#else
//  const uint4 result = zMinU4Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//float zMinFImpl(const float x, const float y)
//{
//  const float result = zSelectF(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//float2 zMinF2Impl(const float2 x, const float2 y)
//{
//  const float2 result = zSelectF2(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//float3 zMinF3Impl(const float3 x, const float3 y)
//{
//  const float3 result = zSelectF3(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//float4 zMinF4Impl(const float4 x, const float4 y)
//{
//  const float4 result = zSelectF4(x, y, y < x);
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//float zMinF(const float x, const float y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const float result = min(x, y);
//#else
//  const float result = zMinFImpl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//float2 zMinF2(const float2 x, const float2 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const float2 result = min(x, y);
//#else
//  const float2 result = zMinF2Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//float3 zMinF3(const float3 x, const float3 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const float3 result = min(x, y);
//#else
//  const float3 result = zMinF3Impl(x, y);
//#endif
//  return result;
//}
//
////! Return y if y < x, otherwise it returns x
//float4 zMinF4(const float4 x, const float4 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
//  const float4 result = min(x, y);
//#else
//  const float4 result = zMinF4Impl(x, y);
//#endif
//  return result;
//}
//
//#define ZINVUL_MIX_IMPL(x, y, a) (x + a * (y - x))
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float zMixImpl(const float x, const float y, const float a)
//{
//  const float result = ZINVUL_MIX_IMPL(x, y, a);
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float2 zMix2Impl(const float2 x, const float2 y, const float2 a)
//{
//  const float2 result = ZINVUL_MIX_IMPL(x, y, a);
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float2 zMix2FImpl(const float2 x, const float2 y, const float a)
//{
//  const float2 result = ZINVUL_MIX_IMPL(x, y, a);
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float3 zMix3Impl(const float3 x, const float3 y, const float3 a)
//{
//  const float3 result = ZINVUL_MIX_IMPL(x, y, a);
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float3 zMix3FImpl(const float3 x, const float3 y, const float a)
//{
//  const float3 result = ZINVUL_MIX_IMPL(x, y, a);
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float4 zMix4Impl(const float4 x, const float4 y, const float4 a)
//{
//  const float4 result = ZINVUL_MIX_IMPL(x, y, a);
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float4 zMix4FImpl(const float4 x, const float4 y, const float a)
//{
//  const float4 result = ZINVUL_MIX_IMPL(x, y, a);
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float zMix(const float x, const float y, const float a)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
//  const float result = mix(x, y, a);
//#else
//  const float result = zMixImpl(x, y, a);
//#endif
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float2 zMix2(const float2 x, const float2 y, const float2 a)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
//  const float2 result = mix(x, y, a);
//#else
//  const float2 result = zMix2Impl(x, y, a);
//#endif
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float2 zMix2F(const float2 x, const float2 y, const float a)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
//  const float2 result = mix(x, y, a);
//#else
//  const float2 result = zMix2FImpl(x, y, a);
//#endif
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float3 zMix3(const float3 x, const float3 y, const float3 a)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
//  const float3 result = mix(x, y, a);
//#else
//  const float3 result = zMix3Impl(x, y, a);
//#endif
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float3 zMix3F(const float3 x, const float3 y, const float a)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
//  const float3 result = mix(x, y, a);
//#else
//  const float3 result = zMix3FImpl(x, y, a);
//#endif
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float4 zMix4(const float4 x, const float4 y, const float4 a)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
//  const float4 result = mix(x, y, a);
//#else
//  const float4 result = zMix4Impl(x, y, a);
//#endif
//  return result;
//}
//
////! Return the linear blend of x & y implemented as: x + (y - x) * a
//float4 zMix4F(const float4 x, const float4 y, const float a)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
//  const float4 result = mix(x, y, a);
//#else
//  const float4 result = zMix4FImpl(x, y, a);
//#endif
//  return result;
//}
//
//#define ZINVUL_CLAMP_IMPL(zmin, zmax, x, minval, maxval) zmin(zmax(x, minval), maxval)
//
////! Return min(max(x, minval), maxval)
//int32b zClampImpl(const int32b x, const int32b minval, const int32b maxval)
//{
//  const int32b result = ZINVUL_CLAMP_IMPL(zMin, zMax, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//int2 zClamp2Impl(const int2 x, const int2 minval, const int2 maxval)
//{
//  const int2 result = ZINVUL_CLAMP_IMPL(zMin2, zMax2, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//int3 zClamp3Impl(const int3 x, const int3 minval, const int3 maxval)
//{
//  const int3 result = ZINVUL_CLAMP_IMPL(zMin3, zMax3, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//int4 zClamp4Impl(const int4 x, const int4 minval, const int4 maxval)
//{
//  const int4 result = ZINVUL_CLAMP_IMPL(zMin4, zMax4, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//int32b zClamp(const int32b x, const int32b minval, const int32b maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const int32b result = clamp(x, minval, maxval);
//#else
//  const int32b result = zClampImpl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//int2 zClamp2(const int2 x, const int2 minval, const int2 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const int2 result = clamp(x, minval, maxval);
//#else
//  const int2 result = zClamp2Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//int3 zClamp3(const int3 x, const int3 minval, const int3 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const int3 result = clamp(x, minval, maxval);
//#else
//  const int3 result = zClamp3Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//int4 zClamp4(const int4 x, const int4 minval, const int4 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const int4 result = clamp(x, minval, maxval);
//#else
//  const int4 result = zClamp4Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//uint32b zClampUImpl(const uint32b x, const uint32b minval, const uint32b maxval)
//{
//  const uint32b result = ZINVUL_CLAMP_IMPL(zMinU, zMaxU, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//uint2 zClampU2Impl(const uint2 x, const uint2 minval, const uint2 maxval)
//{
//  const uint2 result = ZINVUL_CLAMP_IMPL(zMinU2, zMaxU2, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//uint3 zClampU3Impl(const uint3 x, const uint3 minval, const uint3 maxval)
//{
//  const uint3 result = ZINVUL_CLAMP_IMPL(zMinU3, zMaxU3, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//uint4 zClampU4Impl(const uint4 x, const uint4 minval, const uint4 maxval)
//{
//  const uint4 result = ZINVUL_CLAMP_IMPL(zMinU4, zMaxU4, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//uint32b zClampU(const uint32b x, const uint32b minval, const uint32b maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const uint32b result = clamp(x, minval, maxval);
//#else
//  const uint32b result = zClampUImpl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//uint2 zClampU2(const uint2 x, const uint2 minval, const uint2 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const uint2 result = clamp(x, minval, maxval);
//#else
//  const uint2 result = zClampU2Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//uint3 zClampU3(const uint3 x, const uint3 minval, const uint3 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const uint3 result = clamp(x, minval, maxval);
//#else
//  const uint3 result = zClampU3Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//uint4 zClampU4(const uint4 x, const uint4 minval, const uint4 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const uint4 result = clamp(x, minval, maxval);
//#else
//  const uint4 result = zClampU4Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float zClampFImpl(const float x, const float minval, const float maxval)
//{
//  const float result = ZINVUL_CLAMP_IMPL(zMinF, zMaxF, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float2 zClampF2Impl(const float2 x, const float2 minval, const float2 maxval)
//{
//  const float2 result = ZINVUL_CLAMP_IMPL(zMinF2, zMaxF2, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float2 zClampF2FImpl(const float2 x, const float minval, const float maxval)
//{
//  const float2 result = zClampF2Impl(x,
//                                     zBroadcastF2(minval),
//                                     zBroadcastF2(maxval));
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float3 zClampF3Impl(const float3 x, const float3 minval, const float3 maxval)
//{
//  const float3 result = ZINVUL_CLAMP_IMPL(zMinF3, zMaxF3, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float3 zClampF3FImpl(const float3 x, const float minval, const float maxval)
//{
//  const float3 result = zClampF3Impl(x,
//                                     zBroadcastF3(minval),
//                                     zBroadcastF3(maxval));
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float4 zClampF4Impl(const float4 x, const float4 minval, const float4 maxval)
//{
//  const float4 result = ZINVUL_CLAMP_IMPL(zMinF4, zMaxF4, x, minval, maxval);
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float4 zClampF4FImpl(const float4 x, const float minval, const float maxval)
//{
//  const float4 result = zClampF4Impl(x,
//                                     zBroadcastF4(minval),
//                                     zBroadcastF4(maxval));
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float zClampF(const float x, const float minval, const float maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const float result = clamp(x, minval, maxval);
//#else
//  const float result = zClampFImpl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float2 zClampF2F(const float2 x, const float minval, const float maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const float2 result = clamp(x, minval, maxval);
//#else
//  const float2 result = zClampF2FImpl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float2 zClampF2(const float2 x, const float2 minval, const float2 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const float2 result = clamp(x, minval, maxval);
//#else
//  const float2 result = zClampF2Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float3 zClampF3F(const float3 x, const float minval, const float maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const float3 result = clamp(x, minval, maxval);
//#else
//  const float3 result = zClampF3FImpl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float3 zClampF3(const float3 x, const float3 minval, const float3 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const float3 result = clamp(x, minval, maxval);
//#else
//  const float3 result = zClampF3Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float4 zClampF4F(const float4 x, const float minval, const float maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const float4 result = clamp(x, minval, maxval);
//#else
//  const float4 result = zClampF4FImpl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return min(max(x, minval), maxval)
//float4 zClampF4(const float4 x, const float4 minval, const float4 maxval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
//  const float4 result = clamp(x, minval, maxval);
//#else
//  const float4 result = zClampF4Impl(x, minval, maxval);
//#endif
//  return result;
//}
//
////! Return remainder of the floating point division operation by 1
//#define ZINVUL_FRACT_IMPL(FType, floorF, minF, broadcastF, x, iptr) \
//  const FType i = floorF(x); \
//  const FType y = minF(x - i, broadcastF(0x1.fffffep-1f)); \
//  *iptr = i
//
////! Return remainder of the floating point division operation by 1
//float zFractImpl(const float x, __private float* iptr)
//{
//  ZINVUL_FRACT_IMPL(float, zFloor, zMinF, , x, iptr);
//  return y;
//}
//
////! Return remainder of the floating point division operation by 1
//float2 zFract2Impl(const float2 x, __private float2* iptr)
//{
//  ZINVUL_FRACT_IMPL(float2, zFloor2, zMinF2, zBroadcastF2, x, iptr);
//  return y;
//}
//
////! Return remainder of the floating point division operation by 1
//float3 zFract3Impl(const float3 x, __private float3* iptr)
//{
//  ZINVUL_FRACT_IMPL(float3, zFloor3, zMinF3, zBroadcastF3, x, iptr);
//  return y;
//}
//
////! Return remainder of the floating point division operation by 1
//float4 zFract4Impl(const float4 x, __private float4* iptr)
//{
//  ZINVUL_FRACT_IMPL(float4, zFloor4, zMinF4, zBroadcastF4, x, iptr);
//  return y;
//}
//
////! Return remainder of the floating point division operation by 1
//float zFract(const float x, __private float* iptr)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRACT)
//  const float y = fract(x, iptr);
//#else
//  const float y = zFractImpl(x, iptr);
//#endif
//  return y;
//}
//
////! Return remainder of the floating point division operation by 1
//float2 zFract2(const float2 x, __private float2* iptr)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRACT)
//  const float2 y = fract(x, iptr);
//#else
//  const float2 y = zFract2Impl(x, iptr);
//#endif
//  return y;
//}
//
////! Return remainder of the floating point division operation by 1
//float3 zFract3(const float3 x, __private float3* iptr)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRACT)
//  const float3 y = fract(x, iptr);
//#else
//  const float3 y = zFract3Impl(x, iptr);
//#endif
//  return y;
//}
//
////! Return remainder of the floating point division operation by 1
//float4 zFract4(const float4 x, __private float4* iptr)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRACT)
//  const float4 y = fract(x, iptr);
//#else
//  const float4 y = zFract4Impl(x, iptr);
//#endif
//  return y;
//}
//
////! Return remainder of the floating point division operation
//#define ZINVUL_MOD_IMPL(truncF, x, y) (x - y * truncF(x / y))
//
////! Return remainder of the floating point division operation
//float zModImpl(const float x, const float y)
//{
//  const float z = ZINVUL_MOD_IMPL(zTrunc, x, y);
//  return z;
//}
//
////! Return remainder of the floating point division operation
//float2 zMod2Impl(const float2 x, const float2 y)
//{
//  const float2 z = ZINVUL_MOD_IMPL(zTrunc2, x, y);
//  return z;
//}
//
////! Return remainder of the floating point division operation
//float3 zMod3Impl(const float3 x, const float3 y)
//{
//  const float3 z = ZINVUL_MOD_IMPL(zTrunc3, x, y);
//  return z;
//}
//
////! Return remainder of the floating point division operation
//float4 zMod4Impl(const float4 x, const float4 y)
//{
//  const float4 z = ZINVUL_MOD_IMPL(zTrunc4, x, y);
//  return z;
//}
//
//
////! Return remainder of the floating point division operation
//float zMod(const float x, const float y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MOD)
//  const float z = fmod(x, y);
//#else
//  const float z = zModImpl(x, y);
//#endif
//  return z;
//}
//
////! Return remainder of the floating point division operation
//float2 zMod2(const float2 x, const float2 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MOD)
//  const float2 z = fmod(x, y);
//#else
//  const float2 z = zMod2Impl(x, y);
//#endif
//  return z;
//}
//
////! Return remainder of the floating point division operation
//float3 zMod3(const float3 x, const float3 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MOD)
//  const float3 z = fmod(x, y);
//#else
//  const float3 z = zMod3Impl(x, y);
//#endif
//  return z;
//}
//
////! Return remainder of the floating point division operation
//float4 zMod4(const float4 x, const float4 y)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MOD)
//  const float4 z = fmod(x, y);
//#else
//  const float4 z = zMod4Impl(x, y);
//#endif
//  return z;
//}
//
//#define ZINVUL_DEGREES_IMPL(r) ((180.0f * zInvPiF) * r);
//
////! Convert radians to degrees. (180 / pi) * radians
//float zDegreesImpl(const float r)
//{
//  const float degree = ZINVUL_DEGREES_IMPL(r);
//  return degree;
//}
//
////! Convert radians to degrees. (180 / pi) * radians
//float2 zDegrees2Impl(const float2 r)
//{
//  const float2 degree = ZINVUL_DEGREES_IMPL(r);
//  return degree;
//}
//
////! Convert radians to degrees. (180 / pi) * radians
//float3 zDegrees3Impl(const float3 r)
//{
//  const float3 degree = ZINVUL_DEGREES_IMPL(r);
//  return degree;
//}
//
////! Convert radians to degrees. (180 / pi) * radians
//float4 zDegrees4Impl(const float4 r)
//{
//  const float4 degree = ZINVUL_DEGREES_IMPL(r);
//  return degree;
//}
//
////! Convert radians to degrees. (180 / pi) * radians
//float zDegrees(const float r)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
//  const float degree = degrees(r);
//#else
//  const float degree = zDegreesImpl(r);
//#endif
//  return degree;
//}
//
////! Convert radians to degrees. (180 / pi) * radians
//float2 zDegrees2(const float2 r)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
//  const float2 degree = degrees(r);
//#else
//  const float2 degree = zDegrees2Impl(r);
//#endif
//  return degree;
//}
//
////! Convert radians to degrees. (180 / pi) * radians
//float3 zDegrees3(const float3 r)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
//  const float3 degree = degrees(r);
//#else
//  const float3 degree = zDegrees3Impl(r);
//#endif
//  return degree;
//}
//
////! Convert radians to degrees. (180 / pi) * radians
//float4 zDegrees4(const float4 r)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
//  const float4 degree = degrees(r);
//#else
//  const float4 degree = zDegrees4Impl(r);
//#endif
//  return degree;
//}
//
//#define ZINVUL_RADIANS_IMPL(d) ((zPiF / 180.0f) * d)
//
////! Convert degrees to radians. (pi / 180) *degrees 
//float zRadiansImpl(const float d)
//{
//  const float radian = ZINVUL_RADIANS_IMPL(d);
//  return radian;
//}
//
////! Convert degrees to radians. (pi / 180) *degrees 
//float2 zRadians2Impl(const float2 d)
//{
//  const float2 radian = ZINVUL_RADIANS_IMPL(d);
//  return radian;
//}
//
////! Convert degrees to radians. (pi / 180) *degrees 
//float3 zRadians3Impl(const float3 d)
//{
//  const float3 radian = ZINVUL_RADIANS_IMPL(d);
//  return radian;
//}
//
////! Convert degrees to radians. (pi / 180) *degrees 
//float4 zRadians4Impl(const float4 d)
//{
//  const float4 radian = ZINVUL_RADIANS_IMPL(d);
//  return radian;
//}
//
////! Convert degrees to radians. (pi / 180) *degrees 
//float zRadians(const float d)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
//  const float radian = radians(d);
//#else
//  const float radian = zRadiansImpl(d);
//#endif
//  return radian;
//}
//
////! Convert degrees to radians. (pi / 180) *degrees 
//float2 zRadians2(const float2 d)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
//  const float2 radian = radians(d);
//#else
//  const float2 radian = zRadians2Impl(d);
//#endif
//  return radian;
//}
//
////! Convert degrees to radians. (pi / 180) *degrees 
//float3 zRadians3(const float3 d)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
//  const float3 radian = radians(d);
//#else
//  const float3 radian = zRadians3Impl(d);
//#endif
//  return radian;
//}
//
////! Convert degrees to radians. (pi / 180) *degrees 
//float4 zRadians4(const float4 d)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
//  const float4 radian = radians(d);
//#else
//  const float4 radian = zRadians4Impl(d);
//#endif
//  return radian;
//}
//
//// Floating point manipulation functions
//
////! Decompose a number into significand and a power of 2 \todo Support subnormal value
//#define ZINVUL_FREXP_BIASED_IMPL(FType, UType, IType, asFType, asUType, selectF, x, e) \
//  const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
//  UType data = asUType(x); \
//  const UType expt = ((data & zExponentBitMaskF) >> (uint32b)zSignificandBitSizeF) + 1u; \
//  data = (data & ~zExponentBitMaskF) | 0x3f000000u; \
//  const FType y = selectF(asFType(data), x, is_special)
//
////! Decompose a number into significand and a power of 2
//float zFrexpImpl(const float x, __private int32b* e)
//{
//  ZINVUL_FREXP_BIASED_IMPL(float, uint32b, int32b, as_float, as_uint, zSelectF, x, e);
//  *e = zSelect((int32b)expt - zExponentBiasF, 0, is_special);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float2 zFrexp2Impl(const float2 x, __private int2* e)
//{
//  ZINVUL_FREXP_BIASED_IMPL(float2, uint2, int2, as_float2, as_uint2, zSelectF2, x, e);
//  *e = zSelect2(zU2ToI2(expt) - zExponentBiasF, zBroadcast2(0), is_special);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float3 zFrexp3Impl(const float3 x, __private int3* e)
//{
//  ZINVUL_FREXP_BIASED_IMPL(float3, uint3, int3, as_float3, as_uint3, zSelectF3, x, e);
//  *e = zSelect3(zU3ToI3(expt) - zExponentBiasF, zBroadcast3(0), is_special);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float4 zFrexp4Impl(const float4 x, __private int4* e)
//{
//  ZINVUL_FREXP_BIASED_IMPL(float4, uint4, int4, as_float4, as_uint4, zSelectF4, x, e);
//  *e = zSelect4(zU4ToI4(expt) - zExponentBiasF, zBroadcast4(0), is_special);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float zFrexp(const float x, __private int32b* e)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  const float y = frexp(x, e);
//#else
//  const float y = zFrexpImpl(x, e);
//#endif
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float2 zFrexp2(const float2 x, __private int2* e)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  const float2 y = frexp(x, e);
//#else
//  const float2 y = zFrexp2Impl(x, e);
//#endif
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float3 zFrexp3(const float3 x, __private int3* e)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  const float3 y = frexp(x, e);
//#else
//  const float3 y = zFrexp3Impl(x, e);
//#endif
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float4 zFrexp4(const float4 x, __private int4* e)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  const float4 y = frexp(x, e);
//#else
//  const float4 y = zFrexp4Impl(x, e);
//#endif
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float zFrexpBiasedImpl(const float x, __private uint32b* e)
//{
//  ZINVUL_FREXP_BIASED_IMPL(float, uint32b, int32b, as_float, as_uint, zSelectF, x, e);
//  *e = zSelectU(expt, 0u, is_special);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float2 zFrexpBiased2Impl(const float2 x, __private uint2* e)
//{
//  ZINVUL_FREXP_BIASED_IMPL(float2, uint2, int2, as_float2, as_uint2, zSelectF2, x, e);
//  *e = zSelectU2(expt, zBroadcastU2(0u), is_special);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float3 zFrexpBiased3Impl(const float3 x, __private uint3* e)
//{
//  ZINVUL_FREXP_BIASED_IMPL(float3, uint3, int3, as_float3, as_uint3, zSelectF3, x, e);
//  *e = zSelectU3(expt, zBroadcastU3(0u), is_special);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float4 zFrexpBiased4Impl(const float4 x, __private uint4* e)
//{
//  ZINVUL_FREXP_BIASED_IMPL(float4, uint4, int4, as_float4, as_uint4, zSelectF4, x, e);
//  *e = zSelectU4(expt, zBroadcastU4(0u), is_special);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float zFrexpBiased(const float x, __private uint32b* e)
//{
//  const float y = zFrexpBiasedImpl(x, e);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float2 zFrexpBiased2(const float2 x, __private uint2* e)
//{
//  const float2 y = zFrexpBiased2Impl(x, e);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float3 zFrexpBiased3(const float3 x, __private uint3* e)
//{
//  const float3 y = zFrexpBiased3Impl(x, e);
//  return y;
//}
//
////! Decompose a number into significand and a power of 2
//float4 zFrexpBiased4(const float4 x, __private uint4* e)
//{
//  const float4 y = zFrexpBiased4Impl(x, e);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power. \todo Support subnormal value
//#define ZINVUL_LDEXP_BIASED_IMPL(FType, UType, IType, asFType, selectF, x, e) \
//  const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
//  const UType data = e << (uint32b)zSignificandBitSizeF; \
//  FType y = asFType(data) * x; \
//  y = selectF(y, x, is_special); \
//
////! Multiply a number by 2 raised to a power
//float zLdexpImpl(const float x, const int32b e)
//{
//  const uint32b expt = (uint32b)(e + zExponentBiasF);
//  ZINVUL_LDEXP_BIASED_IMPL(float, uint32b, int32b, as_float, zSelectF, x, expt);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float2 zLdexp2Impl(const float2 x, const int2 e)
//{
//  const uint2 expt = zI2ToU2(e + zExponentBiasF);
//  ZINVUL_LDEXP_BIASED_IMPL(float2, uint2, int2, as_float2, zSelectF2, x, expt);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float3 zLdexp3Impl(const float3 x, const int3 e)
//{
//  const uint3 expt = zI3ToU3(e + zExponentBiasF);
//  ZINVUL_LDEXP_BIASED_IMPL(float3, uint3, int3, as_float3, zSelectF3, x, expt);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float4 zLdexp4Impl(const float4 x, const int4 e)
//{
//  const uint4 expt = zI4ToU4(e + zExponentBiasF);
//  ZINVUL_LDEXP_BIASED_IMPL(float4, uint4, int4, as_float4, zSelectF4, x, expt);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float zLdexp(const float x, const int32b e)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  const float y = ldexp(x, e);
//#else
//  const float y = zLdexpImpl(x, e);
//#endif
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float2 zLdexp2(const float2 x, const int2 e)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  const float2 y = ldexp(x, e);
//#else
//  const float2 y = zLdexp2Impl(x, e);
//#endif
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float3 zLdexp3(const float3 x, const int3 e)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  const float3 y = ldexp(x, e);
//#else
//  const float3 y = zLdexp3Impl(x, e);
//#endif
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float4 zLdexp4(const float4 x, const int4 e)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  const float4 y = ldexp(x, e);
//#else
//  const float4 y = zLdexp4Impl(x, e);
//#endif
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float zLdexpBiasedImpl(const float x, const uint32b e)
//{
//  ZINVUL_LDEXP_BIASED_IMPL(float, uint32b, int32b, as_float, zSelectF, x, e);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float2 zLdexpBiased2Impl(const float2 x, const uint2 e)
//{
//  ZINVUL_LDEXP_BIASED_IMPL(float2, uint2, int2, as_float2, zSelectF2, x, e);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float3 zLdexpBiased3Impl(const float3 x, const uint3 e)
//{
//  ZINVUL_LDEXP_BIASED_IMPL(float3, uint3, int3, as_float3, zSelectF3, x, e);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float4 zLdexpBiased4Impl(const float4 x, const uint4 e)
//{
//  ZINVUL_LDEXP_BIASED_IMPL(float4, uint4, int4, as_float4, zSelectF4, x, e);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float zLdexpBiased(const float x, const uint32b e)
//{
//  const float y = zLdexpBiasedImpl(x, e);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float2 zLdexpBiased2(const float2 x, const uint2 e)
//{
//  const float2 y = zLdexpBiased2Impl(x, e);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float3 zLdexpBiased3(const float3 x, const uint3 e)
//{
//  const float3 y = zLdexpBiased3Impl(x, e);
//  return y;
//}
//
////! Multiply a number by 2 raised to a power
//float4 zLdexpBiased4(const float4 x, const uint4 e)
//{
//  const float4 y = zLdexpBiased4Impl(x, e);
//  return y;
//}
//
//// Integer
//
////! Return the number of leading 0-bits
//#define ZINVUL_CLZ_IMPL(x) clz(x)
//
////! Return the number of leading 0-bits
//int32b zClzImpl(const int32b x)
//{
//  const int32b y = ZINVUL_CLZ_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//int2 zClz2Impl(const int2 x)
//{
//  const int2 y = ZINVUL_CLZ_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//int3 zClz3Impl(const int3 x)
//{
//  const int3 y = ZINVUL_CLZ_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//int4 zClz4Impl(const int4 x)
//{
//  const int4 y = ZINVUL_CLZ_IMPL(x);
//  return y;
//}
//
//
////! Return the number of leading 0-bits
//int32b zClz(const int32b x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLZ)
//  const int32b y = clz(x);
//#else
//  const int32b y = zClzImpl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//int2 zClz2(const int2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLZ)
//  const int2 y = clz(x);
//#else
//  const int2 y = zClz2Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//int3 zClz3(const int3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLZ)
//  const int3 y = clz(x);
//#else
//  const int3 y = zClz3Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//int4 zClz4(const int4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLZ)
//  const int4 y = clz(x);
//#else
//  const int4 y = zClz4Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//#define ZINVUL_CLZU_IMPL(x) clz(x)
//
////! Return the number of leading 0-bits
//uint32b zClzUImpl(const uint32b x)
//{
//  const uint32b y = ZINVUL_CLZU_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint2 zClzU2Impl(const uint2 x)
//{
//  const uint2 y = ZINVUL_CLZU_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint3 zClzU3Impl(const uint3 x)
//{
//  const uint3 y = ZINVUL_CLZU_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint4 zClzU4Impl(const uint4 x)
//{
//  const uint4 y = ZINVUL_CLZU_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint32b zClzU(const uint32b x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLZ)
//  const uint32b y = clz(x);
//#else
//  const uint32b y = zClzUImpl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint2 zClzU2(const uint2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLZ)
//  const uint2 y = clz(x);
//#else
//  const uint2 y = zClzU2Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint3 zClzU3(const uint3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLZ)
//  const uint3 y = clz(x);
//#else
//  const uint3 y = zClzU3Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint4 zClzU4(const uint4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLZ)
//  const uint4 y = clz(x);
//#else
//  const uint4 y = zClzU4Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//#define ZINVUL_POPCOUNT_IMPL(x) popcount(x)
//
////! Return the number of leading 0-bits
//int32b zPopcountImpl(const int32b x)
//{
//  const int32b y = ZINVUL_POPCOUNT_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//int2 zPopcount2Impl(const int2 x)
//{
//  const int2 y = ZINVUL_POPCOUNT_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//int3 zPopcount3Impl(const int3 x)
//{
//  const int3 y = ZINVUL_POPCOUNT_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//int4 zPopcount4Impl(const int4 x)
//{
//  const int4 y = ZINVUL_POPCOUNT_IMPL(x);
//  return y;
//}
////! Return the number of leading 0-bits
//int32b zPopcount(const int32b x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POPCOUNT)
//  const int32b y = popcount(x);
//#else
//  const int32b y = zPopcountImpl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//int2 zPopcount2(const int2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POPCOUNT)
//  const int2 y = popcount(x);
//#else
//  const int2 y = zPopcount2Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//int3 zPopcount3(const int3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POPCOUNT)
//  const int3 y = popcount(x);
//#else
//  const int3 y = zPopcount3Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//int4 zPopcount4(const int4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POPCOUNT)
//  const int4 y = popcount(x);
//#else
//  const int4 y = zPopcount4Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//#define ZINVUL_POPCOUNTU_IMPL(x) popcount(x)
//
////! Return the number of leading 0-bits
//uint32b zPopcountUImpl(const uint32b x)
//{
//  const uint32b y = ZINVUL_POPCOUNTU_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint2 zPopcountU2Impl(const uint2 x)
//{
//  const uint2 y = ZINVUL_POPCOUNTU_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint3 zPopcountU3Impl(const uint3 x)
//{
//  const uint3 y = ZINVUL_POPCOUNTU_IMPL(x);
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint4 zPopcountU4Impl(const uint4 x)
//{
//  const uint4 y = ZINVUL_POPCOUNT_IMPL(x);
//  return y;
//}
////! Return the number of leading 0-bits
//uint32b zPopcountU(const uint32b x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POPCOUNT)
//  const uint32b y = popcount(x);
//#else
//  const uint32b y = zPopcountUImpl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint2 zPopcountU2(const uint2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POPCOUNT)
//  const uint2 y = popcount(x);
//#else
//  const uint2 y = zPopcountU2Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint3 zPopcountU3(const uint3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POPCOUNT)
//  const uint3 y = popcount(x);
//#else
//  const uint3 y = zPopcountU3Impl(x);
//#endif
//  return y;
//}
//
////! Return the number of leading 0-bits
//uint4 zPopcountU4(const uint4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POPCOUNT)
//  const uint4 y = popcount(x);
//#else
//  const uint4 y = zPopcountU4Impl(x);
//#endif
//  return y;
//}
//
//// Exponential functions
//
////! Return e raised to the given power
//#define ZINVUL_EXP_IMPL(FType, fToU, broadcastF, selectF, loadExp, roundf, x) \
//  const FType q = roundf(zInvLn2F * x); \
//  FType z = fma(q, broadcastF(-0.693145751953125f), x); \
//  z = fma(q, broadcastF(-1.428606765330187045e-06f), z); \
//  FType y = broadcastF(0.0f); \
//  { \
//    FType t = z; \
//    for (size_t i = 2; i < 7; ++i) { \
//      t = t * (z / (float)i); \
//      y = y + t; \
//    } \
//  } \
//  y = 1.0f + z + y; \
//  y = loadExp(y, fToU(q + (float)zExponentBiasF)); \
//  y = selectF(y, x, x == INFINITY); \
//  y = selectF(y, broadcastF(0.0f), x == -INFINITY)
//
////! Return e raised to the given power
//float zExpImpl(const float x)
//{
//  ZINVUL_EXP_IMPL(float, (uint32b),, zSelectF, zLdexpBiased, zRound, x);
//  return y;
//}
//
////! Return e raised to the given power
//float2 zExp2Impl(const float2 x)
//{
//  ZINVUL_EXP_IMPL(float2, zF2ToU2, zBroadcastF2, zSelectF2, zLdexpBiased2, zRound2, x);
//  return y;
//}
//
////! Return e raised to the given power
//float3 zExp3Impl(const float3 x)
//{
//  ZINVUL_EXP_IMPL(float3, zF3ToU3, zBroadcastF3, zSelectF3, zLdexpBiased3, zRound3, x);
//  return y;
//}
//
////! Return e raised to the given power
//float4 zExp4Impl(const float4 x)
//{
//  ZINVUL_EXP_IMPL(float4, zF4ToU4, zBroadcastF4, zSelectF4, zLdexpBiased4, zRound4, x);
//  return y;
//}
//
////! Return e raised to the given power
//float zExp(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_EXP)
//  const float y = exp(x);
//#else
//  const float y = zExpImpl(x);
//#endif
//  return y;
//}
//
////! Return e raised to the given power
//float2 zExp2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_EXP)
//  const float2 y = exp(x);
//#else
//  const float2 y = zExp2Impl(x);
//#endif
//  return y;
//}
//
////! Return e raised to the given power
//float3 zExp3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_EXP)
//  const float3 y = exp(x);
//#else
//  const float3 y = zExp3Impl(x);
//#endif
//  return y;
//}
//
////! Return e raised to the given power
//float4 zExp4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_EXP)
//  const float4 y = exp(x);
//#else
//  const float4 y = zExp4Impl(x);
//#endif
//  return y;
//}
//
////! Compute natural logarithm of the given number. \todo Support subnormal
//#define ZINVUL_LOG_IMPL(FType, UType, IType, uToF, broadcastU, broadcastF, selectU, selectF, fracExp, x, is_base2) \
//  UType e = broadcastU(0); \
//  FType z = fracExp(x, &e); \
//  { \
//    const IType c = z < zInvSqrt2F; \
//    e = selectU(e, e - 1u, c); \
//    z = selectF(z, 2.0f * z, c); \
//  } \
//  z = (z - 1.0f) / (z + 1.0f); \
//  FType y = broadcastF(0.0f); \
//  { \
//    FType t = z; \
//    for (size_t i = 1; i < 4; ++i) { \
//      t = t * z * z; \
//      y = y + t / (float)(2 * i + 1); \
//    } \
//  } \
//  y = 2.0f * (z + y); \
//  if (is_base2) \
//    y = fma(y, broadcastF(zInvLn2F), uToF(e) - (float)zExponentBiasF); \
//  else \
//    y = fma(uToF(e) - (float)zExponentBiasF, broadcastF(zLn2F), y); \
//  { \
//    y = selectF(y, broadcastF(-INFINITY), x == 0.0f); \
//    y = selectF(y, x, isinf(x)); \
//    y = selectF(y, broadcastF(NAN), (x < 0.0f) || isnan(x)); \
//  } \
//
////! Compute natural logarithm of the given number
//float zLogImpl(const float x)
//{
//  ZINVUL_LOG_IMPL(float, uint32b, int32b, (float),,, zSelectU, zSelectF, zFrexpBiased, x, 0);
//  return y;
//}
//
////! Compute natural logarithm of the given number
//float2 zLogF2Impl(const float2 x)
//{
//  ZINVUL_LOG_IMPL(float2, uint2, int2, zU2ToF2, zBroadcastU2, zBroadcastF2, zSelectU2, zSelectF2, zFrexpBiased2, x, 0);
//  return y;
//}
//
////! Compute natural logarithm of the given number
//float3 zLogF3Impl(const float3 x)
//{
//  ZINVUL_LOG_IMPL(float3, uint3, int3, zU3ToF3, zBroadcastU3, zBroadcastF3, zSelectU3, zSelectF3, zFrexpBiased3, x, 0);
//  return y;
//}
//
////! Compute natural logarithm of the given number
//float4 zLogF4Impl(const float4 x)
//{
//  ZINVUL_LOG_IMPL(float4, uint4, int4, zU4ToF4, zBroadcastU4, zBroadcastF4, zSelectU4, zSelectF4, zFrexpBiased4, x, 0);
//  return y;
//}
//
////! Compute natural logarithm of the given number
//float zLog(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
//  const float y = log(x);
//#else
//  const float y = zLogImpl(x);
//#endif
//  return y;
//}
//
////! Compute natural logarithm of the given number
//float2 zLogF2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
//  const float2 y = log(x);
//#else
//  const float2 y = zLogF2Impl(x);
//#endif
//  return y;
//}
//
////! Compute natural logarithm of the given number
//float3 zLogF3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
//  const float3 y = log(x);
//#else
//  const float3 y = zLogF3Impl(x);
//#endif
//  return y;
//}
//
////! Compute natural logarithm of the given number
//float4 zLogF4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
//  const float4 y = log(x);
//#else
//  const float4 y = zLogF4Impl(x);
//#endif
//  return y;
//}
//
////! Compute base 2 logarithm of the given number
//float zLog2Impl(const float x)
//{
//  ZINVUL_LOG_IMPL(float, uint32b, int32b, (float),,, zSelectU, zSelectF, zFrexpBiased, x, 1);
//  return y;
//}
//
////! Compute base 2 logarithm of the given number
//float2 zLog2F2Impl(const float2 x)
//{
//  ZINVUL_LOG_IMPL(float2, uint2, int2, zU2ToF2, zBroadcastU2, zBroadcastF2, zSelectU2, zSelectF2, zFrexpBiased2, x, 1);
//  return y;
//}
//
////! Compute base 2 logarithm of the given number
//float3 zLog2F3Impl(const float3 x)
//{
//  ZINVUL_LOG_IMPL(float3, uint3, int3, zU3ToF3, zBroadcastU3, zBroadcastF3, zSelectU3, zSelectF3, zFrexpBiased3, x, 1);
//  return y;
//}
//
////! Compute base 2 logarithm of the given number
//float4 zLog2F4Impl(const float4 x)
//{
//  ZINVUL_LOG_IMPL(float4, uint4, int4, zU4ToF4, zBroadcastU4, zBroadcastF4, zSelectU4, zSelectF4, zFrexpBiased4, x, 1);
//  return y;
//}
//
////! Compute base 2 logarithm of the given number
//float zLog2(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
//  const float y = log2(x);
//#else
//  const float y = zLog2Impl(x);
//#endif
//  return y;
//}
//
////! Compute base 2 logarithm of the given number
//float2 zLog2F2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
//  const float2 y = log2(x);
//#else
//  const float2 y = zLog2F2Impl(x);
//#endif
//  return y;
//}
//
////! Compute base 2 logarithm of the given number
//float3 zLog2F3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
//  const float3 y = log2(x);
//#else
//  const float3 y = zLog2F3Impl(x);
//#endif
//  return y;
//}
//
////! Compute base 2 logarithm of the given number
//float4 zLog2F4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
//  const float4 y = log2(x);
//#else
//  const float4 y = zLog2F4Impl(x);
//#endif
//  return y;
//}
//
//// Power functions
//
////! Raise a number to the given power
//#define ZINVUL_POW_IMPL(expf, loge, base, expt) expf(expt * loge(base))
//
////! Raise a number to the given power
//float zPowImpl(const float base, const float expt)
//{
//  const float y = ZINVUL_POW_IMPL(zExp, zLog, base, expt);
//  return y;
//}
//
////! Raise a number to the given power
//float2 zPow2Impl(const float2 base, const float2 expt)
//{
//  const float2 y = ZINVUL_POW_IMPL(zExp2, zLogF2, base, expt);
//  return y;
//}
//
////! Raise a number to the given power
//float3 zPow3Impl(const float3 base, const float3 expt)
//{
//  const float3 y = ZINVUL_POW_IMPL(zExp3, zLogF3, base, expt);
//  return y;
//}
//
////! Raise a number to the given power
//float4 zPow4Impl(const float4 base, const float4 expt)
//{
//  const float4 y = ZINVUL_POW_IMPL(zExp4, zLogF4, base, expt);
//  return y;
//}
//
////! Raise a number to the given power
//float zPow(const float base, const float expt)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POW)
//  const float y = pow(base, expt);
//#else
//  const float y = zPowImpl(base, expt);
//#endif
//  return y;
//}
//
////! Raise a number to the given power
//float2 zPow2(const float2 base, const float2 expt)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POW)
//  const float2 y = pow(base, expt);
//#else
//  const float2 y = zPow2Impl(base, expt);
//#endif
//  return y;
//}
//
////! Raise a number to the given power
//float3 zPow3(const float3 base, const float3 expt)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POW)
//  const float3 y = pow(base, expt);
//#else
//  const float3 y = zPow3Impl(base, expt);
//#endif
//  return y;
//}
//
////! Raise a number to the given power
//float4 zPow4(const float4 base, const float4 expt)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_POW)
//  const float4 y = pow(base, expt);
//#else
//  const float4 y = zPow4Impl(base, expt);
//#endif
//  return y;
//}
//
////! Raise a number to the given integer power
//#define ZINVUL_POWN_IMPL(FType, IType, UType, selectF, broadcastF, absolute, hasTrue, isOdd, base, expt) \
//  FType y = base; \
//  FType b = selectF(base, 1.0f / base, expt < 0); \
//  for (UType e = absolute(expt - 1); hasTrue(0u < e); e = e >> 1u) { \
//    y = selectF(y, y * b, isOdd(e)); \
//    b = selectF(b, b * b, 1u < e); \
//  } \
//  y = selectF(y, broadcastF(1.0f), expt == 0); \
//  y = selectF(y, 1.0f / base, ((base == 0.0f) || (isinf(base))) && (expt < 0))
//
////! Raise a number to the given integer power
//float zPown(const float base, const int32b expt)
//{
//  ZINVUL_POWN_IMPL(float, int32b, uint32b, zSelectF,, zAbs,, zIsOddU, base, expt);
//  return y;
//}
//
////! Raise a number to the given integer power
//float2 zPown2(const float2 base, const int2 expt)
//{
//  ZINVUL_POWN_IMPL(float2, int2, uint2, zSelectF2, zBroadcastF2, zAbs2, zHasTrue2, zIsOddU2, base, expt);
//  return y;
//}
//
////! Raise a number to the given integer power
//float3 zPown3(const float3 base, const int3 expt)
//{
//  ZINVUL_POWN_IMPL(float3, int3, uint3, zSelectF3, zBroadcastF3, zAbs3, zHasTrue3, zIsOddU3, base, expt);
//  return y;
//}
//
////! Raise a number to the given integer power
//float4 zPown4(const float4 base, const int4 expt)
//{
//  ZINVUL_POWN_IMPL(float4, int4, uint4, zSelectF4, zBroadcastF4, zAbs4, zHasTrue4, zIsOddU4, base, expt);
//  return y;
//}
//
////! Compute approximate inverse square root
//#define ZINVUL_APPROXIMATE_INV_SQRT_IMPL(FType, IType, asFType, asIType, x) \
//  const IType data = 0x5f376908 - (asIType(x) >> 1); \
//  const FType y = asFType(data)
//
////! Compute approximate inverse square root
//float zApproxRsqrt(const float x)
//{
//  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float, int32b, as_float, as_int, x);
//  return y;
//}
//
////! Compute approximate inverse square root
//float2 zApproxRsqrt2(const float2 x)
//{
//  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float2, int2, as_float2, as_int2, x);
//  return y;
//}
//
////! Compute approximate inverse square root
//float3 zApproxRsqrt3(const float3 x)
//{
//  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float3, int3, as_float3, as_int3, x);
//  return y;
//}
//
////! Compute approximate inverse square root
//float4 zApproxRsqrt4(const float4 x)
//{
//  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float4, int4, as_float4, as_int4, x);
//  return y;
//}
//
////! Compute inverse square root. \todo Support subnormal
//#define ZINVUL_INV_SQRT_IMPL(FType, selectF, broadcastF, approxRsqrt, x) \
//  const FType z = selectF(x, x * 4.294967296e+09f, x < 2.328306437e-10f); \
//  FType y = approxRsqrt(z); \
//  { \
//    const FType halfx = -0.5f * z; \
//    y = y * fma(halfx * y, y, broadcastF(1.5008789f)); \
//    y = y * fma(halfx * y, y, broadcastF(1.5000006f)); \
//    const FType hh = fma(halfx * y, y, broadcastF(0.5f)); \
//    y = fma(y, hh, y); \
//  } \
//  y = selectF(y, y * 6.553600000e+04f, x < 2.328306437e-10f)
//
////! Compute inverse square root
//float zRsqrtImpl(const float x)
//{
//  ZINVUL_INV_SQRT_IMPL(float, zSelectF,, zApproxRsqrt, x);
//  return y;
//}
//
////! Compute inverse square root
//float2 zRsqrt2Impl(const float2 x)
//{
//  ZINVUL_INV_SQRT_IMPL(float2, zSelectF2, zBroadcastF2, zApproxRsqrt2, x);
//  return y;
//}
//
////! Compute inverse square root
//float3 zRsqrt3Impl(const float3 x)
//{
//  ZINVUL_INV_SQRT_IMPL(float3, zSelectF3, zBroadcastF3, zApproxRsqrt3, x);
//  return y;
//}
//
////! Compute inverse square root
//float4 zRsqrt4Impl(const float4 x)
//{
//  ZINVUL_INV_SQRT_IMPL(float4, zSelectF4, zBroadcastF4, zApproxRsqrt4, x);
//  return y;
//}
//
////! Compute inverse square root
//float zRsqrt(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
//  const float y = rsqrt(x);
//#else
//  const float y = zRsqrtImpl(x);
//#endif
//  return y;
//}
//
////! Compute inverse square root
//float2 zRsqrt2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
//  const float2 y = rsqrt(x);
//#else
//  const float2 y = zRsqrt2Impl(x);
//#endif
//  return y;
//}
//
////! Compute inverse square root
//float3 zRsqrt3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
//  const float3 y = rsqrt(x);
//#else
//  const float3 y = zRsqrt3Impl(x);
//#endif
//  return y;
//}
//
////! Compute inverse square root
//float4 zRsqrt4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
//  const float4 y = rsqrt(x);
//#else
//  const float4 y = zRsqrt4Impl(x);
//#endif
//  return y;
//}
//
////! Compute square root
//#define ZINVUL_SQRT_IMPL(FType, IType, selectF, invSqrt, x) \
//  FType y = x * invSqrt(x); \
//  { \
//    const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
//    y = selectF(y, x, is_special); \
//  }
//
////! Compute square root
//float zSqrtImpl(const float x)
//{
//  ZINVUL_SQRT_IMPL(float, int32b, zSelectF, zRsqrt, x);
//  return y;
//}
//
////! Compute square root
//float2 zSqrt2Impl(const float2 x)
//{
//  ZINVUL_SQRT_IMPL(float2, int2, zSelectF2, zRsqrt2, x);
//  return y;
//}
//
////! Compute square root
//float3 zSqrt3Impl(const float3 x)
//{
//  ZINVUL_SQRT_IMPL(float3, int3, zSelectF3, zRsqrt3, x);
//  return y;
//}
//
////! Compute square root
//float4 zSqrt4Impl(const float4 x)
//{
//  ZINVUL_SQRT_IMPL(float4, int4, zSelectF4, zRsqrt4, x);
//  return y;
//}
//
////! Compute square root
//float zSqrt(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
//  const float y = sqrt(x);
//#else
//  const float y = zSqrtImpl(x);
//#endif
//  return y;
//}
//
////! Compute square root
//float2 zSqrt2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
//  const float2 y = sqrt(x);
//#else
//  const float2 y = zSqrt2Impl(x);
//#endif
//  return y;
//}
//
////! Compute square root
//float3 zSqrt3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
//  const float3 y = sqrt(x);
//#else
//  const float3 y = zSqrt3Impl(x);
//#endif
//  return y;
//}
//
////! Compute square root
//float4 zSqrt4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
//  const float4 y = sqrt(x);
//#else
//  const float4 y = zSqrt4Impl(x);
//#endif
//  return y;
//}
//
////! Compute approximate cubic root
//#define ZINVUL_APPROXIMATE_CBRT_IMPL(FType, IType, asFType, asIType, selectF, absoluteF, x) \
//  IType data = asIType(absoluteF(x)); \
//  data = (data >> 2) + (data >> 4); \
//  data = data + (data >> 4); \
//  data = data + (data >> 8); \
//  data = 0x2a5137a0 + data; \
//  FType y = asFType(data); \
//  y = selectF(y, -y, x < 0.0f)
//
////! Compute approximate cubic root
//float zApproxCbrt(const float x)
//{
//  ZINVUL_APPROXIMATE_CBRT_IMPL(float, int32b, as_float, as_int, zSelectF, zAbsF, x);
//  return y;
//}
//
////! Compute approximate cubic root
//float2 zApproxCbrt2(const float2 x)
//{
//  ZINVUL_APPROXIMATE_CBRT_IMPL(float2, int2, as_float2, as_int2, zSelectF2, zAbsF2, x);
//  return y;
//}
//
////! Compute approximate cubic root
//float3 zApproxCbrt3(const float3 x)
//{
//  ZINVUL_APPROXIMATE_CBRT_IMPL(float3, int3, as_float3, as_int3, zSelectF3, zAbsF3, x);
//  return y;
//}
//
////! Compute approximate cubic root
//float4 zApproxCbrt4(const float4 x)
//{
//  ZINVUL_APPROXIMATE_CBRT_IMPL(float4, int4, as_float4, as_int4, zSelectF4, zAbsF4, x);
//  return y;
//}
//
////! Compute cubic root
//#define ZINVUL_CBRT_IMPL(FType, IType, broadcastF, selectF, approxCbrt, x) \
//  const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
//  const FType z = selectF(x, broadcastF(1.0f), is_special); \
//  FType y = approxCbrt(z); \
//  { \
//    const FType a = 1.5f * z; \
//    for (size_t i = 0; i < 2; ++i) { \
//      const FType t = fma(2.0f * y, y * y, z); \
//      y = y * (0.5f + a / t); \
//    } \
//  } \
//  y = selectF(y, x, is_special)
//
////! Compute cubic root
//float zCbrtImpl(const float x)
//{
//  ZINVUL_CBRT_IMPL(float, int32b,, zSelectF, zApproxCbrt, x);
//  return y;
//}
//
////! Compute cubic root
//float2 zCbrt2Impl(const float2 x)
//{
//  ZINVUL_CBRT_IMPL(float2, int2, zBroadcastF2, zSelectF2, zApproxCbrt2, x);
//  return y;
//}
//
////! Compute cubic root
//float3 zCbrt3Impl(const float3 x)
//{
//  ZINVUL_CBRT_IMPL(float3, int3, zBroadcastF3, zSelectF3, zApproxCbrt3, x);
//  return y;
//}
//
////! Compute cubic root
//float4 zCbrt4Impl(const float4 x)
//{
//  ZINVUL_CBRT_IMPL(float4, int4, zBroadcastF4, zSelectF4, zApproxCbrt4, x);
//  return y;
//}
//
////! Compute cubic root
//float zCbrt(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CBRT)
//  const float y = cbrt(x);
//#else
//  const float y = zCbrtImpl(x);
//#endif
//  return y;
//}
//
////! Compute cubic root
//float2 zCbrt2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CBRT)
//  const float2 y = cbrt(x);
//#else
//  const float2 y = zCbrt2Impl(x);
//#endif
//  return y;
//}
//
////! Compute cubic root
//float3 zCbrt3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CBRT)
//  const float3 y = cbrt(x);
//#else
//  const float3 y = zCbrt3Impl(x);
//#endif
//  return y;
//}
//
////! Compute cubic root
//float4 zCbrt4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CBRT)
//  const float4 y = cbrt(x);
//#else
//  const float4 y = zCbrt4Impl(x);
//#endif
//  return y;
//}
//
//// Trigonometric functions
//
////! Compute sine
//#define ZINVUL_SINCOS_IMPL(FType, IType, iToF, fToI, broadcastF, selectF, absoluteF, squareRoot, theta, trigonometric_type) \
//  FType y = absoluteF(theta); \
//  const IType q = fToI((4.0f / zPiF) * y); \
//  { \
//    const FType r = iToF(q + (q & 1)); \
//    y = fma(r, broadcastF(-0.25f * 3.140625f), y); \
//    y = fma(r, broadcastF(-0.25f * 0.0009670257568359375f), y); \
//    y = fma(r, broadcastF(-0.25f * 6.2771141529083251953e-07f), y); \
//    y = fma(r, broadcastF(-0.25f * 1.2154201256553420762e-10f), y); \
//  } \
//  FType c = broadcastF(0.0f); \
//  { \
//    y = 0.125f * y; \
//    y = y * y; \
//    FType t = broadcastF(-2.0f); \
//    for (size_t i = 1; i < 4; ++i) { \
//      t = -t * y / (float)((2 * i) * (2 * i - 1)); \
//      c = c + t; \
//    } \
//    for (size_t i = 0; i < 3; ++i) \
//      c = (4.0f - c) * c; \
//    c = 0.5f * c; \
//  } \
//  FType s = squareRoot((2.0f - c) * c); \
//  c = 1.0f - c; \
//  const IType condition = ((q + 1) & 2) != 0; \
//  if (trigonometric_type == 0) { \
//    s = selectF(s, c, condition); \
//    y = selectF(s, -s, ((q & 4) != 0) != (theta < 0.0f)); \
//  } \
//  else if (trigonometric_type == 1) {\
//    c = selectF(c, s, condition); \
//    y = selectF(c, -c, ((q + 2) & 4) != 0); \
//  } \
//  else { \
//    y = s; \
//    s = selectF(s, c, condition); \
//    c = selectF(c, y, condition); \
//    s = selectF(s, -s, ((q & 4) != 0) != (theta < 0.0f)); \
//    c = selectF(c, -c, ((q + 2) & 4) != 0); \
//  }
//
////! Compute sine
//float zSinImpl(const float theta)
//{
//  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 0);
//  return y;
//}
//
////! Compute sine
//float2 zSin2Impl(const float2 theta)
//{
//  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 0);
//  return y;
//}
//
////! Compute sine
//float3 zSin3Impl(const float3 theta)
//{
//  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 0);
//  return y;
//}
//
////! Compute sine
//float4 zSin4Impl(const float4 theta)
//{
//  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 0);
//  return y;
//}
//
////! Compute sine
//float zSin(const float theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float y = sin(theta);
//#else
//  const float y = zSinImpl(theta);
//#endif
//  return y;
//}
//
////! Compute sine
//float2 zSin2(const float2 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float2 y = sin(theta);
//#else
//  const float2 y = zSin2Impl(theta);
//#endif
//  return y;
//}
//
////! Compute sine
//float3 zSin3(const float3 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float3 y = sin(theta);
//#else
//  const float3 y = zSin3Impl(theta);
//#endif
//  return y;
//}
//
////! Compute sine
//float4 zSin4(const float4 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float4 y = sin(theta);
//#else
//  const float4 y = zSin4Impl(theta);
//#endif
//  return y;
//}
//
////! Compute cosine
//float zCosImpl(const float theta)
//{
//  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 1);
//  return y;
//}
//
////! Compute cosine
//float2 zCos2Impl(const float2 theta)
//{
//  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 1);
//  return y;
//}
//
////! Compute cosine
//float3 zCos3Impl(const float3 theta)
//{
//  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 1);
//  return y;
//}
//
////! Compute cosine
//float4 zCos4Impl(const float4 theta)
//{
//  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 1);
//  return y;
//}
//
////! Compute cosine
//float zCos(const float theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float y = cos(theta);
//#else
//  const float y = zCosImpl(theta);
//#endif
//  return y;
//}
//
////! Compute cosine
//float2 zCos2(const float2 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float2 y = cos(theta);
//#else
//  const float2 y = zCos2Impl(theta);
//#endif
//  return y;
//}
//
////! Compute cosine
//float3 zCos3(const float3 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float3 y = cos(theta);
//#else
//  const float3 y = zCos3Impl(theta);
//#endif
//  return y;
//}
//
////! Compute cosine
//float4 zCos4(const float4 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float4 y = cos(theta);
//#else
//  const float4 y = zCos4Impl(theta);
//#endif
//  return y;
//}
//
////! Compute sine and cosine
//float zSincosImpl(const float theta, __private float* cosval)
//{
//  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 2);
//  y = s;
//  *cosval = c;
//  return y;
//}
//
////! Compute sine and cosine
//float2 zSincos2Impl(const float2 theta, __private float2* cosval)
//{
//  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 2);
//  y = s;
//  *cosval = c;
//  return y;
//}
//
////! Compute sine and cosine
//float3 zSincos3Impl(const float3 theta, __private float3* cosval)
//{
//  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 2);
//  y = s;
//  *cosval = c;
//  return y;
//}
//
////! Compute sine and cosine
//float4 zSincos4Impl(const float4 theta, __private float4* cosval)
//{
//  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 2);
//  y = s;
//  *cosval = c;
//  return y;
//}
////! Compute sine and cosine
//float zSincos(const float theta, __private float* cosval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
////  const float y = sincos(theta, cosval);
//  *cosval = cos(theta);
//  const float y = sin(theta);
//#else
//  const float y = zSincosImpl(theta, cosval);
//#endif
//  return y;
//}
//
////! Compute sine and cosine
//float2 zSincos2(const float2 theta, __private float2* cosval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
////  const float2 y = sincos(theta, cosval);
//  *cosval = cos(theta);
//  const float2 y = sin(theta);
//#else
//  const float2 y = zSincos2Impl(theta, cosval);
//#endif
//  return y;
//}
//
////! Compute sine and cosine
//float3 zSincos3(const float3 theta, __private float3* cosval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
////  const float3 y = sincos(theta, cosval);
//  *cosval = cos(theta);
//  const float3 y = sin(theta);
//#else
//  const float3 y = zSincos3Impl(theta, cosval);
//#endif
//  return y;
//}
//
////! Compute sine and cosine
//float4 zSincos4(const float4 theta, __private float4* cosval)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
////  const float4 y = sincos(theta, cosval);
//  *cosval = cos(theta);
//  const float4 y = sin(theta);
//#else
//  const float4 y = zSincos4Impl(theta, cosval);
//#endif
//  return y;
//}
//
////! Compute tangent
//float zTanImpl(const float theta)
//{
//  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 2);
//  y = s / c;
//  return y;
//}
//
////! Compute tangent
//float2 zTan2Impl(const float2 theta)
//{
//  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 2);
//  y = s / c;
//  return y;
//}
//
////! Compute tangent
//float3 zTan3Impl(const float3 theta)
//{
//  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 2);
//  y = s / c;
//  return y;
//}
//
////! Compute tangent
//float4 zTan4Impl(const float4 theta)
//{
//  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 2);
//  y = s / c;
//  return y;
//}
//
////! Compute tangent
//float zTan(const float theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float y = tan(theta);
//#else
//  const float y = zTanImpl(theta);
//#endif
//  return y;
//}
//
////! Compute tangent
//float2 zTan2(const float2 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float2 y = tan(theta);
//#else
//  const float2 y = zTan2Impl(theta);
//#endif
//  return y;
//}
//
////! Compute tangent
//float3 zTan3(const float3 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float3 y = tan(theta);
//#else
//  const float3 y = zTan3Impl(theta);
//#endif
//  return y;
//}
//
////! Compute tangent
//float4 zTan4(const float4 theta)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float4 y = tan(theta);
//#else
//  const float4 y = zTan4Impl(theta);
//#endif
//  return y;
//}
//
////! Compute arc tangent
//#define ZINVUL_ATAN_IMPL(FType, IType, iToF, selectI, selectF, broadcast, broadcastF, absoluteF, squareRoot, x) \
//  FType z = absoluteF(x); \
//  z = selectF(z, 1.0f / z, z < 1.0f); \
//  IType n = selectI(broadcast(1), broadcast(2), z < 4.294967040e+09f); \
//  n = selectI(broadcast(0), n, z < 1.844674297e+19f); \
//  z = z + selectF(broadcastF(0.0f), squareRoot(fma(z, z, broadcastF(1.0f))), 1 <= n); \
//  z = z + selectF(broadcastF(0.0f), squareRoot(fma(z, z, broadcastF(1.0f))), 2 <= n); \
//  z = 1.0f / z; \
//  FType y = z / fma(z, z, broadcastF(1.0f)); \
//  { \
//    const FType k = y; \
//    FType t = k; \
//    for (size_t i = 1; i < 5; ++i) { \
//      const float s = (float)(2 * i * i) / (float)((2 * i + 1) * i); \
//      t = (t * z * k) * s; \
//      y = y + t; \
//    } \
//  } \
//  y = y * iToF(broadcast(1) << n); \
//  y = selectF(y, (0.5f * zPiF) - y, 1.0f < absoluteF(x)); \
//  y = selectF(y, -y, x < 0.0f)
//
////! Compute arc tangent
//float zAtanImpl(const float x)
//{
//  ZINVUL_ATAN_IMPL(float, int32b, (float), zSelect, zSelectF,,, zAbsF, zSqrt, x);
//  return y;
//}
//
////! Compute arc tangent
//float2 zAtan2Impl(const float2 x)
//{
//  ZINVUL_ATAN_IMPL(float2, int2, zI2ToF2, zSelect2, zSelectF2, zBroadcast2, zBroadcastF2, zAbsF2, zSqrt2, x);
//  return y;
//}
//
////! Compute arc tangent
//float3 zAtan3Impl(const float3 x)
//{
//  ZINVUL_ATAN_IMPL(float3, int3, zI3ToF3, zSelect3, zSelectF3, zBroadcast3, zBroadcastF3, zAbsF3, zSqrt3, x);
//  return y;
//}
//
////! Compute arc tangent
//float4 zAtan4Impl(const float4 x)
//{
//  ZINVUL_ATAN_IMPL(float4, int4, zI4ToF4, zSelect4, zSelectF4, zBroadcast4, zBroadcastF4, zAbsF4, zSqrt4, x);
//  return y;
//}
//
////! Compute arc tangent
//float zAtan(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float y = atan(x);
//#else
//  const float y = zAtanImpl(x);
//#endif
//  return y;
//}
//
////! Compute arc tangent
//float2 zAtan2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float2 y = atan(x);
//#else
//  const float2 y = zAtan2Impl(x);
//#endif
//  return y;
//}
//
////! Compute arc tangent
//float3 zAtan3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float3 y = atan(x);
//#else
//  const float3 y = zAtan3Impl(x);
//#endif
//  return y;
//}
//
////! Compute arc tangent
//float4 zAtan4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float4 y = atan(x);
//#else
//  const float4 y = zAtan4Impl(x);
//#endif
//  return y;
//}
//
////! Compute arc sine
//#define ZINVUL_ASIN_IMPL(FType, invSquareRoot, arctan, x) \
//  const FType z = x * invSquareRoot((1.0f + x) * (1.0f - x)); \
//  const FType y = arctan(z)
//
////! Compute arc sine
//float zAsinImpl(const float x)
//{
//  ZINVUL_ASIN_IMPL(float, zRsqrt, zAtan, x);
//  return y;
//}
//
////! Compute arc sine
//float2 zAsin2Impl(const float2 x)
//{
//  ZINVUL_ASIN_IMPL(float2, zRsqrt2, zAtan2, x);
//  return y;
//}
//
////! Compute arc sine
//float3 zAsin3Impl(const float3 x)
//{
//  ZINVUL_ASIN_IMPL(float3, zRsqrt3, zAtan3, x);
//  return y;
//}
//
////! Compute arc sine
//float4 zAsin4Impl(const float4 x)
//{
//  ZINVUL_ASIN_IMPL(float4, zRsqrt4, zAtan4, x);
//  return y;
//}
//
////! Compute arc sine
//float zAsin(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float y = asin(x);
//#else
//  const float y = zAsinImpl(x);
//#endif
//  return y;
//}
//
////! Compute arc sine
//float2 zAsin2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float2 y = asin(x);
//#else
//  const float2 y = zAsin2Impl(x);
//#endif
//  return y;
//}
//
////! Compute arc sine
//float3 zAsin3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float3 y = asin(x);
//#else
//  const float3 y = zAsin3Impl(x);
//#endif
//  return y;
//}
//
////! Compute arc sine
//float4 zAsin4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float4 y = asin(x);
//#else
//  const float4 y = zAsin4Impl(x);
//#endif
//  return y;
//}
//
////! Compute arc cosine
//#define ZINVUL_ACOS_IMPL(FType, selectF, broadcastF, squareRoot, arctan, x) \
//  const FType z = squareRoot((1.0f + x) * (1.0f - x)) / x; \
//  const FType y = arctan(z) + selectF(broadcastF(0.0f), broadcastF(zPiF), x < 0.0f); \
//  return y
//
////! Compute arc cosine
//float zAcosImpl(const float x)
//{
//  ZINVUL_ACOS_IMPL(float, zSelectF,, zSqrt, zAtan, x);
//  return y;
//}
//
////! Compute arc cosine
//float2 zAcos2Impl(const float2 x)
//{
//  ZINVUL_ACOS_IMPL(float2, zSelectF2, zBroadcastF2, zSqrt2, zAtan2, x);
//  return y;
//}
//
////! Compute arc cosine
//float3 zAcos3Impl(const float3 x)
//{
//  ZINVUL_ACOS_IMPL(float3, zSelectF3, zBroadcastF3, zSqrt3, zAtan3, x);
//  return y;
//}
//
////! Compute arc cosine
//float4 zAcos4Impl(const float4 x)
//{
//  ZINVUL_ACOS_IMPL(float4, zSelectF4, zBroadcastF4, zSqrt4, zAtan4, x);
//  return y;
//}
////! Compute arc cosine
//float zAcos(const float x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float y = acos(x);
//#else
//  const float y = zAcosImpl(x);
//#endif
//  return y;
//}
//
////! Compute arc cosine
//float2 zAcos2(const float2 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float2 y = acos(x);
//#else
//  const float2 y = zAcos2Impl(x);
//#endif
//  return y;
//}
//
////! Compute arc cosine
//float3 zAcos3(const float3 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float3 y = acos(x);
//#else
//  const float3 y = zAcos3Impl(x);
//#endif
//  return y;
//}
//
////! Compute arc cosine
//float4 zAcos4(const float4 x)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
//  const float4 y = acos(x);
//#else
//  const float4 y = zAcos4Impl(x);
//#endif
//  return y;
//}
//
//// Geometric functions
//
////! Compute dot product
//float zDot2Impl(const float2 p0, const float2 p1)
//{
//  const float2 d = p0 * p1;
//  const float result = d.x + d.y;
//  return result;
//}
//
////! Compute dot product
//float zDot3Impl(const float3 p0, const float3 p1)
//{
//  const float3 d = p0 * p1;
//  const float result = d.x + d.y + d.z;
//  return result;
//}
//
////! Compute dot product
//float zDot3F4Impl(const float4 p0, const float4 p1)
//{
//  const float4 d = p0 * p1;
//  const float result = d.x + d.y + d.z;
//  return result;
//}
//
////! Compute dot product
//float zDot4Impl(const float4 p0, const float4 p1)
//{
//  const float4 d = p0 * p1;
//  const float result = d.x + d.y + d.z + d.w;
//  return result;
//}
//
////! Compute dot product
//float zDot2(const float2 p0, const float2 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = dot(p0, p1);
//#else
//  const float result = zDot2Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Compute dot product
//float zDot3(const float3 p0, const float3 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = dot(p0, p1);
//#else
//  const float result = zDot3Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Compute dot product
//float zDot3F4(const float4 p0, const float4 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float3 q0 = zMakeFloat3(p0.x, p0.y, p0.z);
//  const float3 q1 = zMakeFloat3(p1.x, p1.y, p1.z);
//  const float result = dot(q0, q1);
//#else
//  const float result = zDot3F4Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Compute dot product
//float zDot4(const float4 p0, const float4 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = dot(p0, p1);
//#else
//  const float result = zDot4Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Return the cross product of p0.xyz and p1.xyz
//float3 zCross3Impl(const float3 p0, const float3 p1)
//{
//  const float3 result = zMakeFloat3(p0.y * p1.z - p0.z * p1.y,
//                                    p0.z * p1.x - p0.x * p1.z,
//                                    p0.x * p1.y - p0.y * p1.x);
//  return result;
//}
//
////! Return the cross product of p0.xyz and p1.xyz
//float4 zCross4Impl(const float4 p0, const float4 p1)
//{
//  const float4 result = zMakeFloat4(p0.y * p1.z - p0.z * p1.y,
//                                    p0.z * p1.x - p0.x * p1.z,
//                                    p0.x * p1.y - p0.y * p1.x,
//                                    0.0f);
//  return result;
//}
//
////! Return the cross product of p0.xyz and p1.xyz
//float3 zCross3(const float3 p0, const float3 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float3 result = cross(p0, p1);
//#else
//  const float3 result = zCross3Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Return the cross product of p0.xyz and p1.xyz
//float4 zCross4(const float4 p0, const float4 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float4 result = cross(p0, p1);
//#else
//  const float4 result = zCross4Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Return the length of vector p
//float zLength2Impl(const float2 p)
//{
//  const float d2 = zDot2(p, p);
//  const float result = zSqrt(d2);
//  return result;
//}
//
////! Return the length of vector p
//float zLength3Impl(const float3 p)
//{
//  const float d2 = zDot3(p, p);
//  const float result = zSqrt(d2);
//  return result;
//}
//
////! Return the length of vector p
//float zLength3F4Impl(const float4 p)
//{
//  const float d2 = zDot3F4(p, p);
//  const float result = zSqrt(d2);
//  return result;
//}
//
////! Return the length of vector p
//float zLength4Impl(const float4 p)
//{
//  const float d2 = zDot4(p, p);
//  const float result = zSqrt(d2);
//  return result;
//}
//
////! Return the length of vector p
//float zLength2(const float2 p)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = length(p);
//#else
//  const float result = zLength2Impl(p);
//#endif
//  return result;
//}
//
////! Return the length of vector p
//float zLength3(const float3 p)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = length(p);
//#else
//  const float result = zLength3Impl(p);
//#endif
//  return result;
//}
//
////! Return the length of vector p
//float zLength3F4(const float4 p)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float3 q = zMakeFloat3(p.x, p.y, p.z);
//  const float result = length(q);
//#else
//  const float result = zLength3F4Impl(p);
//#endif
//  return result;
//}
//
////! Return the length of vector p
//float zLength4(const float4 p)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = length(p);
//#else
//  const float result = zLength4Impl(p);
//#endif
//  return result;
//}
//
////! Return the distance between p0 and p1
//float zDistance2Impl(const float2 p0, const float2 p1)
//{
//  const float result = zLength2(p0 - p1);
//  return result;
//}
//
////! Return the distance between p0 and p1
//float zDistance3Impl(const float3 p0, const float3 p1)
//{
//  const float result = zLength3(p0 - p1);
//  return result;
//}
//
////! Return the distance between p0 and p1
//float zDistance3F4Impl(const float4 p0, const float4 p1)
//{
//  const float result = zLength3F4(p0 - p1);
//  return result;
//}
//
////! Return the distance between p0 and p1
//float zDistance4Impl(const float4 p0, const float4 p1)
//{
//  const float result = zLength4(p0 - p1);
//  return result;
//}
//
////! Return the distance between p0 and p1
//float zDistance2(const float2 p0, const float2 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = distance(p0, p1);
//#else
//  const float result = zDistance2Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Return the distance between p0 and p1
//float zDistance3(const float3 p0, const float3 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = distance(p0, p1);
//#else
//  const float result = zDistance3Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Return the distance between p0 and p1
//float zDistance3F4(const float4 p0, const float4 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float3 q0 = zMakeFloat3(p0.x, p0.y, p0.z);
//  const float3 q1 = zMakeFloat3(p1.x, p1.y, p1.z);
//  const float result = distance(q0, q1);
//#else
//  const float result = zDistance3F4Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Return the distance between p0 and p1
//float zDistance4(const float4 p0, const float4 p1)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float result = distance(p0, p1);
//#else
//  const float result = zDistance4Impl(p0, p1);
//#endif
//  return result;
//}
//
////! Return a vector in the same direction as p but with a length of 1
//float2 zNormalize2Impl(const float2 p)
//{
//  const float d2 = zDot2(p, p);
//  const float2 result = p * zRsqrt(d2);
//  return result;
//}
//
////! Return a vector in the same direction as p but with a length of 1
//float3 zNormalize3Impl(const float3 p)
//{
//  const float d2 = zDot3(p, p);
//  const float3 result = p * zRsqrt(d2);
//  return result;
//}
//
////! Return a vector in the same direction as p but with a length of 1
//float4 zNormalize3F4Impl(const float4 p)
//{
//  const float d2 = zDot3F4(p, p);
//  float4 result = p * zRsqrt(d2);
//  result.w = 0.0f;
//  return result;
//}
//
////! Return a vector in the same direction as p but with a length of 1
//float4 zNormalize4Impl(const float4 p)
//{
//  const float d2 = zDot4(p, p);
//  const float4 result = p * zRsqrt(d2);
//  return result;
//}
//
////! Return a vector in the same direction as p but with a length of 1
//float2 zNormalize2(const float2 p)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float2 result = normalize(p);
//#else
//  const float2 result = zNormalize2Impl(p);
//#endif
//  return result;
//}
//
////! Return a vector in the same direction as p but with a length of 1
//float3 zNormalize3(const float3 p)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float3 result = normalize(p);
//#else
//  const float3 result = zNormalize3Impl(p);
//#endif
//  return result;
//}
//
////! Return a vector in the same direction as p but with a length of 1
//float4 zNormalize3F4(const float4 p)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  float4 q = p;
//  q.w = 0.0f;
//  const float4 result = normalize(q);
//#else
//  const float4 result = zNormalize3F4Impl(p);
//#endif
//  return result;
//}
//
////! Return a vector in the same direction as p but with a length of 1
//float4 zNormalize4(const float4 p)
//{
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_GEOMETRY)
//  const float4 result = normalize(p);
//#else
//  const float4 result = zNormalize4Impl(p);
//#endif
//  return result;
//}

} // namespace zinvul

#include "math-inl.cl"

#endif /* ZINVUL_MATH_CL */
