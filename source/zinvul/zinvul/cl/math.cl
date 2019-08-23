/*!
  \file math.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_CL
#define ZINVUL_MATH_CL

// Zinvul
#include "math_const.cl"
#include "types.cl"

namespace zinvul {

/*!
  */
class Math
{
 public:
  // Nearest integer floating point operations

  //! Return the nearest integer not less than the given value
  template <typename FloatN>
  static FloatN ceil(const FloatN x) noexcept;

  //! Return the nearest integer not greater than the given value
  template <typename FloatN>
  static FloatN floor(const FloatN x) noexcept;

  //! Return the nearest integer not greater in magnitude than the given value
  template <typename FloatN>
  static FloatN trunc(const FloatN x) noexcept;

  //! Return the nearest integer, rounding away from zero in halfway cases
  template <typename FloatN>
  static FloatN round(const FloatN x) noexcept;

  //! Round to integral value
  template <typename FloatN>
  static FloatN rint(const FloatN x) noexcept;

  // Basic operations

  //! Return the remainder of the floating point division operation by 1
  template <typename FloatN, typename FloatNPtr>
  static FloatN fract(const FloatN x, FloatNPtr iptr) noexcept;

  //! Return the remainder of the floating point division operation
  template <typename FloatN>
  static FloatN fmod(const FloatN x, const FloatN y) noexcept;

  //! Fused multiply-add operation
  template <typename FloatN>
  static FloatN fma(const FloatN a, const FloatN b, const FloatN c) noexcept;

  // Exponential functions

  //! Return e raised to the given power
  template <typename FloatN>
  static FloatN exp(const FloatN x) noexcept;

  //! Return 2 raised to the given power
  template <typename FloatN>
  static FloatN exp2(const FloatN x) noexcept;

  //! Compute natural logarithm of the given number
  template <typename FloatN>
  static FloatN log(const FloatN x) noexcept;

  //! Compute base2 logarithm of the given number
  template <typename FloatN>
  static FloatN log2(const FloatN x) noexcept;

  // Power functions

  //! Raise a number to the given power
  template <typename FloatN>
  static FloatN pow(const FloatN base, const FloatN e) noexcept;

  //! Compute inverse square root
  template <typename FloatN>
  static FloatN rsqrt(const FloatN x) noexcept;

  //! Compute square root
  template <typename FloatN>
  static FloatN sqrt(const FloatN x) noexcept;

  //! Compute cubic root
  template <typename FloatN>
  static FloatN cbrt(const FloatN x) noexcept;

  // Trigonometric functions

  //! Compute sine
  template <typename FloatN>
  static FloatN sin(const FloatN theta) noexcept;

  //! Compute cosine
  template <typename FloatN>
  static FloatN cos(const FloatN theta) noexcept;

  //! Compute tangent
  template <typename FloatN>
  static FloatN tan(const FloatN theta) noexcept;

  //! Compute arc sine
  template <typename FloatN>
  static FloatN asin(const FloatN x) noexcept;

  //! Compute arc cosine
  template <typename FloatN>
  static FloatN acos(const FloatN x) noexcept;

  //! Compute arc tangent
  template <typename FloatN>
  static FloatN atan(const FloatN x) noexcept;

  // Floating point manipulation functions

  //! Decompose a number into significand and power of 2
  template <typename FloatN, typename IntegerNPtr>
  static FloatN frexp(const FloatN x, IntegerNPtr e) noexcept;

  //! Multiply a number by 2 raised to a power
  template <typename FloatN, typename IntegerN>
  static FloatN ldexp(const FloatN x, const IntegerN e) noexcept;

  //! Copy the sign of a floating point value
  template <typename FloatN>
  static FloatN copysign(const FloatN x, const FloatN y) noexcept;

  /*!
    */
  class Builtin
  {
   public:
    // Nearest integer floating point operations

    //! Return the nearest integer not less than the given value
    template <typename FloatN>
    static FloatN ceil(const FloatN x) noexcept;

    //! Return the nearest integer not greater than the given value
    template <typename FloatN>
    static FloatN floor(const FloatN x) noexcept;

    //! Return the nearest integer not greater in magnitude than the given value
    template <typename FloatN>
    static FloatN trunc(const FloatN x) noexcept;

    //! Return the nearest integer, rounding away from zero in halfway cases
    template <typename FloatN>
    static FloatN round(const FloatN x) noexcept;

    //! Round to integral value
    template <typename FloatN>
    static FloatN rint(const FloatN x) noexcept;

