/*!
  \file device-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_INL_HPP
#define ZINVUL_DEVICE_INL_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
inline
Device::Device(DeviceOptions& options) noexcept :
    mem_resource_{options.mem_resource_},
    work_resource_{options.work_resource_}
{
}

/*!
  */
inline
Device::~Device() noexcept
{
}

/*!
  */
inline
zisc::pmr::memory_resource* Device::memoryResource() noexcept
{
  ZISC_ASSERT(mem_resource_ != nullptr, "The memory resource is null.");
  return mem_resource_;
}

/*!
  */
inline
const zisc::pmr::memory_resource* Device::memoryResource() const noexcept
{
  ZISC_ASSERT(mem_resource_ != nullptr, "The memory resource is null.");
  return mem_resource_;
}

/*!
  */
inline
std::size_t Device::maxMemoryUsage() const noexcept
{
  return max_memory_usage_;
}

/*!
  */
inline
std::size_t Device::memoryUsage() const noexcept
{
  return memory_usage_;
}

/*!
  */
inline
void Device::setMemoryUsage(const std::size_t memory_usage) noexcept
{
  memory_usage_ = memory_usage;
  if (maxMemoryUsage() < memory_usage)
    max_memory_usage_ = memory_usage;
}

/*!
  */
inline
zisc::pmr::memory_resource* Device::workResource() noexcept
{
  ZISC_ASSERT(work_resource_ != nullptr, "The work resource is null.");
  return work_resource_;
}

/*!
  */
inline
const zisc::pmr::memory_resource* Device::workResource() const noexcept
{
  ZISC_ASSERT(work_resource_ != nullptr, "The work resource is null.");
  return work_resource_;
}

} // namespace zinvul

#endif // ZINVUL_DEVICE_INL_HPP
