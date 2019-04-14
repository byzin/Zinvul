/*!
  \file math.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_MATH_HPP
#define ZINVUL_CL_MATH_HPP

// Standard C++ library
#include <climits>
#include <cmath>
#include <type_traits>
// Zisc
#include "zisc/const_math.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

// Float constants
#if !defined(MAXFLOAT)
constexpr float MAXFLOAT = std::numeric_limits<float>::max();
#endif // MAXFLOAT
constexpr float M_E_F = 2.718281746e+00f;
constexpr float M_LOG2E_F = 1.442695022e+00f;
constexpr float M_LOG10E_F = 4.342944920e-01f;
constexpr float M_LN2_F = 6.931471825e-01f;
constexpr float M_LN10_F = 2.302585125e+00f;
constexpr float M_PI_F = 3.141592741e+00f;
constexpr float M_PI_2_F = M_PI_F / 2.0f;
constexpr float M_PI_4_F = M_PI_F / 4.0f;
constexpr float M_1_PI_F = 1.0f / M_PI_F;
constexpr float M_2_PI_F = 2.0f / M_PI_F;
constexpr float M_2_SQRTPI_F = 1.128379226e+00f;
constexpr float M_SQRT2_F = 1.414213538e+00f;
constexpr float M_SQRT1_2_F = 1.0f / M_SQRT2_F;

//! Return 1 if x > 0, -1 if x < 0, otherwise 0
template <typename FloatN>
FloatN sign(const FloatN& x) noexcept;

// Nearest integer floating point operations

//! Return the nearest integer not less than the given value
template <typename FloatN>
FloatN ceil(const FloatN& x) noexcept;

//! Return the nearest integer not greater than the given value
template <typename FloatN>
FloatN floor(const FloatN& x) noexcept;

//! Return the nearest integer not greater in magnitude than the given value
template <typename FloatN>
FloatN trunc(const FloatN& x) noexcept;

//! Return the nearest integer, rounding away from zero in halfway cases
template <typename FloatN>
FloatN round(const FloatN& x) noexcept;

// Basic operations

//! Return |x|
template <typename IntegerN>
auto abs(const IntegerN& x) noexcept;

//! Return |x|
template <typename FloatN>
FloatN fabs(const FloatN& x) noexcept;

//! Return the remainder of the floating point division operation by 1
template <typename FloatN>
FloatN fract(const FloatN& x, FloatN* iptr) noexcept;

//! Return the remainder of the floating point division operation
template <typename FloatN>
FloatN fmod(const FloatN& x, const FloatN& y) noexcept;

//! Return min(max(x, minval), maxval)
template <typename Type1N, typename Type2N>
Type1N clamp(const Type1N& x, const Type2N& minval, const Type2N& maxval) noexcept;

//! Convert radians to degrees. (180 / pi) * radians
template <typename FloatN>
FloatN degrees(const FloatN& r) noexcept;

//! Fused multiply-add operation
template <typename FloatN>
FloatN fma(const FloatN& a, const FloatN& b, const FloatN& c) noexcept;

//! Return y if x < y, otherwise it returns x
template <typename TypeN>
TypeN max(const TypeN& x, const TypeN& y) noexcept;

//! Return y if y < x, otherwise it returns x
template <typename TypeN>
TypeN min(const TypeN& x, const TypeN& y) noexcept;

//! Return the linear blend of x & y implemented as: x + (y - x) * a
template <typename TypeN1, typename TypeN2>
TypeN1 mix(const TypeN1& x, const TypeN1& y, const TypeN2& a) noexcept;

//! Convert degrees to radians. (pi / 180) * degrees
template <typename FloatN>
FloatN radians(const FloatN& d) noexcept;

// Exponential functions

//! Return e raised to the given power
template <typename FloatN>
FloatN exp(const FloatN& x) noexcept;

//! Compute natural logarithm of the given number
template <typename FloatN>
FloatN log(const FloatN& x) noexcept;

//! Compute base2 logarithm of the given number
template <typename FloatN>
FloatN log2(const FloatN& x) noexcept;

// Power functions

//! Raise a number to the given power
template <typename FloatN>
FloatN pow(const FloatN& base, const FloatN& e) noexcept;

//! Compute inverse square root
template <typename FloatN>
FloatN rsqrt(const FloatN& x) noexcept;

//! Compute square root
template <typename FloatN>
FloatN sqrt(const FloatN& x) noexcept;

//! Trigonometric functions

//! Compute sine
template <typename FloatN>
FloatN sin(const FloatN& theta) noexcept;

//! Compute cosine
template <typename FloatN>
FloatN cos(const FloatN& theta) noexcept;

//! Compute sine and cosine
template <typename FloatN>
FloatN sincos(const FloatN& theta, FloatN* cosval) noexcept;

//! Compute tangent
template <typename FloatN>
FloatN tan(const FloatN& theta) noexcept;

//! Compute arc sine
template <typename FloatN>
FloatN asin(const FloatN& x) noexcept;

//! Compute arc cosine
template <typename FloatN>
FloatN acos(const FloatN& x) noexcept;

//! Compute arc tangent
template <typename FloatN>
FloatN atan(const FloatN& x) noexcept;

// Floating point manipulation functions

//! Decompose a number into significand and power of 2
template <typename FloatN, typename IntegerN>
FloatN frexp(const FloatN& x, IntegerN* e) noexcept;

//! Multiplie a number by 2 raised to a power 
template <typename FloatN, typename IntegerN>
FloatN ldexp(const FloatN& x, const IntegerN& e) noexcept;

// Integer

//! Return the number of leading 0-bits
template <typename IntegerN>
IntegerN clz(const IntegerN& x) noexcept;

//! Return the number of non-zero bits
template <typename IntegerN>
IntegerN popcount(const IntegerN& x) noexcept;

// Geometric functions

//! Return the cross product of p0.xyz and p1.xyz
template <typename FloatN>
FloatN cross(const FloatN& p0, const FloatN& p1) noexcept;

//! Compute dot product
template <typename FloatN>
float dot(const FloatN& p0, const FloatN& p1) noexcept;

//! Return the distance between p0 and p1
template <typename FloatN>
float distance(const FloatN& p0, const FloatN& p1) noexcept;

//! Return the length of vector p
template <typename FloatN>
float length(const FloatN& p) noexcept;

//! Return a vector in the same direction as p but with a length of 1
template <typename FloatN>
FloatN normalize(const FloatN& p) noexcept;

} // namespace cl

} // namespace zinvul

#include "math-inl.hpp"

#endif // ZINVUL_CL_MATH_HPP
