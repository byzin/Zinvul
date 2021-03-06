/*!
  \file test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "test.hpp"
// Standard C++ library
#include <cmath>
#include <cstdlib>
#include <limits>
#include <string>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/math.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/cpu/cpu_device.hpp"
#ifdef ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
#include "zinvul/vulkan/vulkan_device.hpp"
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM

std::vector<zinvul::UniqueDevice> makeTestDeviceList(zinvul::DeviceOptions& options)
{
  std::vector<zinvul::UniqueDevice> device_list;
  // CPU
  options.device_type_ = zinvul::SubPlatformType::kCpu;
  device_list.emplace_back(zinvul::makeDevice(options));
  // Vulkan
#ifdef ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
  options.device_type_ = zinvul::SubPlatformType::kVulkan;
  const auto vulkan_device_list = zinvul::VulkanDevice::getPhysicalDeviceInfoList();
  for (std::size_t i = 0; i < vulkan_device_list.size(); ++i) {
    options.vulkan_device_number_ = static_cast<zinvul::uint32b>(i);
    device_list.emplace_back(zinvul::makeDevice(options));
  }
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM

  return device_list;
}

zinvul::DeviceOptions makeTestOptions()
{
  zinvul::DeviceOptions options;
  options.app_name_ = "ZinvulTest";
  options.app_version_major_ = kZinvulVersionMajor;
  options.app_version_minor_ = kZinvulVersionMinor;
  options.app_version_patch_ = kZinvulVersionPatch;
  return options;
}

std::string getTestDeviceInfo(const zinvul::Device& device)
{
  using namespace std::string_literals;
  std::string info;
  switch (device.SubPlatformType()) {
   case zinvul::SubPlatformType::kCpu: {
    auto d = static_cast<const zinvul::CpuDevice*>(&device);
    info = "## Device: CPU\n"s;
    info += "  Vendor: "s + device.vendorName().data() + "\n"s;
    info += "  Name: "s + device.name().data() + "\n"s;
    info += "  Threads: "s + std::to_string(d->numOfThreads()) + "\n"s;
    info += "  Subgroup: "s + std::to_string(device.subgroupSize()) + "\n"s;
    break;
   }
#ifdef ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
   case zinvul::SubPlatformType::kVulkan: {
//    auto d = static_cast<const zinvul::VulkanDevice*>(&device);
    info = "## Device: Vulkan\n"s;
    info += "  Vendor: "s + device.vendorName().data() + "\n"s;
    info += "  Name: "s + device.name().data() + "\n"s;
    info += "  Subgroup: "s + std::to_string(device.subgroupSize()) + "\n"s;
    break;
   }
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
   default: {
    info = "## Device: N/A\n"s;
    break;
   }
  }
  return info;
}

std::string getTestDeviceUsedMemory(const zinvul::Device& device)
{
  using namespace std::string_literals;
  auto to_mb_string = [](const std::size_t usage)
  {
    const double memory = static_cast<double>(usage) / (1024.0 * 1024.0);
    std::string info;
    info.resize(13);
    std::sprintf(info.data(), "%6.3lf MB", memory);
    return info;
  };
  const auto info = "  Peak device memory usage: "s +
                    to_mb_string(device.peakDeviceMemoryUsage()) + "\n" +
                    "  Peak   host memory usage: "s +
                    to_mb_string(device.peakHostMemoryUsage()) + "\n";
  return info;
}

float calcUlps(const float lhs, const float rhs)
{
  constexpr float e = std::numeric_limits<float>::epsilon();
  const float ulp = std::abs(lhs - rhs) / (std::abs(lhs + rhs) * e);
  return ulp;
}
