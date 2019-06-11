/*!
  \file device.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_HPP
#define ZINVUL_DEVICE_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>
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
  zisc::pmr::memory_resource* mem_resource_ =
      zisc::SimpleMemoryResource::sharedResource();
  zisc::pmr::memory_resource* work_resource_ =
      zisc::SimpleMemoryResource::sharedResource();
  DeviceType device_type_ = DeviceType::kCpu;
  uint32b app_version_major_ = 0;
  uint32b app_version_minor_ = 0;
  uint32b app_version_patch_ = 0;
  // CPU options
  uint32b cpu_num_of_threads_ = 0; //!< 0: Use optimal number of threads
  uint32b cpu_task_bucket_size_ = 64; //!< Bucket size of tasks per thread
  // Vulkan options
  uint32b vulkan_device_number_ = 0;
  bool vulkan_enable_validation_layers_ = true;
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


  //! Return the current memory usage of a device
  std::size_t deviceMemoryUsage() const noexcept;

  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Return the current memory usage of the host
  std::size_t hostMemoryUsage() const noexcept;

  //! Returna a memory resource
  zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Returna a memory resource
  const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the device name
  virtual std::string_view name() const noexcept = 0;

  //! Return the peak memory usage of a device
  std::size_t peakDeviceMemoryUsage() const noexcept;

  //! Return the peak memory usage of the host
  std::size_t peakHostMemoryUsage() const noexcept;

  //! Set the current memory usage of a device
  void setDeviceMemoryUsage(const std::size_t memory_usage) noexcept;

  //! Set the current memory usage of the host
  void setHostMemoryUsage(const std::size_t memory_usage) noexcept;

  //! Return the subgroup size
  virtual uint32b subgroupSize() const noexcept = 0;

  //! Wait this thread until all commands in the queue are completed
  virtual void waitForCompletion() const noexcept = 0;

  //! Returna a work resource
  zisc::pmr::memory_resource* workResource() noexcept;

  //! Returna a work resource
  const zisc::pmr::memory_resource* workResource() const noexcept;

  //! Return the vendor name
  virtual std::string_view vendorName() const noexcept = 0;

 private:
  zisc::pmr::memory_resource* mem_resource_;
  zisc::pmr::memory_resource* work_resource_;
  std::size_t device_memory_usage_ = 0;
  std::size_t host_memory_usage_ = 0;
  std::size_t peak_device_memory_usage_ = 0;
  std::size_t peak_host_memory_usage_ = 0;
};

// Type aliases
using UniqueDevice = zisc::UniqueMemoryPointer<Device>;

} // namespace zinvul

#include "device-inl.hpp"

#endif // ZINVUL_DEVICE_HPP
