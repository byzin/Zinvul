///*!
//  \file cpu_device-inl.hpp
//  \author Sho Ikeda
//  \brief No brief description
//
//  \details
//  No detailed description.
//
//  \copyright
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZINVUL_CPU_DEVICE_INL_HPP
//#define ZINVUL_CPU_DEVICE_INL_HPP
//
//#include "cpu_device.hpp"
//// Standard C++ library
//#include <array>
//#include <atomic>
//#include <cstddef>
//#include <numeric>
//#include <string>
//#include <string_view>
//#include <type_traits>
//#include <utility>
//// Zisc
//#include "zisc/error.hpp"
//#include "zisc/function_reference.hpp"
//#include "zisc/math.hpp"
//#include "zisc/std_memory_resource.hpp"
//#include "zisc/thread_manager.hpp"
//#include "zisc/utility.hpp"
//// Zinvul
////#include "cpu_buffer.hpp"
////#include "cpu_kernel.hpp"
//#include "zinvul/device_options.hpp"
//#include "zinvul/zinvul_config.hpp"
//#include "zinvul/cppcl/atomic.hpp"
//#include "zinvul/cppcl/utility.hpp"
//
//namespace zinvul {
//
///*!
//  \details No detailed description
//
//  \param [in] options No description.
//  */
//inline
//CpuDevice::CpuDevice(DeviceOptions& options) noexcept :
//    Device(options),
//    thread_manager_{options.cpuNumOfThreads(), memoryResource()},
//    name_{zisc::pmr::string::allocator_type{memoryResource()}},
//    vendor_name_{zisc::pmr::string::allocator_type{memoryResource()}},
//    task_batch_size_{zisc::max(options.cpuTaskBatchSize(), 1u)}
//{
//  initialize(options);
//}
//
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
//
///*!
//  \details No detailed description
//
//  \return No description
//  */
//inline
//SubPlatformType CpuDevice::subPlatformType() const noexcept
//{
//  return SubPlatformType::kCpu;
//}
//
/////*!
////  */
////template <DescriptorType kDescriptor, typename Type> inline
////UniqueBuffer<kDescriptor, Type> CpuDevice::makeBuffer(
////    const BufferUsage usage_flag) noexcept
////{
////  using DeviceBuffer = CpuBuffer<kDescriptor, Type>;
////  using UniqueCpuBuffer = zisc::UniqueMemoryPointer<DeviceBuffer>;
////  auto buffer = UniqueCpuBuffer::make(memoryResource(), this, usage_flag);
////  return std::move(buffer);
////}
////
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
//
///*!
//  \details No detailed description
//
//  \return No description
//  */
//inline
//std::string_view CpuDevice::name() const noexcept
//{
//  std::string_view device_name{name_};
//  return device_name;
//}
//
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
//
//  \return No description
//  */
//inline
//uint32b CpuDevice::subgroupSize() const noexcept
//{
//  return 1;
//}
//
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
//
///*!
//  \details No detailed description
//
//  \return No description
//  */
//inline
//std::string_view CpuDevice::vendorName() const noexcept
//{
//  std::string_view vendor_name{vendor_name_};
//  return vendor_name;
//}
//
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
//
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
//
//} // namespace zinvul
//
//#endif // ZINVUL_CPU_DEVICE_INL_HPP
