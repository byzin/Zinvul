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
#include <numeric>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_device_info.hpp"
#include "vulkan_sub_platform.hpp"
#include "utility/vulkan.hpp"
#include "zinvul/device_info.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] sub_platform No description.
  \param [in] device_info No description.
  */
VulkanDevice::VulkanDevice(VulkanSubPlatform* sub_platform,
                           const VulkanDeviceInfo* device_info) noexcept :
    Device(sub_platform->memoryResource()),
    sub_platform_{sub_platform},
    device_info_{device_info},
    queue_family_index_list_{
        decltype(queue_family_index_list_)::allocator_type{memoryResource()}},
    queue_family_index_ref_list_{{0, 0}}
{
}

///*!
//  */
//inline
//VulkanDevice::VulkanDevice(DeviceOptions& options) noexcept :
//    Device(options),
//    device_info_{memoryResource()},
//    shader_module_list_{memoryResource()},
//    command_pool_list_{memoryResource()},
//    queue_family_index_list_{memoryResource()},
//    queue_family_index_ref_list_{{std::numeric_limits<uint32b>::max(),
//                                  std::numeric_limits<uint32b>::max()}}
//{
//  initialize(options);
//}

/*!
  \details No detailed description
  */
VulkanDevice::~VulkanDevice() noexcept
{
  destroy();
}

///*!
//  */
//template <DescriptorType kDescriptor, typename Type> inline
//void VulkanDevice::allocate(const std::size_t size,
//                            VulkanBuffer<kDescriptor, Type>* buffer) noexcept
//{
//  ZISC_ASSERT(buffer != nullptr, "The buffer is null.");
//  auto& b = buffer->buffer();
//  auto& memory = buffer->memory();
//  auto& alloc_info = buffer->allocationInfo();
//
//  vk::BufferCreateInfo buffer_create_info;
//  buffer_create_info.size = sizeof(Type) * size;
//  buffer_create_info.usage = vk::BufferUsageFlagBits::eTransferSrc |
//                             vk::BufferUsageFlagBits::eTransferDst;
//  constexpr bool is_uniform_buffer = Config::isUniformBufferEnabled() &&
//                                     (kDescriptor == DescriptorType::kUniform);
//  buffer_create_info.usage = buffer_create_info.usage | (is_uniform_buffer
//      ? vk::BufferUsageFlagBits::eUniformBuffer
//      : vk::BufferUsageFlagBits::eStorageBuffer);
//  buffer_create_info.queueFamilyIndexCount =
//      zisc::cast<uint32b>(queue_family_index_list_.size());
//  buffer_create_info.pQueueFamilyIndices = queue_family_index_list_.data();
//
//  VmaAllocationCreateInfo alloc_create_info;
//  switch (buffer->usage()) {
//   case BufferUsage::kHostOnly: {
//    alloc_create_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;
//    break;
//   }
//   case BufferUsage::kHostToDevice: {
//    alloc_create_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
//    break;
//   }
//   case BufferUsage::kDeviceToHost: {
//    alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_TO_CPU;
//    break;
//   }
//   case BufferUsage::kDeviceOnly:
//   default: {
//    alloc_create_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
//    break;
//   }
//  }
//  alloc_create_info.flags = 0;
//  alloc_create_info.requiredFlags = 0;
//  alloc_create_info.preferredFlags = 0;
//  alloc_create_info.memoryTypeBits = 0;
//  alloc_create_info.pool = VK_NULL_HANDLE;
//  alloc_create_info.pUserData = nullptr;
//
//  const auto result = vmaCreateBuffer(
//      allocator_,
//      &zisc::cast<const VkBufferCreateInfo&>(buffer_create_info),
//      &alloc_create_info,
//      reinterpret_cast<VkBuffer*>(&b),
//      &memory,
//      &alloc_info);
//  ZISC_ASSERT(result == VK_SUCCESS, "Buffer creation failed.");
//  (void)result;
//
//  if (buffer->isDeviceMemory()) {
//    const std::size_t memory_usage = deviceMemoryUsage() + buffer->memoryUsage();
//    setDeviceMemoryUsage(memory_usage);
//  }
//  else {
//    const std::size_t memory_usage = hostMemoryUsage() + buffer->memoryUsage();
//    setHostMemoryUsage(memory_usage);
//  }
//}
//
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
//
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

