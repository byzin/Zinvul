/*!
  \file math.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_MATH_HPP
#define ZINVUL_CL_MATH_HPP

// Standard C++ library
#include <cmath>
#include <type_traits>
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

// Float constants
constexpr float MAXFLOAT = std::numeric_limits<float>::max();

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_MATH_HPP
