/*!
  \file algorithm-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_ALGORITHM_INL_HPP
#define ZINVUL_CL_ALGORITHM_INL_HPP

#include "algorithm.hpp"
// Standard C++ library
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>
// Zisc
#include "zisc/algorithm.hpp"
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
template <typename IntegerN> inline
auto Algorithm::abs(const IntegerN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<IntegerN> ||
                                  std::is_floating_point_v<IntegerN>;
  // Scalar
  if constexpr (is_scalar_type) {
    if constexpr (std::is_signed_v<IntegerN>) {
      using UnsignedI = std::make_unsigned_t<IntegerN>;
      constexpr auto zero = zisc::cast<IntegerN>(0);
      const auto result = zisc::cast<UnsignedI>((x < zero) ? -x : x);
      return result;
    }
    else {
      return x;
    }
  }
  // Vector
  else {
    const auto result = Vec::abs(x);
    return result;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Algorithm::fabs(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<FloatN> ||
                                  std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto result = std::abs(x);
    return result;
  }
  // Vector
  else {
    const auto result = Vec::fabs(x);
    return result;
  }
}

/*!
  */
template <typename Type1N, typename Type2N> inline
Type1N Algorithm::clamp(const Type1N& x,
                        const Type2N& minval,
                        const Type2N& maxval) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<Type1N> ||
                                  std::is_floating_point_v<Type1N>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto result = std::clamp(x, minval, maxval);
    return result;
  }
  // Vector
  else {
    const auto result = Vec::clamp(x, minval, maxval);
    return result;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Algorithm::degrees(const FloatN& r) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<FloatN> ||
                                  std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr FloatN k = zisc::cast<FloatN>(180.0 / zisc::kPi<double>);
    const auto result = k * r;
    return result;
  }
  // Vector
  else {
    const auto result = Vec::degrees(r);
    return result;
  }
}

/*!
  */
template <typename TypeN> inline
TypeN Algorithm::max(const TypeN& x, const TypeN& y) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<TypeN> ||
                                  std::is_floating_point_v<TypeN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto result = (x < y) ? y : x;
    return result;
  }
  // Vector
  else {
    const auto result = Vec::max(x, y);
    return result;
  }
}

/*!
  */
template <typename TypeN> inline
TypeN Algorithm::min(const TypeN& x, const TypeN& y) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<TypeN> ||
                                  std::is_floating_point_v<TypeN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto result = (y < x) ? y : x;
    return result;
  }
  // Vector
  else {
    const auto result = Vec::min(x, y);
    return result;
  }
}

/*!
  */
template <typename FloatN1, typename FloatN2> inline
FloatN1 Algorithm::mix(const FloatN1& x,
                       const FloatN1& y,
                       const FloatN2& a) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<FloatN1> ||
                                  std::is_floating_point_v<FloatN1>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto result = x + a * (y - x);
    return result;
  }
  // Vector
  else {
    const auto result = Vec::mix(x, y, a);
    return result;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Algorithm::radians(const FloatN& d) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<FloatN> ||
                                  std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr FloatN k = zisc::cast<FloatN>(zisc::kPi<double> / 180.0);
    const auto result = k * d;
    return result;
  }
  // Vector
  else {
    const auto result = Vec::radians(d);
    return result;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Algorithm::sign(const FloatN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<FloatN> ||
                                  std::is_floating_point_v<FloatN>;
  // Scalar
  if constexpr (is_scalar_type) {
    constexpr FloatN zero = zisc::cast<FloatN>(0);
    constexpr FloatN one = zisc::cast<FloatN>(1);
    const FloatN y = (x > zero) ? one :
                     (x < zero) ? -one
                                : zero;
    return y;
  }
  // Vector
  else {
    const auto y = Vec::sign(x);
    return y;
  }
}


/*!
  */
template <typename IntegerN> inline
IntegerN Algorithm::clz(const IntegerN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<IntegerN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto result = zisc::cast<IntegerN>(0);
    return result;
  }
  // Vector
  else {
    const auto result = Vec::clz(x);
    return result;
  }
}

/*!
  */
template <typename IntegerN> inline
IntegerN Algorithm::popcount(const IntegerN& x) noexcept
{
  constexpr bool is_scalar_type = std::is_integral_v<IntegerN>;
  // Scalar
  if constexpr (is_scalar_type) {
    const auto result = zisc::Algorithm::popcount(x);
    return result;
  }
  // Vector
  else {
    const auto result = Vec::popcount(x);
    return result;
  }
}

