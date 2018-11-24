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
#include <array>
#include <cstddef>
// Zisc
#include "zisc/error.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
Kernel<GroupType, kDimension, ArgumentTypes...>::~Kernel() noexcept
{
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
constexpr std::size_t Kernel<GroupType, kDimension, ArgumentTypes...>::
    numOfArguments() noexcept
{
  constexpr std::size_t size = sizeof...(ArgumentTypes);
  return size;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
constexpr std::size_t Kernel<GroupType, kDimension, ArgumentTypes...>::
    workgroupDimension() noexcept
{
  return kDimension;
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_INL_HPP
