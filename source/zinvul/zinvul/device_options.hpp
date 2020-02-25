/*!
  \file device_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_OPTIONS_HPP
#define ZINVUL_DEVICE_OPTIONS_HPP

// Standard C++ library
#include <string>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"

namespace zinvul {

/*!
  \brief No brief description

  No detailed description.
  */
class DeviceOptions : private zisc::NonCopyable<DeviceOptions>
{
 public:
  //! Create a device options
  DeviceOptions(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move option data
  DeviceOptions(DeviceOptions&& other) noexcept;


  //! Move option data
  DeviceOptions& operator=(DeviceOptions&& other) noexcept;


  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the application name
  std::string_view applicationName() const noexcept;

  //! Return the device type
  SubPlatformType subPlatformType() const noexcept;

  //! Return the value of the major component of the application version number
  uint32b applicationVersionMajor() const noexcept;

  //! Return the value of the minor component of the application version number
  uint32b applicationVersionMinor() const noexcept;

  //! Return the value of the patch component of the application version number
  uint32b applicationVersionPatch() const noexcept;

  //! Return whether the debug mode is enabled
  bool debugMode() const noexcept;

  //! Return the number of threads used in the CPU backend
  uint32b cpuNumOfThreads() const noexcept;

  //! Return the number of tasks that a thread processes at once
  uint32b cpuTaskBatchSize() const noexcept;

  //! Return the device number used in Vulkan
  uint32b vulkanDeviceNumber() const noexcept;

  //! Set the application name
  void setApplicationName(std::string_view name) noexcept;

  //! Set the device type
  void setSubPlatformType(const SubPlatformType device_type) noexcept;

  //! Set the value of the major component of the application version number
  void setApplicationVersionMajor(const uint32b major) noexcept;

  //! Set the value of the minor component of the application version number
  void setApplicationVersionMinor(const uint32b minor) noexcept;

  //! Set the value of the patch component of the application version number
  void setApplicationVersionPatch(const uint32b patch) noexcept;

  //! Set the debug mode
  void setDebugMode(const bool is_debug_mode) noexcept;

  //! Set the number of threads used in the CPU backend
  void setCpuNumOfThreads(const uint32b num_of_threads) noexcept;

  //! Set the number of tasks that a thread processes at once
  void setCpuTaskBatchSize(const uint32b batch_size) noexcept;

  //! Set the device number used in Vulkan
  void setVulkanDeviceNumber(const uint32b device_number) noexcept;

 private:
  //! Initialize options
  void initialize() noexcept;


  // Memory resources
  zisc::pmr::memory_resource* mem_resource_;
  // Common options
  zisc::pmr::string app_name_;
  SubPlatformType device_type_;
  uint32b app_version_major_;
  uint32b app_version_minor_;
  uint32b app_version_patch_;
  uint32b debug_mode_; //!< Enable debugging in Zinvul
  // CPU options
  uint32b cpu_num_of_threads_; //!< 0: Use optimal number of threads
  uint32b cpu_task_batch_size_; //!< The number of tasks that a thread processes at once
  // Vulkan options
  uint32b vulkan_device_number_; //!< The device number used in Vulkan, please see the result of vulkaninfo command
};

} // namespace zinvul

#include "device_options-inl.hpp"

#endif // ZINVUL_DEVICE_OPTIONS_HPP
