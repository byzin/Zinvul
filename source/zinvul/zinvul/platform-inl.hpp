/*!
  \file platform-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_PLATFORM_INL_HPP
#define ZINVUL_PLATFORM_INL_HPP

#include "platform.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "sub_platform.hpp"
#include "zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] type No description.
  \return No description
  */
inline
bool Platform::hasSubPlatform(const SubPlatformType type) const noexcept
{
  const std::size_t index = zisc::cast<std::size_t>(type);
  ZISC_ASSERT(index < kNumOfSubPlatforms, "The index is out of range.");
  const bool result = sub_platform_list_[index] != nullptr;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::pmr::memory_resource* Platform::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const std::pmr::memory_resource* Platform::memoryResource() const noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \param [in] type No description.
  \return No description
  */
inline
SubPlatform* Platform::subPlatform(const SubPlatformType type) noexcept
{
  const std::size_t index = zisc::cast<std::size_t>(type);
  ZISC_ASSERT(index < kNumOfSubPlatforms, "The index is out of range.");
  UniqueSubPlatform& sub_platform = sub_platform_list_[index];
  return sub_platform.get();
}

/*!
  \details No detailed description

  \param [in] type No description.
  \return No description
  */
inline
const SubPlatform* Platform::subPlatform(const SubPlatformType type) const noexcept
{
  const std::size_t index = zisc::cast<std::size_t>(type);
  ZISC_ASSERT(index < kNumOfSubPlatforms, "The index is out of range.");
  const UniqueSubPlatform& sub_platform = sub_platform_list_[index];
  return sub_platform.get();
}

/*!
  \details No detailed description

  \param [in] type No description.
  \param [in] sub_platform No description.
  */
inline
void Platform::setSubPlatform(const SubPlatformType type,
                              UniqueSubPlatform&& sub_platform) noexcept
{
  const std::size_t index = zisc::cast<std::size_t>(type);
  ZISC_ASSERT(index < kNumOfSubPlatforms, "The index is out of range.");
  sub_platform_list_[index] = std::move(sub_platform);
}

} // namespace zinvul

#endif // ZINVUL_PLATFORM_INL_HPP
