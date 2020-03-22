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
#include <memory>
// Zisc
#include "zisc/function_reference.hpp"
#include "zisc/memory.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/thread_manager.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
//#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"

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
  // Type aliases
  using Command = zisc::FunctionReference<void ()>;


  //! Initialize the cpu device
  CpuDevice(IdData&& id);

  //! Finalize the cpu instance
  ~CpuDevice() noexcept override;


  //! Return the underlying device info
  const CpuDeviceInfo& deviceInfoData() const noexcept;

  //! Make a buffer
  template <typename Type>
  SharedBuffer<Type> makeBuffer(const BufferUsage flag) noexcept;

//  //! Make a kernel
//  template <std::size_t kDimension, typename Function, typename ...BufferArgs>
//  UniqueKernel<kDimension, BufferArgs...> makeKernel(
//      Function func) noexcept;

  //! Notify of device memory allocation
  void notifyAllocation(const std::size_t size) noexcept;

  //! Notify of device memory deallocation
  void notifyDeallocation(const std::size_t size) noexcept;

  //! Return the number of underlying command queues
  std::size_t numOfQueues() const noexcept override;

  //! Return the number of threads which are used for kernel execution
  std::size_t numOfThreads() const noexcept;

  //! Return the peak memory usage of the heap of the given index
  std::size_t peakMemoryUsage(const std::size_t number) const noexcept override;

  //! Submit a kernel command
  void submit(const std::array<uint32b, 3>& work_size,
              const Command& command) noexcept;

  //! Return the task batch size per thread
  std::size_t taskBatchSize() const noexcept;

  //! Return the underlying thread manager which is used for kernel exection
  zisc::ThreadManager& threadManager() noexcept;

  //! Return the underlying thread manager which is used for kernel exection
  const zisc::ThreadManager& threadManager() const noexcept;

  //! Return the current memory usage of the heap of the given index
  std::size_t totalMemoryUsage(const std::size_t number) const noexcept override;

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

  //! Initialize the device
  void initData() override;

 private:
  //! Return the sub-platform
  CpuSubPlatform& parentImpl() noexcept;

  //! Return the sub-platform
  const CpuSubPlatform& parentImpl() const noexcept;


  zisc::Memory::Usage heap_usage_;
  zisc::pmr::unique_ptr<zisc::ThreadManager> thread_manager_;
};

} // namespace zinvul

#include "cpu_device-inl.hpp"

#endif // ZINVUL_CPU_DEVICE_HPP
