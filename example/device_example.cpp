/*!
  \file device_example.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <iostream>
#include <string>
#include <vector>
// Zisc
#include "zisc/simple_memory_resource.hpp"
// Zinvul
#include "zinvul/device.hpp"
#include "zinvul/zinvul.hpp"
#include "zinvul/zinvul_config.hpp"

using zinvul::uint32b;

namespace {

std::string getSubPlatformTypeString(const zinvul::SubPlatformType type) noexcept
{
  std::string type_string;
  switch (type) {
   case zinvul::SubPlatformType::kCpu: {
    type_string = "CPU";
    break;
   }
   case zinvul::SubPlatformType::kVulkan: {
    type_string = "Vulkan";
    break;
   }
  }
  return type_string;
}

} // namespace 

int main(int /* argc */, char** /* argv */)
{
  std::cout << "Supported devices." << std::endl;
  std::cout << std::endl;

  zisc::SimpleMemoryResource mem_resource;
  auto param_list = zinvul::Device::getDeviceParamList(&mem_resource);
  for (const zinvul::Device::Param& param : param_list) {
    // Set device options
    zinvul::DeviceOptions options{&mem_resource};
    options.setSubPlatformType(param.type_);
    switch (options.subPlatformType()) {
     case zinvul::SubPlatformType::kCpu: {
      options.setCpuNumOfThreads(4);
      options.setCpuTaskBatchSize(16);
      break;
     }
     case zinvul::SubPlatformType::kVulkan: {
      options.setVulkanDeviceNumber(param.number_);
      break;
     }
    }

    // Create a device
    auto device = zinvul::makeDevice(options);

    // Print device info
    std::cout << "Device type: " << ::getSubPlatformTypeString(device->subPlatformType()) << std::endl;
    std::cout << "  - Device name: " << device->name() << std::endl;
    std::cout << "  - Vendor name: " << device->vendorName() << std::endl;
    std::cout << "  - Subgroup size: " << device->subgroupSize() << std::endl;
    std::cout << std::endl;
  }

  return 0;
}
