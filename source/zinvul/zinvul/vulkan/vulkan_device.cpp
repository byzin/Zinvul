/*!
  \file vulkan_device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_device.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>
// Vulkan
#include <vulkan/vulkan.h>
// VMA
#include <vk_mem_alloc.h>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_device_info.hpp"
#include "vulkan_sub_platform.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zinvul/device_info.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"

namespace zinvul {

void* getUserData(VmaAllocator alloc) noexcept;

void setUserData(VmaAllocator alloc, void* data) noexcept;

/*!
  \details No detailed description

  \param [in] id No description.
  */
VulkanDevice::VulkanDevice(IdData&& id) : Device(std::move(id))
{
}

/*!
  \details No detailed description
  */
VulkanDevice::~VulkanDevice() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in] size No description.
  \param [in] buffer_usage No description.
  \param [in] user_data No description.
  \param [out] buffer No description.
  \param [out] vm_allocation No description.
  \param [out] alloc_info No description.
  */
void VulkanDevice::allocateMemory(const std::size_t size,
                                  const BufferUsage buffer_usage,
                                  void* user_data,
                                  VkBuffer* buffer,
                                  VmaAllocation* vm_allocation,
                                  VmaAllocationInfo* alloc_info)
{
  // Buffer create info
  zinvulvk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = size;
  buffer_create_info.usage = zinvulvk::BufferUsageFlagBits::eTransferSrc |
                             zinvulvk::BufferUsageFlagBits::eTransferDst |
                             zinvulvk::BufferUsageFlagBits::eStorageBuffer;
  buffer_create_info.sharingMode = zinvulvk::SharingMode::eExclusive;
  const uint32b queue_family_index = queueFamilyIndex();
  buffer_create_info.queueFamilyIndexCount = 1;
  buffer_create_info.pQueueFamilyIndices = std::addressof(queue_family_index);

  // VMA allocation create info
  VmaAllocationCreateInfo alloc_create_info;
  alloc_create_info.flags = 0;
  switch (buffer_usage) {
   case BufferUsage::kDeviceOnly: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    break;
   }
   case BufferUsage::kHostOnly: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    break;
   }
   case BufferUsage::kHostToDevice: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    break;
   }
   case BufferUsage::kDeviceToHost: {
    alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
    break;
   }
  }
  alloc_create_info.requiredFlags = 0;
  alloc_create_info.preferredFlags = 0;
  alloc_create_info.memoryTypeBits = 0;
  alloc_create_info.pool = VK_NULL_HANDLE;
  alloc_create_info.pUserData = user_data;

  // 
  const auto binfo = zisc::cast<VkBufferCreateInfo>(buffer_create_info);
  const auto result = vmaCreateBuffer(memoryAllocator(),
                                      std::addressof(binfo),
                                      std::addressof(alloc_create_info),
                                      buffer,
                                      vm_allocation,
                                      alloc_info);
  if (result != VK_SUCCESS) {
    //! \todo Handle exception
    printf("[Warning]: Device memory allocation failed.\n");
  }
}

///*!
//  */
//template <std::size_t kDimension> inline
//std::array<uint32b, 3> VulkanDevice::calcWorkGroupSize(
//    const std::array<uint32b, kDimension>& works) const noexcept
//{
//  std::array<uint32b, 3> work_group_size{{1, 1, 1}};
//  const auto& local_work_size = localWorkSize<kDimension>();
//  for (std::size_t i = 0; i < kDimension; ++i) {
//    work_group_size[i] = ((works[i] % local_work_size[i]) == 0)
//        ? works[i] / local_work_size[i]
//        : works[i] / local_work_size[i] + 1;
//  }
//  return work_group_size;
//}
//
///*!
//  */
//inline
//auto VulkanDevice::commandPool(const QueueType queue_type) noexcept
//    -> vk::CommandPool&
//{
//  const std::size_t list_index = zisc::cast<std::size_t>(queue_type);
//  const std::size_t ref_index = queue_family_index_ref_list_[list_index];
//  return command_pool_list_[ref_index];
//}
//
///*!
//  */
//inline
//auto VulkanDevice::commandPool(const QueueType queue_type) const noexcept
//    -> const vk::CommandPool&
//{
//  const std::size_t list_index = zisc::cast<std::size_t>(queue_type);
//  const std::size_t ref_index = queue_family_index_ref_list_[list_index];
//  return command_pool_list_[ref_index];
//}

