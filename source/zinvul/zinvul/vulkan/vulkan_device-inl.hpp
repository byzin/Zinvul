/*!
  \file vulkan_device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_DEVICE_INL_HPP
#define ZINVUL_VULKAN_DEVICE_INL_HPP

#include "vulkan_device.hpp"
// Standard C++ library
#include <limits>
// Zinvul
#include "vulkan_device_info.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b VulkanDevice::invalidQueueIndex() noexcept
{
  const uint32b index = std::numeric_limits<uint32b>::max();
  return index;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDeviceInfo& VulkanDevice::vulkanDeviceInfo() const noexcept
{
  return *device_info_;
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_DEVICE_INL_HPP
