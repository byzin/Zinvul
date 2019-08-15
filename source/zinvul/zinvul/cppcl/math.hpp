/*!
  \file math.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "vector.hpp"
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

/*!
  */
class Math
{
 public:
  // Nearest integer floating point operations

  //! Return the nearest integer not less than the given value
  template <typename FloatN>
  static FloatN ceil(const FloatN& x) noexcept;

  //! Return the nearest integer not greater than the given value
  template <typename FloatN>
  static FloatN floor(const FloatN& x) noexcept;

  //! Return the nearest integer not greater in magnitude than the given value
  template <typename FloatN>
  static FloatN trunc(const FloatN& x) noexcept;

  //! Return the nearest integer, rounding away from zero in halfway cases
  template <typename FloatN>
  static FloatN round(const FloatN& x) noexcept;

  // Basic operations

  //! Return the remainder of the floating point division operation by 1
  template <typename FloatN>
  static FloatN fract(const FloatN& x, FloatN* iptr) noexcept;

  //! Return the remainder of the floating point division operation
  template <typename FloatN>
  static FloatN fmod(const FloatN& x, const FloatN& y) noexcept;

  //! Fused multiply-add operation
  template <typename FloatN>
  static FloatN fma(const FloatN& a, const FloatN& b, const FloatN& c) noexcept;

  // Exponential functions

  //! Return e raised to the given power
  template <typename FloatN>
  static FloatN exp(const FloatN& x) noexcept;

  //! Compute natural logarithm of the given number
  template <typename FloatN>
  static FloatN log(const FloatN& x) noexcept;

  //! Compute base2 logarithm of the given number
  template <typename FloatN>
  static FloatN log2(const FloatN& x) noexcept;

  // Power functions

  //! Raise a number to the given power
  template <typename FloatN>
  static FloatN pow(const FloatN& base, const FloatN& e) noexcept;

  //! Compute inverse square root
  template <typename FloatN>
  static FloatN rsqrt(const FloatN& x) noexcept;

  //! Compute square root
  template <typename FloatN>
  static FloatN sqrt(const FloatN& x) noexcept;

  //! Trigonometric functions

  //! Compute sine
  template <typename FloatN>
  static FloatN sin(const FloatN& theta) noexcept;

  //! Compute cosine
  template <typename FloatN>
  static FloatN cos(const FloatN& theta) noexcept;

  //! Compute sine and cosine
  template <typename FloatN>
  static FloatN sincos(const FloatN& theta, FloatN* cosval) noexcept;

  //! Compute tangent
  template <typename FloatN>
  static FloatN tan(const FloatN& theta) noexcept;

  //! Compute arc sine
  template <typename FloatN>
  static FloatN asin(const FloatN& x) noexcept;

  //! Compute arc cosine
  template <typename FloatN>
  static FloatN acos(const FloatN& x) noexcept;

  //! Compute arc tangent
  template <typename FloatN>
  static FloatN atan(const FloatN& x) noexcept;

  // Floating point manipulation functions

  //! Decompose a number into significand and power of 2
  template <typename FloatN, typename IntegerN>
  static FloatN frexp(const FloatN& x, IntegerN* e) noexcept;

  //! Multiplie a number by 2 raised to a power 
  template <typename FloatN, typename IntegerN>
  static FloatN ldexp(const FloatN& x, const IntegerN& e) noexcept;

  //! Copy the sign of a floating point value
  template <typename FloatN>
  static FloatN copysign(const FloatN& x, const FloatN& y) noexcept;

 private:
  /*!
    */
  class Vec
  {
   public:
    // Nearest integer floating point operations

    //! Return the nearest integer not less than the given value
    template <typename Float, std::size_t kN>
    static auto ceil(const Vector<Float, kN>& x) noexcept;

    //! Return the nearest integer not greater than the given value
    template <typename Float, std::size_t kN>
    static auto floor(const Vector<Float, kN>& x) noexcept;

    //! Return the nearest integer not greater in magnitude than the given value
    template <typename Float, std::size_t kN>
    static auto trunc(const Vector<Float, kN>& x) noexcept;

    //! Return the nearest integer, rounding away from zero in halfway cases
    template <typename Float, std::size_t kN>
    static auto round(const Vector<Float, kN>& x) noexcept;

    // Basic operations

    //! Return the remainder of the floating point division operation by 1
    template <typename Float, std::size_t kN>
    static auto fract(const Vector<Float, kN>& x,
                      Vector<Float, kN>* iptr) noexcept;

