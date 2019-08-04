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

namespace zinvul {

namespace constmath {

//! Calculate circular constant
template <typename Float>
constexpr Float calcPi(const int32b n) noexcept;

//! Return circular constant
template <typename Float>
constexpr Float pi() noexcept;

//! Fused multiply-add operation
template <typename Arith>
constexpr Arith fusedMulAdd(const Arith a, const Arith b, const Arith c) noexcept;

} // namespace constmath

} // namespace zinvul

#include "math_const-inl.cl"

#endif /* ZINVUL_MATH_CONST_CL */
