/*!
  \file vulkan_device-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_device.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <string>
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
// Zisc
#include "zisc/error.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

#ifndef ZINVUL_VULKAN_DEVICE_INL_HPP
#define ZINVUL_VULKAN_DEVICE_INL_HPP

namespace zinvul {

/*!
  */
inline
VulkanDevice::VulkanDevice(DeviceOptions& options) noexcept :
    Device(options)
{
}

/*!
  */
inline
DeviceType VulkanDevice::deviceType() const noexcept
{
  return DeviceType::kVulkan;
}

/*!
  */
inline
std::string VulkanDevice::getVendorName(const uint32b vendor_id) noexcept
{
  using namespace std::string_literals;
  std::string vendor_name;
  switch (vendor_id) {
   case static_cast<uint32b>(VendorId::kAmd): {
    vendor_name = "AMD"s;
    break;
   }
   case static_cast<uint32b>(VendorId::kImgTec): {
    vendor_name = "ImgTec"s;
    break;
   }
   case static_cast<uint32b>(VendorId::kNvidia): {
    vendor_name = "NVIDIA"s;
    break;
   }
   case static_cast<uint32b>(VendorId::kArm): {
    vendor_name = "ARM"s;
    break;
   }
   case static_cast<uint32b>(VendorId::kQualcomm): {
    vendor_name = "Qualcomm"s;
    break;
   }
   case static_cast<uint32b>(VendorId::kIntel): {
    vendor_name = "INTEL"s;
    break;
   }
   default: {
    vendor_name = "Unknown"s;
    break;
   }
  }
  return vendor_name;
}

/*!
  */
inline
auto VulkanDevice::getPhysicalDeviceInfoList() noexcept
    -> std::vector<PhysicalDeviceInfo>
{
  const auto app_info = makeApplicationInfo("", 0, 0, 0);
  auto instance = makeInstance(app_info);
  std::vector<PhysicalDeviceInfo> device_info_list;
  if (instance) {
    auto [result, physical_device_list] = instance.enumeratePhysicalDevices();
    if (result == vk::Result::eSuccess) {
      device_info_list.reserve(physical_device_list.size());
      for (const auto& device : physical_device_list)
        device_info_list.emplace_back(getPhysicalDeviceInfo(device));
    }
  }
  instance.destroy();
  return device_info_list;
}

/*!
  */
inline
auto VulkanDevice::getPhysicalDeviceInfo(const vk::PhysicalDevice& device) noexcept
    -> PhysicalDeviceInfo
{
  PhysicalDeviceInfo device_info;
  // Queue properties
  device_info.queue_family_properties_ = device.getQueueFamilyProperties();
  // Device properties
  vk::PhysicalDeviceProperties2 properties2;
  properties2.pNext = &device_info.subgroup_properties_;
  device.getProperties2(&properties2);
  device_info.properties_ = properties2.properties;
  // Device features
  device_info.features_ = device.getFeatures();
  // Memory properties
  device_info.memory_properties_ = device.getMemoryProperties();
  return device_info;
}

/*!
  */
inline
vk::Instance VulkanDevice::makeInstance(const vk::ApplicationInfo& app_info)
    noexcept
{
  std::array<const char*, 1> layers{{
    "VK_LAYER_LUNARG_standard_validation"
    }};
  std::array<const char*, 2> extensions{{
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
  }};

  const vk::InstanceCreateInfo createInfo{
      vk::InstanceCreateFlags{},
      &app_info,
      static_cast<uint32b>(layers.size()),
      layers.data(),
      static_cast<uint32b>(extensions.size()),
      extensions.data()};
  auto [result, instance] = vk::createInstance(createInfo);

  return (result == vk::Result::eSuccess) ? instance : vk::Instance{};
}

/*!
  */
inline
vk::ApplicationInfo VulkanDevice::makeApplicationInfo(
    const char* app_name,
    const uint32b app_version_major,
    const uint32b app_version_minor,
    const uint32b app_version_patch) noexcept
{
  const uint32b app_version = VK_MAKE_VERSION(app_version_major,
                                              app_version_minor,
                                              app_version_patch);
  constexpr char engine_name[] = "Zinvul";
  constexpr uint32b engine_version = VK_MAKE_VERSION(kZinvulVersionMajor,
                                                     kZinvulVersionMinor,
                                                     kZinvulVersionPatch);
  constexpr uint32b api_version = VK_API_VERSION_1_1;
  return vk::ApplicationInfo{app_name,
                             app_version,
                             engine_name,
                             engine_version,
                             api_version};
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_DEVICE_INL_HPP
