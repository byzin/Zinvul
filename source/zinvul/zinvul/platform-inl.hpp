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
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "device_info.hpp"
#include "sub_platform.hpp"
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::vector<const DeviceInfo*>& Platform::deviceInfoList() const noexcept
{
  return *device_info_list_;
}

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
  const auto& sub_platform = sub_platform_list_[index];
  const bool result = (sub_platform != nullptr) && sub_platform->isAvailable();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool Platform::isDebugMode() const noexcept
{
  const bool result = is_debug_mode_ == Config::scalarResultTrue();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
IdData Platform::issueId() noexcept
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
zisc::pmr::memory_resource* Platform::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::memory_resource* Platform::memoryResource() const noexcept
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
  SharedSubPlatform& sub_platform = sub_platform_list_[index];
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
  const SharedSubPlatform& sub_platform = sub_platform_list_[index];
  return sub_platform.get();
}

/*!
  \details No detailed description

  \param [in] sub_platform No description.
  */
inline
void Platform::setSubPlatform(SharedSubPlatform&& sub_platform) noexcept
{
  const std::size_t index = zisc::cast<std::size_t>(sub_platform->type());
  ZISC_ASSERT(index < kNumOfSubPlatforms, "The index is out of range.");
  sub_platform_list_[index] = std::move(sub_platform);
}

} // namespace zinvul

#endif // ZINVUL_PLATFORM_INL_HPP
