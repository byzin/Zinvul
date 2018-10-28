/*!
  \file kernel_group.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_GROUP_HPP
#define ZINVUL_KERNEL_GROUP_HPP

// Standard C++ library
#include <array>
// Zisc
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


  //! Set the global work ID
  void setGlobalWorkId(const uint32b x, const uint32b y, const uint32b z) noexcept;

  // OpenCL functions

  //! Return the global work-item ID
  size_t get_global_id(const uint dimension) const noexcept;

  //! Return the offset values
  size_t get_global_offset(const uint dimension) const noexcept;

  //! Return the number of dimensions in use
  uint get_work_dim() const noexcept;

 private:
  std::array<uint32b, 3> global_work_id_{{0, 0, 0}};
};

} // namespace zinvul

#include "kernel_group-inl.hpp"

#endif // ZINVUL_KERNEL_GROUP_HPP
