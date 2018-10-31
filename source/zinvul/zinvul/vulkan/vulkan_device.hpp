/*!
  \file vulkan_device.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_DEVICE_HPP
#define ZINVUL_VULKAN_DEVICE_HPP

// Standard C++ library
#include <cstddef>
#include <string>
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
// Zisc
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
class VulkanDevice : public Device
{
 public:
  enum class VendorId : uint32b
  {
    kAmd = 0x1002, // AMD
    kImgTec = 0x1010, // ImgTec
    kNvidia = 0x10de, // NVIDIA
    kArm = 0x13b5, // ARM
    kQualcomm = 0x5143, // Qualcomm
    kIntel = 0x8086 // INTEL
  };

  /*!
    */
  struct PhysicalDeviceInfo
  {
    std::vector<vk::QueueFamilyProperties> queue_family_properties_;
    vk::PhysicalDeviceProperties properties_;
    vk::PhysicalDeviceFeatures features_;
    vk::PhysicalDeviceSubgroupProperties subgroup_properties_;
    vk::PhysicalDeviceMemoryProperties memory_properties_;
  };


  //! Initialize a vulkan device
  VulkanDevice(DeviceOptions& options) noexcept;


  //! Return vulkan type
  DeviceType deviceType() const noexcept override;

  //! Return the vendor name corresponding to the vendor ID
  static std::string getVendorName(const uint32b vendorId) noexcept;

  //! Make a buffer
  template <typename Type>
  UniqueBuffer<Type> makeBuffer(const int usage_flags) noexcept;

  //! Make a kernel
  template <typename GroupType, typename ...ArgumentTypes>
  UniqueKernel<GroupType, ArgumentTypes...> makeKernel(
      const typename Kernel<GroupType, ArgumentTypes...>::KernelFunction func) noexcept;

  //! Return the list of device info
  static std::vector<PhysicalDeviceInfo> getPhysicalDeviceInfoList() noexcept;

 private:
  //! Get physical device info
  static PhysicalDeviceInfo getPhysicalDeviceInfo(const vk::PhysicalDevice& device)
      noexcept;

  //! Make a vulkan instance
  static vk::Instance makeInstance(const vk::ApplicationInfo& app_info) noexcept;

  //! Make an application info
  static vk::ApplicationInfo makeApplicationInfo(
      const char* app_name,
      const uint32b app_version_major,
      const uint32b app_version_minor,
      const uint32b app_version_patch) noexcept;
};

} // namespace zinvul

#include "vulkan_device-inl.hpp"

#endif // ZINVUL_VULKAN_DEVICE_HPP
