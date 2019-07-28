/*!
  \file vector-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_VECTOR_INL_HPP
#define ZINVUL_CL_VECTOR_INL_HPP

#include "vector.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/floating_point.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

/*!
  */
template <typename Type> inline
Vector<Type, 2>::Vector() noexcept : Vector(zisc::cast<Type>(0))
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 2>::Vector(const Type v) noexcept : Vector(v, v)
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 2>::Vector(const Type v0, const Type v1) noexcept :
    x{v0},
    y{v1}
{
  static_assert(sizeof(Vector) == 2 * sizeof(Type),
                "The size of Vector2 is wrong.");
  static_assert(std::alignment_of_v<Vector> == 2 * sizeof(Type),
                "The alignment of Vector2 is wrong.");
}

/*!
  */
template <typename Type> inline
Type& Vector<Type, 2>::operator[](const std::size_t index) noexcept
{
  auto& result = (index == 0) ? x : y;
  return result;
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 2>::operator[](const std::size_t index) const noexcept
{
  const auto& result = (index == 0) ? x : y;
  return result;
}

/*!
  */
template <typename Type> inline
constexpr std::size_t Vector<Type, 2>::size() noexcept
{
  return 2;
}

/*!
  */
template <typename Type> inline
Vector<Type, 3>::Vector() noexcept : Vector(zisc::cast<Type>(0))
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 3>::Vector(const Type v) noexcept : Vector(v, v, v)
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 3>::Vector(const Type v0, const Type v1, const Type v2) noexcept :
    x{v0},
    y{v1},
    z{v2},
    __padding_{zisc::cast<Type>(0)}
{
  static_assert(sizeof(Vector) == 4 * sizeof(Type),
                "The size of Vector3 is wrong.");
  static_assert(std::alignment_of_v<Vector> == 4 * sizeof(Type),
                "The size of Vector3 is wrong.");
}

/*!
  */
template <typename Type> inline
Type& Vector<Type, 3>::operator[](const std::size_t index) noexcept
{
  auto& result = (index == 0) ? x : (index == 1) ? y : z;
  return result;
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 3>::operator[](const std::size_t index) const noexcept
{
  const auto& result = (index == 0) ? x : (index == 1) ? y : z;
  return result;
}

/*!
  */
template <typename Type> inline
constexpr std::size_t Vector<Type, 3>::size() noexcept
{
  return 3;
}

/*!
  */
template <typename Type> inline
Vector<Type, 4>::Vector() noexcept : Vector(zisc::cast<Type>(0))
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 4>::Vector(const Type v) noexcept : Vector(v, v, v, v)
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 4>::Vector(const Type v0, const Type v1, const Type v2, const Type v3)
    noexcept :
        x{v0},
        y{v1},
        z{v2},
        w{v3}
{
  static_assert(sizeof(Vector) == 4 * sizeof(Type),
                "The size of Vector4 is wrong.");
  static_assert(std::alignment_of_v<Vector> == 4 * sizeof(Type),
                "The size of Vector4 is wrong.");
}

/*!
  */
template <typename Type> inline
Type& Vector<Type, 4>::operator[](const std::size_t index) noexcept
{
  auto& result = ((index & 2u) == 0)
      ? (index == 0) ? x : y
      : (index == 2) ? z : w;
  return result;
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 4>::operator[](const std::size_t index) const noexcept
{
  const auto& result = ((index & 2u) == 0)
      ? (index == 0) ? x : y
      : (index == 2) ? z : w;
  return result;
}

/*!
  */
template <typename Type> inline
constexpr std::size_t Vector<Type, 4>::size() noexcept
{
  return 4;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator+=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator+=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator-=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs - rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator-=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  lhs = lhs - rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator*=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs * rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator*=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  lhs = lhs * rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator/=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  lhs = lhs / rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator/=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  lhs = lhs / rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator%=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  lhs = lhs % rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator%=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  lhs = lhs % rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator&=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  lhs = lhs & rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator&=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  lhs = lhs & rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator|=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  lhs = lhs | rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator|=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  lhs = lhs | rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator^=(Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  lhs = lhs ^ rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator^=(Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  lhs = lhs ^ rhs;
  return lhs;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                               const Vector<Type2, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  lhs = lhs << rhs;
  return lhs;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN>& operator<<=(Vector<Type1, kN>& lhs,
                               const Type2& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  lhs = lhs << rhs;
  return lhs;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                               const Vector<Type2, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  lhs = lhs >> rhs;
  return lhs;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN>& operator>>=(Vector<Type1, kN>& lhs,
                               const Type2& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  lhs = lhs >> rhs;
  return lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator++(Vector<Type, kN>& value) noexcept
{
  value = value + zisc::cast<Type>(1);
  return value;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN>& operator--(Vector<Type, kN>& value) noexcept
{
  value = value - zisc::cast<Type>(1);
  return value;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator++(Vector<Type, kN>& value, int) noexcept
{
  const auto temp = value;
  value = value + zisc::cast<Type>(1);
  return temp;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator--(Vector<Type, kN>& value, int) noexcept
{
  const auto temp = value;
  value = value - zisc::cast<Type>(1);
  return temp;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator-(const Vector<Type, kN>& value) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = -value[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] + rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator+(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs + rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept
{
  const auto result = rhs + lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] - rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator-(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs - rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept
{
  const auto result = -(rhs - lhs);
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] * rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator*(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs * rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept
{
  const auto result = rhs * lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] / rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator/(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs / rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] / rhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator%(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] % rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator%(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs % rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator%(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] % rhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator~(const Vector<Type, kN>& value) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = ~value[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator&(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] & rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator&(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs & rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator&(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  const auto result = rhs & lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator|(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] | rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator|(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs | rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator|(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  const auto result = rhs | lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator^(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] ^ rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator^(const Type& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs ^ rhs[index];
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator^(const Vector<Type, kN>& lhs,
                           const Type& rhs) noexcept
{
  static_assert(std::is_integral_v<Type>, "The Type isn't integer type.");
  const auto result = rhs ^ lhs;
  return result;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                             const Vector<Type2, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  Vector<Type1, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs[index] << rhs[index]);
  return result;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN> operator<<(const Type1& lhs,
                             const Vector<Type2, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  Vector<Type1, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs << rhs[index]);
  return result;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN> operator<<(const Vector<Type1, kN>& lhs,
                             const Type2& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  Vector<Type1, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs[index] << rhs);
  return result;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                             const Vector<Type2, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  Vector<Type1, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs[index] >> rhs[index]);
  return result;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN> operator>>(const Type1& lhs,
                             const Vector<Type2, kN>& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  Vector<Type1, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs >> rhs[index]);
  return result;
}

/*!
  */
template <typename Type1, typename Type2, std::size_t kN> inline
Vector<Type1, kN> operator>>(const Vector<Type1, kN>& lhs,
                             const Type2& rhs) noexcept
{
  static_assert(std::is_integral_v<Type1>, "The Type1 isn't integer type.");
  static_assert(std::is_integral_v<Type2>, "The Type2 isn't integer type.");
  Vector<Type1, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = zisc::cast<Type1>(lhs[index] >> rhs);
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator==(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] == rhs[index])
        ? Config::vecResultTrue<Type>() 
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator==(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs == rhs[index])
        ? Config::vecResultTrue<Type>() 
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator==(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept
{
  const auto result = rhs == lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator!=(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  const auto result = ~(lhs == rhs);
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator!=(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  const auto result = ~(lhs == rhs);
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator!=(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept
{
  const auto result = rhs != lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator<(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] < rhs[index])
        ? Config::vecResultTrue<Type>() 
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator<(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs < rhs[index])
        ? Config::vecResultTrue<Type>() 
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator<(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] < rhs)
        ? Config::vecResultTrue<Type>() 
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator<=(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  const auto result = (lhs == rhs) || (lhs < rhs);
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator<=(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  const auto result = (lhs == rhs) || (lhs < rhs);
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator<=(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept
{
  const auto result = (lhs == rhs) || (lhs < rhs);
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator>(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  const auto result = rhs < lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator>(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  const auto result = rhs < lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator>(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept
{
  const auto result = rhs < lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator>=(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  const auto result = rhs <= lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator>=(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  const auto result = rhs <= lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator>=(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept
{
  const auto result = rhs <= lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator!(
    const Vector<Type, kN>& value) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = !value[index]
        ? Config::vecResultTrue<Type>() 
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator&&(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] && rhs[index])
        ? Config::vecResultTrue<Type>() 
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator&&(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs && rhs[index])
        ? Config::vecResultTrue<Type>() 
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator&&(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept
{
  const auto result = rhs && lhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator||(
    const Vector<Type, kN>& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] || rhs[index])
        ? Config::vecResultTrue<Type>()
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator||(
    const Type& lhs,
    const Vector<Type, kN>& rhs) noexcept
{
  Vector<Config::ComparisonResultType<Type>, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs || rhs[index])
        ? Config::vecResultTrue<Type>()
        : Config::vecResultFalse<Type>();
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Config::ComparisonResultType<Type>, kN> operator||(
    const Vector<Type, kN>& lhs,
    const Type& rhs) noexcept
{
  const auto result = rhs || lhs;
  return result;
}

namespace clinner {

template <typename Type, std::size_t kN> inline
Vector<Type, kN> vloadn(const size_t offset, const Type* p) noexcept
{
  Vector<Type, kN> data;
  const Type* address = p + offset * kN;
  for (std::size_t i = 0; i < kN; ++i)
    data[i] = address[i];
  return data;
}

template <std::size_t kN> inline
Vector<float, kN> vload_halfn(const size_t offset, const half* p) noexcept
{
  Vector<float, kN> data;
  const half* address = p + offset * kN;
  for (std::size_t i = 0; i < kN; ++i) {
    const zisc::SingleFloat v{address[i]};
    data[i] = v.toFloat();
  }
  return data;
}

template <typename Type, std::size_t kN> inline
void vstoren(const Vector<Type, kN>& data,
             const size_t offset,
             Type* p) noexcept
{
  Type* address = p + offset * kN;
  for (std::size_t i = 0; i < kN; ++i)
    address[i] = data[i];
}

template <std::size_t kN> inline
void vstore_halfn(const Vector<float, kN>& data,
                  const size_t offset,
                  half* p) noexcept
{
  half* address = p + offset * kN;
  for (std::size_t i = 0; i < kN; ++i) {
    const auto v = zisc::SingleFloat::fromFloat(data[i]);
    address[i] = v;
  }
}

} // namespace clinner

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
Vector<std::remove_cv_t<Type>, 2> vload2(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  const auto result = clinner::vloadn<T, 2>(offset, p.get());
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
Vector<std::remove_cv_t<Type>, 3> vload3(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  const auto result = clinner::vloadn<T, 3>(offset, p.get());
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
Vector<std::remove_cv_t<Type>, 4> vload4(
    const size_t offset,
    const AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  const auto result = clinner::vloadn<T, 4>(offset, p.get());
  return result;
}

///*!
//  */
//template <typename Type> inline
//float vload_half(const size_t offset, const Type& p) noexcept
//{
//  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
//  if constexpr (std::is_pointer_v<T>) {
//    const half* address = p + offset;
//    const zisc::SingleFloat data{*address};
//    return data.toFloat();
//  }
//  else {
//    const half* address = p.get() + offset;
//    const zisc::SingleFloat data{*address};
//    return data.toFloat();
//  }
//}
//
///*!
//  */
//inline
//template <typename Type> inline
//float2 vload_half2(const size_t offset, const Type& p) noexcept
//{
//  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
//  if constexpr (std::is_pointer_v<T>) {
//    const auto result = clinner::vload_halfn<2>(offset, p);
//    return result;
//  }
//  else {
//    const auto result = clinner::vload_halfn<2>(offset, p.get());
//    return result;
//  }
//}
//
///*!
//  */
//template <typename Type> inline
//float3 vload_half3(const size_t offset, const Type& p) noexcept
//{
//  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
//  if constexpr (std::is_pointer_v<T>) {
//    const auto result = clinner::vload_halfn<3>(offset, p);
//    return result;
//  }
//  else {
//    const auto result = clinner::vload_halfn<3>(offset, p.get());
//    return result;
//  }
//}
//
///*!
//  */
//template <typename Type> inline
//float4 vload_half4(const size_t offset, const Type& p) noexcept
//{
//  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
//  if constexpr (std::is_pointer_v<T>) {
//    const auto result = clinner::vload_halfn<4>(offset, p);
//    return result;
//  }
//  else {
//    const auto result = clinner::vload_halfn<4>(offset, p.get());
//    return result;
//  }
//}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void vstore2(const Vector<Type, 2>& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  using T = std::remove_cv_t<Type>;
  clinner::vstoren<T, 2>(data, offset, p.get());
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void vstore3(const Vector<Type, 3>& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
  clinner::vstoren<T, 3>(data, offset, p.get());
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
void vstore4(const Vector<Type, 4>& data,
    const size_t offset,
    AddressSpacePointer<kAddressSpaceType, Type> p) noexcept
{
  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
  clinner::vstoren<T, 4>(data, offset, p.get());
}

///*!
//  */
//template <typename Type> inline
//void vstore_half(const float data, const size_t offset, Type&& p) noexcept
//{
//  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
//  if constexpr (std::is_pointer_v<T>) {
//    half* address = p + offset;
//    const auto fdata = zisc::SingleFloat::fromFloat(data);
//    *address = fdata;
//  }
//  else {
//    half* address = p.get() + offset;
//    const auto fdata = zisc::SingleFloat::fromFloat(data);
//    *address = fdata;
//  }
//}
//
///*!
//  */
//template <typename Type> inline
//void vstore_half2(const float2& data, const size_t offset, Type&& p) noexcept
//{
//  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
//  if constexpr (std::is_pointer_v<T>)
//    clinner::vstore_halfn(data, offset, p);
//  else
//    clinner::vstore_halfn(data, offset, p.get());
//}
//
///*!
//  */
//template <typename Type> inline
//void vstore_half3(const float3& data, const size_t offset, Type&& p) noexcept
//{
//  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
//  if constexpr (std::is_pointer_v<T>)
//    clinner::vstore_halfn(data, offset, p);
//  else
//    clinner::vstore_halfn(data, offset, p.get());
//}
//
///*!
//  */
//template <typename Type> inline
//void vstore_half4(const float4& data, const size_t offset, Type&& p) noexcept
//{
//  using T = std::remove_cv_t<std::remove_reference_t<Type>>;
//  if constexpr (std::is_pointer_v<T>)
//    clinner::vstore_halfn(data, offset, p);
//  else
//    clinner::vstore_halfn(data, offset, p.get());
//}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_VECTOR_INL_HPP
