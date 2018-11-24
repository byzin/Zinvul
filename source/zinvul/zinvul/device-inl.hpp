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
#include <array>
#include <cstddef>
#include <numeric>
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
template <std::size_t kDimension> inline
std::array<uint32b, 3> Device::calcWorkGroupSize(
    const std::array<uint32b, kDimension>& works) const noexcept
{
  std::array<uint32b, 3> work_group_size{{1, 1, 1}};
  const auto& local_work_size = workgroupSize<kDimension>();
  for (std::size_t i = 0; i < kDimension; ++i) {
    work_group_size[i] = ((works[i] % local_work_size[i]) == 0)
        ? works[i] / local_work_size[i]
        : works[i] / local_work_size[i] + 1;
  }
  return work_group_size;
}

/*!
  */
inline
void Device::initWorkgroupSize(const uint32b subgroup_size) noexcept
{
  for (uint32b dim = 1; dim <= workgroup_size_list_.size(); ++dim) {
    std::array<uint32b, 3> workgroup_size{{1, 1, 1}};
    const auto product = [](const std::array<uint32b, 3>& s)
    {
      return std::accumulate(s.begin(), s.end(), 1u, std::multiplies<uint32b>());
    };
    for (uint32b i = 0; product(workgroup_size) < subgroup_size; i = (i + 1) % dim)
      workgroup_size[i] *= 2;
    ZISC_ASSERT(product(workgroup_size) == subgroup_size,
                "The subgroup size should be power of 2: ", subgroup_size);
    workgroup_size_list_[dim - 1] = workgroup_size;
  }
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
uint32b Device::subgroupSize() const noexcept
{
  return workgroup_size_list_[0][0];
}

/*!
  */
template <std::size_t kDimension> inline
const std::array<uint32b, 3>& Device::workgroupSize()
    const noexcept
{
  static_assert((0 < kDimension) && (kDimension <= 3),
                "The dimension is out of range.");
  return workgroup_size_list_[kDimension - 1];
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