void VulkanDevice::deallocateMemory(VkBuffer* buffer,
                                    VmaAllocation* vm_allocation,
                                    VmaAllocationInfo* alloc_info) noexcept
{
  if (zinvulvk::Buffer{*buffer}) {
    vmaDestroyBuffer(memoryAllocator(), *buffer, *vm_allocation);
  }
}

///*!
//  */
//template <DescriptorType kDescriptor, typename Type> inline
//void VulkanDevice::deallocate(VulkanBuffer<kDescriptor, Type>* buffer) noexcept
//{
//  ZISC_ASSERT(buffer != nullptr, "The buffer is null.");
//  auto& b = buffer->buffer();
//  auto& memory = buffer->memory();
//  auto& alloc_info = buffer->allocationInfo();
//  if (b) {
//    vmaDestroyBuffer(allocator_, *reinterpret_cast<VkBuffer*>(&b), memory);
//
//    if (buffer->isDeviceMemory()) {
//      const std::size_t memory_usage = deviceMemoryUsage() - buffer->memoryUsage();
//      setDeviceMemoryUsage(memory_usage);
//    }
//    else {
//      const std::size_t memory_usage = hostMemoryUsage() - buffer->memoryUsage();
//      setHostMemoryUsage(memory_usage);
//    }
//
//    b = nullptr;
//    memory = VK_NULL_HANDLE;
//    alloc_info.size = 0;
//  }
//}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDevice::numOfQueues() const noexcept
{
  const auto& info = deviceInfoData();
  const uint32b index = queueFamilyIndex();
  const auto& queue_family_list = info.queueFamilyPropertiesList();

  const auto& p = queue_family_list[index].properties1_;
  const std::size_t n = p.queueCount;
  return n;
}

/*!
  \details No detailed description

  \param [in] number No description.
  \return No description
  */
std::size_t VulkanDevice::peakMemoryUsage(const std::size_t number) const noexcept
{
  std::size_t s = 0;
  if (heap_usage_list_) {
    const auto& usage = (*heap_usage_list_)[number];
    s = usage.peak();
  }
  return s;
}

/*!
  \details No detailed description

  \param [in] number No description.
  \return No description
  */
std::size_t VulkanDevice::totalMemoryUsage(const std::size_t number) const noexcept
{
  std::size_t s = 0;
  if (heap_usage_list_) {
    const auto& usage = (*heap_usage_list_)[number];
    s = usage.total();
  }
  return s;
}

/*!
  \details No detailed description
  */
void VulkanDevice::destroyData() noexcept
{
  queue_family_index_ = invalidQueueIndex();

  if (vm_allocator_) {
    vmaDestroyAllocator(vm_allocator_);
    vm_allocator_ = VK_NULL_HANDLE;
  }

//  if (device_) {
//    for (auto& module : shader_module_list_) {
//      if (module) {
//        device_.destroyShaderModule(module);
//        module = nullptr;
//      }
//    }
//    for (std::size_t i = 0; i < command_pool_list_.size(); ++i) {
//      auto command_pool = command_pool_list_[i];
//      if (command_pool) {
//        device_.destroyCommandPool(command_pool);
//        command_pool_list_[i] = nullptr;
//      }
//    }
//    device_.destroy();
//    device_ = nullptr;
//  }

  zinvulvk::Device d{device()};
  if (d) {
    auto& sub_platform = parentImpl();
    zinvulvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};
    const auto loader = dispatcher().loaderImpl();
    d.destroy(alloc, *loader);
    device_ = VK_NULL_HANDLE;
  }

  dispatcher_.reset();
  heap_usage_list_.reset();
}

/*!
  \details No detailed description
  */
