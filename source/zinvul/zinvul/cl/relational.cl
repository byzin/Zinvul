/*!
  \file relational.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_RELATIONAL_CL
#define ZINVUL_RELATIONAL_CL

// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  \brief Relational functions
  */
class Relation
{
 public:
  //! Return the component-wise compare of x == y
  template <typename FloatN>
  static ComparisonResultType<FloatN> isEqual(const FloatN x,
                                              const FloatN y) noexcept;

  //! Return the component-wise compare of x != y
  template <typename FloatN>
  static ComparisonResultType<FloatN> isNotEqual(const FloatN x,
                                                 const FloatN y) noexcept;

  //! Return the component-wise compare of x > y
  template <typename FloatN>
  static ComparisonResultType<FloatN> isGreater(const FloatN x,
                                                const FloatN y) noexcept;

  //! Return the component-wise compare of x >= y
  template <typename FloatN>
  static ComparisonResultType<FloatN> isGreaterEqual(const FloatN x,
                                                     const FloatN y) noexcept;

  //! Return the component-wise compare of x < y
  template <typename FloatN>
  static ComparisonResultType<FloatN> isLess(const FloatN x,
                                             const FloatN y) noexcept;

  //! Return the component-wise compare of x <= y
  template <typename FloatN>
  static ComparisonResultType<FloatN> isLessEqual(const FloatN x,
                                                  const FloatN y) noexcept;

  //! Determin if the given floating point is a positive or negative infinity
  template <typename FloatN>
  static ComparisonResultType<FloatN> isInf(const FloatN arg) noexcept;

  //! Determin if the given floating point is a NaN
  template <typename FloatN>
  static ComparisonResultType<FloatN> isNan(const FloatN arg) noexcept;

  //! Determin if the given floating point is negative
  template <typename FloatN>
  static ComparisonResultType<FloatN> isSignBitSet(const FloatN arg) noexcept;

  //! Each bit of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
  template <typename TypeN>
  static TypeN selectBit(const TypeN a, const TypeN b, const TypeN c) noexcept;

  //! Each component of the result is set to c[i] ? b[i] : a[i]
  template <typename TypeN, typename IntegerN>
  static TypeN selectValue(const TypeN a, const TypeN b, const IntegerN c) noexcept;
};

// OpenCL function aliases

//! Return the component-wise compare of x == y
template <typename FloatN>
ComparisonResultType<FloatN> isequal(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x != y
template <typename FloatN>
ComparisonResultType<FloatN> isnotequal(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x > y
template <typename FloatN>
ComparisonResultType<FloatN> isgreater(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x >= y
template <typename FloatN>
ComparisonResultType<FloatN> isgreaterequal(const FloatN x,
                                            const FloatN y) noexcept;

//! Return the component-wise compare of x < y
template <typename FloatN>
ComparisonResultType<FloatN> isless(const FloatN x, const FloatN y) noexcept;

//! Return the component-wise compare of x <= y
template <typename FloatN>
ComparisonResultType<FloatN> islessequal(const FloatN x,
                                         const FloatN y) noexcept;

//! Determin if the given floating point is a positive or negative infinity
template <typename FloatN>
ComparisonResultType<FloatN> isinf(const FloatN arg) noexcept;

//! Determin if the given floating point is a NaN
template <typename FloatN>
ComparisonResultType<FloatN> isnan(const FloatN arg) noexcept;

//! Determin if the given floating point is negative
template <typename FloatN>
ComparisonResultType<FloatN> signbit(const FloatN arg) noexcept;

//! Each bit of the result is set to (a[i] ^ c[i]) | (b[i] & c[i])
template <typename TypeN>
TypeN bitselect(const TypeN a, const TypeN b, const TypeN c) noexcept;

//! Each component of the result is set to c[i] ? b[i] : a[i]
template <typename TypeN, typename IntegerN>
TypeN select(const TypeN a, const TypeN b, const IntegerN c) noexcept;

} // namespace zinvul

#include "relational-inl.cl"

#endif /* ZINVUL_RELATIONAL_CL */
