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
#include <array>
#include <cstddef>
#include <limits>
#include <string>
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
template <typename> class VulkanBuffer;

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
    std::vector<vk::QueueFamilyProperties> queue_family_list_;
    vk::PhysicalDeviceProperties properties_;
    vk::PhysicalDeviceFeatures features_;
    vk::PhysicalDeviceSubgroupProperties subgroup_properties_;
    vk::PhysicalDeviceMemoryProperties memory_properties_;
  };


  //! Initialize a vulkan device
  VulkanDevice(DeviceOptions& options) noexcept;

  //! Destroy a vulkan instance
  ~VulkanDevice() noexcept override;


  //! Allocate a memory of a buffer
  template <typename Type>
  void allocate(const std::size_t size, VulkanBuffer<Type>* buffer) noexcept;

  //! Return the command pool
  vk::CommandPool& commandPool() noexcept;

  //! Return the command pool
  const vk::CommandPool& commandPool() const noexcept;

  //! Deallocate a memory of a buffer
  template <typename Type>
  void deallocate(VulkanBuffer<Type>* buffer) noexcept;

  //! Destroy a vulkan instance
  void destroy() noexcept;

  //! Return the device body
  vk::Device& device() noexcept;

  //! Return the device body
  const vk::Device& device() const noexcept;

  //! Return vulkan type
  DeviceType deviceType() const noexcept override;

  //! Return the list of device info
  static std::vector<PhysicalDeviceInfo> getPhysicalDeviceInfoList() noexcept;

  //! Return the shader module by the index
  const vk::ShaderModule& getShaderModule(const std::size_t index) const noexcept;

  //! Return the vendor name corresponding to the vendor ID
  static std::string getVendorName(const uint32b vendorId) noexcept;

  //! Get the workgroup size
  std::array<uint32b, 3> getWorkgroupSize(const uint32b dimension) const noexcept;

  //! Check if the device has the shader module
  bool hasShaderModule(const std::size_t index) const noexcept;

  //! Make a buffer
  template <typename Type>
  UniqueBuffer<Type> makeBuffer(const BufferUsage usage_flag) noexcept;

  //! Make a kernel
  template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
  UniqueKernel<GroupType, kDimension, ArgumentTypes...> makeKernel(
      const uint32b module_index,
      const char* kernel_name) noexcept;

  //! Map a buffer memory to a host
  template <typename Type>
  void* mapMemory(const VulkanBuffer<Type>& buffer) const noexcept;

  //! Return the physical device info
  const PhysicalDeviceInfo& physicalDeviceInfo() const noexcept;

  //! Set a shader module
  void setShaderModule(const zisc::pmr::vector<uint32b>& spirv_code,
                       const std::size_t index) noexcept;

  //! Submit a command
  void submit(const uint32b queue_index,
              const vk::CommandBuffer& command) noexcept;

  //! Unmap a buffer memory
  template <typename Type>
  void unmapMemory(const VulkanBuffer<Type>& buffer) const noexcept;

  //! Wait this thread until all commands in the queue are completed
  void waitForCompletion() noexcept override;

 private:
  //! Output a debug message
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* /* userData */);

  //! Find the index of the optimal queue familty for shader
  uint32b findQueueFamilyForShader() const noexcept;

  //! Get physical device info
  static PhysicalDeviceInfo getPhysicalDeviceInfo(const vk::PhysicalDevice& device)
      noexcept;

  //! Initialize a command pool
  void initCommandPool() noexcept;

  //! Initialize a debug messenger
  void initDebugMessenger() noexcept;

  //! Initialize a device
  void initDevice() noexcept;

  //! Initialize a memory allocator
  void initMemoryAllocator() noexcept;

  //! Initialize a vulkan device
  void initialize(const DeviceOptions& options) noexcept;

  //! Initialize a physical device
  void initPhysicalDevice(const DeviceOptions& options) noexcept;

  //! Make a vulkan instance
  static vk::Instance makeInstance(const vk::ApplicationInfo& app_info) noexcept;

  //! Make an application info
  static vk::ApplicationInfo makeApplicationInfo(
      const char* app_name,
      const uint32b app_version_major,
      const uint32b app_version_minor,
      const uint32b app_version_patch) noexcept;


  PhysicalDeviceInfo device_info_;
  zisc::pmr::vector<vk::ShaderModule> shader_module_list_;
  vk::ApplicationInfo app_info_;
  vk::Instance instance_;
  vk::DebugUtilsMessengerEXT debug_messenger_;
  vk::PhysicalDevice physical_device_;
  vk::Device device_;
  vk::CommandPool command_pool_;
  VmaAllocator allocator_ = VK_NULL_HANDLE;
  uint32b queue_family_index_ = std::numeric_limits<uint32b>::max();
  uint32b memory_type_index_ = std::numeric_limits<uint32b>::max();
};

} // namespace zinvul

#include "vulkan_device-inl.hpp"

#endif // ZINVUL_VULKAN_DEVICE_HPP