void VulkanDevice::initData()
{
  {
    auto mem_resource = memoryResource();
    using UsageList = decltype(heap_usage_list_)::element_type;
    zisc::pmr::polymorphic_allocator<zisc::Memory::Usage> alloce{mem_resource};
    UsageList usage_list{alloce};

    zisc::pmr::polymorphic_allocator<UsageList> alloc{mem_resource};
    heap_usage_list_ = zisc::pmr::allocateUnique(alloc, std::move(usage_list));

    const auto& info = deviceInfoData();
    heap_usage_list_->resize(info.numOfHeaps());
  }

  initDispatcher();
  initLocalWorkGroupSize();
  initQueueFamilyIndexList();
  initDevice();
  initMemoryAllocator();
//  initCommandPool();
}

///*!
//  */
//inline
//const vk::ShaderModule& VulkanDevice::getShaderModule(
//    const std::size_t index) const noexcept
//{
//  ZISC_ASSERT(hasShaderModule(index), "The shader module doesn't exist.");
//  return shader_module_list_[index];
//}

///*!
//  */
//inline
//bool VulkanDevice::hasShaderModule(const std::size_t index) const noexcept
//{
//  const bool flag = (index < shader_module_list_.size()) &&
//                    shader_module_list_[index];
//  return flag;
//}

///*!
//  */
//template <std::size_t kDimension> inline
//const std::array<uint32b, 3>& VulkanDevice::localWorkSize() const noexcept
//{
//  static_assert((0 < kDimension) && (kDimension <= 3),
//                "The dimension is out of range.");
//  return local_work_size_list_[kDimension - 1];
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename Type> inline
//UniqueBuffer<kDescriptor, Type> VulkanDevice::makeBuffer(
//    const BufferUsage usage_flag) noexcept
//{
//  using UniqueVulkanBuffer =
//      zisc::UniqueMemoryPointer<VulkanBuffer<kDescriptor, Type>>;
//  auto buffer = UniqueVulkanBuffer::make(memoryResource(), this, usage_flag);
//  return std::move(buffer);
//}
//
///*!
//  */
//template <std::size_t kDimension, typename Function, typename ...ArgumentTypes>
//inline
//UniqueKernel<kDimension, ArgumentTypes...> VulkanDevice::makeKernel(
//    const uint32b module_index,
//    const std::string_view kernel_name) noexcept
//{
//  using UniqueVulkanKernel = zisc::UniqueMemoryPointer<VulkanKernel<
//      kDimension,
//      Function,
//      ArgumentTypes...>>;
//  UniqueKernel<kDimension, ArgumentTypes...> kernel =
//      UniqueVulkanKernel::make(memoryResource(), this, module_index, kernel_name);
//  return kernel;
//}
//
///*!
//  */
//inline
//VmaAllocator& VulkanDevice::memoryAllocator() noexcept
//{
//  return allocator_;
//}
//
///*!
//  */
//inline
//const VmaAllocator& VulkanDevice::memoryAllocator() const noexcept
//{
//  return allocator_;
//}

///*!
//  */
//inline
//void VulkanDevice::setShaderModule(const zisc::pmr::vector<uint32b>& spirv_code,
//                                   const std::size_t index) noexcept
//{
//  if (shader_module_list_.size() <= index)
//    shader_module_list_.resize(index + 1);
//  else if (hasShaderModule(index))
//    device_.destroyShaderModule(getShaderModule(index));
//
//  const vk::ShaderModuleCreateInfo create_info{vk::ShaderModuleCreateFlags{},
//                                               4 * spirv_code.size(),
//                                               spirv_code.data()};
//  auto [result, shader_module] = device_.createShaderModule(create_info);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Shader module creation failed.");
//  shader_module_list_[index] = shader_module;
//}

///*!
//  */
//inline
//void VulkanDevice::submit(const QueueType queue_type,
//                          const uint32b queue_index,
//                          const vk::CommandBuffer& command) const noexcept
//{
//  auto q = getQueue(queue_type, queue_index);
//  const vk::SubmitInfo info{0, nullptr, nullptr, 1, &command};
//  const auto result = q.submit(1, &info, nullptr);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Command submission failed.");
//  (void)result;
//}

