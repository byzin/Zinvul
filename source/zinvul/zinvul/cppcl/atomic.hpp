/*!
  \file atomic.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_ATOMIC_HPP
#define ZINVUL_CL_ATOMIC_HPP

// Zinvul
#include "address_space_pointer.hpp"
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

//! Compute (old + value) and store result at location pointed by p, return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_add(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept;

//! Compute (old - value) and store result at location pointed by p, return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_sub(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept;

//! Swap the old value at location p with new value, return old
template <AddressSpaceType kAddressSpaceType, typename Type>
Type atomic_xchg(AddressSpacePointer<kAddressSpaceType, Type> p,
                 const Type value) noexcept;

//! Compute (old + 1) and store result at location pointed by p, return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_inc(AddressSpacePointer<kAddressSpaceType, Integer> p) noexcept;

//! Compute (old - 1) and store result at location pointed by p, return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_dec(AddressSpacePointer<kAddressSpaceType, Integer> p) noexcept;

//! Compute (old == comp) ? value : old and store result , return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_cmpxchg(AddressSpacePointer<kAddressSpaceType, Integer> p,
                       const Integer comp,
                       const Integer value) noexcept;

//! Compute min(old, value) and store result , return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_min(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept;

//! Compute max(old, value) and store result , return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_max(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept;

//! Compute (old & value) and store result , return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_and(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept;

//! Compute (old | value) and store result , return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_or(AddressSpacePointer<kAddressSpaceType, Integer> p,
                  const Integer value) noexcept;

//! Compute (old ^ value) and store result , return old
template <AddressSpaceType kAddressSpaceType, typename Integer>
Integer atomic_xor(AddressSpacePointer<kAddressSpaceType, Integer> p,
                   const Integer value) noexcept;

} // namespace cl

} // namespace zinvul

#include "atomic-inl.hpp"

#endif // ZINVUL_CL_ATOMIC_HPP
