/*!
  \file math.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_TEST_MATH_CL
#define ZINVUL_MATH_TEST_MATH_CL

// Zinvul
#include "zinvul/cl/algorithm.cl"
#include "zinvul/cl/floating_point.cl"
#include "zinvul/cl/fnv_1a_hash_engine.cl"
#include "zinvul/cl/limits.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/math.cl"
#include "zinvul/cl/utility.cl"

using zinvul::int8b;
using zinvul::int16b;
using zinvul::int32b;
using zinvul::int64b;
using zinvul::uint8b;
using zinvul::uint16b;
using zinvul::uint32b;
using zinvul::uint64b;
using zinvul::GlobalPtr;
using zinvul::ConstGlobalPtr;
using zinvul::ConstantPtr;
using zinvul::ConstConstantPtr;
using zinvul::Local;
using zinvul::LocalPtr;
using zinvul::ConstLocalPtr;
using zinvul::cast;
using zinvul::treatAs;

// Forward declaration
__kernel void testConstantValue(GlobalPtr<int32b> iresults,
    GlobalPtr<float> fresults);
__kernel void testMakeNormal(GlobalPtr<float> pinputs,
    GlobalPtr<float> inputs,
    GlobalPtr<float> yinputs,
    const uint32b resolution);
__kernel void testCeil(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testFloor(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testTrunc(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRound(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRoundBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRoundZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRint(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRintBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRintZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testFmod(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testFmodBuiltin(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testFmodZinvul(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testExp(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testExpBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testExpZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testExp2(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testExp2Builtin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testExp2Zinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testLog(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testLogBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testLogZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testLog2(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testLog2Builtin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testLog2Zinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testPow(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testPowBuiltin(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testPowZinvul(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRsqrt(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRsqrtBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testRsqrtZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSqrt(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSqrtBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSqrtZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCbrt(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCbrtBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCbrtZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSinBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSinZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCos(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCosBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCosZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testTan(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testTanBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testTanZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSinPi(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSinPiBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testSinPiZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCosPi(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCosPiBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testCosPiZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testTanPi(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testTanPiBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testTanPiZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAsin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAsinBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAsinZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAcos(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAcosBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAcosZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAtan(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAtanBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testAtanZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testFrLdexp(GlobalPtr<float> inputs,
    GlobalPtr<float> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<float> lresults,
    const uint32b resolution);
__kernel void testFrLdexpBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<float> lresults,
    const uint32b resolution);
__kernel void testFrLdexpZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<float> lresults,
    const uint32b resolution);
__kernel void testModf(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    GlobalPtr<float> fresults,
    const uint32b resolution);
__kernel void testModfBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    GlobalPtr<float> fresults,
    const uint32b resolution);
__kernel void testModfZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    GlobalPtr<float> fresults,
    const uint32b resolution);
__kernel void testCopysign(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution);
#if !defined(Z_MAC)
__kernel void testConstantValueF64(GlobalPtr<int32b> iresults,
    GlobalPtr<double> fresults);
__kernel void testMakeNormalF64(GlobalPtr<double> pinputs,
    GlobalPtr<double> inputs,
    GlobalPtr<double> yinputs,
    const uint32b resolution);
__kernel void testCeilF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testFloorF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testTruncF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRoundF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRoundBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRoundZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRintF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRintBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRintZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testFmodF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testFmodBuiltinF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testFmodZinvulF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testExpF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testExpBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testExpZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testExp2F64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testExp2BuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testExp2ZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testLogF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testLogBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testLogZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testLog2F64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testLog2BuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testLog2ZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testPowF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testPowBuiltinF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testPowZinvulF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRsqrtF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRsqrtBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testRsqrtZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSqrtF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSqrtBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSqrtZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCbrtF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCbrtBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCbrtZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSinBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSinZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCosF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCosBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCosZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testTanF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testTanBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testTanZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSinPiF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSinPiBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testSinPiZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCosPiF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCosPiBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testCosPiZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testTanPiF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testTanPiBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testTanPiZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAsinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAsinBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAsinZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAcosF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAcosBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAcosZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAtanF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAtanBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testAtanZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution);
__kernel void testFrLdexpF64(GlobalPtr<double> inputs,
    GlobalPtr<double> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<double> lresults,
    const uint32b resolution);
__kernel void testFrLdexpBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<double> lresults,
    const uint32b resolution);
__kernel void testFrLdexpZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<double> lresults,
    const uint32b resolution);
__kernel void testModfF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    GlobalPtr<double> fresults,
    const uint32b resolution);
__kernel void testModfBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    GlobalPtr<double> fresults,
    const uint32b resolution);
__kernel void testModfZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    GlobalPtr<double> fresults,
    const uint32b resolution);
__kernel void testCopysignF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution);

#endif // !Z_MAC

namespace test {

template <typename Float>
Float makeNormal01(const uint32b index, const uint32b resolution) noexcept;

template <typename Float>
Float makeNormal01P(const uint32b index, const uint32b resolution) noexcept;

template <typename Float>
Float makeNormal(const uint32b index,
                 const uint32b resolution,
                 const Float scale = static_cast<Float>(1.0)) noexcept;

template <typename Float>
Float makeNormalP(const uint32b index,
                  const uint32b resolution,
                  const Float scale = static_cast<Float>(1.0)) noexcept;

template <typename Float>
Float makeNormalF(const Float x) noexcept;

template <typename Float> inline
Float makeNormalY(const uint32b p,
                  const Float scale = static_cast<Float>(1.0)) noexcept;

template <typename Float> inline
Float makeNormal01(const uint32b index, const uint32b resolution) noexcept
{
  const auto k = zinvul::Algorithm::invert(static_cast<Float>(resolution));
  const auto i = static_cast<Float>(2 * index) - static_cast<Float>(resolution);
  const auto x = k * static_cast<Float>(i);
  return x;
}

template <typename Float> inline
Float makeNormal01P(const uint32b index, const uint32b resolution) noexcept
{
  const auto k = zinvul::Algorithm::invert(static_cast<Float>(resolution));
  const auto x = k * static_cast<Float>(index);
  return x;
}

template <typename Float> inline
Float makeNormal(const uint32b index,
                 const uint32b resolution,
                 const Float scale) noexcept
{
  auto x = makeNormal01<Float>(index, resolution);
  constexpr auto minval = static_cast<Float>(-1.0);
  constexpr auto maxval = static_cast<Float>(1.0);
  x = zinvul::clamp(scale * x, minval, maxval);
  const auto n = (index == 0)
    ? zinvul::NumericLimits<Float>::epsilon()
    : makeNormalF(x);
  return n;
}

template <typename Float> inline
Float makeNormalP(const uint32b index,
                  const uint32b resolution,
                  const Float scale) noexcept
{
  auto x = makeNormal01P<Float>(index, resolution);
  constexpr auto minval = static_cast<Float>(-1.0);
  constexpr auto maxval = static_cast<Float>(1.0);
  x = zinvul::clamp(scale * x, minval, maxval);
  const auto n = (index == 0)
    ? zinvul::NumericLimits<Float>::epsilon()
    : makeNormalF(x);
  return n;
}

template <typename Float> inline
Float makeNormalF(const Float x) noexcept
{
  using FloatInfo = zinvul::FloatingPointFromBytes<sizeof(Float)>;
  using BitType = typename FloatInfo::BitType;
  constexpr auto bias = static_cast<Float>(FloatInfo::exponentBias());
  constexpr Float zero = static_cast<Float>(0.0);
  constexpr Float one = static_cast<Float>(1.0);
  constexpr Float two = static_cast<Float>(2.0);
  const Float k = two * zinvul::abs(x) - static_cast<Float>(1.0);
  // Sign
  const Float signb = (x < zero) ? -one : one;
  // Exponent
  auto exponent = static_cast<Float>(k * static_cast<Float>(bias));
  exponent = zinvul::clamp(exponent, -bias + 1, bias);
  // Significand
  BitType h = zinvul::NumericLimits<BitType>::max();
  h = static_cast<BitType>(zinvul::abs(x) * static_cast<Float>(h));
  h = zinvul::Fnv1aHashEngine<BitType>::hash(h);
  const Float s = FloatInfo::mapTo01(h);

  Float normal = s * zinvul::pow(two, exponent);
  normal = signb * zinvul::clamp(normal,
                                 zinvul::NumericLimits<Float>::min(),
                                 zinvul::NumericLimits<Float>::max());
  return normal;
}

template <typename Float> inline
Float makeNormalY(const uint32b p, const Float scale) noexcept
{
  constexpr Float zero = static_cast<Float>(0.0);
  constexpr Float one = static_cast<Float>(1.0);
  constexpr Float two = static_cast<Float>(2.0);
  constexpr auto m = zinvul::NumericLimits<uint32b>::max();
  const auto a = zinvul::Fnv1aHash32::hash(p);
  auto x = static_cast<Float>(a) / static_cast<Float>(m);
  x = two * x - one;

  const Float s = (x < zero) ? -scale : scale;
  x = s * zinvul::clamp(zinvul::abs(x), zinvul::NumericLimits<Float>::min(), one);
  return x;
}

} // namespace test

__kernel void testConstantValue(GlobalPtr<int32b> iresults,
    GlobalPtr<float> fresults)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr int32b k = FLT_DIG;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MANT_DIG;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MAX_10_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MAX_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MIN_10_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_MIN_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = FLT_RADIX;
      iresults[i++] = k;
    }

    i = 0;
    {
      constexpr float k = FLT_MAX;
      fresults[i++] = k;
    }
    {
      constexpr float k = FLT_MIN;
      fresults[i++] = k;
    }
    {
      constexpr float k = FLT_EPSILON;
      fresults[i++] = k;
    }

    // Math constant values
    {
      constexpr float k = M_E_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_LOG2E_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_LOG10E_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_LN2_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_LN10_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_PI_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_PI_2_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_PI_4_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_1_PI_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_2_PI_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_2_SQRTPI_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_SQRT2_F;
      fresults[i++] = k;
    }
    {
      constexpr float k = M_SQRT1_2_F;
      fresults[i++] = k;
    }

    {
      constexpr float pi = zinvul::kPi<float>;
      fresults[i++] = pi;
    }
    {
      constexpr float pi = zinvul::kPi<float>;
      constexpr float s = zinvul::mathconst::sqrt(pi);
      fresults[i++] = s;
    }
    {
      constexpr float s = zinvul::mathconst::sqrt(2.0f);
      fresults[i++] = s;
    }
    {
      constexpr float s = zinvul::mathconst::sqrt(3.0f);
      fresults[i++] = s;
    }
    {
      constexpr float pi = zinvul::kPi<float>;
      constexpr float c = zinvul::mathconst::cbrt(pi);
      fresults[i++] = c;
    }
    {
      constexpr float c = zinvul::mathconst::cbrt(2.0f);
      fresults[i++] = c;
    }
    {
      constexpr float c = zinvul::mathconst::cbrt(3.0f);
      fresults[i++] = c;
    }
  }
}

__kernel void testMakeNormal(GlobalPtr<float> pinputs,
    GlobalPtr<float> inputs,
    GlobalPtr<float> yinputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    {
      const auto x = test::makeNormalP<float>(index, resolution);
      pinputs[index] = x;
    }
    {
      const auto x = test::makeNormal<float>(index, resolution);
      inputs[index] = x;
    }
    {
      const auto x = test::makeNormalY<float>(index, 4.0f);
      yinputs[index] = x;
    }
  }
}

__kernel void testCeil(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::ceil(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::ceil(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::ceil(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::ceil(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testFloor(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::floor(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::floor(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::floor(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::floor(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testTrunc(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::trunc(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::trunc(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::trunc(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::trunc(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testRound(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::round(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::round(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::round(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::round(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testRoundBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::round(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::round(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::round(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::round(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testRoundZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::round(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::round(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::round(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::round(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testRint(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::rint(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::rint(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::rint(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::rint(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testRintBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::rint(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::rint(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::rint(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::rint(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testRintZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::rint(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::rint(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::rint(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::rint(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testFmod(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.9f;
  constexpr float yscale = 10.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      const float y = test::makeNormalY<float>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::fmod(x, y);
      results[gindex] = z;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      const float2 y{test::makeNormalY<float>(gindex + 1, yscale),
                     test::makeNormalY<float>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::fmod(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      const float3 y{test::makeNormalY<float>(gindex + 3, yscale),
                     test::makeNormalY<float>(gindex + 4, yscale),
                     test::makeNormalY<float>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::fmod(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      const float4 y{test::makeNormalY<float>(gindex + 6, yscale),
                     test::makeNormalY<float>(gindex + 7, yscale),
                     test::makeNormalY<float>(gindex + 8, yscale),
                     test::makeNormalY<float>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::fmod(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
  }
}

__kernel void testFmodBuiltin(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.9f;
  constexpr float yscale = 10.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      const float y = test::makeNormalY<float>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::Math::Builtin::fmod(x, y);
      results[gindex] = z;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      const float2 y{test::makeNormalY<float>(gindex + 1, yscale),
                     test::makeNormalY<float>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::Math::Builtin::fmod(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      const float3 y{test::makeNormalY<float>(gindex + 3, yscale),
                     test::makeNormalY<float>(gindex + 4, yscale),
                     test::makeNormalY<float>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::Math::Builtin::fmod(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      const float4 y{test::makeNormalY<float>(gindex + 6, yscale),
                     test::makeNormalY<float>(gindex + 7, yscale),
                     test::makeNormalY<float>(gindex + 8, yscale),
                     test::makeNormalY<float>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::Math::Builtin::fmod(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
  }
}

__kernel void testFmodZinvul(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.9f;
  constexpr float yscale = 10.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      const float y = test::makeNormalY<float>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::Math::Zinvul::fmod(x, y);
      results[gindex] = z;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      const float2 y{test::makeNormalY<float>(gindex + 1, yscale),
                     test::makeNormalY<float>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::Math::Zinvul::fmod(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      const float3 y{test::makeNormalY<float>(gindex + 3, yscale),
                     test::makeNormalY<float>(gindex + 4, yscale),
                     test::makeNormalY<float>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::Math::Zinvul::fmod(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      const float4 y{test::makeNormalY<float>(gindex + 6, yscale),
                     test::makeNormalY<float>(gindex + 7, yscale),
                     test::makeNormalY<float>(gindex + 8, yscale),
                     test::makeNormalY<float>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::Math::Zinvul::fmod(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
  }
}

__kernel void testExp(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 16.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalY<float>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::exp(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalY<float>(gindex + 1, scale),
                     test::makeNormalY<float>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::exp(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalY<float>(gindex + 3, scale),
                     test::makeNormalY<float>(gindex + 4, scale),
                     test::makeNormalY<float>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::exp(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalY<float>(gindex + 6, scale),
               test::makeNormalY<float>(gindex + 7, scale),
               test::makeNormalY<float>(gindex + 8, scale),
               test::makeNormalY<float>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::exp(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testExpBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 16.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalY<float>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::exp(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalY<float>(gindex + 1, scale),
                     test::makeNormalY<float>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::exp(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalY<float>(gindex + 3, scale),
                     test::makeNormalY<float>(gindex + 4, scale),
                     test::makeNormalY<float>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::exp(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalY<float>(gindex + 6, scale),
               test::makeNormalY<float>(gindex + 7, scale),
               test::makeNormalY<float>(gindex + 8, scale),
               test::makeNormalY<float>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::exp(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testExpZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 16.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalY<float>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::exp(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalY<float>(gindex + 1, scale),
                     test::makeNormalY<float>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::exp(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalY<float>(gindex + 3, scale),
                     test::makeNormalY<float>(gindex + 4, scale),
                     test::makeNormalY<float>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::exp(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalY<float>(gindex + 6, scale),
               test::makeNormalY<float>(gindex + 7, scale),
               test::makeNormalY<float>(gindex + 8, scale),
               test::makeNormalY<float>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::exp(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testExp2(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 16.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalY<float>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::exp2(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalY<float>(gindex + 1, scale),
                     test::makeNormalY<float>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::exp2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalY<float>(gindex + 3, scale),
                     test::makeNormalY<float>(gindex + 4, scale),
                     test::makeNormalY<float>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::exp2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalY<float>(gindex + 6, scale),
               test::makeNormalY<float>(gindex + 7, scale),
               test::makeNormalY<float>(gindex + 8, scale),
               test::makeNormalY<float>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::exp2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testExp2Builtin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 16.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalY<float>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::exp2(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalY<float>(gindex + 1, scale),
                     test::makeNormalY<float>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::exp2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalY<float>(gindex + 3, scale),
                     test::makeNormalY<float>(gindex + 4, scale),
                     test::makeNormalY<float>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::exp2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalY<float>(gindex + 6, scale),
               test::makeNormalY<float>(gindex + 7, scale),
               test::makeNormalY<float>(gindex + 8, scale),
               test::makeNormalY<float>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::exp2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testExp2Zinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 16.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalY<float>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::exp2(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalY<float>(gindex + 1, scale),
                     test::makeNormalY<float>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::exp2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalY<float>(gindex + 3, scale),
                     test::makeNormalY<float>(gindex + 4, scale),
                     test::makeNormalY<float>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::exp2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalY<float>(gindex + 6, scale),
               test::makeNormalY<float>(gindex + 7, scale),
               test::makeNormalY<float>(gindex + 8, scale),
               test::makeNormalY<float>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::exp2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testLog(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::log(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::log(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::log(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::log(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testLogBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::log(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::log(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::log(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::log(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testLogZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::log(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::log(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::log(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::log(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testLog2(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::log2(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::log2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::log2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::log2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testLog2Builtin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::log2(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::log2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::log2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::log2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testLog2Zinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::log2(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::log2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::log2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::log2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testPow(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.6f;
  constexpr float yscale = 4.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      const float y = test::makeNormalY<float>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::pow(x, y);
      results[gindex] = z;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      const float2 y{test::makeNormalY<float>(gindex + 1, yscale),
                     test::makeNormalY<float>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::pow(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      const float3 y{test::makeNormalY<float>(gindex + 3, yscale),
                     test::makeNormalY<float>(gindex + 4, yscale),
                     test::makeNormalY<float>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::pow(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      const float4 y{test::makeNormalY<float>(gindex + 6, yscale),
                     test::makeNormalY<float>(gindex + 7, yscale),
                     test::makeNormalY<float>(gindex + 8, yscale),
                     test::makeNormalY<float>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::pow(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
  }
}

__kernel void testPowBuiltin(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.6f;
  constexpr float yscale = 4.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      const float y = test::makeNormalY<float>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::Math::Builtin::pow(x, y);
      results[gindex] = z;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      const float2 y{test::makeNormalY<float>(gindex + 1, yscale),
                     test::makeNormalY<float>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::Math::Builtin::pow(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      const float3 y{test::makeNormalY<float>(gindex + 3, yscale),
                     test::makeNormalY<float>(gindex + 4, yscale),
                     test::makeNormalY<float>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::Math::Builtin::pow(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      const float4 y{test::makeNormalY<float>(gindex + 6, yscale),
                     test::makeNormalY<float>(gindex + 7, yscale),
                     test::makeNormalY<float>(gindex + 8, yscale),
                     test::makeNormalY<float>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::Math::Builtin::pow(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
  }
}

__kernel void testPowZinvul(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.6f;
  constexpr float yscale = 4.0f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      const float y = test::makeNormalY<float>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::Math::Zinvul::pow(x, y);
      results[gindex] = z;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      const float2 y{test::makeNormalY<float>(gindex + 1, yscale),
                     test::makeNormalY<float>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::Math::Zinvul::pow(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      const float3 y{test::makeNormalY<float>(gindex + 3, yscale),
                     test::makeNormalY<float>(gindex + 4, yscale),
                     test::makeNormalY<float>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::Math::Zinvul::pow(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      const float4 y{test::makeNormalY<float>(gindex + 6, yscale),
                     test::makeNormalY<float>(gindex + 7, yscale),
                     test::makeNormalY<float>(gindex + 8, yscale),
                     test::makeNormalY<float>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::Math::Zinvul::pow(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
  }
}

__kernel void testRsqrt(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::rsqrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::rsqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::rsqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::rsqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testRsqrtBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::rsqrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::rsqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::rsqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::rsqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testRsqrtZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::rsqrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::rsqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::rsqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::rsqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSqrt(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::sqrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::sqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::sqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::sqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSqrtBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::sqrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::sqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::sqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::sqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSqrtZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormalP<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::sqrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormalP<float>(gindex + 1, gresolution),
                     test::makeNormalP<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::sqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormalP<float>(gindex + 3, gresolution),
                     test::makeNormalP<float>(gindex + 4, gresolution),
                     test::makeNormalP<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::sqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormalP<float>(gindex + 6, gresolution),
               test::makeNormalP<float>(gindex + 7, gresolution),
               test::makeNormalP<float>(gindex + 8, gresolution),
               test::makeNormalP<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::sqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCbrt(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::cbrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::cbrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::cbrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::cbrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCbrtBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::cbrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::cbrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::cbrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::cbrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCbrtZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::cbrt(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::cbrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::cbrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::cbrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::sin(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSinBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSinZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCos(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::cos(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCosBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCosZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testTan(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::tan(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testTanBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testTanZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr float scale = 0.56f;
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution, scale),
                     test::makeNormal<float>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution, scale),
                     test::makeNormal<float>(gindex + 4, gresolution, scale),
                     test::makeNormal<float>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution, scale),
               test::makeNormal<float>(gindex + 7, gresolution, scale),
               test::makeNormal<float>(gindex + 8, gresolution, scale),
               test::makeNormal<float>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSinPi(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::sin(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSinPiBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testSinPiZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCosPi(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::cos(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCosPiBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testCosPiZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testTanPi(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::tan(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testTanPiBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testTanPiZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr float k = 2.0f * zinvul::kPi<float>;

    {
      const float x = k * test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex] = y;
    }
    {
      const float2 x{k * test::makeNormal01<float>(gindex + 1, gresolution),
                     k * test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{k * test::makeNormal01<float>(gindex + 3, gresolution),
                     k * test::makeNormal01<float>(gindex + 4, gresolution),
                     k * test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{k * test::makeNormal01<float>(gindex + 6, gresolution),
               k * test::makeNormal01<float>(gindex + 7, gresolution),
               k * test::makeNormal01<float>(gindex + 8, gresolution),
               k * test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}


__kernel void testAsin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::asin(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal01<float>(gindex + 1, gresolution),
                     test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::asin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal01<float>(gindex + 3, gresolution),
                     test::makeNormal01<float>(gindex + 4, gresolution),
                     test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::asin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal01<float>(gindex + 6, gresolution),
               test::makeNormal01<float>(gindex + 7, gresolution),
               test::makeNormal01<float>(gindex + 8, gresolution),
               test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::asin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testAsinBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::asin(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal01<float>(gindex + 1, gresolution),
                     test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::asin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal01<float>(gindex + 3, gresolution),
                     test::makeNormal01<float>(gindex + 4, gresolution),
                     test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::asin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal01<float>(gindex + 6, gresolution),
               test::makeNormal01<float>(gindex + 7, gresolution),
               test::makeNormal01<float>(gindex + 8, gresolution),
               test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::asin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testAsinZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::asin(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal01<float>(gindex + 1, gresolution),
                     test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::asin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal01<float>(gindex + 3, gresolution),
                     test::makeNormal01<float>(gindex + 4, gresolution),
                     test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::asin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal01<float>(gindex + 6, gresolution),
               test::makeNormal01<float>(gindex + 7, gresolution),
               test::makeNormal01<float>(gindex + 8, gresolution),
               test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::asin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testAcos(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::acos(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal01<float>(gindex + 1, gresolution),
                     test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::acos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal01<float>(gindex + 3, gresolution),
                     test::makeNormal01<float>(gindex + 4, gresolution),
                     test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::acos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal01<float>(gindex + 6, gresolution),
               test::makeNormal01<float>(gindex + 7, gresolution),
               test::makeNormal01<float>(gindex + 8, gresolution),
               test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::acos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testAcosBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::acos(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal01<float>(gindex + 1, gresolution),
                     test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::acos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal01<float>(gindex + 3, gresolution),
                     test::makeNormal01<float>(gindex + 4, gresolution),
                     test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::acos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal01<float>(gindex + 6, gresolution),
               test::makeNormal01<float>(gindex + 7, gresolution),
               test::makeNormal01<float>(gindex + 8, gresolution),
               test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::acos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testAcosZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal01<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::acos(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal01<float>(gindex + 1, gresolution),
                     test::makeNormal01<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::acos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal01<float>(gindex + 3, gresolution),
                     test::makeNormal01<float>(gindex + 4, gresolution),
                     test::makeNormal01<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::acos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal01<float>(gindex + 6, gresolution),
               test::makeNormal01<float>(gindex + 7, gresolution),
               test::makeNormal01<float>(gindex + 8, gresolution),
               test::makeNormal01<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::acos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testAtan(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::atan(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::atan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::atan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::atan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testAtanBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::atan(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::atan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::atan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::atan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testAtanZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::atan(x);
      results[gindex] = y;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::atan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::atan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::atan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
  }
}

__kernel void testFrLdexp(GlobalPtr<float> inputs,
    GlobalPtr<float> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<float> lresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      float x = test::makeNormal<float>(gindex, gresolution);
      int e = 0;
      inputs[gindex] = x;
      const auto s = zinvul::frexp(x, &e);
      fresults[gindex] = s;
      eresults[gindex] = e;
      x = zinvul::ldexp(s, e);
      lresults[gindex] = x;
    }
    {
      float2 x{test::makeNormal<float>(gindex + 1, gresolution),
               test::makeNormal<float>(gindex + 2, gresolution)};
      int2 e{0, 0};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto s = zinvul::frexp(x, &e);
      fresults[gindex + 1] = s.x;
      fresults[gindex + 2] = s.y;
      eresults[gindex + 1] = e.x;
      eresults[gindex + 2] = e.y;
      x = zinvul::ldexp(s, e);
      lresults[gindex + 1] = x.x;
      lresults[gindex + 2] = x.y;
    }
    {
      float3 x{test::makeNormal<float>(gindex + 3, gresolution),
               test::makeNormal<float>(gindex + 4, gresolution),
               test::makeNormal<float>(gindex + 5, gresolution)};
      int3 e{0, 0, 0};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto s = zinvul::frexp(x, &e);
      fresults[gindex + 3] = s.x;
      fresults[gindex + 4] = s.y;
      fresults[gindex + 5] = s.z;
      eresults[gindex + 3] = e.x;
      eresults[gindex + 4] = e.y;
      eresults[gindex + 5] = e.z;
      x = zinvul::ldexp(s, e);
      lresults[gindex + 3] = x.x;
      lresults[gindex + 4] = x.y;
      lresults[gindex + 5] = x.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      int4 e{0, 0, 0, 0};
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto s = zinvul::frexp(x, &e);
      fresults[gindex + 6] = s.x;
      fresults[gindex + 7] = s.y;
      fresults[gindex + 8] = s.z;
      fresults[gindex + 9] = s.w;
      eresults[gindex + 6] = e.x;
      eresults[gindex + 7] = e.y;
      eresults[gindex + 8] = e.z;
      eresults[gindex + 9] = e.w;
      x = zinvul::ldexp(s, e);
      lresults[gindex + 6] = x.x;
      lresults[gindex + 7] = x.y;
      lresults[gindex + 8] = x.z;
      lresults[gindex + 9] = x.w;
    }
  }
}

__kernel void testFrLdexpBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<float> lresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      float x = test::makeNormal<float>(gindex, gresolution);
      int e = 0;
      inputs[gindex] = x;
      const auto s = zinvul::Math::Builtin::frexp(x, &e);
      fresults[gindex] = s;
      eresults[gindex] = e;
      x = zinvul::Math::Builtin::ldexp(s, e);
      lresults[gindex] = x;
    }
    {
      float2 x{test::makeNormal<float>(gindex + 1, gresolution),
               test::makeNormal<float>(gindex + 2, gresolution)};
      int2 e{0, 0};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto s = zinvul::Math::Builtin::frexp(x, &e);
      fresults[gindex + 1] = s.x;
      fresults[gindex + 2] = s.y;
      eresults[gindex + 1] = e.x;
      eresults[gindex + 2] = e.y;
      x = zinvul::Math::Builtin::ldexp(s, e);
      lresults[gindex + 1] = x.x;
      lresults[gindex + 2] = x.y;
    }
    {
      float3 x{test::makeNormal<float>(gindex + 3, gresolution),
               test::makeNormal<float>(gindex + 4, gresolution),
               test::makeNormal<float>(gindex + 5, gresolution)};
      int3 e{0, 0, 0};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto s = zinvul::Math::Builtin::frexp(x, &e);
      fresults[gindex + 3] = s.x;
      fresults[gindex + 4] = s.y;
      fresults[gindex + 5] = s.z;
      eresults[gindex + 3] = e.x;
      eresults[gindex + 4] = e.y;
      eresults[gindex + 5] = e.z;
      x = zinvul::Math::Builtin::ldexp(s, e);
      lresults[gindex + 3] = x.x;
      lresults[gindex + 4] = x.y;
      lresults[gindex + 5] = x.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      int4 e{0, 0, 0, 0};
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto s = zinvul::Math::Builtin::frexp(x, &e);
      fresults[gindex + 6] = s.x;
      fresults[gindex + 7] = s.y;
      fresults[gindex + 8] = s.z;
      fresults[gindex + 9] = s.w;
      eresults[gindex + 6] = e.x;
      eresults[gindex + 7] = e.y;
      eresults[gindex + 8] = e.z;
      eresults[gindex + 9] = e.w;
      x = zinvul::Math::Builtin::ldexp(s, e);
      lresults[gindex + 6] = x.x;
      lresults[gindex + 7] = x.y;
      lresults[gindex + 8] = x.z;
      lresults[gindex + 9] = x.w;
    }
  }
}

__kernel void testFrLdexpZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<float> lresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      float x = test::makeNormal<float>(gindex, gresolution);
      int e = 0;
      inputs[gindex] = x;
      const auto s = zinvul::Math::Zinvul::frexp(x, &e);
      fresults[gindex] = s;
      eresults[gindex] = e;
      x = zinvul::Math::Zinvul::ldexp(s, e);
      lresults[gindex] = x;
    }
    {
      float2 x{test::makeNormal<float>(gindex + 1, gresolution),
               test::makeNormal<float>(gindex + 2, gresolution)};
      int2 e{0, 0};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto s = zinvul::Math::Zinvul::frexp(x, &e);
      fresults[gindex + 1] = s.x;
      fresults[gindex + 2] = s.y;
      eresults[gindex + 1] = e.x;
      eresults[gindex + 2] = e.y;
      x = zinvul::Math::Zinvul::ldexp(s, e);
      lresults[gindex + 1] = x.x;
      lresults[gindex + 2] = x.y;
    }
    {
      float3 x{test::makeNormal<float>(gindex + 3, gresolution),
               test::makeNormal<float>(gindex + 4, gresolution),
               test::makeNormal<float>(gindex + 5, gresolution)};
      int3 e{0, 0, 0};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto s = zinvul::Math::Zinvul::frexp(x, &e);
      fresults[gindex + 3] = s.x;
      fresults[gindex + 4] = s.y;
      fresults[gindex + 5] = s.z;
      eresults[gindex + 3] = e.x;
      eresults[gindex + 4] = e.y;
      eresults[gindex + 5] = e.z;
      x = zinvul::Math::Zinvul::ldexp(s, e);
      lresults[gindex + 3] = x.x;
      lresults[gindex + 4] = x.y;
      lresults[gindex + 5] = x.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      int4 e{0, 0, 0, 0};
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto s = zinvul::Math::Zinvul::frexp(x, &e);
      fresults[gindex + 6] = s.x;
      fresults[gindex + 7] = s.y;
      fresults[gindex + 8] = s.z;
      fresults[gindex + 9] = s.w;
      eresults[gindex + 6] = e.x;
      eresults[gindex + 7] = e.y;
      eresults[gindex + 8] = e.z;
      eresults[gindex + 9] = e.w;
      x = zinvul::Math::Zinvul::ldexp(s, e);
      lresults[gindex + 6] = x.x;
      lresults[gindex + 7] = x.y;
      lresults[gindex + 8] = x.z;
      lresults[gindex + 9] = x.w;
    }
  }
}

__kernel void testModf(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    GlobalPtr<float> fresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      float i = 0.0f;
      const auto y = zinvul::modf(x, &i);
      results[gindex] = y;
      fresults[gindex] = i;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      float2 i{0.0f, 0.0f};
      const auto y = zinvul::modf(x, &i);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
      fresults[gindex + 1] = i.x;
      fresults[gindex + 2] = i.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      float3 i{0.0f, 0.0f, 0.0f};
      const auto y = zinvul::modf(x, &i);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
      fresults[gindex + 3] = i.x;
      fresults[gindex + 4] = i.y;
      fresults[gindex + 5] = i.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      float4 i{0.0f, 0.0f, 0.0f, 0.0f};
      const auto y = zinvul::modf(x, &i);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
      fresults[gindex + 6] = i.x;
      fresults[gindex + 7] = i.y;
      fresults[gindex + 8] = i.z;
      fresults[gindex + 9] = i.w;
    }
  }
}

__kernel void testModfBuiltin(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    GlobalPtr<float> fresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      float i = 0.0f;
      const auto y = zinvul::Math::Builtin::modf(x, &i);
      results[gindex] = y;
      fresults[gindex] = i;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      float2 i{0.0f, 0.0f};
      const auto y = zinvul::Math::Builtin::modf(x, &i);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
      fresults[gindex + 1] = i.x;
      fresults[gindex + 2] = i.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      float3 i{0.0f, 0.0f, 0.0f};
      const auto y = zinvul::Math::Builtin::modf(x, &i);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
      fresults[gindex + 3] = i.x;
      fresults[gindex + 4] = i.y;
      fresults[gindex + 5] = i.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      float4 i{0.0f, 0.0f, 0.0f, 0.0f};
      const auto y = zinvul::Math::Builtin::modf(x, &i);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
      fresults[gindex + 6] = i.x;
      fresults[gindex + 7] = i.y;
      fresults[gindex + 8] = i.z;
      fresults[gindex + 9] = i.w;
    }
  }
}

__kernel void testModfZinvul(GlobalPtr<float> inputs,
    GlobalPtr<float> results,
    GlobalPtr<float> fresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      inputs[gindex] = x;
      float i = 0.0f;
      const auto y = zinvul::Math::Zinvul::modf(x, &i);
      results[gindex] = y;
      fresults[gindex] = i;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      float2 i{0.0f, 0.0f};
      const auto y = zinvul::Math::Zinvul::modf(x, &i);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
      fresults[gindex + 1] = i.x;
      fresults[gindex + 2] = i.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      float3 i{0.0f, 0.0f, 0.0f};
      const auto y = zinvul::Math::Zinvul::modf(x, &i);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
      fresults[gindex + 3] = i.x;
      fresults[gindex + 4] = i.y;
      fresults[gindex + 5] = i.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      float4 i{0.0f, 0.0f, 0.0f, 0.0f};
      const auto y = zinvul::Math::Zinvul::modf(x, &i);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
      fresults[gindex + 6] = i.x;
      fresults[gindex + 7] = i.y;
      fresults[gindex + 8] = i.z;
      fresults[gindex + 9] = i.w;
    }
  }
}

__kernel void testCopysign(GlobalPtr<float> xinputs,
    GlobalPtr<float> yinputs,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const float x = test::makeNormal<float>(gindex, gresolution);
      const float y = test::makeNormalY<float>(gindex);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::copysign(x, y);
      results[gindex] = z;
    }
    {
      const float2 x{test::makeNormal<float>(gindex + 1, gresolution),
                     test::makeNormal<float>(gindex + 2, gresolution)};
      const float2 y{test::makeNormalY<float>(gindex + 1),
                     test::makeNormalY<float>(gindex + 2)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::copysign(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const float3 x{test::makeNormal<float>(gindex + 3, gresolution),
                     test::makeNormal<float>(gindex + 4, gresolution),
                     test::makeNormal<float>(gindex + 5, gresolution)};
      const float3 y{test::makeNormalY<float>(gindex + 3),
                     test::makeNormalY<float>(gindex + 4),
                     test::makeNormalY<float>(gindex + 5)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::copysign(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      float4 x{test::makeNormal<float>(gindex + 6, gresolution),
               test::makeNormal<float>(gindex + 7, gresolution),
               test::makeNormal<float>(gindex + 8, gresolution),
               test::makeNormal<float>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = float4{0.0f,
                   zinvul::NumericLimits<float>::infinity(),
                   -zinvul::NumericLimits<float>::infinity(),
                   zinvul::NumericLimits<float>::quietNan()};
      }
      const float4 y{test::makeNormalY<float>(gindex + 6),
                     test::makeNormalY<float>(gindex + 7),
                     test::makeNormalY<float>(gindex + 8),
                     test::makeNormalY<float>(gindex + 9)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::copysign(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
  }
}

#if !defined(Z_MAC)

#if defined(ZINVUL_VULKAN)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif // ZINVUL_VULKAN

// Forward declaration
__kernel void testConstantValueF64(GlobalPtr<int32b> iresults,
    GlobalPtr<double> fresults)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index == 0) {
    size_t i = 0;
    {
      constexpr int32b k = DBL_DIG;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MANT_DIG;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MAX_10_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MAX_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MIN_10_EXP;
      iresults[i++] = k;
    }
    {
      constexpr int32b k = DBL_MIN_EXP;
      iresults[i++] = k;
    }

    i = 0;
    {
      constexpr double k = DBL_MAX;
      fresults[i++] = k;
    }
    {
      constexpr double k = DBL_MIN;
      fresults[i++] = k;
    }
    {
      constexpr double k = DBL_EPSILON;
      fresults[i++] = k;
    }

    // Math constant values
    {
      constexpr double k = M_E;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_LOG2E;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_LOG10E;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_LN2;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_LN10;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_PI;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_PI_2;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_PI_4;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_1_PI;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_2_PI;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_2_SQRTPI;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_SQRT2;
      fresults[i++] = k;
    }
    {
      constexpr double k = M_SQRT1_2;
      fresults[i++] = k;
    }

    {
      constexpr double pi = zinvul::kPi<double>;
      fresults[i++] = pi;
    }
    {
      constexpr double pi = zinvul::kPi<double>;
      constexpr double s = zinvul::mathconst::sqrt(pi);
      fresults[i++] = s;
    }
    {
      constexpr double s = zinvul::mathconst::sqrt(2.0);
      fresults[i++] = s;
    }
    {
      constexpr double s = zinvul::mathconst::sqrt(3.0);
      fresults[i++] = s;
    }
    {
      constexpr double pi = zinvul::kPi<double>;
      constexpr double c = zinvul::mathconst::cbrt(pi);
      fresults[i++] = c;
    }
    {
      constexpr double c = zinvul::mathconst::cbrt(2.0);
      fresults[i++] = c;
    }
    {
      constexpr double c = zinvul::mathconst::cbrt(3.0);
      fresults[i++] = c;
    }
  }
}

__kernel void testMakeNormalF64(GlobalPtr<double> pinputs,
    GlobalPtr<double> inputs,
    GlobalPtr<double> yinputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    {
      const auto x = test::makeNormalP<double>(index, resolution);
      pinputs[index] = x;
    }
    {
      const auto x = test::makeNormal<double>(index, resolution);
      inputs[index] = x;
    }
    {
      const auto x = test::makeNormalY<double>(index, 4.0);
      yinputs[index] = x;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCeilF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::ceil(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::ceil(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::ceil(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::ceil(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testFloorF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::floor(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::floor(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::floor(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::floor(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testTruncF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::trunc(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::trunc(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::trunc(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::trunc(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRoundF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::round(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::round(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::round(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::round(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRoundBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::round(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::round(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::round(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::round(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRoundZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::round(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::round(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::round(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::round(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRintF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::rint(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::rint(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::rint(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::rint(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRintBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::rint(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::rint(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::rint(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::rint(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRintZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::rint(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::rint(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::rint(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::rint(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testFmodF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.9;
  constexpr double yscale = 10.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      const double y = test::makeNormalY<double>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::fmod(x, y);
      results[gindex] = z;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      const double2 y{test::makeNormalY<double>(gindex + 1, yscale),
                      test::makeNormalY<double>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::fmod(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      const double3 y{test::makeNormalY<double>(gindex + 3, yscale),
                      test::makeNormalY<double>(gindex + 4, yscale),
                      test::makeNormalY<double>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::fmod(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      const double4 y{test::makeNormalY<double>(gindex + 6, yscale),
                      test::makeNormalY<double>(gindex + 7, yscale),
                      test::makeNormalY<double>(gindex + 8, yscale),
                      test::makeNormalY<double>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::fmod(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testFmodBuiltinF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.9;
  constexpr double yscale = 10.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      const double y = test::makeNormalY<double>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::Math::Builtin::fmod(x, y);
      results[gindex] = z;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      const double2 y{test::makeNormalY<double>(gindex + 1, yscale),
                      test::makeNormalY<double>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::Math::Builtin::fmod(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      const double3 y{test::makeNormalY<double>(gindex + 3, yscale),
                      test::makeNormalY<double>(gindex + 4, yscale),
                      test::makeNormalY<double>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::Math::Builtin::fmod(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      const double4 y{test::makeNormalY<double>(gindex + 6, yscale),
                      test::makeNormalY<double>(gindex + 7, yscale),
                      test::makeNormalY<double>(gindex + 8, yscale),
                      test::makeNormalY<double>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::Math::Builtin::fmod(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testFmodZinvulF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.9;
  constexpr double yscale = 10.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      const double y = test::makeNormalY<double>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::Math::Zinvul::fmod(x, y);
      results[gindex] = z;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      const double2 y{test::makeNormalY<double>(gindex + 1, yscale),
                      test::makeNormalY<double>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::Math::Zinvul::fmod(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      const double3 y{test::makeNormalY<double>(gindex + 3, yscale),
                      test::makeNormalY<double>(gindex + 4, yscale),
                      test::makeNormalY<double>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::Math::Zinvul::fmod(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      const double4 y{test::makeNormalY<double>(gindex + 6, yscale),
                      test::makeNormalY<double>(gindex + 7, yscale),
                      test::makeNormalY<double>(gindex + 8, yscale),
                      test::makeNormalY<double>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::Math::Zinvul::fmod(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testExpF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 16.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalY<double>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::exp(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalY<double>(gindex + 1, scale),
                      test::makeNormalY<double>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::exp(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalY<double>(gindex + 3, scale),
                      test::makeNormalY<double>(gindex + 4, scale),
                      test::makeNormalY<double>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::exp(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalY<double>(gindex + 6, scale),
                test::makeNormalY<double>(gindex + 7, scale),
                test::makeNormalY<double>(gindex + 8, scale),
                test::makeNormalY<double>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::exp(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testExpBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 16.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalY<double>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::exp(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalY<double>(gindex + 1, scale),
                      test::makeNormalY<double>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::exp(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalY<double>(gindex + 3, scale),
                      test::makeNormalY<double>(gindex + 4, scale),
                      test::makeNormalY<double>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::exp(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalY<double>(gindex + 6, scale),
                test::makeNormalY<double>(gindex + 7, scale),
                test::makeNormalY<double>(gindex + 8, scale),
                test::makeNormalY<double>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::exp(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testExpZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 16.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalY<double>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::exp(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalY<double>(gindex + 1, scale),
                      test::makeNormalY<double>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::exp(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalY<double>(gindex + 3, scale),
                      test::makeNormalY<double>(gindex + 4, scale),
                      test::makeNormalY<double>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::exp(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalY<double>(gindex + 6, scale),
                test::makeNormalY<double>(gindex + 7, scale),
                test::makeNormalY<double>(gindex + 8, scale),
                test::makeNormalY<double>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::exp(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testExp2F64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 16.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalY<double>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::exp2(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalY<double>(gindex + 1, scale),
                      test::makeNormalY<double>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::exp2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalY<double>(gindex + 3, scale),
                      test::makeNormalY<double>(gindex + 4, scale),
                      test::makeNormalY<double>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::exp2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalY<double>(gindex + 6, scale),
                test::makeNormalY<double>(gindex + 7, scale),
                test::makeNormalY<double>(gindex + 8, scale),
                test::makeNormalY<double>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::exp2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testExp2BuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 16.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalY<double>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::exp2(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalY<double>(gindex + 1, scale),
                      test::makeNormalY<double>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::exp2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalY<double>(gindex + 3, scale),
                      test::makeNormalY<double>(gindex + 4, scale),
                      test::makeNormalY<double>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::exp2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalY<double>(gindex + 6, scale),
                test::makeNormalY<double>(gindex + 7, scale),
                test::makeNormalY<double>(gindex + 8, scale),
                test::makeNormalY<double>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::exp2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testExp2ZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 16.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalY<double>(gindex, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::exp2(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalY<double>(gindex + 1, scale),
                      test::makeNormalY<double>(gindex + 2, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::exp2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalY<double>(gindex + 3, scale),
                      test::makeNormalY<double>(gindex + 4, scale),
                      test::makeNormalY<double>(gindex + 5, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::exp2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalY<double>(gindex + 6, scale),
                test::makeNormalY<double>(gindex + 7, scale),
                test::makeNormalY<double>(gindex + 8, scale),
                test::makeNormalY<double>(gindex + 9, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::exp2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testLogF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::log(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::log(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::log(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::log(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testLogBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::log(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::log(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::log(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::log(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testLogZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::log(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::log(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::log(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::log(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testLog2F64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::log2(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::log2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::log2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::log2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testLog2BuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::log2(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::log2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::log2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::log2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testLog2ZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::log2(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::log2(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::log2(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::log2(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testPowF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.6;
  constexpr double yscale = 4.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      const double y = test::makeNormalY<double>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::pow(x, y);
      results[gindex] = z;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      const double2 y{test::makeNormalY<double>(gindex + 1, yscale),
                      test::makeNormalY<double>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::pow(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      const double3 y{test::makeNormalY<double>(gindex + 3, yscale),
                      test::makeNormalY<double>(gindex + 4, yscale),
                      test::makeNormalY<double>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::pow(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      const double4 y{test::makeNormalY<double>(gindex + 6, yscale),
                      test::makeNormalY<double>(gindex + 7, yscale),
                      test::makeNormalY<double>(gindex + 8, yscale),
                      test::makeNormalY<double>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::pow(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testPowBuiltinF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.6;
  constexpr double yscale = 4.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      const double y = test::makeNormalY<double>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::Math::Builtin::pow(x, y);
      results[gindex] = z;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      const double2 y{test::makeNormalY<double>(gindex + 1, yscale),
                      test::makeNormalY<double>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::Math::Builtin::pow(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      const double3 y{test::makeNormalY<double>(gindex + 3, yscale),
                      test::makeNormalY<double>(gindex + 4, yscale),
                      test::makeNormalY<double>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::Math::Builtin::pow(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      const double4 y{test::makeNormalY<double>(gindex + 6, yscale),
                      test::makeNormalY<double>(gindex + 7, yscale),
                      test::makeNormalY<double>(gindex + 8, yscale),
                      test::makeNormalY<double>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::Math::Builtin::pow(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testPowZinvulF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.6;
  constexpr double yscale = 4.0;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      const double y = test::makeNormalY<double>(gindex, yscale);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::Math::Zinvul::pow(x, y);
      results[gindex] = z;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      const double2 y{test::makeNormalY<double>(gindex + 1, yscale),
                      test::makeNormalY<double>(gindex + 2, yscale)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::Math::Zinvul::pow(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      const double3 y{test::makeNormalY<double>(gindex + 3, yscale),
                      test::makeNormalY<double>(gindex + 4, yscale),
                      test::makeNormalY<double>(gindex + 5, yscale)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::Math::Zinvul::pow(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                   zinvul::NumericLimits<double>::infinity(),
                   -zinvul::NumericLimits<double>::infinity(),
                   zinvul::NumericLimits<double>::quietNan()};
      }
      const double4 y{test::makeNormalY<double>(gindex + 6, yscale),
                      test::makeNormalY<double>(gindex + 7, yscale),
                      test::makeNormalY<double>(gindex + 8, yscale),
                      test::makeNormalY<double>(gindex + 9, yscale)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::Math::Zinvul::pow(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRsqrtF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::rsqrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::rsqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::rsqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::rsqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRsqrtBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::rsqrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::rsqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::rsqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::rsqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testRsqrtZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::rsqrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::rsqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::rsqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::rsqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSqrtF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::sqrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::sqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::sqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::sqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSqrtBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::sqrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::sqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::sqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::sqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSqrtZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormalP<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::sqrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormalP<double>(gindex + 1, gresolution),
                      test::makeNormalP<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::sqrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormalP<double>(gindex + 3, gresolution),
                      test::makeNormalP<double>(gindex + 4, gresolution),
                      test::makeNormalP<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::sqrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormalP<double>(gindex + 6, gresolution),
                test::makeNormalP<double>(gindex + 7, gresolution),
                test::makeNormalP<double>(gindex + 8, gresolution),
                test::makeNormalP<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::sqrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCbrtF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::cbrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::cbrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::cbrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::cbrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCbrtBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::cbrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::cbrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::cbrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::cbrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCbrtZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::cbrt(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::cbrt(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::cbrt(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::cbrt(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::sin(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSinBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSinZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCosF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::cos(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCosBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCosZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testTanF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::tan(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testTanBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testTanZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  constexpr double scale = 0.56;
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution, scale);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution, scale),
                      test::makeNormal<double>(gindex + 2, gresolution, scale)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution, scale),
                      test::makeNormal<double>(gindex + 4, gresolution, scale),
                      test::makeNormal<double>(gindex + 5, gresolution, scale)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution, scale),
                test::makeNormal<double>(gindex + 7, gresolution, scale),
                test::makeNormal<double>(gindex + 8, gresolution, scale),
                test::makeNormal<double>(gindex + 9, gresolution, scale)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSinPiF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::sin(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSinPiBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testSinPiZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::sin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCosPiF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::cos(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCosPiBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCosPiZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::cos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testTanPiF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::tan(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testTanPiBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testTanPiZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;
    constexpr double k = 2.0 * zinvul::kPi<double>;

    {
      const double x = k * test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex] = y;
    }
    {
      const double2 x{k * test::makeNormal01<double>(gindex + 1, gresolution),
                      k * test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{k * test::makeNormal01<double>(gindex + 3, gresolution),
                      k * test::makeNormal01<double>(gindex + 4, gresolution),
                      k * test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{k * test::makeNormal01<double>(gindex + 6, gresolution),
                k * test::makeNormal01<double>(gindex + 7, gresolution),
                k * test::makeNormal01<double>(gindex + 8, gresolution),
                k * test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::tan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}


__kernel void testAsinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::asin(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal01<double>(gindex + 1, gresolution),
                      test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::asin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal01<double>(gindex + 3, gresolution),
                      test::makeNormal01<double>(gindex + 4, gresolution),
                      test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::asin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal01<double>(gindex + 6, gresolution),
                test::makeNormal01<double>(gindex + 7, gresolution),
                test::makeNormal01<double>(gindex + 8, gresolution),
                test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::asin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testAsinBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::asin(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal01<double>(gindex + 1, gresolution),
                      test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::asin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal01<double>(gindex + 3, gresolution),
                      test::makeNormal01<double>(gindex + 4, gresolution),
                      test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::asin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal01<double>(gindex + 6, gresolution),
                test::makeNormal01<double>(gindex + 7, gresolution),
                test::makeNormal01<double>(gindex + 8, gresolution),
                test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::asin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testAsinZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::asin(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal01<double>(gindex + 1, gresolution),
                      test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::asin(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal01<double>(gindex + 3, gresolution),
                      test::makeNormal01<double>(gindex + 4, gresolution),
                      test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::asin(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal01<double>(gindex + 6, gresolution),
                test::makeNormal01<double>(gindex + 7, gresolution),
                test::makeNormal01<double>(gindex + 8, gresolution),
                test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::asin(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testAcosF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::acos(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal01<double>(gindex + 1, gresolution),
                      test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::acos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal01<double>(gindex + 3, gresolution),
                      test::makeNormal01<double>(gindex + 4, gresolution),
                      test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::acos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal01<double>(gindex + 6, gresolution),
                test::makeNormal01<double>(gindex + 7, gresolution),
                test::makeNormal01<double>(gindex + 8, gresolution),
                test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::acos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testAcosBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::acos(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal01<double>(gindex + 1, gresolution),
                      test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::acos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal01<double>(gindex + 3, gresolution),
                      test::makeNormal01<double>(gindex + 4, gresolution),
                      test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::acos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal01<double>(gindex + 6, gresolution),
                test::makeNormal01<double>(gindex + 7, gresolution),
                test::makeNormal01<double>(gindex + 8, gresolution),
                test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::acos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testAcosZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal01<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::acos(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal01<double>(gindex + 1, gresolution),
                      test::makeNormal01<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::acos(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal01<double>(gindex + 3, gresolution),
                      test::makeNormal01<double>(gindex + 4, gresolution),
                      test::makeNormal01<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::acos(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal01<double>(gindex + 6, gresolution),
                test::makeNormal01<double>(gindex + 7, gresolution),
                test::makeNormal01<double>(gindex + 8, gresolution),
                test::makeNormal01<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::acos(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testAtanF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::atan(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::atan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::atan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::atan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testAtanBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Builtin::atan(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Builtin::atan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Builtin::atan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Builtin::atan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testAtanZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      const auto y = zinvul::Math::Zinvul::atan(x);
      results[gindex] = y;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto y = zinvul::Math::Zinvul::atan(x);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto y = zinvul::Math::Zinvul::atan(x);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto y = zinvul::Math::Zinvul::atan(x);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testFrLdexpF64(GlobalPtr<double> inputs,
    GlobalPtr<double> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<double> lresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      double x = test::makeNormal<double>(gindex, gresolution);
      int e = 0;
      inputs[gindex] = x;
      const auto s = zinvul::frexp(x, &e);
      fresults[gindex] = s;
      eresults[gindex] = e;
      x = zinvul::ldexp(s, e);
      lresults[gindex] = x;
    }
    {
      double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                test::makeNormal<double>(gindex + 2, gresolution)};
      int2 e{0, 0};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto s = zinvul::frexp(x, &e);
      fresults[gindex + 1] = s.x;
      fresults[gindex + 2] = s.y;
      eresults[gindex + 1] = e.x;
      eresults[gindex + 2] = e.y;
      x = zinvul::ldexp(s, e);
      lresults[gindex + 1] = x.x;
      lresults[gindex + 2] = x.y;
    }
    {
      double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                test::makeNormal<double>(gindex + 4, gresolution),
                test::makeNormal<double>(gindex + 5, gresolution)};
      int3 e{0, 0, 0};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto s = zinvul::frexp(x, &e);
      fresults[gindex + 3] = s.x;
      fresults[gindex + 4] = s.y;
      fresults[gindex + 5] = s.z;
      eresults[gindex + 3] = e.x;
      eresults[gindex + 4] = e.y;
      eresults[gindex + 5] = e.z;
      x = zinvul::ldexp(s, e);
      lresults[gindex + 3] = x.x;
      lresults[gindex + 4] = x.y;
      lresults[gindex + 5] = x.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      int4 e{0, 0, 0, 0};
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto s = zinvul::frexp(x, &e);
      fresults[gindex + 6] = s.x;
      fresults[gindex + 7] = s.y;
      fresults[gindex + 8] = s.z;
      fresults[gindex + 9] = s.w;
      eresults[gindex + 6] = e.x;
      eresults[gindex + 7] = e.y;
      eresults[gindex + 8] = e.z;
      eresults[gindex + 9] = e.w;
      x = zinvul::ldexp(s, e);
      lresults[gindex + 6] = x.x;
      lresults[gindex + 7] = x.y;
      lresults[gindex + 8] = x.z;
      lresults[gindex + 9] = x.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testFrLdexpBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<double> lresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      double x = test::makeNormal<double>(gindex, gresolution);
      int e = 0;
      inputs[gindex] = x;
      const auto s = zinvul::Math::Builtin::frexp(x, &e);
      fresults[gindex] = s;
      eresults[gindex] = e;
      x = zinvul::Math::Builtin::ldexp(s, e);
      lresults[gindex] = x;
    }
    {
      double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                test::makeNormal<double>(gindex + 2, gresolution)};
      int2 e{0, 0};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto s = zinvul::Math::Builtin::frexp(x, &e);
      fresults[gindex + 1] = s.x;
      fresults[gindex + 2] = s.y;
      eresults[gindex + 1] = e.x;
      eresults[gindex + 2] = e.y;
      x = zinvul::Math::Builtin::ldexp(s, e);
      lresults[gindex + 1] = x.x;
      lresults[gindex + 2] = x.y;
    }
    {
      double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                test::makeNormal<double>(gindex + 4, gresolution),
                test::makeNormal<double>(gindex + 5, gresolution)};
      int3 e{0, 0, 0};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto s = zinvul::Math::Builtin::frexp(x, &e);
      fresults[gindex + 3] = s.x;
      fresults[gindex + 4] = s.y;
      fresults[gindex + 5] = s.z;
      eresults[gindex + 3] = e.x;
      eresults[gindex + 4] = e.y;
      eresults[gindex + 5] = e.z;
      x = zinvul::Math::Builtin::ldexp(s, e);
      lresults[gindex + 3] = x.x;
      lresults[gindex + 4] = x.y;
      lresults[gindex + 5] = x.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      int4 e{0, 0, 0, 0};
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto s = zinvul::Math::Builtin::frexp(x, &e);
      fresults[gindex + 6] = s.x;
      fresults[gindex + 7] = s.y;
      fresults[gindex + 8] = s.z;
      fresults[gindex + 9] = s.w;
      eresults[gindex + 6] = e.x;
      eresults[gindex + 7] = e.y;
      eresults[gindex + 8] = e.z;
      eresults[gindex + 9] = e.w;
      x = zinvul::Math::Builtin::ldexp(s, e);
      lresults[gindex + 6] = x.x;
      lresults[gindex + 7] = x.y;
      lresults[gindex + 8] = x.z;
      lresults[gindex + 9] = x.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testFrLdexpZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> fresults,
    GlobalPtr<int32b> eresults,
    GlobalPtr<double> lresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      double x = test::makeNormal<double>(gindex, gresolution);
      int e = 0;
      inputs[gindex] = x;
      const auto s = zinvul::Math::Zinvul::frexp(x, &e);
      fresults[gindex] = s;
      eresults[gindex] = e;
      x = zinvul::Math::Zinvul::ldexp(s, e);
      lresults[gindex] = x;
    }
    {
      double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                test::makeNormal<double>(gindex + 2, gresolution)};
      int2 e{0, 0};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      const auto s = zinvul::Math::Zinvul::frexp(x, &e);
      fresults[gindex + 1] = s.x;
      fresults[gindex + 2] = s.y;
      eresults[gindex + 1] = e.x;
      eresults[gindex + 2] = e.y;
      x = zinvul::Math::Zinvul::ldexp(s, e);
      lresults[gindex + 1] = x.x;
      lresults[gindex + 2] = x.y;
    }
    {
      double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                test::makeNormal<double>(gindex + 4, gresolution),
                test::makeNormal<double>(gindex + 5, gresolution)};
      int3 e{0, 0, 0};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      const auto s = zinvul::Math::Zinvul::frexp(x, &e);
      fresults[gindex + 3] = s.x;
      fresults[gindex + 4] = s.y;
      fresults[gindex + 5] = s.z;
      eresults[gindex + 3] = e.x;
      eresults[gindex + 4] = e.y;
      eresults[gindex + 5] = e.z;
      x = zinvul::Math::Zinvul::ldexp(s, e);
      lresults[gindex + 3] = x.x;
      lresults[gindex + 4] = x.y;
      lresults[gindex + 5] = x.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      int4 e{0, 0, 0, 0};
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      const auto s = zinvul::Math::Zinvul::frexp(x, &e);
      fresults[gindex + 6] = s.x;
      fresults[gindex + 7] = s.y;
      fresults[gindex + 8] = s.z;
      fresults[gindex + 9] = s.w;
      eresults[gindex + 6] = e.x;
      eresults[gindex + 7] = e.y;
      eresults[gindex + 8] = e.z;
      eresults[gindex + 9] = e.w;
      x = zinvul::Math::Zinvul::ldexp(s, e);
      lresults[gindex + 6] = x.x;
      lresults[gindex + 7] = x.y;
      lresults[gindex + 8] = x.z;
      lresults[gindex + 9] = x.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testModfF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    GlobalPtr<double> fresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      double i = 0.0;
      const auto y = zinvul::modf(x, &i);
      results[gindex] = y;
      fresults[gindex] = i;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                     test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      double2 i{0.0, 0.0};
      const auto y = zinvul::modf(x, &i);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
      fresults[gindex + 1] = i.x;
      fresults[gindex + 2] = i.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                     test::makeNormal<double>(gindex + 4, gresolution),
                     test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      double3 i{0.0, 0.0, 0.0};
      const auto y = zinvul::modf(x, &i);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
      fresults[gindex + 3] = i.x;
      fresults[gindex + 4] = i.y;
      fresults[gindex + 5] = i.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      double4 i{0.0, 0.0, 0.0, 0.0};
      const auto y = zinvul::modf(x, &i);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
      fresults[gindex + 6] = i.x;
      fresults[gindex + 7] = i.y;
      fresults[gindex + 8] = i.z;
      fresults[gindex + 9] = i.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testModfBuiltinF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    GlobalPtr<double> fresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      double i = 0.0;
      const auto y = zinvul::Math::Builtin::modf(x, &i);
      results[gindex] = y;
      fresults[gindex] = i;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      double2 i{0.0, 0.0};
      const auto y = zinvul::Math::Builtin::modf(x, &i);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
      fresults[gindex + 1] = i.x;
      fresults[gindex + 2] = i.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      double3 i{0.0, 0.0, 0.0};
      const auto y = zinvul::Math::Builtin::modf(x, &i);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
      fresults[gindex + 3] = i.x;
      fresults[gindex + 4] = i.y;
      fresults[gindex + 5] = i.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      double4 i{0.0, 0.0, 0.0, 0.0};
      const auto y = zinvul::Math::Builtin::modf(x, &i);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
      fresults[gindex + 6] = i.x;
      fresults[gindex + 7] = i.y;
      fresults[gindex + 8] = i.z;
      fresults[gindex + 9] = i.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testModfZinvulF64(GlobalPtr<double> inputs,
    GlobalPtr<double> results,
    GlobalPtr<double> fresults,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      inputs[gindex] = x;
      double i = 0.0;
      const auto y = zinvul::Math::Zinvul::modf(x, &i);
      results[gindex] = y;
      fresults[gindex] = i;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      inputs[gindex + 1] = x.x;
      inputs[gindex + 2] = x.y;
      double2 i{0.0, 0.0};
      const auto y = zinvul::Math::Zinvul::modf(x, &i);
      results[gindex + 1] = y.x;
      results[gindex + 2] = y.y;
      fresults[gindex + 1] = i.x;
      fresults[gindex + 2] = i.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      inputs[gindex + 3] = x.x;
      inputs[gindex + 4] = x.y;
      inputs[gindex + 5] = x.z;
      double3 i{0.0, 0.0, 0.0};
      const auto y = zinvul::Math::Zinvul::modf(x, &i);
      results[gindex + 3] = y.x;
      results[gindex + 4] = y.y;
      results[gindex + 5] = y.z;
      fresults[gindex + 3] = i.x;
      fresults[gindex + 4] = i.y;
      fresults[gindex + 5] = i.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      inputs[gindex + 6] = x.x;
      inputs[gindex + 7] = x.y;
      inputs[gindex + 8] = x.z;
      inputs[gindex + 9] = x.w;
      double4 i{0.0, 0.0, 0.0, 0.0};
      const auto y = zinvul::Math::Zinvul::modf(x, &i);
      results[gindex + 6] = y.x;
      results[gindex + 7] = y.y;
      results[gindex + 8] = y.z;
      results[gindex + 9] = y.w;
      fresults[gindex + 6] = i.x;
      fresults[gindex + 7] = i.y;
      fresults[gindex + 8] = i.z;
      fresults[gindex + 9] = i.w;
    }
#endif // ZINVUL_CPU
  }
}

__kernel void testCopysignF64(GlobalPtr<double> xinputs,
    GlobalPtr<double> yinputs,
    GlobalPtr<double> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
#if defined(ZINVUL_CPU)
    constexpr uint32b n = 10u;
    const uint32b gresolution = n * resolution;
    const uint32b gindex = n * index;

    {
      const double x = test::makeNormal<double>(gindex, gresolution);
      const double y = test::makeNormalY<double>(gindex);
      xinputs[gindex] = x;
      yinputs[gindex] = y;
      const auto z = zinvul::copysign(x, y);
      results[gindex] = z;
    }
    {
      const double2 x{test::makeNormal<double>(gindex + 1, gresolution),
                      test::makeNormal<double>(gindex + 2, gresolution)};
      const double2 y{test::makeNormalY<double>(gindex + 1),
                      test::makeNormalY<double>(gindex + 2)};
      xinputs[gindex + 1] = x.x;
      xinputs[gindex + 2] = x.y;
      yinputs[gindex + 1] = y.x;
      yinputs[gindex + 2] = y.y;
      const auto z = zinvul::copysign(x, y);
      results[gindex + 1] = z.x;
      results[gindex + 2] = z.y;
    }
    {
      const double3 x{test::makeNormal<double>(gindex + 3, gresolution),
                      test::makeNormal<double>(gindex + 4, gresolution),
                      test::makeNormal<double>(gindex + 5, gresolution)};
      const double3 y{test::makeNormalY<double>(gindex + 3),
                      test::makeNormalY<double>(gindex + 4),
                      test::makeNormalY<double>(gindex + 5)};
      xinputs[gindex + 3] = x.x;
      xinputs[gindex + 4] = x.y;
      xinputs[gindex + 5] = x.z;
      yinputs[gindex + 3] = y.x;
      yinputs[gindex + 4] = y.y;
      yinputs[gindex + 5] = y.z;
      const auto z = zinvul::copysign(x, y);
      results[gindex + 3] = z.x;
      results[gindex + 4] = z.y;
      results[gindex + 5] = z.z;
    }
    {
      double4 x{test::makeNormal<double>(gindex + 6, gresolution),
                test::makeNormal<double>(gindex + 7, gresolution),
                test::makeNormal<double>(gindex + 8, gresolution),
                test::makeNormal<double>(gindex + 9, gresolution)};
      if (index == (resolution - 1)) {
        x = double4{0.0,
                    zinvul::NumericLimits<double>::infinity(),
                    -zinvul::NumericLimits<double>::infinity(),
                    zinvul::NumericLimits<double>::quietNan()};
      }
      const double4 y{test::makeNormalY<double>(gindex + 6),
                     test::makeNormalY<double>(gindex + 7),
                     test::makeNormalY<double>(gindex + 8),
                     test::makeNormalY<double>(gindex + 9)};
      xinputs[gindex + 6] = x.x;
      xinputs[gindex + 7] = x.y;
      xinputs[gindex + 8] = x.z;
      xinputs[gindex + 9] = x.w;
      yinputs[gindex + 6] = y.x;
      yinputs[gindex + 7] = y.y;
      yinputs[gindex + 8] = y.z;
      yinputs[gindex + 9] = y.w;
      const auto z = zinvul::copysign(x, y);
      results[gindex + 6] = z.x;
      results[gindex + 7] = z.y;
      results[gindex + 8] = z.z;
      results[gindex + 9] = z.w;
    }
#endif // ZINVUL_CPU
  }
}

#if defined(ZINVUL_VULKAN)
#pragma clang diagnostic pop
#pragma clang diagnostic pop
#endif // ZINVUL_VULKAN

#endif // !Z_MAC

#endif /* ZINVUL_MATH_TEST_MATH_CL */
