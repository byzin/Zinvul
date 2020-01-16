/*!
  \file math-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_INL_CL
#define ZINVUL_MATH_INL_CL

#include "math.cl"
// Zinvul
#include "algorithm.cl"
#include "floating_point.cl"
#include "limits.cl"
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
template <typename FloatN> inline
auto Math::ilogb(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = Config::isBuiltinMathIlogbUsed()
    ? Builtin::ilogb(x)
    : Zinvul::ilogb(x);
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
//  const auto y = ZINVUL_GLOBAL_NAMESPACE::cbrt(x);
  const auto y = x;
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
template <typename FloatN> inline
auto Math::Builtin::ilogb(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;
//  const IntegerN y = ZINVUL_GLOBAL_NAMESPACE::ilogb(x);
  const IntegerN y = make<IntegerN>(0);
  (void)x;
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
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using FloatInfo = FloatingPointFromBytes<sizeof(Float)>;

  //! \todo Support double
  constexpr auto h = static_cast<Float>(0.5);
  auto y = x + h;
  auto fr = initFraction(y);
  {
    constexpr auto zero = static_cast<Float>(0.0);
    constexpr auto one = static_cast<Float>(1.0);
    const auto zerov = make<FloatN>(zero);
    auto flag = zinvul::islessequal(y, zerov) & zinvul::isequal(fr, zerov);
    y = zinvul::select(y, y - one, flag);

    flag = zinvul::isless(fr, zerov);
    fr = zinvul::select(fr, fr + one, flag);

    constexpr auto half_nextafter = getHalfNextafter<Float, 0>();
    const auto k = make<FloatN>(half_nextafter);
    flag = zinvul::isequal(x, k);
    y = zinvul::select(y, zerov, flag);
  }
  {
    constexpr size_t sig_size = FloatInfo::significandBitSize();
    constexpr auto sig_shift_f = static_cast<Float>(1ull << sig_size);
    const auto k = make<FloatN>(sig_shift_f);
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
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using FLimits = NumericLimits<Float>;

  const auto nu = zinvul::abs(x);
  const auto de = zinvul::abs(y);

  //! \todo subnormal case
  {
  }

  auto z = fmodImpl(nu, de);

  z = mulsign(z, x);
  auto flag = zinvul::isless(nu, de);
  z = zinvul::select(z, x, flag);

  constexpr auto zero = static_cast<Float>(0.0);
  flag = zinvul::isequal(de, make<FloatN>(zero));
  z = zinvul::select(z, make<FloatN>(FLimits::quietNan()), flag);

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
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using CmpType = ComparisonResultType<FloatN>;

  constexpr auto one = static_cast<Float>(1.0);
  auto q = make<FloatN>(one);
  const auto e = ilogbImpl(zinvul::abs(x)) + 1;
  {
    const auto r = (e + 6144) % 3;
    constexpr auto c0 = getCbrtCoeff<Float, 0>();
    auto flag = cast<CmpType>(r == 1);
    q = zinvul::select(q, make<FloatN>(c0), flag);

    constexpr auto c1 = getCbrtCoeff<Float, 1>();
    flag = cast<CmpType>(r == 2);
    q = zinvul::select(q, make<FloatN>(c1), flag);
  }
  q = ldexpImpl(q, ((e + 6144) / 3) - 2048);
  auto d = ldexpImpl(x, -e);
  q = mulsign(q, d);
  d = zinvul::abs(d);

  auto t = make<FloatN>(CbrtPolyConstants::template get<Float, 0>());
  t = zinvul::fma(t, d, make<FloatN>(CbrtPolyConstants::template get<Float, 1>()));
  t = zinvul::fma(t, d, make<FloatN>(CbrtPolyConstants::template get<Float, 2>()));
  t = zinvul::fma(t, d, make<FloatN>(CbrtPolyConstants::template get<Float, 3>()));
  t = zinvul::fma(t, d, make<FloatN>(CbrtPolyConstants::template get<Float, 4>()));
  t = zinvul::fma(t, d, make<FloatN>(CbrtPolyConstants::template get<Float, 5>()));

  constexpr auto zero = static_cast<Float>(0.0);
  auto y = make<FloatN>(zero);
  if constexpr (sizeof(Float) == 8) {
    y = t * t;
    y = y * y;
    constexpr auto k = static_cast<Float>(1.0) / static_cast<Float>(3.0);
    t = t - k * (d * y - t);
  }
  y = d * t * t;
  constexpr auto k = static_cast<Float>(2.0) / static_cast<Float>(3.0);
  y = (y - k * y * (y * t - one)) * q;

  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::sin(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = sinImpl(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::cos(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = cosImpl(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::tan(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = tanImpl(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::asin(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;

  constexpr Float h = static_cast<Float>(0.5);
  constexpr Float one = static_cast<Float>(1.0);
  constexpr Float two = static_cast<Float>(2.0);

  const auto flag = zinvul::isless(zinvul::abs(x), make<FloatN>(h));
  const auto t2 = zinvul::select(h * (one - zinvul::abs(x)), x * x, flag);
  const auto t = zinvul::select(zinvul::sqrt(t2), zinvul::abs(x), flag);

  auto y = asinImpl(t2);
  y = zinvul::fma(y, t * t2, t);

  constexpr auto half_pi = h * kPi<Float>;
  y = zinvul::select(half_pi - two * y, y, flag);
  y = mulsign(y, x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::acos(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;

  constexpr Float zero = static_cast<Float>(0.0);
  constexpr Float h = static_cast<Float>(0.5);
  constexpr Float one = static_cast<Float>(1.0);
  constexpr Float two = static_cast<Float>(2.0);

  auto flag = zinvul::isless(zinvul::abs(x), make<FloatN>(h));
  const auto t2 = zinvul::select(h * (one - zinvul::abs(x)), x * x, flag);
  auto t = zinvul::select(zinvul::sqrt(t2), zinvul::abs(x), flag);
  {
    const auto flag2 = zinvul::isequal(zinvul::abs(x), make<FloatN>(one));
    t = zinvul::select(t, make<FloatN>(zero), flag2);
  }

  auto y = acosImpl(t2);
  y = y * (t * t2);

  constexpr Float c0 = getAcosCoeff<Float, 0>();
  constexpr Float c1 = getAcosCoeff<Float, 1>();
  constexpr Float c2 = getAcosCoeff<Float, 2>();
  {
    const auto u = y;
    y = h * c0 - (mulsign(t, x) + mulsign(y, x));
    t = t + u;
  }
  y = zinvul::select(two * t, y, flag);
  {
    const F2<FloatN> a{make<FloatN>(c1), make<FloatN>(c2)};
    const auto r = addF2F<FloatN>(a, -y);
    flag = !flag && zinvul::isless(x, make<FloatN>(zero));
    y = zinvul::select(y, r.x_, flag);
  }

  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::atan(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  constexpr Float one = static_cast<Float>(1.0);

  auto s = zinvul::abs(x);
  auto flag = zinvul::isequal(zinvul::sign(x), make<FloatN>(-one));
  auto q = zinvul::select(make<IntegerN>(0), make<IntegerN>(2), flag);
  flag = zinvul::isgreater(s, make<FloatN>(one));
  s = zinvul::select(s, Algorithm::invert(s), flag);
  q = zinvul::select(q, q | 1, flag);

  auto t = s * s;
  auto y = atanImpl(t);
  t = s + s * (t * y);

  using FlagType = decltype(flag);
  constexpr Float c0 = getAtanCoeff<Float, 0>();
  flag = cast<FlagType>((q & 1) != 0);
  t = zinvul::select(t, c0 - t, flag);
  flag = cast<FlagType>((q & 2) != 0);
  y = zinvul::select(t, -t, flag);

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

  auto ret = make<IntegerN>(0);
  auto y = frexpImpl(x, &ret);

  constexpr auto zero = static_cast<Float>(0.0);
  const auto is_special_case = zinvul::isequal(x, make<FloatN>(zero)) |
                               zinvul::isinf(x) |
                               zinvul::isnan(x);
  const auto zerov = make<IntegerN>(0);
  ret = zinvul::select(ret, zerov, is_special_case);
  y = zinvul::select(y, x, is_special_case);
  *e = ret;
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::Zinvul::ldexp(const FloatN x, const IntegerN e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;

  auto y = ldexpImpl(x, e);
  // Special case
  constexpr auto zero = static_cast<Float>(0.0);
  const auto is_special_case = zinvul::isequal(x, make<FloatN>(zero)) |
                               zinvul::isinf(x) |
                               zinvul::isnan(x);
  y = zinvul::select(y, x, is_special_case);
  return y;
}

/*!
  */
