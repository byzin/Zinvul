/*!
  \file types.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPES_HPP
#define ZINVUL_TYPES_HPP

// Zisc
#include "zisc/floating_point.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

// Scalar data types
using uchar = uint8b;
using ushort = uint16b;
using ulong = uint64b;
using half = zisc::HalfFloat;
// Pointer types
using size_t = uint32b;
using ptrdiff_t = int32b;
using intptr_t = int32b;
using uintptr_t = uint32b;

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_TYPES_HPP
