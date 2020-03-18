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
#include "buffer.hpp"
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"
#include "utility/zinvul_object.hpp"

namespace zinvul {

// Forward declaration
class DeviceInfo;
class SubPlatform;

/*!
  \brief No brief description

  No detailed description.
  */
class Device : public ZinvulObject
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<Device>;
  using WeakPtr = std::weak_ptr<Device>;


  //! Initialize the device
  Device(IdData&& id) noexcept;

  //! Finalize the device
  virtual ~Device() noexcept;


  //! Destroy the data
  void destroy() noexcept;

  //! Return the underlying device info
  const DeviceInfo& deviceInfo() const noexcept;

  //! Initialize the device
  void initialize(ZinvulObject::SharedPtr&& parent,
                  WeakPtr&& own,
                  const DeviceInfo& device_info);

  //! Make a buffer
  template <typename T>
  SharedBuffer<T> makeBuffer(const BufferUsage flag);

  //! Return the number of underlying command queues
  virtual std::size_t numOfQueues() const noexcept = 0;

  //! Return the peak memory usage of the heap of the given number
  virtual std::size_t peakMemoryUsage(const std::size_t number) const noexcept = 0;

  //! Return the current memory usage of the heap of the given number
  virtual std::size_t totalMemoryUsage(const std::size_t number) const noexcept = 0;

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

  //! Initialize the device
  virtual void initData() = 0;

 private:
  const DeviceInfo* device_info_ = nullptr;
};

// Type aliases
using SharedDevice = Device::SharedPtr;
using WeakDevice = Device::WeakPtr;

} // namespace zinvul

#include "device-inl.hpp"

#endif // ZINVUL_DEVICE_HPP
