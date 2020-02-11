/*!
  \file device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "device.hpp"
// Standard C++ library
#include <vector>
#include <iostream>
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
// Vulkan
#include <vulkan/vulkan.hpp>
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
#include "vulkan/vulkan_device.hpp"
#include "vulkan/vulkan_device_info.hpp"
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::vector<Device::Param> Device::getDeviceParamList(
    zisc::pmr::memory_resource* mem_resource)
{
  using DeviceParamList = zisc::pmr::vector<Param>;
  DeviceParamList param_list{DeviceParamList::allocator_type{mem_resource}};
  param_list.reserve(5); // Assume that a machine has a CPU and 4 GPUs at most

  // CPU
  param_list.emplace_back(Param{SubPlatformType::kCpu, 0});

#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
  // Vulkan
  try {
    auto device_info_list = VulkanDevice::getDeviceInfoList(mem_resource);
  }
  catch (const vk::SystemError& error) {
    std::cerr << error.what();
    //! \todo Handle errors
  }
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM

  return param_list;
}

} // namespace zinvul
