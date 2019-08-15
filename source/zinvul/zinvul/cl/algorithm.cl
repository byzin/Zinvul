/*!
  \file algorithm.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ALGORITHM_CL
#define ZINVUL_ALGORITHM_CL

// Zinvul
#include "types.cl"

namespace zinvul {

//! Return the absolute value of the given value
template <typename ArithN>
ArithN abs(const ArithN x) noexcept;

//! Clamp a value between a pair of boundary values
template <typename Arith1N, typename Arith2N>
Arith1N clamp(const Arith1N x, const Arith2N minval, const Arith2N maxval) noexcept;

//! Return the greater of the given values
template <typename ArithN>
ArithN max(const ArithN x, const ArithN y) noexcept;

//! Return the smaller of the given values
template <typename ArithN>
ArithN min(const ArithN x, const ArithN y) noexcept;

} // namespace zinvul

#include "algorithm-inl.cl"

#endif /* ZINVUL_ALGORITHM_CL */
