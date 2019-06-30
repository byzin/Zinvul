/*!
  \file relational.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_RELATIONAL_CL
#define ZINVUL_RELATIONAL_CL

// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

//! Return the component-wise compare of x == y
template <typename FloatN>
ComparisonResultType<FloatN> isEqual(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x != y
template <typename FloatN>
ComparisonResultType<FloatN> isNotEqual(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x > y
template <typename FloatN>
ComparisonResultType<FloatN> isGreater(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x >= y
template <typename FloatN>
ComparisonResultType<FloatN> isGreaterEqual(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x < y
template <typename FloatN>
ComparisonResultType<FloatN> isLess(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x <= y
template <typename FloatN>
ComparisonResultType<FloatN> isLessEqual(const FloatN x, const FloatN y) noexcept;

//! Determin if the given floating point is a positive or negative infinity
template <typename FloatN>
ComparisonResultType<FloatN> isInf(const FloatN arg) noexcept;

//! Determin if the given floating point is a NaN
template <typename FloatN>
ComparisonResultType<FloatN> isNan(const FloatN arg) noexcept;

//! Determin if the given floating point is negative
template <typename FloatN>
ComparisonResultType<FloatN> isSignBitSet(const FloatN arg) noexcept;

//! Each bit of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
template <typename TypeN>
TypeN selectBit(const TypeN a, const TypeN b, const TypeN c) noexcept;

//! Each component of the result is set to c[i] ? b[i] : a[i]
template <typename TypeN, typename IntegerN>
TypeN selectValue(const TypeN a, const TypeN b, const IntegerN c) noexcept;

} // namespace zinvul

#include "relational-inl.cl"

#endif /* ZINVUL_RELATIONAL_CL */
