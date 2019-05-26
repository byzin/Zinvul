/*!
  \file kernel_set-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_SET_INL_HPP
#define ZINVUL_KERNEL_SET_INL_HPP

#include "kernel_set.hpp"
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
template <typename SetType> inline
zisc::pmr::vector<uint32b> KernelSet<SetType>::getKernelSpirvCode(
    zisc::pmr::memory_resource* mem_resource) noexcept
{
  return SetType::getKernelSpirvCode(mem_resource);
}

/*!
  */
template <typename SetType> inline
constexpr uint32b KernelSet<SetType>::getId() noexcept
{
  return SetType::getId();
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_SET_INL_HPP
