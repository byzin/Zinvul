/*!
  \file sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_SUB_PLATFORM_HPP
#define ZINVUL_SUB_PLATFORM_HPP

// Standard C++ library
#include <atomic>
#include <memory>
#include <vector>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "device.hpp"
#include "device_info.hpp"
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"

namespace zinvul {

// Forward declaration
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class SubPlatform : private zisc::NonCopyable<SubPlatform>
{
 public:
  //! Create an empty sub-platform
  SubPlatform() noexcept;

  //! Finalize the platform
  virtual ~SubPlatform() noexcept;


  //! Destroy the sub-platform
  void destroy() noexcept;

  //! Add the underlying device info into the given list
  virtual void getDeviceInfoList(
      zisc::pmr::vector<const DeviceInfo*>& device_info_list) const noexcept = 0;

  //! Initialize the sub-platform
  void initialize(PlatformOptions& platform_options);

  //! Check if the sub-platform is in debug mode
  bool isDebugMode() const noexcept;

  //! Issue an ID of an object
  IdData issueId() noexcept;

  //! Make a unique device
  virtual UniqueDevice makeDevice(const DeviceInfo& device_info) = 0;

  //! Return the underlying memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the number of available devices 
  virtual std::size_t numOfDevices() const noexcept = 0;

  //! Return the sub-platform type
  virtual SubPlatformType type() const noexcept = 0;

  //! Update the device info list
  virtual void updateDeviceInfoList() noexcept = 0;

 protected:
  //! Destroy the sub-platform
  virtual void destroyData() noexcept = 0;

  //! Initialize the sub-platform
  virtual void initData(PlatformOptions& platform_options) = 0;

 private:
  //! Set debug mode
  void setDebugMode(const bool is_debug_mode) noexcept;


  zisc::pmr::memory_resource* mem_resource_ = nullptr;
  std::atomic<uint32b> id_count_ = 0;
  int32b is_debug_mode_ = Config::scalarResultFalse();
};

// Type aliases
using UniqueSubPlatform = zisc::pmr::unique_ptr<SubPlatform>;

} // namespace zinvul

#include "sub_platform-inl.hpp"

#endif // ZINVUL_SUB_PLATFORM_HPP
