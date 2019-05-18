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
  const int32b result = atomic_add(p, value);
  return result;
}

} // namespace zinvul

#endif /* ZINVUL_ATOMIC_INL_CL */
