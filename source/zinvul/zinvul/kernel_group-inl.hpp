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
// Zisc
#include "zisc/error.hpp"
#include "zisc/spin_lock_mutex.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

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
void KernelGroup::setGlobalWorkId__(const std::array<uint32b, 3> global_work_id)
    noexcept
{
  global_work_id_ = global_work_id;
}

/*!
  */
inline
void KernelGroup::setMutex(zisc::SpinLockMutex* mutex) noexcept
{
  mutex_ = mutex;
}

// OpenCL functions

/*!
  */
inline
size_t KernelGroup::get_global_id(const uint dimension) const noexcept
{
  ZISC_ASSERT(dimension < get_work_dim(), "The dimension is out of the range.");
  return global_work_id_[dimension];
}

/*!
  */
inline
size_t KernelGroup::get_global_offset(const uint /* dimension */) const noexcept
{
  return 0;
}

/*!
  */
inline
uint KernelGroup::get_work_dim() const noexcept
{
  return 3;
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_GROUP_INL_HPP
