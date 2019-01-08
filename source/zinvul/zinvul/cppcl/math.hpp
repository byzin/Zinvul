/*!
  \file math.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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

// Basic operations

//! Return |x|
template <typename Integer>
auto abs(const Integer& x) noexcept;

//! Return |x|
template <typename Type>
Type fabs(const Type& x) noexcept;

//! Return min(max(x, minval), maxval)
template <typename Type>
Type clamp(const Type& x, const Type& minval, const Type& maxval) noexcept;

//! Convert radians to degrees. (180 / pi) * radians
template <typename Type>
Type degrees(const Type& r) noexcept;

//! Fused multiply-add operation
template <typename FloatN>
FloatN fma(const FloatN& a, const FloatN& b, const FloatN& c) noexcept;

//! Return y if x < y, otherwise it returns x
template <typename Type>
Type max(const Type& x, const Type& y) noexcept;

//! Return y if y < x, otherwise it returns x
template <typename Type>
Type min(const Type& x, const Type& y) noexcept;

//! Convert degrees to radians. (pi / 180) * degrees
template <typename Type>
Type radians(const Type& d) noexcept;

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

} // namespace cl

} // namespace zinvul

#include "math-inl.hpp"

#endif // ZINVUL_CL_MATH_HPP
