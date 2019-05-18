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
#include "zisc/math.hpp"
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
    thread_manager_{options.cpu_num_of_threads_, memoryResource()},
    task_bucket_size_{zisc::max(options.task_bucket_size_, 1u)}
{
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
inline
uint32b CpuDevice::subgroupSize() const noexcept
{
  return 1;
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
    const auto group_size = expandTo3dWorkGroupSize(works);
    const uint32b num_of_works = group_size[0] * group_size[1] * group_size[2];
    const uint32b n = ((num_of_works % taskBucketSize()) == 0)
        ? num_of_works / taskBucketSize()
        : num_of_works / taskBucketSize() + 1;
    cl::clinner::Atomic::setMutex(&mutex_);
    cl::clinner::WorkGroup::setWorkGroupSize(group_size);
    for (uint32b bucket_id = id++; bucket_id < n; bucket_id = id++) {
      for (uint32b i = 0; i < taskBucketSize(); ++i) {
        const uint32b group_id = bucket_id * taskBucketSize() + i;
        if (group_id < num_of_works) {
          cl::clinner::WorkGroup::setWorkGroupId(group_id);
          command();
        }
      }
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

/*!
  */
template <std::size_t kDimension> inline
std::array<uint32b, 3> CpuDevice::expandTo3dWorkGroupSize(
    const std::array<uint32b, kDimension>& works) const noexcept
{
  std::array<uint32b, 3> work_group_size{{1, 1, 1}};
  for (std::size_t i = 0; i < kDimension; ++i)
    work_group_size[i] = works[i];
  return work_group_size;
}

/*!
  */
inline
uint32b CpuDevice::taskBucketSize() const noexcept
{
  return task_bucket_size_;
}

} // namespace zinvul

#endif // ZINVUL_CPU_DEVICE_INL_HPP
