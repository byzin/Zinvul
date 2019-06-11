/*!
  \file memory_fence.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MEMORY_FENCE_CL
#define ZINVUL_MEMORY_FENCE_CL

namespace zinvul {

//! Any variables stored in local or global memories are flushed
void performBarrier() noexcept;

//! Any variables stored in local memory are flushed
void performLocalBarrier() noexcept;

//! Any variables stored in global memory are flushed
void performGlobalBarrier() noexcept;

} // namespace zinvul

#include "memory_fence-inl.cl"

#endif /* ZINVUL_MEMORY_FENCE_CL */
