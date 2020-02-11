/*!
  \file vulkan_device.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_DEVICE_HPP
#define ZINVUL_VULKAN_DEVICE_HPP

// Standard C++ library
#include <array>
#include <atomic>
#include <cstddef>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
// Zisc
#include "zisc/std_memory_resource.hpp"
//#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "vulkan_device_info.hpp"
//#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
//#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"

namespace zinvul {

// Forward declaration
//template <DescriptorType, typename> class VulkanBuffer;

/*!
  \brief No brief description

  No detailed description.
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


  //! Initialize a vulkan device
  VulkanDevice(DeviceOptions& options) noexcept;

  //! Destroy a vulkan instance
  ~VulkanDevice() noexcept override;


//  //! Allocate a memory of a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  void allocate(const std::size_t size,
//                VulkanBuffer<kDescriptor, Type>* buffer) noexcept;

//  //! Return the workgroup size for the work dimension
//  template <std::size_t kDimension>
//  std::array<uint32b, 3> calcWorkGroupSize(
//      const std::array<uint32b, kDimension>& works) const noexcept;
//
//  //! Return the command pool
//  vk::CommandPool& commandPool(const QueueType queue_type) noexcept;
//
//  //! Return the command pool
//  const vk::CommandPool& commandPool(const QueueType queue_type) const noexcept;

//  //! Deallocate a memory of a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  void deallocate(VulkanBuffer<kDescriptor, Type>* buffer) noexcept;

//  //! Destroy a vulkan instance
//  void destroy() noexcept;
//
//  //! Return the device body
//  vk::Device& device() noexcept;
//
//  //! Return the device body
//  const vk::Device& device() const noexcept;

  //! Return vulkan type
  SubPlatformType subPlatformType() const noexcept override;

  //! Return the list of device info
  static zisc::pmr::vector<VulkanDeviceInfo> getDeviceInfoList(
      zisc::pmr::memory_resource* mem_resource);

//  //! Return the shader module by the index
//  const vk::ShaderModule& getShaderModule(const std::size_t index) const noexcept;
//
//  //! Return the vendor name corresponding to the vendor ID
//  static std::string getVendorName(const uint32b id) noexcept;
//
//  //! Return the subgroup size of vendor device
//  static uint32b getVendorSubgroupSize(const uint32b id) noexcept;
//
//  //! Check if the device has the shader module
//  bool hasShaderModule(const std::size_t index) const noexcept;
//
//  //! Initialize local-work size
//  void initLocalWorkSize(const uint32b subgroup_size) noexcept;

  //! Issue an ID of a vulkan object
  IdData issueId() noexcept;

//  //! Return the local-work size for the work dimension
//  template <std::size_t kDimension>
//  const std::array<uint32b, 3>& localWorkSize() const noexcept;

//  //! Make a buffer
//  template <DescriptorType kDescriptor, typename Type>
//  UniqueBuffer<kDescriptor, Type> makeBuffer(
//      const BufferUsage usage_flag) noexcept;
//
//  //! Make a kernel
//  template <std::size_t kDimension, typename Function, typename ...ArgumentTypes>
//  UniqueKernel<kDimension, ArgumentTypes...> makeKernel(
//      const uint32b module_index,
//      const std::string_view kernel_name) noexcept;

//  //! Return the memory allocator of the device
//  VmaAllocator& memoryAllocator() noexcept;
//
//  //! Return the memory allocator of the device
//  const VmaAllocator& memoryAllocator() const noexcept;

  //! Return the device name
  std::string_view name() const noexcept override;

  //! Return the physical device info
//  const VulkanPhysicalDeviceInfo& physicalDeviceInfo() const noexcept;

  //! Set a shader module
//  void setShaderModule(const zisc::pmr::vector<uint32b>& spirv_code,
//                       const std::size_t index) noexcept;

  //! Return the subgroup size
  uint32b subgroupSize() const noexcept override;

  //! Submit a command
//  void submit(const QueueType queue_type,
//              const uint32b queue_index,
//              const vk::CommandBuffer& command) const noexcept;

  //! Return the vendor name
  std::string_view vendorName() const noexcept override;

  //! Wait this thread until all commands in the device are completed
  void waitForCompletion() const noexcept override;

  //! Wait this thread until all commands in the queues are completed
  void waitForCompletion(const QueueType queue_type) const noexcept override;

  //! Wait this thread until all commands in the queue are completed
  void waitForCompletion(const QueueType queue_type,
                         const uint32b queue_index) const noexcept override;

 private:
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

  //! Find the index of the optimal queue familty
  uint32b findQueueFamily(const QueueType queue_type) const noexcept;

  //! Return a queue
  vk::Queue getQueue(const QueueType queue_type,
                     const uint32b queue_index) const noexcept;

  //! Initialize a command pool
  void initCommandPool() noexcept;

  //! Initialize a debug messenger
  void initDebugMessenger() noexcept;

  //! Initialize a device
  void initDevice(const DeviceOptions& options) noexcept;

  //! Initialize a memory allocator
  void initMemoryAllocator() noexcept;

  //! Initialize a vulkan device
  void initialize(const DeviceOptions& options) noexcept;

  //! Initialize a physical device
  void initPhysicalDevice(const DeviceOptions& options) noexcept;

  //! Initialize a queue family index list
  void initQueueFamilyIndexList() noexcept;

  //! Make an allocator
  static vk::AllocationCallbacks makeAllocator(
      zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Make an application info
  static vk::ApplicationInfo makeApplicationInfo(
      const std::string_view app_name,
      const uint32b app_version_major,
      const uint32b app_version_minor,
      const uint32b app_version_patch) noexcept;

  //! Make a vulkan instance
  static vk::Instance makeInstance(const vk::ApplicationInfo& app_info,
                                   const bool enable_debug,
                                   vk::AllocationCallbacks* alloc);

  //! Return an index of a queue family
  uint32b queueFamilyIndex(const QueueType queue_type) const noexcept;


//  VulkanPhysicalDeviceInfo device_info_;
//  zisc::pmr::vector<vk::ShaderModule> shader_module_list_;
//  zisc::pmr::vector<vk::CommandPool> command_pool_list_;
  std::atomic<uint32b> id_count_;
  vk::ApplicationInfo app_info_;
//  vk::Instance instance_;
//  vk::DebugUtilsMessengerEXT debug_messenger_;
//  vk::PhysicalDevice physical_device_;
//  vk::Device device_;
//  VmaAllocator allocator_ = VK_NULL_HANDLE;
//  std::string vendor_name_;
//  zisc::pmr::vector<uint32b> queue_family_index_list_;
//  std::array<std::size_t, 2> queue_family_index_ref_list_;
//  std::array<std::array<uint32b, 3>, 3> local_work_size_list_;
};

} // namespace zinvul

//#include "vulkan_device-inl.hpp"

#endif // ZINVUL_VULKAN_DEVICE_HPP
