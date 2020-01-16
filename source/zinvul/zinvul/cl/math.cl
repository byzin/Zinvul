/*!
  \file math.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
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

  //! Extracts exponent of the given number
  template <typename FloatN>
  static auto ilogb(const FloatN x) noexcept;

  //! Decompose a number into integer and fractional parts
  template <typename FloatN, typename FloatNPtr>
  static FloatN modf(const FloatN x, FloatNPtr iptr) noexcept;

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

    //! Extracts exponent of the given number
    template <typename FloatN>
    static auto ilogb(const FloatN x) noexcept;

    //! Decompose a number into integer and fractional parts
    template <typename FloatN, typename FloatNPtr>
    static FloatN modf(const FloatN x, FloatNPtr iptr) noexcept;

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

    //! Extracts exponent of the given number
    template <typename FloatN>
    static auto ilogb(const FloatN x) noexcept;

    //! Decompose a number into integer and fractional parts
    template <typename FloatN, typename FloatNPtr>
    static FloatN modf(const FloatN x, FloatNPtr iptr) noexcept;

   private:
    struct CbrtPolyConstants
    {
      template <typename Float, size_t kIndex>
      static constexpr Float get() noexcept;
    };

    struct SinCosPolyConstants
    {
      template <typename Float, size_t kIndex>
      static constexpr Float get() noexcept;
    };

    struct TanPolyConstants
    {
      template <typename Float, size_t kIndex>
      static constexpr Float get() noexcept;
    };

    struct AsinPolyConstants
    {
      template <typename Float, size_t kIndex>
      static constexpr Float get() noexcept;
    };

    struct AtanPolyConstants
    {
      template <typename Float, size_t kIndex>
      static constexpr Float get() noexcept;
    };

    template <typename FloatN>
    struct F2
    {
      FloatN x_;
      FloatN y_;
    };

    template <typename FloatN>
    static FloatN upper(const FloatN x) noexcept;

    template <typename FloatN>
    static F2<FloatN> addF2F(const F2<FloatN> lhs, const FloatN rhs) noexcept;

    template <typename FloatN>
    static F2<FloatN> add2F2F2(const F2<FloatN> lhs, const F2<FloatN> rhs) noexcept;

    template <typename FloatN>
    static F2<FloatN> mulFF(const FloatN lhs, const FloatN rhs) noexcept;

    template <typename FloatN>
    static F2<FloatN> normalizeF2(const F2<FloatN> x) noexcept;

    template <typename FloatN>
    static FloatN toward0(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN removeLsb(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN mulsign(const FloatN x, const FloatN y) noexcept;

    template <typename FloatN, typename IntegerN>
    static FloatN pow2i(const IntegerN q) noexcept;

    //! Evaluate polygomials using Estrin's method
    template <typename Constants, size_t kIndex, typename FloatN>
    static FloatN evalPoly2(const FloatN x) noexcept;

    //! Evaluate polygomials using Estrin's method
    template <typename Constants, size_t kIndex, typename FloatN>
    static FloatN evalPoly3(const FloatN x, const FloatN x3) noexcept;

    //! Evaluate polygomials using Estrin's method
    template <typename Constants, size_t kIndex, typename FloatN>
    static FloatN evalPoly4(const FloatN x1, const FloatN x2) noexcept;

    //! Evaluate polygomials using Estrin's method
    template <typename Constants, size_t kIndex, typename FloatN>
    static FloatN evalPoly6(const FloatN x1,
                            const FloatN x2,
                            const FloatN x3) noexcept;

    //! Evaluate polygomials using Estrin's method
    template <typename Constant, size_t kIndex, typename FloatN>
    static FloatN evalPoly8(const FloatN x1,
                            const FloatN x2,
                            const FloatN x3) noexcept;

    //! Evaluate polygomials using Estrin's method
    template <typename Constant, size_t kIndex, typename FloatN>
    static FloatN evalPoly12(const FloatN x1,
                             const FloatN x2,
                             const FloatN x3,
                             const FloatN x4) noexcept;

    //! Evaluate polygomials using Estrin's method
    template <typename Constant, size_t kIndex, typename FloatN>
    static FloatN evalPoly16(const FloatN x1,
                             const FloatN x2,
                             const FloatN x3,
                             const FloatN x4) noexcept;

    //! Evaluate polygomials using Estrin's method
    template <typename Constant, size_t kIndex, typename FloatN>
    static FloatN evalPoly19(const FloatN x1,
                             const FloatN x2,
                             const FloatN x3,
                             const FloatN x4,
                             const FloatN x5) noexcept;


    //! Decompose a number into significand and power of 2
    template <typename FloatN, typename IntegerNPtr>
    static FloatN frexpImpl(const FloatN x, IntegerNPtr e) noexcept;

    template <typename Float, size_t kIndex>
    static constexpr auto getUpperCoeff() noexcept;

    template <typename Float, size_t kIndex>
    static constexpr auto getCbrtCoeff() noexcept;

    template <typename Float>
    static constexpr int32b getFrexpCoeff() noexcept;

    template <typename Float, int32b kTo>
    static constexpr Float getHalfNextafter() noexcept;

    template <typename Float>
    static constexpr int32b getIlogb0() noexcept;

    template <typename Float>
    static constexpr int32b getIlogbNan() noexcept;

    template <typename Float, size_t kIndex>
    static constexpr auto getIlogbCoeff() noexcept;

    template <typename Float, size_t kIndex>
    static constexpr Float getPi() noexcept;

    template <typename Float, size_t kIndex>
    static constexpr Float getPi2() noexcept;

    template <typename Float, size_t kIndex>
    static constexpr Float getTrigRangeMax() noexcept;

    template <typename Float, size_t kIndex>
    static constexpr auto getAcosCoeff() noexcept;

    template <typename Float, size_t kIndex>
    static constexpr auto getAtanCoeff() noexcept;

    //! Extracts exponent of the given number
    template <typename FloatN>
    static auto ilogbImpl(FloatN x) noexcept;

    template <typename FloatN>
    static FloatN initFraction(const FloatN x) noexcept;

    //! Multiply a number by 2 raised to a power
    template <typename FloatN, typename IntegerN>
    static FloatN ldexpImpl(const FloatN x, const IntegerN e) noexcept;

    template <typename FloatN>
    static FloatN rintImpl(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN fmodImpl(const FloatN x, const FloatN y) noexcept;

    template <typename FloatN>
    static FloatN fmodImplF(const FloatN x, const FloatN y) noexcept;

    template <typename FloatN>
    static FloatN fmodImplD(const FloatN x, const FloatN y) noexcept;

    template <typename FloatN>
    static FloatN sinImpl(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN sinImplF(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN sinImplD(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN cosImpl(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN cosImplF(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN cosImplD(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN tanImpl(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN tanImplF(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN tanImplD(const FloatN theta) noexcept;

    template <typename FloatN>
    static FloatN asinImpl(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN asinImplF(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN asinImplD(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN acosImpl(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN atanImpl(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN atanImplF(const FloatN x) noexcept;

    template <typename FloatN>
    static FloatN atanImplD(const FloatN x) noexcept;
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

//! Extracts exponent of the given number
template <typename FloatN>
auto ilogb(const FloatN x) noexcept;

//! Decompose a number into integer and fractional parts
template <typename FloatN, typename FloatNPtr>
FloatN modf(const FloatN x, FloatNPtr iptr) noexcept;

//! Copy the sign of a floating point value
template <typename FloatN>
FloatN copysign(const FloatN x, const FloatN y) noexcept;

} // namespace zinvul

#include "math-inl.cl"

#endif /* ZINVUL_MATH_CL */
