/*!
  \file platform_options-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_PLATFORM_OPTIONS_INL_HPP
#define ZINVUL_PLATFORM_OPTIONS_INL_HPP

#include "platform_options.hpp"
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
PlatformOptions::PlatformOptions(zisc::pmr::memory_resource* mem_resource)
    noexcept :
        mem_resource_{mem_resource},
        platform_name_{"Platform",
                       zisc::pmr::string::allocator_type{mem_resource_}},
        platform_version_major_{0},
        platform_version_minor_{0},
        platform_version_patch_{0},
        debug_mode_enabled_{Config::scalarResultFalse()},
        cpu_num_of_threads_{0},
        cpu_task_batch_size_{32},
        vulkan_sub_platform_enabled_{Config::scalarResultTrue()},
        vulkan_instance_ptr_{nullptr},
        vulkan_get_proc_addr_ptr_{nullptr}
{
  initialize();
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
inline
PlatformOptions::PlatformOptions(PlatformOptions&& other) noexcept :
    mem_resource_{other.mem_resource_},
    platform_name_{std::move(other.platform_name_)},
    platform_version_major_{other.platform_version_major_},
    platform_version_minor_{other.platform_version_minor_},
    platform_version_patch_{other.platform_version_patch_},
    debug_mode_enabled_{other.debug_mode_enabled_},
    cpu_num_of_threads_{other.cpu_num_of_threads_},
    cpu_task_batch_size_{other.cpu_task_batch_size_},
    vulkan_sub_platform_enabled_{other.vulkan_sub_platform_enabled_},
    vulkan_instance_ptr_{other.vulkan_instance_ptr_},
    vulkan_get_proc_addr_ptr_{other.vulkan_get_proc_addr_ptr_}
{
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
inline
PlatformOptions& PlatformOptions::operator=(PlatformOptions&& other) noexcept
{
  mem_resource_ = other.mem_resource_;
  platform_name_ = std::move(other.platform_name_);
  platform_version_major_ = other.platform_version_major_;
  platform_version_minor_ = other.platform_version_minor_;
  platform_version_patch_ = other.platform_version_patch_;
  debug_mode_enabled_ = other.debug_mode_enabled_;
  cpu_num_of_threads_ = other.cpu_num_of_threads_;
  cpu_task_batch_size_ = other.cpu_task_batch_size_;
  vulkan_sub_platform_enabled_ = other.vulkan_sub_platform_enabled_;
  vulkan_instance_ptr_ = other.vulkan_instance_ptr_;
  vulkan_get_proc_addr_ptr_ = other.vulkan_get_proc_addr_ptr_;
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b PlatformOptions::cpuNumOfThreads() const noexcept
{
  return cpu_num_of_threads_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b PlatformOptions::cpuTaskBatchSize() const noexcept
{
  return cpu_task_batch_size_;
}

/*!
  \details No detailed description

  \param [in] debug_mode_enabled No description.
  */
inline
void PlatformOptions::enableDebugMode(const bool debug_mode_enabled) noexcept
{
  debug_mode_enabled_ = debug_mode_enabled ? Config::scalarResultTrue() :
                                             Config::scalarResultFalse();
}

/*!
  \details No detailed description

  \param [in] sub_platform_enabled No description.
  */
inline
void PlatformOptions::enableVulkanSubPlatform(const bool sub_platform_enabled)
    noexcept
{
  vulkan_sub_platform_enabled_ = sub_platform_enabled
      ? Config::scalarResultTrue()
      : Config::scalarResultFalse();
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* PlatformOptions::memoryResource() noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::memory_resource* PlatformOptions::memoryResource() const noexcept
{
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view PlatformOptions::platformName() const noexcept
{
  std::string_view name = platform_name_;
  return name;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b PlatformOptions::platformVersionMajor() const noexcept
{
  return platform_version_major_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b PlatformOptions::platformVersionMinor() const noexcept
{
  return platform_version_minor_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b PlatformOptions::platformVersionPatch() const noexcept
{
  return platform_version_patch_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool PlatformOptions::debugModeEnabled() const noexcept
{
  const bool result = debug_mode_enabled_ == Config::scalarResultTrue();
  return result;
}

/*!
  \details No detailed description

  \param [in] num_of_threads No description.
  */
inline
void PlatformOptions::setCpuNumOfThreads(const uint32b num_of_threads) noexcept
{
  cpu_num_of_threads_ = num_of_threads;
}

/*!
  \details No detailed description

  \param [in] task_batch_size No description.
  */
inline
void PlatformOptions::setCpuTaskBatchSize(const uint32b task_batch_size) noexcept
{
  cpu_task_batch_size_ = task_batch_size;
}

/*!
  \details No detailed description

  \param [in] name No description.
  */
inline
void PlatformOptions::setPlatformName(std::string_view name) noexcept
{
  platform_name_ = name;
}

/*!
  \details No detailed description

  \param [in] major No description.
  */
inline
void PlatformOptions::setPlatformVersionMajor(const uint32b major) noexcept
{
  platform_version_major_ = major;
}

/*!
  \details No detailed description

  \param [in] minor No description.
  */
inline
void PlatformOptions::setPlatformVersionMinor(const uint32b minor) noexcept
{
  platform_version_minor_ = minor;
}

/*!
  \details No detailed description

  \param [in] patch No description.
  */
inline
void PlatformOptions::setPlatformVersionPatch(const uint32b patch) noexcept
{
  platform_version_patch_ = patch;
}

/*!
  \details No detailed description

  \param [in] instance_ptr No description.
  */
inline
void PlatformOptions::setVulkanInstancePtr(void* instance_ptr) noexcept
{
  vulkan_instance_ptr_ = instance_ptr;
}

/*!
  \details No detailed description

  \param [in] get_proc_addr_ptr No description.
  */
inline
void PlatformOptions::setVulkanGetProcAddrPtr(void* get_proc_addr_ptr) noexcept
{
  vulkan_get_proc_addr_ptr_ = get_proc_addr_ptr;
}

/*!
  \details No detailed description

  \return No description
  */
inline
void* PlatformOptions::vulkanInstancePtr() noexcept
{
  return vulkan_instance_ptr_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
void* PlatformOptions::vulkanGetProcAddrPtr() noexcept
{
  return vulkan_get_proc_addr_ptr_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool PlatformOptions::vulkanSubPlatformEnabled() const noexcept
{
  const bool result = vulkan_sub_platform_enabled_ == Config::scalarResultTrue();
  return result;
}

/*!
  \details No detailed description
  */
inline
void PlatformOptions::initialize() noexcept
{
#if defined(Z_DEBUG_MODE)
  enableDebugMode(true);
#endif // Z_DEBUG_MODE
}

} // namespace zinvul

#endif // ZINVUL_PLATFORM_OPTIONS_INL_HPP
