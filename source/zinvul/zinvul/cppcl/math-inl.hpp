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

template <typename Type, std::size_t kN> inline
auto degrees(const Vector<Type, kN>& r) noexcept
{
  Vector<Type, kN> result;
  constexpr Type k = zisc::cast<Type>(180.0 / zisc::kPi<double>);
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = k * r[i];
  return result;
}

template <typename Float, std::size_t kN> inline
auto fma(const Vector<Float, kN>& a,
         const Vector<Float, kN>& b,
         const Vector<Float, kN>& c) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::fma(a[i], b[i], c[i]);
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

template <typename Type, std::size_t kN> inline
auto radians(const Vector<Type, kN>& d) noexcept
{
  Vector<Type, kN> result;
  constexpr Type k = zisc::cast<Type>(zisc::kPi<double> / 180.0);
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = k * d[i];
  return result;
}

template <typename Float, std::size_t kN> inline
auto sqrt(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::sqrt(x[i]);
  return result;
}

template <typename Float, std::size_t kN> inline
auto rsqrt(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = zisc::invert(std::sqrt(x[i]));
  return result;
}

/*!
  */
template <typename Float, typename Integer, std::size_t kN> inline
auto frexp(const Vector<Float, kN>& x, Vector<Integer, kN>* e) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::frexp(x[i], &(*e)[i]);
  return result;
}

/*!
  */
template <typename Float, typename Integer, std::size_t kN> inline
auto ldexp(const Vector<Float, kN>& x, const Vector<Integer, kN>& e) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::ldexp(x[i], e[i]);
  return result;
}

} // namespace inner

/*!
  */
template <typename Integer> inline
auto abs(const Integer& x) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<Integer>;
  // Scalar
  if constexpr (is_scalar_type) {
    using UnsignedI = std::make_unsigned_t<Integer>;
    constexpr auto zero = zisc::cast<Integer>(0);
    return zisc::cast<UnsignedI>((x < zero) ? -x : x);
  }
  // Vector
  else {
    return inner::abs(x);
  }
}

/*!
  */
template <typename Float> inline
Float fabs(const Float& x) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<Float>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr auto zero = zisc::cast<Float>(0);
    return (x < zero) ? -x : x;
  }
  // Vector
  else {
    return inner::fabs(x);
  }
}

/*!
  */
template <typename Type> inline
Type clamp(const Type& x, const Type& minval, const Type& maxval) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<Type> ||
                                  std::is_floating_point_v<Type>;
  // Scalar
  if constexpr (is_scalar_type)
    return min(max(x, minval), maxval);
  else
    return inner::clamp(x, minval, maxval);
}

/*!
  */
template <typename Type> inline
Type degrees(const Type& r) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<Type> ||
                                  std::is_floating_point_v<Type>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr Type k = zisc::cast<Type>(180.0 / zisc::kPi<double>);
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
template <typename Type> inline
Type max(const Type& x, const Type& y) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<Type> ||
                                  std::is_floating_point_v<Type>;
  // Scalar
  if constexpr (is_scalar_type)
    return (x < y) ? y : x;
  else
    return inner::max(x, y);
}

/*!
  */
template <typename Type> inline
Type min(const Type& x, const Type& y) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<Type> ||
                                  std::is_floating_point_v<Type>;
  // Scalar
  if constexpr (is_scalar_type)
    return (y < x) ? y : x;
  else
    return inner::min(x, y);
}

/*!
  */
template <typename Type> inline
Type radians(const Type& d) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<Type> ||
                                  std::is_floating_point_v<Type>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr Type k = zisc::cast<Type>(zisc::kPi<double> / 180.0);
    return k * d;
  }
  else {
    return inner::radians(d);
  }
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
