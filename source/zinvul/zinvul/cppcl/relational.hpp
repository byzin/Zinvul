/*!
  \file relational.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_RELATIONAL_HPP
#define ZINVUL_CL_RELATIONAL_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/utility.hpp"
// Zinvul
#include "types.hpp"
#include "vector.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

//! Return the component-wize compare of lhs == rhs
int32b isequal(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs == rhs
int32b isequal(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs == rhs
template <typename Float, std::size_t kN>
Vector<int32b, kN> isequal(const Vector<Float, kN>& lhs,
                           const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs != rhs
int32b isnotequal(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs != rhs
int32b isnotequal(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs != rhs
template <typename Float, std::size_t kN>
Vector<int32b, kN> isnotequal(const Vector<Float, kN>& lhs,
                              const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs > rhs
int32b isgreater(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs > rhs
int32b isgreater(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs > rhs
template <typename Float, std::size_t kN>
Vector<int32b, kN> isgreater(const Vector<Float, kN>& lhs,
                             const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs >= rhs
int32b isgreaterequal(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs >= rhs
int32b isgreaterequal(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs >= rhs
template <typename Float, std::size_t kN>
Vector<int32b, kN> isgreaterequal(const Vector<Float, kN>& lhs,
                                  const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs < rhs
int32b isless(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs < rhs
int32b isless(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs < rhs
template <typename Float, std::size_t kN>
Vector<int32b, kN> isless(const Vector<Float, kN>& lhs,
                          const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs <= rhs
int32b islessequal(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs <= rhs
int32b islessequal(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs <= rhs
template <typename Float, std::size_t kN>
Vector<int32b, kN> islessequal(const Vector<Float, kN>& lhs,
                               const Vector<Float, kN>& rhs) noexcept;

//! Test for infinity value
int32b isinf(const float value) noexcept;

//! Test for infinity value
int32b isinf(const double value) noexcept;

//! Test for infinity value
template <typename Float, std::size_t kN>
Vector<int32b, kN> isinf(const Vector<Float, kN>& value) noexcept;

//! Test for a NaN
int32b isnan(const float value) noexcept;

//! Test for a NaN
int32b isnan(const double value) noexcept;

//! Test for a NaN
template <typename Float, std::size_t kN>
Vector<int32b, kN> isnan(const Vector<Float, kN>& value) noexcept;

//! Test for sign bit
int32b signbit(const float value) noexcept;

//! Test for sign bit
int32b signbit(const double value) noexcept;

//! Test for sign bit
template <typename Float, std::size_t kN>
Vector<int32b, kN> signbit(const Vector<Float, kN>& value) noexcept;

} // namespace cl

} // namespace zinvul

#include "relational-inl.hpp"

#endif // ZINVUL_CL_RELATIONAL_HPP
