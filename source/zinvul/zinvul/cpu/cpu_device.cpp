/*!
  \file cpu_device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_device.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/memory.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/thread_manager.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_device_info.hpp"
#include "cpu_sub_platform.hpp"
#include "zinvul/device.hpp"
#include "zinvul/device_info.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] id No description.
  */
CpuDevice::CpuDevice(IdData&& id) : Device(std::move(id))
{
}

/*!
  \details No detailed description
  */
CpuDevice::~CpuDevice() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t CpuDevice::numOfQueues() const noexcept
{
  return 1;
}

/*!
  \details No detailed description

  \param [in] number No description.
  \return No description
  */
std::size_t CpuDevice::peakMemoryUsage(const std::size_t /* number */) const noexcept
{
  return heap_usage_.peak();
}

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

/*!
  \details No detailed description

  \param [in] work_size No description.
  \param [in] command No description.
  */
void CpuDevice::submit(const std::array<uint32b, 3>& work_size,
                       const Command& command) noexcept
{
}

/*!
  \details No detailed description

  \param [in] number No description.
  \return No description
  */
std::size_t CpuDevice::totalMemoryUsage(const std::size_t /* number */) const noexcept
{
  return heap_usage_.total();
}

/*!
  \details No detailed description
  */
void CpuDevice::destroyData() noexcept
{
  thread_manager_.reset();
}

/*!
  \details No detailed description
  */
void CpuDevice::initData()
{
  auto& device = parentImpl();

  heap_usage_.setPeak(0);
  heap_usage_.setTotal(0);
  auto mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<zisc::ThreadManager> alloc{mem_resource};
  thread_manager_ = zisc::pmr::allocateUnique(alloc,
                                              device.numOfThreads(),
                                              mem_resource);
}

} // namespace zinvul
