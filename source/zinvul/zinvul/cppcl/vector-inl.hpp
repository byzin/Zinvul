/*!
  \file vector-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VECTOR_INL_HPP
#define ZINVUL_VECTOR_INL_HPP

#include "vector.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/utility.hpp"
// Zinvul
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

/*!
  */
template <typename Type> inline
Vector<Type, 2>::Vector() noexcept :
    Vector(zisc::cast<Type>(0), zisc::cast<Type>(0))
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
Vector<Type, 3>::Vector() noexcept :
    Vector(zisc::cast<Type>(0), zisc::cast<Type>(0), zisc::cast<Type>(0))
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
Vector<Type, 4>::Vector() noexcept :
    Vector(zisc::cast<Type>(0), zisc::cast<Type>(0),
           zisc::cast<Type>(0), zisc::cast<Type>(0))
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
  return (index == 0) ? x : (index == 1) ? y : (index == 2) ? z : w;
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 4>::operator[](const std::size_t index) const noexcept
{
  return (index == 0) ? x : (index == 1) ? y : (index == 2) ? z : w;
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
Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  Vector<Type, kN> result;
  for (std::size_t index = 0; index < kN; ++index)
    result[index] = lhs[index] / rhs[index];
  return result;
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_VECTOR_INL_HPP
