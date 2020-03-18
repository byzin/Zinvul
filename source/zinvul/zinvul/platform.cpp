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
#include <vector>
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "device.hpp"
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
Platform::Platform(Platform&& other) noexcept :
    device_info_list_{std::move(other.device_info_list_)},
    id_count_{other.id_count_.load()},
    is_debug_mode_{other.is_debug_mode_}
{
  std::swap(mem_resource_, other.mem_resource_);
  std::move(other.sub_platform_list_.begin(),
            other.sub_platform_list_.end(),
            sub_platform_list_.begin());
}

/*!
  \details No detailed description
  */
Platform::~Platform() noexcept
{
  destroy();
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
  device_info_list_ = std::move(other.device_info_list_);
  id_count_ = other.id_count_.load();
  is_debug_mode_ = other.is_debug_mode_;
  return *this;
}

/*!
  \details No detailed description
  */
void Platform::destroy() noexcept
{
  device_info_list_.reset();
  for (auto& sub_platform : sub_platform_list_)
    sub_platform.reset();
  mem_resource_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] device_index No description.
  \return No description
  */
SharedDevice Platform::makeDevice(const std::size_t device_index)
{
  const DeviceInfo* info = deviceInfoList()[device_index];
  SubPlatform* sub_platform = subPlatform(info->type());
  auto device = sub_platform->makeDevice(*info);
  return device;
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
  setDebugMode(platform_options.debugModeEnabled());

  // Initialize sub-platforms
  initCpuSubPlatform(platform_options);
  if (platform_options.vulkanSubPlatformEnabled())
    initVulkanSubPlatform(platform_options);

  // Get device info list
  using DeviceInfoList = zisc::pmr::vector<const DeviceInfo*>;
  DeviceInfoList new_info_list{DeviceInfoList::allocator_type{mem_resource_}};
  device_info_list_ = zisc::pmr::allocateUnique<DeviceInfoList>(
      mem_resource_,
      std::move(new_info_list));
  updateDeviceInfoList();
}

/*!
  \details No detailed description
  */
void Platform::updateDeviceInfoList() noexcept
{
  device_info_list_->clear();
  // 
  for (auto& sub_platform : sub_platform_list_) {
    if (sub_platform)
      sub_platform->updateDeviceInfoList();
  }
  //
  std::size_t num_of_devices = 0;
  for (const auto& sub_platform : sub_platform_list_) {
    if (sub_platform)
      num_of_devices += sub_platform->numOfDevices();
  }
  device_info_list_->reserve(num_of_devices);
  //
  for (const auto& sub_platform : sub_platform_list_) {
    if (sub_platform)
      sub_platform->getDeviceInfoList(*device_info_list_);
  }
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void Platform::initCpuSubPlatform(PlatformOptions& platform_options)
{
  zisc::pmr::polymorphic_allocator<CpuSubPlatform> alloc{memoryResource()};
  auto sub_platform = std::allocate_shared<CpuSubPlatform>(alloc, this);
  std::weak_ptr<CpuSubPlatform> own{sub_platform};
  sub_platform->initialize(std::move(own), platform_options);
  setSubPlatform(std::move(sub_platform));
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void Platform::initVulkanSubPlatform(PlatformOptions& platform_options)
{
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
  zisc::pmr::polymorphic_allocator<CpuSubPlatform> alloc{memoryResource()};
  auto sub_platform = std::allocate_shared<VulkanSubPlatform>(alloc, this);
  std::weak_ptr<VulkanSubPlatform> own{sub_platform};
  sub_platform->initialize(std::move(own), platform_options);
  setSubPlatform(std::move(sub_platform));
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
  static_cast<void>(platform_options);
}

/*!
  \details No detailed description

  \param [in] is_debug_mode No description.
  */
void Platform::setDebugMode(const bool is_debug_mode) noexcept
{
  is_debug_mode_ = is_debug_mode ? Config::scalarResultTrue()
                                 : Config::scalarResultFalse();
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  \return No description
  */
UniquePlatform makePlatform(zisc::pmr::memory_resource* mem_resource) noexcept
{
  auto platform = zisc::pmr::allocateUnique<Platform>(mem_resource);
  return platform;
}

} // namespace zinvul