    // Basic operations

    //! Return the remainder of the floating point division operation by 1
    template <typename FloatN, typename FloatNPtr>
    static FloatN fract(const FloatN x, FloatNPtr iptr) noexcept;

    //! Return the remainder of the floating point division operation
    template <typename FloatN>
    static FloatN fmod(const FloatN x, const FloatN y) noexcept;

    //! Fused multiply-add operation
    template <typename FloatN>
    static FloatN fma(const FloatN a, const FloatN b, const FloatN c) noexcept;

    // Exponential functions

    //! Return e raised to the given power
    template <typename FloatN>
    static FloatN exp(const FloatN x) noexcept;

    //! Return 2 raised to the given power
    template <typename FloatN>
    static FloatN exp2(const FloatN x) noexcept;

    //! Compute natural logarithm of the given number
    template <typename FloatN>
    static FloatN log(const FloatN x) noexcept;

    //! Compute base2 logarithm of the given number
    template <typename FloatN>
    static FloatN log2(const FloatN x) noexcept;

    // Power functions

    //! Raise a number to the given power
    template <typename FloatN>
    static FloatN pow(const FloatN base, const FloatN e) noexcept;

    //! Compute inverse square root
    template <typename FloatN>
    static FloatN rsqrt(const FloatN x) noexcept;

    //! Compute square root
    template <typename FloatN>
    static FloatN sqrt(const FloatN x) noexcept;

    //! Compute cubic root
    template <typename FloatN>
    static FloatN cbrt(const FloatN x) noexcept;

    // Trigonometric functions

    //! Compute sine
    template <typename FloatN>
    static FloatN sin(const FloatN theta) noexcept;

    //! Compute cosine
    template <typename FloatN>
    static FloatN cos(const FloatN theta) noexcept;

    //! Compute tangent
    template <typename FloatN>
    static FloatN tan(const FloatN theta) noexcept;

    //! Compute arc sine
    template <typename FloatN>
    static FloatN asin(const FloatN x) noexcept;

    //! Compute arc cosine
    template <typename FloatN>
    static FloatN acos(const FloatN x) noexcept;

    //! Compute arc tangent
    template <typename FloatN>
    static FloatN atan(const FloatN x) noexcept;

    // Floating point manipulation functions

    //! Decompose a number into significand and power of 2
    template <typename FloatN, typename IntegerNPtr>
    static FloatN frexp(const FloatN x, IntegerNPtr e) noexcept;

    //! Multiply a number by 2 raised to a power
    template <typename FloatN, typename IntegerN>
    static FloatN ldexp(const FloatN x, const IntegerN e) noexcept;

    //! Copy the sign of a floating point value
    template <typename FloatN>
    static FloatN copysign(const FloatN x, const FloatN y) noexcept;
  };

  /*!
    */
  class Zinvul
  {
   public:
    // Nearest integer floating point operations

    //! Return the nearest integer, rounding away from zero in halfway cases
    template <typename FloatN>
    static FloatN round(const FloatN x) noexcept;

    //! Round to integral value
    template <typename FloatN>
    static FloatN rint(const FloatN x) noexcept;

    // Basic operations

    //! Return the remainder of the floating point division operation by 1
    template <typename FloatN, typename FloatNPtr>
    static FloatN fract(const FloatN x, FloatNPtr iptr) noexcept;

    //! Return the remainder of the floating point division operation
    template <typename FloatN>
    static FloatN fmod(const FloatN x, const FloatN y) noexcept;

    // Exponential functions

    //! Return e raised to the given power
    template <typename FloatN>
    static FloatN exp(const FloatN x) noexcept;

    //! Return 2 raised to the given power
    template <typename FloatN>
    static FloatN exp2(const FloatN x) noexcept;

    //! Compute natural logarithm of the given number
    template <typename FloatN>
    static FloatN log(const FloatN x) noexcept;

    //! Compute base2 logarithm of the given number
    template <typename FloatN>
    static FloatN log2(const FloatN x) noexcept;

    // Power functions

    //! Raise a number to the given power
    template <typename FloatN>
    static FloatN pow(const FloatN base, const FloatN e) noexcept;

    //! Compute inverse square root
    template <typename FloatN>
    static FloatN rsqrt(const FloatN x) noexcept;

    //! Compute square root
    template <typename FloatN>
    static FloatN sqrt(const FloatN x) noexcept;

    //! Compute cubic root
    template <typename FloatN>
    static FloatN cbrt(const FloatN x) noexcept;

    // Trigonometric functions

