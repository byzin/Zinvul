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

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
std::array<uint32b, 3> Kernel<GroupType, kDimension, ArgumentTypes...>::
    getNumOfWorks(const std::array<uint32b, kDimension>& works) const noexcept
{
  std::array<uint32b, 3> num_of_works{{1, 1, 1}};
  for (std::size_t i = 0; i < works.size(); ++i) {
    ZISC_ASSERT(0 < works[i], "The workgroup size is 0.");
    num_of_works[i] = works[i];
  }
  return num_of_works;
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_INL_HPP