template <typename FloatN> inline
auto Math::Zinvul::ilogb(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  auto y = ilogbImpl(zinvul::abs(x));
  // Special case
  // Zero case
  {
    constexpr auto zero = static_cast<Float>(0.0);
    const auto zerov = make<FloatN>(zero);
    const auto is_zero = zinvul::isequal(x, zerov);
    constexpr auto ilogb0 = getIlogb0<Float>();
    const auto ilogb0v = make<IntegerN>(ilogb0);
    y = zinvul::select(y, ilogb0v, is_zero);
  }
  // Nan case
  {
    const auto is_nan = zinvul::isnan(x);
    constexpr auto ilogbnan = getIlogbNan<Float>();
    const auto ilogbnanv = make<IntegerN>(ilogbnan);
    y = zinvul::select(y, ilogbnanv, is_nan);
  }
  // Infinity case
  {
    const auto is_inf = zinvul::isinf(x);
    constexpr auto imax = NumericLimits<int32b>::max();
    const auto imaxv = make<IntegerN>(imax);
    y = zinvul::select(y, imaxv, is_inf);
  }
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
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using FloatInfo = FloatingPointFromBytes<sizeof(Float)>;

  auto fr = initFraction(x);
  {
    constexpr size_t sig_size = FloatInfo::significandBitSize();
    constexpr auto sig_shift_f = static_cast<Float>(1ull << sig_size);
    const auto k = make<FloatN>(sig_shift_f);
    const auto flag = zinvul::isgreater(zinvul::abs(x), k);

    constexpr auto zero = static_cast<Float>(0.0);
    const auto zerov = make<FloatN>(zero);
    fr = zinvul::select(fr, zerov, flag);
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
template <typename Float, size_t kIndex> inline
constexpr Float Math::Zinvul::CbrtPolyConstants::get() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  Float c = static_cast<Float>(0.0);
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0)
      c = -0.601564466953277587890625f;
    else if constexpr (kIndex == 1)
      c = 2.8208892345428466796875f;
    else if constexpr (kIndex == 2)
      c = -5.532182216644287109375f;
    else if constexpr (kIndex == 3)
      c = 5.898262500762939453125f;
    else if constexpr (kIndex == 4)
      c = -3.8095417022705078125f;
    else if constexpr (kIndex == 5)
      c = 2.2241256237030029296875f;
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0)
      c = -0.640245898480692909870982;
    else if constexpr (kIndex == 1)
      c = 2.96155103020039511818595;
    else if constexpr (kIndex == 2)
      c = -5.73353060922947843636166;
    else if constexpr (kIndex == 3)
      c = 6.03990368989458747961407;
    else if constexpr (kIndex == 4)
      c = -3.85841935510444988821632;
    else if constexpr (kIndex == 5)
      c = 2.2307275302496609725722;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
  }
  return c;
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr Float Math::Zinvul::SinCosPolyConstants::get() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  Float c = static_cast<Float>(0.0);
  if constexpr (sizeof(Float) == 4) {
    c = 0.0f;
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0)
      c = 0.00833333333333332974823815;
    else if constexpr (kIndex == 1)
      c = -0.000198412698412696162806809;
    else if constexpr (kIndex == 2)
      c = 2.75573192239198747630416e-06;
    else if constexpr (kIndex == 3)
      c = -2.50521083763502045810755e-08;
    else if constexpr (kIndex == 4)
      c = 1.60590430605664501629054e-10;
    else if constexpr (kIndex == 5)
      c = -7.64712219118158833288484e-13;
    else if constexpr (kIndex == 6)
      c = 2.81009972710863200091251e-15;
    else if constexpr (kIndex == 7)
      c = -7.97255955009037868891952e-18;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
  }
  return c;
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr Float Math::Zinvul::TanPolyConstants::get() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  Float c = static_cast<Float>(0.0);
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0)
      c = 0.333331853151321411132812f;
    else if constexpr (kIndex == 1)
      c = 0.133383005857467651367188f;
    else if constexpr (kIndex == 2)
      c = 0.0534495301544666290283203f;
    else if constexpr (kIndex == 3)
      c = 0.0242998078465461730957031f;
    else if constexpr (kIndex == 4)
      c = 0.00331984995864331722259521f;
    else if constexpr (kIndex == 5)
      c = 0.00927245803177356719970703f;
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0)
      c = +0.1333333333330500581e+0;
    else if constexpr (kIndex == 1)
      c = +0.5396825399517272970e-1;
    else if constexpr (kIndex == 2)
      c = +0.2186948728185535498e-1;
    else if constexpr (kIndex == 3)
      c = +0.8863268409563113126e-2;
    else if constexpr (kIndex == 4)
      c = +0.3591611540792499519e-2;
    else if constexpr (kIndex == 5)
      c = +0.1460781502402784494e-2;
    else if constexpr (kIndex == 6)
      c = +0.5619219738114323735e-3;
    else if constexpr (kIndex == 7)
      c = +0.3245098826639276316e-3;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
  }
  return c;
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr Float Math::Zinvul::AsinPolyConstants::get() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  Float c = static_cast<Float>(0.0);
  if constexpr (sizeof(Float) == 4) {
    c = 0.0f;
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0)
      c = 0.1666666666666497543e+0;
    else if constexpr (kIndex == 1)
      c = 0.7500000000378581611e-1;
    else if constexpr (kIndex == 2)
      c = 0.4464285681377102438e-1;
    else if constexpr (kIndex == 3)
      c = 0.3038195928038132237e-1;
    else if constexpr (kIndex == 4)
      c = 0.2237176181932048341e-1;
    else if constexpr (kIndex == 5)
      c = 0.1735956991223614604e-1;
    else if constexpr (kIndex == 6)
      c = 0.1388715184501609218e-1;
    else if constexpr (kIndex == 7)
      c = 0.1215360525577377331e-1;
    else if constexpr (kIndex == 8)
      c = 0.6606077476277170610e-2;
    else if constexpr (kIndex == 9)
      c = 0.1929045477267910674e-1;
    else if constexpr (kIndex == 10)
      c = -0.1581918243329996643e-1;
    else if constexpr (kIndex == 11)
      c = 0.3161587650653934628e-1;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
  }
  return c;
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr Float Math::Zinvul::AtanPolyConstants::get() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  Float c = static_cast<Float>(0.0);
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0)
      c = -0.333331018686294555664062f;
    else if constexpr (kIndex == 1)
      c = 0.199926957488059997558594f;
    else if constexpr (kIndex == 2)
      c = -0.142027363181114196777344f;
    else if constexpr (kIndex == 3)
      c = 0.106347933411598205566406f;
    else if constexpr (kIndex == 4)
      c = -0.0748900920152664184570312f;
    else if constexpr (kIndex == 5)
      c = 0.0425049886107444763183594f;
    else if constexpr (kIndex == 6)
      c = -0.0159569028764963150024414f;
    else if constexpr (kIndex == 7)
      c = 0.00282363896258175373077393f;
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0)
      c = -0.333333333333311110369124;
    else if constexpr (kIndex == 1)
      c = 0.199999999996591265594148;
    else if constexpr (kIndex == 2)
      c = -0.14285714266771329383765;
    else if constexpr (kIndex == 3)
      c = 0.111111105648261418443745;
    else if constexpr (kIndex == 4)
      c = -0.090908995008245008229153;
    else if constexpr (kIndex == 5)
      c = 0.0769219538311769618355029;
    else if constexpr (kIndex == 6)
      c = -0.0666573579361080525984562;
    else if constexpr (kIndex == 7)
      c = 0.0587666392926673580854313;
    else if constexpr (kIndex == 8)
      c = -0.0523674852303482457616113;
    else if constexpr (kIndex == 9)
      c = 0.0466667150077840625632675;
    else if constexpr (kIndex == 10)
      c = -0.0407629191276836500001934;
    else if constexpr (kIndex == 11)
      c = 0.0337852580001353069993897;
    else if constexpr (kIndex == 12)
      c = -0.0254517624932312641616861;
    else if constexpr (kIndex == 13)
      c = 0.016599329773529201970117;
    else if constexpr (kIndex == 14)
      c = -0.00889896195887655491740809;
    else if constexpr (kIndex == 15)
      c = 0.00370026744188713119232403;
    else if constexpr (kIndex == 16)
      c = -0.00110611831486672482563471;
    else if constexpr (kIndex == 17)
      c = 0.000209850076645816976906797;
    else if constexpr (kIndex == 18)
      c = -1.88796008463073496563746e-05;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
  }
  return c;
}

