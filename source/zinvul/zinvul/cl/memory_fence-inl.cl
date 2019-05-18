/*!
  \file memory_fence-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MEMORY_FENCE_INL_CL
#define ZINVUL_MEMORY_FENCE_INL_CL

#include "memory_fence.cl"

namespace zinvul {

/*!
  */
inline
void MemoryFence::performBarrier() noexcept
{
  barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
}

/*!
  */
inline
void MemoryFence::performLocalBarrier() noexcept
{
  barrier(CLK_LOCAL_MEM_FENCE);
}

/*!
  */
inline
void MemoryFence::performGlobalBarrier() noexcept
{
  barrier(CLK_GLOBAL_MEM_FENCE);
}

} // namespace zinvul

#endif /* ZINVUL_MEMORY_FENCE_INL_CL */
