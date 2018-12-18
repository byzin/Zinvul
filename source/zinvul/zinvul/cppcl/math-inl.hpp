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

template <typename Integer, std::size_t kN>
auto abs(const Vector<Integer, kN>& x,
         zisc::EnableIfInteger<Integer> = zisc::kEnabler) noexcept
{
  using UnsignedI = std::make_unsigned_t<Integer>;
  Vector<UnsignedI, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::abs(x[i]);
  return result;
}

template <typename Float, std::size_t kN>
auto fabs(const Vector<Float, kN>& x,
         zisc::EnableIfFloat<Float> = zisc::kEnabler) noexcept
{
  Vector<Float, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::fabs(x[i]);
  return result;
}

template <typename Type, std::size_t kN>
auto clamp(const Vector<Type, kN>& x,
           const Vector<Type, kN>& minval,
           const Vector<Type, kN>& maxval) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::clamp(x[i], minval[i], maxval[i]);
  return result;
}

template <typename Type, std::size_t kN>
auto degrees(const Vector<Type, kN>& r) noexcept
{
  Vector<Type, kN> result;
  constexpr Type k = zisc::cast<Type>(180.0 / zisc::kPi<double>);
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = k * r[i];
  return result;
}

template <typename Type, std::size_t kN>
auto max(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::max(x[i], y[i]);
  return result;
}

template <typename Type, std::size_t kN>
auto min(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = cl::min(x[i], y[i]);
  return result;
}

template <typename Type, std::size_t kN>
auto radians(const Vector<Type, kN>& d) noexcept
{
  Vector<Type, kN> result;
  constexpr Type k = zisc::cast<Type>(zisc::kPi<double> / 180.0);
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = k * d[i];
  return result;
}

} // namespace inner

/*!
  */
template <typename Integer>
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
template <typename Float>
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
template <typename Type>
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
template <typename Type>
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
template <typename Type>
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
template <typename Type>
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
template <typename Type>
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

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_MATH_INL_HPP
