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
#include <cmath>
#include <type_traits>
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

// Float constants
constexpr float MAXFLOAT = std::numeric_limits<float>::max();

//! Return |x|
template <typename Integer>
auto abs(const Integer x) noexcept;

//! Return |x|
template <typename Type>
Type fabs(const Type x) noexcept;

//! Return min(max(x, minval, maxval))
template <typename Type>
Type clamp(const Type x, const Type minval, const Type maxval) noexcept;

//! Return y if x < y, otherwise it returns x
template <typename Type>
Type max(const Type x, const Type y) noexcept;

//! Return y if y < x, otherwise it returns x
template <typename Type>
Type min(const Type x, const Type y) noexcept;

} // namespace cl

} // namespace zinvul

#include "math-inl.hpp"

#endif // ZINVUL_CL_MATH_HPP
