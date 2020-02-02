/*!
  \file vulkan_device_info_example.cpp
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
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"
#include "zinvul/vulkan/vulkan_device.hpp"

namespace {

std::string toVersionString(const zinvul::uint32b version) noexcept
{
  const auto version_str = std::to_string(VK_VERSION_MAJOR(version)) + "." +
                           std::to_string(VK_VERSION_MINOR(version)) + "." +
                           std::to_string(VK_VERSION_PATCH(version));
  return version_str;
}

void printVulkanInstanceInfo()
{
}

} // namespace

int main(int /* argc */, char** /* argv */)
{
  ::printVulkanInstanceInfo();

//  zisc::SimpleMemoryResource mem_resource;

  return 0;
}
