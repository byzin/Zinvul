/*!
  \file atomic.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_ATOMIC_HPP
#define ZINVUL_CL_ATOMIC_HPP

// Zisc
#include "zisc/spin_lock_mutex.hpp"
// Zinvul
#include "address_space_pointer.hpp"
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

//! Compute (old + value) and store result at location pointed by p, return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_add(AddressSpacePointer<Integer, kAddressSpaceType> p,
                   const Integer value) noexcept;

//! Compute (old - value) and store result at location pointed by p, return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_sub(AddressSpacePointer<Integer, kAddressSpaceType> p,
                   const Integer value) noexcept;

//! Swap the old value at location p with new value, return old
template <typename Type, AddressSpaceType kAddressSpaceType>
Type atomic_xchg(AddressSpacePointer<Type, kAddressSpaceType> p,
                 const Type value) noexcept;

//! Compute (old + 1) and store result at location pointed by p, return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_inc(AddressSpacePointer<Integer, kAddressSpaceType> p) noexcept;

//! Compute (old - 1) and store result at location pointed by p, return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_dec(AddressSpacePointer<Integer, kAddressSpaceType> p) noexcept;

//! Compute (old == comp) ? value : old and store result , return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_cmpxchg(AddressSpacePointer<Integer, kAddressSpaceType> p,
                       const Integer comp,
                       const Integer value) noexcept;

//! Compute min(old, value) and store result , return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_min(AddressSpacePointer<Integer, kAddressSpaceType> p,
                   const Integer value) noexcept;

//! Compute max(old, value) and store result , return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_max(AddressSpacePointer<Integer, kAddressSpaceType> p,
                   const Integer value) noexcept;

//! Compute (old & value) and store result , return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_and(AddressSpacePointer<Integer, kAddressSpaceType> p,
                   const Integer value) noexcept;

//! Compute (old | value) and store result , return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_or(AddressSpacePointer<Integer, kAddressSpaceType> p,
                  const Integer value) noexcept;

//! Compute (old ^ value) and store result , return old
template <typename Integer, AddressSpaceType kAddressSpaceType>
Integer atomic_xor(AddressSpacePointer<Integer, kAddressSpaceType> p,
                   const Integer value) noexcept;

} // namespace cl

} // namespace zinvul

#include "atomic-inl.hpp"

#endif // ZINVUL_CL_ATOMIC_HPP
