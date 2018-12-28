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
#ifndef MAXFLOAT
constexpr float MAXFLOAT = std::numeric_limits<float>::max();
#endif // MAXFLOAT
constexpr float M_E_F = zisc::constant::exp(1.0f);
constexpr float M_LOG2E_F = zisc::constant::log2(M_E_F);
constexpr float M_LOG10E_F = zisc::constant::log10(M_E_F);
constexpr float M_LN2_F = zisc::constant::log(2.0f);
constexpr float M_LN10_F = zisc::constant::log(10.0f);
constexpr float M_PI_F = zisc::constant::pi<float>();
constexpr float M_PI_2_F = M_PI_F / 2.0f;
constexpr float M_PI_4_F = M_PI_F / 4.0f;
constexpr float M_1_PI_F = 1.0f / M_PI_F;
constexpr float M_2_PI_F = 2.0f / M_PI_F;
constexpr float M_2_SQRTPI_F = 2.0f / zisc::constant::sqrt(M_PI_F);
constexpr float M_SQRT2_F = zisc::constant::sqrt(2.0f);
constexpr float M_SQRT1_2_F = 1.0f / zisc::constant::sqrt(2.0f);

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

//! Return y if x < y, otherwise it returns x
template <typename Type>
Type max(const Type& x, const Type& y) noexcept;

//! Return y if y < x, otherwise it returns x
template <typename Type>
Type min(const Type& x, const Type& y) noexcept;

//! Convert degrees to radians. (pi / 180) * degrees
template <typename Type>
Type radians(const Type& d) noexcept;

// Floating point manipulation functions

//! Decompose a number into significand and power of 2
template <typename FloatN, typename IntegerN>
FloatN frexp(const FloatN& x, IntegerN* e) noexcept;

} // namespace cl

} // namespace zinvul

#include "math-inl.hpp"

#endif // ZINVUL_CL_MATH_HPP
