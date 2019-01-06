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
#define zDenormMinF 1.401298464e-45f
#define zPiF 3.141592741f
#define zInvPiF (1.0f / 3.141592741f)
#define zSignBitMaskF 0x80000000u
#define zExponentBitMaskF 0x7f800000u
#define zExponentBitSizeF 8
#define zExponentBiasF 127
#define zSignificandBitMaskF 0x7fffffu
#define zSignificandBitSizeF 23

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
  const int32b result = zSelect(x, -x, x < 0);
  return (uint32b)result;
}

//! Return |x|
uint2 zAbs2(const int2 x)
{
  const int2 result = zSelect2(x, -x, x < 0);
  return zI2ToU2(result);
}

//! Return |x|
uint3 zAbs3(const int3 x)
{
  const int3 result = zSelect3(x, -x, x < 0);
  return zI3ToU3(result);
}

//! Return |x|
uint4 zAbs4(const int4 x)
{
  const int4 result = zSelect4(x, -x, x < 0);
  return zI4ToU4(result);
}

//! Return |x|
float zAbsF(const float x)
{
  const float result = zSelectF(x, -x, x < 0.0f);
  return result;
}

//! Return |x|
float2 zAbsF2(const float2 x)
{
  const float2 result = zSelectF2(x, -x, x < 0.0f);
  return result;
}

//! Return |x|
float3 zAbsF3(const float3 x)
{
  const float3 result = zSelectF3(x, -x, x < 0.0f);
  return result;
}

//! Return |x|
float4 zAbsF4(const float4 x)
{
  const float4 result = zSelectF4(x, -x, x < 0.0f);
  return result;
}

//! Check if the given two values are almost same. \todo Support subnormal
#define ZINVUL_IS_ALMOST_SAME_IMPL(FType, IType, absolute, lhs, rhs) \
  const FType e = (4.0f * FLT_EPSILON) * absolute(lhs + rhs); \
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
  //! \todo Optimize zMax on macOS
#if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  const int32b x_is_negative = zIsNegative(x);
  const int32b y_is_negative = zIsNegative(y);
  const int32b result = (x_is_negative && y_is_negative) ? zSelect(x, y, -y < -x) :
                        (x_is_negative || y_is_negative) ? zSelect(x, y, y_is_negative < x_is_negative ) :
                                                           zSelect(x, y, x < y);
