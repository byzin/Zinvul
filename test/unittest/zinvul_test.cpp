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
#endif // ZINVUL_ENABLE_VULKAN_BACKEND
// Zisc
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/experiment.hpp"
#include "test.hpp"

#ifdef ZINVUL_ENABLE_VULKAN_BACKEND
TEST(ZinvulTest, VulkanInstanceTest)
{
  const auto to_version_string = [](const zinvul::uint32b version)
  {
    return std::to_string(VK_VERSION_MAJOR(version)) + "." +
        std::to_string(VK_VERSION_MINOR(version)) + "." +
        std::to_string(VK_VERSION_PATCH(version));
  };

  {
    std::cout << "  Extensions" << std::endl;
    auto [result, extensions] = vk::enumerateInstanceExtensionProperties();
    auto cmp = [](const vk::ExtensionProperties& lhs,
                  const vk::ExtensionProperties& rhs)
    {
      return std::strcmp(lhs.extensionName, rhs.extensionName) < 0;
    };
    std::sort(extensions.begin(), extensions.end(), cmp);
    for (const auto& ext : extensions) {
      std::cout << "    " << ext.extensionName << ": "
                << to_version_string(ext.specVersion) << std::endl;
    }
  }
  {
    std::cout << "  Layers" << std::endl;
    auto [result, layers] = vk::enumerateInstanceLayerProperties();
    auto cmp = [](const vk::LayerProperties& lhs,
                  const vk::LayerProperties& rhs)
    {
      return std::strcmp(lhs.layerName, rhs.layerName) < 0;
    };
    std::sort(layers.begin(), layers.end(), cmp);
    for (const auto& layer : layers) {
      std::cout << "    " << layer.layerName << ": "
                << to_version_string(layer.specVersion) << std::endl;
    }
  }
}

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
    {
      const auto& p = info.properties();
      const auto& properties1 = p.properties1_;
      std::cout << "Device[" << number << "]" << std::endl;
      std::cout << "  Name: " << properties1.deviceName << std::endl;
      std::cout << "  Vendor ID: " << properties1.vendorID << std::endl;
      std::cout << "  Vendor Name: "
                << zinvul::VulkanDevice::getVendorName(properties1.vendorID)
                << std::endl;
      std::cout << "  Type: " << vk::to_string(properties1.deviceType) << std::endl;
      std::cout << "  Driver version: "
                << to_version_string(properties1.driverVersion) << std::endl;
      std::cout << "    ID: " << vk::to_string(p.driver_.driverID) << std::endl;
      std::cout << "    Name: " << p.driver_.driverName << std::endl;
      std::cout << "    Info: " << p.driver_.driverInfo << std::endl;
      std::cout << "  API version: "
                << to_version_string(properties1.apiVersion) << std::endl;
      std::cout << "  Max memory allocation size: "
                << to_gb_size(p.maintenance3_.maxMemoryAllocationSize)
                << " GB" << std::endl;
      std::cout << "  SubgroupSize: "
                << p.subgroup_.subgroupSize << std::endl;
    }
    {
      const auto& p = info.features();
      const auto features1 = p.features1_;
      std::cout << "  Features" << std::endl;
      std::cout << "    Shader 64bit integer: "
                << features1.shaderInt64 << std::endl;
      std::cout << "    Shader 16bit integer: "
                << features1.shaderInt16 << std::endl;
      std::cout << "    Shader 8bit integer: "
                << p.float16_int8_.shaderInt8 << std::endl;
      std::cout << "    Shader 64bit float: "
                << features1.shaderFloat64 << std::endl;
      std::cout << "    Shader 16bit float: "
                << p.float16_int8_.shaderFloat16 << std::endl;
      std::cout << "    16bit storage buffer access: "
                << p.b16bit_storage_.storageBuffer16BitAccess << std::endl;
      std::cout << "    16bit uniform buffer access: "
                << p.b16bit_storage_.uniformAndStorageBuffer16BitAccess << std::endl;
      std::cout << "    16bit storage push constant: "
                << p.b8bit_storage_.storagePushConstant8 << std::endl;
      std::cout << "    8bit storage buffer access: "
                << p.b8bit_storage_.storageBuffer8BitAccess << std::endl;
      std::cout << "    8bit uniform buffer access: "
                << p.b8bit_storage_.uniformAndStorageBuffer8BitAccess << std::endl;
      std::cout << "    8bit storage push constant: "
                << p.b8bit_storage_.storagePushConstant8 << std::endl;
      std::cout << "    Protected memory: "
                << p.protected_memory_.protectedMemory << std::endl;
      std::cout << "    Atomic 64bit integer on buffers: "
                << p.shader_atomic_int64_.shaderBufferInt64Atomics << std::endl;
      std::cout << "    Atomic 64bit integer on shared memory: "
                << p.shader_atomic_int64_.shaderSharedInt64Atomics << std::endl;
      std::cout << "    Uniform buffer standard layout: "
                << p.uniform_buffer_standard_layout_.uniformBufferStandardLayout << std::endl;
      std::cout << "    Variable pointers storage buffer: "
                << p.variable_pointers_.variablePointersStorageBuffer << std::endl;
      std::cout << "    Variable pointers: "
                << p.variable_pointers_.variablePointers << std::endl;
    }
    {
      const auto& p = info.queueFamilyPropertiesList();
      for (zinvul::uint32b i = 0; i < p.size(); ++i) {
        std::cout << "  QueueFamily[" << i << "]" << std::endl;
        std::cout << "    Type: "
                  << vk::to_string(p[i].properties1_.queueFlags) << std::endl;
        std::cout << "    Count: "
                  << p[i].properties1_.queueCount << std::endl;
      }
    }
    {
      const auto& p = info.memoryProperties();
      for (zinvul::uint32b i = 0; i < p.properties1_.memoryTypeCount; ++i) {
        std::cout << "  MemoryType[" << i << "]" << std::endl;
        std::cout << "    MemoryType: "
                  << vk::to_string(p.properties1_.memoryTypes[i].propertyFlags)
                  << std::endl;
        std::cout << "    HeapIndex: "
                  << p.properties1_.memoryTypes[i].heapIndex << std::endl;
      }
      for (zinvul::uint32b i = 0; i < p.properties1_.memoryHeapCount; ++i) {
        std::cout << "  HeapType[" << i << "]" << std::endl;
        std::cout << "    HeapType: "
                  << vk::to_string(p.properties1_.memoryHeaps[i].flags)
                  << std::endl;
        std::cout << "    HeapSize: "
                  << to_gb_size(p.properties1_.memoryHeaps[i].size)
                  << " GB" << std::endl;
        std::cout << "    HeapBudget: "
                  << to_gb_size(p.budget_.heapBudget[i])
                  <<  " GB" << std::endl;
        std::cout << "    HeapUsage: "
                  << to_gb_size(p.budget_.heapUsage[i])
                  <<  " GB" << std::endl;
      }
    }
    {
      std::cout << "  Extensions" << std::endl;
      const auto& e = info.extensionPropertiesList();
      {
        std::vector<vk::ExtensionProperties> extensions;
        extensions.reserve(e.size());
        for (const auto& ext : e)
          extensions.emplace_back(ext.properties1_);
        auto cmp = [](const vk::ExtensionProperties& lhs,
                      const vk::ExtensionProperties& rhs)
        {
          return std::strcmp(lhs.extensionName, rhs.extensionName) < 0;
        };
        std::sort(extensions.begin(), extensions.end(), cmp);
        for (const auto& ext : extensions) {
          std::cout << "    " << ext.extensionName << ": "
                    << to_version_string(ext.specVersion) << std::endl;
        }
      }
    }
    {
      std::cout << "  Layers" << std::endl;
      const auto& e = info.layerPropertiesList();
      {
        std::vector<vk::LayerProperties> layers;
        layers.reserve(e.size());
        for (const auto& layer : e)
          layers.emplace_back(layer.properties1_);
        auto cmp = [](const vk::LayerProperties& lhs,
                      const vk::LayerProperties& rhs)
        {
          return std::strcmp(lhs.layerName, rhs.layerName) < 0;
        };
        std::sort(layers.begin(), layers.end(), cmp);
        for (const auto& layer : layers) {
          std::cout << "    " << layer.layerName << ": "
                    << to_version_string(layer.specVersion) << std::endl;
        }
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

    auto buffer0 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    buffer0->setSize(resolution);
    {
      std::vector<cl::float4> inputs;
      inputs.resize(resolution);
      buffer0->write(inputs.data(), resolution, 0, 0);
    }

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);


    auto kernel = zinvul::makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(experiment, experiment));
    kernel->run(*buffer0, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    std::cout << std::endl;
  }
}
