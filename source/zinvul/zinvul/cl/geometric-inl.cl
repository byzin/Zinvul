/*!
  \file geometric-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_GEOMETRIC_INL_CL
#define ZINVUL_GEOMETRIC_INL_CL

#include "geometric.cl"
// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  */
template <typename FloatN> inline
FloatN Geometry::cross(const FloatN p0, const FloatN p1) noexcept
{
  using VecInfo = VectorTypeInfo<FloatN>;
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert((VecInfo::size() == 3) || (VecInfo::size() == 4),
                "The size of vector should be 3 or 4.");
  const auto result = crossImpl(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::dot(const FloatN p0, const FloatN p1) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto result = dotImpl(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::dot3(const FloatN p0, const FloatN p1) noexcept
{
  using VecInfo = VectorTypeInfo<FloatN>;
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert((VecInfo::size() == 3) || (VecInfo::size() == 4),
                "The size of vector should be 3 or 4.");
  const auto result = dot3Impl(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::distance(const FloatN p0, const FloatN p1) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto result = distanceImpl(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::length(const FloatN p) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto result = lengthImpl(p);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN Geometry::normalize(const FloatN p) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto result = normalizeImpl(p);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN Geometry::normalize3(const FloatN p) noexcept
{
  using VecInfo = VectorTypeInfo<FloatN>;
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert((VecInfo::size() == 3) || (VecInfo::size() == 4),
                "The size of vector should be 3 or 4.");
  const auto result = normalize3Impl(p);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN Geometry::crossImpl(const FloatN p0, const FloatN p1) noexcept
{
  using VecInfo = VectorTypeInfo<FloatN>;
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert((VecInfo::size() == 3) || (VecInfo::size() == 4),
                "The size of vector should be 3 or 4.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::cross(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::dotImpl(const FloatN p0, const FloatN p1) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::dot(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::dot3Impl(FloatN p0, FloatN p1) noexcept
{
  using VecInfo = VectorTypeInfo<FloatN>;
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert((VecInfo::size() == 3) || (VecInfo::size() == 4),
                "The size of vector should be 3 or 4.");
  if constexpr (VecInfo::size() == 4) {
    using Float = typename VecInfo::ElementType;
    p0.w = static_cast<Float>(0.0);
    p1.w = static_cast<Float>(0.0);
  }
  const auto result = dotImpl(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::distanceImpl(const FloatN p0, const FloatN p1) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::distance(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto Geometry::lengthImpl(const FloatN p) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::length(p);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN Geometry::normalizeImpl(const FloatN p) noexcept
{
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  const auto result = ZINVUL_GLOBAL_NAMESPACE::normalize(p);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN Geometry::normalize3Impl(FloatN p) noexcept
{
  using VecInfo = VectorTypeInfo<FloatN>;
  static_assert(kIsFloatingPoint<FloatN>, "The FloatN isn't floating point.");
  static_assert((VecInfo::size() == 3) || (VecInfo::size() == 4),
                "The size of vector should be 3 or 4.");
  if constexpr (VecInfo::size() == 4) {
    using Float = typename VecInfo::ElementType;
    p.w = static_cast<Float>(0.0);
  }
  const auto result = normalizeImpl(p);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN cross(const FloatN p0, const FloatN p1) noexcept
{
  const auto result = Geometry::cross(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto dot(const FloatN p0, const FloatN p1) noexcept
{
  const auto result = Geometry::dot(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto distance(const FloatN p0, const FloatN p1) noexcept
{
  const auto result = Geometry::distance(p0, p1);
  return result;
}

/*!
  */
template <typename FloatN> inline
auto length(const FloatN p) noexcept
{
  const auto result = Geometry::length(p);
  return result;
}

/*!
  */
template <typename FloatN> inline
FloatN normalize(const FloatN p) noexcept
{
  const auto result = Geometry::normalize(p);
  return result;
}

} // namespace zinvul

#endif /* ZINVUL_GEOMETRIC_INL_CL */
