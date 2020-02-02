/*!
  \file device.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_HPP
#define ZINVUL_DEVICE_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class DeviceOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class Device : private zisc::NonCopyable<Device>
{
 public:
  /*!
    \brief No brief description

    No detailed description.
    */
  struct Param
  {
    SubPlatformType type_;
    uint32b number_;
  };


  //! Initialize a device
  Device(DeviceOptions& options) noexcept;

  //!
  virtual ~Device() noexcept;


  //! Return the device type
  virtual SubPlatformType subPlatformType() const noexcept = 0;

  //! Return the device type and device number list
  static zisc::pmr::vector<Param> getDeviceParamList(
      std::pmr::memory_resource* mem_resource);

  //! Returna a memory resource
  std::pmr::memory_resource* memoryResource() noexcept;

  //! Returna a memory resource
  const std::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the device name
  virtual std::string_view name() const noexcept = 0;

  //! Return the peak memory usage
  std::size_t peakMemoryUsage() const noexcept;

  //! Return the subgroup size
  virtual uint32b subgroupSize() const noexcept = 0;

  //! Return the current memory usage
  std::size_t totalMemoryUsage() const noexcept;

  //! Return the vendor name
  virtual std::string_view vendorName() const noexcept = 0;

  //! Wait this thread until all commands in the device are completed
  virtual void waitForCompletion() const noexcept = 0;

  //! Wait this thread until all commands in the queues are completed
  virtual void waitForCompletion(const QueueType queue_type) const noexcept = 0;

  //! Wait this thread until all commands in the queue are completed
  virtual void waitForCompletion(const QueueType queue_type,
                                 const uint32b queue_index) const noexcept = 0;

 protected:
  std::size_t total_memory_usage_ = 0;
  std::size_t peak_memory_usage_ = 0;

 private:
  std::pmr::memory_resource* mem_resource_;
};

// Type aliases
using UniqueDevice = zisc::pmr::unique_ptr<Device>;

} // namespace zinvul

#include "device-inl.hpp"

#endif // ZINVUL_DEVICE_HPP
