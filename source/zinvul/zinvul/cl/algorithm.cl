/*!
  \file algorithm.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ALGORITHM_CL
#define ZINVUL_ALGORITHM_CL

// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  */
class Algorithm
{
 public:
  //! Return the absolute value of the given value
  template <typename ArithN>
  static auto absolute(const ArithN x) noexcept;

  //! Clamp a value between a pair of boundary values
  template <typename Arith1N, typename Arith2N>
  static Arith1N clamp(const Arith1N x,
                       const Arith2N minval,
                       const Arith2N maxval) noexcept;

  //! Return an inverse number
  template <typename FloatN>
  static FloatN invert(const FloatN x) noexcept;

  //! Test whether the given x is negative
  template <typename ArithN>
  static ComparisonResultType<ArithN> isNegative(const ArithN x) noexcept;

  //! Test whether the given x is odd 
  template <typename IntegerN>
  static ComparisonResultType<IntegerN> isOdd(const IntegerN x) noexcept;

  //! Test whether the given x is odd 
  template <typename IntegerN>
  static ComparisonResultType<IntegerN> isPowerOf2(const IntegerN x) noexcept;

  //! Return the greater of the given values
  template <typename ArithN>
  static ArithN max(const ArithN x, const ArithN y) noexcept;

  //! Return the smaller of the given values
  template <typename ArithN>
  static ArithN min(const ArithN x, const ArithN y) noexcept;

  //! Return the linear blend of x & y implemented as: x + (y - x) * a
  template <typename Float1N, typename Float2N>
  static Float1N mix(const Float1N x, const Float1N y, const Float2N a) noexcept;

  //! Return 1 if x > 0, -1 if x < 0, otherwise 0
  template <typename FloatN>
  static FloatN sign(const FloatN x) noexcept;

  // Integer
  //! Return the number of non-zero bits in the given x
  template <typename IntegerN>
  static IntegerN popcount(const IntegerN x) noexcept;
};

// OpenCL style function aliases

//! Return the absolute value of the given value
template <typename ArithN>
auto abs(const ArithN x) noexcept;

//! Clamp a value between a pair of boundary values
template <typename Arith1N, typename Arith2N>
Arith1N clamp(const Arith1N x, const Arith2N minval, const Arith2N maxval) noexcept;

//! Return the greater of the given values
template <typename ArithN>
ArithN max(const ArithN x, const ArithN y) noexcept;

//! Return the smaller of the given values
template <typename ArithN>
ArithN min(const ArithN x, const ArithN y) noexcept;

//! Return the linear blend of x & y implemented as: x + (y - x) * a
template <typename Float1N, typename Float2N>
Float1N mix(const Float1N x, const Float1N y, const Float2N a) noexcept;

//! Return the smaller of the given values
template <typename IntegerN>
IntegerN popcount(const IntegerN x) noexcept;

//! Return 1 if x > 0, -1 if x < 0, otherwise 0
template <typename FloatN>
FloatN sign(const FloatN x) noexcept;

} // namespace zinvul

#include "algorithm-inl.cl"

#endif /* ZINVUL_ALGORITHM_CL */
