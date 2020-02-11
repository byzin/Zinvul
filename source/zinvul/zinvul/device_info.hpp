/*!
  \file device_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_INFO_HPP
#define ZINVUL_DEVICE_INFO_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
// Zinvul
#include "zinvul_config.hpp"

namespace zinvul {

/*!
  \brief No brief description

  No detailed description.
  */
class DeviceInfo : public zisc::NonCopyable<DeviceInfo>
{
 public:
  //! Initialize the device info
  DeviceInfo() noexcept;

  //! Move a data
  DeviceInfo(DeviceInfo&& other) noexcept;

  //! Finalize the device info
  virtual ~DeviceInfo() noexcept;


  //! Move a data
  DeviceInfo& operator=(DeviceInfo&& other) noexcept;


  //! Return the amount of actual device memory currently available in bytes
  virtual std::size_t availableMemory(const std::size_t heap_index) const noexcept = 0;

  //! Return the maximum size of an allocation in bytes
  virtual std::size_t maxAllocationSize() const noexcept = 0;

  //! Return the maximum work group count
  virtual std::array<uint32b, 3> maxWorkGroupCount() const noexcept = 0;

  //! Return the device name
  virtual std::string_view name() const noexcept = 0;

  //! Return the number of heaps of the device local
  virtual std::size_t numOfHeaps() const noexcept = 0;

  //! Return the amount of actual device memory in bytes
  virtual std::size_t totalMemory(const std::size_t heap_index) const noexcept = 0;

  //! Return the sub-platform type
  virtual SubPlatformType type() const noexcept = 0;

  //! Return the vendor name
  virtual std::string_view vendorName() const noexcept = 0;

  //! Return the local work group size of the device
  virtual uint32b workGroupSize() const noexcept = 0;
};

} // namespace zinvul

#endif // ZINVUL_DEVICE_INFO_HPP
