/*!
  \file kernel_group-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_GROUP_INL_HPP
#define ZINVUL_KERNEL_GROUP_INL_HPP

#include "kernel_group.hpp"
// Standard C++ library
#include <cstddef>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename GroupType> inline
zisc::pmr::vector<uint32b> KernelGroup<GroupType>::getKernelSpirvCode(
    zisc::pmr::memory_resource* mem_resource) noexcept
{
  return GroupType::getKernelSpirvCode(mem_resource);
}

/*!
  */
template <typename GroupType> inline
constexpr uint32b KernelGroup<GroupType>::getId() noexcept
{
  return GroupType::getId();
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_GROUP_INL_HPP
