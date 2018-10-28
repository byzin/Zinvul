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
  zisc::pmr::memory_resource* mem_resource_ = zisc::SimpleMemoryResource::sharedResource();
  DeviceType device_type_ = DeviceType::kCpu;
  // CPU options
  uint32b num_of_threads_ = 1;
  // Vulkan options
  uint32b device_number_ = 0;
  uint32b padding_;
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

 private:
  zisc::pmr::memory_resource* mem_resource_;
};

// Type aliases
using UniqueDevice = zisc::UniqueMemoryPointer<Device>;

} // namespace zinvul

#include "device-inl.hpp"

#endif // ZINVUL_DEVICE_HPP
