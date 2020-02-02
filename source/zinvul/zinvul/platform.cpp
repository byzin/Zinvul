/*!
  \file platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "platform.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <utility>
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "platform_options.hpp"
#include "sub_platform.hpp"
#include "zinvul_config.hpp"
#include "cpu/cpu_sub_platform.hpp"
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
#include "vulkan/vulkan_sub_platform.hpp"
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM

namespace zinvul {

/*!
  \details No detailed description
  */
Platform::Platform() noexcept
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
Platform::Platform(Platform&& other) noexcept
{
  std::swap(mem_resource_, other.mem_resource_);
  std::move(other.sub_platform_list_.begin(),
            other.sub_platform_list_.end(),
            sub_platform_list_.begin());
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
Platform& Platform::operator=(Platform&& other) noexcept
{
  std::swap(mem_resource_, other.mem_resource_);
  std::move(other.sub_platform_list_.begin(),
            other.sub_platform_list_.end(),
            sub_platform_list_.begin());
  return *this;
}

/*!
  \details No detailed description
  */
void Platform::destroy() noexcept
{
  for (auto& sub_platform : sub_platform_list_)
    sub_platform.reset();
  mem_resource_ = nullptr;
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void Platform::initialize(PlatformOptions& platform_options)
{
  // Clear the previous platform data first 
  destroy();

  mem_resource_ = platform_options.memoryResource();

  // Initialize sub-platforms
  initCpuSubPlatform(platform_options);
  if (platform_options.vulkanSubPlatformEnabled())
    initVulkanSubPlatform(platform_options);
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void Platform::initCpuSubPlatform(PlatformOptions& platform_options)
{
  std::pmr::memory_resource* mem_resource = memoryResource();
  auto sub_platform = zisc::pmr::allocateUnique<CpuSubPlatform>(mem_resource);
  sub_platform->initialize(platform_options);
  setSubPlatform(SubPlatformType::kCpu, std::move(sub_platform));
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void Platform::initVulkanSubPlatform(PlatformOptions& platform_options)
{
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
  std::pmr::memory_resource* mem_resource = memoryResource();
  auto sub_platform = zisc::pmr::allocateUnique<VulkanSubPlatform>(mem_resource);
  sub_platform->initialize(platform_options);
  setSubPlatform(SubPlatformType::kVulkan, std::move(sub_platform));
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
  static_cast<void>(platform_options);
}

} // namespace zinvul