///*!
//  \details No detailed description
//  */
//void VulkanDevice::waitForCompletion() const noexcept
//{
////  const auto result = device_.waitIdle();
////  ZISC_ASSERT(result == vk::Result::eSuccess, "Device waiting failed.");
////  (void)result;
//}
//
///*!
//  \details No detailed description
//
//  \param [in] queue_type No description.
//  */
//void VulkanDevice::waitForCompletion(const QueueType queue_type) const noexcept
//{
////  const uint32b family_index = queueFamilyIndex(queue_type);
////  const auto& info = physicalDeviceInfo();
////  const auto& family_info_list = info.queueFamilyPropertiesList();
////  const auto& family_info = family_info_list[family_index].properties1_;
////  for (uint32b i = 0; i < family_info.queueCount; ++i)
////    waitForCompletion(queue_type, i);
//}
//
///*!
//  \details No detailed description
//
//  \param [in] queue_type No description.
//  \param [in] queue_index No description.
//  */
//void VulkanDevice::waitForCompletion(const QueueType queue_type,
//                                     const uint32b queue_index) const noexcept
//{
////  auto q = getQueue(queue_type, queue_index);
////  const auto result = q.waitIdle();
////  ZISC_ASSERT(result == vk::Result::eSuccess, "Queue waiting failed.");
////  (void)result;
//}

/*!
  \details No detailed description

  \param [in] device No description.
  \param [in] memory_type No description.
  \param [out] number No description.
  \return No description
  */
bool VulkanDevice::Callbacks::getHeapNumber(const VulkanDevice& device,
                                           const uint32b memory_type,
                                           std::size_t* number) noexcept
{
  const auto& info = device.deviceInfoData();
  const auto& mem_props = info.memoryProperties().properties1_;
  const std::size_t index = mem_props.memoryTypes[memory_type].heapIndex;
  const zinvulvk::MemoryHeap heap{mem_props.memoryHeaps[index]};
  bool is_device_heap = false;
  if (heap.flags & zinvulvk::MemoryHeapFlagBits::eDeviceLocal) {
    *number = info.getDeviceHeapNumber(index);
    is_device_heap = true;
  }
  return is_device_heap;
}

/*!
  \details No detailed description

  \param [in] vm_allocator No description.
  \param [in] memory_type No description.
  \param [in] memory No description.
  \param [in] size No description.
  */
void VulkanDevice::Callbacks::notifyOfDeviceMemoryAllocation(
    VmaAllocator vm_allocator,
    uint32b memory_type,
    VkDeviceMemory /* memory */,
    VkDeviceSize size)
{
  auto device = zisc::cast<VulkanDevice*>(getUserData(vm_allocator));
  std::size_t heap_index = 0;
  const bool is_index_found = getHeapNumber(*device, memory_type, &heap_index);
  if (is_index_found)
    (*device->heap_usage_list_)[heap_index].add(size);
}

/*!
  \details No detailed description

  \param [in] vm_allocator No description.
  \param [in] memory_type No description.
  \param [in] memory No description.
  \param [in] size No description.
  */
void VulkanDevice::Callbacks::notifyOfDeviceMemoryFreeing(
    VmaAllocator vm_allocator,
    uint32b memory_type,
    VkDeviceMemory /* memory */,
    VkDeviceSize size)
{
  auto device = zisc::cast<VulkanDevice*>(getUserData(vm_allocator));
  std::size_t heap_index = 0;
  const bool is_index_found = getHeapNumber(*device, memory_type, &heap_index);
  if (is_index_found)
    (*device->heap_usage_list_)[heap_index].release(size);
}

/*!
  \details No detailed description

  \return No description
  */
