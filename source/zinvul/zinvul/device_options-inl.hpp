/*!
  \file device_options-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_OPTIONS_INL_HPP
#define ZINVUL_DEVICE_OPTIONS_INL_HPP

#include "device_options.hpp"
// Standard C++ library
#include <string>
#include <string_view>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
inline
DeviceOptions::DeviceOptions(zisc::pmr::memory_resource* mem_resource) noexcept :
    mem_resource_{mem_resource},
    app_name_{"Application", zisc::pmr::string::allocator_type{mem_resource_}},
    device_type_{SubPlatformType::kCpu},
    app_version_major_{0},
    app_version_minor_{0},
    app_version_patch_{0},
    debug_mode_{Config::scalarResultFalse()},
    cpu_num_of_threads_{0},
    cpu_task_batch_size_{64},
    vulkan_device_number_{0}
{
  initialize();
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
inline
DeviceOptions::DeviceOptions(DeviceOptions&& other) noexcept :
    mem_resource_{other.mem_resource_},
    app_name_{std::move(other.app_name_)},
    device_type_{other.device_type_},
    app_version_major_{other.app_version_major_},
    app_version_minor_{other.app_version_minor_},
    app_version_patch_{other.app_version_patch_},
    debug_mode_{other.debug_mode_},
    cpu_num_of_threads_{other.cpu_num_of_threads_},
    cpu_task_batch_size_{other.cpu_task_batch_size_},
    vulkan_device_number_{other.vulkan_device_number_}
{
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
inline
DeviceOptions& DeviceOptions::operator=(DeviceOptions&& other) noexcept
{
  mem_resource_ = other.mem_resource_;
  app_name_ = std::move(other.app_name_);
  device_type_ = other.device_type_;
  app_version_major_ = other.app_version_major_;
  app_version_minor_ = other.app_version_minor_;
  app_version_patch_ = other.app_version_patch_;
  debug_mode_ = other.debug_mode_;
  cpu_num_of_threads_ = other.cpu_num_of_threads_;
  cpu_task_batch_size_ = other.cpu_task_batch_size_;
  vulkan_device_number_ = other.vulkan_device_number_;
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* DeviceOptions::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::memory_resource* DeviceOptions::memoryResource() const noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view DeviceOptions::applicationName() const noexcept
{
  std::string_view name = app_name_;
  return name;
}

/*!
  \details No detailed description

  \return No description
  */
inline
SubPlatformType DeviceOptions::subPlatformType() const noexcept
{
  return device_type_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b DeviceOptions::applicationVersionMajor() const noexcept
{
  return app_version_major_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b DeviceOptions::applicationVersionMinor() const noexcept
{
  return app_version_minor_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b DeviceOptions::applicationVersionPatch() const noexcept
{
  return app_version_patch_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool DeviceOptions::debugMode() const noexcept
{
  const bool result = debug_mode_ == Config::scalarResultTrue();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b DeviceOptions::cpuNumOfThreads() const noexcept
{
  return cpu_num_of_threads_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b DeviceOptions::cpuTaskBatchSize() const noexcept
{
  return cpu_task_batch_size_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b DeviceOptions::vulkanDeviceNumber() const noexcept
{
  return vulkan_device_number_;
}

/*!
  \details No detailed description

  \param [in] name No description.
  */
inline
void DeviceOptions::setApplicationName(std::string_view name) noexcept
{
  app_name_ = name;
}

/*!
  \details No detailed description

  \param [in] device_type No description.
  */
inline
void DeviceOptions::setSubPlatformType(const SubPlatformType device_type) noexcept
{
  device_type_ = device_type;
}

/*!
  \details No detailed description

  \param [in] major No description.
  */
inline
void DeviceOptions::setApplicationVersionMajor(const uint32b major) noexcept
{
  app_version_major_ = major;
}

/*!
  \details No detailed description

  \param [in] minor No description.
  */
inline
void DeviceOptions::setApplicationVersionMinor(const uint32b minor) noexcept
{
  app_version_minor_ = minor;
}

/*!
  \details No detailed description

  \param [in] patch No description.
  */
inline
void DeviceOptions::setApplicationVersionPatch(const uint32b patch) noexcept
{
  app_version_patch_ = patch;
}

/*!
  \details No detailed description

  \param [in] is_debug_mode No description.
  */
inline
void DeviceOptions::setDebugMode(const bool is_debug_mode) noexcept
{
  debug_mode_ = is_debug_mode ? Config::scalarResultTrue() :
                                Config::scalarResultFalse();
}

/*!
  \details No detailed description

  \param [in] num_of_threads No description.
  */
inline
void DeviceOptions::setCpuNumOfThreads(const uint32b num_of_threads) noexcept
{
  cpu_num_of_threads_ = num_of_threads;
}

/*!
  \details No detailed description

  \param [in] batch_size No description.
  */
inline
void DeviceOptions::setCpuTaskBatchSize(const uint32b batch_size) noexcept
{
  cpu_task_batch_size_ = batch_size;
}

/*!
  \details No detailed description

  \param [in] device_number No description.
  */
inline
void DeviceOptions::setVulkanDeviceNumber(const uint32b device_number) noexcept
{
  vulkan_device_number_ = device_number;
}

/*!
  \details No detailed description
  */
inline
void DeviceOptions::initialize() noexcept
{
#if defined(Z_DEBUG_MODE)
  setDebugMode(true);
#endif // Z_DEBUG_MODE
}

} // namespace zinvul

#endif // ZINVUL_DEVICE_OPTIONS_INL_HPP
