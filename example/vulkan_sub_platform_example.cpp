/*!
  \file vulkan_sub_platform_example.cpp
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
#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
// Vulkan
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
#include <vulkan/vulkan.hpp>
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
// Zisc
#include "zisc/simple_memory_resource.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/platform.hpp"
#include "zinvul/platform_options.hpp"
#include "zinvul/zinvul_config.hpp"
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
#include "zinvul/vulkan/vulkan_device_info.hpp"
#include "zinvul/vulkan/vulkan_sub_platform.hpp"
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM

namespace {

double toMegaBytes(const std::size_t bytes) noexcept
{
  const double mb = zisc::cast<double>(bytes) / (1024.0 * 1024.0);
  return mb;
}

std::string toVersionString(const zinvul::uint32b version)
{
  return std::to_string(VK_VERSION_MAJOR(version)) + "." +
      std::to_string(VK_VERSION_MINOR(version)) + "." +
      std::to_string(VK_VERSION_PATCH(version));
}

} // namespace

int main(int /* argc */, char** /* argv */)
{
  zisc::SimpleMemoryResource mem_resource;

  // Platform options
  zinvul::PlatformOptions platform_options{&mem_resource};
  platform_options.setPlatformName("VulkanSubPlatformExample");
  platform_options.setPlatformVersionMajor(zinvul::Config::versionMajor());
  platform_options.setPlatformVersionMinor(zinvul::Config::versionMinor());
  platform_options.setPlatformVersionPatch(zinvul::Config::versionPatch());
  platform_options.enableVulkanSubPlatform(true);
  platform_options.enableDebugMode(true);

  // Platform
  std::cout << "Create a platform." << std::endl;
  auto platform = zinvul::makePlatform(&mem_resource);
  platform->initialize(platform_options);

  const std::string indent1 = "    ";
  // Vulkan sub-platform
  const bool has_subplatform =
      platform->hasSubPlatform(zinvul::SubPlatformType::kVulkan);
  if (has_subplatform) {
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
    const std::string indent2 = indent1 + indent1;
    const std::string indent3 = indent2 + indent1;
    const std::string indent4 = indent3 + indent1;
    const std::string indent5 = indent4 + indent1;
    // Show Vulkan instance extensions
    {
      std::cout << indent1 << "Vulkan extensions" << std::endl;
      auto properties = vk::enumerateInstanceExtensionProperties();
      using Type = decltype(properties)::value_type;
      auto cmp = [](const Type& lhs, const Type& rhs) noexcept
      {
        return std::string_view{lhs.extensionName} <
               std::string_view{rhs.extensionName};
      };
      std::sort(properties.begin(), properties.end(), cmp);
      for (const auto& ext : properties) {
        std::cout << indent2 << ext.extensionName << ": "
                  << ::toVersionString(ext.specVersion) << std::endl;
      }
    }
    // Show Vulkan instance layeres
    std::cout << std::endl;
    {
      std::cout << indent1 << "Vulkan layeres" << std::endl;
      auto properties = vk::enumerateInstanceLayerProperties();
      for (const auto& layer : properties) {
        std::cout << indent2 << layer.layerName << ": "
                  << ::toVersionString(layer.specVersion)
                  << " (" << ::toVersionString(layer.implementationVersion)
                  << "): " << layer.description 
                  << std::endl;
      }
    }
    // Show Vulkan device info
    std::cout << std::endl;
    platform->updateDeviceInfoList();
    auto sub_platform = zisc::cast<zinvul::VulkanSubPlatform*>(
        platform->subPlatform(zinvul::SubPlatformType::kVulkan));
    const auto& info_list = sub_platform->deviceInfoList();
    for (std::size_t i = 0; i < info_list.size(); ++i) {
      const zinvul::VulkanDeviceInfo& info = info_list[i];
      std::cout << std::endl;
      std::cout << indent1 << "## VulkanDevice[" << i << "]: "
                << info.properties().properties1_.deviceName
                << std::endl;
      // Extension properties
      {
        std::cout << indent3 << "Extensions" << std::endl;
        std::vector<VkExtensionProperties> props;
        {
          const auto& props2 = info.extensionPropertiesList();
          props.resize(props2.size());
          std::copy(props2.begin(), props2.end(), props.begin());
        }
        using Type = decltype(props)::value_type;
        auto cmp = [](const Type& lhs, const Type& rhs) noexcept
        {
          return std::string_view{lhs.extensionName} <
                 std::string_view{rhs.extensionName};
        };
        std::sort(props.begin(), props.end(), cmp);
        for (const auto& ext : props) {
          std::cout << indent4 << ext.extensionName << ": "
                    << ::toVersionString(ext.specVersion) << std::endl;
        }
      }
      // Layer
      std::cout << std::endl;
      {
        std::cout << indent3 << "Layeres" << std::endl;
        const auto& props = info.layerPropertiesList();
        for (const auto& layer : props) {
          std::cout << indent4 << layer.layerName << ": "
                    << ::toVersionString(layer.specVersion)
                    << " (" << ::toVersionString(layer.implementationVersion)
                    << "): " << layer.description 
                    << std::endl;
        }
      }
      // Queue family
      std::cout << std::endl;
      {
        std::cout << indent3 << "Queue families" << std::endl;
        const auto& props = info.queueFamilyPropertiesList();
        for (std::size_t index = 0; index < props.size(); ++index) {
          const auto& prop = props[index];
          std::cout << indent4 << "QueueFamily[" << index << "]: "
                    << "counts: " << prop.properties1_.queueCount
                    << ", " << std::endl
                    << indent5 << "capabilities: "
                    << vk::to_string(zisc::cast<vk::QueueFlags>(prop.properties1_.queueFlags))
                    << std::endl;
        }
      }
      // Memory
      std::cout << std::endl;
      {
        std::cout << indent3 << "MemoryHeap" << std::endl;
        const auto& props = info.memoryProperties();
        const auto& mem = props.properties1_;
        for (std::size_t index = 0; index < mem.memoryHeapCount; ++index) {
          const auto& heap = mem.memoryHeaps[index];
          std::cout << indent4 << "Heap[" << index << "] attribute: "
                    << vk::to_string(zisc::cast<vk::MemoryHeapFlags>(heap.flags))
                    << std::endl;
          std::cout << indent5 << "total  : "
                    << ::toMegaBytes(heap.size) << " MB"
                    << std::endl
                    << indent5 << "budget : "
                    << ::toMegaBytes(props.budget_.heapBudget[index]) << " MB"
                    << std::endl
                    << indent5 << "used   : "
                    << ::toMegaBytes(props.budget_.heapUsage[index]) << " MB"
                    << std::endl;
        }
      }
      std::cout << std::endl;
      {
        std::cout << indent3 << "Memory" << std::endl;
        const auto& props = info.memoryProperties();
        const auto& mem = props.properties1_;
        for (std::size_t index = 0; index < mem.memoryTypeCount; ++index) {
          const auto& memory = mem.memoryTypes[index];
          std::cout << indent4 << "Memory[" << index << "] -> Heap["
                    << memory.heapIndex << "], properties: "
                    << vk::to_string(zisc::cast<vk::MemoryPropertyFlags>(memory.propertyFlags))
                    << std::endl;
        }
      }
      std::cout << std::endl;
      {
        std::cout << indent3 << "ExternalMemory" << std::endl;
        const auto& props = info.properties();
        std::cout << indent4 << "Min imported pointer alignment: "
                  << props.external_memory_host_.minImportedHostPointerAlignment
                  << " bytes" << std::endl;
      }
    }
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
  }

  std::cout << std::endl;
  std::cout << indent1 << "Memory usage: "
            << ::toMegaBytes(mem_resource.totalMemoryUsage()) << " MB."
            << std::endl;
  std::cout << indent1 << "Peak memory usage: "
            << ::toMegaBytes(mem_resource.peakMemoryUsage()) << " MB."
            << std::endl;

  return 0;
}
