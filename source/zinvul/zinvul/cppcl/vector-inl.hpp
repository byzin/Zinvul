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
  return (index == 0) ? x : y;
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 2>::operator[](const std::size_t index) const noexcept
{
  return (index == 0) ? x : y;
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
  return (index == 0) ? x : (index == 1) ? y : z;
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 3>::operator[](const std::size_t index) const noexcept
{
  return (index == 0) ? x : (index == 1) ? y : z;
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
  return ((index & 2u) == 0)
      ? (index == 0) ? x : y
      : (index == 2) ? z : w;
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 4>::operator[](const std::size_t index) const noexcept
{
  return ((index & 2u) == 0)
      ? (index == 0) ? x : y
      : (index == 2) ? z : w;
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
  return rhs + lhs;
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
  return -(rhs - lhs);
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
  return rhs * lhs;
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
  return rhs & lhs;
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
  return rhs | lhs;
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
  return rhs ^ lhs;
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
    result[index] = lhs[index] << rhs[index];
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
    result[index] = lhs << rhs[index];
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
    result[index] = lhs[index] << rhs;
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
    result[index] = lhs[index] >> rhs[index];
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
    result[index] = lhs >> rhs[index];
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
    result[index] = lhs[index] >> rhs;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator==(const Vector<Type, kN>& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] == rhs[index]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator==(const Type& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs == rhs[index]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator==(const Vector<Type, kN>& lhs,
                              const Type& rhs) noexcept
{
  return rhs == lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator!=(const Vector<Type, kN>& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  return ~(lhs == rhs);
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator!=(const Type& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  return ~(lhs == rhs);
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator!=(const Vector<Type, kN>& lhs,
                              const Type& rhs) noexcept
{
  return rhs != lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator<(const Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] < rhs[index]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator<(const Type& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs < rhs[index]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator<(const Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] < rhs) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator<=(const Vector<Type, kN>& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  return (lhs == rhs) || (lhs < rhs);
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator<=(const Type& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  return (lhs == rhs) || (lhs < rhs);
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator<=(const Vector<Type, kN>& lhs,
                              const Type& rhs) noexcept
{
  return (lhs == rhs) || (lhs < rhs);
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator>(const Vector<Type, kN>& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  return rhs < lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator>(const Type& lhs,
                             const Vector<Type, kN>& rhs) noexcept
{
  return rhs < lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator>(const Vector<Type, kN>& lhs,
                             const Type& rhs) noexcept
{
  return rhs < lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator>=(const Vector<Type, kN>& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  return rhs <= lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator>=(const Type& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  return rhs <= lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator>=(const Vector<Type, kN>& lhs,
                              const Type& rhs) noexcept
{
  return rhs <= lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator!(const Vector<Type, kN>& value) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = !value[index] ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator&&(const Vector<Type, kN>& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] && rhs[index]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator&&(const Type& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs && rhs[index]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator&&(const Vector<Type, kN>& lhs,
                              const Type& rhs) noexcept
{
  return rhs && lhs;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator||(const Vector<Type, kN>& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs[index] || rhs[index]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator||(const Type& lhs,
                              const Vector<Type, kN>& rhs) noexcept
{
  Vector<int32b, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = (lhs || rhs[index]) ? kVecTrue : kVecFalse;
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<int32b, kN> operator||(const Vector<Type, kN>& lhs,
                              const Type& rhs) noexcept
{
  return rhs || lhs;
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
template <typename Type> inline
Vector<Type, 2> vload2(const size_t offset, const Type* p) noexcept
{
  return clinner::vloadn<Type, 2>(offset, p);
}

/*!
  */
template <typename Type> inline
Vector<Type, 3> vload3(const size_t offset, const Type* p) noexcept
{
  return clinner::vloadn<Type, 3>(offset, p);
}

/*!
  */
template <typename Type> inline
Vector<Type, 4> vload4(const size_t offset, const Type* p) noexcept
{
  return clinner::vloadn<Type, 4>(offset, p);
}

/*!
  */
inline
float vload_half(const size_t offset, const half* p) noexcept
{
  const half* address = p + offset;
  const zisc::SingleFloat data{*address};
  return data.toFloat();
}

/*!
  */
inline
float2 vload_half2(const size_t offset, const half* p) noexcept
{
  return clinner::vload_halfn<2>(offset, p);
}

/*!
  */
inline
float3 vload_half3(const size_t offset, const half* p) noexcept
{
  return clinner::vload_halfn<3>(offset, p);
}

/*!
  */
inline
float4 vload_half4(const size_t offset, const half* p) noexcept
{
  return clinner::vload_halfn<4>(offset, p);
}

/*!
  */
template <typename Type> inline
void vstore2(const Vector<Type, 2>& data, const size_t offset, Type* p) noexcept
{
  clinner::vstoren<Type, 2>(data, offset, p);
}

/*!
  */
template <typename Type> inline
void vstore3(const Vector<Type, 3>& data, const size_t offset, Type* p) noexcept
{
  clinner::vstoren<Type, 3>(data, offset, p);
}

/*!
  */
template <typename Type> inline
void vstore4(const Vector<Type, 4>& data, const size_t offset, Type* p) noexcept
{
  clinner::vstoren<Type, 4>(data, offset, p);
}

/*!
  */
inline
void vstore_half(const float data, const size_t offset, half* p) noexcept
{
  half* address = p + offset;
  const auto fdata = zisc::SingleFloat::fromFloat(data);
  *address = fdata;
}

/*!
  */
inline
void vstore_half2(const float2& data, const size_t offset, half* p) noexcept
{
  clinner::vstore_halfn(data, offset, p);
}

/*!
  */
inline
void vstore_half3(const float3& data, const size_t offset, half* p) noexcept
{
  clinner::vstore_halfn(data, offset, p);
}

/*!
  */
inline
void vstore_half4(const float4& data, const size_t offset, half* p) noexcept
{
  clinner::vstore_halfn(data, offset, p);
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_VECTOR_INL_HPP