void VulkanDevice::destroyData() noexcept
{
  queue_family_index_ref_list_.fill(0);
  queue_family_index_list_.clear();

//  if (device_) {
//    for (auto& module : shader_module_list_) {
//      if (module) {
//        device_.destroyShaderModule(module);
//        module = nullptr;
//      }
//    }
//    if (allocator_)  {
//      vmaDestroyAllocator(allocator_);
//      allocator_ = VK_NULL_HANDLE;
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
//
//  if (debug_messenger_) {
//    auto destroyDebugUtilsMessengerEXT =
//        reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
//            instance_.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
//    destroyDebugUtilsMessengerEXT(
//        zisc::cast<VkInstance>(instance_),
//        zisc::cast<VkDebugUtilsMessengerEXT>(debug_messenger_),
//        nullptr);
//    debug_messenger_ = nullptr;
//  }
//
//  if (instance_) {
//    instance_.destroy();
//    instance_ = nullptr;
//  }

  device_info_ = nullptr;
  sub_platform_ = nullptr;
}

///*!
//  */
//inline
//vk::Device& VulkanDevice::device() noexcept
//{
//  return device_;
//}
//
///*!
//  */
//inline
//const vk::Device& VulkanDevice::device() const noexcept
//{
//  return device_;
//}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType VulkanDevice::type() const noexcept
{
  return SubPlatformType::kVulkan;
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

  \param [in] queue_type No description.
  \return No description
  */
uint32b VulkanDevice::findQueueFamily() const noexcept
{
  const auto& info = vulkanDeviceInfo();
  const auto& queue_family_list = info.queueFamilyPropertiesList();

  uint32b index = invalidQueueIndex();
  bool is_found = false;

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
                        (!has_flag(sparse) || !sparse_excluded) &&
                        has_flag(compute) &&
                        has_flag(transfer);
    return result;
  };

  constexpr auto graphics = zinvulvk::QueueFlagBits::eGraphics;

  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
    const auto& p = queue_family_list[i].properties1_;
    if (!has_flag(p, graphics) && !has_flag(p, other) && has_flag(p, target)) {
      index = zisc::cast<uint32b>(i);
      is_found = true;
    }
  }

  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
    const auto& p = queue_family_list[i].properties1_;
    if (!has_flag(p, other) && has_flag(p, target)) {
      index = zisc::cast<uint32b>(i);
      is_found = true;
    }
  }

  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
    const auto& p = queue_family_list[i].properties1_;
    if (!has_flag(p, graphics) && has_flag(p, target)) {
      index = zisc::cast<uint32b>(i);
      is_found = true;
    }
  }

  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
    const auto& p = queue_family_list[i].properties1_;
    if (has_flag(p, target)) {
      index = zisc::cast<uint32b>(i);
      is_found = true;
    }
  }

  return index;
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
//
///*!
//  */
//inline
//void VulkanDevice::initDebugMessenger() noexcept
//{
//  auto createDebugUtilsMessengerEXT =
//      reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
//          instance_.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
//
//  VkDebugUtilsMessengerCreateInfoEXT create_info;
//  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//  create_info.pNext = nullptr;
//  create_info.flags = 0;
//  create_info.messageSeverity =
////      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
////      VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
//      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
//      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//  create_info.messageType =
//      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
//      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
//      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//  create_info.pfnUserCallback = &VulkanDevice::debugMessengerCallback;
//  create_info.pUserData = nullptr;
//
//  vk::DebugUtilsMessengerEXT debug_messenger;
//  createDebugUtilsMessengerEXT(
//      zisc::cast<VkInstance>(instance_),
//      &create_info,
//      nullptr,
//      reinterpret_cast<VkDebugUtilsMessengerEXT*>(&debug_messenger));
//  debug_messenger_ = debug_messenger;
//}
//
///*!
//  */
//inline
//void VulkanDevice::initDevice(const DeviceOptions& options) noexcept
//{
//  std::vector<const char*> layers{};
//  const std::vector<const char*> extensions{{
//      VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
//      VK_KHR_8BIT_STORAGE_EXTENSION_NAME,
//      VK_KHR_16BIT_STORAGE_EXTENSION_NAME,
//      VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME,
//      VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME,
//      VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME}};
//
//  if (options.enable_debug_) {
//    layers.emplace_back("VK_LAYER_KHRONOS_validation");
//  }
//
//  const auto& info = physicalDeviceInfo();
//  vk::PhysicalDeviceFeatures device_features;
//  {
//    const auto& features = info.features().features1_;
//    device_features.shaderFloat64 = features.shaderFloat64;
//    device_features.shaderInt64 = features.shaderInt64;
//    device_features.shaderInt16 = features.shaderInt16;
//  }
//
//  std::vector<std::vector<float>> priority_list;
//  priority_list.reserve(queue_family_index_list_.size());
//  std::vector<vk::DeviceQueueCreateInfo> queue_create_info_list;
//  queue_create_info_list.reserve(queue_family_index_list_.size());
//  for (auto family_index : queue_family_index_list_) {
//    const auto& family_info_list = info.queueFamilyPropertiesList();
//    const auto& family_info = family_info_list[family_index].properties1_;
//    priority_list.emplace_back();
//    priority_list.back().resize(family_info.queueCount, 0.0f);
//    queue_create_info_list.emplace_back(vk::DeviceQueueCreateFlags{},
//                                        family_index,
//                                        family_info.queueCount,
//                                        priority_list.back().data());
//  }
//
//  vk::DeviceCreateInfo device_create_info{
//      vk::DeviceCreateFlags{},
//      zisc::cast<uint32b>(queue_create_info_list.size()),
//      queue_create_info_list.data(),
//      zisc::cast<uint32b>(layers.size()),
//      layers.data(),
//      zisc::cast<uint32b>(extensions.size()),
//      extensions.data(),
//      &device_features};
//
//  // features2
//  auto b16bit_storage_feature = info.features().b16bit_storage_;
//  auto b8bit_storage_feature = info.features().b8bit_storage_;
//  auto float16_int8_feature = info.features().float16_int8_;
//  auto variable_pointers_feature = info.features().variable_pointers_;
//  VulkanPhysicalDeviceInfo::link(device_create_info,
//                                 b16bit_storage_feature,
//                                 b8bit_storage_feature,
//                                 float16_int8_feature,
//                                 variable_pointers_feature);
//
//  auto [result, device] = physical_device_.createDevice(device_create_info);
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Vulkan device creation failed.");
//  device_ = device;
//}

