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
#define zExponentBitSizeF 8u
#define zExponentBiasF 127
#define zSignificandBitMaskF 0x7fffffu
#define zSignificandBitSizeF 23u

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

// Power functions

//! Compute approximate inverse square root
#define ZINVUL_APPROXIMATE_INV_SQRT_IMPL(FType, IType, x) \
  union Float \
  { \
    FType value_; \
    IType data_; \
  }; \
  union Float z = {x}; \
  z.data_ = 0x5f376908 - (z.data_ >> 1); \
  return z.value_

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
#define ZINVUL_INV_SQRT_IMPL(FType, x, selectF, approxRsqrt, assertE) \
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
  ZINVUL_INV_SQRT_IMPL(float, x, zSelectF, zApproxRsqrt, zAssert);
}

//! Compute inverse square root
float2 zRsqrt2(const float2 x)
{
  ZINVUL_INV_SQRT_IMPL(float2, x, zSelectF2, zApproxRsqrt2, zAssert2);
}

//! Compute inverse square root
float3 zRsqrt3(const float3 x)
{
  ZINVUL_INV_SQRT_IMPL(float3, x, zSelectF3, zApproxRsqrt3, zAssert3);
}

//! Compute inverse square root
float4 zRsqrt4(const float4 x)
{
  ZINVUL_INV_SQRT_IMPL(float4, x, zSelectF4, zApproxRsqrt4, zAssert4);
}

//! Compute square root
#define ZINVUL_SQRT_IMPL(FType, IType, x, invSqrt, selectF) \
  FType y = x * invSqrt(x); \
  { \
    const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
    y = selectF(y, x, is_special); \
  } \
  return y

//! Compute square root
float zSqrt(const float x)
{
  ZINVUL_SQRT_IMPL(float, int32b, x, zRsqrt, zSelectF);
}

//! Compute square root
float2 zSqrt2(const float2 x)
{
  ZINVUL_SQRT_IMPL(float2, int2, x, zRsqrt2, zSelectF2);
}

//! Compute square root
float3 zSqrt3(const float3 x)
{
  ZINVUL_SQRT_IMPL(float3, int3, x, zRsqrt3, zSelectF3);
}

//! Compute square root
float4 zSqrt4(const float4 x)
{
  ZINVUL_SQRT_IMPL(float4, int4, x, zRsqrt4, zSelectF4);
}

// Floating point manipulation functions

//! \todo Support subnormal value
#define ZINVUL_FREXP_IMPL(FType, UType, IType, zero, uToI, selectI, selectF, x, e) \
  union Float \
  { \
    FType value_; \
    UType data_; \
  }; \
  union Float y = {x}; \
  const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
  { \
    IType expt = uToI((y.data_ & zExponentBitMaskF) >> zSignificandBitSizeF) - \
                 (zExponentBiasF - 1); \
    expt = selectI(expt, zero, is_special); \
    *e = expt; \
  } \
  y.data_ = (y.data_ & ~zExponentBitMaskF) | 0x3f000000u; \
  y.value_ = selectF(y.value_, x, is_special); \
  return y.value_

//! Decompose a number into significand and a power of 2
float zFrexp(const float x, __private int32b* e)
{
  ZINVUL_FREXP_IMPL(float, uint32b, int32b, 0, (int32b), zSelect, zSelectF, x, e);
}

//! Decompose a number into significand and a power of 2
float2 zFrexp2(const float2 x, __private int2* e)
{
  const int2 zero = zBroadcast2(0);
  ZINVUL_FREXP_IMPL(float2, uint2, int2, zero, zU2ToI2, zSelect2, zSelectF2, x, e);
}

//! Decompose a number into significand and a power of 2
float3 zFrexp3(const float3 x, __private int3* e)
{
  const int3 zero = zBroadcast3(0);
  ZINVUL_FREXP_IMPL(float3, uint3, int3, zero, zU3ToI3, zSelect3, zSelectF3, x, e);
}

//! Decompose a number into significand and a power of 2
float4 zFrexp4(const float4 x, __private int4* e)
{
  const int4 zero = zBroadcast4(0);
  ZINVUL_FREXP_IMPL(float4, uint4, int4, zero, zU4ToI4, zSelect4, zSelectF4, x, e);
}

//! \todo Support subnormal value
#define ZINVUL_LDEXP_IMPL(FType, UType, IType, iToU, selectF, x, e) \
  union Float \
  { \
    FType value_; \
    UType data_; \
  }; \
  union Float y = {x}; \
  { \
    const UType expt_bits = iToU(e + (zExponentBiasF - 1)) << zSignificandBitSizeF; \
    y.data_ = (y.data_ & ~zExponentBitMaskF) | expt_bits; \
  } \
  { \
    const IType is_special = (x == 0.0f) || isinf(x) || isnan(x); \
    y.value_ = selectF(y.value_, x, is_special); \
  } \
  return y.value_

//! Multiply a number by 2 raised to a power
float zLdexp(const float x, const int32b e)
{
  ZINVUL_LDEXP_IMPL(float, uint32b, int32b, (uint32b), zSelectF, x, e);
}

//! Multiply a number by 2 raised to a power
float2 zLdexp2(const float2 x, const int2 e)
{
  ZINVUL_LDEXP_IMPL(float2, uint2, int2, zI2ToU2, zSelectF2, x, e);
}

//! Multiply a number by 2 raised to a power
float3 zLdexp3(const float3 x, const int3 e)
{
  ZINVUL_LDEXP_IMPL(float3, uint3, int3, zI3ToU3, zSelectF3, x, e);
}

//! Multiply a number by 2 raised to a power
float4 zLdexp4(const float4 x, const int4 e)
{
  ZINVUL_LDEXP_IMPL(float4, uint4, int4, zI4ToU4, zSelectF4, x, e);
}

#endif /* ZINVUL_MATH_CL */
