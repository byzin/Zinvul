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
#include <mutex>
#include <type_traits>
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
void KernelGroup::__setMutex(zisc::SpinLockMutex* mutex) noexcept
{
  mutex_ = mutex;
}

/*!
  */
inline
void KernelGroup::__setWorkGroupId(const uint32b id) noexcept
{
  work_group_id_[0] = id % work_group_size_[0];
  work_group_id_[1] = (id / work_group_size_[0]) % work_group_size_[1];
  work_group_id_[2] = id / (work_group_size_[0] * work_group_size_[1]);
  ZISC_ASSERT(work_group_id_[2] < work_group_size_[2],
              "The workgroup ID is invalid: ID=", id);
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
cl::size_t KernelGroup::get_global_id(const uint32b dimension) const noexcept
{
  const auto id = get_group_id(dimension);
  return id;
}

/*!
  */
inline
cl::size_t KernelGroup::get_global_offset(const uint32b /* dimension */) const noexcept
{
  return 0;
}

/*!
  */
inline
cl::size_t KernelGroup::get_global_size(const uint32b dimension) const noexcept
{
  return get_num_groups(dimension);
}

/*!
  */
inline
cl::size_t KernelGroup::get_group_id(const uint32b dimension) const noexcept
{
  const cl::size_t id = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? work_group_id_[dimension]
      : 0;
  return id;
}

/*!
  */
inline
cl::size_t KernelGroup::get_local_id(const uint32b /* dimension */) const noexcept
{
  return 0;
}

/*!
  */
inline
cl::size_t KernelGroup::get_local_size(const uint32b /* dimension */) const noexcept
{
  return 1;
}

/*!
  */
inline
cl::size_t KernelGroup::get_num_groups(const uint32b dimension) const noexcept
{
  const cl::size_t size = zisc::isInBounds(dimension, 0u, get_work_dim())
      ? work_group_size_[dimension]
      : 1;
  return size;
}

/*!
  */
inline
constexpr uint32b KernelGroup::get_work_dim() noexcept
{
  return 3;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_add(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = old + value;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_sub(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = old - value;
  }
  return old;
}

/*!
  */
template <typename Type> inline
Type KernelGroup::atomic_xchg(Type* p, const Type value) noexcept
{
  static_assert(std::is_same_v<Type, int32b> || std::is_same_v<Type, uint32b> ||
                std::is_same_v<Type, float>,
                "The Type isn't int, unsigned int or float.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Type old = zisc::cast<Type>(0);
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = value;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_inc(Integer* p) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = (*p)++;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_dec(Integer* p) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = (*p)--;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_cmpxchg(Integer* p,
                                    const Integer comp,
                                    const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = (old == comp) ? value : old;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_min(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = (value < old) ? value : old;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_max(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = (old < value) ? value : old;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_and(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = old & value;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_or(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = old | value;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer KernelGroup::atomic_xor(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*mutex_};
    old = *p;
    *p = old ^ value;
  }
  return old;
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_GROUP_INL_HPP