uint32b VulkanDevice::findQueueFamily() const noexcept
{
  auto has_flags = [](const zinvulvk::QueueFamilyProperties& props,
                      const bool graphics_excluded,
                      const bool sparse_excluded) noexcept
  {
    constexpr auto graphics = zinvulvk::QueueFlagBits::eGraphics;
    constexpr auto compute = zinvulvk::QueueFlagBits::eCompute;
    constexpr auto transfer = zinvulvk::QueueFlagBits::eTransfer;
    constexpr auto sparse = zinvulvk::QueueFlagBits::eSparseBinding;

    auto has_flag = [&props](const zinvulvk::QueueFlagBits flag) noexcept
    {
      const bool result = (props.queueFlags & flag) == flag;
      return result;
    };

    const bool result = (!has_flag(graphics) || !graphics_excluded) &&
                        has_flag(compute) &&
                        has_flag(transfer) &&
                        (!has_flag(sparse) || !sparse_excluded);
    return result;
  };

  const auto& info = deviceInfoData();
  const auto& queue_family_list = info.queueFamilyPropertiesList();

  uint32b index = invalidQueueIndex();
  uint32b index2 = invalidQueueIndex();
  uint32b num_of_queues = 0;
  uint32b num_of_queues2 = 0;

  for (std::size_t i = 0; i < queue_family_list.size(); ++i) {
    const auto& p = queue_family_list[i].properties1_;
    if (has_flags(p, true, false) && (num_of_queues <= p.queueCount)) {
      index = zisc::cast<uint32b>(i);
      num_of_queues = p.queueCount;
    }
    if (has_flags(p, false, false) && (num_of_queues2 <= p.queueCount)) {
      index2 = zisc::cast<uint32b>(i);
      num_of_queues2 = p.queueCount;
    }
  }

  index = (index != invalidQueueIndex()) ? index : index2;
  return index;
}

/*!
  \details No detailed description

  \return No description
  */
VmaVulkanFunctions VulkanDevice::getVmaVulkanFunctions() noexcept
{
  const auto loader = dispatcher().loaderImpl();
  VmaVulkanFunctions functions;
  functions.vkGetPhysicalDeviceProperties = loader->vkGetPhysicalDeviceProperties;
  functions.vkGetPhysicalDeviceMemoryProperties = loader->vkGetPhysicalDeviceMemoryProperties;
  functions.vkAllocateMemory = loader->vkAllocateMemory;
  functions.vkFreeMemory = loader->vkFreeMemory;
  functions.vkMapMemory = loader->vkMapMemory;
  functions.vkUnmapMemory = loader->vkUnmapMemory;
  functions.vkFlushMappedMemoryRanges = loader->vkFlushMappedMemoryRanges;
  functions.vkInvalidateMappedMemoryRanges = loader->vkInvalidateMappedMemoryRanges;
  functions.vkBindBufferMemory = loader->vkBindBufferMemory;
  functions.vkBindImageMemory = loader->vkBindImageMemory;
  functions.vkGetBufferMemoryRequirements = loader->vkGetBufferMemoryRequirements;
  functions.vkGetImageMemoryRequirements = loader->vkGetImageMemoryRequirements;
  functions.vkCreateBuffer = loader->vkCreateBuffer;
  functions.vkDestroyBuffer = loader->vkDestroyBuffer;
  functions.vkCreateImage = loader->vkCreateImage;
  functions.vkDestroyImage = loader->vkDestroyImage;
  functions.vkCmdCopyBuffer = loader->vkCmdCopyBuffer;

  if (loader->vkGetBufferMemoryRequirements2)
    functions.vkGetBufferMemoryRequirements2KHR = loader->vkGetBufferMemoryRequirements2;
  else
    functions.vkGetBufferMemoryRequirements2KHR = loader->vkGetBufferMemoryRequirements2KHR;
  if (loader->vkGetImageMemoryRequirements2)
    functions.vkGetImageMemoryRequirements2KHR = loader->vkGetImageMemoryRequirements2;
  else 
    functions.vkGetImageMemoryRequirements2KHR = loader->vkGetImageMemoryRequirements2KHR;
  if (loader->vkBindBufferMemory2)
    functions.vkBindBufferMemory2KHR = loader->vkBindBufferMemory2;
  else
    functions.vkBindBufferMemory2KHR = loader->vkBindBufferMemory2KHR;
  if (loader->vkBindImageMemory2)
    functions.vkBindImageMemory2KHR = loader->vkBindImageMemory2;
  else
    functions.vkBindImageMemory2KHR = loader->vkBindImageMemory2KHR;
  if (loader->vkGetPhysicalDeviceMemoryProperties2)
    functions.vkGetPhysicalDeviceMemoryProperties2KHR = loader->vkGetPhysicalDeviceMemoryProperties2;
  else
    functions.vkGetPhysicalDeviceMemoryProperties2KHR = loader->vkGetPhysicalDeviceMemoryProperties2KHR;
  return functions;
}

