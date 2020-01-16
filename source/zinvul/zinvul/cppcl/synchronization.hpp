/*!
  \file synchronization.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_SYNCHRONIZATION_HPP
#define ZINVUL_CL_SYNCHRONIZATION_HPP

// Zinvul
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

/*!
 \brief Memory address space
 */
enum cl_mem_fence_flags
{
  CLK_LOCAL_MEM_FENCE =  0b01 << 0,
  CLK_GLOBAL_MEM_FENCE = 0b01 << 1
};

// Forward declaration
void barrier(const int32b /* flags */) noexcept;

//! Dummy function
inline
void barrier(const int32b /* flags */) noexcept {}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_SYNCHRONIZATION_HPP
