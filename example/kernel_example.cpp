/*!
  \file kernel_example.cpp
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
#include "zinvul/zinvul.hpp"

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
  platform_options.setPlatformName("KernelExample");
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
  const std::string indent2 = indent1 + indent1;
  // CPU sub-platform
  bool has_subplatform = platform->hasSubPlatform(zinvul::SubPlatformType::kCpu);
  std::cout << indent1 << "CPU sub-platform: " << has_subplatform << std::endl;

  // Vulkan sub-platform
  has_subplatform = platform->hasSubPlatform(zinvul::SubPlatformType::kVulkan);
  std::cout << indent1 << "Vulkan sub-platform: " << has_subplatform << std::endl;

  // Show device info
  std::cout << std::endl;
  platform->updateDeviceInfoList();
  const auto& device_info_list = platform->deviceInfoList();
  for (std::size_t i = 0; i < device_info_list.size(); ++i) {
    std::cout << std::endl;
    const auto info = device_info_list[i];
    std::cout << indent1 << "## Device[" << i << "]" << std::endl;
    std::cout << indent2 << "Type                : "
              << ::getSubPlatformTypeString(info->type()) << std::endl;
    std::cout << indent2 << "Name                : "
              << info->name() << std::endl;
    std::cout << indent2 << "Vendor name         : "
              << info->vendorName() << std::endl;
    std::cout << indent2 << "Num of heaps        : "
              << info->numOfHeaps() << std::endl;

    auto device = platform->makeDevice(i);
    std::cout << indent2 << "Num of queues       : "
              << device->numOfQueues() << std::endl;

    std::cout << std::endl;
    const std::string indent3 = indent2 + indent1;
    auto print_buffer_info = [&indent3](zinvul::Buffer<int>* buffer)
    {
      using Type = zinvul::Buffer<int>::Type;
      const std::size_t s = sizeof(Type) * buffer->size();
      std::cout << indent3 << "n = " << buffer->size()
                << " (" << ::toMegaBytes(s) << " MB)." << std::endl;
      std::cout << indent3
                << "DeviceLocal: " << buffer->isDeviceLocal() << ", "
                << "HostVisible: " << buffer->isHostVisible() << ", "
                << "HostCoherent: " << buffer->isHostCoherent() << ", "
                << "HostCached: " << buffer->isHostCached() << std::endl;
    };
    constexpr std::size_t size = 128 * 1024 * 1024;
    constexpr std::size_t n = size / sizeof(int);

    auto buffer1 = device->makeBuffer<int>(zinvul::BufferUsage::kDeviceOnly);
    buffer1->setSize(n);
    std::cout << indent2 << "Create a device only buffer: " << std::endl;
    print_buffer_info(buffer1.get());

    auto buffer2 = device->makeBuffer<int>(zinvul::BufferUsage::kHostOnly);
    buffer2->setSize(n);
    std::cout << indent2 << "Create a host only buffer: " << std::endl;
    print_buffer_info(buffer2.get());

    auto buffer3 = device->makeBuffer<int>(zinvul::BufferUsage::kDeviceToHost);
    buffer3->setSize(n);
    std::cout << indent2 << "Create a device to host buffer: " << std::endl;
    print_buffer_info(buffer3.get());

    auto buffer4 = device->makeBuffer<int>(zinvul::BufferUsage::kHostToDevice);
    buffer4->setSize(n);
    std::cout << indent2 << "Create a host to device buffer: " << std::endl;
    print_buffer_info(buffer4.get());

    std::cout << std::endl;
    for (std::size_t num = 0; num < info->numOfHeaps(); ++num) {
      std::cout << indent2 << "[Heap" << num << "]      Memory usage: "
                << ::toMegaBytes(device->totalMemoryUsage(num))
                << " MB." << std::endl;
      std::cout << indent2 << "[Heap" << num << "] Peak Memory usage: "
                << ::toMegaBytes(device->peakMemoryUsage(num))
                << " MB." << std::endl;
    }
  }

  std::cout << std::endl;
  std::cout << indent1 << "Host memory usage     : "
            << ::toMegaBytes(mem_resource.totalMemoryUsage()) << " MB."
            << std::endl;
  std::cout << indent1 << "Host peak memory usage: "
            << ::toMegaBytes(mem_resource.peakMemoryUsage()) << " MB."
            << std::endl;

  return 0;
}
