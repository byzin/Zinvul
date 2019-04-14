/*!
  \file cpu_device.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_DEVICE_HPP
#define ZINVUL_CPU_DEVICE_HPP

// Standard C++ library
#include <array>
#include <cstddef>
// Zisc
#include "zisc/function_reference.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/spin_lock_mutex.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
template <typename> class CpuBuffer;

/*!
  */
class CpuDevice : public Device
{
 public:
  using Command = zisc::FunctionReference<void ()>;


  //! Initialize a cpu device
  CpuDevice(DeviceOptions& options) noexcept;


  //! Allocate a memory of a buffer
  template <typename Type>
  void allocate(const std::size_t size, CpuBuffer<Type>* buffer) noexcept;

  //! Deallocate a memory of a buffer
  template <typename Type>
  void deallocate(CpuBuffer<Type>* buffer) noexcept;

  //! Return cpu type
  DeviceType deviceType() const noexcept override;

  //! Make a buffer
  template <typename Type>
  UniqueBuffer<Type> makeBuffer(const BufferUsage usage_flag) noexcept;

  //! Make a kernel
  template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
  UniqueKernel<GroupType, kDimension, ArgumentTypes...> makeKernel(
      const typename Kernel<GroupType, kDimension, ArgumentTypes...>::Function func)
          noexcept;

  //! Return the number of threads
  std::size_t numOfThreads() const noexcept;

  //! Submit a command
  template <std::size_t kDimension>
  void submit(const std::array<uint32b, kDimension>& works,
              const Command& command) noexcept;

  //! Wait this thread until all commands in the queue are completed
  void waitForCompletion() const noexcept override;

 private:
  zisc::ThreadManager thread_manager_;
  zisc::SpinLockMutex mutex_;
};

} // namespace zinvul

#include "cpu_device-inl.hpp"

#endif // ZINVUL_CPU_DEVICE_HPP
