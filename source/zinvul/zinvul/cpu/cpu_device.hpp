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
#include <string>
#include <string_view>
// Zisc
#include "zisc/function_reference.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/thread_manager.hpp"
// Zinvul
//#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
//#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
template <DescriptorType, typename> class CpuBuffer;

/*!
  \brief No brief description

  No detailed description.
  */
class CpuDevice : public Device
{
 public:
  using Command = zisc::FunctionReference<void ()>;


  //! Creat a CPU device
  CpuDevice(DeviceOptions& options) noexcept;


//  //! Allocate a memory of a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  void allocate(const std::size_t size,
//                CpuBuffer<kDescriptor, Type>* buffer) noexcept;
//
//  //! Deallocate a memory of a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  void deallocate(CpuBuffer<kDescriptor, Type>* buffer) noexcept;

  //! Return cpu type
  SubPlatformType subPlatformType() const noexcept override;

//  //! Make a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  UniqueBuffer<kDescriptor, Type> makeBuffer(
//      const BufferUsage usage_flag) noexcept;

//  //! Make a kernel
//  template <std::size_t kDimension, typename Function, typename ...BufferArgs>
//  UniqueKernel<kDimension, BufferArgs...> makeKernel(
//      Function func) noexcept;

  //! Return the device name
  std::string_view name() const noexcept override;

  //! Return the number of threads
  std::size_t numOfThreads() const noexcept;

  //! Return the subgroup size
  uint32b subgroupSize() const noexcept override;

  //! Submit a command
  template <std::size_t kDimension>
  void submit(const std::array<uint32b, kDimension>& works,
              const Command& command) noexcept;

  //! Return the vendor name
  std::string_view vendorName() const noexcept override;

  //! Wait this thread until all commands in the device are completed
  void waitForCompletion() const noexcept override;

  //! Wait this thread until all commands in the queues are completed
  void waitForCompletion(const QueueType queue_type) const noexcept override;

  //! Wait this thread until all commands in the queue are completed
  void waitForCompletion(const QueueType queue_type,
                         const uint32b queue_index) const noexcept override;

 private:
  //! Expand the given work-group size array to 3d work-group size array
  template <std::size_t kDimension>
  std::array<uint32b, 3> expandTo3dWorkGroupSize(
      const std::array<uint32b, kDimension>& works) const noexcept;

  //! Initialize CPU info
  void initCpuInfo() noexcept;

  //! Initialize a CPU device
  void initialize(DeviceOptions& options) noexcept;

  //! Return the task batch size
  uint32b taskBatchSize() const noexcept;


  zisc::ThreadManager thread_manager_;
  zisc::pmr::string name_;
  zisc::pmr::string vendor_name_;
  uint32b task_batch_size_;
};

} // namespace zinvul

#include "cpu_device-inl.hpp"

#endif // ZINVUL_CPU_DEVICE_HPP
