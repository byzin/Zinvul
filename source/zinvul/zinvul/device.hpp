/*!
  \file device.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_HPP
#define ZINVUL_DEVICE_HPP

// Zisc
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
enum class DeviceType
{
  kCpu,
  kVulkan
};

/*!
  */
class Device
{
 public:
  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Make a device
  static zisc::UniqueMemoryPointer<Device> makeDevice(
      const DeviceType type,
      const uint device_number) noexcept;

 private:
};

} // namespace zinvul

#endif // ZINVUL_DEVICE_HPP
