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
#include <memory>
#include <type_traits>
// Zisc
#include "zisc/floating_point.hpp"
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
  const auto result = (lhs == rhs) ? Config::scalarResultTrue()
                                   : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b isnotequal(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs != rhs) ? Config::scalarResultTrue()
                                   : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b isgreater(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs > rhs) ? Config::scalarResultTrue()
                                  : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b isgreaterequal(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs >= rhs) ? Config::scalarResultTrue()
                                   : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b isless(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs < rhs) ? Config::scalarResultTrue()
                                  : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b islessequal(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs <= rhs) ? Config::scalarResultTrue()
                                   : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b isinf(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = std::isinf(value) ? Config::scalarResultTrue()
                                        : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b isnan(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = std::isnan(value) ? Config::scalarResultTrue()
                                        : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b signbit(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = std::signbit(value) ? Config::scalarResultTrue()
                                          : Config::scalarResultFalse();
  return result;
}

template <typename Type, std::size_t kN> inline
Vector<Type, kN> bitselect(const Vector<Type, kN>& a,
                           const Vector<Type, kN>& b,
                           const Vector<Type, kN>& c) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = zinvul::cl::bitselect(a[i], b[i], c[i]);
  return result;
}

template <typename Type, typename Integer, std::size_t kN> inline
Vector<Type, kN> select(const Vector<Type, kN>& a,
                        const Vector<Type, kN>& b,
                        const Vector<Integer, kN>& c) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = zinvul::cl::select(a[i], b[i], c[i]);
  return result;
}

} // namespace clinner

/*!
  */
inline
int32b isequal(const float lhs, const float rhs) noexcept
{
  const auto result = clinner::isequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isequal(const double lhs, const double rhs) noexcept
{
  const auto result = clinner::isequal(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = lhs == rhs;
  return result;
}

/*!
  */
inline
int32b isnotequal(const float lhs, const float rhs) noexcept
{
  const auto result = clinner::isnotequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isnotequal(const double lhs, const double rhs) noexcept
{
  const auto result = clinner::isnotequal(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isnotequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = lhs != rhs;
  return result;
}

/*!
  */
inline
int32b isgreater(const float lhs, const float rhs) noexcept
{
  const auto result = clinner::isgreater(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isgreater(const double lhs, const double rhs) noexcept
{
  const auto result = clinner::isgreater(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isgreater(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = lhs > rhs;
  return result;
}

/*!
  */
inline
int32b isgreaterequal(const float lhs, const float rhs) noexcept
{
  const auto result = clinner::isgreaterequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isgreaterequal(const double lhs, const double rhs) noexcept
{
  const auto result = clinner::isgreaterequal(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isgreaterequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = lhs >= rhs;
  return result;
}

/*!
  */
inline
int32b isless(const float lhs, const float rhs) noexcept
{
  const auto result = clinner::isless(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isless(const double lhs, const double rhs) noexcept
{
  const auto result = clinner::isless(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isless(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = lhs < rhs;
  return result;
}

/*!
  */
inline
int32b islessequal(const float lhs, const float rhs) noexcept
{
  const auto result = clinner::islessequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b islessequal(const double lhs, const double rhs) noexcept
{
  const auto result = clinner::islessequal(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> islessequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = lhs <= rhs;
  return result;
}

/*!
  */
inline
int32b isinf(const float value) noexcept
{
  const auto result = clinner::isinf(value);
  return result;
}

/*!
  */
inline
int32b isinf(const double value) noexcept
{
  const auto result = clinner::isinf(value);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isinf(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<Config::ComparisonResultType<Float>, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::isinf(value[i])
        ? Config::vecResultTrue<Float>()
        : Config::vecResultFalse<Float>();
  return result;
}

/*!
  */
inline
int32b isnan(const float value) noexcept
{
  const auto result = clinner::isnan(value);
  return result;
}

/*!
  */
inline
int32b isnan(const double value) noexcept
{
  const auto result = clinner::isnan(value);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isnan(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<Config::ComparisonResultType<Float>, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::isnan(value[i])
        ? Config::vecResultTrue<Float>()
        : Config::vecResultFalse<Float>();
  return result;
}

/*!
  */
inline
int32b signbit(const float value) noexcept
{
  const auto result = clinner::signbit(value);
  return result;
}

/*!
  */
inline
int32b signbit(const double value) noexcept
{
  const auto result = clinner::signbit(value);
  return result;
}

/*!
  */
template <typename Float, std::size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> signbit(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<Config::ComparisonResultType<Float>, kN> result;
  for (std::size_t i = 0; i < kN; ++i)
    result[i] = std::signbit(value[i])
        ? Config::vecResultTrue<Float>()
        : Config::vecResultFalse<Float>();
  return result;
}

/*!
  */
template <typename TypeN> inline
TypeN bitselect(const TypeN& a, const TypeN& b, const TypeN& c) noexcept
{
  constexpr bool is_scalar_value = std::is_integral_v<TypeN> ||
                                   std::is_floating_point_v<TypeN>;
  if constexpr (is_scalar_value) {
    if constexpr (std::is_integral_v<TypeN>) {
      const auto result = zisc::cast<TypeN>((a ^ c) | (b & c));
      return result;
    }
    else {
      // Floating point pattern
      using BitType = typename zisc::FloatingPointFromBytes<sizeof(TypeN)>::BitType;
      const auto data = bitselect(*zisc::treatAs<const BitType*>(&a),
                                  *zisc::treatAs<const BitType*>(&b),
                                  *zisc::treatAs<const BitType*>(&c));
      const auto result = *zisc::treatAs<const TypeN*>(&data);
      return result;
    }
  }
  else {
    const auto result = clinner::bitselect(a, b, c);
    return result;
  }
}

/*!
  */
template <typename TypeN, typename IntegerN> inline
TypeN select(const TypeN& a, const TypeN& b, const IntegerN& c) noexcept
{
  constexpr bool is_scalar_value = std::is_integral_v<TypeN> ||
                                   std::is_floating_point_v<TypeN>;
  constexpr bool is_scalar_condition = std::is_integral_v<IntegerN> ||
                                       std::is_floating_point_v<IntegerN>;
  static_assert((is_scalar_value && is_scalar_condition) ||
                (!is_scalar_value && !is_scalar_condition),
                "The TypeN and IntegerN don't have same n component.");
  if constexpr (is_scalar_value) {
    const auto result =  c ? b : a;
    return result;
  }
  else {
    const auto result = clinner::select(a, b, c);
    return result;
  }
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_RELATIONAL_INL_HPP
