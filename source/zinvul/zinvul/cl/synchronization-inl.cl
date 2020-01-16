/*!
  \file synchronization-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_SYNCHRONIZATION_INL_CL
#define ZINVUL_SYNCHRONIZATION_INL_CL

#include "synchronization.cl"
// Zinvul
#include "types.cl"

namespace zinvul {

/*!
  */
inline
void Synchronization::barrier(const int32b flags) noexcept
{
  ZINVUL_GLOBAL_NAMESPACE::barrier(flags);
}

/*!
  */
inline
void Synchronization::barrierGlobal() noexcept
{
  barrier(CLK_GLOBAL_MEM_FENCE);
}

/*!
  */
inline
void Synchronization::barrierLocal() noexcept
{
  barrier(CLK_LOCAL_MEM_FENCE);
}

/*!
  */
inline
void Synchronization::barrierAll() noexcept
{
  barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
}

/*!
  */
inline
void barrier(const int32b flags) noexcept
{
  Synchronization::barrier(flags);
}

} // namespace zinvul

#endif /* ZINVUL_SYNCHRONIZATION_INL_CL */
