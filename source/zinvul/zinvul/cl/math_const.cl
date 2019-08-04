/*!
  \file math_const.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_CONST_CL
#define ZINVUL_MATH_CONST_CL

// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

namespace mathconst {

/*!
 \brief constexpr math functions
 */
class Math
{
 public:
  // Constexpr values

  //! Return circular constant
  template <typename Float>
  static constexpr Float pi() noexcept;

  // Basic operations

  //! Return the absolute value of the given value
  template <typename Arith>
  static constexpr Arith absolute(const Arith x) noexcept;

  //! Fused multiply-add operation
  template <typename Arith>
  static constexpr Arith fusedMulAdd(const Arith a,
                                     const Arith b,
                                     const Arith c) noexcept;

  //! Invert the given value
  template <typename Float>
  static constexpr Float inverse(const Float x) noexcept;

  //! Check whether the given values is negative
  template <typename Arith>
  static constexpr ComparisonResultType<Arith> isNegative(const Arith x) noexcept;

  //! Check whether the given value is odd
  template <typename Integer>
  static constexpr ComparisonResultType<Integer> isOdd(const Integer x) noexcept;

  // Power functions

  //! Raise a number to the given power
  template <int32b kExponent, typename Arith>
  static constexpr Arith power(Arith base) noexcept;

  //! Compute square root
  template <typename Float>
  static constexpr Float squareRoot(const Float x) noexcept;

  //! Compute cubic root
  template <typename Float>
  static constexpr Float cubicRoot(const Float x) noexcept;

  // Floating point manipulation

  //! Return the ULPs of the given value
  template <size_t kUlpScale = 2, typename Float>
  static constexpr Float getUlps(const Float x) noexcept;

  //! Check whether the given two values are almost same
  template <size_t kUlpScale = 2, typename Arith>
  static constexpr ComparisonResultType<Arith> isAlmostEqual(
      const Arith lhs,
      const Arith rhs) noexcept;

 private:
  // Constexpr values

  //! Calculate circular constant
  template <typename Float>
  static constexpr Float calcPi(const int32b n) noexcept;
};

// CL function aliases

// Basic operations

//! Return the absolute value of the given value
template <typename Arith>
constexpr Arith abs(const Arith x) noexcept;

//! Fused multiply-add operation
template <typename Arith>
constexpr Arith fma(const Arith a, const Arith b, const Arith c) noexcept;

// Power functions

//! Raise a number to the given power
template <int32b kExponent, typename Arith>
constexpr Arith power(Arith base) noexcept;

//! Compute square root
template <typename Float>
constexpr Float sqrt(const Float x) noexcept;

//! Compute cubic root
template <typename Float>
constexpr Float cbrt(const Float x) noexcept;

// Floating point manipulation

//! Decompose given floating point value
//template <typename Float>
//constexpr Float frexp(const Float x, PrivatePtr<int32b> e) noexcept;

//! Multiply a floating point value x by the number 2 raised to the power
//template <typename Float>
//constexpr Float ldexp(const Float x, const int32b e) noexcept;

} // namespace mathconst

} // namespace zinvul

#include "math_const-inl.cl"

#endif /* ZINVUL_MATH_CONST_CL */
