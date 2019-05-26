/*!
  \file kernel_group.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_GROUP_HPP
#define ZINVUL_KERNEL_GROUP_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/memory_resource.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename GroupType>
class KernelGroup
{
 public:
  //! Return the SPIR-V code
  static zisc::pmr::vector<uint32b> getKernelSpirvCode(
      zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Return the group number
  static constexpr uint32b getId() noexcept;
};

} // namespace zinvul

#include "kernel_group-inl.hpp"

#endif // ZINVUL_KERNEL_GROUP_HPP