template <typename Integer, size_t kN> inline
auto Algorithm::Vec::abs(const Vector<Integer, kN>& x) noexcept
{
  using UnsignedI = std::make_unsigned_t<Integer>;
  Vector<UnsignedI, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::abs(x[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Algorithm::Vec::fabs(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::fabs(x[i]);
  return result;
}

template <typename Type1, typename Type2, size_t kN> inline
auto Algorithm::Vec::clamp(const Vector<Type1, kN>& x,
                           const Type2 minval,
                           const Type2 maxval) noexcept
{
  Vector<Type1, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::clamp(x[i], minval, maxval);
  return result;
}

template <typename Type1, typename Type2, size_t kN> inline
auto Algorithm::Vec::clamp(const Vector<Type1, kN>& x,
                           const Vector<Type2, kN>& minval,
                           const Vector<Type2, kN>& maxval) noexcept
{
  Vector<Type1, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::clamp(x[i], minval[i], maxval[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Algorithm::Vec::degrees(const Vector<Float, kN>& r) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::degrees(r[i]);
  return result;
}

template <typename Type, size_t kN> inline
auto Algorithm::Vec::max(const Vector<Type, kN>& x,
                         const Vector<Type, kN>& y) noexcept
{
  Vector<Type, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::max(x[i], y[i]);
  return result;
}

template <typename Type, size_t kN> inline
auto Algorithm::Vec::min(const Vector<Type, kN>& x,
                         const Vector<Type, kN>& y) noexcept
{
  Vector<Type, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = cl::min(x[i], y[i]);
  return result;
}

template <typename Float1, typename Float2, size_t kN> inline
auto Algorithm::Vec::mix(const Vector<Float1, kN>& x,
                         const Vector<Float1, kN>& y,
                         const Float2 a) noexcept
{
  Vector<Float1, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::mix(x[i], y[i], a);
  return result;
}

template <typename Float1, typename Float2, size_t kN> inline
auto Algorithm::Vec::mix(const Vector<Float1, kN>& x,
                         const Vector<Float1, kN>& y,
                         const Vector<Float2, kN>& a) noexcept
{
  Vector<Float1, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::mix(x[i], y[i], a[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Algorithm::Vec::radians(const Vector<Float, kN>& d) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::radians(d[i]);
  return result;
}

template <typename Float, size_t kN> inline
auto Algorithm::Vec::sign(const Vector<Float, kN>& x) noexcept
{
  Vector<Float, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::sign(x[i]);
  return result;
}

/*!
  */
template <typename Integer, size_t kN> inline
auto Algorithm::Vec::clz(const Vector<Integer, kN>& x) noexcept
{
  Vector<Integer, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::clz(x[i]);
  return result;
}

/*!
  */
template <typename Integer, size_t kN> inline
auto Algorithm::Vec::popcount(const Vector<Integer, kN>& x) noexcept
{
  Vector<Integer, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Algorithm::popcount(x[i]);
  return result;
}

/*!
  */
template <typename IntegerN> inline
auto abs(const IntegerN& x) noexcept
{
  const auto result = Algorithm::abs(x);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN fabs(const FloatN& x) noexcept
{
  const auto result = Algorithm::fabs(x);
  return result;
}

/*!
  */
template <typename Type1N, typename Type2N> inline
Type1N clamp(const Type1N& x, const Type2N& minval, const Type2N& maxval) noexcept
{
  const auto result = Algorithm::clamp(x, minval, maxval);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN degrees(const FloatN& r) noexcept
{
  const auto result = Algorithm::degrees(r);
  return result;
}

/*!
  */
template <typename TypeN> inline
TypeN max(const TypeN& x, const TypeN& y) noexcept
{
  const auto result = Algorithm::max(x, y);
  return result;
}

/*!
  */
template <typename TypeN> inline
TypeN min(const TypeN& x, const TypeN& y) noexcept
{
  const auto result = Algorithm::min(x, y);
  return result;
}

/*!
  */
template <typename TypeN1, typename TypeN2> inline
TypeN1 mix(const TypeN1& x, const TypeN1& y, const TypeN2& a) noexcept
{
  const auto result = Algorithm::mix(x, y, a);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN radians(const FloatN& d) noexcept
{
  const auto result = Algorithm::radians(d);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN sign(const FloatN& x) noexcept
{
  const auto result = Algorithm::sign(x);
  return result;
}

/*!
  */
template <typename IntegerN> inline
IntegerN clz(const IntegerN& x) noexcept
{
  const auto result = Algorithm::clz(x);
  return result;
}

/*!
  */
template <typename IntegerN> inline
IntegerN popcount(const IntegerN& x) noexcept
{
  const auto result = Algorithm::popcount(x);
  return result;
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_ALGORITHM_INL_HPP
