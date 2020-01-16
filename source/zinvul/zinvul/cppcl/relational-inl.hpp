/*!
  \file relational-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
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

/*!
  */
inline
int32b Relation::isequal(const float lhs, const float rhs) noexcept
{
  const auto result = isequalImpl(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b Relation::isequal(const double lhs, const double rhs) noexcept
{
  const auto result = isequalImpl(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::isequal(
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
int32b Relation::isnotequal(const float lhs, const float rhs) noexcept
{
  const auto result = isnotequalImpl(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b Relation::isnotequal(const double lhs, const double rhs) noexcept
{
  const auto result = isnotequalImpl(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::isnotequal(
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
int32b Relation::isgreater(const float lhs, const float rhs) noexcept
{
  const auto result = isgreaterImpl(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b Relation::isgreater(const double lhs, const double rhs) noexcept
{
  const auto result = isgreaterImpl(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::isgreater(
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
int32b Relation::isgreaterequal(const float lhs, const float rhs) noexcept
{
  const auto result = isgreaterequalImpl(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b Relation::isgreaterequal(const double lhs, const double rhs) noexcept
{
  const auto result = isgreaterequalImpl(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::isgreaterequal(
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
int32b Relation::isless(const float lhs, const float rhs) noexcept
{
  const auto result = islessImpl(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b Relation::isless(const double lhs, const double rhs) noexcept
{
  const auto result = islessImpl(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::isless(
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
int32b Relation::islessequal(const float lhs, const float rhs) noexcept
{
  const auto result = islessequalImpl(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b Relation::islessequal(const double lhs, const double rhs) noexcept
{
  const auto result = islessequalImpl(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::islessequal(
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
int32b Relation::isinf(const float value) noexcept
{
  const auto result = isinfImpl(value);
  return result;
}

/*!
  */
inline
int32b Relation::isinf(const double value) noexcept
{
  const auto result = isinfImpl(value);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::isinf(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<Config::ComparisonResultType<Float>, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = std::isinf(value[i])
        ? Config::vecResultTrue<Float>()
        : Config::vecResultFalse<Float>();
  return result;
}

/*!
  */
inline
int32b Relation::isnan(const float value) noexcept
{
  const auto result = isnanImpl(value);
  return result;
}

/*!
  */
inline
int32b Relation::isnan(const double value) noexcept
{
  const auto result = isnanImpl(value);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::isnan(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<Config::ComparisonResultType<Float>, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = std::isnan(value[i])
        ? Config::vecResultTrue<Float>()
        : Config::vecResultFalse<Float>();
  return result;
}

/*!
  */
inline
int32b Relation::signbit(const float value) noexcept
{
  const auto result = signbitImpl(value);
  return result;
}

/*!
  */
inline
int32b Relation::signbit(const double value) noexcept
{
  const auto result = signbitImpl(value);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> Relation::signbit(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  Vector<Config::ComparisonResultType<Float>, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = std::signbit(value[i])
        ? Config::vecResultTrue<Float>()
        : Config::vecResultFalse<Float>();
  return result;
}

/*!
  */
template <typename TypeN> inline
TypeN Relation::bitselect(const TypeN& a, const TypeN& b, const TypeN& c) noexcept
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
    const auto result = Vec::bitselect(a, b, c);
    return result;
  }
}

/*!
  */
template <typename TypeN, typename IntegerN> inline
TypeN Relation::select(const TypeN& a, const TypeN& b, const IntegerN& c) noexcept
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
    const auto result = Vec::select(a, b, c);
    return result;
  }
}

template <typename Float> inline
int32b Relation::isequalImpl(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs == rhs) ? Config::scalarResultTrue()
                                   : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b Relation::isnotequalImpl(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs != rhs) ? Config::scalarResultTrue()
                                   : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b Relation::isgreaterImpl(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs > rhs) ? Config::scalarResultTrue()
                                  : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b Relation::isgreaterequalImpl(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs >= rhs) ? Config::scalarResultTrue()
                                   : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b Relation::islessImpl(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs < rhs) ? Config::scalarResultTrue()
                                  : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b Relation::islessequalImpl(const Float lhs, const Float rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = (lhs <= rhs) ? Config::scalarResultTrue()
                                   : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b Relation::isinfImpl(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = std::isinf(value) ? Config::scalarResultTrue()
                                        : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b Relation::isnanImpl(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = std::isnan(value) ? Config::scalarResultTrue()
                                        : Config::scalarResultFalse();
  return result;
}

template <typename Float> inline
int32b Relation::signbitImpl(const Float value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = std::signbit(value) ? Config::scalarResultTrue()
                                          : Config::scalarResultFalse();
  return result;
}

template <typename Type, size_t kN> inline
Vector<Type, kN> Relation::Vec::bitselect(const Vector<Type, kN>& a,
                                          const Vector<Type, kN>& b,
                                          const Vector<Type, kN>& c) noexcept
{
  Vector<Type, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Relation::bitselect(a[i], b[i], c[i]);
  return result;
}

template <typename Type, typename Integer, size_t kN> inline
Vector<Type, kN> Relation::Vec::select(const Vector<Type, kN>& a,
                                       const Vector<Type, kN>& b,
                                       const Vector<Integer, kN>& c) noexcept
{
  Vector<Type, kN> result;
  for (size_t i = 0; i < kN; ++i)
    result[i] = Relation::select(a[i], b[i], c[i]);
  return result;
}

/*!
  */
inline
int32b isequal(const float lhs, const float rhs) noexcept
{
  const auto result = Relation::isequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isequal(const double lhs, const double rhs) noexcept
{
  const auto result = Relation::isequal(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::isequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isnotequal(const float lhs, const float rhs) noexcept
{
  const auto result = Relation::isnotequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isnotequal(const double lhs, const double rhs) noexcept
{
  const auto result = Relation::isnotequal(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isnotequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::isnotequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isgreater(const float lhs, const float rhs) noexcept
{
  const auto result = Relation::isgreater(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isgreater(const double lhs, const double rhs) noexcept
{
  const auto result = Relation::isgreater(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isgreater(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::isgreater(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isgreaterequal(const float lhs, const float rhs) noexcept
{
  const auto result = Relation::isgreaterequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isgreaterequal(const double lhs, const double rhs) noexcept
{
  const auto result = Relation::isgreaterequal(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isgreaterequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::isgreaterequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isless(const float lhs, const float rhs) noexcept
{
  const auto result = Relation::isless(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isless(const double lhs, const double rhs) noexcept
{
  const auto result = Relation::isless(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isless(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::isless(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b islessequal(const float lhs, const float rhs) noexcept
{
  const auto result = Relation::islessequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b islessequal(const double lhs, const double rhs) noexcept
{
  const auto result = Relation::islessequal(lhs, rhs);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> islessequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::islessequal(lhs, rhs);
  return result;
}

/*!
  */
inline
int32b isinf(const float value) noexcept
{
  const auto result = Relation::isinf(value);
  return result;
}

/*!
  */
inline
int32b isinf(const double value) noexcept
{
  const auto result = Relation::isinf(value);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isinf(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::isinf(value);
  return result;
}

/*!
  */
inline
int32b isnan(const float value) noexcept
{
  const auto result = Relation::isnan(value);
  return result;
}

/*!
  */
inline
int32b isnan(const double value) noexcept
{
  const auto result = Relation::isnan(value);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> isnan(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::isnan(value);
  return result;
}

/*!
  */
inline
int32b signbit(const float value) noexcept
{
  const auto result = Relation::signbit(value);
  return result;
}

/*!
  */
inline
int32b signbit(const double value) noexcept
{
  const auto result = Relation::signbit(value);
  return result;
}

/*!
  */
template <typename Float, size_t kN> inline
Vector<Config::ComparisonResultType<Float>, kN> signbit(
    const Vector<Float, kN>& value) noexcept
{
  static_assert(std::is_floating_point_v<Float>, "The Float isn't float type.");
  const auto result = Relation::signbit(value);
  return result;
}

/*!
  */
template <typename TypeN> inline
TypeN bitselect(const TypeN& a, const TypeN& b, const TypeN& c) noexcept
{
  const auto result = Relation::bitselect(a, b, c);
  return result;
}

/*!
  */
template <typename TypeN, typename IntegerN> inline
TypeN select(const TypeN& a, const TypeN& b, const IntegerN& c) noexcept
{
  const auto result = Relation::select(a, b, c);
  return result;
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_RELATIONAL_INL_HPP
