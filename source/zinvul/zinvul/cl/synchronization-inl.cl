/*!
  \file synchronization-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_SYNCHRONIZATION_INL_CL
#define ZINVUL_SYNCHRONIZATION_INL_CL

#include "synchronization.cl"

namespace zinvul {

/*!
  */
inline
void barrierMemory() noexcept
{
  barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
}

/*!
  */
inline
void barrierLocalMemory() noexcept
{
  barrier(CLK_LOCAL_MEM_FENCE);
}

/*!
  */
inline
void barrierGlobalMemory() noexcept
{
  barrier(CLK_GLOBAL_MEM_FENCE);
}

} // namespace zinvul

#endif /* ZINVUL_SYNCHRONIZATION_INL_CL */
