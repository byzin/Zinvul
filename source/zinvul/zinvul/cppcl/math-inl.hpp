/*!
  \file math-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_MATH_INL_HPP
#define ZINVUL_CL_MATH_INL_HPP

#include "math.hpp"
// Standard C++ library
#include <cmath>
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/math.hpp"
#include "zisc/type_traits.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vector.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

namespace inner {

template <typename Float, std::size_t kN> inline
auto sign(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::sign(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto ceil(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::ceil(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto floor(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::floor(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto trunc(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::trunc(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto round(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::round(x[i]);
  return result;
}

template <typename Integer, std::size_t kN> inline
auto abs(const Vector<Integer, kN>& x,
         zisc::EnableIfInteger<Integer> = zisc::kEnabler) noexcept
{
  using UnsignedI = std::make_unsigned_t<Integer>;
  Vector<UnsignedI, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::abs(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto fabs(const Vector<Float, kN>& x,
         zisc::EnableIfFloat<Float> = zisc::kEnabler) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::fabs(x[i]);
  return result;
}

template <typename Type, std::size_t kN> inline
auto clamp(const Vector<Type, kN>& x,
           const Vector<Type, kN>& minval,
           const Vector<Type, kN>& maxval) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::clamp(x[i], minval[i], maxval[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto degrees(const Vector<Float, kN>& r) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::degrees(r[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto fma(const Vector<Float, kN>& a,
         const Vector<Float, kN>& b,
         const Vector<Float, kN>& c) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::fma(a[i], b[i], c[i]);
  return result;
}

template <typename Type, std::size_t kN> inline
auto max(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::max(x[i], y[i]);
  return result;
}

template <typename Type, std::size_t kN> inline
auto min(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::min(x[i], y[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto radians(const Vector<Float, kN>& d) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::radians(d[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto exp(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::exp(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto log(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::log(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto log2(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::log2(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto pow(const Vector<Float, kN>& base, const Vector<Float, kN>& e) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::pow(base[i], e[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto sqrt(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::sqrt(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto rsqrt(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::rsqrt(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto sin(const Vector<Float, kN>& theta) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::sin(theta[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto cos(const Vector<Float, kN>& theta) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::cos(theta[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto tan(const Vector<Float, kN>& theta) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::tan(theta[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto asin(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::asin(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto acos(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::acos(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto atan(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::atan(x[i]);
  return result;
}

/*!
  */
template <typename Float, typename Integer, std::size_t kN> inline
auto frexp(const Vector<Float, kN>& x, Vector<Integer, kN>* e) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::frexp(x[i], &(*e)[i]);
  return result;
}

/*!
  */
template <typename Float, typename Integer, std::size_t kN> inline
auto ldexp(const Vector<Float, kN>& x, const Vector<Integer, kN>& e) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::ldexp(x[i], e[i]);
  return result;
}

} // namespace inner

/*!
  */
template <typename FloatN> inline
FloatN sign(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const FloatN zero = zisc::cast<FloatN>(0);
    const FloatN one = zisc::cast<FloatN>(1);
    const FloatN y = (x > zero) ? one :
                     (x < zero) ? -one
                                : zero;
    return y;
  }
  // Vector
  else {
    return inner::sign(x);
  }
}

/*!
  */
template <typename FloatN> inline
FloatN ceil(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    return std::ceil(x);
  }
  // Vector
  else {
    return inner::ceil(x);
  }
}

/*!
  */
template <typename FloatN> inline
FloatN floor(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    return std::floor(x);
  }
  // Vector
  else {
    return inner::floor(x);
  }
}

/*!
  */
template <typename FloatN> inline
FloatN trunc(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    return std::trunc(x);
  }
  // Vector
  else {
    return inner::trunc(x);
  }
}

/*!
  */
template <typename FloatN> inline
FloatN round(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    return std::round(x);
  }
  // Vector
  else {
    return inner::round(x);
  }
}

/*!
  */
template <typename IntegerN> inline
auto abs(const IntegerN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<IntegerN>;
  // Scalar
  if constexpr (is_scalar_type) {
    using UnsignedI = std::make_unsigned_t<IntegerN>;
    constexpr auto zero = zisc::cast<IntegerN>(0);
    return zisc::cast<UnsignedI>((x < zero) ? -x : x);
  }
  // Vector
  else {
    return inner::abs(x);
  }
}

/*!
  */
template <typename FloatN> inline
FloatN fabs(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr auto zero = zisc::cast<FloatN>(0);
    return (x < zero) ? -x : x;
  }
  // Vector
  else {
    return inner::fabs(x);
  }
}

