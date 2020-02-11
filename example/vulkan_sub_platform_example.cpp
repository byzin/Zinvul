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
#include <string>
#include <memory>
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
#include "zinvul/vulkan/vulkan_sub_platform.hpp"
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM

namespace {

double toMegaBytes(const std::size_t bytes) noexcept
{
  const double mb = zisc::cast<double>(bytes) / (1000.0 * 1000.0);
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

  // Vulkan sub-platform
  const bool has_subplatform =
      platform->hasSubPlatform(zinvul::SubPlatformType::kVulkan);
  if (has_subplatform) {
#if defined(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
    {
      std::cout << "    Vulkan extensions" << std::endl;
      auto properties = vk::enumerateInstanceExtensionProperties();
      for (const auto& ext : properties) {
        std::cout << "        " << ext.extensionName << ": "
                  << ::toVersionString(ext.specVersion) << std::endl;
      }
    }
    std::cout << std::endl;
    {
      std::cout << "    Vulkan layeres" << std::endl;
      auto properties = vk::enumerateInstanceLayerProperties();
      for (const auto& layer : properties) {
        std::cout << "        " << layer.layerName << ": "
                  << ::toVersionString(layer.specVersion) << std::endl;
      }
    }
    auto sub_platform = zisc::cast<zinvul::VulkanSubPlatform*>(
        platform->subPlatform(zinvul::SubPlatformType::kVulkan));
#endif // ZINVUL_ENABLE_VULKAN_SUB_PLATFORM
  }

  std::cout << std::endl;
  std::cout << "    Memory usage: "
            << ::toMegaBytes(mem_resource.totalMemoryUsage()) << " MB."
            << std::endl;
  std::cout << "    Peak memory usage: "
            << ::toMegaBytes(mem_resource.peakMemoryUsage()) << " MB."
            << std::endl;

  return 0;
}
