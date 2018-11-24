/*!
  \file vector.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VECTOR_HPP
#define ZINVUL_VECTOR_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/utility.hpp"
// Zinvul
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

/*!
  */
template <typename Type, std::size_t kN> struct Vector;

/*!
  */
template <typename Type>
struct alignas(2 * sizeof(Type)) Vector<Type, 2>
{
  //! Initialize a vector
  Vector() noexcept;

  //! Initialize a vector
  Vector(const Type v0, const Type v1) noexcept;


  //! Return a reference to the element
  Type& operator[](const std::size_t index) noexcept;

  //! Return a reference to the element
  const Type& operator[](const std::size_t index) const noexcept;


  Type x,
       y;
};

/*!
  */
template <typename Type>
struct alignas(4 * sizeof(Type)) Vector<Type, 3>
{
  //! Initialize a vector
  Vector() noexcept;

  //! Initialize a vector
  Vector(const Type v0, const Type v1, const Type v2) noexcept;


  //! Return a reference to the element
  Type& operator[](const std::size_t index) noexcept;

  //! Return a reference to the element
  const Type& operator[](const std::size_t index) const noexcept;


  Type x,
       y,
       z,
       __padding_;
};

/*!
  */
template <typename Type>
struct alignas(4 * sizeof(Type)) Vector<Type, 4>
{
  //! Initialize a vector
  Vector() noexcept;

  //! Initialize a vector
  Vector(const Type v0, const Type v1, const Type v2, const Type v3) noexcept;


  //! Return a reference to the element
  Type& operator[](const std::size_t index) noexcept;

  //! Return a reference to the element
  const Type& operator[](const std::size_t index) const noexcept;


  Type x,
       y,
       z,
       w;
};

//! Compute an addition of two vectors
template <typename Type, std::size_t kN>
Vector<Type, kN> operator+(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a subtract of two vectors
template <typename Type, std::size_t kN>
Vector<Type, kN> operator-(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a multiplication of two vectors
template <typename Type, std::size_t kN>
Vector<Type, kN> operator*(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

//! Compute a division of two vectors
template <typename Type, std::size_t kN>
Vector<Type, kN> operator/(const Vector<Type, kN>& lhs,
                           const Vector<Type, kN>& rhs) noexcept;

// Type aliases
// Vector2
using char2 = Vector<int8b, 2>;
using uchar2 = Vector<uint8b, 2>;
using short2 = Vector<int16b, 2>;
using ushort2 = Vector<uint16b, 2>;
using int2 = Vector<int32b, 2>;
using uint2 = Vector<uint32b, 2>;
using long2 = Vector<int64b, 2>;
using ulong2 = Vector<uint64b, 2>;
using half2 = Vector<half, 2>;
using float2 = Vector<float, 2>;
using double2 = Vector<double, 2>;
// Vector3
using char3 = Vector<int8b, 3>;
using uchar3 = Vector<uint8b, 3>;
using short3 = Vector<int16b, 3>;
using ushort3 = Vector<uint16b, 3>;
using int3 = Vector<int32b, 3>;
using uint3 = Vector<uint32b, 3>;
using long3 = Vector<int64b, 3>;
using ulong3 = Vector<uint64b, 3>;
using half3 = Vector<half, 3>;
using float3 = Vector<float, 3>;
using double3 = Vector<double, 3>;
// Vector4
using char4 = Vector<int8b, 4>;
using uchar4 = Vector<uint8b, 4>;
using short4 = Vector<int16b, 4>;
using ushort4 = Vector<uint16b, 4>;
using int4 = Vector<int32b, 4>;
using uint4 = Vector<uint32b, 4>;
using long4 = Vector<int64b, 4>;
using ulong4 = Vector<uint64b, 4>;
using half4 = Vector<half, 4>;
using float4 = Vector<float, 4>;
using double4 = Vector<double, 4>;

} // namespace cl

} // namespace zinvul

#include "vector-inl.hpp"

#endif // ZINVUL_VECTOR_HPP