///*!
//  */
//inline
//void VulkanDevice::initMemoryAllocator() noexcept
//{
//  VmaAllocatorCreateInfo allocator_create_info{};
//  allocator_create_info.flags = 0;
//  allocator_create_info.physicalDevice = physical_device_;
//  allocator_create_info.device = device_;
//  allocator_create_info.preferredLargeHeapBlockSize = 0;
//  allocator_create_info.pAllocationCallbacks = nullptr;
//  allocator_create_info.pDeviceMemoryCallbacks = nullptr;
//  allocator_create_info.frameInUseCount = 0;
//  allocator_create_info.pHeapSizeLimit = nullptr;
//  allocator_create_info.pVulkanFunctions = nullptr;
//  allocator_create_info.pRecordSettings = nullptr;
//
//  auto result = vmaCreateAllocator(&allocator_create_info, &allocator_);
//  ZISC_ASSERT(result == VK_SUCCESS, "Memory allocator creation failed.");
//  (void)result;
//}

///*!
//  */
//inline
//void VulkanDevice::initPhysicalDevice(const DeviceOptions& options) noexcept
//{
//  auto [result, physical_device_list] = instance_.enumeratePhysicalDevices();
//  ZISC_ASSERT(result == vk::Result::eSuccess,
//              "Vulkan physical device enumeration failed.");
//  ZISC_ASSERT(options.vulkan_device_number_ < physical_device_list.size(),
//              "The specified device doesn't exist.");
//  physical_device_ = physical_device_list[options.vulkan_device_number_];
//  device_info_.fetch(physical_device_);
//}

/*!
  \details No detailed description
  */
void VulkanDevice::initialize() noexcept
{
  initLocalWorkGroupSize();
  initQueueFamilyIndexList();
//  initDevice(options);
//  initCommandPool();
//  initMemoryAllocator();
}

/*!
  \details No detailed description
  */
void VulkanDevice::initLocalWorkGroupSize() noexcept
{
  const auto& info = vulkanDeviceInfo();
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
  queue_family_index_ = findQueueFamily();
}

///*!
//  */
//inline
//uint32b VulkanDevice::queueFamilyIndex(const QueueType queue_type) const noexcept
//{
//  const std::size_t list_index = zisc::cast<std::size_t>(queue_type);
//  const std::size_t ref_index = queue_family_index_ref_list_[list_index];
//  const uint32b family_index = queue_family_index_list_[ref_index];
//  return family_index;
//}

} // namespace zinvul
