/*!
  \file cpu_device.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_DEVICE_HPP
#define ZINVUL_CPU_DEVICE_HPP

// Standard C++ library
#include <array>
#include <cstddef>
// Zisc
//#include "zisc/function_reference.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/thread_manager.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
//#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class DeviceInfo;
class CpuDeviceInfo;
class CpuSubPlatform;

/*!
  \brief No brief description

  No detailed description.
  */
class CpuDevice : public Device
{
 public:
//  using Command = zisc::FunctionReference<void ()>;


  //! Initialize the cpu device
  CpuDevice(CpuSubPlatform* sub_platform,
            const CpuDeviceInfo* device_info);

  //! Move a data
  CpuDevice(CpuDevice&& other) noexcept;

  //! Finalize the cpu instance
  ~CpuDevice() noexcept override;


  //! Move a data
  CpuDevice& operator=(CpuDevice&& other) noexcept;


//  //! Allocate a memory of a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  void allocate(const std::size_t size,
//                CpuBuffer<kDescriptor, Type>* buffer) noexcept;

  //! Return the underlying device info
  const CpuDeviceInfo& cpuDeviceInfo() const noexcept;

//  //! Deallocate a memory of a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  void deallocate(CpuBuffer<kDescriptor, Type>* buffer) noexcept;

  //! Return the underlying device info
  const DeviceInfo& deviceInfo() const noexcept override;

  //! Make a buffer
  template <typename Type>
  UniqueBuffer<Type> makeBuffer(const BufferUsage flag) noexcept;

//  //! Make a kernel
//  template <std::size_t kDimension, typename Function, typename ...BufferArgs>
//  UniqueKernel<kDimension, BufferArgs...> makeKernel(
//      Function func) noexcept;

  //! Return the number of threads
//  std::size_t numOfThreads() const noexcept;

  //! Submit a command
//  template <std::size_t kDimension>
//  void submit(const std::array<uint32b, kDimension>& works,
//              const Command& command) noexcept;

  //! Return the number of underlying command queues
  std::size_t numOfQueues() const noexcept override;

  //! Return the peak memory usage of the heap of the given index
  std::size_t peakMemoryUsage(const std::size_t index) const noexcept override;

  //! Return the current memory usage of the heap of the given index
  std::size_t totalMemoryUsage(const std::size_t index) const noexcept override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

//  //! Wait this thread until all commands in the device are completed
//  void waitForCompletion() const noexcept override;
//
//  //! Wait this thread until all commands in the queues are completed
//  void waitForCompletion(const QueueType queue_type) const noexcept override;
//
//  //! Wait this thread until all commands in the queue are completed
//  void waitForCompletion(const QueueType queue_type,
//                         const uint32b queue_index) const noexcept override;

 protected:
  //! Destroy the device
  void destroyData() noexcept override;

 private:
//  //! Expand the given work-group size array to 3d work-group size array
//  template <std::size_t kDimension>
//  std::array<uint32b, 3> expandTo3dWorkGroupSize(
//      const std::array<uint32b, kDimension>& works) const noexcept;

  //! Initialize the cpu device
  void initialize() noexcept;

  //! Return the sub-platform
  CpuSubPlatform& subPlatform() noexcept;

  //! Return the sub-platform
  const CpuSubPlatform& subPlatform() const noexcept;

//  //! Return the task batch size
//  uint32b taskBatchSize() const noexcept;


  CpuSubPlatform* sub_platform_ = nullptr;
  const CpuDeviceInfo* device_info_ = nullptr;
  zisc::ThreadManager thread_manager_;
//  uint32b task_batch_size_;
};

} // namespace zinvul

#include "cpu_device-inl.hpp"

#endif // ZINVUL_CPU_DEVICE_HPP
