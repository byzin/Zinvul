/*!
  \file zinvul_config-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CONFIG_INL_HPP
#define ZINVUL_CONFIG_INL_HPP

#include "zinvul/zinvul_config.hpp"

namespace zinvul {

static_assert(sizeof(int8) == 1, "The size of int8 is wrong.");
static_assert(sizeof(int16) == 2, "The size of int16 is wrong.");
static_assert(sizeof(int32) == 4, "The size of int32 is wrong.");
static_assert(sizeof(int64) == 8, "The size of int64 is wrong.");
static_assert(sizeof(uint8) == 1, "The size of uint8 is wrong.");
static_assert(sizeof(uint16) == 2, "The size of uint16 is wrong.");
static_assert(sizeof(uint32) == 4, "The size of uint32 is wrong.");
static_assert(sizeof(uint64) == 8, "The size of uint64 is wrong.");

} // namespace zinvul

#endif // ZINVUL_CONFIG_INL_HPP
