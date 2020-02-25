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
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class DeviceInfo;

/*!
  \brief No brief description

  No detailed description.
  */
class Device : private zisc::NonCopyable<Device>
{
 public:
  //! Initialize the device
  Device(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move a data
  Device(Device&& other) noexcept;

  //! Finalize the device
  virtual ~Device() noexcept;


  //! Move a data
  Device& operator=(Device&& other) noexcept;


  //! Destroy the data
  void destroy() noexcept;

  //! Return the underlying device info
  virtual const DeviceInfo& deviceInfo() const noexcept = 0;

  //! Returna a memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Returna a memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the number of underlying command queues
  virtual std::size_t numOfQueues() const noexcept = 0;

  //! Return the peak memory usage of the heap of the given index
  virtual std::size_t peakMemoryUsage(const std::size_t index) const noexcept = 0;

  //! Return the current memory usage of the heap of the given index
  virtual std::size_t totalMemoryUsage(const std::size_t index) const noexcept = 0;

  //! Return the sub-platform type
  virtual SubPlatformType type() const noexcept = 0;

//  //! Wait this thread until all commands in the device are completed
//  virtual void waitForCompletion() const noexcept = 0;
//
//  //! Wait this thread until all commands in the queues are completed
//  virtual void waitForCompletion(const QueueType queue_type) const noexcept = 0;
//
//  //! Wait this thread until all commands in the queue are completed
//  virtual void waitForCompletion(const QueueType queue_type,
//                                 const uint32b queue_index) const noexcept = 0;

 protected:
  //! Destroy the data
  virtual void destroyData() noexcept = 0;

 private:
  zisc::pmr::memory_resource* mem_resource_;
};

// Type aliases
using UniqueDevice = zisc::pmr::unique_ptr<Device>;

} // namespace zinvul

#include "device-inl.hpp"

#endif // ZINVUL_DEVICE_HPP
