/*!
  \file test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "test.hpp"
// Standard C++ library
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/zinvul_config.hpp"

/*!
  */
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

std::vector<zinvul::UniqueDevice> makeTestDeviceList(
    zinvul::DeviceOptions& options) noexcept
{
  std::vector<zinvul::UniqueDevice> device_list;
  // CPU
  options.device_type_ = zinvul::DeviceType::kCpu;
  device_list.emplace_back(zinvul::makeDevice(options));
  // Vulkan
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
  options.device_type_ = zinvul::DeviceType::kVulkan;
  const auto vulkan_device_list = zinvul::VulkanDevice::getPhysicalDeviceInfoList();
  for (std::size_t i = 0; i < vulkan_device_list.size(); ++i) {
    options.device_number_ = static_cast<zinvul::uint32b>(i);
    device_list.emplace_back(zinvul::makeDevice(options));
  }
#endif // ZINVUL_ENABLE_VULKAN_BACKEND

  return device_list;
}

zinvul::DeviceOptions makeTestOptions() noexcept
{
  zinvul::DeviceOptions options;
  options.app_name_ = "ZinvulTest";
  options.app_version_major_ = kZinvulVersionMajor;
  options.app_version_minor_ = kZinvulVersionMinor;
  options.app_version_patch_ = kZinvulVersionPatch;
  return options;
}
