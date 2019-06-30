/*!
  \file relational.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
Vector<Config::ComparisonResultType<Float>, kN> isequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs != rhs
int32b isnotequal(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs != rhs
int32b isnotequal(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs != rhs
template <typename Float, std::size_t kN>
Vector<Config::ComparisonResultType<Float>, kN> isnotequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs > rhs
int32b isgreater(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs > rhs
int32b isgreater(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs > rhs
template <typename Float, std::size_t kN>
Vector<Config::ComparisonResultType<Float>, kN> isgreater(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs >= rhs
int32b isgreaterequal(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs >= rhs
int32b isgreaterequal(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs >= rhs
template <typename Float, std::size_t kN>
Vector<Config::ComparisonResultType<Float>, kN> isgreaterequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs < rhs
int32b isless(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs < rhs
int32b isless(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs < rhs
template <typename Float, std::size_t kN>
Vector<Config::ComparisonResultType<Float>, kN> isless(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept;

//! Return the component-wize compare of lhs <= rhs
int32b islessequal(const float lhs, const float rhs) noexcept;

//! Return the component-wize compare of lhs <= rhs
int32b islessequal(const double lhs, const double rhs) noexcept;

//! Return the component-wize compare of lhs <= rhs
template <typename Float, std::size_t kN>
Vector<Config::ComparisonResultType<Float>, kN> islessequal(
    const Vector<Float, kN>& lhs,
    const Vector<Float, kN>& rhs) noexcept;

//! Determin if the given floating point is a positive or negative infinity
int32b isinf(const float value) noexcept;

//! Determin if the given floating point is a positive or negative infinity
int32b isinf(const double value) noexcept;

//! Determin if the given floating point is a positive or negative infinity
template <typename Float, std::size_t kN>
Vector<Config::ComparisonResultType<Float>, kN> isinf(
    const Vector<Float, kN>& value) noexcept;

//! Determin if the given floating point is a Nan
int32b isnan(const float value) noexcept;

//! Determin if the given floating point is a Nan
int32b isnan(const double value) noexcept;

//! Determin if the given floating point is a Nan
template <typename Float, std::size_t kN>
Vector<Config::ComparisonResultType<Float>, kN> isnan(
    const Vector<Float, kN>& value) noexcept;

//! Determin if the given floating point is negative
int32b signbit(const float value) noexcept;

//! Determin if the given floating point is negative
int32b signbit(const double value) noexcept;

//! Determin if the given floating point is negative
template <typename Float, std::size_t kN>
Vector<Config::ComparisonResultType<Float>, kN> signbit(
    const Vector<Float, kN>& value) noexcept;

//! Each component of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
template <typename TypeN>
TypeN bitselect(const TypeN& a, const TypeN& b, const TypeN& c) noexcept;

//! Each component of the result is set to c[i] ? b[i] : a[i]
template <typename TypeN, typename IntegerN>
TypeN select(const TypeN& a, const TypeN& b, const IntegerN& c) noexcept;

} // namespace cl

} // namespace zinvul

#include "relational-inl.hpp"

#endif // ZINVUL_CL_RELATIONAL_HPP
