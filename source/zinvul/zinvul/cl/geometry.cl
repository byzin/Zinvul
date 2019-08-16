/*!
  \file geometry.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_GEOMETRY_CL
#define ZINVUL_GEOMETRY_CL

// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  */
class Geometry
{
 public:
  //! Return the cross product of p0.xyz and p1.xyz
  template <typename FloatN>
  static FloatN cross(const FloatN p0, const FloatN p1) noexcept;

  //! Compute dot product
  template <typename FloatN>
  static auto dot(const FloatN p0, const FloatN p1) noexcept;

  //! Compute dot product of p0.xyz and p1.xyz
  template <typename FloatN>
  static auto dot3(const FloatN p0, const FloatN p1) noexcept;

  //! Return the distance between p0 and p1
  template <typename FloatN>
  static auto distance(const FloatN p0, const FloatN p1) noexcept;

  //! Return the length of vector p
  template <typename FloatN>
  static auto length(const FloatN p) noexcept;

  //! Return a vector in the same direction as p but with a length of 1
  template <typename FloatN>
  static FloatN normalize(const FloatN p) noexcept;

  //! Return a vector in the same direction as p.xyz but with a length of 1
  template <typename FloatN>
  static FloatN normalize3(const FloatN p) noexcept;

 private:
  //! Return the cross product of p0.xyz and p1.xyz
  template <typename FloatN>
  static FloatN crossImpl(const FloatN p0, const FloatN p1) noexcept;

  //! Compute dot product
  template <typename FloatN>
  static auto dotImpl(const FloatN p0, const FloatN p1) noexcept;

  //! Compute dot product of p0.xyz and p1.xyz
  template <typename FloatN>
  static auto dot3Impl(FloatN p0, FloatN p1) noexcept;

  //! Return the distance between p0 and p1
  template <typename FloatN>
  static auto distanceImpl(const FloatN p0, const FloatN p1) noexcept;

  //! Return the length of vector p
  template <typename FloatN>
  static auto lengthImpl(const FloatN p) noexcept;

  //! Return a vector in the same direction as p but with a length of 1
  template <typename FloatN>
  static FloatN normalizeImpl(const FloatN p) noexcept;

  //! Return a vector in the same direction as p.xyz but with a length of 1
  template <typename FloatN>
  static FloatN normalize3Impl(FloatN p) noexcept;
};

// OpenCL style function aliases

//! Return the cross product of p0.xyz and p1.xyz
template <typename FloatN>
FloatN cross(const FloatN p0, const FloatN p1) noexcept;

//! Compute dot product
template <typename FloatN>
auto dot(const FloatN p0, const FloatN p1) noexcept;

//! Return the distance between p0 and p1
template <typename FloatN>
auto distance(const FloatN p0, const FloatN p1) noexcept;

//! Return the length of vector p
template <typename FloatN>
auto length(const FloatN p) noexcept;

//! Return a vector in the same direction as p but with a length of 1
template <typename FloatN>
FloatN normalize(const FloatN p) noexcept;

} // namespace zinvul

#include "geometry-inl.cl"

#endif /* ZINVUL_GEOMETRY_CL */
