/*!
  \file algorithm-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ALGORITHM_INL_CL
#define ZINVUL_ALGORITHM_INL_CL

#include "algorithm.cl"
// Zinvul
#include "types.cl"
#include "type_traits.cl"
#include "utility.cl"

namespace zinvul {

/*!
  */
template <typename ArithN> inline
auto Algorithm::absolute(const ArithN x) noexcept
{
  static_assert(kIsArithmetic<ArithN>, "The ArithN isn't arithmetic type.");
  if constexpr (kIsInteger<ArithN>) {
    const auto result = ZINVUL_GLOBAL_NAMESPACE::abs(x);
    return result;
  }
  else {
    const auto result = ZINVUL_GLOBAL_NAMESPACE::fabs(x);
    return result;
  }
}

/*!
  */
template <typename Arith1N, typename Arith2N> inline
Arith1N Algorithm::clamp(const Arith1N x,
                         const Arith2N minval,
                         const Arith2N maxval) noexcept
{
  static_assert(kIsArithmetic<Arith1N>, "The Arith1N isn't arithmetic type.");
  static_assert(kIsArithmetic<Arith2N>, "The Arith2N isn't arithmetic type.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::clamp(x, minval, maxval);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN Algorithm::invert(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using VecInfo = VectorTypeInfo<FloatN>;
  constexpr auto k = static_cast<typename VecInfo::ElementType>(1.0);
  const auto inv_x = k / x;
  return inv_x;
}

/*!
  */
template <typename ArithN> inline
ComparisonResultType<ArithN> Algorithm::isNegative(const ArithN x) noexcept
{
  static_assert(kIsArithmetic<ArithN>, "The ArithN isn't arithmetic type.");
  if constexpr (kIsSigned<ArithN>) {
    using VecInfo = VectorTypeInfo<ArithN>;
    constexpr auto zero = static_cast<typename VecInfo::ElementType>(0b0);
    const auto result = x < zero;
    return cast<ComparisonResultType<ArithN>>(result);
  }
  else {
    using CmpResult = ComparisonResultType<ArithN>;
    using CmpInfo = VectorTypeInfo<CmpResult>;
    constexpr auto f = static_cast<typename CmpInfo::ElementType>(kResultFalse);
    const auto result = make<CmpResult>(f);
    return result;
  }
}

/*!
  */
template <typename IntegerN> inline
ComparisonResultType<IntegerN> Algorithm::isOdd(const IntegerN x) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  using VecInfo = VectorTypeInfo<IntegerN>;
  constexpr auto lsb_mask = static_cast<typename VecInfo::ElementType>(0b1);
  const auto result = (x & lsb_mask) == lsb_mask;
  return cast<ComparisonResultType<IntegerN>>(result);
}

/*!
  */
template <typename IntegerN> inline
ComparisonResultType<IntegerN> Algorithm::isPowerOf2(const IntegerN x) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  using VecInfo = VectorTypeInfo<IntegerN>;
  constexpr auto zero = static_cast<typename VecInfo::ElementType>(0b0);
  constexpr auto one = static_cast<typename VecInfo::ElementType>(0b1);
  const auto result = (x & (x - one)) == zero;
  return cast<ComparisonResultType<IntegerN>>(result);
}

/*!
  */
template <typename ArithN> inline
ArithN Algorithm::max(const ArithN x, const ArithN y) noexcept
{
  static_assert(kIsArithmetic<ArithN>, "The ArithN isn't arithmetic type.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::max(x, y);
  return result;
}

/*!
  */
template <typename ArithN> inline
ArithN Algorithm::min(const ArithN x, const ArithN y) noexcept
{
  static_assert(kIsArithmetic<ArithN>, "The ArithN isn't arithmetic type.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::min(x, y);
  return result;
}

/*!
  */
template <typename Float1N, typename Float2N> inline
Float1N Algorithm::mix(const Float1N x, const Float1N y, const Float2N a) noexcept
{
  static_assert(kIsFloatingPoint<Float1N>, "The Float1N isn't floating point.");
  static_assert(kIsFloatingPoint<Float2N>, "The Float1N isn't floating point.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::mix(x, y, a);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN Algorithm::sign(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The Float1N isn't floating point.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::sign(x);
  return result;
}

/*!
  */
template <typename IntegerN> inline
IntegerN Algorithm::popcount(const IntegerN x) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::popcount(x);
  return result;
}

/*!
  */
template <typename ArithN> inline
auto abs(const ArithN x) noexcept
{
  static_assert(kIsArithmetic<ArithN>, "The ArithN isn't arithmetic type.");
  const auto result = Algorithm::absolute(x);
  return result;
}

/*!
  */
template <typename Arith1N, typename Arith2N> inline
Arith1N clamp(const Arith1N x, const Arith2N minval, const Arith2N maxval) noexcept
{
  static_assert(kIsArithmetic<Arith1N>, "The Arith1N isn't arithmetic type.");
  static_assert(kIsArithmetic<Arith2N>, "The Arith2N isn't arithmetic type.");
  const auto result = Algorithm::clamp(x, minval, maxval);
  return result;
}

/*!
  */
template <typename ArithN> inline
ArithN max(const ArithN x, const ArithN y) noexcept
{
  static_assert(kIsArithmetic<ArithN>, "The ArithN isn't arithmetic type.");
  const auto result = Algorithm::max(x, y);
  return result;
}

/*!
  */
template <typename ArithN> inline
ArithN min(const ArithN x, const ArithN y) noexcept
{
  static_assert(kIsArithmetic<ArithN>, "The ArithN isn't arithmetic type.");
  const auto result = Algorithm::min(x, y);
  return result;
}

/*!
  */
template <typename Float1N, typename Float2N> inline
Float1N mix(const Float1N x, const Float1N y, const Float2N a) noexcept
{
  static_assert(kIsFloatingPoint<Float1N>, "The Float1N isn't floating point.");
  static_assert(kIsFloatingPoint<Float2N>, "The Float1N isn't floating point.");
  const auto result = Algorithm::mix(x, y, a);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN sign(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The Float1N isn't floating point.");
  const auto result = Algorithm::sign(x);
  return result;
}
/*!
  */
template <typename IntegerN> inline
IntegerN popcount(const IntegerN x) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const auto result = Algorithm::popcount(x);
  return result;
}

} // namespace zinvul

#endif /* ZINVUL_ALGORITHM_INL_CL */
