/*!
  \file kernel_group-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_GROUP_INL_HPP
#define ZINVUL_KERNEL_GROUP_INL_HPP

#include "kernel_group.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
// Zisc
#include "zisc/error.hpp"
#include "zisc/spin_lock_mutex.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"
#include "zinvul/cppcl/types.hpp"

namespace zinvul {

/*!
  */
inline
KernelGroup::~KernelGroup() noexcept
{
}

/*!
  */
inline
void KernelGroup::__setGlobalWorkId(const std::array<uint32b, 3>& id)
    noexcept
{
  for (std::size_t dim = 0; dim < 3; ++dim) {
    local_work_id_[dim] = id[dim] % local_work_size_[dim];
    work_group_id_[dim] = id[dim] / local_work_size_[dim];
    ZISC_ASSERT(work_group_id_[dim] < work_group_size_[dim],
                "The work group id is out of range.");
  }
}

/*!
  */
inline
void KernelGroup::__setLocalWorkSize(const std::array<uint32b, 3>& size) noexcept
{
  local_work_size_ = size;
}

/*!
  */
inline
void KernelGroup::__setMutex(zisc::SpinLockMutex* mutex) noexcept
{
  mutex_ = mutex;
}

/*!
  */
inline
void KernelGroup::__setWorkGroupSize(const std::array<uint32b, 3>& size) noexcept
{
  work_group_size_ = size;
}

// OpenCL functions

/*!
  */
inline
cl::size_t KernelGroup::get_global_id(const uint dimension) const noexcept
{
  const auto id = get_group_id(dimension) * get_local_size(dimension) +
                  get_local_id(dimension);
  return id;
}

/*!
  */
inline
cl::size_t KernelGroup::get_global_offset(const uint /* dimension */) const noexcept
{
  return 0;
}

/*!
  */
inline
cl::size_t KernelGroup::get_global_size(const uint dimension) const noexcept
{
  return get_local_size(dimension) * get_num_groups(dimension);
}

/*!
  */
inline
cl::size_t KernelGroup::get_group_id(const uint dimension) const noexcept
{
  const cl::size_t id = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? work_group_id_[dimension]
      : 0;
  return id;
}

/*!
  */
inline
cl::size_t KernelGroup::get_local_id(const uint dimension) const noexcept
{
  const cl::size_t id = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? local_work_id_[dimension]
      : 0;
  return id;
}

/*!
  */
inline
cl::size_t KernelGroup::get_local_size(const uint dimension) const noexcept
{
  const cl::size_t size = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? local_work_size_[dimension]
      : 1;
  return size;
}

/*!
  */
inline
cl::size_t KernelGroup::get_num_groups(const uint dimension) const noexcept
{
  const cl::size_t size = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? work_group_size_[dimension]
      : 1;
  return size;
}

/*!
  */
inline
constexpr uint KernelGroup::get_work_dim() noexcept
{
  return 3;
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_GROUP_INL_HPP
