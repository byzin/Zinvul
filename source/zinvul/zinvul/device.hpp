/*!
  \file device.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_HPP
#define ZINVUL_DEVICE_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/non_copyable.hpp"
#include "zisc/simple_memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
struct DeviceOptions
{
  // Common options
  const char* app_name_ = "Application";
  zisc::pmr::memory_resource* mem_resource_ = zisc::SimpleMemoryResource::sharedResource();
  zisc::pmr::memory_resource* work_resource_ = zisc::SimpleMemoryResource::sharedResource();
  DeviceType device_type_ = DeviceType::kCpu;
  uint32b app_version_major_ = 0;
  uint32b app_version_minor_ = 0;
  uint32b app_version_patch_ = 0;
  // CPU options
  uint32b num_of_threads_ = 1;
  // Vulkan options
  uint32b device_number_ = 0;
};

/*!
  */
class Device : private zisc::NonCopyable<Device>
{
 public:
  //! Initialize a device
  Device(DeviceOptions& options) noexcept;

  //!
  virtual ~Device() noexcept;


  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Returna a memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Returna a memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the max memory usage
  std::size_t maxMemoryUsage() const noexcept;

  //! Return the current memory usage
  std::size_t memoryUsage() const noexcept;

  //! Set the current memory usage
  void setMemoryUsage(const std::size_t memory_usage) noexcept;

  //! Wait this thread until all commands in the queue are completed
  virtual void waitForCompletion() noexcept = 0;

  //! Returna a work resource
  zisc::pmr::memory_resource* workResource() noexcept;

  //! Returna a work resource
  const zisc::pmr::memory_resource* workResource() const noexcept;

 private:
  zisc::pmr::memory_resource* mem_resource_;
  zisc::pmr::memory_resource* work_resource_;
  std::size_t memory_usage_ = 0;
  std::size_t max_memory_usage_ = 0;
};

// Type aliases
using UniqueDevice = zisc::UniqueMemoryPointer<Device>;

} // namespace zinvul

#include "device-inl.hpp"

#endif // ZINVUL_DEVICE_HPP
