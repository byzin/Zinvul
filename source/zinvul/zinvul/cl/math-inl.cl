/*!
  \file math-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_INL_CL
#define ZINVUL_MATH_INL_CL

#include "math.cl"
// Zinvul
#include "algorithm.cl"
#include "floating_point.cl"
#include "relational.cl"
#include "types.cl"
#include "type_traits.cl"
#include "utility.cl"
#include "zinvul_config.cl"

namespace zinvul {

/*!
  */
template <typename FloatN> inline
FloatN Math::ceil(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Builtin::ceil(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::floor(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Builtin::floor(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::trunc(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Builtin::trunc(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::round(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathRoundUsed()
    ? Builtin::round(x)
    : Zinvul::round(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::rint(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathRoundUsed()
    ? Builtin::rint(x)
    : Zinvul::rint(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::fmod(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto z = Config::isBuiltinMathFmodUsed()
    ? Builtin::fmod(x, y)
    : Zinvul::fmod(x, y);
  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::fma(const FloatN a, const FloatN b, const FloatN c) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Builtin::fma(a, b, c);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::exp(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathExpUsed()
    ? Builtin::exp(x)
    : Zinvul::exp(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::exp2(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathExpUsed()
    ? Builtin::exp2(x)
    : Zinvul::exp2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::log(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathLogUsed()
    ? Builtin::log(x)
    : Zinvul::log(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::log2(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathLogUsed()
    ? Builtin::log2(x)
    : Zinvul::log2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::pow(const FloatN base, const FloatN e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathPowUsed()
    ? Builtin::pow(base, e)
    : Zinvul::pow(base, e);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::rsqrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathSqrtUsed()
    ? Builtin::rsqrt(x)
    : Zinvul::rsqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::sqrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathSqrtUsed()
    ? Builtin::sqrt(x)
    : Zinvul::sqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::cbrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathCbrtUsed()
    ? Builtin::cbrt(x)
    : Zinvul::cbrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::sin(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathTrigonometricUsed()
    ? Builtin::sin(theta)
    : Zinvul::sin(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::cos(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathTrigonometricUsed()
    ? Builtin::cos(theta)
    : Zinvul::cos(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::tan(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathTrigonometricUsed()
    ? Builtin::tan(theta)
    : Zinvul::tan(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::asin(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathInvTrigonometricUsed()
    ? Builtin::asin(x)
    : Zinvul::asin(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::acos(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathInvTrigonometricUsed()
    ? Builtin::acos(x)
    : Zinvul::acos(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::atan(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathInvTrigonometricUsed()
    ? Builtin::atan(x)
    : Zinvul::atan(x);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerNPtr> inline
FloatN Math::frexp(const FloatN x, IntegerNPtr e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<IntegerNPtr>;
  using IntegerN = typename ASpaceInfo::DataType;
  static_assert(kIsInteger<IntegerN> & ASpaceInfo::isPointer(),
                "The IntegerNPtr isn't integer pointer type.");
  const auto y = Config::isBuiltinMathFrexpUsed()
    ? Builtin::frexp(x, e)
    : Zinvul::frexp(x, e);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::ldexp(const FloatN x, const IntegerN e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const auto y = Config::isBuiltinMathLdexpUsed()
    ? Builtin::ldexp(x, e)
    : Zinvul::ldexp(x, e);
  return y;
}

/*!
  */
template <typename FloatN, typename FloatNPtr> inline
FloatN Math::modf(const FloatN x, FloatNPtr iptr) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<FloatNPtr>;
  using FloatNType = typename ASpaceInfo::DataType;
  static_assert(kIsFloatingPoint<FloatNType> & ASpaceInfo::isPointer(),
                "The FloatNPtr isn't floating point.");
  static_assert(kIsSame<FloatN, FloatNType>, "The x and iptr types aren't same.");
  const auto y = Config::isBuiltinMathModfUsed()
    ? Builtin::modf(x, iptr)
    : Zinvul::modf(x, iptr);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::copysign(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto z = Builtin::copysign(x, y);
  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::ceil(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::ceil(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::floor(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::floor(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::trunc(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::trunc(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::round(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::round(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::rint(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  //! \todo Use rint instead of round
//  const auto y = ZINVUL_GLOBAL_NAMESPACE::rint(x);
  const auto y = ZINVUL_GLOBAL_NAMESPACE::round(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::fmod(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto z = ZINVUL_GLOBAL_NAMESPACE::fmod(x, y);
  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::fma(const FloatN a, const FloatN b, const FloatN c) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::fma(a, b, c);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::exp(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::exp(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::exp2(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::exp2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::log(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::log(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::log2(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::log2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::pow(const FloatN base, const FloatN e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::pow(base, e);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::rsqrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::rsqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::sqrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::sqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::cbrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(!Config::isBuiltinMathCbrtUsed(),
                "Builtin 'cbrt' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::cbrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::sin(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::sin(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::cos(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::cos(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::tan(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::tan(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::asin(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::asin(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::acos(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::acos(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::atan(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::atan(x);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerNPtr> inline
FloatN Math::Builtin::frexp(const FloatN x, IntegerNPtr e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<IntegerNPtr>;
  using IntegerN = typename ASpaceInfo::DataType;
  static_assert(kIsInteger<IntegerN> & ASpaceInfo::isPointer(),
                "The IntegerNPtr isn't integer pointer type.");
  static_assert(!Config::isBuiltinMathFrexpUsed(),
                "The builtin 'frexp' isn't implemented yet.");
//  const auto y = ZINVUL_GLOBAL_NAMESPACE::frexp(x, e);
  const auto y = x;
  (void)e;
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::Builtin::ldexp(const FloatN x, const IntegerN e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::ldexp(x, e);
  return y;
}

/*!
  */
template <typename FloatN, typename FloatNPtr> inline
FloatN Math::Builtin::modf(const FloatN x, FloatNPtr iptr) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<FloatNPtr>;
  using FloatNType = typename ASpaceInfo::DataType;
  static_assert(kIsFloatingPoint<FloatNType> & ASpaceInfo::isPointer(),
                "The FloatNPtr isn't floating point.");
  static_assert(kIsSame<FloatN, FloatNType>, "The x and iptr types aren't same.");
//  const auto y = ZINVUL_GLOBAL_NAMESPACE::modf(x, iptr);
  const auto y = x;
  (void)iptr;
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Builtin::copysign(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto z = ZINVUL_GLOBAL_NAMESPACE::copysign(x, y);
  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::round(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using FloatInfo = VectorTypeInfo<FloatN>;
  using Float = typename FloatInfo::ElementType;

  //! \todo Support double
  constexpr auto h = static_cast<Float>(0.5);
  auto y = x + h;
  auto fr = y - Math::trunc(y);
  {
    const auto zero = make<FloatN>(static_cast<Float>(0.0));
    const auto one = static_cast<Float>(1.0);
    auto flag = zinvul::islessequal(y, zero) & zinvul::isequal(fr, zero);
    y = zinvul::select(y, y - one, flag);

    flag = zinvul::isless(fr, zero);
    fr = zinvul::select(fr, fr + one, flag);

    const auto k = make<FloatN>(static_cast<Float>(0.4999999701976776123f));
    flag = zinvul::isequal(x, k);
    y = zinvul::select(y, zero, flag);
  }
  {
    const auto k = make<FloatN>(static_cast<Float>(1u << 23));
    const auto flag = zinvul::isinf(x) | zinvul::isgreaterequal(zinvul::abs(x), k);
    y = zinvul::select(zinvul::copysign(y - fr, x), x, flag);
  }
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::rint(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  //! \todo Use rint instead of round
  const auto y = Math::Zinvul::round(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::fmod(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto z = ZINVUL_GLOBAL_NAMESPACE::fmod(x, y);
  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::exp(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathExpUsed(),
                "Zinvul 'exp' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::exp(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::exp2(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathExpUsed(),
                "Zinvul 'exp2' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::exp2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::log(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathLogUsed(),
                "Zinvul 'log' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::log(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::log2(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathLogUsed(),
                "Zinvul 'log2' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::log2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::pow(const FloatN base, const FloatN e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathPowUsed(),
                "Zinvul 'pow' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::pow(base, e);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::rsqrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathSqrtUsed(),
                "Zinvul 'rsqrt' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::rsqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::sqrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathSqrtUsed(),
                "Zinvul 'sqrt' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::sqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::cbrt(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::cbrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::sin(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathTrigonometricUsed(),
                "Zinvul 'sin' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::sin(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::cos(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathTrigonometricUsed(),
                "Zinvul 'cos' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::cos(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::tan(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathTrigonometricUsed(),
                "Zinvul 'tan' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::tan(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::asin(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathTrigonometricUsed(),
                "Zinvul 'asin' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::asin(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::acos(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathTrigonometricUsed(),
                "Zinvul 'acos' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::acos(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::atan(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(Config::isBuiltinMathTrigonometricUsed(),
                "Zinvul 'atan' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::atan(x);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerNPtr> inline
FloatN Math::Zinvul::frexp(const FloatN x, IntegerNPtr e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<IntegerNPtr>;
  using IntegerN = typename ASpaceInfo::DataType;
  static_assert(kIsInteger<IntegerN> & ASpaceInfo::isPointer(),
                "The IntegerNPtr isn't integer pointer type.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using FloatInfo = FloatingPointFromBytes<sizeof(Float)>;
  using BitVec = UIntegerTypeFromVec<FloatN>;

  constexpr auto exp_mask = FloatInfo::exponentBitMask();
  const auto zero = make<FloatN>(static_cast<Float>(0.0));
  const auto is_special = zinvul::isequal(zero, x) |
                          zinvul::isinf(x) |
                          zinvul::isnan(x);

  auto data = treatAs<BitVec>(x);
  // Compute exponent value
  {
    constexpr auto sig_size = FloatInfo::significandBitSize();
    constexpr auto exp_bias = static_cast<int32b>(FloatInfo::exponentBias());
    auto expt = cast<IntegerN>((data & exp_mask) >> sig_size) - (exp_bias - 1);
    const auto z = make<IntegerN>(0);
    expt = zinvul::select(expt, z, is_special);
    *e = expt;
  }
  // Comppute significand value
  data = cast<BitVec>((data & ~exp_mask) | FloatInfo::halfBit());
  const auto y = zinvul::select(treatAs<FloatN>(data), x, is_special);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::Zinvul::ldexp(const FloatN x, const IntegerN e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");

  static_assert(Config::isBuiltinMathLdexpUsed(),
                "Zinvul 'ldexp' isn't implemented yet.");
  const auto y = ZINVUL_GLOBAL_NAMESPACE::ldexp(x, e);
  return y;
}

/*!
  */
template <typename FloatN, typename FloatNPtr> inline
FloatN Math::Zinvul::modf(const FloatN x, FloatNPtr iptr) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<FloatNPtr>;
  using FloatNType = typename ASpaceInfo::DataType;
  static_assert(kIsFloatingPoint<FloatNType> & ASpaceInfo::isPointer(),
                "The FloatNPtr isn't floating point.");
  static_assert(kIsSame<FloatN, FloatNType>, "The x and iptr types aren't same.");
  using FloatInfo = VectorTypeInfo<FloatN>;
  using Float = typename FloatInfo::ElementType;

  auto fr = x - zinvul::trunc(x);
  {
    const auto zero = make<FloatN>(static_cast<Float>(0.0));
    const auto k = make<FloatN>(static_cast<Float>(1u << 23));
    const auto flag = zinvul::isgreater(zinvul::abs(x), k);
    fr = zinvul::select(fr, zero, flag);
  }
  // Integer part
  {
    const auto i = zinvul::copysign(x - fr, x);
    *iptr = i;
  }
  // Fraction part
  const auto y = zinvul::copysign(fr, x);

  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN ceil(const FloatN x) noexcept
{
  const auto y = Math::ceil(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN floor(const FloatN x) noexcept
{
  const auto y = Math::floor(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN trunc(const FloatN x) noexcept
{
  const auto y = Math::trunc(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN round(const FloatN x) noexcept
{
  const auto y = Math::round(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN rint(const FloatN x) noexcept
{
  const auto y = Math::rint(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN fmod(const FloatN x, const FloatN y) noexcept
{
  const auto z = Math::fmod(x, y);
  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN fma(const FloatN a, const FloatN b, const FloatN c) noexcept
{
  const auto y = Math::fma(a, b, c);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN exp(const FloatN x) noexcept
{
  const auto y = Math::exp(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN exp2(const FloatN x) noexcept
{
  const auto y = Math::exp2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN log(const FloatN x) noexcept
{
  const auto y = Math::log(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN log2(const FloatN x) noexcept
{
  const auto y = Math::log2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN pow(const FloatN base, const FloatN e) noexcept
{
  const auto y = Math::pow(base, e);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN rsqrt(const FloatN x) noexcept
{
  const auto y = Math::rsqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN sqrt(const FloatN x) noexcept
{
  const auto y = Math::sqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN cbrt(const FloatN x) noexcept
{
  const auto y = Math::cbrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN sin(const FloatN theta) noexcept
{
  const auto y = Math::sin(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN cos(const FloatN theta) noexcept
{
  const auto y = Math::cos(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN tan(const FloatN theta) noexcept
{
  const auto y = Math::tan(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN asin(const FloatN x) noexcept
{
  const auto y = Math::asin(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN acos(const FloatN x) noexcept
{
  const auto y = Math::acos(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN atan(const FloatN x) noexcept
{
  const auto y = Math::atan(x);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN frexp(const FloatN x, IntegerN* e) noexcept
{
  const auto y = Math::frexp(x, e);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN ldexp(const FloatN x, const IntegerN e) noexcept
{
  const auto y = Math::ldexp(x, e);
  return y;
}

/*!
  */
template <typename FloatN, typename FloatNPtr> inline
FloatN modf(const FloatN x, FloatNPtr iptr) noexcept
{
  const auto y = Math::modf(x, iptr);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN copysign(const FloatN x, const FloatN y) noexcept
{
  const auto z = Math::copysign(x, y);
  return z;
}

} // namespace zinvul

#endif /* ZINVUL_MATH_INL_CL */