    //! Return the remainder of the floating point division operation
    template <typename Float, std::size_t kN>
    static auto fmod(const Vector<Float, kN>& x,
                     const Vector<Float, kN>& y) noexcept;

    //! Fused multiply-add operation
    template <typename Float, std::size_t kN>
    static auto fma(const Vector<Float, kN>& a,
                    const Vector<Float, kN>& b,
                    const Vector<Float, kN>& c) noexcept;

    // Exponential functions

    //! Return e raised to the given power
    template <typename Float, std::size_t kN>
    static auto exp(const Vector<Float, kN>& x) noexcept;

    //! Compute natural logarithm of the given number
    template <typename Float, std::size_t kN>
    static auto log(const Vector<Float, kN>& x) noexcept;

    //! Compute base2 logarithm of the given number
    template <typename Float, std::size_t kN>
    static auto log2(const Vector<Float, kN>& x) noexcept;

    // Power functions

    //! Raise a number to the given power
    template <typename Float, std::size_t kN>
    static auto pow(const Vector<Float, kN>& base,
                    const Vector<Float, kN>& e) noexcept;

    //! Compute inverse square root
    template <typename Float, std::size_t kN>
    static auto rsqrt(const Vector<Float, kN>& x) noexcept;

    //! Compute square root
    template <typename Float, std::size_t kN>
    static auto sqrt(const Vector<Float, kN>& x) noexcept;

    //! Trigonometric functions

    //! Compute sine
    template <typename Float, std::size_t kN>
    static auto sin(const Vector<Float, kN>& theta) noexcept;

    //! Compute cosine
    template <typename Float, std::size_t kN>
    static auto cos(const Vector<Float, kN>& theta) noexcept;

    //! Compute tangent
    template <typename Float, std::size_t kN>
    static auto tan(const Vector<Float, kN>& theta) noexcept;

    //! Compute arc sine
    template <typename Float, std::size_t kN>
    static auto asin(const Vector<Float, kN>& x) noexcept;

    //! Compute arc cosine
    template <typename Float, std::size_t kN>
    static auto acos(const Vector<Float, kN>& x) noexcept;

    //! Compute arc tangent
    template <typename Float, std::size_t kN>
    static auto atan(const Vector<Float, kN>& x) noexcept;

    // Floating point manipulation functions

    //! Decompose a number into significand and power of 2
    template <typename Float, typename Integer, std::size_t kN>
    static auto frexp(const Vector<Float, kN>& x,
                      Vector<Integer, kN>* e) noexcept;

    //! Multiplie a number by 2 raised to a power 
    template <typename Float, typename Integer, std::size_t kN>
    static auto ldexp(const Vector<Float, kN>& x,
                      const Vector<Integer, kN>& e) noexcept;

    //! Copy the sign of a floating point value
    template <typename Float, std::size_t kN>
    static auto copysign(const Vector<Float, kN>& x,
                         const Vector<Float, kN>& y) noexcept;
  };
};

// OpenCL style function aliases

// Nearest integer floating point operations

//! Return the nearest integer not less than the given value
template <typename FloatN>
FloatN ceil(const FloatN& x) noexcept;

//! Return the nearest integer not greater than the given value
template <typename FloatN>
FloatN floor(const FloatN& x) noexcept;

//! Return the nearest integer not greater in magnitude than the given value
template <typename FloatN>
FloatN trunc(const FloatN& x) noexcept;

//! Return the nearest integer, rounding away from zero in halfway cases
template <typename FloatN>
FloatN round(const FloatN& x) noexcept;

// Basic operations

//! Return the remainder of the floating point division operation by 1
template <typename FloatN>
FloatN fract(const FloatN& x, FloatN* iptr) noexcept;

//! Return the remainder of the floating point division operation
template <typename FloatN>
FloatN fmod(const FloatN& x, const FloatN& y) noexcept;

//! Fused multiply-add operation
template <typename FloatN>
FloatN fma(const FloatN& a, const FloatN& b, const FloatN& c) noexcept;

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

//! Compute sine and cosine
template <typename FloatN>
FloatN sincos(const FloatN& theta, FloatN* cosval) noexcept;

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

//! Multiplie a number by 2 raised to a power 
template <typename FloatN, typename IntegerN>
FloatN ldexp(const FloatN& x, const IntegerN& e) noexcept;

//! Copy the sign of a floating point value
template <typename FloatN>
FloatN copysign(const FloatN& x, const FloatN& y) noexcept;

} // namespace cl

} // namespace zinvul

#include "math-inl.hpp"

#endif // ZINVUL_CL_MATH_HPP