template <typename FloatN> inline
FloatN Math::Zinvul::upper(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using BitVec = UIntegerTypeFromVec<FloatN>;

  constexpr auto mask = getUpperCoeff<Float, 0>();
  const auto u = treatAs<BitVec>(x);
  const auto result = treatAs<FloatN>(u & mask);
  return result;
}

template <typename FloatN> inline
auto Math::Zinvul::addF2F(const F2<FloatN> lhs, const FloatN rhs) noexcept
    -> F2<FloatN>
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  auto result = lhs;
  result.x_ = lhs.x_ + rhs;
  result.y_ = lhs.x_ - result.x_ + rhs + lhs.y_;
  return result;
}

template <typename FloatN> inline
auto Math::Zinvul::add2F2F2(const F2<FloatN> lhs, const F2<FloatN> rhs) noexcept
    -> F2<FloatN>
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  auto result = lhs;
  result.x_ = lhs.x_ + rhs.x_;
  const auto v = result.x_ - lhs.x_;
  result.y_ = (lhs.x_ - (result.x_ - v)) + (rhs.x_ - v);
  result.y_ = result.y_ + (lhs.y_ + rhs.y_);
  return result;
}

template <typename FloatN> inline
auto Math::Zinvul::mulFF(const FloatN lhs, const FloatN rhs) noexcept -> F2<FloatN>
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto xh = upper(lhs);
  const auto yh = upper(rhs);
  const auto xl = lhs - xh;
  const auto yl = rhs - yh;

  F2<FloatN> result{lhs, rhs};
  result.x_ = lhs * rhs;
  result.y_ = xh * yh - result.x_ + xl * yh + xh * yl + xl * yl;
  return result;
}

template <typename FloatN> inline
auto Math::Zinvul::normalizeF2(const F2<FloatN> x) noexcept -> F2<FloatN>
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  auto result = x;
  result.x_ = x.x_ + x.y_;
  result.y_ = x.x_ - result.x_ + x.y_;
  return result;
}

