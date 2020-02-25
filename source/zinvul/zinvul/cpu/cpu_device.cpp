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
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_device_info.hpp"
#include "cpu_sub_platform.hpp"
#include "zinvul/device.hpp"
#include "zinvul/device_info.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] sub_platform No description.
  \param [in] device_info No description.
  */
CpuDevice::CpuDevice(CpuSubPlatform* sub_platform,
                     const CpuDeviceInfo* device_info) :
    Device(sub_platform->memoryResource()),
    sub_platform_{sub_platform},
    device_info_{device_info},
    thread_manager_{sub_platform->memoryResource()}
{
  initialize();
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
const DeviceInfo& CpuDevice::deviceInfo() const noexcept
{
  const auto& device_info = cpuDeviceInfo();
  return device_info;
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

  \param [in] index No description.
  \return No description
  */
std::size_t CpuDevice::peakMemoryUsage(const std::size_t index) const noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
std::size_t CpuDevice::totalMemoryUsage(const std::size_t index) const noexcept
{
  return 0;
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType CpuDevice::type() const noexcept
{
  return SubPlatformType::kCpu;
}

/*!
  \details No detailed description
  */
void CpuDevice::destroyData() noexcept
{
  device_info_ = nullptr;
  sub_platform_ = nullptr;
}

/*!
  \details No detailed description
  */
void CpuDevice::initialize() noexcept
{
}

} // namespace zinvul
