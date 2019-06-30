/*!
  \file synchronization.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_SYNCHRONIZATION_CL
#define ZINVUL_SYNCHRONIZATION_CL

namespace zinvul {

//! Any variables stored in local or global memories are flushed
void barrierMemory() noexcept;

//! Any variables stored in local memory are flushed
void barrierLocalMemory() noexcept;

//! Any variables stored in global memory are flushed
void barrierGlobalMemory() noexcept;

} // namespace zinvul

#include "synchronization-inl.cl"

#endif /* ZINVUL_SYNCHRONIZATION_CL */
