/*!
  \file zinvul_test.cpp
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
//#include <algorithm>
//#include <cstddef>
//#include <cstring>
//#include <iostream>
#include <memory>
//#include <string>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
//#include "zisc/math.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/simple_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
#include "zinvul/vulkan/vulkan_sub_platform.hpp"
#include "zinvul/vulkan/utility/vulkan.hpp"
#include "zinvul/vulkan/utility/vulkan_dispatch_loader.hpp"
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
//#include "zinvul/kernel_set/experiment.hpp"
//#include "test.hpp"

#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)

TEST(VulkanSubPlatformTest, GetInstanceProcAddrOptionTest)
{
  zisc::SimpleMemoryResource mem_resource;

  // Dispatcher
  auto dispatch_loader = std::make_unique<zinvul::VulkanDispatchLoader>(std::addressof(mem_resource));
  ASSERT_TRUE(dispatch_loader->isAvailable()) << "Loader isn't available.";
  auto loader = dispatch_loader->loaderImpl();

  {

    zinvul::PlatformOptions platform_options{&mem_resource};
    platform_options.setPlatformName("GetInstanceProcAddrOptionTest");
    platform_options.setPlatformVersionMajor(zinvul::Config::versionMajor());
    platform_options.setPlatformVersionMinor(zinvul::Config::versionMinor());
    platform_options.setPlatformVersionPatch(zinvul::Config::versionPatch());
    platform_options.enableVulkanSubPlatform(true);
    platform_options.enableDebugMode(true);
    platform_options.setVulkanGetProcAddrPtr(std::addressof(loader->vkGetInstanceProcAddr));

    auto platform = zinvul::makePlatform(std::addressof(mem_resource));
    platform->initialize(platform_options);

    ASSERT_TRUE(platform->hasSubPlatform(zinvul::SubPlatformType::kVulkan)) <<
        "Vulkan initilaization failed.";

    // Get an index of a vulkan device
    std::size_t index = 0;
    const auto& device_info_list = platform->deviceInfoList();
    for (index = 0; index < device_info_list.size(); ++index) {
      const auto& info = device_info_list[index];
      if (info->type() == zinvul::SubPlatformType::kVulkan)
        break;
    }

    auto device = platform->makeDevice(index);
  }
  dispatch_loader.reset();
}

TEST(VulkanSubPlatformTest, InstanceOptionTest)
{
  zisc::SimpleMemoryResource mem_resource;

  // Dispatcher
  auto dispatch_loader = std::make_unique<zinvul::VulkanDispatchLoader>(std::addressof(mem_resource));
  ASSERT_TRUE(dispatch_loader->isAvailable()) << "Loader isn't available.";
  auto loader = dispatch_loader->loaderImpl();

  // Instance
  using zinvul::uint32b;
  std::vector<const char*> extensions;
  extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  std::vector<const char*> layers;
  layers.push_back("VK_LAYER_KHRONOS_validation");
  zinvulvk::ApplicationInfo app_info;
  app_info.apiVersion = zinvul::VulkanSubPlatform::apiVersion();
  zinvulvk::InstanceCreateInfo create_info{zinvulvk::InstanceCreateFlags{},
                                           std::addressof(app_info),
                                           zisc::cast<uint32b>(layers.size()),
                                           layers.data(),
                                           zisc::cast<uint32b>(extensions.size()),
                                           extensions.data()};
  zinvulvk::Instance instance = zinvulvk::createInstance(create_info, nullptr, *loader);
  dispatch_loader->set(zisc::cast<VkInstance>(instance));

  {
    zinvul::PlatformOptions platform_options{&mem_resource};
    platform_options.setPlatformName("GetInstanceProcAddrOptionTest");
    platform_options.setPlatformVersionMajor(zinvul::Config::versionMajor());
    platform_options.setPlatformVersionMinor(zinvul::Config::versionMinor());
    platform_options.setPlatformVersionPatch(zinvul::Config::versionPatch());
    platform_options.enableVulkanSubPlatform(true);
    platform_options.enableDebugMode(true);
    platform_options.setVulkanInstancePtr(std::addressof(instance));

    auto platform = zinvul::makePlatform(std::addressof(mem_resource));
    platform->initialize(platform_options);

    ASSERT_TRUE(platform->hasSubPlatform(zinvul::SubPlatformType::kVulkan)) <<
        "Vulkan initilaization failed.";

    // Get an index of a vulkan device
    std::size_t index = 0;
    const auto& device_info_list = platform->deviceInfoList();
    for (index = 0; index < device_info_list.size(); ++index) {
      const auto& info = device_info_list[index];
      if (info->type() == zinvul::SubPlatformType::kVulkan)
        break;
    }

    auto device = platform->makeDevice(index);
  }

  instance.destroy(nullptr, *loader);
  dispatch_loader.reset();
}

#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM

//TEST(Experiment, ZinvulTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1u;
//
//    auto buffer0 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
//    buffer0->setSize(resolution);
//    {
//      std::vector<cl::float4> inputs;
//      inputs.resize(resolution);
//      buffer0->write(inputs.data(), resolution, 0, 0);
//    }
//
//    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
//    res_buff->setSize(1);
//    res_buff->write(&resolution, 1, 0, 0);
//
//
//    auto kernel = zinvul::makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(experiment, experiment));
//    kernel->run(*buffer0, *res_buff, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    std::cout << std::endl;
//  }
//}
