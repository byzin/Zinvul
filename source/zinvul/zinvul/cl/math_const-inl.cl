/*!
  \file math_const-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_CONST_INL_CL
#define ZINVUL_MATH_CONST_INL_CL

#include "math_const.cl"
// Zinvul
#include "limits.cl"
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

namespace constmath {

/*!
  */
template <typename Float> inline
constexpr Float calcPi(const int32b n) noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  Float x = static_cast<Float>(2.0);
  for (int32b k = n; 0 < k; --k) {
    const Float t = static_cast<Float>(2 * k + 1);
    x = fusedMulAdd(static_cast<Float>(k), x, static_cast<Float>(2.0) * t) / t;
  }
  return x;
}

/*!
  */
template <typename Float> inline
constexpr Float pi() noexcept
{
  static_assert(kIsFloatingPoint<Float>, "The Float ins't floating point.");
  constexpr int32b n = NumericLimits<Float>::digits();
  const Float p = calcPi<Float>(n);
  return p;
}

/*!
  */
template <typename Arith> inline
constexpr Arith fusedMulAdd(const Arith a, const Arith b, const Arith c) noexcept
{
  static_assert(kIsArithmetic<Arith>, "The Arith isn't arithmetic type.");
  const Arith result = a * b + c;
  return result;
}

} // namespace constmath

} // namespace zinvul

#endif /* ZINVUL_MATH_CONST_INL_CL */
