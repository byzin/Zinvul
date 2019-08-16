/*!
  \file geometry-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_GEOMETRY_INL_HPP
#define ZINVUL_CL_GEOMETRY_INL_HPP

#include "geometry.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/type_traits.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "math.hpp"
#include "vector.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

/*!
  */
template <typename FloatN> inline
FloatN Geometry::cross(const FloatN& p0, const FloatN& p1) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> ||
                                  std::is_integral_v<FloatN>;
  static_assert(!is_scalar_type, "FloatN should be vector3 or vector4 type.");
  const auto result = Vec::cross(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::dot(const FloatN& p0, const FloatN& p1) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> ||
                                  std::is_integral_v<FloatN>;
  if constexpr (is_scalar_type) {
    const auto result = p0 * p1;
    return result;
  }
  else {
    const auto result = Vec::dot(p0, p1);
    return result;
  }
}

/*!
  */
template <typename FloatN> inline
auto Geometry::distance(const FloatN& p0, const FloatN& p1) noexcept
{
  const auto result = length(p0 - p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::length(const FloatN& p) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> ||
                                  std::is_integral_v<FloatN>;
  if constexpr (is_scalar_type) {
    const auto result = abs(p);
    return result;
  }
  else {
    const auto d2 = dot(p, p);
    const auto result = Math::sqrt(d2);
    return result;
  }
}

/*!
  */
template <typename FloatN> inline
FloatN Geometry::normalize(const FloatN& p) noexcept
{
  constexpr bool is_scalar_type = std::is_floating_point_v<FloatN> ||
                                  std::is_integral_v<FloatN>;
  if constexpr (is_scalar_type) {
    constexpr FloatN zero = zisc::cast<FloatN>(0);
    constexpr FloatN one = zisc::cast<FloatN>(1);
    const auto result = (p < zero) ? -one : one;
    return result;
  }
  else {
    const auto result = Vec::normalize(p);
    return result;
  }
}

/*!
  */
template <typename Float, std::size_t kN> inline
auto Geometry::Vec::cross(const Vector<Float, kN>& p0,
                          const Vector<Float, kN>& p1) noexcept
{
  static_assert((kN == 3) || (kN == 4), "The kN should be 3 or 4.");
  Vector<Float, kN> result;
  result[0] = p0[1] * p1[2] - p0[2] * p1[1];
  result[1] = p0[2] * p1[0] - p0[0] * p1[2];
  result[2] = p0[0] * p1[1] - p0[1] * p1[0];
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
auto Geometry::Vec::dot(const Vector<Float, kN>& p0,
                        const Vector<Float, kN>& p1) noexcept
{
  Float result = zisc::cast<Float>(0);
  for (std::size_t i = 0; i < kN; ++i)
    result += Geometry::dot(p0[i], p1[i]);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
auto Geometry::Vec::normalize(const Vector<Float, kN>& p) noexcept
{
  const auto d2 = Geometry::dot(p, p);
  const auto result = p * Math::rsqrt(d2);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN cross(const FloatN& p0, const FloatN& p1) noexcept
{
  const auto result = Geometry::cross(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto dot(const FloatN& p0, const FloatN& p1) noexcept
{
  const auto result = Geometry::dot(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto distance(const FloatN& p0, const FloatN& p1) noexcept
{
  const auto result = Geometry::distance(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto length(const FloatN& p) noexcept
{
  const auto result = Geometry::length(p);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN normalize(const FloatN& p) noexcept
{
  const auto result = Geometry::normalize(p);
  return result;
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_GEOMETRY_INL_HPP
