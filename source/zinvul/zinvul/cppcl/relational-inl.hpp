/*!
  \file relational-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_RELATIONAL_INL_HPP
#define ZINVUL_CL_RELATIONAL_INL_HPP

#include "relational.hpp"
// Standard C++ library
#include <cmath>
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/utility.hpp"
// Zinvul
#include "types.hpp"
#include "vector.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

namespace clinner {

template <typename Float> inline
int32b isequal(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return (lhs == rhs) ? 1 : 0;
}

template <typename Float> inline
int32b isnotequal(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return (lhs != rhs) ? 1 : 0;
}

template <typename Float> inline
int32b isgreater(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return (lhs > rhs) ? 1 : 0;
}

template <typename Float> inline
int32b isgreaterequal(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return (lhs >= rhs) ? 1 : 0;
}

template <typename Float> inline
int32b isless(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return (lhs < rhs) ? 1 : 0;
}

template <typename Float> inline
int32b islessequal(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return (lhs <= rhs) ? 1 : 0;
}

template <typename Float> inline
int32b isinf(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return std::isinf(value) ? 1 : 0;
}

template <typename Float> inline
int32b isnan(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return std::isnan(value) ? 1 : 0;
}

template <typename Float> inline
int32b signbit(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return std::signbit(value) ? 1 : 0;
}

template <typename Type1, typename Type2, const std::size_t kN> inline
Vector<Type1, kN> select(const Vector<Type1, kN>& a,
                         const Vector<Type1, kN>& b,
                         const Vector<Type2, kN>& c) noexcept
{
  Vector<Type1, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = c[i] ? b[i] : a[i];
  return result;
}

} // namespace clinner

/*!
  */
inline
int32b isequal(const float lhs, const float rhs) noexcept
{
  return clinner::isequal(lhs, rhs);
}

/*!
  */
inline
int32b isequal(const double lhs, const double rhs) noexcept
{
  return clinner::isequal(lhs, rhs);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> isequal(const Vector<Float, kN>& lhs,
                           const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return lhs == rhs;
}

/*!
  */
inline
int32b isnotequal(const float lhs, const float rhs) noexcept
{
  return clinner::isnotequal(lhs, rhs);
}

/*!
  */
inline
int32b isnotequal(const double lhs, const double rhs) noexcept
{
  return clinner::isnotequal(lhs, rhs);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> isnotequal(const Vector<Float, kN>& lhs,
                              const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return lhs != rhs;
}

/*!
  */
inline
int32b isgreater(const float lhs, const float rhs) noexcept
{
  return clinner::isgreater(lhs, rhs);
}

/*!
  */
inline
int32b isgreater(const double lhs, const double rhs) noexcept
{
  return clinner::isgreater(lhs, rhs);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> isgreater(const Vector<Float, kN>& lhs,
                             const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return lhs > rhs;
}

/*!
  */
inline
int32b isgreaterequal(const float lhs, const float rhs) noexcept
{
  return clinner::isgreaterequal(lhs, rhs);
}

/*!
  */
inline
int32b isgreaterequal(const double lhs, const double rhs) noexcept
{
  return clinner::isgreaterequal(lhs, rhs);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> isgreaterequal(const Vector<Float, kN>& lhs,
                                  const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return lhs >= rhs;
}

/*!
  */
inline
int32b isless(const float lhs, const float rhs) noexcept
{
  return clinner::isless(lhs, rhs);
}

/*!
  */
inline
int32b isless(const double lhs, const double rhs) noexcept
{
  return clinner::isless(lhs, rhs);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> isless(const Vector<Float, kN>& lhs,
                          const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return lhs < rhs;
}

/*!
  */
inline
int32b islessequal(const float lhs, const float rhs) noexcept
{
  return clinner::islessequal(lhs, rhs);
}

/*!
  */
inline
int32b islessequal(const double lhs, const double rhs) noexcept
{
  return clinner::islessequal(lhs, rhs);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> islessequal(const Vector<Float, kN>& lhs,
                               const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  return lhs <= rhs;
}

/*!
  */
inline
int32b isinf(const float value) noexcept
{
  return clinner::isinf(value);
}

/*!
  */
inline
int32b isinf(const double value) noexcept
{
  return clinner::isinf(value);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> isinf(const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<int32b, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::isinf(value[i]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
inline
int32b isnan(const float value) noexcept
{
  return clinner::isnan(value);
}

/*!
  */
inline
int32b isnan(const double value) noexcept
{
  return clinner::isnan(value);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> isnan(const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<int32b, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::isnan(value[i]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
inline
int32b signbit(const float value) noexcept
{
  return clinner::signbit(value);
}

/*!
  */
inline
int32b signbit(const double value) noexcept
{
  return clinner::signbit(value);
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<int32b, kN> signbit(const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<int32b, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::signbit(value[i]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type1, typename Type2> inline
Type1 select(const Type1& a, const Type1& b, const Type2& c) noexcept
{
  constexpr bool is_scalar_type1 = std::is_integral_v<Type1> ||
                                   std::is_floating_point_v<Type1>;
  constexpr bool is_scalar_type2 = std::is_integral_v<Type2> ||
                                   std::is_floating_point_v<Type2>;
  static_assert((is_scalar_type1 && is_scalar_type2) ||
                (!is_scalar_type1 && !is_scalar_type2),
                "The Type1 and Type2 don't have same n component.");
  if constexpr (is_scalar_type1)
    return c ? b : a;
  else
    return clinner::select(a, b, c);
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_RELATIONAL_INL_HPP
