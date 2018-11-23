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
#include "zisc/arith_array.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename Type> inline
Vector<Type, 2>::Vector() noexcept :
    Vector(Array{})
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 2>::Vector(const Array& data) noexcept :
    __data_{data}
{
  static_assert(sizeof(Vector) == 2 * sizeof(Type),
                "The size of Vector2 is wrong.");
  static_assert(std::alignment_of_v<Vector> == 2 * sizeof(Type),
                "The size of Vector2 is wrong.");
}

/*!
  */
template <typename Type> inline
Vector<Type, 2>::Vector(const Type x, const Type y) noexcept :
    Vector(Array{{x, y}})
{
}

/*!
  */
template <typename Type> inline
Type& Vector<Type, 2>::operator[](const std::size_t index) noexcept
{
  return __data_[index];
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 2>::operator[](const std::size_t index) const noexcept
{
  return __data_[index];
}

/*!
  */
template <typename Type> inline
Vector<Type, 3>::Vector() noexcept :
    Vector(Array{})
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 3>::Vector(const Array& data) noexcept :
    __data_{data},
    __padding0_{zisc::cast<Type>(0)}
{
  static_assert(sizeof(Vector) == 4 * sizeof(Type),
                "The size of Vector3 is wrong.");
  static_assert(std::alignment_of_v<Vector> == 4 * sizeof(Type),
                "The size of Vector3 is wrong.");
}

/*!
  */
template <typename Type> inline
Vector<Type, 3>::Vector(const Type x, const Type y, const Type z) noexcept :
    Vector(Array{{x, y, z}})
{
}

/*!
  */
template <typename Type> inline
Type& Vector<Type, 3>::operator[](const std::size_t index) noexcept
{
  return __data_[index];
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 3>::operator[](const std::size_t index) const noexcept
{
  return __data_[index];
}

/*!
  */
template <typename Type> inline
Vector<Type, 4>::Vector() noexcept :
    Vector(Array{})
{
}

/*!
  */
template <typename Type> inline
Vector<Type, 4>::Vector(const Array& data) noexcept :
    __data_{data}
{
  static_assert(sizeof(Vector) == 4 * sizeof(Type),
                "The size of Vector4 is wrong.");
  static_assert(std::alignment_of_v<Vector> == 4 * sizeof(Type),
                "The size of Vector4 is wrong.");
}

/*!
  */
template <typename Type> inline
Vector<Type, 4>::Vector(const Type x, const Type y, const Type z, const Type w)
    noexcept :
        Vector(Array{{x, y, z, w}})
{
}

/*!
  */
template <typename Type> inline
Type& Vector<Type, 4>::operator[](const std::size_t index) noexcept
{
  return __data_[index];
}

/*!
  */
template <typename Type> inline
const Type& Vector<Type, 4>::operator[](const std::size_t index) const noexcept
{
  return __data_[index];
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  const Vector<Type, kN> result{lhs.__data_ + rhs.__data_};
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  const Vector<Type, kN> result{lhs.__data_ - rhs.__data_};
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  const Vector<Type, kN> result{lhs.__data_ * rhs.__data_};
  return result;
}

/*!
  */
template <typename Type, std::size_t kN> inline
Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept
{
  const Vector<Type, kN> result{lhs.__data_ / rhs.__data_};
  return result;
}

} // namespace zinvul

#endif // ZINVUL_VECTOR_INL_HPP
