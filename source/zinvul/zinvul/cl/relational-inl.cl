/*!
  \file relational-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_RELATIONAL_INL_CL
#define ZINVUL_RELATIONAL_INL_CL

#include "relational.cl"
// Zinvul
#include "types.cl"
#include "type_traits.cl"
#include "utility.cl"

namespace zinvul {

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isEqual(const FloatN x,
                                               const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::isequal(x, y));
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isNotEqual(const FloatN x,
                                                  const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::isnotequal(x, y));
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isGreater(const FloatN x,
                                                 const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::isgreater(x, y));
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isGreaterEqual(const FloatN x,
                                                      const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::isgreaterequal(x, y));
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isLess(const FloatN x,
                                              const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::isless(x, y));
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isLessEqual(const FloatN x,
                                                   const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::islessequal(x, y));
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isInf(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::isinf(arg));
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isNan(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::isnan(arg));
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> Relation::isSignBitSet(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  using ResultType = ComparisonResultType<FloatN>;
  const auto result = cast<ResultType>(ZINVUL_GLOBAL_NAMESPACE::signbit(arg));
  return result;
}

/*!
  */
template <typename TypeN> inline
TypeN Relation::selectBit(const TypeN a, const TypeN b, const TypeN c) noexcept
{
  const auto result = ZINVUL_GLOBAL_NAMESPACE::bitselect(a, b, c);
  return result;
}

/*!
  */
template <typename TypeN, typename IntegerN> inline
TypeN Relation::selectValue(const TypeN a,
                            const TypeN b,
                            const IntegerN c) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::select(a, b, c);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isequal(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isEqual(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isnotequal(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isNotEqual(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isgreater(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isGreater(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isgreaterequal(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isGreaterEqual(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isless(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isLess(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> islessequal(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isLessEqual(x, y);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isinf(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isInf(arg);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> isnan(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isNan(arg);
  return result;
}

/*!
  */
template <typename FloatN> inline
ComparisonResultType<FloatN> signbit(const FloatN arg) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point type.");
  const auto result = Relation::isSignBitSet(arg);
  return result;
}

/*!
  */
template <typename TypeN> inline
TypeN bitselect(const TypeN a, const TypeN b, const TypeN c) noexcept
{
  const auto result = Relation::selectBit(a, b, c);
  return result;
}

/*!
  */
template <typename TypeN, typename IntegerN> inline
TypeN select(const TypeN a, const TypeN b, const IntegerN c) noexcept
{
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const auto result = Relation::selectValue(a, b, c);
  return result;
}

} // namespace zinvul

#endif /* ZINVUL_RELATIONAL_INL_CL */
