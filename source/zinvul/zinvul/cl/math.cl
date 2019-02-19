/*!
  \file math.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_CL
#define ZINVUL_MATH_CL

#include "relational.cl"
#include "types.cl"
#include "utility.cl"

// Constants
#define zFloatMax 3.402823466e+38f
#define zFloatMin 1.175494351e-38f
#define zFloatEpsilon 1.192092896e-07f
#define zFloatDenormMax 1.175494211e-38f
#define zFloatDenormMin 1.401298464e-45f
#define zEF 2.718281746e+00f //!< The value of 'e'
#define zInvEF 3.678794503e-01f //!< The value of '1 / e'
#define zLog2EF 1.442695022e+00f //!< The value of 'log2(e)'
#define zInvLog2EF 6.931471825e-01f //!< The value of '1 / log2(e)'
#define zLog10EF 4.342944920e-01f //!< The value of 'log10(e)'
#define zInvLog10EF 2.302585125e+00f //!< The value of '1 / log10(e)'
#define zLn2F 6.931471825e-01f //!< The value of 'log(2)'
#define zInvLn2F 1.442695022e+00f //!< The value of '1 / log(2)'
#define zLn10F 2.302585125e+00f //!< The value of 'log(10)'
#define zInvLn10F 4.342944920e-01f //!< The value of '1 / log(10)'
#define zPiF 3.141592741e+00f //!< The value of 'pi'
#define zPi2F 1.570796371e+00f //!< The value of 'pi / 2'
#define zPi4F 7.853981853e-01f //!< The value of 'pi / 4'
#define zInvPiF 3.183098733e-01f //!< The value of '1 / pi'
#define z2InvPiF 6.366197467e-01f //!< The value of '2 / pi'
#define z2InvSqrtPiF 1.128379226e+00f //!< The value of '2 / sqrt(pi)'
#define zSqrt2F 1.414213538e+00f //!< The value of 'sqrt(2)'
#define zInvSqrt2F 7.071067691e-01f //!< The value of '1/sqrt(2)'
#define zSqrt3F 1.732050776e+00f //!< The value of 'sqrt(3)'
#define zInvSqrt3F 5.773502588e-01f //!< The value of '1/sqrt(3)'

#define zSignBitMaskF 0x80000000u
#define zExponentBitMaskF 0x7f800000u
#define zExponentBitSizeF 8
#define zExponentBiasF 127
#define zSignificandBitMaskF 0x7fffffu
#define zSignificandBitSizeF 23

//! Return 1 if x > 0, -1 if x < 0, otherwise 0
#define ZINVUL_SIGN_IMPL(FType, selectF, broadcastF, x) \
  FType y = selectF(broadcastF(0.0f), broadcastF(1.0f), 0.0f < x); \
  y = selectF(y, broadcastF(-1.0f), x < 0.0f)

//! Return 1 if x > 0, -1 if x < 0, otherwise 0
float zSign(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SIGN)
  const float y = sign(x);
#else
  ZINVUL_SIGN_IMPL(float, zSelectF, , x);
#endif
  return y;
}

//! Return 1 if x > 0, -1 if x < 0, otherwise 0
float2 zSign2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SIGN)
  const float2 y = sign(x);
#else
  ZINVUL_SIGN_IMPL(float2, zSelectF2, zBroadcastF2, x);
#endif
  return y;
}

//! Return 1 if x > 0, -1 if x < 0, otherwise 0
float3 zSign3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SIGN)
  const float3 y = sign(x);
#else
  ZINVUL_SIGN_IMPL(float3, zSelectF3, zBroadcastF3, x);
#endif
  return y;
}

//! Return 1 if x > 0, -1 if x < 0, otherwise 0
float4 zSign4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SIGN)
  const float4 y = sign(x);
#else
  ZINVUL_SIGN_IMPL(float4, zSelectF4, zBroadcastF4, x);
#endif
  return y;
}

// Nearest integer floating point operations

//! Return the nearest integer not less than the given value. \todo Implement
#define ZINVUL_ICEIL_IMPL(x) ceil(x)

//! Return the nearest integer not less than the given value
int32b zICeil(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float y = ceil(x);
#else
  const float y = ZINVUL_ICEIL_IMPL(x);
#endif
  return (int32b)y;
}

//! Return the nearest integer not less than the given value
int2 zICeil2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float2 y = ceil(x);
#else
  const float2 y = ZINVUL_ICEIL_IMPL(x);
#endif
  return zF2ToI2(y);
}

//! Return the nearest integer not less than the given value
int3 zICeil3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float3 y = ceil(x);
#else
  const float3 y = ZINVUL_ICEIL_IMPL(x);
#endif
  return zF3ToI3(y);
}

//! Return the nearest integer not less than the given value
int4 zICeil4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float4 y = ceil(x);
#else
  const float4 y = ZINVUL_ICEIL_IMPL(x);
#endif
  return zF4ToI4(y);
}

//! Return the nearest integer not less than the given value. \todo Implement
#define ZINVUL_CEIL_IMPL(x) ceil(x)

//! Return the nearest integer not less than the given value
float zCeil(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float y = ceil(x);
#else
  const float y = ZINVUL_CEIL_IMPL(x);
#endif
  return y;
}

//! Return the nearest integer not less than the given value
float2 zCeil2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float2 y = ceil(x);
#else
  const float2 y = ZINVUL_CEIL_IMPL(x);
#endif
  return y;
}

//! Return the nearest integer not less than the given value
float3 zCeil3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float3 y = ceil(x);
#else
  const float3 y = ZINVUL_CEIL_IMPL(x);
#endif
  return y;
}

//! Return the nearest integer not less than the given value
float4 zCeil4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float4 y = ceil(x);
#else
  const float4 y = ZINVUL_CEIL_IMPL(x);
#endif
  return y;
}

//! Return the nearest integer not greater than the given value. \todo Implement
#define ZINVUL_IFLOOR_IMPL(x) floor(x)

//! Return the nearest integer not greater than the given value
int32b zIFloor(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float y = floor(x);
#else
  const float y = ZINVUL_IFLOOR_IMPL(x);
#endif
  return (int32b)y;
}

//! Return the nearest integer not greater than the given value
int2 zIFloor2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float2 y = floor(x);
#else
  const float2 y = ZINVUL_IFLOOR_IMPL(x);
#endif
  return zF2ToI2(y);
}

//! Return the nearest integer not greater than the given value
int3 zIFloor3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float3 y = floor(x);
#else
  const float3 y = ZINVUL_IFLOOR_IMPL(x);
#endif
  return zF3ToI3(y);
}

//! Return the nearest integer not greater than the given value
int4 zIFloor4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float4 y = floor(x);
#else
  const float4 y = ZINVUL_IFLOOR_IMPL(x);
#endif
  return zF4ToI4(y);
}

//! Return the nearest integer not greater than the given value. \todo Implement
#define ZINVUL_FLOOR_IMPL(x) floor(x)

//! Return the nearest integer not greater than the given value
float zFloor(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float y = floor(x);
#else
  const float y = ZINVUL_FLOOR_IMPL(x);
#endif
  return y;
}

//! Return the nearest integer not greater than the given value
float2 zFloor2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float2 y = floor(x);
#else
  const float2 y = ZINVUL_FLOOR_IMPL(x);
#endif
  return y;
}

//! Return the nearest integer not greater than the given value
float3 zFloor3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float3 y = floor(x);
#else
  const float3 y = ZINVUL_FLOOR_IMPL(x);
#endif
  return y;
}

//! Return the nearest integer not greater than the given value
float4 zFloor4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float4 y = floor(x);
#else
  const float4 y = ZINVUL_FLOOR_IMPL(x);
#endif
  return y;
}

//! Return the nearest integer not greater in magnitude than the given value
int32b zITrunc(const float x)
{
  return (int32b)x;
}

//! Return the nearest integer not greater in magnitude than the given value
int2 zITrunc2(const float2 x)
{
  return zF2ToI2(x);
}

//! Return the nearest integer not greater in magnitude than the given value
int3 zITrunc3(const float3 x)
{
  return zF3ToI3(x);
}

//! Return the nearest integer not greater in magnitude than the given value
int4 zITrunc4(const float4 x)
{
  return zF4ToI4(x);
}

//! Return the nearest integer not greater in magnitude than the given value
#define ZINVUL_TRUNC_IMPL(FType, iToF, selectF, iTrunc, x) \
  FType y = iToF(iTrunc(x)); \
  y = selectF(y, x, isinf(x) || isnan(x))

//! Return the nearest integer not greater in magnitude than the given value
float zTrunc(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float y = trunc(x);
#else
  ZINVUL_TRUNC_IMPL(float, (float), zSelectF, zITrunc, x);
#endif
  return y;
}

//! Return the nearest integer not greater in magnitude than the given value
float2 zTrunc2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float2 y = trunc(x);
#else
  ZINVUL_TRUNC_IMPL(float2, zI2ToF2, zSelectF2, zITrunc2, x);
#endif
  return y;
}

//! Return the nearest integer not greater in magnitude than the given value
float3 zTrunc3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float3 y = trunc(x);
#else
  ZINVUL_TRUNC_IMPL(float3, zI3ToF3, zSelectF3, zITrunc3, x);
#endif
  return y;
}

//! Return the nearest integer not greater in magnitude than the given value
float4 zTrunc4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float4 y = trunc(x);
#else
  ZINVUL_TRUNC_IMPL(float4, zI4ToF4, zSelectF4, zITrunc4, x);
#endif
  return y;
}

//! Return the nearest integer, rounding away from zero in halfway cases
#define ZINVUL_IROUND_IMPL(getSign, x) (getSign(x) * 0.5f + x);

//! Return the nearest integer, rounding away from zero in halfway cases
int32b zIRound(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float y = round(x);
#else
  const float y = ZINVUL_IROUND_IMPL(zSign, x);
#endif
  return (int32b)y;
}

//! Return the nearest integer, rounding away from zero in halfway cases
int2 zIRound2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float2 y = round(x);
#else
  const float2 y = ZINVUL_IROUND_IMPL(zSign2, x);
#endif
  return zF2ToI2(y);
}

//! Return the nearest integer, rounding away from zero in halfway cases
int3 zIRound3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float3 y = round(x);
#else
  const float3 y = ZINVUL_IROUND_IMPL(zSign3, x);
#endif
  return zF3ToI3(y);
}

//! Return the nearest integer, rounding away from zero in halfway cases
int4 zIRound4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float4 y = round(x);
#else
  const float4 y = ZINVUL_IROUND_IMPL(zSign4, x);
#endif
  return zF4ToI4(y);
}

//! Return the nearest integer, rounding away from zero in halfway cases
#define ZINVUL_ROUND_IMPL(FType, iToF, selectF, iRound, x) \
  FType y = iToF(iRound(x)); \
  y = selectF(y, x, isinf(x) || isnan(x))

//! Return the nearest integer, rounding away from zero in halfway cases
float zRound(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float y = round(x);
#else
  ZINVUL_ROUND_IMPL(float, (float), zSelectF, zIRound, x);
#endif
  return y;
}

//! Return the nearest integer, rounding away from zero in halfway cases
float2 zRound2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float2 y = round(x);
#else
  ZINVUL_ROUND_IMPL(float2, zI2ToF2, zSelectF2, zIRound2, x);
#endif
  return y;
}

//! Return the nearest integer, rounding away from zero in halfway cases
float3 zRound3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float3 y = round(x);
#else
  ZINVUL_ROUND_IMPL(float3, zI3ToF3, zSelectF3, zIRound3, x);
#endif
  return y;
}

//! Return the nearest integer, rounding away from zero in halfway cases
float4 zRound4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  const float4 y = round(x);
#else
  ZINVUL_ROUND_IMPL(float4, zI4ToF4, zSelectF4, zIRound4, x);
#endif
  return y;
}

// Basic operations

//! Check if the x is negative
int32b zIsNegative(const int32b x)
{
  const int32b sign_bit = INT_MIN;
  const int32b result = (x & sign_bit) == sign_bit;
  return result;
}

//! Return |x|
uint32b zAbs(const int32b x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
  const uint32b result = abs(x);
#else
  const uint32b result = (uint32b)zSelect(x, -x, x < 0);
#endif
  return result;
}

//! Return |x|
uint2 zAbs2(const int2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
  const uint2 result = abs(x);
#else
  const uint2 result = zI2ToU2(zSelect2(x, -x, x < 0));
#endif
  return result;
}

//! Return |x|
uint3 zAbs3(const int3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
  const uint3 result = abs(x);
#else
  const uint3 result = zI3ToU3(zSelect3(x, -x, x < 0));
#endif
  return result;
}

//! Return |x|
uint4 zAbs4(const int4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
  const uint4 result = abs(x);
#else
  const uint4 result = zI4ToU4(zSelect4(x, -x, x < 0));
#endif
  return result;
}

//! Return |x|
float zAbsF(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
  const float result = fabs(x);
#else
  const float result = zSelectF(x, -x, x < 0.0f);
#endif
  return result;
}

//! Return |x|
float2 zAbsF2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
  const float2 result = fabs(x);
#else
  const float2 result = zSelectF2(x, -x, x < 0.0f);
#endif
  return result;
}

//! Return |x|
float3 zAbsF3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
  const float3 result = fabs(x);
#else
  const float3 result = zSelectF3(x, -x, x < 0.0f);
#endif
  return result;
}

//! Return |x|
float4 zAbsF4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_ABS)
  const float4 result = fabs(x);
#else
  const float4 result = zSelectF4(x, -x, x < 0.0f);
#endif
  return result;
}

//! Check if the given two values are almost same. \todo Support subnormal
#define ZINVUL_IS_ALMOST_SAME_IMPL(FType, IType, absolute, lhs, rhs) \
  const FType e = (4.0f * zFloatEpsilon) * absolute(lhs + rhs); \
  const FType v = absolute(lhs - rhs); \
  const IType result = (v <= e); \
  return result

//! Check if the given two values are almost same
int32b isAlmostSame(const float lhs, const float rhs)
{
  ZINVUL_IS_ALMOST_SAME_IMPL(float, int32b, zAbsF, lhs, rhs);
}

//! Check if the given two values are almost same
int2 isAlmostSame2(const float2 lhs, const float2 rhs)
{
  ZINVUL_IS_ALMOST_SAME_IMPL(float2, int2, zAbsF2, lhs, rhs);
}

//! Check if the given two values are almost same
int3 isAlmostSame3(const float3 lhs, const float3 rhs)
{
  ZINVUL_IS_ALMOST_SAME_IMPL(float3, int3, zAbsF3, lhs, rhs);
}

//! Check if the given two values are almost same
int4 isAlmostSame4(const float4 lhs, const float4 rhs)
{
  ZINVUL_IS_ALMOST_SAME_IMPL(float4, int4, zAbsF4, lhs, rhs);
}

//! Check if the given value is odd
int32b isOdd(const int32b value)
{
  const int32b result = (value & 1) == 1;
  return result;
}

//! Check if the given value is odd
int2 isOdd2(const int2 value)
{
  const int2 result = (value & 1) == 1;
  return result;
}

//! Check if the given value is odd
int3 isOdd3(const int3 value)
{
  const int3 result = (value & 1) == 1;
  return result;
}

//! Check if the given value is odd
int4 isOdd4(const int4 value)
{
  const int4 result = (value & 1) == 1;
  return result;
}

//! Check if the given value is odd
int32b isOddU(const uint32b value)
{
  const int32b result = (value & 1u) == 1u;
  return result;
}

//! Check if the given value is odd
int2 isOddU2(const uint2 value)
{
  const int2 result = (value & 1u) == 1u;
  return result;
}

//! Check if the given value is odd
int3 isOddU3(const uint3 value)
{
  const int3 result = (value & 1u) == 1u;
  return result;
}

//! Check if the given value is odd
int4 isOddU4(const uint4 value)
{
  const int4 result = (value & 1u) == 1u;
  return result;
}

//! Return y if x < y, otherwise it returns x
int32b zMax(const int32b x, const int32b y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const int32b result = max(x, y);
#else
  #if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  //! \todo Optimize zMax on macOS
  const int32b x_is_negative = zIsNegative(x);
  const int32b y_is_negative = zIsNegative(y);
  const int32b result = (x_is_negative && y_is_negative) ? zSelect(x, y, -y < -x) :
                        (x_is_negative || y_is_negative) ? zSelect(x, y, y_is_negative < x_is_negative ) :
                                                           zSelect(x, y, x < y);
  #else
  const int32b result = zSelect(x, y, x < y);
  #endif
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
int2 zMax2(const int2 x, const int2 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const int2 result = max(x, y);
#else
  #if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  //! \todo Optimize zMax on macOS
  const int2 result = zMakeInt2(zMax(x.x, y.x), zMax(x.y, y.y));
  #else
  const int2 result = zSelect2(x, y, x < y);
  #endif
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
int3 zMax3(const int3 x, const int3 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const int3 result = max(x, y);
#else
  #if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  //! \todo Optimize zMax on macOS
  const int3 result = zMakeInt3(zMax(x.x, y.x), zMax(x.y, y.y),
                                zMax(x.z, y.z));
  #else
  const int3 result = zSelect3(x, y, x < y);
  #endif
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
int4 zMax4(const int4 x, const int4 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const int4 result = max(x, y);
#else
  #if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  //! \todo Optimize zMax on macOS
  const int4 result = zMakeInt4(zMax(x.x, y.x), zMax(x.y, y.y),
                                zMax(x.z, y.z), zMax(x.w, y.w));
  #else
  const int4 result = zSelect4(x, y, x < y);
  #endif
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
uint32b zMaxU(const uint32b x, const uint32b y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const uint32b result = max(x, y);
#else
  const uint32b result = zSelectU(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
uint2 zMaxU2(const uint2 x, const uint2 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const uint2 result = max(x, y);
#else
  const uint2 result = zSelectU2(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
uint3 zMaxU3(const uint3 x, const uint3 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const uint3 result = max(x, y);
#else
  const uint3 result = zSelectU3(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
uint4 zMaxU4(const uint4 x, const uint4 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const uint4 result = max(x, y);
#else
  const uint4 result = zSelectU4(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
float zMaxF(const float x, const float y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const float result = max(x, y);
#else
  const float result = zSelectF(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
float2 zMaxF2(const float2 x, const float2 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const float2 result = max(x, y);
#else
  const float2 result = zSelectF2(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
float3 zMaxF3(const float3 x, const float3 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const float3 result = max(x, y);
#else
  const float3 result = zSelectF3(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
float4 zMaxF4(const float4 x, const float4 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const float4 result = max(x, y);
#else
  const float4 result = zSelectF4(x, y, x < y);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
int32b zMin(const int32b x, const int32b y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const int32b result = min(x, y);
#else
  #if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  //! \todo Optimize zMax on macOS
  const int32b x_is_negative = zIsNegative(x);
  const int32b y_is_negative = zIsNegative(y);
  const int32b result = (x_is_negative && y_is_negative) ? zSelect(x, y, -x < -y) :
                        (x_is_negative || y_is_negative) ? zSelect(x, y, x_is_negative < y_is_negative) :
                                                           zSelect(x, y, y < x);
  #else
  const int32b result = zSelect(x, y, y < x);
  #endif
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
int2 zMin2(const int2 x, const int2 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const int2 result = min(x, y);
#else
  #if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  //! \todo Optimize zMax on macOS
  const int2 result = zMakeInt2(zMin(x.x, y.x), zMin(x.y, y.y));
  #else
  const int2 result = zSelect2(x, y, y < x);
  #endif
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
int3 zMin3(const int3 x, const int3 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const int3 result = min(x, y);
#else
  #if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  //! \todo Optimize zMax on macOS
  const int3 result = zMakeInt3(zMin(x.x, y.x), zMin(x.y, y.y),
                                zMin(x.z, y.z));
  #else
  const int3 result = zSelect3(x, y, y < x);
  #endif
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
int4 zMin4(const int4 x, const int4 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const int4 result = min(x, y);
#else
  #if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  //! \todo Optimize zMax on macOS
  const int4 result = zMakeInt4(zMin(x.x, y.x), zMin(x.y, y.y),
                                zMin(x.z, y.z), zMin(x.w, y.w));
  #else
  const int4 result = zSelect4(x, y, y < x);
  #endif
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
uint32b zMinU(const uint32b x, const uint32b y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const uint32b result = min(x, y);
#else
  const uint32b result = zSelectU(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
uint2 zMinU2(const uint2 x, const uint2 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const uint2 result = min(x, y);
#else
  const uint2 result = zSelectU2(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
uint3 zMinU3(const uint3 x, const uint3 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const uint3 result = min(x, y);
#else
  const uint3 result = zSelectU3(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
uint4 zMinU4(const uint4 x, const uint4 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const uint4 result = min(x, y);
#else
  const uint4 result = zSelectU4(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
float zMinF(const float x, const float y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const float result = min(x, y);
#else
  const float result = zSelectF(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
float2 zMinF2(const float2 x, const float2 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const float2 result = min(x, y);
#else
  const float2 result = zSelectF2(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
float3 zMinF3(const float3 x, const float3 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const float3 result = min(x, y);
#else
  const float3 result = zSelectF3(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
float4 zMinF4(const float4 x, const float4 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MINMAX)
  const float4 result = min(x, y);
#else
  const float4 result = zSelectF4(x, y, y < x);
#endif
  return result;
}

#define ZINVUL_MIX_IMPL(x, y, a) (x + a * (y - x))

//! Return the linear blend of x & y implemented as: x + (y - x) * a
float zMix(const float x, const float y, const float a)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
  const float result = mix(x, y, a);
#else
  const float result = ZINVUL_MIX_IMPL(x, y, a);
#endif
  return result;
}

//! Return the linear blend of x & y implemented as: x + (y - x) * a
float2 zMix2(const float2 x, const float2 y, const float2 a)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
  const float2 result = mix(x, y, a);
#else
  const float2 result = ZINVUL_MIX_IMPL(x, y, a);
#endif
  return result;
}

//! Return the linear blend of x & y implemented as: x + (y - x) * a
float2 zMix2F(const float2 x, const float2 y, const float a)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
  const float2 result = mix(x, y, a);
#else
  const float2 result = ZINVUL_MIX_IMPL(x, y, a);
#endif
  return result;
}

//! Return the linear blend of x & y implemented as: x + (y - x) * a
float3 zMix3(const float3 x, const float3 y, const float3 a)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
  const float3 result = mix(x, y, a);
#else
  const float3 result = ZINVUL_MIX_IMPL(x, y, a);
#endif
  return result;
}

//! Return the linear blend of x & y implemented as: x + (y - x) * a
float3 zMix3F(const float3 x, const float3 y, const float a)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
  const float3 result = mix(x, y, a);
#else
  const float3 result = ZINVUL_MIX_IMPL(x, y, a);
#endif
  return result;
}

//! Return the linear blend of x & y implemented as: x + (y - x) * a
float4 zMix4(const float4 x, const float4 y, const float4 a)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
  const float4 result = mix(x, y, a);
#else
  const float4 result = ZINVUL_MIX_IMPL(x, y, a);
#endif
  return result;
}

//! Return the linear blend of x & y implemented as: x + (y - x) * a
float4 zMix4F(const float4 x, const float4 y, const float a)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MIX)
  const float4 result = mix(x, y, a);
#else
  const float4 result = ZINVUL_MIX_IMPL(x, y, a);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
int32b zClamp(const int32b x, const int32b minval, const int32b maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const int32b result = clamp(x, minval, maxval);
#else
  const int32b result = zMin(zMax(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
int2 zClamp2(const int2 x, const int2 minval, const int2 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const int2 result = clamp(x, minval, maxval);
#else
  const int2 result = zMin2(zMax2(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
int3 zClamp3(const int3 x, const int3 minval, const int3 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const int3 result = clamp(x, minval, maxval);
#else
  const int3 result = zMin3(zMax3(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
int4 zClamp4(const int4 x, const int4 minval, const int4 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const int4 result = clamp(x, minval, maxval);
#else
  const int4 result = zMin4(zMax4(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
uint32b zClampU(const uint32b x, const uint32b minval, const uint32b maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const uint32b result = clamp(x, minval, maxval);
#else
  const uint32b result = zMinU(zMaxU(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
uint2 zClampU2(const uint2 x, const uint2 minval, const uint2 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const uint2 result = clamp(x, minval, maxval);
#else
  const uint2 result = zMinU2(zMaxU2(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
uint3 zClampU3(const uint3 x, const uint3 minval, const uint3 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const uint3 result = clamp(x, minval, maxval);
#else
  const uint3 result = zMinU3(zMaxU3(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
uint4 zClampU4(const uint4 x, const uint4 minval, const uint4 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const uint4 result = clamp(x, minval, maxval);
#else
  const uint4 result = zMinU4(zMaxU4(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
float zClampF(const float x, const float minval, const float maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const float result = clamp(x, minval, maxval);
#else
  const float result = zMinF(zMaxF(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
float2 zClampF2(const float2 x, const float2 minval, const float2 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const float2 result = clamp(x, minval, maxval);
#else
  const float2 result = zMinF2(zMaxF2(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
float3 zClampF3(const float3 x, const float3 minval, const float3 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const float3 result = clamp(x, minval, maxval);
#else
  const float3 result = zMinF3(zMaxF3(x, minval), maxval);
#endif
  return result;
}

//! Return min(max(x, minval), maxval)
float4 zClampF4(const float4 x, const float4 minval, const float4 maxval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CLAMP)
  const float4 result = clamp(x, minval, maxval);
#else
  const float4 result = zMinF4(zMaxF4(x, minval), maxval);
#endif
  return result;
}

//! Return remainder of the floating point division operation by 1
#define ZINVUL_FRACT_IMPL(FType, floorF, minF, broadcastF, x, iptr) \
  const FType i = floorF(x); \
  const FType y = minF(x - i, broadcastF(0x1.fffffep-1f)); \
  *iptr = i

//! Return remainder of the floating point division operation by 1
float zFract(const float x, __private float* iptr)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRACT)
  const float y = fract(x, iptr);
#else
  ZINVUL_FRACT_IMPL(float, zFloor, zMinF, , x, iptr);
#endif
  return y;
}

//! Return remainder of the floating point division operation by 1
float2 zFract2(const float2 x, __private float2* iptr)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRACT)
  const float2 y = fract(x, iptr);
#else
  ZINVUL_FRACT_IMPL(float2, zFloor2, zMinF2, zBroadcastF2, x, iptr);
#endif
  return y;
}

//! Return remainder of the floating point division operation by 1
float3 zFract3(const float3 x, __private float3* iptr)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRACT)
  const float3 y = fract(x, iptr);
#else
  ZINVUL_FRACT_IMPL(float3, zFloor3, zMinF3, zBroadcastF3, x, iptr);
#endif
  return y;
}

//! Return remainder of the floating point division operation by 1
float4 zFract4(const float4 x, __private float4* iptr)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRACT)
  const float4 y = fract(x, iptr);
#else
  ZINVUL_FRACT_IMPL(float4, zFloor4, zMinF4, zBroadcastF4, x, iptr);
#endif
  return y;
}

//! Return remainder of the floating point division operation
#define ZINVUL_MOD_IMPL(truncF, x, y) (x - y * truncF(x / y))

//! Return remainder of the floating point division operation
float zMod(const float x, const float y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MOD)
  const float z = fmod(x, y);
#else
  const float z = ZINVUL_MOD_IMPL(zTrunc, x, y);
#endif
  return z;
}

//! Return remainder of the floating point division operation
float2 zMod2(const float2 x, const float2 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MOD)
  const float2 z = fmod(x, y);
#else
  const float2 z = ZINVUL_MOD_IMPL(zTrunc2, x, y);
#endif
  return z;
}

//! Return remainder of the floating point division operation
float3 zMod3(const float3 x, const float3 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MOD)
  const float3 z = fmod(x, y);
#else
  const float3 z = ZINVUL_MOD_IMPL(zTrunc3, x, y);
#endif
  return z;
}

//! Return remainder of the floating point division operation
float4 zMod4(const float4 x, const float4 y)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_MOD)
  const float4 z = fmod(x, y);
#else
  const float4 z = ZINVUL_MOD_IMPL(zTrunc4, x, y);
#endif
  return z;
}

#define ZINVUL_DEGREES_IMPL(r) ((180.0f * zInvPiF) * r);

//! Convert radians to degrees. (180 / pi) * radians
float zDegrees(const float r)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
  const float degree = degrees(r);
#else
  const float degree = ZINVUL_DEGREES_IMPL(r);
#endif
  return degree;
}

//! Convert radians to degrees. (180 / pi) * radians
float2 zDegrees2(const float2 r)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
  const float2 degree = degrees(r);
#else
  const float2 degree = ZINVUL_DEGREES_IMPL(r);
#endif
  return degree;
}

//! Convert radians to degrees. (180 / pi) * radians
float3 zDegrees3(const float3 r)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
  const float3 degree = degrees(r);
#else
  const float3 degree = ZINVUL_DEGREES_IMPL(r);
#endif
  return degree;
}

//! Convert radians to degrees. (180 / pi) * radians
float4 zDegrees4(const float4 r)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
  const float4 degree = degrees(r);
#else
  const float4 degree = ZINVUL_DEGREES_IMPL(r);
#endif
  return degree;
}

#define ZINVUL_RADIANS_IMPL(d) ((zPiF / 180.0f) * d)

//! Convert degrees to radians. (pi / 180) *degrees 
float zRadians(const float d)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
  const float radian = radians(d);
#else
  const float radian = ZINVUL_RADIANS_IMPL(d);
#endif
  return radian;
}

//! Convert degrees to radians. (pi / 180) *degrees 
float2 zRadians2(const float2 d)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
  const float2 radian = radians(d);
#else
  const float2 radian = ZINVUL_RADIANS_IMPL(d);
#endif
  return radian;
}

//! Convert degrees to radians. (pi / 180) *degrees 
float3 zRadians3(const float3 d)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
  const float3 radian = radians(d);
#else
  const float3 radian = ZINVUL_RADIANS_IMPL(d);
#endif
  return radian;
}

//! Convert degrees to radians. (pi / 180) *degrees 
float4 zRadians4(const float4 d)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_RADIAN)
  const float4 radian = radians(d);
#else
  const float4 radian = ZINVUL_RADIANS_IMPL(d);
#endif
  return radian;
}

// Floating point manipulation functions

//! \todo Support subnormal value
#define ZINVUL_FREXP_IMPL(FType, UType, IType, asFType, asUType, uToI, broadcast, selectI, selectF, x, e) \
  const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
  UType data = asUType(x); \
  { \
    IType expt = (uToI(data & zExponentBitMaskF) >> zSignificandBitSizeF) - \
                 (zExponentBiasF - 1); \
    expt = selectI(expt, broadcast(0), is_special); \
    *e = expt; \
  } \
  data = (data & ~zExponentBitMaskF) | 0x3f000000u; \
  const FType y = selectF(asFType(data), x, is_special)

//! Decompose a number into significand and a power of 2
float zFrexp(const float x, __private int32b* e)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  return frexp(x, e);
//#else
  ZINVUL_FREXP_IMPL(float, uint32b, int32b, as_float, as_uint, (int32b),, zSelect, zSelectF, x, e);
//#endif
  return y;
}

//! Decompose a number into significand and a power of 2
float2 zFrexp2(const float2 x, __private int2* e)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  return frexp(x, e);
//#else
  ZINVUL_FREXP_IMPL(float2, uint2, int2, as_float2, as_uint2, zU2ToI2, zBroadcast2, zSelect2, zSelectF2, x, e);
//#endif
  return y;
}

//! Decompose a number into significand and a power of 2
float3 zFrexp3(const float3 x, __private int3* e)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  return frexp(x, e);
//#else
  ZINVUL_FREXP_IMPL(float3, uint3, int3, as_float3, as_uint3, zU3ToI3, zBroadcast3, zSelect3, zSelectF3, x, e);
//#endif
  return y;
}

//! Decompose a number into significand and a power of 2
float4 zFrexp4(const float4 x, __private int4* e)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  return frexp(x, e);
//#else
  ZINVUL_FREXP_IMPL(float4, uint4, int4, as_float4, as_uint4, zU4ToI4, zBroadcast4, zSelect4, zSelectF4, x, e);
//#endif
  return y;
}

//! \todo Support subnormal value
#define ZINVUL_LDEXP_IMPL(FType, IType, asFType, selectF, x, e) \
  const IType data = (e + zExponentBiasF) << zSignificandBitSizeF; \
  FType y = asFType(data) * x; \
  { \
    const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
    y = selectF(y, x, is_special); \
  } \

//! Multiply a number by 2 raised to a power
float zLdexp(const float x, const int32b e)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  return ldexp(x, e);
//#else
  ZINVUL_LDEXP_IMPL(float, int32b, as_float, zSelectF, x, e);
//#endif
  return y;
}

//! Multiply a number by 2 raised to a power
float2 zLdexp2(const float2 x, const int2 e)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  return ldexp(x, e);
//#else
  ZINVUL_LDEXP_IMPL(float2, int2, as_float2, zSelectF2, x, e);
//#endif
  return y;
}

//! Multiply a number by 2 raised to a power
float3 zLdexp3(const float3 x, const int3 e)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  return ldexp(x, e);
//#else
  ZINVUL_LDEXP_IMPL(float3, int3, as_float3, zSelectF3, x, e);
//#endif
  return y;
}

//! Multiply a number by 2 raised to a power
float4 zLdexp4(const float4 x, const int4 e)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_FRLDEXP)
//  return ldexp(x, e);
//#else
  ZINVUL_LDEXP_IMPL(float4, int4, as_float4, zSelectF4, x, e);
//#endif
  return y;
}

// Exponential functions

//! Return e raised to the given power
#define ZINVUL_EXP_IMPL(FType, IType, iToF, broadcastF, selectF, loadExp, iRound, x) \
  const IType q = iRound(zInvLn2F * x); \
  FType z = fma(iToF(q), broadcastF(-0.693145751953125f), x); \
  z = fma(iToF(q), broadcastF(-1.428606765330187045e-06f), z); \
  FType y = broadcastF(0.0f); \
  { \
    FType t = z; \
    for (size_t i = 2; i < 7; ++i) { \
      t = t * (z / (float)i); \
      y = y + t; \
    } \
  } \
  y = 1.0f + z + y; \
  y = loadExp(y, q); \
  y = selectF(y, broadcastF(0.0f), x == -INFINITY)

//! Return e raised to the given power
float zExp(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_EXP)
  const float y = exp(x);
#else
  ZINVUL_EXP_IMPL(float, int32b, (float),, zSelectF, zLdexp, zIRound, x);
#endif
  return y;
}

//! Return e raised to the given power
float2 zExp2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_EXP)
  const float2 y = exp(x);
#else
  ZINVUL_EXP_IMPL(float2, int2, zI2ToF2, zBroadcastF2, zSelectF2, zLdexp2, zIRound2, x);
#endif
  return y;
}

//! Return e raised to the given power
float3 zExp3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_EXP)
  const float3 y = exp(x);
#else
  ZINVUL_EXP_IMPL(float3, int3, zI3ToF3, zBroadcastF3, zSelectF3, zLdexp3, zIRound3, x);
#endif
  return y;
}

//! Return e raised to the given power
float4 zExp4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_EXP)
  const float4 y = exp(x);
#else
  ZINVUL_EXP_IMPL(float4, int4, zI4ToF4, zBroadcastF4, zSelectF4, zLdexp4, zIRound4, x);
#endif
  return y;
}

//! Compute natural logarithm of the given number. \todo Support subnormal
#define ZINVUL_LOG_IMPL(FType, IType, iToF, broadcast, broadcastF, selectI, selectF, fracExp, x, is_base2) \
  IType e = broadcast(0); \
  FType z = fracExp(x, &e); \
  { \
    const IType c = z < zInvSqrt2F; \
    e = selectI(e, e - 1, c); \
    z = selectF(z, 2.0f * z, c); \
  } \
  z = (z - 1.0f) / (z + 1.0f); \
  FType y = broadcastF(0.0f); \
  { \
    FType t = z; \
    for (size_t i = 1; i < 4; ++i) { \
      t = t * z * z; \
      y = y + t / (float)(2 * i + 1); \
    } \
  } \
  y = 2.0f * (z + y); \
  if (is_base2) \
    y = iToF(e) + zInvLn2F * y; \
  else \
    y = zLn2F * iToF(e) + y; \
  { \
    y = selectF(y, broadcastF(-INFINITY), x == 0.0f); \
    y = selectF(y, x, isinf(x)); \
    y = selectF(y, broadcastF(NAN), (x < 0.0f) || isnan(x)); \
  } \

//! Compute natural logarithm of the given number
float zLog(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float y = log(x);
#else
  ZINVUL_LOG_IMPL(float, int32b, (float),,, zSelect, zSelectF, zFrexp, x, 0);
#endif
  return y;
}

//! Compute natural logarithm of the given number
float2 zLogF2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float2 y = log(x);
#else
  ZINVUL_LOG_IMPL(float2, int2, zI2ToF2, zBroadcast2, zBroadcastF2, zSelect2, zSelectF2, zFrexp2, x, 0);
#endif
  return y;
}

//! Compute natural logarithm of the given number
float3 zLogF3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float3 y = log(x);
#else
  ZINVUL_LOG_IMPL(float3, int3, zI3ToF3, zBroadcast3, zBroadcastF3, zSelect3, zSelectF3, zFrexp3, x, 0);
#endif
  return y;
}

//! Compute natural logarithm of the given number
float4 zLogF4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float4 y = log(x);
#else
  ZINVUL_LOG_IMPL(float4, int4, zI4ToF4, zBroadcast4, zBroadcastF4, zSelect4, zSelectF4, zFrexp4, x, 0);
#endif
  return y;
}

//! Compute base 2 logarithm of the given number
float zLog2(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float y = log2(x);
#else
  ZINVUL_LOG_IMPL(float, int32b, (float),,, zSelect, zSelectF, zFrexp, x, 1);
#endif
  return y;
}

//! Compute base 2 logarithm of the given number
float2 zLog2F2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float2 y = log2(x);
#else
  ZINVUL_LOG_IMPL(float2, int2, zI2ToF2, zBroadcast2, zBroadcastF2, zSelect2, zSelectF2, zFrexp2, x, 1);
#endif
  return y;
}

//! Compute base 2 logarithm of the given number
float3 zLog2F3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float3 y = log2(x);
#else
  ZINVUL_LOG_IMPL(float3, int3, zI3ToF3, zBroadcast3, zBroadcastF3, zSelect3, zSelectF3, zFrexp3, x, 1);
#endif
  return y;
}

//! Compute base 2 logarithm of the given number
float4 zLog2F4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float4 y = log2(x);
#else
  ZINVUL_LOG_IMPL(float4, int4, zI4ToF4, zBroadcast4, zBroadcastF4, zSelect4, zSelectF4, zFrexp4, x, 1);
#endif
  return y;
}

// Power functions

//! Raise a number to the given power
float zPow(const float base, const float expt)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float y = pow(base, expt);
#else
  //! \todo Implement custom pow function
  const float y = pow(base, expt);
#endif
  return y;
}

//! Raise a number to the given power
float2 zPow2(const float2 base, const float2 expt)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float2 y = pow(base, expt);
#else
  //! \todo Implement custom pow function
  const float2 y =pow(base, expt);
#endif
  return y;
}

//! Raise a number to the given power
float3 zPow3(const float3 base, const float3 expt)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float3 y = pow(base, expt);
#else
  //! \todo Implement custom pow function
  const float3 y = pow(base, expt);
#endif
  return y;
}

//! Raise a number to the given power
float4 zPow4(const float4 base, const float4 expt)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_LOG)
  const float4 y = pow(base, expt);
#else
  //! \todo Implement custom pow function
  const float4 y = pow(base, expt);
#endif
  return y;
}

//! Raise a number to the given integer power
#define ZINVUL_POWN_IMPL(FType, IType, UType, selectF, broadcastF, absolute, hasTrue, isOdd, base, expt) \
  FType y = base; \
  FType b = selectF(base, 1.0f / base, expt < 0); \
  for (UType e = absolute(expt - 1); hasTrue(0u < e); e = e >> 1u) { \
    y = selectF(y, y * b, isOdd(e)); \
    b = selectF(b, b * b, 1u < e); \
  } \
  y = selectF(y, broadcastF(1.0f), expt == 0); \
  y = selectF(y, 1.0f / base, ((base == 0.0f) || (isinf(base))) && (expt < 0))

//! Raise a number to the given integer power
float zPown(const float base, const int32b expt)
{
  ZINVUL_POWN_IMPL(float, int32b, uint32b, zSelectF,, zAbs,, isOddU, base, expt);
  return y;
}

//! Raise a number to the given integer power
float2 zPown2(const float2 base, const int2 expt)
{
  ZINVUL_POWN_IMPL(float2, int2, uint2, zSelectF2, zBroadcastF2, zAbs2, zHasTrue2, isOddU2, base, expt);
  return y;
}

//! Raise a number to the given integer power
float3 zPown3(const float3 base, const int3 expt)
{
  ZINVUL_POWN_IMPL(float3, int3, uint3, zSelectF3, zBroadcastF3, zAbs3, zHasTrue3, isOddU3, base, expt);
  return y;
}

//! Raise a number to the given integer power
float4 zPown4(const float4 base, const int4 expt)
{
  ZINVUL_POWN_IMPL(float4, int4, uint4, zSelectF4, zBroadcastF4, zAbs4, zHasTrue4, isOddU4, base, expt);
  return y;
}

//! Compute approximate inverse square root
#define ZINVUL_APPROXIMATE_INV_SQRT_IMPL(FType, IType, asFType, asIType, x) \
  const IType data = 0x5f376908 - (asIType(x) >> 1); \
  const FType y = asFType(data)

//! Compute approximate inverse square root
float zApproxRsqrt(const float x)
{
  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float, int32b, as_float, as_int, x);
  return y;
}

//! Compute approximate inverse square root
float2 zApproxRsqrt2(const float2 x)
{
  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float2, int2, as_float2, as_int2, x);
  return y;
}

//! Compute approximate inverse square root
float3 zApproxRsqrt3(const float3 x)
{
  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float3, int3, as_float3, as_int3, x);
  return y;
}

//! Compute approximate inverse square root
float4 zApproxRsqrt4(const float4 x)
{
  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float4, int4, as_float4, as_int4, x);
  return y;
}

//! Compute inverse square root. \todo Support subnormal
#define ZINVUL_INV_SQRT_IMPL(FType, selectF, approxRsqrt, x) \
  const FType z = selectF(x, x * 4.294967296e+09f, x < 2.328306437e-10f); \
  FType y = approxRsqrt(z); \
  { \
    const FType halfx = -0.5f * z; \
    y = y * (halfx * y * y + 1.5008789f); \
    y = y * (halfx * y * y + 1.5000006f); \
    const FType hh = halfx * y * y + 0.5f; \
    y = fma(y, hh, y); \
  } \
  y = selectF(y, y * 6.553600000e+04f, x < 2.328306437e-10f)

//! Compute inverse square root
float zRsqrt(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
  const float y = rsqrt(x);
#else
  ZINVUL_INV_SQRT_IMPL(float, zSelectF, zApproxRsqrt, x);
#endif
  return y;
}

//! Compute inverse square root
float2 zRsqrt2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
  const float2 y = rsqrt(x);
#else
  ZINVUL_INV_SQRT_IMPL(float2, zSelectF2, zApproxRsqrt2, x);
#endif
  return y;
}

//! Compute inverse square root
float3 zRsqrt3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
  const float3 y = rsqrt(x);
#else
  ZINVUL_INV_SQRT_IMPL(float3, zSelectF3, zApproxRsqrt3, x);
#endif
  return y;
}

//! Compute inverse square root
float4 zRsqrt4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
  const float4 y = rsqrt(x);
#else
  ZINVUL_INV_SQRT_IMPL(float4, zSelectF4, zApproxRsqrt4, x);
#endif
  return y;
}

//! Compute square root
#define ZINVUL_SQRT_IMPL(FType, IType, selectF, invSqrt, x) \
  FType y = x * invSqrt(x); \
  { \
    const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
    y = selectF(y, x, is_special); \
  }

//! Compute square root
float zSqrt(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
  const float y = sqrt(x);
#else
  ZINVUL_SQRT_IMPL(float, int32b, zSelectF, zRsqrt, x);
#endif
  return y;
}

//! Compute square root
float2 zSqrt2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
  const float2 y = sqrt(x);
#else
  ZINVUL_SQRT_IMPL(float2, int2, zSelectF2, zRsqrt2, x);
#endif
  return y;
}

//! Compute square root
float3 zSqrt3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
  const float3 y = sqrt(x);
#else
  ZINVUL_SQRT_IMPL(float3, int3, zSelectF3, zRsqrt3, x);
#endif
  return y;
}

//! Compute square root
float4 zSqrt4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_SQRT)
  const float4 y = sqrt(x);
#else
  ZINVUL_SQRT_IMPL(float4, int4, zSelectF4, zRsqrt4, x);
#endif
  return y;
}

//! Compute approximate cubic root
#define ZINVUL_APPROXIMATE_CBRT_IMPL(FType, IType, asFType, asIType, selectF, absoluteF, x) \
  IType data = asIType(absoluteF(x)); \
  data = (data >> 2) + (data >> 4); \
  data = data + (data >> 4); \
  data = data + (data >> 8); \
  data = 0x2a5137a0 + data; \
  FType y = asFType(data); \
  y = selectF(y, -y, x < 0.0f)

//! Compute approximate cubic root
float zApproxCbrt(const float x)
{
  ZINVUL_APPROXIMATE_CBRT_IMPL(float, int32b, as_float, as_int, zSelectF, zAbsF, x);
  return y;
}

//! Compute approximate cubic root
float2 zApproxCbrt2(const float2 x)
{
  ZINVUL_APPROXIMATE_CBRT_IMPL(float2, int2, as_float2, as_int2, zSelectF2, zAbsF2, x);
  return y;
}

//! Compute approximate cubic root
float3 zApproxCbrt3(const float3 x)
{
  ZINVUL_APPROXIMATE_CBRT_IMPL(float3, int3, as_float3, as_int3, zSelectF3, zAbsF3, x);
  return y;
}

//! Compute approximate cubic root
float4 zApproxCbrt4(const float4 x)
{
  ZINVUL_APPROXIMATE_CBRT_IMPL(float4, int4, as_float4, as_int4, zSelectF4, zAbsF4, x);
  return y;
}

//! Compute cubic root
#define ZINVUL_CBRT_IMPL(FType, IType, broadcastF, selectF, approxCbrt, x) \
  const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
  const FType z = selectF(x, broadcastF(1.0f), is_special); \
  FType y = approxCbrt(z); \
  { \
    const FType a = 1.5f * z; \
    for (size_t i = 0; i < 2; ++i) { \
      const FType t = fma(2.0f * y, y * y, z); \
      y = y * (0.5f + a / t); \
    } \
  } \
  y = selectF(y, x, is_special)

//! Compute cubic root
float zCbrt(const float x)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CBRT)
//  const float y = cbrt(x);
//#else
  ZINVUL_CBRT_IMPL(float, int32b,, zSelectF, zApproxCbrt, x);
//#endif
  return y;
}

//! Compute cubic root
float2 zCbrt2(const float2 x)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CBRT)
//  const float2 y = cbrt(x);
//#else
  ZINVUL_CBRT_IMPL(float2, int2, zBroadcastF2, zSelectF2, zApproxCbrt2, x);
//#endif
  return y;
}

//! Compute cubic root
float3 zCbrt3(const float3 x)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CBRT)
//  const float3 y = cbrt(x);
//#else
  ZINVUL_CBRT_IMPL(float3, int3, zBroadcastF3, zSelectF3, zApproxCbrt3, x);
//#endif
  return y;
}

//! Compute cubic root
float4 zCbrt4(const float4 x)
{
//! \todo Remove comment out
//#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_CBRT)
//  const float4 y = cbrt(x);
//#else
  ZINVUL_CBRT_IMPL(float4, int4, zBroadcastF4, zSelectF4, zApproxCbrt4, x);
//#endif
  return y;
}

// Trigonometric functions

//! Compute sine
#define ZINVUL_SINCOS_IMPL(FType, IType, iToF, fToI, broadcastF, selectF, absoluteF, squareRoot, theta, trigonometric_type) \
  FType y = absoluteF(theta); \
  const IType q = fToI((4.0f / zPiF) * y); \
  { \
    const FType r = iToF(q + (q & 1)); \
    y = fma(r, broadcastF(-0.25f * 3.140625f), y); \
    y = fma(r, broadcastF(-0.25f * 0.0009670257568359375f), y); \
    y = fma(r, broadcastF(-0.25f * 6.2771141529083251953e-07f), y); \
    y = fma(r, broadcastF(-0.25f * 1.2154201256553420762e-10f), y); \
  } \
  FType c = broadcastF(0.0f); \
  { \
    y = 0.125f * y; \
    y = y * y; \
    FType t = broadcastF(-2.0f); \
    for (size_t i = 1; i < 4; ++i) { \
      t = -t * y / (float)((2 * i) * (2 * i - 1)); \
      c = c + t; \
    } \
    for (size_t i = 0; i < 3; ++i) \
      c = (4.0f - c) * c; \
    c = 0.5f * c; \
  } \
  FType s = squareRoot((2.0f - c) * c); \
  c = 1.0f - c; \
  const IType condition = ((q + 1) & 2) != 0; \
  if (trigonometric_type == 0) { \
    s = selectF(s, c, condition); \
    y = selectF(s, -s, ((q & 4) != 0) != (theta < 0.0f)); \
  } \
  else if (trigonometric_type == 1) {\
    c = selectF(c, s, condition); \
    y = selectF(c, -c, ((q + 2) & 4) != 0); \
  } \
  else { \
    y = s; \
    s = selectF(s, c, condition); \
    c = selectF(c, y, condition); \
    s = selectF(s, -s, ((q & 4) != 0) != (theta < 0.0f)); \
    c = selectF(c, -c, ((q + 2) & 4) != 0); \
  }

//! Compute sine
float zSin(const float theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float y = sin(theta);
#else
  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 0);
#endif
  return y;
}

//! Compute sine
float2 zSin2(const float2 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float2 y = sin(theta);
#else
  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 0);
#endif
  return y;
}

//! Compute sine
float3 zSin3(const float3 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float3 y = sin(theta);
#else
  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 0);
#endif
  return y;
}

//! Compute sine
float4 zSin4(const float4 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float4 y = sin(theta);
#else
  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 0);
#endif
  return y;
}

//! Compute cosine
float zCos(const float theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float y = cos(theta);
#else
  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 1);
#endif
  return y;
}

//! Compute cosine
float2 zCos2(const float2 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float2 y = cos(theta);
#else
  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 1);
#endif
  return y;
}

//! Compute cosine
float3 zCos3(const float3 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float3 y = cos(theta);
#else
  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 1);
#endif
  return y;
}

//! Compute cosine
float4 zCos4(const float4 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float4 y = cos(theta);
#else
  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 1);
#endif
  return y;
}

//! Compute sine and cosine
float zSincos(const float theta, __private float* cosval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float y = sincos(theta, cosval);
  *cosval = cos(theta);
  const float y = sin(theta);
#else
  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 2);
  y = s;
  *cosval = c;
#endif
  return y;
}

//! Compute sine and cosine
float2 zSincos2(const float2 theta, __private float2* cosval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float2 y = sincos(theta, cosval);
  *cosval = cos(theta);
  const float2 y = sin(theta);
#else
  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 2);
  y = s;
  *cosval = c;
#endif
  return y;
}

//! Compute sine and cosine
float3 zSincos3(const float3 theta, __private float3* cosval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float3 y = sincos(theta, cosval);
  *cosval = cos(theta);
  const float3 y = sin(theta);
#else
  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 2);
  y = s;
  *cosval = c;
#endif
  return y;
}

//! Compute sine and cosine
float4 zSincos4(const float4 theta, __private float4* cosval)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
//  const float4 y = sincos(theta, cosval);
  *cosval = cos(theta);
  const float4 y = sin(theta);
#else
  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 2);
  y = s;
  *cosval = c;
#endif
  return y;
}

//! Compute tangent
float zTan(const float theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float y = tan(theta);
#else
  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 2);
  y = s / c;
#endif
  return y;
}

//! Compute tangent
float2 zTan2(const float2 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float2 y = tan(theta);
#else
  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 2);
  y = s / c;
#endif
  return y;
}

//! Compute tangent
float3 zTan3(const float3 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float3 y = tan(theta);
#else
  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 2);
  y = s / c;
#endif
  return y;
}

//! Compute tangent
float4 zTan4(const float4 theta)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  const float4 y = tan(theta);
#else
  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 2);
  y = s / c;
#endif
  return y;
}

//! Compute arc tangent
#define ZINVUL_ATAN_IMPL(FType, IType, iToF, selectI, selectF, broadcast, broadcastF, absoluteF, squareRoot, x) \
  FType z = absoluteF(x); \
  z = selectF(z, 1.0f / z, z < 1.0f); \
  IType n = selectI(broadcast(1), broadcast(2), z < 4.294967040e+09f); \
  n = selectI(broadcast(0), n, z < 1.844674297e+19f); \
  z = z + selectF(broadcastF(0.0f), squareRoot(fma(z, z, broadcastF(1.0f))), 1 <= n); \
  z = z + selectF(broadcastF(0.0f), squareRoot(fma(z, z, broadcastF(1.0f))), 2 <= n); \
  z = 1.0f / z; \
  FType y = z / (1.0f + z * z); \
  { \
    const FType k = y; \
    FType t = k; \
    for (size_t i = 1; i < 5; ++i) { \
      const float s = (float)(2 * i * i) / (float)((2 * i + 1) * i); \
      t = (t * z * k) * s; \
      y = y + t; \
    } \
  } \
  y = y * iToF(broadcast(1) << n); \
  y = selectF(y, (0.5f * zPiF) - y, 1.0f < absoluteF(x)); \
  y = selectF(y, -y, x < 0.0f)

//! Compute arc tangent
float zAtan(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float y = atan(x);
#else
  ZINVUL_ATAN_IMPL(float, int32b, (float), zSelect, zSelectF,,, zAbsF, zSqrt, x);
#endif
  return y;
}

//! Compute arc tangent
float2 zAtan2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float2 y = atan(x);
#else
  ZINVUL_ATAN_IMPL(float2, int2, zI2ToF2, zSelect2, zSelectF2, zBroadcast2, zBroadcastF2, zAbsF2, zSqrt2, x);
#endif
  return y;
}

//! Compute arc tangent
float3 zAtan3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float3 y = atan(x);
#else
  ZINVUL_ATAN_IMPL(float3, int3, zI3ToF3, zSelect3, zSelectF3, zBroadcast3, zBroadcastF3, zAbsF3, zSqrt3, x);
#endif
  return y;
}

//! Compute arc tangent
float4 zAtan4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float4 y = atan(x);
#else
  ZINVUL_ATAN_IMPL(float4, int4, zI4ToF4, zSelect4, zSelectF4, zBroadcast4, zBroadcastF4, zAbsF4, zSqrt4, x);
#endif
  return y;
}

//! Compute arc sine
#define ZINVUL_ASIN_IMPL(FType, squareRoot, arctan, x) \
  const FType z = x / squareRoot((1.0f + x) * (1.0f - x)); \
  const FType y = arctan(z)

//! Compute arc sine
float zAsin(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float y = asin(x);
#else
  ZINVUL_ASIN_IMPL(float, zSqrt, zAtan, x);
#endif
  return y;
}

//! Compute arc sine
float2 zAsin2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float2 y = asin(x);
#else
  ZINVUL_ASIN_IMPL(float2, zSqrt2, zAtan2, x);
#endif
  return y;
}

//! Compute arc sine
float3 zAsin3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float3 y = asin(x);
#else
  ZINVUL_ASIN_IMPL(float3, zSqrt3, zAtan3, x);
#endif
  return y;
}

//! Compute arc sine
float4 zAsin4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float4 y = asin(x);
#else
  ZINVUL_ASIN_IMPL(float4, zSqrt4, zAtan4, x);
#endif
  return y;
}

//! Compute arc cosine
#define ZINVUL_ACOS_IMPL(FType, selectF, broadcastF, squareRoot, arctan, x) \
  const FType z = squareRoot((1.0f + x) * (1.0f - x)) / x; \
  const FType y = arctan(z) + selectF(broadcastF(0.0f), broadcastF(zPiF), x < 0.0f); \
  return y

//! Compute arc cosine
float zAcos(const float x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float y = acos(x);
#else
  ZINVUL_ACOS_IMPL(float, zSelectF,, zSqrt, zAtan, x);
#endif
  return y;
}

//! Compute arc cosine
float2 zAcos2(const float2 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float2 y = acos(x);
#else
  ZINVUL_ACOS_IMPL(float2, zSelectF2, zBroadcastF2, zSqrt2, zAtan2, x);
#endif
  return y;
}

//! Compute arc cosine
float3 zAcos3(const float3 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float3 y = acos(x);
#else
  ZINVUL_ACOS_IMPL(float3, zSelectF3, zBroadcastF3, zSqrt3, zAtan3, x);
#endif
  return y;
}

//! Compute arc cosine
float4 zAcos4(const float4 x)
{
#if defined(ZINVUL_MATH_BUILTIN) || defined(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  const float4 y = acos(x);
#else
  ZINVUL_ACOS_IMPL(float4, zSelectF4, zBroadcastF4, zSqrt4, zAtan4, x);
#endif
  return y;
}

#endif /* ZINVUL_MATH_CL */
