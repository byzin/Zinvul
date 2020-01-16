/*!
  \file kernel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_INL_HPP
#define ZINVUL_KERNEL_INL_HPP

#include "kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <std::size_t kDimension, typename ...BufferArgs> inline
Kernel<kDimension, BufferArgs...>::~Kernel() noexcept
{
}

/*!
  */
template <std::size_t kDimension, typename ...BufferArgs> inline
constexpr std::size_t Kernel<kDimension, BufferArgs...>::numOfArguments()
    noexcept
{
  constexpr std::size_t size = sizeof...(BufferArgs);
  return size;
}

/*!
  */
template <std::size_t kDimension, typename ...BufferArgs> inline
constexpr std::size_t Kernel<kDimension, BufferArgs...>::workgroupDimension()
    noexcept
{
  return kDimension;
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_INL_HPP