/*!
  */
template <typename TypeN> inline
TypeN clamp(const TypeN& x, const TypeN& minval, const TypeN& maxval) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<TypeN> ||
                                  std::is_floating_point_v<TypeN>;
  // Scalar
  if constexpr (is_scalar_type)
    return min(max(x, minval), maxval);
  else
    return inner::clamp(x, minval, maxval);
}

/*!
  */
template <typename FloatN> inline
FloatN degrees(const FloatN& r) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<FloatN> ||
                                  std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr FloatN k = zisc::cast<FloatN>(180.0 / zisc::kPi<double>);
    return k * r;
  }
  else {
    return inner::degrees(r);
  }
}

/*!
  */
template <typename FloatN> inline
FloatN fma(const FloatN& a, const FloatN& b, const FloatN& c) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::fma(a, b, c);
  else
    return inner::fma(a, b, c);
}


/*!
  */
template <typename TypeN> inline
TypeN max(const TypeN& x, const TypeN& y) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<TypeN> ||
                                  std::is_floating_point_v<TypeN>;
  // Scalar
  if constexpr (is_scalar_type)
    return (x < y) ? y : x;
  else
    return inner::max(x, y);
}

/*!
  */
template <typename TypeN> inline
TypeN min(const TypeN& x, const TypeN& y) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<TypeN> ||
                                  std::is_floating_point_v<TypeN>;
  // Scalar
  if constexpr (is_scalar_type)
    return (y < x) ? y : x;
  else
    return inner::min(x, y);
}

/*!
  */
template <typename TypeN1, typename TypeN2> inline
TypeN1 mix(const TypeN1& x, const TypeN1& y, const TypeN2& a) noexcept
{
  const TypeN1 result = x + a * (y - x);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN radians(const FloatN& d) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<FloatN> ||
                                  std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr FloatN k = zisc::cast<FloatN>(zisc::kPi<double> / 180.0);
    return k * d;
  }
  else {
    return inner::radians(d);
  }
}

/*!
  */
template <typename FloatN> inline
FloatN exp(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::exp(x);
  else
    return inner::exp(x);
}

/*!
  */
template <typename FloatN> inline
FloatN log(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::log(x);
  else
    return inner::log(x);
}

/*!
  */
template <typename FloatN> inline
FloatN log2(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::log2(x);
  else
    return inner::log2(x);
}

/*!
  */
template <typename FloatN> inline
FloatN pow(const FloatN& base, const FloatN& e) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::pow(base, e);
  else
    return inner::pow(base, e);
}

/*!
  */
template <typename FloatN> inline
FloatN rsqrt(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return zisc::invert(std::sqrt(x));
  else
    return inner::rsqrt(x);
}

/*!
  */
template <typename FloatN> inline
FloatN sqrt(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::sqrt(x);
  else
    return inner::sqrt(x);
}

/*!
  */
template <typename FloatN> inline
FloatN sin(const FloatN& theta) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::sin(theta);
  else
    return inner::sin(theta);
}

/*!
  */
template <typename FloatN> inline
FloatN cos(const FloatN& theta) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::cos(theta);
  else
    return inner::cos(theta);
}

/*!
  */
template <typename FloatN> inline
FloatN sincos(const FloatN& theta, FloatN* cosval) noexcept
{
  *cosval = cos(theta);
  return sin(theta);
}

/*!
  */
template <typename FloatN> inline
FloatN tan(const FloatN& theta) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::tan(theta);
  else
    return inner::tan(theta);
}

/*!
  */
template <typename FloatN> inline
FloatN asin(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::asin(x);
  else
    return inner::asin(x);
}

/*!
  */
template <typename FloatN> inline
FloatN acos(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::acos(x);
  else
    return inner::acos(x);
}

/*!
  */
template <typename FloatN> inline
FloatN atan(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type)
    return std::atan(x);
  else
    return inner::atan(x);
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN frexp(const FloatN& x, IntegerN* e) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> &&
                                  std::is_integral_v<IntegerN>;
  // Scalar
  if constexpr (is_scalar_type) {
    return std::frexp(x, e);
  }
  else {
    return inner::frexp(x, e);
  }
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN ldexp(const FloatN& x, const IntegerN& e) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> &&
                                  std::is_integral_v<IntegerN>;
  // Scalar
  if constexpr (is_scalar_type) {
    return std::ldexp(x, e);
  }
  else {
    return inner::ldexp(x, e);
  }
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_MATH_INL_HPP
