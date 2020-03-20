/*!
  \file cpu_device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_DEVICE_INL_HPP
#define ZINVUL_CPU_DEVICE_INL_HPP

#include "cpu_device.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/memory.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_device_info.hpp"
#include "cpu_sub_platform.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/////*!
////  */
////template <DescriptorType kDescriptor, typename Type> inline
////void CpuDevice::allocate(const std::size_t size,
////                         CpuBuffer<kDescriptor, Type>* buffer) noexcept
////{
////  auto& b = buffer->buffer();
////  b.resize(size);
////
////  const std::size_t memory_usage = deviceMemoryUsage() + buffer->memoryUsage();
/*!
  \details No detailed description

  \return No description
  */
inline
const CpuDeviceInfo& CpuDevice::deviceInfoData() const noexcept
{
  const auto& info = deviceInfo();
  return *zisc::cast<const CpuDeviceInfo*>(std::addressof(info));
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void CpuDevice::notifyAllocation(const std::size_t size) noexcept
{
  heap_usage_.add(size);
}

/*!
  \details No detailed description

  \param [in] size No description.
  */
inline
void CpuDevice::notifyDeallocation(const std::size_t size) noexcept
{
  heap_usage_.release(size);
}

////  setDeviceMemoryUsage(memory_usage);
////  setHostMemoryUsage(memory_usage);
////}
////
/////*!
////  */
////template <DescriptorType kDescriptor, typename Type> inline
////void CpuDevice::deallocate(CpuBuffer<kDescriptor, Type>* buffer) noexcept
////{
////  const std::size_t memory_usage = deviceMemoryUsage() - buffer->memoryUsage();
////  setDeviceMemoryUsage(memory_usage);
////  setHostMemoryUsage(memory_usage);
////}

/////*!
////  */
////template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
////UniqueKernel<kDimension, BufferArgs...> CpuDevice::makeKernel(
////    Function func) noexcept
////{
////  using DeviceKernel = CpuKernel<kDimension, Function, BufferArgs...>;
////  using UniqueCpuKernel = zisc::UniqueMemoryPointer<DeviceKernel>;
////  auto kernel = UniqueCpuKernel::make(memoryResource(), this, func);
////  return std::move(kernel);
////}

///*!
//  \details No detailed description
//
//  \return No description
//  */
//inline
//std::size_t CpuDevice::numOfThreads() const noexcept
//{
//  return thread_manager_.numOfThreads();
//}
//
///*!
//  \details No detailed description

///*!
//  \details No detailed description
//
//  \tparam kDimension No description.
//  \param [in] works No description.
//  \param [in] command No description.
//  */
////template <std::size_t kDimension> inline
////void CpuDevice::submit(const std::array<uint32b, kDimension>& works,
////                       const Command& command) noexcept
////{
////  std::atomic<uint32b> id{0};
////  auto task = [this, &command, &works, &id](const uint, const uint)
////  {
////    const auto group_size = expandTo3dWorkGroupSize(works);
////    const uint32b num_of_works = group_size[0] * group_size[1] * group_size[2];
////    const uint32b n = ((num_of_works % taskBucketSize()) == 0)
////        ? num_of_works / taskBucketSize()
////        : num_of_works / taskBucketSize() + 1;
////    cl::clinner::WorkGroup::setWorkGroupSize(group_size);
////    for (uint32b bucket_id = id++; bucket_id < n; bucket_id = id++) {
////      for (uint32b i = 0; i < taskBucketSize(); ++i) {
////        const uint32b group_id = bucket_id * taskBucketSize() + i;
////        if (group_id < num_of_works) {
////          cl::clinner::WorkGroup::setWorkGroupId(group_id);
////          command();
////        }
////      }
////    }
////  };
////
////  constexpr uint start = 0;
////  const uint end = thread_manager_.numOfThreads();
////  auto result = thread_manager_.enqueueLoop(task, start, end, workResource());
////  result->wait();
////}

///*!
//  \details No detailed description
//  */
//inline
//void CpuDevice::waitForCompletion() const noexcept
//{
//}
//
///*!
//  \details No detailed description
//
//  \param [in] queue_type No description.
//  */
//inline
//void CpuDevice::waitForCompletion(const QueueType /* queue_type */) const noexcept
//{
//}
//
///*!
//  \details No detailed description
//
//  \param [in] queue_type No description.
//  \param [in] queue_index No description.
//  */
//inline
//void CpuDevice::waitForCompletion(const QueueType /* queue_type */,
//                                  const uint32b /* queue_index */) const noexcept
//{
//}
//
///*!
//  \details No detailed description
//
//  \tparam kDimension No description.
//  \param [in] works No description.
//  \return No description
//  */
//template <std::size_t kDimension> inline
//std::array<uint32b, 3> CpuDevice::expandTo3dWorkGroupSize(
//    const std::array<uint32b, kDimension>& works) const noexcept
//{
//  std::array<uint32b, 3> work_group_size{{1, 1, 1}};
//  for (std::size_t i = 0; i < kDimension; ++i)
//    work_group_size[i] = works[i];
//  return work_group_size;
//}

/*!
  \details No detailed description

  \return No description
  */
inline
CpuSubPlatform& CpuDevice::parentImpl() noexcept
{
  auto p = getParent();
  return *zisc::cast<CpuSubPlatform*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
inline
const CpuSubPlatform& CpuDevice::parentImpl() const noexcept
{
  const auto p = getParent();
  return *zisc::cast<const CpuSubPlatform*>(p);
}

///*!
//  \details No detailed description
//
//  \return No description
//  */
//inline
//uint32b CpuDevice::taskBatchSize() const noexcept
//{
//  return task_batch_size_;
//}

} // namespace zinvul

#endif // ZINVUL_CPU_DEVICE_INL_HPP
