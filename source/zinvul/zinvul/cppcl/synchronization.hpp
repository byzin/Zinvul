/*!
  \file synchronization.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_SYNCHRONIZATION_HPP
#define ZINVUL_CL_SYNCHRONIZATION_HPP

namespace zinvul {

namespace cl {

// Forward declaration
void barrier(const uint32b /* flags */) noexcept;

constexpr uint32b CLK_LOCAL_MEM_FENCE = 0b01u << 0;
constexpr uint32b CLK_GLOBAL_MEM_FENCE = 0b01u << 1;

//! Dummy function
inline
void barrier(const uint32b /* flags */) noexcept {}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_SYNCHRONIZATION_HPP
