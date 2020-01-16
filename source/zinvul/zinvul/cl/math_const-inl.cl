/*!
  \file math_const-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_CONST_INL_CL
#define ZINVUL_MATH_CONST_INL_CL

#include "math_const.cl"
// Zinvul
#include "limits.cl"
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

namespace mathconst {

/*!
  */
template <typename Float> inline
constexpr Float Math::pi() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  constexpr int32b n = NumericLimits<Float>::digits();
  const Float p = calcPi<Float>(n);
  return p;
}

/*!
  */
template <typename Arith> inline
constexpr Arith Math::absolute(const Arith x) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  if constexpr (kIsSigned<Arith>) {
    const Arith result = isNegative(x) ? -x : x;
    return result;
  }
  else {
    return x;
  }
}

/*!
  */
template <typename Arith> inline
constexpr Arith Math::fusedMulAdd(const Arith a,
                                  const Arith b,
                                  const Arith c) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  const Arith result = a * b + c;
  return result;
}

/*!
  */
template <typename Arith> inline
constexpr ComparisonResultType<Arith> Math::isNegative(const Arith x) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  using CmpResult = ComparisonResultType<Arith>;
  if constexpr (kIsSigned<Arith>) {
    constexpr auto zero = static_cast<Arith>(0);
    const auto result = static_cast<CmpResult>(x < zero);
    return result;
  }
  else {
    return kResultTrue<Arith>;
  }
}

/*!
  */
template <typename Integer> inline
constexpr ComparisonResultType<Integer> Math::isOdd(const Integer x) noexcept
{
  static_assert(kIsInteger<Integer>, "The Integer isn't integer type.");
  using CmpResult = ComparisonResultType<Integer>;
  constexpr Integer one = 0b01;
  const auto result = static_cast<CmpResult>((x & one) == one);
  return result;
}

/*!
  */
template <int32b kExponent, typename Arith> inline
constexpr Arith Math::power(Arith base) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  const auto zero = static_cast<Arith>(0);
  Arith y = zero;
  // Check if base is zero
  if (base != zero) {
    y = static_cast<Arith>(1);
    // Inverse pow
    if constexpr (kIsFloatingPoint<Arith> && isNegative(kExponent))
      base = invert(base);
    // Compute pow
    for (int32b exponent = abs(kExponent); 0 < exponent; exponent = exponent >> 1) {
      if (isOdd(exponent))
        y = y * base;
      if (1 < exponent)
        base = base * base;
    }
  }
  return y;
}

/*!
  */
template <typename Float> inline
constexpr Float Math::squareRoot(const Float x) noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  constexpr Float l = static_cast<Float>(0.5);
  constexpr Float h = static_cast<Float>(2.0);
  Float y = (h < x) ? l * x : (x < l) ? h * x : x;
  for (Float pre_y = static_cast<Float>(0.0); !isAlmostEqual(y, pre_y);) {
    constexpr Float k = static_cast<Float>(0.5);
    pre_y = y;
    y = k * (y + x / y);
  }
  return y;
}

/*!
  */
template <typename Float> inline
constexpr Float Math::cubicRoot(const Float x) noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  constexpr Float l = static_cast<Float>(1.0 / 3.0);
  constexpr Float h = static_cast<Float>(3.0);
  Float y = (h < x) ? l * x : (x < l) ? h * x : x;
  for (Float pre_y = static_cast<Float>(0.0); !isAlmostEqual(y, pre_y);) {
    constexpr Float k = static_cast<Float>(2.0);
    const Float y3 = power<3>(y);
    pre_y = y;
    y = y * (fma(k, x, y3) / fma(k, y3, x));
  }
  return y;
}

/*!
  */
template <size_t kUlpScale, typename Float> inline
constexpr Float Math::getUlps(const Float x) noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  constexpr Float e = static_cast<Float>(kUlpScale) *
                      NumericLimits<Float>::epsilon();
  const Float ulps = e * abs(x);
  return ulps;
}

/*!
  */
template <size_t kUlpScale, typename Arith> inline
constexpr ComparisonResultType<Arith> Math::isAlmostEqual(
    const Arith lhs,
    const Arith rhs) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  using CmpResult = ComparisonResultType<Arith>;
  if constexpr (kIsFloatingPoint<Arith>) {
    const auto v = abs(lhs - rhs);
    const auto ulps = getUlps<kUlpScale, Arith>(lhs + rhs);
    const auto result = static_cast<CmpResult>(v <= ulps);
    return result;
  }
  else {
    const auto result = static_cast<CmpResult>(lhs == rhs);
    return result;
  }
}

/*!
  */
template <typename Float> inline
constexpr Float Math::calcPi(const int32b n) noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  Float x = static_cast<Float>(2.0);
  for (int32b k = n; 0 < k; --k) {
    const Float t = static_cast<Float>(2 * k + 1);
    x = fma(static_cast<Float>(k), x, static_cast<Float>(2.0) * t) / t;
  }
  return x;
}

/*!
  */
template <typename Arith> inline
constexpr Arith abs(const Arith x) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  const auto result = Math::absolute(x);
  return result;
}

/*!
  */
template <typename Arith> inline
constexpr Arith fma(const Arith a, const Arith b, const Arith c) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  const auto result = Math::fusedMulAdd(a, b, c);
  return result;
}

/*!
  */
template <int32b kExponent, typename Arith> inline
constexpr Arith power(const Arith base) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  const auto y = Math::power<kExponent, Arith>(base);
  return y;
}

/*!
  */
template <typename Float> inline
constexpr Float sqrt(const Float x) noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  const auto y = Math::squareRoot(x);
  return y;
}

/*!
  */
template <typename Float> inline
constexpr Float cbrt(const Float x) noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  const auto y = Math::cubicRoot(x);
  return y;
}

} // namespace mathconst

} // namespace zinvul

#endif /* ZINVUL_MATH_CONST_INL_CL */
