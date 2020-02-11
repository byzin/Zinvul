/*!
  \file platform_example.cpp
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
#include <memory>
#include <string>
// Zisc
#include "zisc/simple_memory_resource.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/platform.hpp"
#include "zinvul/platform_options.hpp"
#include "zinvul/zinvul_config.hpp"

namespace {

std::string getSubPlatformTypeString(const zinvul::SubPlatformType type)
{
  std::string type_string;
  switch (type) {
   case zinvul::SubPlatformType::kCpu:
    type_string = "CPU";
    break;
   case zinvul::SubPlatformType::kVulkan:
    type_string = "Vulkan";
    break;
  }
  return type_string;
}

double toMegaBytes(const std::size_t bytes) noexcept
{
  const double mb = zisc::cast<double>(bytes) / (1024.0 * 1024.0);
  return mb;
}

} // namespace

int main(int /* argc */, char** /* argv */)
{
  zisc::SimpleMemoryResource mem_resource;

  // Platform options
  zinvul::PlatformOptions platform_options{&mem_resource};
  platform_options.setPlatformName("PlatformExample");
  platform_options.setPlatformVersionMajor(zinvul::Config::versionMajor());
  platform_options.setPlatformVersionMinor(zinvul::Config::versionMinor());
  platform_options.setPlatformVersionPatch(zinvul::Config::versionPatch());
  platform_options.enableVulkanSubPlatform(true);
  platform_options.enableDebugMode(true);

  // Platform
  std::cout << "Create a platform." << std::endl;
  auto platform = zinvul::makePlatform(&mem_resource);
  platform->initialize(platform_options);

  // CPU sub-platform
  bool has_subplatform = platform->hasSubPlatform(zinvul::SubPlatformType::kCpu);
  std::cout << "    CPU sub-platform: " << has_subplatform << std::endl;

  // Vulkan sub-platform
  has_subplatform = platform->hasSubPlatform(zinvul::SubPlatformType::kVulkan);
  std::cout << "    Vulkan sub-platform: " << has_subplatform << std::endl;

  // Show device info
  std::cout << std::endl;
  platform->updateDeviceInfoList();
  const auto& device_info_list = platform->deviceInfoList();
  for (std::size_t i = 0; i < device_info_list.size(); ++i) {
    const auto info = device_info_list[i];
    std::cout << "    ## Device[" << i << "]" << std::endl;
    std::cout << "        Type                : "
              << ::getSubPlatformTypeString(info->type()) << std::endl;
    std::cout << "        Name                : "
              << info->name() << std::endl;
    std::cout << "        Vendor name         : "
              << info->vendorName() << std::endl;
    const auto workgroup_counts = info->maxWorkGroupCount();
    std::cout << "        Max work group count: " << "("
              << workgroup_counts[0] << ", "
              << workgroup_counts[1] << ", "
              << workgroup_counts[2] << ")" << std::endl;
    std::cout << "        Work group size     : "
              << info->workGroupSize() << std::endl;
    std::cout << "        Max allocation size : "
              << ::toMegaBytes(info->maxAllocationSize()) << " MB." << std::endl;
    for (std::size_t index = 0; index < info->numOfHeaps(); ++index) {
      std::cout << "        MemoryHeap[" << index << "]" << std::endl;
      std::cout << "            Total memory    : "
                << ::toMegaBytes(info->totalMemory(index)) << " MB." << std::endl;
      std::cout << "            Available memory: "
                << ::toMegaBytes(info->availableMemory(index)) << " MB." << std::endl;
    }
  }

  std::cout << std::endl;
  std::cout << "    Host memory usage     : "
            << ::toMegaBytes(mem_resource.totalMemoryUsage()) << " MB."
            << std::endl;
  std::cout << "    Host peak memory usage: "
            << ::toMegaBytes(mem_resource.peakMemoryUsage()) << " MB."
            << std::endl;

  return 0;
}
