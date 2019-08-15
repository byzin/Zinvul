/*!
  \file algorithm.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_ALGORITHM_HPP
#define ZINVUL_CL_ALGORITHM_HPP

// Standard C++ library
#include <type_traits>
// Zinvul
#include "vector.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

/*!
  */
class Algorithm
{
 public:
  //! Return the absolute value of the given value
  template <typename IntegerN>
  static auto abs(const IntegerN& x) noexcept;

  //! Return the absolute value of the given value
  template <typename FloatN>
  static FloatN fabs(const FloatN& x) noexcept;

  //! Clamp a value between pair of boundary values
  template <typename Type1N, typename Type2N>
  static Type1N clamp(const Type1N& x,
                      const Type2N& minval,
                      const Type2N& maxval) noexcept;

  //! Convert radians to degrees. (180 / pi) * radians
  template <typename FloatN>
  static FloatN degrees(const FloatN& r) noexcept;

  //! Return y if x < y, otherwise it returns x
  template <typename TypeN>
  static TypeN max(const TypeN& x, const TypeN& y) noexcept;

  //! Return y if y < x, otherwise it returns x
  template <typename TypeN>
  static TypeN min(const TypeN& x, const TypeN& y) noexcept;

  //! Return the linear blend of x & y implemented as: x + (y - x) * a
  template <typename TypeN1, typename TypeN2>
  static TypeN1 mix(const TypeN1& x, const TypeN1& y, const TypeN2& a) noexcept;

  //! Convert degrees to radians. (pi / 180) * degrees
  template <typename FloatN>
  static FloatN radians(const FloatN& d) noexcept;

  //! Return 1 if x > 0, -1 if x < 0, otherwise 0
  template <typename FloatN>
  static FloatN sign(const FloatN& x) noexcept;

  // Integer functions

  //! Return the number of leading 0-bits
  template <typename IntegerN>
  static IntegerN clz(const IntegerN& x) noexcept;

  //! Return the number of non-zero bits
  template <typename IntegerN>
  static IntegerN popcount(const IntegerN& x) noexcept;

 private:
  /*!
    */
  class Vec
  {
   public:
    //! Return the absolute value of the given value
    template <typename Integer, std::size_t kN>
    static auto abs(const Vector<Integer, kN>& x) noexcept;

    //! Return the absolute value of the given value
    template <typename Float, std::size_t kN>
    static auto fabs(const Vector<Float, kN>& x) noexcept;

    //! Clamp a value between pair of boundary values
    template <typename Type1, typename Type2, std::size_t kN>
    static auto clamp(const Vector<Type1, kN>& x,
                      const Type2 minval,
                      const Type2 maxval) noexcept;

    //! Clamp a value between pair of boundary values
    template <typename Type1, typename Type2, std::size_t kN>
    static auto clamp(const Vector<Type1, kN>& x,
                      const Vector<Type2, kN>& minval,
                      const Vector<Type2, kN>& maxval) noexcept;

    //! Convert radians to degrees. (180 / pi) * radians
    template <typename Float, std::size_t kN>
    static auto degrees(const Vector<Float, kN>& r) noexcept;

    //! Return y if x < y, otherwise it returns x
    template <typename Type, std::size_t kN>
    static auto max(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept;

    //! Return y if y < x, otherwise it returns x
    template <typename Type, std::size_t kN>
    static auto min(const Vector<Type, kN>& x, const Vector<Type, kN>& y) noexcept;

    //! Return the linear blend of x & y implemented as: x + (y - x) * a
    template <typename Float1, typename Float2, std::size_t kN>
    static auto mix(const Vector<Float1, kN>& x,
                    const Vector<Float1, kN>& y,
                    const Float2 a) noexcept;

    //! Return the linear blend of x & y implemented as: x + (y - x) * a
    template <typename Float1, typename Float2, std::size_t kN>
    static auto mix(const Vector<Float1, kN>& x,
                    const Vector<Float1, kN>& y,
                    const Vector<Float2, kN>& a) noexcept;

    //! Convert degrees to radians. (pi / 180) * degrees
    template <typename Float, std::size_t kN>
    static auto radians(const Vector<Float, kN>& d) noexcept;

    //! Return 1 if x > 0, -1 if x < 0, otherwise 0
    template <typename Float, std::size_t kN>
    static auto sign(const Vector<Float, kN>& x) noexcept;

    // Integer functions

    //! Return the number of non-zero bits
    template <typename Integer, std::size_t kN>
    static auto clz(const Vector<Integer, kN>& x) noexcept;

    //! Return the number of non-zero bits
    template <typename Integer, std::size_t kN>
    static auto popcount(const Vector<Integer, kN>& x) noexcept;
  };
};

// OpenCL style function aliases

//! Return the absolute value of the given value
template <typename IntegerN>
auto abs(const IntegerN& x) noexcept;

//! Return the absolute value of the given value
template <typename FloatN>
FloatN fabs(const FloatN& x) noexcept;

//! Clamp a value between pair of boundary values
template <typename Type1N, typename Type2N>
Type1N clamp(const Type1N& x, const Type2N& minval, const Type2N& maxval) noexcept;

//! Convert radians to degrees. (180 / pi) * radians
template <typename FloatN>
FloatN degrees(const FloatN& r) noexcept;

//! Return y if x < y, otherwise it returns x
template <typename TypeN>
TypeN max(const TypeN& x, const TypeN& y) noexcept;

//! Return y if y < x, otherwise it returns x
template <typename TypeN>
TypeN min(const TypeN& x, const TypeN& y) noexcept;

//! Return the linear blend of x & y implemented as: x + (y - x) * a
template <typename TypeN1, typename TypeN2>
TypeN1 mix(const TypeN1& x, const TypeN1& y, const TypeN2& a) noexcept;

//! Convert degrees to radians. (pi / 180) * degrees
template <typename FloatN>
FloatN radians(const FloatN& d) noexcept;

//! Return 1 if x > 0, -1 if x < 0, otherwise 0
template <typename FloatN>
FloatN sign(const FloatN& x) noexcept;

// Integer functions

//! Return the number of leading 0-bits
template <typename IntegerN>
IntegerN clz(const IntegerN& x) noexcept;

//! Return the number of non-zero bits
template <typename IntegerN>
IntegerN popcount(const IntegerN& x) noexcept;

} // namespace cl

} // namespace zinvul

#include "algorithm-inl.hpp"

#endif // ZINVUL_CL_ALGORITHM_HPP
