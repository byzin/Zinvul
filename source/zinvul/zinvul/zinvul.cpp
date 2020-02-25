/*!
  \file zinvul.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "zinvul.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "device.hpp"
#include "device_options.hpp"
#include "cpu/cpu_device.hpp"
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
#include "vulkan/vulkan_device.hpp"
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
namespace zinvul {

/*!
  \details No detailed description

  \param [in] options No description.
  \return No description
  */
UniqueDevice makeDevice(DeviceOptions& options)
{
  UniqueDevice device{};
//  zisc::pmr::memory_resource* mem_resource = options.memoryResource();
//  switch (options.subPlatformType()) {
//   case SubPlatformType::kCpu: {
//    zisc::pmr::polymorphic_allocator<CpuDevice> alloc{mem_resource};
//    device = zisc::pmr::allocateUnique<CpuDevice>(alloc, options);
//    break;
//   }
//#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
//   case SubPlatformType::kVulkan: {
//    zisc::pmr::polymorphic_allocator<VulkanDevice> alloc{mem_resource};
//    device = zisc::pmr::allocateUnique<VulkanDevice>(alloc, options);
//    break;
//   }
//#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
//  }
  return device;
}

} // namespace zinvul
