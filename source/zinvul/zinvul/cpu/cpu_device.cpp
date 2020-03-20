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
  heap_usage_.setPeak(0);
  heap_usage_.setTotal(0);
  auto mem_resource = memoryResource();
  zisc::pmr::polymorphic_allocator<zisc::ThreadManager> alloc{mem_resource};
  thread_manager_ = zisc::pmr::allocateUnique(alloc, mem_resource);
}

} // namespace zinvul
