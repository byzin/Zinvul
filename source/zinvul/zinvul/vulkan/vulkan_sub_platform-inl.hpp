/*!
  \file vulkan_sub_platform-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_SUB_PLATFORM_INL_HPP
#define ZINVUL_VULKAN_SUB_PLATFORM_INL_HPP

#include "vulkan_sub_platform.hpp"
// Standard C++ library
#include <vector>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "utility/vulkan_dispatch_loader.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::vector<VkPhysicalDevice>& VulkanSubPlatform::deviceList() const noexcept
{
  return *device_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VulkanDispatchLoader& VulkanSubPlatform::dispatcher() const noexcept
{
  return *dispatcher_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
VkInstance& VulkanSubPlatform::instance() noexcept
{
  return instance_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const VkInstance& VulkanSubPlatform::instance() const noexcept
{
  return instance_;
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_SUB_PLATFORM_INL_HPP
