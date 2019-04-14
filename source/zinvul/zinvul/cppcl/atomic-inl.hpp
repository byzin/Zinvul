/*!
  \file atomic-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_ATOMIC_INL_HPP
#define ZINVUL_CL_ATOMIC_INL_HPP

#include "atomic.hpp"
// Standard C++ library
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

namespace cl {

static thread_local zisc::SpinLockMutex* __mutex = nullptr; //!< Referred by all cl atomic functions

/*!
  */
inline
void __setMutex(zisc::SpinLockMutex* mutex) noexcept
{
  __mutex = mutex;
}

/*!
  */
template <typename Integer> inline
Integer atomic_add(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = old + value;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_sub(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = old - value;
  }
  return old;
}

/*!
  */
template <typename Type> inline
Type atomic_xchg(Type* p, const Type value) noexcept
{
  static_assert(std::is_same_v<Type, int32b> || std::is_same_v<Type, uint32b> ||
                std::is_same_v<Type, float>,
                "The Type isn't int, unsigned int or float.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Type old = zisc::cast<Type>(0);
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = value;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_inc(Integer* p) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = (*p)++;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_dec(Integer* p) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = (*p)--;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_cmpxchg(Integer* p,
                       const Integer comp,
                       const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = (old == comp) ? value : old;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_min(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = (value < old) ? value : old;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_max(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = (old < value) ? value : old;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_and(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = old & value;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_or(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = old | value;
  }
  return old;
}

/*!
  */
template <typename Integer> inline
Integer atomic_xor(Integer* p, const Integer value) noexcept
{
  static_assert(std::is_same_v<Integer, int32b> || std::is_same_v<Integer, uint32b>,
                "The Integer isn't int or unsigned int.");
  ZISC_ASSERT(__mutex != nullptr, "The mutex is null.");
  Integer old = 0;
  {
    std::unique_lock<zisc::SpinLockMutex> lock{*__mutex};
    old = *p;
    *p = old ^ value;
  }
  return old;
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_ATOMIC_INL_HPP