template <typename FloatN> inline
FloatN Math::Zinvul::toward0(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using BitVec = UIntegerTypeFromVec<FloatN>;
  using BitType = typename VectorTypeInfo<BitVec>::ElementType;

  constexpr auto one = static_cast<BitType>(1u);
  const auto u = treatAs<BitVec>(x);
  auto result = treatAs<FloatN>(u - one);

  constexpr auto zero = static_cast<Float>(0.0);
  const auto flag = zinvul::isequal(x, make<FloatN>(zero));
  result = zinvul::select(result, x, flag);

  return result;
}

template <typename FloatN> inline
FloatN Math::Zinvul::removeLsb(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using FloatInfo = FloatingPointFromBytes<sizeof(Float)>;
  using BitVec = UIntegerTypeFromVec<FloatN>;

  constexpr auto mask = FloatInfo::exponentBitMask() |
                        FloatInfo::significandBitMask();
  const auto u = treatAs<BitVec>(x);
  const auto result = treatAs<FloatN>(u & mask);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::mulsign(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using FloatInfo = FloatingPointFromBytes<sizeof(Float)>;
  using BitType = typename FloatInfo::BitType;
  using BitVec = UIntegerTypeFromVec<FloatN>;

  constexpr size_t bits = 8 * sizeof(Float);
  constexpr BitType k = BitType{1u} << (bits - 1);
  const auto zi = treatAs<BitVec>(x) ^ (treatAs<BitVec>(y) & k);
  const auto z = treatAs<FloatN>(zi);
  return z;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::Zinvul::pow2i(const IntegerN q) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using Integer = typename VectorTypeInfo<IntegerN>::ElementType;
  using FloatInfo = FloatingPointFromBytes<sizeof(Float)>;
  constexpr auto exp_bias = static_cast<Integer>(FloatInfo::exponentBias());
  constexpr auto sig_size = static_cast<Integer>(FloatInfo::significandBitSize());
  const IntegerN result = (q + exp_bias) << sig_size;;
  return treatAs<FloatN>(result);
}

/*!
  */
template <typename Constants, size_t kIndex, typename FloatN> inline
FloatN Math::Zinvul::evalPoly2(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  constexpr auto c0 = Constants::template get<Float, kIndex + 0>();
  constexpr auto c1 = Constants::template get<Float, kIndex + 1>();
  const auto a = make<FloatN>(c1);
  const auto b = make<FloatN>(c0);
  const auto y = zinvul::fma(x, a, b);
  return y;
}

/*!
  */
template <typename Constants, size_t kIndex, typename FloatN> inline
FloatN Math::Zinvul::evalPoly3(const FloatN x1,
                               const FloatN x2) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  constexpr auto c2 = Constants::template get<Float, kIndex + 2>();
  const auto a = make<FloatN>(c2);
  const auto b = evalPoly2<Constants, kIndex + 0>(x1);
  const auto y = zinvul::fma(x2, a, b);
  return y;
}

/*!
  */
template <typename Constants, size_t kIndex, typename FloatN> inline
FloatN Math::Zinvul::evalPoly4(const FloatN x1,
                               const FloatN x2) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto a = evalPoly2<Constants, kIndex + 2>(x1);
  const auto b = evalPoly2<Constants, kIndex + 0>(x1);
  const auto y = zinvul::fma(x2, a, b);
  return y;
}

/*!
  */
template <typename Constants, size_t kIndex, typename FloatN> inline
FloatN Math::Zinvul::evalPoly6(const FloatN x1,
                               const FloatN x2,
                               const FloatN x3) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto a = evalPoly2<Constants, kIndex + 4>(x1);
  const auto b = evalPoly4<Constants, kIndex + 0>(x1, x2);
  const auto y = zinvul::fma(x3, a, b);
  return y;
}

/*!
  */
template <typename Constants, size_t kIndex, typename FloatN> inline
FloatN Math::Zinvul::evalPoly8(const FloatN x1,
                               const FloatN x2,
                               const FloatN x3) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto a = evalPoly4<Constants, kIndex + 4>(x1, x2);
  const auto b = evalPoly4<Constants, kIndex + 0>(x1, x2);
  const auto y = zinvul::fma(x3, a, b);
  return y;
}

/*!
  */
template <typename Constants, size_t kIndex, typename FloatN> inline
FloatN Math::Zinvul::evalPoly12(const FloatN x1,
                                const FloatN x2,
                                const FloatN x3,
                                const FloatN x4) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto a = evalPoly4<Constants, kIndex + 8>(x1, x2);
  const auto b = evalPoly8<Constants, kIndex + 0>(x1, x2, x3);
  const auto y = zinvul::fma(x4, a, b);
  return y;
}

/*!
  */
template <typename Constants, size_t kIndex, typename FloatN> inline
FloatN Math::Zinvul::evalPoly16(const FloatN x1,
                                const FloatN x2,
                                const FloatN x3,
                                const FloatN x4) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto a = evalPoly8<Constants, kIndex + 8>(x1, x2, x3);
  const auto b = evalPoly8<Constants, kIndex + 0>(x1, x2, x3);
  const auto y = zinvul::fma(x4, a, b);
  return y;
}

/*!
  */
template <typename Constants, size_t kIndex, typename FloatN> inline
FloatN Math::Zinvul::evalPoly19(const FloatN x1,
                                const FloatN x2,
                                const FloatN x3,
                                const FloatN x4,
                                const FloatN x5) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto a = evalPoly3<Constants, kIndex + 16>(x1, x2);
  const auto b = evalPoly16<Constants, kIndex + 0>(x1, x2, x3, x4);
  const auto y = zinvul::fma(x5, a, b);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerNPtr> inline