    //! Compute sine
    template <typename FloatN>
    static FloatN sin(const FloatN theta) noexcept;

    //! Compute cosine
    template <typename FloatN>
    static FloatN cos(const FloatN theta) noexcept;

    //! Compute tangent
    template <typename FloatN>
    static FloatN tan(const FloatN theta) noexcept;

    //! Compute arc sine
    template <typename FloatN>
    static FloatN asin(const FloatN x) noexcept;

    //! Compute arc cosine
    template <typename FloatN>
    static FloatN acos(const FloatN x) noexcept;

    //! Compute arc tangent
    template <typename FloatN>
    static FloatN atan(const FloatN x) noexcept;

    //! Decompose a number into significand and power of 2
    template <typename FloatN, typename IntegerNPtr>
    static FloatN frexp(const FloatN x, IntegerNPtr e) noexcept;

    //! Multiply a number by 2 raised to a power
    template <typename FloatN, typename IntegerN>
    static FloatN ldexp(const FloatN x, const IntegerN e) noexcept;
  };

 private:
};

// Constant values

//! Circular constant
template <typename Float>
constexpr Constant<Float> kPi = mathconst::Math::pi<Float>();

// OpenCL style function aliases

// Nearest integer floating point operations

//! Return the nearest integer not less than the given value
template <typename FloatN>
FloatN ceil(const FloatN x) noexcept;

//! Return the nearest integer not greater than the given value
template <typename FloatN>
FloatN floor(const FloatN x) noexcept;

//! Return the nearest integer not greater in magnitude than the given value
template <typename FloatN>
FloatN trunc(const FloatN x) noexcept;

//! Return the nearest integer, rounding away from zero in halfway cases
template <typename FloatN>
FloatN round(const FloatN x) noexcept;

//! Round to integral value
template <typename FloatN>
FloatN rint(const FloatN x) noexcept;

// Basic operations

//! Return the remainder of the floating point division operation by 1
template <typename FloatN, typename FloatNPtr>
FloatN fract(const FloatN x, FloatNPtr iptr) noexcept;

//! Return the remainder of the floating point division operation
template <typename FloatN>
FloatN fmod(const FloatN x, const FloatN y) noexcept;

//! Fused multiply-add operation
template <typename FloatN>
FloatN fma(const FloatN a, const FloatN b, const FloatN c) noexcept;

// Exponential functions

//! Return e raised to the given power
template <typename FloatN>
FloatN exp(const FloatN x) noexcept;

//! Return 2 raised to the given power
template <typename FloatN>
FloatN exp2(const FloatN x) noexcept;

//! Compute natural logarithm of the given number
template <typename FloatN>
FloatN log(const FloatN x) noexcept;

//! Compute base2 logarithm of the given number
template <typename FloatN>
FloatN log2(const FloatN x) noexcept;

// Power functions

//! Raise a number to the given power
template <typename FloatN>
FloatN pow(const FloatN base, const FloatN e) noexcept;

//! Compute inverse square root
template <typename FloatN>
FloatN rsqrt(const FloatN x) noexcept;

//! Compute square root
template <typename FloatN>
FloatN sqrt(const FloatN x) noexcept;

//! Compute cubic root
template <typename FloatN>
FloatN cbrt(const FloatN x) noexcept;

// Trigonometric functions

//! Compute sine
template <typename FloatN>
FloatN sin(const FloatN theta) noexcept;

//! Compute cosine
template <typename FloatN>
FloatN cos(const FloatN theta) noexcept;

//! Compute tangent
template <typename FloatN>
FloatN tan(const FloatN theta) noexcept;

//! Compute arc sine
template <typename FloatN>
FloatN asin(const FloatN x) noexcept;

//! Compute arc cosine
template <typename FloatN>
FloatN acos(const FloatN x) noexcept;

//! Compute arc tangent
template <typename FloatN>
FloatN atan(const FloatN x) noexcept;

// Floating point manipulation functions

//! Decompose a number into significand and power of 2
template <typename FloatN, typename IntegerNPtr>
FloatN frexp(const FloatN x, IntegerNPtr e) noexcept;

//! Multiply a number by 2 raised to a power
template <typename FloatN, typename IntegerN>
FloatN ldexp(const FloatN x, const IntegerN e) noexcept;

//! Copy the sign of a floating point value
template <typename FloatN>
FloatN copysign(const FloatN x, const FloatN y) noexcept;

} // namespace zinvul

#include "math-inl.cl"

#endif /* ZINVUL_MATH_CL */