///*!
//  */
//inline
//vk::Queue VulkanDevice::getQueue(const QueueType queue_type,
//                                 const uint32b queue_index) const noexcept
//{
//  const uint32b family_index = queueFamilyIndex(queue_type);
//  const auto& info = physicalDeviceInfo();
//  const auto& family_info_list = info.queueFamilyPropertiesList();
//  const auto& family_info = family_info_list[family_index].properties1_;
//  const uint32b index = queue_index % family_info.queueCount;
//  auto q = device_.getQueue(family_index, index);
//  return q;
//}
//
///*!
//  */
//inline
//void VulkanDevice::initCommandPool() noexcept
//{
//  command_pool_list_.reserve(queue_family_index_list_.size());
//  for (std::size_t i = 0; i < queue_family_index_list_.size(); ++i) {
//    const vk::CommandPoolCreateInfo pool_info{
//        vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
//        queue_family_index_list_[i]};
//    auto [result, command_pool] = device_.createCommandPool(pool_info);
//    ZISC_ASSERT(result == vk::Result::eSuccess, "Command pool creation failed.");
//    command_pool_list_.emplace_back(command_pool);
//  }
//}

/*!
  \details No detailed description
  */
void VulkanDevice::initDevice()
{
  auto& sub_platform = parentImpl();

  zisc::pmr::vector<const char*>::allocator_type layer_alloc{memoryResource()};
  zisc::pmr::vector<const char*> layers{layer_alloc};
  zisc::pmr::vector<const char*> extensions{layer_alloc};

  extensions = {//VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME,
#if defined(Z_MAC)
                VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
#endif // Z_MAC
                VK_EXT_MEMORY_BUDGET_EXTENSION_NAME};
  if (sub_platform.isDebugMode()) {
    layers.emplace_back("VK_LAYER_KHRONOS_validation");
  }

  const auto& info = deviceInfoData();

  // Queue create info
  zisc::pmr::vector<float> priority_list{
      zisc::pmr::vector<float>::allocator_type{memoryResource()}};
  zinvulvk::DeviceQueueCreateInfo queue_create_info;
  {
    // Queue family index
    const uint32b index = queueFamilyIndex();
    queue_create_info.setQueueFamilyIndex(index);
    // Queue counts
    const auto& queue_family_list = info.queueFamilyPropertiesList();
    const auto& family_info = queue_family_list[index];
    const uint32b num_of_queues = family_info.properties1_.queueCount;
    queue_create_info.setQueueCount(num_of_queues);
    // Priorities
    priority_list.resize(num_of_queues, 1.0f);
    queue_create_info.setPQueuePriorities(priority_list.data());
  }

  // Device features
  const auto& features = info.features();
  auto b16bit_storage = features.b16bit_storage_;
  auto b8bit_storage = features.b8bit_storage_;
  auto shader_atomic_int64 = features.shader_atomic_int64_;
  auto shader_float16_int8 = features.shader_float16_int8_;
  auto variable_pointers = features.variable_pointers_;
  zinvulvk::PhysicalDeviceFeatures2 device_features;
  {
    device_features.features.shaderFloat64 = features.features1_.shaderFloat64;
    device_features.features.shaderInt64 = features.features1_.shaderInt64;
    device_features.features.shaderInt16 = features.features1_.shaderInt16;
    if (sub_platform.isDebugMode()) {
      device_features.features.vertexPipelineStoresAndAtomics =
          features.features1_.vertexPipelineStoresAndAtomics;
      device_features.features.fragmentStoresAndAtomics =
          features.features1_.fragmentStoresAndAtomics;
    }
    VulkanDeviceInfo::link(device_features,
                           b16bit_storage,
                           b8bit_storage,
                           shader_atomic_int64,
                           shader_float16_int8,
                           variable_pointers);
  }

  zinvulvk::DeviceCreateInfo device_create_info{
      zinvulvk::DeviceCreateFlags{},
      1,
      &queue_create_info,
      zisc::cast<uint32b>(layers.size()),
      layers.data(),
      zisc::cast<uint32b>(extensions.size()),
      extensions.data(),
      nullptr};
  device_create_info.setPNext(std::addressof(device_features));

  zinvulvk::AllocationCallbacks alloc{sub_platform.makeAllocator()};
  const auto loader = dispatcher().loaderImpl();
  const auto pdevice = zisc::cast<zinvulvk::PhysicalDevice>(info.device());
  auto d = pdevice.createDevice(device_create_info, alloc, *loader);
  device_ = zisc::cast<VkDevice>(d);
  dispatcher_->set(device());
}

