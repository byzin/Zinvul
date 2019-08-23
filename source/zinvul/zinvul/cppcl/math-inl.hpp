/*!
  \file math-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_MATH_INL_HPP
#define ZINVUL_CL_MATH_INL_HPP

#include "math.hpp"
// Standard C++ library
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
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

/*!
  */
template <typename FloatN> inline
FloatN Math::ceil(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::ceil(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::ceil(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::floor(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::floor(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::floor(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::trunc(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::trunc(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::trunc(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::round(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::round(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::round(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::fract(const FloatN& x, FloatN* iptr) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr FloatN k = zisc::cast<FloatN>(0x1.fffffep-1f);
    const FloatN i = floor(x);
    const FloatN y = min(x - i, k);
    *iptr = i;
    return y;
  }
  // Vector
  else {
    const auto y = Vec::fract(x, iptr);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::fmod(const FloatN& x, const FloatN& y) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto z = std::fmod(x, y);
    return z;
  }
  // Vector
  else {
    const auto z = Vec::fmod(x, y);
    return z;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::fma(const FloatN& a, const FloatN& b, const FloatN& c) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::fma(a, b, c);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::fma(a, b, c);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::exp(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::exp(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::exp(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::exp2(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::exp2(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::exp2(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::log(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::log(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::log(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::log2(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::log2(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::log2(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::pow(const FloatN& base, const FloatN& e) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::pow(base, e);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::pow(base, e);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::rsqrt(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = zisc::invert(std::sqrt(x));
    return y;
  }
  // Vector
  else {
    const auto y = Vec::rsqrt(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::sqrt(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::sqrt(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::sqrt(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::cbrt(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::cbrt(x);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::cbrt(x);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::sin(const FloatN& theta) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::sin(theta);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::sin(theta);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::cos(const FloatN& theta) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::cos(theta);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::cos(theta);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::sincos(const FloatN& theta, FloatN* cosval) noexcept
{
  *cosval = cos(theta);
  const auto y = sin(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN Math::tan(const FloatN& theta) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::tan(theta);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::tan(theta);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::asin(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto theta = std::asin(x);
    return theta;
  }
  // Vector
  else {
    const auto theta = Vec::asin(x);
    return theta;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::acos(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto theta = std::acos(x);
    return theta;
  }
  // Vector
  else {
    const auto theta = Vec::acos(x);
    return theta;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::atan(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto theta = std::atan(x);
    return theta;
  }
  // Vector
  else {
    const auto theta = Vec::atan(x);
    return theta;
  }
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::frexp(const FloatN& x, IntegerN* e) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> &&
                                  std::is_integral_v<IntegerN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::frexp(x, e);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::frexp(x, e);
    return y;
  }
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN Math::ldexp(const FloatN& x, const IntegerN& e) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> &&
                                  std::is_integral_v<IntegerN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto y = std::ldexp(x, e);
    return y;
  }
  // Vector
  else {
    const auto y = Vec::ldexp(x, e);
    return y;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Math::copysign(const FloatN& x, const FloatN& y) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto z = std::copysign(x, y);
    return z;
  }
  // Vector
  else {
    const auto z = Vec::copysign(x, y);
    return z;
  }
}

template <typename Float, size_t kN> inline
auto Math::Vec::ceil(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::ceil(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::floor(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::floor(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::trunc(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::trunc(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::round(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::round(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::fract(const Vector<Float, kN>& x,
                      Vector<Float, kN>* iptr) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::fract(x[i], &(*iptr)[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::fmod(const Vector<Float, kN>& x,
                     const Vector<Float, kN>& y) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::fmod(x[i], y[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::fma(const Vector<Float, kN>& a,
                    const Vector<Float, kN>& b,
                    const Vector<Float, kN>& c) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::fma(a[i], b[i], c[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::exp(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::exp(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::exp2(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::exp2(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::log(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::log(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::log2(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::log2(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::pow(const Vector<Float, kN>& base,
                    const Vector<Float, kN>& e) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::pow(base[i], e[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::rsqrt(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::rsqrt(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::sqrt(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::sqrt(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::cbrt(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::cbrt(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::sin(const Vector<Float, kN>& theta) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::sin(theta[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::cos(const Vector<Float, kN>& theta) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::cos(theta[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::tan(const Vector<Float, kN>& theta) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::tan(theta[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::asin(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::asin(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::acos(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::acos(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Math::Vec::atan(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::atan(x[i]);
  return result;
}

/*!
  */
template <typename Float, typename Integer, size_t kN> inline
auto Math::Vec::frexp(const Vector<Float, kN>& x,
                      Vector<Integer, kN>* e) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::frexp(x[i], &(*e)[i]);
  return result;
}

/*!
  */
template <typename Float, typename Integer, size_t kN> inline
auto Math::Vec::ldexp(const Vector<Float, kN>& x,
                      const Vector<Integer, kN>& e) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::ldexp(x[i], e[i]);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
auto Math::Vec::copysign(const Vector<Float, kN>& x,
                         const Vector<Float, kN>& y) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Math::copysign(x[i], y[i]);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN ceil(const FloatN& x) noexcept
{
  const auto y = Math::ceil(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN floor(const FloatN& x) noexcept
{
  const auto y = Math::floor(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN trunc(const FloatN& x) noexcept
{
  const auto y = Math::trunc(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN round(const FloatN& x) noexcept
{
  const auto y = Math::round(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN fract(const FloatN& x, FloatN* iptr) noexcept
{
  const auto y = Math::fract(x, iptr);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN fmod(const FloatN& x, const FloatN& y) noexcept
{
  const auto z = Math::fmod(x, y);
  return z;
}

/*!
  */
template <typename FloatN> inline
FloatN fma(const FloatN& a, const FloatN& b, const FloatN& c) noexcept
{
  const auto y = Math::fma(a, b, c);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN exp(const FloatN& x) noexcept
{
  const auto y = Math::exp(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN exp2(const FloatN& x) noexcept
{
  const auto y = Math::exp2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN log(const FloatN& x) noexcept
{
  const auto y = Math::log(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN log2(const FloatN& x) noexcept
{
  const auto y = Math::log2(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN pow(const FloatN& base, const FloatN& e) noexcept
{
  const auto y = Math::pow(base, e);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN rsqrt(const FloatN& x) noexcept
{
  const auto y = Math::rsqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN sqrt(const FloatN& x) noexcept
{
  const auto y = Math::sqrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN cbrt(const FloatN& x) noexcept
{
  const auto y = Math::cbrt(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN sin(const FloatN& theta) noexcept
{
  const auto y = Math::sin(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN cos(const FloatN& theta) noexcept
{
  const auto y = Math::cos(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN sincos(const FloatN& theta, FloatN* cosval) noexcept
{
  const auto y = Math::sincos(theta, cosval);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN tan(const FloatN& theta) noexcept
{
  const auto y = Math::tan(theta);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN asin(const FloatN& x) noexcept
{
  const auto y = Math::asin(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN acos(const FloatN& x) noexcept
{
  const auto y = Math::acos(x);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN atan(const FloatN& x) noexcept
{
  const auto y = Math::atan(x);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN frexp(const FloatN& x, IntegerN* e) noexcept
{
  const auto y = Math::frexp(x, e);
  return y;
}

/*!
  */
template <typename FloatN, typename IntegerN> inline
FloatN ldexp(const FloatN& x, const IntegerN& e) noexcept
{
  const auto y = Math::ldexp(x, e);
  return y;
}

/*!
  */
template <typename FloatN> inline
FloatN copysign(const FloatN& x, const FloatN& y) noexcept
{
  const auto z = Math::copysign(x, y);
  return z;
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_MATH_INL_HPP
