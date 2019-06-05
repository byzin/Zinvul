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
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
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
#include "zinvul/zinvul_config.hpp"
#include "zinvul/cppcl/atomic.hpp"
#include "zinvul/cppcl/utility.hpp"
// cpu_features
#include "cpu_features_macros.h"
#if defined(CPU_FEATURES_ARCH_X86)
#include "cpuinfo_x86.h"
#elif defined(CPU_FEATURES_ARCH_ARM)
#include "cpuinfo_arm.h"
#elif defined(CPU_FEATURES_ARCH_AARCH64)
#include "cpuinfo_aarch64.h"
#elif defined(CPU_FEATURES_ARCH_MIPS)
#include "cpuinfo_mips.h"
#elif defined(CPU_FEATURES_ARCH_PPC)
#include "cpuinfo_ppc.h"
#endif

namespace zinvul {

/*!
  */
inline
CpuDevice::CpuDevice(DeviceOptions& options) noexcept :
    Device(options),
    thread_manager_{options.cpu_num_of_threads_, memoryResource()},
    task_bucket_size_{zisc::max(options.cpu_task_bucket_size_, 1u)}
{
  initialize(options);
}

/*!
  */
template <BufferType kBufferType, typename Type> inline
void CpuDevice::allocate(const std::size_t size,
                         CpuBuffer<kBufferType, Type>* buffer) noexcept
{
  auto& b = buffer->buffer();
  b.resize(size);

  const std::size_t device_memory_usage = memoryUsage() + buffer->memoryUsage();
  setMemoryUsage(device_memory_usage);
}

/*!
  */
template <BufferType kBufferType, typename Type> inline
void CpuDevice::deallocate(CpuBuffer<kBufferType, Type>* buffer) noexcept
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
template <BufferType kBufferType, typename Type> inline
UniqueBuffer<kBufferType, Type> CpuDevice::makeBuffer(
    const BufferUsage usage_flag) noexcept
{
  using DeviceBuffer = CpuBuffer<kBufferType, Type>;
  using UniqueCpuBuffer = zisc::UniqueMemoryPointer<DeviceBuffer>;
  auto buffer = UniqueCpuBuffer::make(memoryResource(), this, usage_flag);
  return std::move(buffer);
}

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
UniqueKernel<kDimension, BufferArgs...> CpuDevice::makeKernel(
    Function func) noexcept
{
  using DeviceKernel = CpuKernel<kDimension, Function, BufferArgs...>;
  using UniqueCpuKernel = zisc::UniqueMemoryPointer<DeviceKernel>;
  auto kernel = UniqueCpuKernel::make(memoryResource(), this, func);
  return std::move(kernel);
}

/*!
  */
inline
std::string_view CpuDevice::name() const noexcept
{
  std::string_view device_name{name_};
  return device_name;
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
std::string_view CpuDevice::vendorName() const noexcept
{
  std::string_view vendor_name{vendor_name_};
  return vendor_name;
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
void CpuDevice::initialize(DeviceOptions& /* options */) noexcept
{
  using namespace cpu_features;
  // Initialize device info
#if defined(CPU_FEATURES_ARCH_X86)
  {
    char brand_string[49];
    FillX86BrandString(brand_string);
    name_ = brand_string;
  }
  {
    const X86Info info = GetX86Info();
    vendor_name_ = info.vendor;
  }
#elif defined(CPU_FEATURES_ARCH_ARM)
  static_assert(false, "Not implemented yet.");
  {
    const ArmInfo info = GetArmInfo();
  }
  vendor_name_ = "ARM";
#elif defined(CPU_FEATURES_ARCH_AARCH64)
  static_assert(false, "Not implemented yet.");
  {
    const Aarch64Info info = GetAarch64Info();
  }
#elif defined(CPU_FEATURES_ARCH_MIPS)
  static_assert(false, "Not implemented yet.");
  {
    const MipsInfo info = GetMipsInfo();
  }
#elif defined(CPU_FEATURES_ARCH_PPC)
  static_assert(false, "Not implemented yet.");
  {
    const PPCInfo info = GetPPCInfo();
    const PPCPlatformStrings strings = GetPPCPlatformStrings();
  }
#endif
  if (name_.empty())
    name_ = "N/A";
  if (vendor_name_.empty())
    vendor_name_ = "N/A";
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
