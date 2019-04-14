/*!
  \file cpu_device-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_DEVICE_INL_HPP
#define ZINVUL_CPU_DEVICE_INL_HPP

#include "cpu_device.hpp"
// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <numeric>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/function_reference.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_buffer.hpp"
#include "cpu_kernel.hpp"
#include "zinvul/kernel_group.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/cppcl/atomic.hpp"
#include "zinvul/cppcl/utility.hpp"

namespace zinvul {

/*!
  */
inline
CpuDevice::CpuDevice(DeviceOptions& options) noexcept :
    Device(options),
    thread_manager_{options.cpu_num_of_threads_, memoryResource()}
{
  initWorkgroupSize(1);
  ZISC_ASSERT(0 < subgroupSize(), "The subgroup size is zero.");
}

/*!
  */
template <typename Type> inline
void CpuDevice::allocate(const std::size_t size, CpuBuffer<Type>* buffer) noexcept
{
  auto& b = buffer->buffer();
  b.resize(size);

  const std::size_t device_memory_usage = memoryUsage() + buffer->memoryUsage();
  setMemoryUsage(device_memory_usage);
}

/*!
  */
template <typename Type> inline
void CpuDevice::deallocate(CpuBuffer<Type>* buffer) noexcept
{
  const std::size_t device_memory_usage = memoryUsage() - buffer->memoryUsage();
  setMemoryUsage(device_memory_usage);
}

/*!
  */
inline
DeviceType CpuDevice::deviceType() const noexcept
{
  return DeviceType::kCpu;
}

/*!
  */
template <typename Type> inline
UniqueBuffer<Type> CpuDevice::makeBuffer(const BufferUsage usage_flag) noexcept
{
  using UniqueCpuBuffer = zisc::UniqueMemoryPointer<CpuBuffer<Type>>;
  UniqueBuffer<Type> buffer =
      UniqueCpuBuffer::make(memoryResource(), this, usage_flag);
  return buffer;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
UniqueKernel<GroupType, kDimension, ArgumentTypes...> CpuDevice::makeKernel(
    const typename Kernel<GroupType, kDimension, ArgumentTypes...>::Function func)
        noexcept
{
  using UniqueCpuKernel = zisc::UniqueMemoryPointer<CpuKernel<GroupType,
                                                              kDimension,
                                                              ArgumentTypes...>>;
  UniqueKernel<GroupType, kDimension, ArgumentTypes...> kernel =
      UniqueCpuKernel::make(memoryResource(), this, func);
  return kernel;
}

/*!
  */
inline
std::size_t CpuDevice::numOfThreads() const noexcept
{
  return thread_manager_.numOfThreads();
}

/*!
  */
template <std::size_t kDimension> inline
void CpuDevice::submit(const std::array<uint32b, kDimension>& works,
                       const Command& command) noexcept
{
  std::atomic<uint32b> id{0};
  auto task = [this, &command, &works, &id](const uint, const uint)
  {
    const uint32b n = std::accumulate(works.begin(), works.end(),
                                      1u, std::multiplies<uint32b>());
    cl::__setMutex(&mutex_);
    cl::__setWorkGroupSize(calcWorkGroupSize(works));
    for (uint32b group_id = id++; group_id < n; group_id = id++) {
      cl::__setWorkGroupId(group_id);
      command();
    }
  };

  constexpr uint start = 0;
  const uint end = thread_manager_.numOfThreads();
  auto result = thread_manager_.enqueueLoop(task, start, end, workResource());
  result->wait();
}

/*!
  */
inline
void CpuDevice::waitForCompletion() const noexcept
{
}

} // namespace zinvul

#endif // ZINVUL_CPU_DEVICE_INL_HPP
