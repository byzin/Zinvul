/*!
  \file platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_PLATFORM_HPP
#define ZINVUL_PLATFORM_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <vector>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "device.hpp"
#include "sub_platform.hpp"
#include "zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class DeviceInfo;
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class Platform : private zisc::NonCopyable<Platform>
{
 public:
  //! Create an empty platform
  Platform() noexcept;

  //! Move a data
  Platform(Platform&& other) noexcept;

  //! Finalize the platform
  ~Platform() noexcept;


  //! Move a data
  Platform& operator=(Platform&& other) noexcept;


  //! Destroy the platform
  void destroy() noexcept;

  //! Return the device info list
  const zisc::pmr::vector<const DeviceInfo*>& deviceInfoList() const noexcept;

  //! Check if the platform has the sub-platform of the given type
  bool hasSubPlatform(const SubPlatformType type) const noexcept;

  //! Make a unique device
  UniqueDevice makeDevice(const std::size_t device_index);

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Initialize the platform
  void initialize(PlatformOptions& platform_options);

  //! Return the sub-platform of the given type
  SubPlatform* subPlatform(const SubPlatformType type) noexcept;

  //! Return the sub-platform of the given type
  const SubPlatform* subPlatform(const SubPlatformType type) const noexcept;

  //! Update the device info list
  void updateDeviceInfoList() noexcept;

 private:
  //! Create a CPU sub-platform
  void initCpuSubPlatform(PlatformOptions& platform_options);

  //! Create a Vulkan sub-platform
  void initVulkanSubPlatform(PlatformOptions& platform_options);

  //! Set a sub-platform of the given type
  void setSubPlatform(const SubPlatformType type,
                      UniqueSubPlatform&& sub_platform) noexcept;


  static constexpr std::size_t kNumOfSubPlatforms = 2;


  zisc::pmr::memory_resource* mem_resource_ = nullptr;
  std::array<UniqueSubPlatform, kNumOfSubPlatforms> sub_platform_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<const DeviceInfo*>> device_info_list_;
};

// Type aliases
using UniquePlatform = zisc::pmr::unique_ptr<Platform>;

//! Make a unique platform
UniquePlatform makePlatform(zisc::pmr::memory_resource* mem_resource) noexcept;

} // namespace zinvul

#include "platform-inl.hpp"

#endif // ZINVUL_PLATFORM_HPP
