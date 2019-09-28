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
#include "zisc/math_const.hpp"
// Zinvul
#include "vector.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

// Float constants
#if !defined(MAXFLOAT)
constexpr float MAXFLOAT = std::numeric_limits<float>::max();
#endif // MAXFLOAT
#if !defined(INFINITY)
constexpr float INFINITY = std::numeric_limits<float>::infinity();
#endif // INFINITY
#if !defined(NAN)
constexpr float NAN = std::numeric_limits<float>::quiet_NaN();
#endif // NAN 
#if !defined(M_E_F)
constexpr float M_E_F = 2.718281746e+00f;
#endif // M_E_F
#if !defined(M_LOG2E_F)
constexpr float M_LOG2E_F = 1.442695022e+00f;
#endif // M_LOG2E_F
#if !defined(M_LOG10E_F)
constexpr float M_LOG10E_F = 4.342944920e-01f;
#endif // M_LOG10E_F
#if !defined(M_LN2_F)
constexpr float M_LN2_F = 6.931471825e-01f;
#endif // M_LN2_F
#if !defined(M_LN10_F)
constexpr float M_LN10_F = 2.302585125e+00f;
#endif // M_LN10_F
#if !defined(M_PI_F)
constexpr float M_PI_F = 3.141592741e+00f;
#endif // M_PI_F
#if !defined(M_PI_2_F)
constexpr float M_PI_2_F = M_PI_F / 2.0f;
#endif // M_PI_2_F
#if !defined(M_PI_4_F)
constexpr float M_PI_4_F = M_PI_F / 4.0f;
#endif // M_PI_4_F
#if !defined(M_1_PI_F)
constexpr float M_1_PI_F = 1.0f / M_PI_F;
#endif // M_1_PI_F
#if !defined(M_2_PI_F)
constexpr float M_2_PI_F = 2.0f / M_PI_F;
#endif // M_2_PI_F
#if !defined(M_2_SQRTPI_F)
constexpr float M_2_SQRTPI_F = 1.128379226e+00f;
#endif // M_2_SQRTPI_F
#if !defined(M_SQRT2_F)
constexpr float M_SQRT2_F = 1.414213538e+00f;
#endif // M_SQRT2_F
#if !defined(M_SQRT1_2_F)
constexpr float M_SQRT1_2_F = 1.0f / M_SQRT2_F;
#endif // M_SQRT1_2_F
#if !defined(M_E)
constexpr double M_E = 2.71828182845904523536;
#endif // M_E
#if !defined(M_LOG2E)
constexpr double M_LOG2E = 1.44269504088896340736;
#endif // M_LOG2E
#if !defined(M_LOG10E)
constexpr double M_LOG10E = 0.434294481903251827651;
#endif // M_LOG10E
#if !defined(M_LN2)
constexpr double M_LN2 = 0.693147180559945309417;
#endif // M_LN2
#if !defined(M_LN10)
constexpr double M_LN10 = 2.30258509299404568402;
#endif // M_LN10
#if !defined(M_PI)
constexpr double M_PI = 3.14159265358979323846;
#endif // M_PI
#if !defined(M_PI_2)
constexpr double M_PI_2 = M_PI / 2.0;
#endif // M_PI_2
#if !defined(M_PI_4)
constexpr double M_PI_4 = M_PI / 4.0;
#endif // M_PI_4
#if !defined(M_1_PI)
constexpr double M_1_PI = 1.0 / M_PI;
#endif // M_1_PI
#if !defined(M_2_PI)
constexpr double M_2_PI = 2.0 / M_PI;
#endif // M_2_PI
#if !defined(M_2_SQRTPI)
constexpr double M_2_SQRTPI = 1.12837916709551257390;
#endif // M_2_SQRTPI
#if !defined(M_SQRT2)
constexpr double M_SQRT2 = 1.41421356237309504880;
#endif // M_SQRT2
#if !defined(M_SQRT1_2)
constexpr double M_SQRT1_2 = 1.0 / M_SQRT2;
#endif // M_SQRT1_2


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

  //! Return 2 raised to the given power
  template <typename FloatN>
  static FloatN exp2(const FloatN& x) noexcept;

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

  //! Compute cubic root
  template <typename FloatN>
  static FloatN cbrt(const FloatN& x) noexcept;

  // Trigonometric functions

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

  //! Extract exponent of the given number
  template <typename FloatN>
  static auto ilogb(const FloatN& x) noexcept;

  //! Decompose a number into integer and fractional parts
  template <typename FloatN>
  static FloatN modf(const FloatN& x, FloatN* iptr) noexcept;

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
    template <typename Float, size_t kN>
    static auto ceil(const Vector<Float, kN>& x) noexcept;

    //! Return the nearest integer not greater than the given value
    template <typename Float, size_t kN>
    static auto floor(const Vector<Float, kN>& x) noexcept;

    //! Return the nearest integer not greater in magnitude than the given value
    template <typename Float, size_t kN>
    static auto trunc(const Vector<Float, kN>& x) noexcept;

    //! Return the nearest integer, rounding away from zero in halfway cases
    template <typename Float, size_t kN>
    static auto round(const Vector<Float, kN>& x) noexcept;

    // Basic operations

    //! Return the remainder of the floating point division operation
    template <typename Float, size_t kN>
    static auto fmod(const Vector<Float, kN>& x,
                     const Vector<Float, kN>& y) noexcept;

    //! Fused multiply-add operation
    template <typename Float, size_t kN>
    static auto fma(const Vector<Float, kN>& a,
                    const Vector<Float, kN>& b,
                    const Vector<Float, kN>& c) noexcept;

    // Exponential functions

    //! Return e raised to the given power
    template <typename Float, size_t kN>
    static auto exp(const Vector<Float, kN>& x) noexcept;

    //! Return 2 raised to the given power
    template <typename Float, size_t kN>
    static auto exp2(const Vector<Float, kN>& x) noexcept;

    //! Compute natural logarithm of the given number
    template <typename Float, size_t kN>
    static auto log(const Vector<Float, kN>& x) noexcept;

    //! Compute base2 logarithm of the given number
    template <typename Float, size_t kN>
    static auto log2(const Vector<Float, kN>& x) noexcept;

    // Power functions

    //! Raise a number to the given power
    template <typename Float, size_t kN>
    static auto pow(const Vector<Float, kN>& base,
                    const Vector<Float, kN>& e) noexcept;

    //! Compute inverse square root
    template <typename Float, size_t kN>
    static auto rsqrt(const Vector<Float, kN>& x) noexcept;

    //! Compute square root
    template <typename Float, size_t kN>
    static auto sqrt(const Vector<Float, kN>& x) noexcept;

    //! Compute cubic root
    template <typename Float, size_t kN>
    static auto cbrt(const Vector<Float, kN>& x) noexcept;

    //! Trigonometric functions

    //! Compute sine
    template <typename Float, size_t kN>
    static auto sin(const Vector<Float, kN>& theta) noexcept;

    //! Compute cosine
    template <typename Float, size_t kN>
    static auto cos(const Vector<Float, kN>& theta) noexcept;

    //! Compute tangent
    template <typename Float, size_t kN>
    static auto tan(const Vector<Float, kN>& theta) noexcept;

    //! Compute arc sine
    template <typename Float, size_t kN>
    static auto asin(const Vector<Float, kN>& x) noexcept;

    //! Compute arc cosine
    template <typename Float, size_t kN>
    static auto acos(const Vector<Float, kN>& x) noexcept;

    //! Compute arc tangent
    template <typename Float, size_t kN>
    static auto atan(const Vector<Float, kN>& x) noexcept;

    // Floating point manipulation functions

    //! Decompose a number into significand and power of 2
    template <typename Float, typename Integer, size_t kN>
    static auto frexp(const Vector<Float, kN>& x,
                      Vector<Integer, kN>* e) noexcept;

    //! Multiply a number by 2 raised to a power 
    template <typename Float, typename Integer, size_t kN>
    static auto ldexp(const Vector<Float, kN>& x,
                      const Vector<Integer, kN>& e) noexcept;

    //! Extract exponent of the given number
    template <typename Float, size_t kN>
    static auto ilogb(const Vector<Float, kN>& x) noexcept;

    //! Decompose a number into integer and fractional parts
    template <typename Float, size_t kN>
    static auto modf(const Vector<Float, kN>& x,
                     Vector<Float, kN>* iptr) noexcept;

    //! Copy the sign of a floating point value
    template <typename Float, size_t kN>
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

//! Return 2 raised to the given power
template <typename FloatN>
FloatN exp2(const FloatN& x) noexcept;

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

//! Compute cubic root
template <typename FloatN>
FloatN cbrt(const FloatN& x) noexcept;

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

//! Extract exponent of the given number
template <typename FloatN>
auto ilogb(const FloatN& x) noexcept;

//! Decompose a number into integer and fractional parts
template <typename FloatN>
FloatN modf(const FloatN& x, FloatN* iptr) noexcept;

//! Copy the sign of a floating point value
template <typename FloatN>
FloatN copysign(const FloatN& x, const FloatN& y) noexcept;

} // namespace cl

} // namespace zinvul

#include "math-inl.hpp"

#endif // ZINVUL_CL_MATH_HPP
