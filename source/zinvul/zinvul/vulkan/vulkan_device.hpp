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
#include <cstddef>
#include <memory>
#include <vector>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "utility/vulkan_dispatch_loader.hpp"
#include "zinvul/device.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class DeviceInfo;
class VulkanDeviceInfo;
class VulkanSubPlatform;
//template <DescriptorType, typename> class VulkanBuffer;

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanDevice : public Device
{
 public:
  //! Initialize the vulkan device
  VulkanDevice(VulkanSubPlatform* sub_platform,
               const VulkanDeviceInfo* device_info);

  //! Move a data
  VulkanDevice(VulkanDevice&& other) noexcept;

  //! Finalize the vulkan instance
  ~VulkanDevice() noexcept override;


  //! Move a data
  VulkanDevice& operator=(VulkanDevice&& other) noexcept;


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

  //! Return the underlying vulkan device
  VkDevice& device() noexcept;

  //! Return the underlying vulkan device
  const VkDevice& device() const noexcept;

  //! Return the underlying device info
  const DeviceInfo& deviceInfo() const noexcept override;

  //! Return the dispatcher of vulkan objects
  const VulkanDispatchLoader& dispatcher() const noexcept;

//  //! Return the shader module by the index
//  const vk::ShaderModule& getShaderModule(const std::size_t index) const noexcept;

//  //! Check if the device has the shader module
//  bool hasShaderModule(const std::size_t index) const noexcept;

  //! Return the invalid queue index in queue families
  static constexpr uint32b invalidQueueIndex() noexcept;

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

  //! Return the number of underlying command queues
  std::size_t numOfQueues() const noexcept override;

  //! Return the peak memory usage of the heap of the given index
  std::size_t peakMemoryUsage(const std::size_t index) const noexcept override;

  //! Return the current memory usage of the heap of the given index
  std::size_t totalMemoryUsage(const std::size_t index) const noexcept override;

  //! Set a shader module
//  void setShaderModule(const zisc::pmr::vector<uint32b>& spirv_code,
//                       const std::size_t index) noexcept;

  //! Submit a command
//  void submit(const QueueType queue_type,
//              const uint32b queue_index,
//              const vk::CommandBuffer& command) const noexcept;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

//  //! Wait this thread until all commands in the device are completed
//  void waitForCompletion() const noexcept override;
//
//  //! Wait this thread until all commands in the queues are completed
//  void waitForCompletion(const QueueType queue_type) const noexcept override;
//
//  //! Wait this thread until all commands in the queue are completed
//  void waitForCompletion(const QueueType queue_type,
//                         const uint32b queue_index) const noexcept override;

  //! Return the underlying device info
  const VulkanDeviceInfo& vulkanDeviceInfo() const noexcept;

 protected:
  //! Destroy the device
  void destroyData() noexcept override;

 private:
  //! Find the index of the optimal queue familty
  uint32b findQueueFamily() const noexcept;

//  //! Return a queue
//  vk::Queue getQueue(const QueueType queue_type,
//                     const uint32b queue_index) const noexcept;
//
//  //! Initialize a command pool
//  void initCommandPool() noexcept;

  //! Initialize a device
  void initDevice();

  //! Initialize the vulkan dispatch loader
  void initDispatcher();

  //! Initialize the vulkan device
  void initialize();

  //! Initialize work group size of dimensions
  void initLocalWorkGroupSize() noexcept;

//  //! Initialize a memory allocator
//  void initMemoryAllocator() noexcept;

  //! Initialize a queue family index list
  void initQueueFamilyIndexList() noexcept;

  //! Return the sub-platform
  VulkanSubPlatform& subPlatform() noexcept;

  //! Return the sub-platform
  const VulkanSubPlatform& subPlatform() const noexcept;

  //! Return an index of a queue family
  uint32b queueFamilyIndex() const noexcept;


  VulkanSubPlatform* sub_platform_ = nullptr;
  const VulkanDeviceInfo* device_info_ = nullptr;
  VkDevice device_ = VK_NULL_HANDLE;
  zisc::pmr::unique_ptr<VulkanDispatchLoader> dispatcher_;
//  zisc::pmr::vector<vk::ShaderModule> shader_module_list_;
//  zisc::pmr::vector<vk::CommandPool> command_pool_list_;
//  VmaAllocator allocator_ = VK_NULL_HANDLE;
  uint32b queue_family_index_ = invalidQueueIndex();
  std::array<std::array<uint32b, 3>, 3> work_group_size_list_;
};

} // namespace zinvul

#include "vulkan_device-inl.hpp"

#endif // ZINVUL_VULKAN_DEVICE_HPP
