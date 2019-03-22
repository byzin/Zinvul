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

  //! Set a mutex
  void __setMutex(zisc::SpinLockMutex* mutex) noexcept;

  //! Set the work group ID
  void __setWorkGroupId(const uint32b id) noexcept;

  //! Set the work group size
  void __setWorkGroupSize(const std::array<uint32b, 3>& size) noexcept;

  // OpenCL functions

  // Work-Item functions

  //! Return the global work-item ID
  cl::size_t get_global_id(const uint32b dimension) const noexcept;

  //! Return the offset values
  cl::size_t get_global_offset(const uint32b dimension) const noexcept;

  //! Return the number of global work-items
  cl::size_t get_global_size(const uint32b dimension) const noexcept;

  //! Return the work-group ID
  cl::size_t get_group_id(const uint32b dimension) const noexcept;

  //! Return the unique local work-item ID
  cl::size_t get_local_id(const uint32b dimension) const noexcept;

  //! Return the number of local work-item
  cl::size_t get_local_size(const uint32b dimension) const noexcept;

  //! Return the number of work-groups that will execute a kernel
  cl::size_t get_num_groups(const uint32b dimension) const noexcept;

  //! Return the number of dimensions in use
  static constexpr uint32b get_work_dim() noexcept;

  // Atomic functions

  //! Compute (old + value) and store result at location pointed by p, return old
  template <typename Integer>
  Integer atomic_add(Integer* p, const Integer value) noexcept;

  //! Compute (old - value) and store result at location pointed by p, return old
  template <typename Integer>
  Integer atomic_sub(Integer* p, const Integer value) noexcept;

  //! Swap the old value at location p with new value, return old
  template <typename Type>
  Type atomic_xchg(Type* p, const Type value) noexcept;

  //! Compute (old + 1) and store result at location pointed by p, return old
  template <typename Integer>
  Integer atomic_inc(Integer* p) noexcept;

  //! Compute (old - 1) and store result at location pointed by p, return old
  template <typename Integer>
  Integer atomic_dec(Integer* p) noexcept;

  //! Compute (old == comp) ? value : old and store result , return old
  template <typename Integer>
  Integer atomic_cmpxchg(Integer* p, const Integer comp, const Integer value) noexcept;

  //! Compute min(old, value) and store result , return old
  template <typename Integer>
  Integer atomic_min(Integer* p, const Integer value) noexcept;

  //! Compute max(old, value) and store result , return old
  template <typename Integer>
  Integer atomic_max(Integer* p, const Integer value) noexcept;

  //! Compute (old & value) and store result , return old
  template <typename Integer>
  Integer atomic_and(Integer* p, const Integer value) noexcept;

  //! Compute (old | value) and store result , return old
  template <typename Integer>
  Integer atomic_or(Integer* p, const Integer value) noexcept;

  //! Compute (old ^ value) and store result , return old
  template <typename Integer>
  Integer atomic_xor(Integer* p, const Integer value) noexcept;

 private:
  zisc::SpinLockMutex* mutex_ = nullptr;
  std::array<uint32b, 3> work_group_id_{{0, 0, 0}};
  std::array<uint32b, 3> work_group_size_{{0, 0, 0}};
};

} // namespace zinvul

#include "kernel_group-inl.hpp"

#endif // ZINVUL_KERNEL_GROUP_HPP
