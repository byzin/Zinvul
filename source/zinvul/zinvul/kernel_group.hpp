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
#include <cstddef>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/spin_lock_mutex.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"
#include "zinvul/cppcl/types.hpp"

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

  //! Set the global work ID
  void __setGlobalWorkId(const std::array<uint32b, 3>& id) noexcept;

  //! Set the local work size
  void __setLocalWorkSize(const std::array<uint32b, 3>& size) noexcept;

  //! Set a mutex
  void __setMutex(zisc::SpinLockMutex* mutex) noexcept;

  //! Set the work group size
  void __setWorkGroupSize(const std::array<uint32b, 3>& size) noexcept;

  // OpenCL functions

  // Work-Item functions

  //! Return the global work-item ID
  cl::size_t get_global_id(const uint dimension) const noexcept;

  //! Return the offset values
  cl::size_t get_global_offset(const uint dimension) const noexcept;

  //! Return the number of global work-items
  cl::size_t get_global_size(const uint dimension) const noexcept;

  //! Return the work-group ID
  cl::size_t get_group_id(const uint dimension) const noexcept;

  //! Return the unique local work-item ID
  cl::size_t get_local_id(const uint dimension) const noexcept;

  //! Return the number of local work-item
  cl::size_t get_local_size(const uint dimension) const noexcept;

  //! Return the number of work-groups that will execute a kernel
  cl::size_t get_num_groups(const uint dimension) const noexcept;

  //! Return the number of dimensions in use
  static constexpr uint get_work_dim() noexcept;

 private:
  zisc::SpinLockMutex* mutex_ = nullptr;
  std::array<uint32b, 3> local_work_id_{{0, 0, 0}};
  std::array<uint32b, 3> local_work_size_{{0, 0, 0}};
  std::array<uint32b, 3> work_group_id_{{0, 0, 0}};
  std::array<uint32b, 3> work_group_size_{{0, 0, 0}};
};

} // namespace zinvul

#include "kernel_group-inl.hpp"

#endif // ZINVUL_KERNEL_GROUP_HPP
