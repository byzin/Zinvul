/*!
  \file sub_platform-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_SUB_PLATFORM_INL_HPP
#define ZINVUL_SUB_PLATFORM_INL_HPP

#include "sub_platform.hpp"
// Standard C++ library
#include <atomic>
#include <memory>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
IdData SubPlatform::issueId() noexcept
{
  const uint32b id = id_count_++;
  IdData id_data{id};
  return id_data;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* SubPlatform::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::memory_resource* SubPlatform::memoryResource() const noexcept
{
  return mem_resource_;
}

} // namespace zinvul

#endif // ZINVUL_SUB_PLATFORM_INL_HPP