#else
  const int32b result = zSelect(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
int2 zMax2(const int2 x, const int2 y)
{
  //! \todo Optimize zMax on macOS
#if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  const int2 result = zMakeInt2(zMax(x.x, y.x), zMax(x.y, y.y));
#else
  const int2 result = zSelect2(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
int3 zMax3(const int3 x, const int3 y)
{
  //! \todo Optimize zMax on macOS
#if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  const int3 result = zMakeInt3(zMax(x.x, y.x), zMax(x.y, y.y),
                                zMax(x.z, y.z));
#else
  const int3 result = zSelect3(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
int4 zMax4(const int4 x, const int4 y)
{
  //! \todo Optimize zMax on macOS
#if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  const int4 result = zMakeInt4(zMax(x.x, y.x), zMax(x.y, y.y),
                                zMax(x.z, y.z), zMax(x.w, y.w));
#else
  const int4 result = zSelect4(x, y, x < y);
#endif
  return result;
}

//! Return y if x < y, otherwise it returns x
uint32b zMaxU(const uint32b x, const uint32b y)
{
  const uint32b result = zSelectU(x, y, x < y);
  return result;
}

//! Return y if x < y, otherwise it returns x
uint2 zMaxU2(const uint2 x, const uint2 y)
{
  const uint2 result = zSelectU2(x, y, x < y);
  return result;
}

//! Return y if x < y, otherwise it returns x
uint3 zMaxU3(const uint3 x, const uint3 y)
{
  const uint3 result = zSelectU3(x, y, x < y);
  return result;
}

//! Return y if x < y, otherwise it returns x
uint4 zMaxU4(const uint4 x, const uint4 y)
{
  const uint4 result = zSelectU4(x, y, x < y);
  return result;
}

//! Return y if x < y, otherwise it returns x
float zMaxF(const float x, const float y)
{
  const float result = zSelectF(x, y, x < y);
  return result;
}

//! Return y if x < y, otherwise it returns x
float2 zMaxF2(const float2 x, const float2 y)
{
  const float2 result = zSelectF2(x, y, x < y);
  return result;
}

//! Return y if x < y, otherwise it returns x
float3 zMaxF3(const float3 x, const float3 y)
{
  const float3 result = zSelectF3(x, y, x < y);
  return result;
}

//! Return y if x < y, otherwise it returns x
float4 zMaxF4(const float4 x, const float4 y)
{
  const float4 result = zSelectF4(x, y, x < y);
  return result;
}

//! Return y if y < x, otherwise it returns x
int32b zMin(const int32b x, const int32b y)
{
  //! \todo Optimize zMax on macOS
#if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  const int32b x_is_negative = zIsNegative(x);
  const int32b y_is_negative = zIsNegative(y);
  const int32b result = (x_is_negative && y_is_negative) ? zSelect(x, y, -x < -y) :
                        (x_is_negative || y_is_negative) ? zSelect(x, y, x_is_negative < y_is_negative) :
                                                           zSelect(x, y, y < x);
#else
  const int32b result = zSelect(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
int2 zMin2(const int2 x, const int2 y)
{
  //! \todo Optimize zMax on macOS
#if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  const int2 result = zMakeInt2(zMin(x.x, y.x), zMin(x.y, y.y));
#else
  const int2 result = zSelect2(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
int3 zMin3(const int3 x, const int3 y)
{
  //! \todo Optimize zMax on macOS
#if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  const int3 result = zMakeInt3(zMin(x.x, y.x), zMin(x.y, y.y),
                                zMin(x.z, y.z));
#else
  const int3 result = zSelect3(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
int4 zMin4(const int4 x, const int4 y)
{
  //! \todo Optimize zMax on macOS
#if defined(ZINVUL_VULKAN) && defined(Z_MAC)
  const int4 result = zMakeInt4(zMin(x.x, y.x), zMin(x.y, y.y),
                                zMin(x.z, y.z), zMin(x.w, y.w));
#else
  const int4 result = zSelect4(x, y, y < x);
#endif
  return result;
}

//! Return y if y < x, otherwise it returns x
uint32b zMinU(const uint32b x, const uint32b y)
{
  const uint32b result = zSelectU(x, y, y < x);
  return result;
}

//! Return y if y < x, otherwise it returns x
uint2 zMinU2(const uint2 x, const uint2 y)
{
  const uint2 result = zSelectU2(x, y, y < x);
  return result;
}

//! Return y if y < x, otherwise it returns x
uint3 zMinU3(const uint3 x, const uint3 y)
{
  const uint3 result = zSelectU3(x, y, y < x);
  return result;
}

//! Return y if y < x, otherwise it returns x
uint4 zMinU4(const uint4 x, const uint4 y)
{
  const uint4 result = zSelectU4(x, y, y < x);
  return result;
}

//! Return y if y < x, otherwise it returns x
float zMinF(const float x, const float y)
{
  const float result = zSelectF(x, y, y < x);
  return result;
}

//! Return y if y < x, otherwise it returns x
float2 zMinF2(const float2 x, const float2 y)
{
  const float2 result = zSelectF2(x, y, y < x);
  return result;
}

//! Return y if y < x, otherwise it returns x
float3 zMinF3(const float3 x, const float3 y)
{
  const float3 result = zSelectF3(x, y, y < x);
  return result;
}

//! Return y if y < x, otherwise it returns x
float4 zMinF4(const float4 x, const float4 y)
{
  const float4 result = zSelectF4(x, y, y < x);
  return result;
}

//! Return min(max(x, minval), maxval)
int32b zClamp(const int32b x, const int32b minval, const int32b maxval)
{
  const int32b result = zMin(zMax(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
int2 zClamp2(const int2 x, const int2 minval, const int2 maxval)
{
  const int2 result = zMin2(zMax2(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
int3 zClamp3(const int3 x, const int3 minval, const int3 maxval)
{
  const int3 result = zMin3(zMax3(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
int4 zClamp4(const int4 x, const int4 minval, const int4 maxval)
{
  const int4 result = zMin4(zMax4(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
uint32b zClampU(const uint32b x, const uint32b minval, const uint32b maxval)
{
  const uint32b result = zMinU(zMaxU(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
uint2 zClampU2(const uint2 x, const uint2 minval, const uint2 maxval)
{
  const uint2 result = zMinU2(zMaxU2(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
uint3 zClampU3(const uint3 x, const uint3 minval, const uint3 maxval)
{
  const uint3 result = zMinU3(zMaxU3(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
uint4 zClampU4(const uint4 x, const uint4 minval, const uint4 maxval)
{
  const uint4 result = zMinU4(zMaxU4(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
float zClampF(const float x, const float minval, const float maxval)
{
  const float result = zMinF(zMaxF(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
float2 zClampF2(const float2 x, const float2 minval, const float2 maxval)
{
  const float2 result = zMinF2(zMaxF2(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
float3 zClampF3(const float3 x, const float3 minval, const float3 maxval)
{
  const float3 result = zMinF3(zMaxF3(x, minval), maxval);
  return result;
}

//! Return min(max(x, minval), maxval)
float4 zClampF4(const float4 x, const float4 minval, const float4 maxval)
{
  const float4 result = zMinF4(zMaxF4(x, minval), maxval);
  return result;
}

//! Convert radians to degrees. (180 / pi) * radians
float zDegreesF(const float r)
{
  const float k = 180.0f * zInvPiF;
  const float degree = k * r;
  return degree;
}

//! Convert radians to degrees. (180 / pi) * radians
float2 zDegreesF2(const float2 r)
{
  const float k = 180.0f * zInvPiF;
  const float2 degree = k * r;
  return degree;
}

//! Convert radians to degrees. (180 / pi) * radians
float3 zDegreesF3(const float3 r)
{
  const float k = 180.0f * zInvPiF;
  const float3 degree = k * r;
  return degree;
}

//! Convert radians to degrees. (180 / pi) * radians
float4 zDegreesF4(const float4 r)
{
  const float k = 180.0f * zInvPiF;
  const float4 degree = k * r;
  return degree;
}

//! Convert degrees to radians. (pi / 180) *degrees 
float zRadiansF(const float d)
{
  const float k = zPiF / 180.0f;
  const float radian = k * d;
  return radian;
}

//! Convert degrees to radians. (pi / 180) *degrees 
float2 zRadiansF2(const float2 d)
{
  const float k = zPiF / 180.0f;
  const float2 radian = k * d;
  return radian;
}

//! Convert degrees to radians. (pi / 180) *degrees 
float3 zRadiansF3(const float3 d)
{
  const float k = zPiF / 180.0f;
  const float3 radian = k * d;
  return radian;
}

//! Convert degrees to radians. (pi / 180) *degrees 
float4 zRadiansF4(const float4 d)
{
  const float k = zPiF / 180.0f;
  const float4 radian = k * d;
  return radian;
}

// Floating point manipulation functions

//! \todo Support subnormal value
#define ZINVUL_FREXP_IMPL(FType, UType, IType, uToI, broadcast, selectI, selectF, x, e) \
  union Float \
  { \
    FType value_; \
    UType data_; \
  }; \
  union Float y = {x}; \
  const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
  { \
    IType expt = (uToI(y.data_ & zExponentBitMaskF) >> zSignificandBitSizeF) - \
                 (zExponentBiasF - 1); \
    expt = selectI(expt, broadcast(0), is_special); \
    *e = expt; \
  } \
  y.data_ = (y.data_ & ~zExponentBitMaskF) | 0x3f000000u; \
  y.value_ = selectF(y.value_, x, is_special); \
  return y.value_

//! Decompose a number into significand and a power of 2
float zFrexp(const float x, __private int32b* e)
{
  ZINVUL_FREXP_IMPL(float, uint32b, int32b, (int32b),, zSelect, zSelectF, x, e);
}

//! Decompose a number into significand and a power of 2
float2 zFrexp2(const float2 x, __private int2* e)
{
  ZINVUL_FREXP_IMPL(float2, uint2, int2, zU2ToI2, zBroadcast2, zSelect2, zSelectF2, x, e);
}

//! Decompose a number into significand and a power of 2
float3 zFrexp3(const float3 x, __private int3* e)
{
  ZINVUL_FREXP_IMPL(float3, uint3, int3, zU3ToI3, zBroadcast3, zSelect3, zSelectF3, x, e);
}

//! Decompose a number into significand and a power of 2
float4 zFrexp4(const float4 x, __private int4* e)
{
  ZINVUL_FREXP_IMPL(float4, uint4, int4, zU4ToI4, zBroadcast4, zSelect4, zSelectF4, x, e);
}

//! \todo Support subnormal value
#define ZINVUL_LDEXP_IMPL(FType, IType, selectF, x, e) \
  union Float \
  { \
    IType data_; \
    FType value_; \
  }; \
  union Float y = {(e + zExponentBiasF) << zSignificandBitSizeF}; \
  y.value_ = x * y.value_; \
  { \
    const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
    y.value_ = selectF(y.value_, x, is_special); \
  } \
  return y.value_

//! Multiply a number by 2 raised to a power
float zLdexp(const float x, const int32b e)
{
  ZINVUL_LDEXP_IMPL(float, int32b, zSelectF, x, e);
}

//! Multiply a number by 2 raised to a power
float2 zLdexp2(const float2 x, const int2 e)
{
  ZINVUL_LDEXP_IMPL(float2, int2, zSelectF2, x, e);
}

//! Multiply a number by 2 raised to a power
float3 zLdexp3(const float3 x, const int3 e)
{
  ZINVUL_LDEXP_IMPL(float3, int3, zSelectF3, x, e);
}

//! Multiply a number by 2 raised to a power
float4 zLdexp4(const float4 x, const int4 e)
{
  ZINVUL_LDEXP_IMPL(float4, int4, zSelectF4, x, e);
}

// Nearest integer floating point operations

//! Return nearest integer
int32b zRint(float x)
{
  x = zSelectF(x + 0.5f, x - 0.5f, x < 0.0f);
  const int32b result = (int32b)x;
  return result;
}

//! Return nearest integer
int2 zRint2(float2 x)
{
  x = zSelectF2(x + 0.5f, x - 0.5f, x < 0.0f);
  const int2 result = zF2ToI2(x);
  return result;
}

//! Return nearest integer
int3 zRint3(float3 x)
{
  x = zSelectF3(x + 0.5f, x - 0.5f, x < 0.0f);
  const int3 result = zF3ToI3(x);
  return result;
}

//! Return nearest integer
int4 zRint4(float4 x)
{
  x = zSelectF4(x + 0.5f, x - 0.5f, x < 0.0f);
  const int4 result = zF4ToI4(x);
  return result;
}

// Exponential functions

//! Return e raised to the given power
#define ZINVUL_EXP_IMPL(FType, IType, iToF, broadcastF, selectF, loadExp, roundInt, x) \
  const IType q = roundInt(x * (1.0f / M_LN2_F)); \
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
  y = selectF(y, broadcastF(0.0f), x == -INFINITY); \
  return y

//! Return e raised to the given power
float zExp(const float x)
{
  ZINVUL_EXP_IMPL(float, int32b, (float),, zSelectF, zLdexp, zRint, x);
}

//! Return e raised to the given power
float2 zExp2(const float2 x)
{
  ZINVUL_EXP_IMPL(float2, int2, zI2ToF2, zBroadcastF2, zSelectF2, zLdexp2, zRint2, x);
}

//! Return e raised to the given power
float3 zExp3(const float3 x)
{
  ZINVUL_EXP_IMPL(float3, int3, zI3ToF3, zBroadcastF3, zSelectF3, zLdexp3, zRint3, x);
}

//! Return e raised to the given power
float4 zExp4(const float4 x)
{
  ZINVUL_EXP_IMPL(float4, int4, zI4ToF4, zBroadcastF4, zSelectF4, zLdexp4, zRint4, x);
}

//! Compute natural logarithm of the given number. \todo Support subnormal
#define ZINVUL_LOG_IMPL(FType, IType, iToF, broadcast, broadcastF, selectI, selectF, fracExp, x, is_base2) \
  IType e = broadcast(0); \
  FType z = fracExp(x, &e); \
  { \
    const IType c = z < 0.7071f; \
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
    y = iToF(e) + y / M_LN2_F; \
  else \
    y = M_LN2_F * iToF(e) + y; \
  { \
    y = selectF(y, broadcastF(-INFINITY), x == 0.0f); \
    y = selectF(y, x, isinf(x)); \
    y = selectF(y, broadcastF(NAN), (x < 0.0f) || isnan(x)); \
  } \
  return y

//! Compute natural logarithm of the given number
float zLog(const float x)
{
  ZINVUL_LOG_IMPL(float, int32b, (float),,, zSelect, zSelectF, zFrexp, x, 0);
}

//! Compute natural logarithm of the given number
float2 zLogF2(const float2 x)
{
  ZINVUL_LOG_IMPL(float2, int2, zI2ToF2, zBroadcast2, zBroadcastF2, zSelect2, zSelectF2, zFrexp2, x, 0);
}

//! Compute natural logarithm of the given number
float3 zLogF3(const float3 x)
{
  ZINVUL_LOG_IMPL(float3, int3, zI3ToF3, zBroadcast3, zBroadcastF3, zSelect3, zSelectF3, zFrexp3, x, 0);
}

//! Compute natural logarithm of the given number
float4 zLogF4(const float4 x)
{
  ZINVUL_LOG_IMPL(float4, int4, zI4ToF4, zBroadcast4, zBroadcastF4, zSelect4, zSelectF4, zFrexp4, x, 0);
}

//! Compute base 2 logarithm of the given number
float zLog2(const float x)
{
  ZINVUL_LOG_IMPL(float, int32b, (float),,, zSelect, zSelectF, zFrexp, x, 1);
}

//! Compute base 2 logarithm of the given number
float2 zLog2F2(const float2 x)
{
  ZINVUL_LOG_IMPL(float2, int2, zI2ToF2, zBroadcast2, zBroadcastF2, zSelect2, zSelectF2, zFrexp2, x, 1);
}

//! Compute base 2 logarithm of the given number
float3 zLog2F3(const float3 x)
{
  ZINVUL_LOG_IMPL(float3, int3, zI3ToF3, zBroadcast3, zBroadcastF3, zSelect3, zSelectF3, zFrexp3, x, 1);
}

//! Compute base 2 logarithm of the given number
float4 zLog2F4(const float4 x)
{
  ZINVUL_LOG_IMPL(float4, int4, zI4ToF4, zBroadcast4, zBroadcastF4, zSelect4, zSelectF4, zFrexp4, x, 1);
}

// Power functions

//! Raise a number to the given power
float zPow(const float base, const float expt)
{
  //! \todo Implement custom pow function
  return pow(base, expt);
}

//! Raise a number to the given power
float2 zPow2(const float2 base, const float2 expt)
{
  return pow(base, expt);
}

//! Raise a number to the given power
float3 zPow3(const float3 base, const float3 expt)
{
  return pow(base, expt);
}

//! Raise a number to the given power
float4 zPow4(const float4 base, const float4 expt)
{
  return pow(base, expt);
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
  y = selectF(y, 1.0f / base, ((base == 0.0f) || (isinf(base))) && (expt < 0)); \
  return y

//! Raise a number to the given integer power
float zPown(const float base, const int32b expt)
{
  ZINVUL_POWN_IMPL(float, int32b, uint32b, zSelectF,, zAbs,, isOddU, base, expt);
}

//! Raise a number to the given integer power
float2 zPown2(const float2 base, const int2 expt)
{
  ZINVUL_POWN_IMPL(float2, int2, uint2, zSelectF2, zBroadcastF2, zAbs2, zHasTrue2, isOddU2, base, expt);
}

//! Raise a number to the given integer power
float3 zPown3(const float3 base, const int3 expt)
{
  ZINVUL_POWN_IMPL(float3, int3, uint3, zSelectF3, zBroadcastF3, zAbs3, zHasTrue3, isOddU3, base, expt);
}

//! Raise a number to the given integer power
float4 zPown4(const float4 base, const int4 expt)
{
  ZINVUL_POWN_IMPL(float4, int4, uint4, zSelectF4, zBroadcastF4, zAbs4, zHasTrue4, isOddU4, base, expt);
}

//! Compute approximate inverse square root
#define ZINVUL_APPROXIMATE_INV_SQRT_IMPL(FType, IType, x) \
  union Float \
  { \
    FType value_; \
    IType data_; \
  }; \
  union Float y = {x}; \
  y.data_ = 0x5f376908 - (y.data_ >> 1); \
  return y.value_

//! Compute approximate inverse square root
float zApproxRsqrt(const float x)
{
  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float, int32b, x);
}

//! Compute approximate inverse square root
float2 zApproxRsqrt2(const float2 x)
{
  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float2, int2, x);
}

//! Compute approximate inverse square root
float3 zApproxRsqrt3(const float3 x)
{
  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float3, int3, x);
}

//! Compute approximate inverse square root
float4 zApproxRsqrt4(const float4 x)
{
  ZINVUL_APPROXIMATE_INV_SQRT_IMPL(float4, int4, x);
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
  y = selectF(y, y * 6.553600000e+04f, x < 2.328306437e-10f); \
  return y

//! Compute inverse square root
float zRsqrt(const float x)
{
  ZINVUL_INV_SQRT_IMPL(float, zSelectF, zApproxRsqrt, x);
}

//! Compute inverse square root
float2 zRsqrt2(const float2 x)
{
  ZINVUL_INV_SQRT_IMPL(float2, zSelectF2, zApproxRsqrt2, x);
}

//! Compute inverse square root
float3 zRsqrt3(const float3 x)
{
  ZINVUL_INV_SQRT_IMPL(float3, zSelectF3, zApproxRsqrt3, x);
}

//! Compute inverse square root
float4 zRsqrt4(const float4 x)
{
  ZINVUL_INV_SQRT_IMPL(float4, zSelectF4, zApproxRsqrt4, x);
}

//! Compute square root
#define ZINVUL_SQRT_IMPL(FType, IType, selectF, invSqrt, x) \
  FType y = x * invSqrt(x); \
  { \
    const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
    y = selectF(y, x, is_special); \
  } \
  return y

//! Compute square root
float zSqrt(const float x)
{
  ZINVUL_SQRT_IMPL(float, int32b, zSelectF, zRsqrt, x);
}

//! Compute square root
float2 zSqrt2(const float2 x)
{
  ZINVUL_SQRT_IMPL(float2, int2, zSelectF2, zRsqrt2, x);
}

//! Compute square root
float3 zSqrt3(const float3 x)
{
  ZINVUL_SQRT_IMPL(float3, int3, zSelectF3, zRsqrt3, x);
}

//! Compute square root
float4 zSqrt4(const float4 x)
{
  ZINVUL_SQRT_IMPL(float4, int4, zSelectF4, zRsqrt4, x);
}

//! Compute approximate cubic root
#define ZINVUL_APPROXIMATE_CBRT_IMPL(FType, IType, selectF, absoluteF, x) \
  union Float \
  { \
    FType value_; \
    IType data_; \
  }; \
  union Float y = {absoluteF(x)}; \
  y.data_ = (y.data_ >> 2) + (y.data_ >> 4); \
  y.data_ = y.data_ + (y.data_ >> 4); \
  y.data_ = y.data_ + (y.data_ >> 8); \
  y.data_ = 0x2a5137a0 + y.data_; \
  return selectF(y.value_, -y.value_, x < 0.0f);

//! Compute approximate cubic root
float zApproxCbrt(const float x)
{
  ZINVUL_APPROXIMATE_CBRT_IMPL(float, int32b, zSelectF, zAbsF, x);
}

//! Compute approximate cubic root
float2 zApproxCbrt2(const float2 x)
{
  ZINVUL_APPROXIMATE_CBRT_IMPL(float2, int2, zSelectF2, zAbsF2, x);
}

//! Compute approximate cubic root
float3 zApproxCbrt3(const float3 x)
{
  ZINVUL_APPROXIMATE_CBRT_IMPL(float3, int3, zSelectF3, zAbsF3, x);
}

//! Compute approximate cubic root
float4 zApproxCbrt4(const float4 x)
{
  ZINVUL_APPROXIMATE_CBRT_IMPL(float4, int4, zSelectF4, zAbsF4, x);
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
  y = selectF(y, x, is_special); \
  return y

//! Compute cubic root
float zCbrt(const float x)
{
  ZINVUL_CBRT_IMPL(float, int32b,, zSelectF, zApproxCbrt, x);
}

//! Compute cubic root
float2 zCbrt2(const float2 x)
{
  ZINVUL_CBRT_IMPL(float2, int2, zBroadcastF2, zSelectF2, zApproxCbrt2, x);
}

//! Compute cubic root
float3 zCbrt3(const float3 x)
{
  ZINVUL_CBRT_IMPL(float3, int3, zBroadcastF3, zSelectF3, zApproxCbrt3, x);
}

//! Compute cubic root
float4 zCbrt4(const float4 x)
{
  ZINVUL_CBRT_IMPL(float4, int4, zBroadcastF4, zSelectF4, zApproxCbrt4, x);
}

// Trigonometric functions

//! Compute sine
#define ZINVUL_SINCOS_IMPL(FType, IType, iToF, fToI, broadcastF, selectF, absoluteF, squareRoot, theta, trigonometric_type) \
  FType a = absoluteF(theta); \
  const IType q = fToI((4.0f / zPiF) * a); \
  { \
    const FType r = iToF(q + (q & 1)); \
    a = fma(r, broadcastF(-0.25f * 3.140625f), a); \
    a = fma(r, broadcastF(-0.25f * 0.0009670257568359375f), a); \
    a = fma(r, broadcastF(-0.25f * 6.2771141529083251953e-07f), a); \
    a = fma(r, broadcastF(-0.25f * 1.2154201256553420762e-10f), a); \
  } \
  FType c = broadcastF(0.0f); \
  { \
    a = 0.125f * a; \
    a = a * a; \
    FType t = broadcastF(-2.0f); \
    for (size_t i = 1; i < 4; ++i) { \
      t = -t * a / (float)((2 * i) * (2 * i - 1)); \
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
    s = selectF(s, -s, ((q & 4) != 0) != (theta < 0.0f)); \
    return s; \
  } \
  else if (trigonometric_type == 1) {\
    c = selectF(c, s, condition); \
    c = selectF(c, -c, ((q + 2) & 4) != 0); \
    return c; \
  } \
  else { \
    a = s; \
    s = selectF(s, c, condition); \
    c = selectF(c, a, condition); \
    s = selectF(s, -s, ((q & 4) != 0) != (theta < 0.0f)); \
    c = selectF(c, -c, ((q + 2) & 4) != 0); \
    return s / c; \
  }

//! Compute sine
float zSin(const float theta)
{
  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 0);
}

//! Compute sine
float2 zSin2(const float2 theta)
{
  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 0);
}

//! Compute sine
float3 zSin3(const float3 theta)
{
  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 0);
}

//! Compute sine
float4 zSin4(const float4 theta)
{
  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 0);
}

//! Compute cosine
float zCos(const float theta)
{
  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 1);
}

//! Compute cosine
float2 zCos2(const float2 theta)
{
  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 1);
}

//! Compute cosine
float3 zCos3(const float3 theta)
{
  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 1);
}

//! Compute cosine
float4 zCos4(const float4 theta)
{
  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 1);
}

//! Compute tangent
float zTan(const float theta)
{
  ZINVUL_SINCOS_IMPL(float, int32b, (float), (int32b),, zSelectF, zAbsF, zSqrt, theta, 2);
}

//! Compute tangent
float2 zTan2(const float2 theta)
{
  ZINVUL_SINCOS_IMPL(float2, int2, zI2ToF2 , zF2ToI2, zBroadcastF2, zSelectF2, zAbsF2, zSqrt2, theta, 2);
}

//! Compute tangent
float3 zTan3(const float3 theta)
{
  ZINVUL_SINCOS_IMPL(float3, int3, zI3ToF3 , zF3ToI3, zBroadcastF3, zSelectF3, zAbsF3, zSqrt3, theta, 2);
}

//! Compute tangent
float4 zTan4(const float4 theta)
{
  ZINVUL_SINCOS_IMPL(float4, int4, zI4ToF4 , zF4ToI4, zBroadcastF4, zSelectF4, zAbsF4, zSqrt4, theta, 2);
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
  y = selectF(y, -y, x < 0.0f); \
  return y

//! Compute arc tangent
float zAtan(const float x)
{
  ZINVUL_ATAN_IMPL(float, int32b, (float), zSelect, zSelectF,,, zAbsF, zSqrt, x);
}

//! Compute arc tangent
float2 zAtan2(const float2 x)
{
  ZINVUL_ATAN_IMPL(float2, int2, zI2ToF2, zSelect2, zSelectF2, zBroadcast2, zBroadcastF2, zAbsF2, zSqrt2, x);
}

//! Compute arc tangent
float3 zAtan3(const float3 x)
{
  ZINVUL_ATAN_IMPL(float3, int3, zI3ToF3, zSelect3, zSelectF3, zBroadcast3, zBroadcastF3, zAbsF3, zSqrt3, x);
}

//! Compute arc tangent
float4 zAtan4(const float4 x)
{
  ZINVUL_ATAN_IMPL(float4, int4, zI4ToF4, zSelect4, zSelectF4, zBroadcast4, zBroadcastF4, zAbsF4, zSqrt4, x);
}

//! Compute arc sine
#define ZINVUL_ASIN_IMPL(FType, squareRoot, arctan, x) \
  const FType z = x / squareRoot((1.0f + x) * (1.0f - x)); \
  const FType y = arctan(z); \
  return y

//! Compute arc sine
float zAsin(const float x)
{
  ZINVUL_ASIN_IMPL(float, zSqrt, zAtan, x);
}

//! Compute arc sine
float2 zAsin2(const float2 x)
{
  ZINVUL_ASIN_IMPL(float2, zSqrt2, zAtan2, x);
}

//! Compute arc sine
float3 zAsin3(const float3 x)
{
  ZINVUL_ASIN_IMPL(float3, zSqrt3, zAtan3, x);
}

//! Compute arc sine
float4 zAsin4(const float4 x)
{
  ZINVUL_ASIN_IMPL(float4, zSqrt4, zAtan4, x);
}

//! Compute arc cosine
#define ZINVUL_ACOS_IMPL(FType, selectF, broadcastF, squareRoot, arctan, x) \
  const FType z = squareRoot((1.0f + x) * (1.0f - x)) / x; \
  const FType y = arctan(z) + selectF(broadcastF(0.0f), broadcastF(zPiF), x < 0.0f); \
  return y

//! Compute arc cosine
float zAcos(const float x)
{
  ZINVUL_ACOS_IMPL(float, zSelectF,, zSqrt, zAtan, x);
}

//! Compute arc cosine
float2 zAcos2(const float2 x)
{
  ZINVUL_ACOS_IMPL(float2, zSelectF2, zBroadcastF2, zSqrt2, zAtan2, x);
}

//! Compute arc cosine
float3 zAcos3(const float3 x)
{
  ZINVUL_ACOS_IMPL(float3, zSelectF3, zBroadcastF3, zSqrt3, zAtan3, x);
}

//! Compute arc cosine
float4 zAcos4(const float4 x)
{
  ZINVUL_ACOS_IMPL(float4, zSelectF4, zBroadcastF4, zSqrt4, zAtan4, x);
}

#endif /* ZINVUL_MATH_CL */
