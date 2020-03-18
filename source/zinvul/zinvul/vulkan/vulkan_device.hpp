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
// VMA
#include <vk_mem_alloc.h>
// Zisc
#include "zisc/memory.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "utility/vulkan_dispatch_loader.hpp"
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"

namespace zinvul {

// Forward declaration
class DeviceInfo;
class VulkanDeviceInfo;
class VulkanSubPlatform;

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanDevice : public Device
{
 public:
  //! Initialize the vulkan device
  VulkanDevice(IdData&& id);

  //! Finalize the vulkan instance
  ~VulkanDevice() noexcept override;


  //! Allocate a device memory
  void allocateMemory(const std::size_t size,
                      const BufferUsage buffer_usage,
                      void* user_data,
                      VkBuffer* buffer,
                      VmaAllocation* vm_allocation,
                      VmaAllocationInfo* alloc_info);

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

  //! Deallocate a device memory
  void deallocateMemory(VkBuffer* buffer,
                        VmaAllocation* vm_allocation,
                        VmaAllocationInfo* alloc_info) noexcept;

  //! Return the underlying vulkan device
  VkDevice& device() noexcept;

  //! Return the underlying vulkan device
  const VkDevice& device() const noexcept;

  //! Return the underlying device info
  const VulkanDeviceInfo& deviceInfoData() const noexcept;

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

  //! Make a buffer
  template <typename Type>
  SharedBuffer<Type> makeBuffer(const BufferUsage flag);

//  //! Make a kernel
//  template <std::size_t kDimension, typename Function, typename ...ArgumentTypes>
//  UniqueKernel<kDimension, ArgumentTypes...> makeKernel(
//      const uint32b module_index,
//      const std::string_view kernel_name) noexcept;

  //! Return the memory allocator of the device
  VmaAllocator& memoryAllocator() noexcept;

  //! Return the memory allocator of the device
  const VmaAllocator& memoryAllocator() const noexcept;

  //! Return the number of underlying command queues
  std::size_t numOfQueues() const noexcept override;

  //! Return the peak memory usage of the heap of the given number
  std::size_t peakMemoryUsage(const std::size_t number) const noexcept override;

  //! Return the current memory usage of the heap of the given number
  std::size_t totalMemoryUsage(const std::size_t number) const noexcept override;

  //! Set a shader module
//  void setShaderModule(const zisc::pmr::vector<uint32b>& spirv_code,
//                       const std::size_t index) noexcept;

  //! Submit a command
//  void submit(const QueueType queue_type,
//              const uint32b queue_index,
//              const vk::CommandBuffer& command) const noexcept;

//  //! Wait this thread until all commands in the device are completed
//  void waitForCompletion() const noexcept override;
//
//  //! Wait this thread until all commands in the queues are completed
//  void waitForCompletion(const QueueType queue_type) const noexcept override;
//
//  //! Wait this thread until all commands in the queue are completed
//  void waitForCompletion(const QueueType queue_type,
//                         const uint32b queue_index) const noexcept override;

 protected:
  //! Destroy the device
  void destroyData() noexcept override;

  //! Initialize the vulkan device
  void initData() override;

 private:
  /*!
    \brief No brief description

    No detailed description.
    */
  class Callbacks
  {
   public:
    //! Return the index of memory heap
    static bool getHeapNumber(const VulkanDevice& device,
                              const uint32b memory_type,
                              std::size_t* number) noexcept;

    //! Notify of a memory allocation in VMA
    static void notifyOfDeviceMemoryAllocation(
        VmaAllocator vm_allocator,
        uint32b memory_type,
        VkDeviceMemory memory,
        VkDeviceSize size);

    //! Notify of a memory freeing in VMA
    static void notifyOfDeviceMemoryFreeing(
        VmaAllocator vm_allocator,
        uint32b memory_type,
        VkDeviceMemory memory,
        VkDeviceSize size);
  };


  //! Find the index of the optimal queue familty
  uint32b findQueueFamily() const noexcept;

  //! Get Vulkan function pointers used in VMA
  VmaVulkanFunctions getVmaVulkanFunctions() noexcept;

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

  //! Initialize work group size of dimensions
  void initLocalWorkGroupSize() noexcept;

  //! Initialize a queue family index list
  void initQueueFamilyIndexList() noexcept;

  //! Initialize a vulkan memory allocator
  void initMemoryAllocator();

  //! Make a device memory allocation notifier
  VmaDeviceMemoryCallbacks makeAllocationNotifier() noexcept;

  //! Return the sub-platform
  VulkanSubPlatform& parentImpl() noexcept;

  //! Return the sub-platform
  const VulkanSubPlatform& parentImpl() const noexcept;

  //! Return an index of a queue family
  uint32b queueFamilyIndex() const noexcept;


  VkDevice device_ = VK_NULL_HANDLE;
  VmaAllocator vm_allocator_ = VK_NULL_HANDLE;
  zisc::pmr::unique_ptr<zisc::pmr::vector<zisc::Memory::Usage>> heap_usage_list_;
  zisc::pmr::unique_ptr<VulkanDispatchLoader> dispatcher_;
//  zisc::pmr::vector<vk::ShaderModule> shader_module_list_;
//  zisc::pmr::vector<vk::CommandPool> command_pool_list_;
  uint32b queue_family_index_ = invalidQueueIndex();
  std::array<std::array<uint32b, 3>, 3> work_group_size_list_;
};

} // namespace zinvul

#include "vulkan_device-inl.hpp"

#endif // ZINVUL_VULKAN_DEVICE_HPP
