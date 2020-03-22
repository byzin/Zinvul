/*!
  \file cpu_sub_platform-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_SUB_PLATFORM_INL_HPP
#define ZINVUL_CPU_SUB_PLATFORM_INL_HPP

#include "cpu_sub_platform.hpp"
// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b CpuSubPlatform::maxTaskBatchSize() noexcept
{
  return 256;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t CpuSubPlatform::numOfThreads() const noexcept
{
  return zisc::cast<std::size_t>(num_of_threads_);
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::size_t CpuSubPlatform::taskBatchSize() const noexcept
{
  return zisc::cast<std::size_t>(task_batch_size_);
}

} // namespace zinvul

#endif // ZINVUL_CPU_SUB_PLATFORM_INL_HPP
