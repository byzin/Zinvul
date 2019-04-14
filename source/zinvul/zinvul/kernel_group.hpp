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
#include "zisc/non_copyable.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
class KernelGroup : private zisc::NonCopyable<KernelGroup>
{
 public:
  //!
  virtual ~KernelGroup() noexcept;


  //! Return the group number
  virtual uint32b __getKernelGroupNumber() const noexcept = 0;

  //! Return the SPIR-V code
  virtual zisc::pmr::vector<uint32b> __getKernelSpirvCode(
      zisc::pmr::memory_resource* mem_resource) const noexcept = 0;
};

} // namespace zinvul

#include "kernel_group-inl.hpp"

#endif // ZINVUL_KERNEL_GROUP_HPP
