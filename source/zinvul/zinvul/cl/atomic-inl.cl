/*!
  \file atomic-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ATOMIC_INL_CL
#define ZINVUL_ATOMIC_INL_CL

#include "atomic.cl"
// Zinvul
#include "types.cl"

namespace zinvul {

/*!
  */
inline
int32b Atomic::add(GlobalPtr<int32b> p, const int32b value) noexcept
{
  const int32b old = atomic_add(p, value);
  return old;
}

/*!
  */
inline
uint32b Atomic::add(GlobalPtr<uint32b> p, const uint32b value) noexcept
{
  const uint32b old = atomic_add(p, value);
  return old;
}

/*!
  */
inline
int32b Atomic::increment(GlobalPtr<int32b> p) noexcept
{
  const int32b old = atomic_inc(p);
  return old;
}

/*!
  */
inline
uint32b Atomic::increment(GlobalPtr<uint32b> p) noexcept
{
  const uint32b old = atomic_inc(p);
  return old;
}

} // namespace zinvul

#endif /* ZINVUL_ATOMIC_INL_CL */
