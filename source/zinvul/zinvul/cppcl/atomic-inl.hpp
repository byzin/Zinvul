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
#include "address_space_pointer.hpp"
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

namespace clinner {

/*!
  */
class Atomic
{
 public:
  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer add(AddressSpacePointer<kAddressSpaceType, Integer> p,
                     const Integer value) noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = old + value;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer sub(AddressSpacePointer<kAddressSpaceType, Integer> p,
                     const Integer value) noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = old - value;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Type>
  static Type swap(AddressSpacePointer<kAddressSpaceType, Type> p,
                   const Type value) noexcept
  {
    static_assert(std::is_same_v<Type, int32b> || std::is_same_v<Type, uint32b> ||
                  std::is_same_v<Type, float>,
                  "The Type isn't int, unsigned int or float.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Type old = zisc::cast<Type>(0);
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = value;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer increment(AddressSpacePointer<kAddressSpaceType, Integer> p)
      noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = (*p)++;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer decrement(AddressSpacePointer<kAddressSpaceType, Integer> p)
      noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = (*p)--;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer compareAndSwap(AddressSpacePointer<kAddressSpaceType, Integer> p,
                                const Integer comp,
                                const Integer value) noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = (old == comp) ? value : old;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer min(AddressSpacePointer<kAddressSpaceType, Integer> p,
                     const Integer value) noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = (value < old) ? value : old;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer max(AddressSpacePointer<kAddressSpaceType, Integer> p,
                     const Integer value) noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = (old < value) ? value : old;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer bitAnd(AddressSpacePointer<kAddressSpaceType, Integer> p,
                        const Integer value) noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = old & value;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer bitOr(AddressSpacePointer<kAddressSpaceType, Integer> p,
                       const Integer value) noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = old | value;
    }
    return old;
  }

  template <AddressSpaceType kAddressSpaceType, typename Integer>
  static Integer bitXor(AddressSpacePointer<kAddressSpaceType, Integer> p,
                        const Integer value) noexcept
  {
    static_assert(std::is_same_v<Integer, int32b> ||
                  std::is_same_v<Integer, uint32b>,
                  "The Integer isn't int or unsigned int.");
    static_assert((kAddressSpaceType == AddressSpaceType::kGlobal) ||
                  (kAddressSpaceType == AddressSpaceType::kLocal),
                  "The address space isn'g global or local.");
    Integer old = 0;
    {
      std::unique_lock<zisc::SpinLockMutex> lock{getMutex()};
      old = *p;
      *p = old ^ value;
    }
    return old;
  }

  //! Return a mutex
  static zisc::SpinLockMutex& getMutex() noexcept
  {
    ZISC_ASSERT(mutex_ != nullptr, "The mutex is null.");
    return *mutex_;
  }

  //! Set a mutex
  static void setMutex(zisc::SpinLockMutex* mutex) noexcept
  {
    mutex_ = mutex;
  }

 private:
  static thread_local zisc::SpinLockMutex* mutex_;
};

} // namespace clinner

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_add(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  return clinner::Atomic::add(p, value);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_sub(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  return clinner::Atomic::sub(p, value);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
Type atomic_xchg(AddressSpacePointer<kAddressSpaceType, Type> p,
                 const Type value) noexcept
{
  return clinner::Atomic::swap(p, value);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_inc(AddressSpacePointer<kAddressSpaceType, Integer> p) noexcept
{
  return clinner::Atomic::increment(p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_dec(AddressSpacePointer<kAddressSpaceType, Integer> p) noexcept
{
  return clinner::Atomic::decrement(p);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_cmpxchg(AddressSpacePointer<kAddressSpaceType, Integer> p,
                       const Integer comp,
                       const Integer value) noexcept
{
  return clinner::Atomic::compareAndSwap(p, comp, value);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_min(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  return clinner::Atomic::min(p, value);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_max(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  return clinner::Atomic::max(p, value);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_and(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  return clinner::Atomic::bitAnd(p, value);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_or(AddressSpacePointer<kAddressSpaceType, Integer> p,
                  const Integer value) noexcept
{
  return clinner::Atomic::bitOr(p, value);
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Integer> inline
Integer atomic_xor(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept
{
  return clinner::Atomic::bitXor(p, value);
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_ATOMIC_INL_HPP
