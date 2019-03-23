/*!
  \file zinvul_test.cpp
  \author Sho Ikeda
  */

// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Vulkan
#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
#include <vulkan/vulkan.hpp>
#define VMA_IMPLEMENTATION
#endif // ZINVUL_ENABLE_VULKAN_BACKEND
// Zisc
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/experiment.hpp"
#include "test.hpp"

#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
TEST(ZinvulTest, VulkanDeviceTest)
{
  const auto device_info_list = zinvul::VulkanDevice::getPhysicalDeviceInfoList();
  ASSERT_GT(device_info_list.size(), 0) << "There is no vulkan device.";

  const auto to_version_string = [](const zinvul::uint32b version)
  {
    return std::to_string(VK_VERSION_MAJOR(version)) + "." +
        std::to_string(VK_VERSION_MINOR(version)) + "." +
        std::to_string(VK_VERSION_PATCH(version));
  };

  const auto to_gb_size = [](const std::size_t bytes)
  {
    const double mb = zisc::cast<double>(bytes) / zisc::power<3>(1024.0);
    return mb;
  };

  for (std::size_t number = 0; number < device_info_list.size(); ++number) {
    const auto& info = device_info_list[number];
    const auto& properties = info.properties_;
    std::cout << "Device[" << number << "]" << std::endl;
    std::cout << "  Name: " << properties.deviceName << std::endl;
    std::cout << "  Vendor ID: " << properties.vendorID << std::endl;
    std::cout << "  Vendor Name: " << zinvul::VulkanDevice::getVendorName(properties.vendorID) << std::endl;
    std::cout << "  Type: " << vk::to_string(properties.deviceType) << std::endl;
    std::cout << "  Driver version: " << to_version_string(properties.driverVersion) << std::endl;
    std::cout << "  API version: " << to_version_string(properties.apiVersion) << std::endl;
    std::cout << "  SubgroupSize: " << info.subgroup_properties_.subgroupSize << std::endl;
    const auto& queue_family_list = info.queue_family_list_;
    for (zinvul::uint32b i = 0; i < queue_family_list.size(); ++i) {
      std::cout << "  QueueFamily[" << i << "]" << std::endl;
      std::cout << "    Type: " << vk::to_string(queue_family_list[i].queueFlags) << std::endl;
      std::cout << "    Count: " << queue_family_list[i].queueCount << std::endl;
    }
    const auto& memory_properties = info.memory_properties_;
    for (zinvul::uint32b i = 0; i < memory_properties.memoryTypeCount; ++i) {
      std::cout << "  MemoryType[" << i << "]" << std::endl;
      std::cout << "    MemoryType: " << vk::to_string(memory_properties.memoryTypes[i].propertyFlags) << std::endl;
      std::cout << "    HeapIndex: " << memory_properties.memoryTypes[i].heapIndex << std::endl;
    }
    for (zinvul::uint32b i = 0; i < memory_properties.memoryHeapCount; ++i) {
      std::cout << "  HeapType[" << i << "]" << std::endl;
      std::cout << "    HeapType: " << vk::to_string(memory_properties.memoryHeaps[i].flags) << std::endl;
      std::cout << "    HeapSize: " << to_gb_size(memory_properties.memoryHeaps[i].size) << " GB" << std::endl;
    }
    {
      std::cout << "  Extensions" << std::endl;
      auto extensions = info.extension_properties_;
      auto cmp = [](const vk::ExtensionProperties& lhs,
                    const vk::ExtensionProperties& rhs)
      {
        return std::strcmp(lhs.extensionName, rhs.extensionName) < 0;
      };
      std::sort(extensions.begin(), extensions.end(), cmp);
      for (const auto& ext : extensions) {
        std::cout << "    " << ext.extensionName << ": " << to_version_string(ext.specVersion) << std::endl;
      }
    }
  }
}
#endif // ZINVUL_ENABLE_VULKAN_BACKEND

TEST(Experiment, ZinvulTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    results1->setSize(1);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
    results2->setSize(1);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
    results3->setSize(1);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
    results4->setSize(1);

    auto kernel = zinvul::makeZinvulKernel(device.get(), experiment, experiment, 1);
    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The '' func is failed.";
    // Scalar
    {
      std::vector<float> results;
      results.resize(1);
      results1->read(results.data(), results.size(), 0, 0);
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(1);
      results2->read(results.data(), results.size(), 0, 0);
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(1);
      results3->read(results.data(), results.size(), 0, 0);
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(1);
      results4->read(results.data(), results.size(), 0, 0);
    }

    std::cout << std::endl;
  }
}

//TEST(Experiment, ZinvulTest)
//{
//  using namespace zinvul;
//
//  constexpr uint resolution = 10000;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
//    result_buff->setSize(1);
//    {
//      const uint32b init = 0;
//      result_buff->write(&init, 1, 0, 0);
//    }
//    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
//    table_buff->setSize(resolution);
//    {
//      std::vector<int32b> init;
//      init.resize(resolution);
//      std::fill(init.begin(), init.end(), 0);
//      table_buff->write(init.data(), init.size(), 0, 0);
//    }
//    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    res_buff->setSize(1);
//    res_buff->write(&resolution, res_buff->size(), 0, 0);
//
//    auto kernel = makeZinvulKernel(device.get(), experiment, experiment, 1);
//    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
//    device->waitForCompletion();
//
//    // atomic_cmpxchg
//    {
//      std::vector<int32b> table;
//      table.resize(resolution);
//      table_buff->read(table.data(), table.size(), 0, 0);
//      for (std::size_t i = 0; i < table.size(); ++i)
//        EXPECT_TRUE(table[i]) << "The 'atomic_cmpxchg[" << i << "]' is wrong.";
//
//      uint32b result;
//      result_buff->read(&result, 1, 0, 0);
//      const uint32b expected = resolution;
//      EXPECT_EQ(expected, result) << "The 'atomic_cmpxchg' is wrong.";
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
