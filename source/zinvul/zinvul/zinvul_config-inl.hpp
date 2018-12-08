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
// Standard C++ library
#include <type_traits>
// Zisc
#include "zisc/utility.hpp"

namespace zinvul {

static_assert(sizeof(int8b) == 1, "The size of int8b is wrong.");
static_assert(sizeof(int16b) == 2, "The size of int16b is wrong.");
static_assert(sizeof(int32b) == 4, "The size of int32b is wrong.");
static_assert(sizeof(int64b) == 8, "The size of int64b is wrong.");
static_assert(sizeof(uint8b) == 1, "The size of uint8b is wrong.");
static_assert(sizeof(uint16b) == 2, "The size of uint16b is wrong.");
static_assert(sizeof(uint32b) == 4, "The size of uint32b is wrong.");
static_assert(sizeof(uint64b) == 8, "The size of uint64b is wrong.");

inline
BufferUsage operator&(const BufferUsage& lhs, const BufferUsage& rhs) noexcept
{
  using Type = std::underlying_type_t<BufferUsage>;
  return zisc::cast<BufferUsage>(zisc::cast<Type>(lhs) & (zisc::cast<Type>(rhs)));
}

inline
BufferUsage operator|(const BufferUsage& lhs, const BufferUsage& rhs) noexcept
{
  using Type = std::underlying_type_t<BufferUsage>;
  return zisc::cast<BufferUsage>(zisc::cast<Type>(lhs) | (zisc::cast<Type>(rhs)));
}

} // namespace zinvul

#endif // ZINVUL_CONFIG_INL_HPP
