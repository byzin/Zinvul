/*!
  \file vulkan_sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_SUB_PLATFORM_HPP
#define ZINVUL_VULKAN_SUB_PLATFORM_HPP

// Standard C++ library
#include <cstddef>
#include <map>
#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/non_copyable.hpp"
// Zinvul
#include "vulkan_device_info.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zinvul/device.hpp"
#include "zinvul/sub_platform.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class Platform;
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanSubPlatform : public SubPlatform
{
 public:
  //! Create an empty vulkan sub-platform
  VulkanSubPlatform(Platform* platform) noexcept;

  //! Finalize the sub-platform
  ~VulkanSubPlatform() noexcept override;


  //! Return the used Vulkan API version
  static constexpr uint32b apiVersion() noexcept;

  //! Return the device list
  const zisc::pmr::vector<VkPhysicalDevice>& deviceList() const noexcept;

  //! Return the device info list
  const zisc::pmr::vector<VulkanDeviceInfo>& deviceInfoList() const noexcept;

  //! Return the dispatcher of vulkan objects
  const VulkanDispatchLoader& dispatcher() const noexcept;

  //! Return the engine name
  std::string_view engineName() const noexcept;

  //! Add the underlying device info into the given list
  void getDeviceInfoList(zisc::pmr::vector<const DeviceInfo*>& device_info_list) const noexcept override;

  //! Return the underlying Vulkan instance
  VkInstance& instance() noexcept;

  //! Return the underlying Vulkan instance
  const VkInstance& instance() const noexcept;

  //! Check if the sub-platform is available
  bool isAvailable() const noexcept override;

  //! Make a host memory allocator for Vulkan object
  VkAllocationCallbacks makeAllocator() noexcept;

  //! Make a unique device
  SharedDevice makeDevice(const DeviceInfo& device_info) override;

  //! Return the number of available devices
  std::size_t numOfDevices() const noexcept override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Update the device info list
  void updateDeviceInfoList() noexcept override;

 protected:
  //! Destroy the sub-platform
  void destroyData() noexcept override;

  //! Initialize the sub-platform
  void initData(PlatformOptions& platform_options) override;

 private:
  using MemoryMap = zisc::pmr::map<std::size_t, std::pair<size_t, size_t>>;

  /*!
    \brief No brief description

    No detailed description.
    */
  class AllocatorData : private zisc::NonCopyable<AllocatorData>
  {
   public:
    //! Initialize the allocator data
    AllocatorData(zisc::pmr::memory_resource* mem_resource,
                  MemoryMap&& memory_map) noexcept;

    zisc::pmr::memory_resource* mem_resource_;
    MemoryMap mem_map_; //!< \todo Make this thread safe
  };

  /*!
    \brief No brief description

    No detailed description.
    */
  class Callbacks
  {
   public:
    // Type aliases
    using DebugMessengerReturnType = std::invoke_result_t<
        PFN_vkDebugUtilsMessengerCallbackEXT,
        VkDebugUtilsMessageSeverityFlagBitsEXT,
        VkDebugUtilsMessageTypeFlagsEXT,
        const VkDebugUtilsMessengerCallbackDataEXT*,
        void*>;
    using AllocationReturnType = std::invoke_result_t<
        PFN_vkAllocationFunction,
        void*,
        size_t,
        size_t,
        VkSystemAllocationScope>;
    using ReallocationReturnType = std::invoke_result_t<
        PFN_vkReallocationFunction,
        void*,
        void*,
        size_t,
        size_t,
        VkSystemAllocationScope>;


    //! Allocate a memory block
    static AllocationReturnType allocateMemory(
        void* user_data,
        size_t size,
        size_t alignment,
        VkSystemAllocationScope scope);

    //! Free a memory block
    static void freeMemory(
        void* user_data,
        void* memory);

    //! Notify of a memory allocation
    static void notifyOfMemoryAllocation(
        void* user_data,
        size_t size,
        VkInternalAllocationType type,
        VkSystemAllocationScope scope);

    //! Notify of a memory freeing
    static void notifyOfMemoryFreeing(
        void* user_data,
        size_t size,
        VkInternalAllocationType type,
        VkSystemAllocationScope scope);

    //! Print a debug message
    static DebugMessengerReturnType printDebugMessage(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity_flags,
        VkDebugUtilsMessageTypeFlagsEXT message_types,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data);

    //! Reallocate a memory block
    static ReallocationReturnType reallocateMemory(
        void* user_data,
        void* original_memory,
        size_t,
        size_t,
        VkSystemAllocationScope);
  };

  //! Make an application info
  VkApplicationInfo makeApplicationInfo(
      const std::string_view app_name,
      const uint32b app_version_major,
      const uint32b app_version_minor,
      const uint32b app_version_patch) const noexcept;

  //! Initialize an allocator which used in the sub-platform
  void initAllocator() noexcept;

  //! Make a vulkan instance
  void initInstance(PlatformOptions& platform_options);

  //! Initialize the physical device list
  void initDeviceList();

  //! Initialize the device info list
  void initDeviceInfoList() noexcept;

  //! Initialize the vulkan dispatch loader
  void initDispatcher(PlatformOptions& platform_options);


  VkInstance instance_ = VK_NULL_HANDLE;
  std::add_pointer_t<VkInstance> instance_ref_ = nullptr;
  zisc::pmr::unique_ptr<AllocatorData> allocator_data_;
  zisc::pmr::unique_ptr<VulkanDispatchLoader> dispatcher_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<VkPhysicalDevice>> device_list_;
  zisc::pmr::unique_ptr<zisc::pmr::vector<VulkanDeviceInfo>> device_info_list_;
  char engine_name_[32] = "Zinvul";
};

} // namespace zinvul

#include "vulkan_sub_platform-inl.hpp"

#endif // ZINVUL_VULKAN_SUB_PLATFORM_HPP
