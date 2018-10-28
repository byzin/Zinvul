/*!
  \file kernel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_INL_HPP
#define ZINVUL_KERNEL_INL_HPP

#include "kernel.hpp"
// Standard C++ library
#include <cstddef>
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> inline
Kernel<GroupType, ArgumentTypes...>::~Kernel() noexcept
{
}

/*!
  */
template <typename GroupType, typename ...ArgumentTypes> inline
constexpr std::size_t Kernel<GroupType, ArgumentTypes...>::numOfArguments() noexcept
{
  constexpr std::size_t size = sizeof...(ArgumentTypes);
  return size;
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_INL_HPP
