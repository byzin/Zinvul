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
