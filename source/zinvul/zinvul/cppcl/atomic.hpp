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
#include "zinvul/zinvul_config.hpp"
#include "zinvul/cppcl/types.hpp"

namespace zinvul {

namespace cl {

//! Set a mutex
void __setMutex(zisc::SpinLockMutex* mutex) noexcept;

// OpenCL functions

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

} // namespace cl

} // namespace zinvul

#include "atomic-inl.hpp"

#endif // ZINVUL_CL_ATOMIC_HPP
