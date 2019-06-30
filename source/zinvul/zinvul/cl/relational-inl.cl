/*!
  \file relational-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_RELATIONAL_INL_CL
#define ZINVUL_RELATIONAL_INL_CL

#include "relational.cl"
// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isEqual(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = isequal(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isNotEqual(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = isnotequal(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isGreater(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = isgreater(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isGreaterEqual(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = isgreaterequal(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isLess(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = isless(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isLessEqual(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = islessequal(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isInf(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = isinf(arg);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isNan(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = isnan(arg);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isSignBitSet(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const Private<ResultType> result = signbit(arg);
  return result;
}

/*!
  */
template <typename TypeN> inline
TypeN selectBit(const TypeN a, const TypeN b, const TypeN c) noexcept
{
  const auto result = bitselect(a, b, c);
  return result;
}

/*!
  */
template <typename TypeN, typename IntegerN> inline
TypeN selectValue(const TypeN a, const TypeN b, const IntegerN c) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const auto result = select(a, b, c);
  return result;
}

} // namespace zinvul

#endif /* ZINVUL_RELATIONAL_INL_CL */
