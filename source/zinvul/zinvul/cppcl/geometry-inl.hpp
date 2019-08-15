///*!
//  \file math-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2019 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZINVUL_CL_MATH_INL_HPP
//#define ZINVUL_CL_MATH_INL_HPP
//
//#include "math.hpp"
//// Standard C++ library
//#include <algorithm>
//#include <cmath>
//#include <cstddef>
//#include <limits>
//#include <type_traits>
//// Zisc
//#include "zisc/math.hpp"
//#include "zisc/type_traits.hpp"
//#include "zisc/utility.hpp"
//// Zinvul
//#include "vector.hpp"
//#include "zinvul/zinvul_config.hpp"
//
//namespace zinvul {
//
//namespace cl {
//
//namespace clinner {
//
//template <typename Float, std::size_t kN> inline
//auto sign(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::sign(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto ceil(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::ceil(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto floor(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::floor(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto trunc(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::trunc(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto round(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::round(x[i]);
//  return result;
//}
//
//template <typename Integer, std::size_t kN> inline
//auto abs(const Vector<Integer, kN>& x,
//         zisc::EnableIfInteger<Integer> = zisc::kEnabler) noexcept
//{
//  using UnsignedI = std::make_unsigned_t<Integer>;
//  Vector<UnsignedI, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::abs(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto fabs(const Vector<Float, kN>& x,
//         zisc::EnableIfFloat<Float> = zisc::kEnabler) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::fabs(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto fract(const Vector<Float, kN>& x, Vector<Float, kN>* iptr) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::fract(x[i], &(*iptr)[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto fmod(const Vector<Float, kN>& x, const Vector<Float, kN>& y) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::fmod(x[i], y[i]);
//  return result;
//}
//
//template <typename Type, std::size_t kN> inline
//auto clamp(const Vector<Type, kN>& x,
//           const Type minval,
//           const Type maxval) noexcept
//{
//  Vector<Type, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::clamp(x[i], minval, maxval);
//  return result;
//}
//
//template <typename Type, std::size_t kN> inline
//auto clamp(const Vector<Type, kN>& x,
//           const Vector<Type, kN>& minval,
//           const Vector<Type, kN>& maxval) noexcept
//{
//  Vector<Type, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::clamp(x[i], minval[i], maxval[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto degrees(const Vector<Float, kN>& r) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::degrees(r[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto fma(const Vector<Float, kN>& a,
//         const Vector<Float, kN>& b,
//         const Vector<Float, kN>& c) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::fma(a[i], b[i], c[i]);
//  return result;
//}
//
//template <typename Type, std::size_t kN> inline
//auto max(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
//{
//  Vector<Type, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::max(x[i], y[i]);
//  return result;
//}
//
//template <typename Type, std::size_t kN> inline
//auto min(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
//{
//  Vector<Type, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::min(x[i], y[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto radians(const Vector<Float, kN>& d) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::radians(d[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto exp(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::exp(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto log(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::log(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto log2(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::log2(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto pow(const Vector<Float, kN>& base, const Vector<Float, kN>& e) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::pow(base[i], e[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto sqrt(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::sqrt(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto rsqrt(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::rsqrt(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto sin(const Vector<Float, kN>& theta) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::sin(theta[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto cos(const Vector<Float, kN>& theta) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::cos(theta[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto tan(const Vector<Float, kN>& theta) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::tan(theta[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto asin(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::asin(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto acos(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::acos(x[i]);
//  return result;
//}
//
//template <typename Float, std::size_t kN> inline
//auto atan(const Vector<Float, kN>& x) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::atan(x[i]);
//  return result;
//}
//
///*!
//  */
//template <typename Float, typename Integer, std::size_t kN> inline
//auto frexp(const Vector<Float, kN>& x, Vector<Integer, kN>* e) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::frexp(x[i], &(*e)[i]);
//  return result;
//}
//
///*!
//  */
//template <typename Float, typename Integer, std::size_t kN> inline
//auto ldexp(const Vector<Float, kN>& x, const Vector<Integer, kN>& e) noexcept
//{
//  Vector<Float, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::ldexp(x[i], e[i]);
//  return result;
//}
//
///*!
//  */
//template <typename Integer> inline
//constexpr Integer getClzMsb() noexcept
//{
//  using zisc::cast;
//  if constexpr (std::is_signed_v<Integer>) {
//    return std::numeric_limits<Integer>::min();
//  }
//  else {
//    const Integer msb = cast<Integer>(0b01) << cast<Integer>(8 * sizeof(Integer) - 1);
//    return msb;
//  }
//}
//
///*!
//  */
//template <typename Integer, std::size_t kN> inline
//auto clz(const Vector<Integer, kN>& x) noexcept
//{
//  Vector<Integer, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::clz(x[i]);
//  return result;
//}
//
///*!
//  */
//template <typename Integer, std::size_t kN> inline
//auto popcount(const Vector<Integer, kN>& x) noexcept
//{
//  Vector<Integer, kN> result;
//  for (std::size_t i = 0; i < kN; ++i)
//    result[i] = cl::popcount(x[i]);
//  return result;
//}
//
///*!
//  */
//template <typename Float, std::size_t kN> inline
//auto cross(const Vector<Float, kN>& p0, const Vector<Float, kN>& p1) noexcept
//{
//  static_assert((kN == 3) || (kN == 4), "The kN should be 3 or 4.");
//  Vector<Float, kN> result;
//  result[0] = p0[1] * p1[2] - p0[2] * p1[1];
//  result[1] = p0[2] * p1[0] - p0[0] * p1[2];
//  result[2] = p0[0] * p1[1] - p0[1] * p1[0];
//  return result;
//}
//
///*!
//  */
//template <typename Float, std::size_t kN> inline
//Float dot(const Vector<Float, kN>& p0, const Vector<Float, kN>& p1) noexcept
//{
//  Float result = zisc::cast<Float>(0);
//  for (std::size_t i = 0; i < kN; ++i)
//    result += cl::dot(p0[i], p1[i]);
//  return result;
//}
//
///*!
//  */
//template <typename Float, std::size_t kN> inline
//auto normalize(const Vector<Float, kN>& p) noexcept
//{
//  const auto d2 = cl::dot(p, p);
//  const auto result = p * cl::rsqrt(d2);
//  return result;
//}
//
//} // namespace clinner
//
///*!
//  */
//template <typename FloatN> inline
//FloatN sign(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    constexpr FloatN zero = zisc::cast<FloatN>(0);
//    constexpr FloatN one = zisc::cast<FloatN>(1);
//    const FloatN y = (x > zero) ? one :
//                     (x < zero) ? -one
//                                : zero;
//    return y;
//  }
//  // Vector
//  else {
//    const auto y = clinner::sign(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN ceil(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::ceil(x);
//    return y;
//  }
//  // Vector
//  else {
//    const auto y = clinner::ceil(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN floor(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::floor(x);
//    return y;
//  }
//  // Vector
//  else {
//    const auto y = clinner::floor(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN trunc(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::trunc(x);
//    return y;
//  }
//  // Vector
//  else {
//    const auto y = clinner::trunc(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN round(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::round(x);
//    return y;
//  }
//  // Vector
//  else {
//    const auto y = clinner::round(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename IntegerN> inline
//auto abs(const IntegerN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_integral_v<IntegerN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    using UnsignedI = std::make_unsigned_t<IntegerN>;
//    constexpr auto zero = zisc::cast<IntegerN>(0);
//    const auto y = zisc::cast<UnsignedI>((x < zero) ? -x : x);
//    return y;
//  }
//  // Vector
//  else {
//    const auto y = clinner::abs(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN fabs(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    constexpr auto zero = zisc::cast<FloatN>(0);
//    const auto y = (x < zero) ? -x : x;
//    return y;
//  }
//  // Vector
//  else {
//    const auto y = clinner::fabs(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN fract(const FloatN& x, FloatN* iptr) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    constexpr FloatN k = zisc::cast<FloatN>(0x1.fffffep-1f);
//    const FloatN i = floor(x);
//    const FloatN y = min(x - i, k);
//    *iptr = i;
//    return y;
//  }
//  // Vector
//  else {
//    const auto y = clinner::fract(x, iptr);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN fmod(const FloatN& x, const FloatN& y) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto z = std::fmod(x, y);
//    return z;
//  }
//  // Vector
//  else {
//    const auto z = clinner::fmod(x, y);
//    return z;
//  }
//}
//
///*!
//  */
//template <typename Type1N, typename Type2N> inline
//Type1N clamp(const Type1N& x, const Type2N& minval, const Type2N& maxval) noexcept
//{
//  constexpr bool is_scalar_type = std::is_integral_v<Type1N> ||
//                                  std::is_floating_point_v<Type1N>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto result = std::clamp(x, minval, maxval);
//    return result;
//  }
//  else {
//    const auto result = clinner::clamp(x, minval, maxval);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN degrees(const FloatN& r) noexcept
//{
//  constexpr bool is_scalar_type = std::is_integral_v<FloatN> ||
//                                  std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    constexpr FloatN k = zisc::cast<FloatN>(180.0 / zisc::kPi<double>);
//    const auto d = k * r;
//    return d;
//  }
//  else {
//    const auto d = clinner::degrees(r);
//    return d;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN fma(const FloatN& a, const FloatN& b, const FloatN& c) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto result = std::fma(a, b, c);
//    return result;
//  }
//  else {
//    const auto result = clinner::fma(a, b, c);
//    return result;
//  }
//}
//
//
///*!
//  */
//template <typename TypeN> inline
//TypeN max(const TypeN& x, const TypeN& y) noexcept
//{
//  constexpr bool is_scalar_type = std::is_integral_v<TypeN> ||
//                                  std::is_floating_point_v<TypeN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto result = (x < y) ? y : x;
//    return result;
//  }
//  else {
//    const auto result = clinner::max(x, y);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename TypeN> inline
//TypeN min(const TypeN& x, const TypeN& y) noexcept
//{
//  constexpr bool is_scalar_type = std::is_integral_v<TypeN> ||
//                                  std::is_floating_point_v<TypeN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto result = (y < x) ? y : x;
//    return result;
//  }
//  else {
//    const auto result = clinner::min(x, y);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename TypeN1, typename TypeN2> inline
//TypeN1 mix(const TypeN1& x, const TypeN1& y, const TypeN2& a) noexcept
//{
//  const TypeN1 result = x + a * (y - x);
//  return result;
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN radians(const FloatN& d) noexcept
//{
//  constexpr bool is_scalar_type = std::is_integral_v<FloatN> ||
//                                  std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    constexpr FloatN k = zisc::cast<FloatN>(zisc::kPi<double> / 180.0);
//    const auto r = k * d;
//    return r;
//  }
//  else {
//    const auto r = clinner::radians(d);
//    return r;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN exp(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::exp(x);
//    return y;
//  }
//  else {
//    const auto y = clinner::exp(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN log(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::log(x);
//    return y;
//  }
//  else {
//    const auto y = clinner::log(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN log2(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::log2(x);
//    return y;
//  }
//  else {
//    const auto y = clinner::log2(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN pow(const FloatN& base, const FloatN& e) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::pow(base, e);
//    return y;
//  }
//  else {
//    const auto y = clinner::pow(base, e);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN rsqrt(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = zisc::invert(std::sqrt(x));
//    return y;
//  }
//  else {
//    const auto y = clinner::rsqrt(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN sqrt(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::sqrt(x);
//    return y;
//  }
//  else {
//    const auto y = clinner::sqrt(x);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN sin(const FloatN& theta) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::sin(theta);
//    return y;
//  }
//  else {
//    const auto y = clinner::sin(theta);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN cos(const FloatN& theta) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::cos(theta);
//    return y;
//  }
//  else {
//    const auto y = clinner::cos(theta);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN sincos(const FloatN& theta, FloatN* cosval) noexcept
//{
//  *cosval = cos(theta);
//  const auto y = sin(theta);
//  return y;
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN tan(const FloatN& theta) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto y = std::tan(theta);
//    return y;
//  }
//  else {
//    const auto y = clinner::tan(theta);
//    return y;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN asin(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto theta = std::asin(x);
//    return theta;
//  }
//  else {
//    const auto theta = clinner::asin(x);
//    return theta;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN acos(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto theta = std::acos(x);
//    return theta;
//  }
//  else {
//    const auto theta = clinner::acos(x);
//    return theta;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN atan(const FloatN& x) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto theta = std::atan(x);
//    return theta;
//  }
//  else {
//    const auto theta = clinner::atan(x);
//    return theta;
//  }
//}
//
///*!
//  */
//template <typename FloatN, typename IntegerN> inline
//FloatN frexp(const FloatN& x, IntegerN* e) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> &&
//                                  std::is_integral_v<IntegerN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto result = std::frexp(x, e);
//    return result;
//  }
//  else {
//    const auto result = clinner::frexp(x, e);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename FloatN, typename IntegerN> inline
//FloatN ldexp(const FloatN& x, const IntegerN& e) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> &&
//                                  std::is_integral_v<IntegerN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    const auto result = std::ldexp(x, e);
//    return result;
//  }
//  else {
//    const auto result = clinner::ldexp(x, e);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename IntegerN> inline
//IntegerN clz(const IntegerN& x) noexcept
//{
//  using zisc::cast;
//  constexpr bool is_scalar_type = std::is_integral_v<IntegerN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    IntegerN result = 0;
//    constexpr IntegerN n = cast<IntegerN>(8 * sizeof(IntegerN));
//    constexpr IntegerN bit = clinner::getClzMsb<IntegerN>();
//    for (IntegerN i = 0, b = x; (i < n) && ((b & bit) != bit); ++i, b = b << 1)
//      ++result;
//    return result;
//  }
//  else {
//    const auto result = clinner::clz(x);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename IntegerN> inline
//IntegerN popcount(const IntegerN& x) noexcept
//{
//  using zisc::cast;
//  constexpr bool is_scalar_type = std::is_integral_v<IntegerN>;
//  // Scalar
//  if constexpr (is_scalar_type) {
//    IntegerN result = 0;
//    constexpr IntegerN n = cast<IntegerN>(8 * sizeof(IntegerN));
//    constexpr IntegerN bit = cast<IntegerN>(0b01);
//    for (IntegerN i = 0, b = x; i < n; ++i, b = b >> 1) {
//      if ((b & bit) == bit)
//        ++result;
//    }
//    return result;
//  }
//  else {
//    const auto result = clinner::popcount(x);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN cross(const FloatN& p0, const FloatN& p1) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> ||
//                                  std::is_integral_v<FloatN>;
//  static_assert(!is_scalar_type, "FloatN should be vector3 or vector4 type.");
//  const auto result = clinner::cross(p0, p1);
//  return result;
//}
//
///*!
//  */
//template <typename FloatN> inline
//float dot(const FloatN& p0, const FloatN& p1) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> ||
//                                  std::is_integral_v<FloatN>;
//  if constexpr (is_scalar_type) {
//    const float result = p0 * p1;
//    return result;
//  }
//  else {
//    const float result = clinner::dot(p0, p1);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//float distance(const FloatN& p0, const FloatN& p1) noexcept
//{
//  const float result = length(p0 - p1);
//  return result;
//}
//
///*!
//  */
//template <typename FloatN> inline
//float length(const FloatN& p) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> ||
//                                  std::is_integral_v<FloatN>;
//  if constexpr (is_scalar_type) {
//    const float result = abs(p);
//    return result;
//  }
//  else {
//    const float d2 = dot(p, p);
//    const float result = sqrt(d2);
//    return result;
//  }
//}
//
///*!
//  */
//template <typename FloatN> inline
//FloatN normalize(const FloatN& p) noexcept
//{
//  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> ||
//                                  std::is_integral_v<FloatN>;
//  if constexpr (is_scalar_type) {
//    constexpr FloatN zero = zisc::cast<FloatN>(0);
//    constexpr FloatN one = zisc::cast<FloatN>(1);
//    const float result = (p < zero) ? -one : one;
//    return result;
//  }
//  else {
//    const auto result = clinner::normalize(p);
//    return result;
//  }
//}
//
//} // namespace cl
//
//} // namespace zinvul
//
//#endif // ZINVUL_CL_MATH_INL_HPP
