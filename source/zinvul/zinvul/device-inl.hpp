/*!
  \file device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_INL_HPP
#define ZINVUL_DEVICE_INL_HPP

#include "device.hpp"
// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "device_options.hpp"
#include "zinvul_config.hpp"

namespace zinvul {

/*!
  */
inline
Device::Device(DeviceOptions& options) noexcept :
    mem_resource_{options.memoryResource()}
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
std::pmr::memory_resource* Device::memoryResource() noexcept
{
  ZISC_ASSERT(mem_resource_ != nullptr, "The memory resource is null.");
  return mem_resource_;
}

/*!
  */
inline
const std::pmr::memory_resource* Device::memoryResource() const noexcept
{
  ZISC_ASSERT(mem_resource_ != nullptr, "The memory resource is null.");
  return mem_resource_;
}

/*!
  */
inline
std::size_t Device::peakMemoryUsage() const noexcept
{
  return peak_memory_usage_;
}

/*!
  */
inline
std::size_t Device::totalMemoryUsage() const noexcept
{
  return total_memory_usage_;
}

} // namespace zinvul

#endif // ZINVUL_DEVICE_INL_HPP