/*!
  \details No detailed description
  */
void VulkanDevice::initDispatcher()
{
  const auto& sub_platform = parentImpl();
  dispatcher_ = zisc::pmr::allocateUnique<VulkanDispatchLoader>(
      memoryResource(),
      sub_platform.dispatcher());
}

/*!
  \details No detailed description
  */
void VulkanDevice::initLocalWorkGroupSize() noexcept
{
  const auto& info = deviceInfoData();
  const uint32b group_size = info.workGroupSize();

  for (uint32b dim = 1; dim <= work_group_size_list_.size(); ++dim) {
    std::array<uint32b, 3> work_group_size{{1, 1, 1}};
    const auto product = [](const std::array<uint32b, 3>& s) noexcept
    {
      return std::accumulate(s.begin(), s.end(), 1u, std::multiplies<uint32b>());
    };
    for (uint32b i = 0; product(work_group_size) < group_size; i = (i + 1) % dim)
      work_group_size[i] *= 2;
    ZISC_ASSERT(product(work_group_size) == group_size,
                "The work-group size should be power of 2: group size = ",
                product(work_group_size));
    work_group_size_list_[dim - 1] = work_group_size;
  }
}

/*!
  \details No detailed description
  */
void VulkanDevice::initQueueFamilyIndexList() noexcept
{
  //! \todo Support queue family option
  queue_family_index_ = findQueueFamily();
}

/*!
  \details No detailed description
  */
void VulkanDevice::initMemoryAllocator()
{
  auto& sub_platform = parentImpl();
  const auto& info = deviceInfoData();

  VkAllocationCallbacks alloc = sub_platform.makeAllocator();
  VmaDeviceMemoryCallbacks notifier = makeAllocationNotifier();
  VmaVulkanFunctions functions = getVmaVulkanFunctions();

  VmaAllocatorCreateInfo create_info;
  create_info.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
  create_info.physicalDevice = info.device();
  create_info.device = device();
  create_info.preferredLargeHeapBlockSize = 0;
  create_info.pAllocationCallbacks = std::addressof(alloc);
  create_info.pDeviceMemoryCallbacks = std::addressof(notifier);
  create_info.frameInUseCount = 0;
  create_info.pHeapSizeLimit = nullptr;
  create_info.pVulkanFunctions = std::addressof(functions);
  create_info.pRecordSettings = nullptr;
  create_info.instance = sub_platform.instance();
  create_info.vulkanApiVersion = sub_platform.apiVersion();
  auto result = vmaCreateAllocator(std::addressof(create_info),
                                   std::addressof(vm_allocator_));
  if (result != VK_SUCCESS) {
    //! \todo Handling exceptions
    printf("[Warning]: Vma creation failed.\n");
  }
  setUserData(vm_allocator_, this);
}

/*!
  \details No detailed description

  \return No description
  */
VmaDeviceMemoryCallbacks VulkanDevice::makeAllocationNotifier() noexcept
{
  VmaDeviceMemoryCallbacks notifier;
  notifier.pfnAllocate = Callbacks::notifyOfDeviceMemoryAllocation;
  notifier.pfnFree = Callbacks::notifyOfDeviceMemoryFreeing;
  return notifier;
}

} // namespace zinvul