FloatN Math::Zinvul::frexpImpl(const FloatN x, IntegerNPtr e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using ASpaceInfo = AddressSpaceInfo<IntegerNPtr>;
  using IntegerN = typename ASpaceInfo::DataType;
  static_assert(kIsInteger<IntegerN> & ASpaceInfo::isPointer(),
                "The IntegerNPtr isn't integer pointer type.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using FloatInfo = FloatingPointFromBytes<sizeof(Float)>;
  using BitVec = UIntegerTypeFromVec<FloatN>;

  constexpr auto zero = static_cast<Float>(0.0);
  auto y = make<FloatN>(zero);;
  {
    constexpr auto minval = NumericLimits<Float>::min();
    const auto minvalv = make<FloatN>(minval);
    const auto flag = zinvul::isless(zinvul::abs(x), minvalv);

    constexpr auto c0 = getFrexpCoeff<Float>();
    constexpr auto f1 = static_cast<Float>(0b1ull << c0);
    y = zinvul::select(x, f1 * x, flag);
    *e = zinvul::select(*e, *e - c0, flag);
  }

  constexpr auto exp_mask = FloatInfo::exponentBitMask();
  auto data = treatAs<BitVec>(y);
  // Compute exponent value
  {
    constexpr auto sig_size = FloatInfo::significandBitSize();
    constexpr auto exp_bias = static_cast<int32b>(FloatInfo::exponentBias());
    *e = *e + cast<IntegerN>((data & exp_mask) >> sig_size) - (exp_bias - 1);
  }
  // Comppute significand value
  data = cast<BitVec>((data & ~exp_mask) | FloatInfo::halfBit());
  y = treatAs<FloatN>(data);
  return y;
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr auto Math::Zinvul::getUpperCoeff() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0) {
      const uint32b c = 0xfffff000u;
      return c;
    }
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0) {
      const uint64b c = 0xfffffffff8000000ull;
      return c;
    }
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    const auto c = static_cast<Float>(0);
    return c;
  }
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr auto Math::Zinvul::getCbrtCoeff() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0) {
      const auto c = 1.2599210498948731647672106f;
      return c;
    }
    else if constexpr (kIndex == 1) {
      const auto c = 1.5874010519681994747517056f;
      return c;
    }
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0) {
      const auto c = 1.2599210498948731647672106;
      return c;
    }
    else if constexpr (kIndex == 1) {
      const auto c = 1.5874010519681994747517056;
      return c;
    }
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    const auto c = static_cast<Float>(0);
    return c;
  }
}

/*!
  */
template <typename Float> inline
constexpr int32b Math::Zinvul::getFrexpCoeff() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  if constexpr (sizeof(Float) == 4) {
    const int32b c = 30;
    return c;
  }
  else if constexpr (sizeof(Float) == 8) {
    const int32b c = 63;
    return c;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    const int32b c = 0;
    return c;
  }
}

/*!
  */
template <typename Float, int32b kTo> inline
constexpr Float Math::Zinvul::getHalfNextafter() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  if constexpr (sizeof(Float) == 4) {
    const auto y = (kTo < 1)
        ? 0.4999999701976776123f
        : 0.50000005960464477539f;
    return y;
  }
  else if constexpr (sizeof(Float) == 8) {
    const auto y = (kTo < 1)
        ? 0.49999999999999994449
        : 0.50000000000000011102;
    return y;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    return static_cast<Float>(0);
  }
}

/*!
  */
template <typename Float> inline
constexpr int32b Math::Zinvul::getIlogb0() noexcept
{
  const int32b i = NumericLimits<int32b>::min();
  return i;
}

/*!
  */
template <typename Float> inline
constexpr int32b Math::Zinvul::getIlogbNan() noexcept
{
  const int32b i = NumericLimits<int32b>::max();
  return i;
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr auto Math::Zinvul::getIlogbCoeff() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0) {
      const auto c = 5.421010862427522E-20f;
      return c;
    }
    else if constexpr (kIndex == 1) {
      const auto c = 1.8446744073709552E19f;
      return c;
    }
    else if constexpr (kIndex == 2) {
      const int32b c = 64;
      return c;
    }
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0) {
      const auto c = 4.9090934652977266E-91;
      return c;
    }
    else if constexpr (kIndex == 1) {
      const auto c = 2.037035976334486E90;
      return c;
    }
    else if constexpr (kIndex == 2) {
      const int32b c = 300;
      return c;
    }
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    const auto c = static_cast<Float>(0);
    return c;
  }
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr Float Math::Zinvul::getPi() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  Float c = static_cast<Float>(0.0);
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0)
      c = 3.140625f;
    else if constexpr (kIndex == 1)
      c = 0.0009670257568359375f;
    else if constexpr (kIndex == 2)
      c = 6.2771141529083251953e-07f;
    else if constexpr (kIndex == 3)
      c = 1.2154201256553420762e-10f;
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0)
      c = 3.1415926218032836914;
    else if constexpr (kIndex == 1)
      c = 3.1786509424591713469e-08;
    else if constexpr (kIndex == 2)
      c = 1.2246467864107188502e-16;
    else if constexpr (kIndex == 3)
      c = 1.2736634327021899816e-24;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
  }
  return c;
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr Float Math::Zinvul::getPi2() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  Float c = static_cast<Float>(0.0);
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0)
      c = 3.1414794921875f;
    else if constexpr (kIndex == 1)
      c = 0.00011315941810607910156f;
    else if constexpr (kIndex == 2)
      c = 1.9841872589410058936e-09f;
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0)
      c = 3.141592653589793116;
    else if constexpr (kIndex == 1)
      c = 1.2246467991473532072e-16;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
  }
  return c;
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr Float Math::Zinvul::getTrigRangeMax() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0) {
      const auto c = 39000.0f;
      return c;
    }
    else if constexpr (kIndex == 1) {
      const auto c = 125.0f;
      return c;
    }
    else if constexpr (kIndex == 2) {
      const auto c = 0.0f;
      return c;
    }
    else if constexpr (kIndex == 3) {
      const auto c = 8e+6f;
      return c;
    }
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0) {
      const auto c = 1e+14;
      return c;
    }
    else if constexpr (kIndex == 1) {
      const auto c = 15.0;
      return c;
    }
    else if constexpr (kIndex == 2) {
      const auto c = 1e+9;
      return c;
    }
    else if constexpr (kIndex == 2) {
      const auto c = 0.0;
      return c;
    }
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    const auto c = static_cast<Float>(0);
    return c;
  }
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr auto Math::Zinvul::getAcosCoeff() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0) {
      const auto c = 3.1415926535897932f;
      return c;
    }
    else if constexpr (kIndex == 1) {
      const auto c = 3.1415927410125732422f;
      return c;
    }
    else if constexpr (kIndex == 2) {
      const auto c = -8.7422776573475857731e-08f;
      return c;
    }
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0) {
      const auto c = 3.1415926535897932;
      return c;
    }
    else if constexpr (kIndex == 1) {
      const auto c = 3.141592653589793116;
      return c;
    }
    else if constexpr (kIndex == 2) {
      const auto c = 1.2246467991473532072e-16;
      return c;
    }
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    const auto c = static_cast<Float>(0);
    return c;
  }
}

/*!
  */
template <typename Float, size_t kIndex> inline
constexpr auto Math::Zinvul::getAtanCoeff() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float isn't floating point.");
  if constexpr (sizeof(Float) == 4) {
    if constexpr (kIndex == 0) {
      const auto c = 1.570796326794896557998982f;
      return c;
    }
  }
  else if constexpr (sizeof(Float) == 8) {
    if constexpr (kIndex == 0) {
      const auto c = 1.570796326794896557998982;
      return c;
    }
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    const auto c = static_cast<Float>(0);
    return c;
  }
}

/*!
  */
template <typename FloatN> inline
auto Math::Zinvul::ilogbImpl(FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using FloatInfo = FloatingPointFromBytes<sizeof(Float)>;
  using BitVec = UIntegerTypeFromVec<FloatN>;
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  constexpr auto c0 = getIlogbCoeff<Float, 0>();
  constexpr auto c1 = getIlogbCoeff<Float, 1>();
  const auto k1 = make<FloatN>(c0);
  const auto flag = zinvul::isless(x, k1);
  x = zinvul::select(x, c1 * x, flag);

  constexpr auto exp_mask = FloatInfo::exponentBitMask();
  constexpr size_t sig_size = FloatInfo::significandBitSize();
  auto y = cast<IntegerN>((treatAs<BitVec>(x) & exp_mask) >> sig_size);

  constexpr int32b offset = static_cast<int32b>(exp_mask >> (sig_size + 1));
  constexpr int32b c2 = getIlogbCoeff<Float, 2>();
  y = zinvul::select(y - offset, y - (c2 + offset), flag);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::initFraction(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  if constexpr (sizeof(Float) == 4) {
    const auto fr = x - zinvul::trunc(x);
    return fr;
  }
  else if constexpr (sizeof(Float) == 8) {
    constexpr Float u32_msb_f = static_cast<Float>(0b1u << 31);
    constexpr Float inv_u32_msb_f = 1.0 / u32_msb_f;
    auto fr = x - make<FloatN>(u32_msb_f) * zinvul::trunc(x * inv_u32_msb_f);
    fr = fr - zinvul::trunc(fr);
    return fr;
  }
  else {
    static_assert(sizeof(Float) == 0, "Unsupported floating point is specified.");
    const auto fr = x;
    return fr;
  }
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::Zinvul::ldexpImpl(const FloatN x, const IntegerN e) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert(kIsInteger<IntegerN>, "The IntegerN isn't integer type.");
  using IntN = IntegerTypeFromVec<FloatN>;
  const auto y = x *
                 pow2i<FloatN>(cast<IntN>(e >> 1)) *
                 pow2i<FloatN>(cast<IntN>(e - (e >> 1)));
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::rintImpl(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  constexpr auto h = static_cast<Float>(0.5);
  const auto flag = Algorithm::isNegative(x);
  auto y = zinvul::select(x + h, x - h, flag);
  y = zinvul::trunc(y);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::fmodImpl(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  if constexpr (sizeof(Float) == 4) {
    const auto z = fmodImplF(x, y);
    return z;
  }
  else if constexpr (sizeof(Float) == 8) {
    const auto z = fmodImplD(x, y);
    return z;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::fmodImplF(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using CmpType = ComparisonResultType<FloatN>;

  constexpr auto zero = static_cast<Float>(0.0);
  constexpr auto one = static_cast<Float>(1.0);
  constexpr auto two = static_cast<Float>(2.0);
  constexpr auto three = static_cast<Float>(3.0);

  F2<FloatN> r{x, make<FloatN>(zero)};
  FloatN q = y;
  const auto rde = toward0(Algorithm::invert(y));
  for (int i = 0; i < 8; ++i) {
    q = zinvul::trunc(toward0(r.x_) * rde);

    auto flag = cast<CmpType>(zinvul::isgreater(three * y, r.x_) &&
                              zinvul::isgreaterequal(r.x_, y));
    q = zinvul::select(q, make<FloatN>(two), flag);

    flag = cast<CmpType>(zinvul::isgreater(two * y, r.x_) &&
                         zinvul::isgreaterequal(r.x_, y));
    q = zinvul::select(q, make<FloatN>(one), flag);

    const auto tmp = normalizeF2(add2F2F2(r, mulFF(q, -y)));
    const auto isnot_first = (0 < i) ? kResultTrue<FloatN> : kResultFalse;
    flag = cast<CmpType>(make<CmpType>(isnot_first) && zinvul::isless(r.x_, y));
    r.x_ = zinvul::select(tmp.x_, r.x_, flag);
    r.y_ = zinvul::select(tmp.y_, r.y_, flag);
  }
  auto z = r.x_ + r.y_;
  const auto flag = zinvul::isequal(z, y);
  z = zinvul::select(z, make<FloatN>(zero), flag);

  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::fmodImplD(const FloatN x, const FloatN y) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  using CmpType = ComparisonResultType<FloatN>;

  constexpr auto zero = static_cast<Float>(0.0);
  constexpr auto one = static_cast<Float>(1.0);
  constexpr auto two = static_cast<Float>(2.0);
  constexpr auto three = static_cast<Float>(3.0);

  F2<FloatN> r{x, make<FloatN>(zero)};
  FloatN q = y;
  const auto rde = toward0(Algorithm::invert(y));
  for (int i = 0; i < 21; ++i) {
    q = removeLsb(zinvul::trunc(toward0(r.x_) * rde));

    auto flag = cast<CmpType>(zinvul::isgreater(three * y, r.x_) &&
                              zinvul::isgreaterequal(r.x_, y));
    q = zinvul::select(q, make<FloatN>(two), flag);

    flag = cast<CmpType>(zinvul::isgreater(two * y, r.x_) &&
                         zinvul::isgreaterequal(r.x_, y));
    q = zinvul::select(q, make<FloatN>(one), flag);

    flag = Algorithm::isNegative(r.y_);
    const auto t = zinvul::select(make<FloatN>(one), make<FloatN>(zero), flag);
    flag = zinvul::isequal(r.x_, y);
    q = zinvul::select(q, t, flag);

    const auto tmp = normalizeF2(add2F2F2(r, mulFF(q, -y)));
    const auto isnot_first = (0 < i) ? kResultTrue<FloatN> : kResultFalse;
    flag = cast<CmpType>(make<CmpType>(isnot_first) && zinvul::isless(r.x_, y));
    r.x_ = zinvul::select(tmp.x_, r.x_, flag);
    r.y_ = zinvul::select(tmp.y_, r.y_, flag);
  }
  auto z = r.x_;
  const auto flag = zinvul::isequal(r.x_ + r.y_, y);
  z = zinvul::select(z, make<FloatN>(zero), flag);

  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::sinImpl(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  if constexpr (sizeof(Float) == 4) {
    const auto y = sinImplF(theta);
    return y;
  }
  else if constexpr (sizeof(Float) == 8) {
    const auto y = sinImplD(theta);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::sinImplF(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  auto x = theta;
  IntegerN q = make<IntegerN>(0);
  {
    constexpr auto m_1_pi = static_cast<float>(1.0 / kPi<double>);
    const auto qf = rintImpl(theta * m_1_pi);

    constexpr auto pi_a = -getPi<float, 0>();
    constexpr auto pi_b = -getPi<float, 1>();
    constexpr auto pi_c = -getPi<float, 2>();
    constexpr auto pi_d = -getPi<float, 3>();
    x = zinvul::fma(qf, make<FloatN>(pi_a), x);
    x = zinvul::fma(qf, make<FloatN>(pi_b), x);
    x = zinvul::fma(qf, make<FloatN>(pi_c), x);
    x = zinvul::fma(qf, make<FloatN>(pi_d), x);

    q = cast<IntegerN>(qf);
  }
  const auto s = x * x;
  const auto is_odd = Algorithm::isOdd(q);
  x = zinvul::select(x, -x, is_odd);
  constexpr float c0 = 2.6083159809786593541503e-06f;
  constexpr float c1 = -0.0001981069071916863322258f;
  constexpr float c2 = 0.00833307858556509017944336f;
  constexpr float c3 = -0.166666597127914428710938f;
  auto y = zinvul::fma(make<FloatN>(c0), s, make<FloatN>(c1));
  y = zinvul::fma(y, s, make<FloatN>(c2));
  y = zinvul::fma(y, s, make<FloatN>(c3));
  y = zinvul::fma(s, y * x, x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::sinImplD(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  auto x = theta;
  IntegerN q = make<IntegerN>(0);
  {
    constexpr auto m_1_pi = static_cast<double>(1.0 / kPi<double>);
    constexpr auto m_24 = static_cast<double>(1 << 24);
    const auto dqh = zinvul::trunc(theta * (m_1_pi / m_24)) * m_24;
    const auto qf = rintImpl(theta * m_1_pi - dqh);

    constexpr auto pi_a = -getPi<double, 0>();
    constexpr auto pi_b = -getPi<double, 1>();
    constexpr auto pi_c = -getPi<double, 2>();
    constexpr auto pi_d = -getPi<double, 3>();
    x = zinvul::fma(dqh, make<FloatN>(pi_a), x);
    x = zinvul::fma(qf, make<FloatN>(pi_a), x);
    x = zinvul::fma(dqh, make<FloatN>(pi_b), x);
    x = zinvul::fma(qf, make<FloatN>(pi_b), x);
    x = zinvul::fma(dqh, make<FloatN>(pi_c), x);
    x = zinvul::fma(qf, make<FloatN>(pi_c), x);
    x = zinvul::fma(dqh + qf, make<FloatN>(pi_d), x);

    q = cast<IntegerN>(qf);
  }
  const auto s = x * x;
  const auto is_odd = Algorithm::isOdd(q);
  x = zinvul::select(x, -x, is_odd);
  auto y = s * s;
  {
    const auto s4 = y * y;
    y = evalPoly8<SinCosPolyConstants, 0>(s, y, s4);
  }
  y = zinvul::fma(y, s, make<FloatN>(-0.166666666666666657414808));
  y = zinvul::fma(s, y * x, x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::cosImpl(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  if constexpr (sizeof(Float) == 4) {
    const auto y = cosImplF(theta);
    return y;
  }
  else if constexpr (sizeof(Float) == 8) {
    const auto y = cosImplD(theta);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::cosImplF(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  auto x = theta;
  IntegerN q = make<IntegerN>(0);
  {
    constexpr auto m_1_pi = static_cast<float>(1.0 / kPi<double>);
    const auto qf = 1.0f + 2.0f * rintImpl(theta * m_1_pi - 0.5f);

    constexpr auto pi_a = -getPi<float, 0>() * 0.5f;
    constexpr auto pi_b = -getPi<float, 1>() * 0.5f;
    constexpr auto pi_c = -getPi<float, 2>() * 0.5f;
    constexpr auto pi_d = -getPi<float, 3>() * 0.5f;
    x = zinvul::fma(qf, make<FloatN>(pi_a), x);
    x = zinvul::fma(qf, make<FloatN>(pi_b), x);
    x = zinvul::fma(qf, make<FloatN>(pi_c), x);
    x = zinvul::fma(qf, make<FloatN>(pi_d), x);

    q = cast<IntegerN>(qf);
  }
  const auto s = x * x;
  const auto is_flag = cast<IntegerN>((q & 2) == 0);
  x = zinvul::select(x, -x, is_flag);
  constexpr float c0 = 2.6083159809786593541503e-06f;
  constexpr float c1 = -0.0001981069071916863322258f;
  constexpr float c2 = 0.00833307858556509017944336f;
  constexpr float c3 = -0.166666597127914428710938f;
  auto y = zinvul::fma(make<FloatN>(c0), s, make<FloatN>(c1));
  y = zinvul::fma(y, s, make<FloatN>(c2));
  y = zinvul::fma(y, s, make<FloatN>(c3));
  y = zinvul::fma(s, y * x, x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::cosImplD(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  auto x = theta;
  IntegerN q = make<IntegerN>(0);
  {
    constexpr auto m_1_pi = static_cast<double>(1.0 / kPi<double>);
    constexpr auto m_23 = static_cast<double>(1 << 23);
    constexpr auto m_24 = static_cast<double>(1 << 24);
    auto dqh = zinvul::trunc(theta * (m_1_pi / m_23) - 0.5 * (m_1_pi / m_23));
    const auto qf = 2.0 * rintImpl(theta * m_1_pi - 0.5 - dqh * m_23) + 1.0;
    dqh = dqh * m_24;

    constexpr auto pi_a = -getPi<double, 0>() * 0.5;
    constexpr auto pi_b = -getPi<double, 1>() * 0.5;
    constexpr auto pi_c = -getPi<double, 2>() * 0.5;
    constexpr auto pi_d = -getPi<double, 3>() * 0.5;
    x = zinvul::fma(dqh, make<FloatN>(pi_a), x);
    x = zinvul::fma(qf, make<FloatN>(pi_a), x);
    x = zinvul::fma(dqh, make<FloatN>(pi_b), x);
    x = zinvul::fma(qf, make<FloatN>(pi_b), x);
    x = zinvul::fma(dqh, make<FloatN>(pi_c), x);
    x = zinvul::fma(qf, make<FloatN>(pi_c), x);
    x = zinvul::fma(dqh + qf, make<FloatN>(pi_d), x);

    q = cast<IntegerN>(qf);
  }
  const auto s = x * x;
  const auto is_flag = cast<IntegerN>((q & 2) == 0);
  x = zinvul::select(x, -x, is_flag);
  auto y = s * s;
  {
    const auto s4 = y * y;
    y = evalPoly8<SinCosPolyConstants, 0>(s, y, s4);
  }
  y = zinvul::fma(y, s, make<FloatN>(-0.166666666666666657414808));
  y = zinvul::fma(s, y * x, x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::tanImpl(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  if constexpr (sizeof(Float) == 4) {
    const auto y = tanImplF(theta);
    return y;
  }
  else if constexpr (sizeof(Float) == 8) {
    const auto y = tanImplD(theta);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::tanImplF(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  auto x = theta;
  IntegerN q = make<IntegerN>(0);
  {
    constexpr auto m_2_pi = static_cast<float>(2.0 / kPi<double>);
    const auto qf = rintImpl(theta * m_2_pi);

    constexpr auto pi_a = -getPi<float, 0>() * 0.5f;
    constexpr auto pi_b = -getPi<float, 1>() * 0.5f;
    constexpr auto pi_c = -getPi<float, 2>() * 0.5f;
    constexpr auto pi_d = -getPi<float, 3>() * 0.5f;
    x = zinvul::fma(qf, make<FloatN>(pi_a), x);
    x = zinvul::fma(qf, make<FloatN>(pi_b), x);
    x = zinvul::fma(qf, make<FloatN>(pi_c), x);
    x = zinvul::fma(qf, make<FloatN>(pi_d), x);

    q = cast<IntegerN>(qf);
  }
  const auto s = x * x;
  const auto is_odd = Algorithm::isOdd(q);
  x = zinvul::select(x, -x, is_odd);
  auto y = s * s;
  {
    const auto s4 = y * y;
    y = evalPoly6<TanPolyConstants, 0>(s, y, s4);
  }
  y = zinvul::fma(s, y * x, x);
  y = zinvul::select(y, Algorithm::invert(y), is_odd);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::tanImplD(const FloatN theta) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  constexpr size_t n = VectorTypeInfo<FloatN>::size();
  using IntegerN = Integer32VecType<n>;

  auto x = theta;
  IntegerN q = make<IntegerN>(0);
  {
    constexpr auto m_2_pi = static_cast<double>(2.0 / kPi<double>);
    constexpr auto m_24 = static_cast<double>(1 << 24);
    const auto dqh = zinvul::trunc(theta * (m_2_pi / m_24)) * m_24;
    const auto qf = rintImpl(theta * m_2_pi - dqh);

    constexpr auto pi_a = -getPi<double, 0>() * 0.5;
    constexpr auto pi_b = -getPi<double, 1>() * 0.5;
    constexpr auto pi_c = -getPi<double, 2>() * 0.5;
    constexpr auto pi_d = -getPi<double, 3>() * 0.5;
    x = zinvul::fma(dqh, make<FloatN>(pi_a), x);
    x = zinvul::fma(qf, make<FloatN>(pi_a), x);
    x = zinvul::fma(dqh, make<FloatN>(pi_b), x);
    x = zinvul::fma(qf, make<FloatN>(pi_b), x);
    x = zinvul::fma(dqh, make<FloatN>(pi_c), x);
    x = zinvul::fma(qf, make<FloatN>(pi_c), x);
    x = zinvul::fma(dqh + qf, make<FloatN>(pi_d), x);

    q = cast<IntegerN>(qf);
  }
  x = 0.5 * x;
  const auto s = x * x;
  auto y = s * s;
  {
    const auto s4 = y * y;
    y = evalPoly8<TanPolyConstants, 0>(s, y, s4);
  }
  y = zinvul::fma(y, s, make<FloatN>(0.3333333333333343695e+0));
  y = zinvul::fma(s, y * x, x);
  auto u = zinvul::fma(y, y, make<FloatN>(-1.0));
  x = -2.0 * y;

  const auto is_odd = Algorithm::isOdd(q);
  {
    const auto t = x;
    x = zinvul::select(x, -u, is_odd);
    u = zinvul::select(u, t, is_odd);
  }
  y = x / u;
  {
    const auto is_zero = zinvul::isequal(theta, make<FloatN>(0.0));
    y = zinvul::select(u, theta, is_zero);
  }
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::asinImpl(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  if constexpr (sizeof(Float) == 4) {
    const auto y = asinImplF(x);
    return y;
  }
  else if constexpr (sizeof(Float) == 8) {
    const auto y = asinImplD(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::asinImplF(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");

  constexpr float c0 = 0.4197454825e-1f;
  constexpr float c1 = 0.2424046025e-1f;
  constexpr float c2 = 0.4547423869e-1f;
  constexpr float c3 = 0.7495029271e-1f;
  constexpr float c4 = 0.1666677296e+0f;

  auto y = zinvul::fma(make<FloatN>(c0), x, make<FloatN>(c1));
  y = zinvul::fma(y, x, make<FloatN>(c2));
  y = zinvul::fma(y, x, make<FloatN>(c3));
  y = zinvul::fma(y, x, make<FloatN>(c4));

  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::asinImplD(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");

  const auto x2 = x * x;
  const auto x4 = x2 * x2;
  const auto x8 = x4 * x4;
  const auto y = evalPoly12<AsinPolyConstants, 0>(x, x2, x4, x8);

  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::acosImpl(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto y = asinImpl(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::atanImpl(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  using Float = typename VectorTypeInfo<FloatN>::ElementType;
  if constexpr (sizeof(Float) == 4) {
    const auto y = atanImplF(x);
    return y;
  }
  else if constexpr (sizeof(Float) == 8) {
    const auto y = atanImplD(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::atanImplF(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");

  const auto x2 = x * x;
  const auto x4 = x2 * x2;
  const auto y = evalPoly8<AtanPolyConstants, 0>(x, x2, x4);

  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::Zinvul::atanImplD(const FloatN x) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");

  const auto x2 = x * x;
  const auto x4 = x2 * x2;
  const auto x8 = x4 * x4;
  const auto x16 = x8 * x8;
  const auto y = evalPoly19<AtanPolyConstants, 0>(x, x2, x4, x8, x16);

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
template <typename FloatN> inline
auto ilogb(const FloatN x) noexcept
{
  const auto y = Math::ilogb(x);
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
