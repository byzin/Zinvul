/*!
  \file vulkan_device-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_DEVICE_INL_HPP
#define ZINVUL_VULKAN_DEVICE_INL_HPP

#include "vulkan_device.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
// Zisc
#include "zisc/error.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_buffer.hpp"
#include "vulkan_kernel.hpp"
#include "zinvul/buffer.hpp"
#include "zinvul/device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
inline
VulkanDevice::VulkanDevice(DeviceOptions& options) noexcept :
    Device(options),
    shader_module_list_{memoryResource()}
{
  initialize(options);
}

/*!
  */
inline
VulkanDevice::~VulkanDevice() noexcept
{
  destroy();
}

/*!
  */
template <typename Type> inline
void VulkanDevice::allocate(const std::size_t size,
                            VulkanBuffer<Type>* buffer) noexcept
{
  ZISC_ASSERT(buffer != nullptr, "The buffer is null.");
  auto& b = buffer->buffer();
  auto& memory = buffer->memory();
  auto& alloc_info = buffer->allocationInfo();

  vk::BufferCreateInfo buffer_create_info;
  buffer_create_info.size = sizeof(Type) * size;
  buffer_create_info.usage = vk::BufferUsageFlagBits::eStorageBuffer;
  buffer_create_info.queueFamilyIndexCount = 1;
  buffer_create_info.pQueueFamilyIndices = &queue_family_index_;

  VmaAllocationCreateInfo alloc_create_info;
  alloc_create_info.flags = 0;
  alloc_create_info.usage = buffer->isHostReadable() ? VMA_MEMORY_USAGE_GPU_TO_CPU :
                            buffer->isHostWritable() ? VMA_MEMORY_USAGE_CPU_TO_GPU :
                                                       VMA_MEMORY_USAGE_GPU_ONLY;
  alloc_create_info.requiredFlags = 0;
  alloc_create_info.preferredFlags = 0;
  alloc_create_info.memoryTypeBits = 0;
  alloc_create_info.pool = VK_NULL_HANDLE;
  alloc_create_info.pUserData = nullptr;

  const auto result = vmaCreateBuffer(
      allocator_,
      &zisc::cast<const VkBufferCreateInfo&>(buffer_create_info),
      &alloc_create_info,
      reinterpret_cast<VkBuffer*>(&b),
      &memory,
      &alloc_info);
  ZISC_ASSERT(result == VK_SUCCESS, "Buffer creation failed.");

  const std::size_t device_memory_usage = memoryUsage() + buffer->memoryUsage();
  setMemoryUsage(device_memory_usage);
}

/*!
  */
inline
vk::CommandPool& VulkanDevice::commandPool() noexcept
{
  return command_pool_;
}

/*!
  */
inline
const vk::CommandPool& VulkanDevice::commandPool() const noexcept
{
  return command_pool_;
}

/*!
  */
template <typename Type> inline
void VulkanDevice::deallocate(VulkanBuffer<Type>* buffer) noexcept
{
  ZISC_ASSERT(buffer != nullptr, "The buffer is null.");
  auto& b = buffer->buffer();
  auto& memory = buffer->memory();
  auto& alloc_info = buffer->allocationInfo();
  if (b) {
    vmaDestroyBuffer(allocator_, *reinterpret_cast<VkBuffer*>(&b), memory);

    const std::size_t device_memory_usage = memoryUsage() - buffer->memoryUsage();
    setMemoryUsage(device_memory_usage);

    b = nullptr;
    memory = VK_NULL_HANDLE;
    alloc_info.size = 0;
  }
}

/*!
  */
inline
void VulkanDevice::destroy() noexcept
{
  if (device_) {
    for (auto& module : shader_module_list_) {
      if (module) {
        device_.destroyShaderModule(module);
        module = nullptr;
      }
    }
    if (allocator_)  {
      vmaDestroyAllocator(allocator_);
      allocator_ = VK_NULL_HANDLE;
    }
    if (command_pool_) {
      device_.destroyCommandPool(command_pool_);
      command_pool_ = nullptr;
    }
    device_.destroy();
    device_ = nullptr;
  }

  if (debug_messenger_) {
    auto destroyDebugUtilsMessengerEXT =
        reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            instance_.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
    destroyDebugUtilsMessengerEXT(
        zisc::cast<VkInstance>(instance_),
        zisc::cast<VkDebugUtilsMessengerEXT>(debug_messenger_),
        nullptr);
    debug_messenger_ = nullptr;
  }

  if (instance_) {
    instance_.destroy();
    instance_ = nullptr;
  }
}

/*!
  */
inline
vk::Device& VulkanDevice::device() noexcept
{
  return device_;
}

/*!
  */
inline
const vk::Device& VulkanDevice::device() const noexcept
{
  return device_;
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
const vk::ShaderModule& VulkanDevice::getShaderModule(
    const std::size_t index) const noexcept
{
  ZISC_ASSERT(hasShaderModule(index), "The shader module doesn't exist.");
  return shader_module_list_[index];
}

/*!
  */
inline
std::string VulkanDevice::getVendorName(const uint32b vendor_id) noexcept
{
  using namespace std::string_literals;
  std::string vendor_name;
  switch (vendor_id) {
   case zisc::cast<uint32b>(VendorId::kAmd): {
    vendor_name = "AMD"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kImgTec): {
    vendor_name = "ImgTec"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kNvidia): {
    vendor_name = "NVIDIA"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kArm): {
    vendor_name = "ARM"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kQualcomm): {
    vendor_name = "Qualcomm"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kIntel): {
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
std::array<uint32b, 3> VulkanDevice::getWorkgroupSize(const uint32b dimension)
    const noexcept
{
  ZISC_ASSERT(zisc::isInBounds(dimension, 1u, 4u),
              "The dimension should be 1, 2 or 3.");
  const auto subgroup_size = physicalDeviceInfo().subgroup_properties_.subgroupSize;
  std::array<uint32b, 3> workgroup_size{{1, 1, 1}};

  const auto product = [](const std::array<uint32b, 3>& s)
  {
    return std::accumulate(s.begin(), s.end(), 1u, std::multiplies<uint32b>());
  };
  for (uint32b i = 0;
       product(workgroup_size) < subgroup_size;
       i = ((i + 1) == dimension) ? 0 : i + 1 ) {
    workgroup_size[i] *= 2;
  }
  ZISC_ASSERT(product(workgroup_size) == subgroup_size,
              "The subgroup size should be power of 2: ", subgroup_size);
  return workgroup_size;
}

/*!
  */
inline
bool VulkanDevice::hasShaderModule(const std::size_t index) const noexcept
{
  const bool flag = (index < shader_module_list_.size()) &&
                    shader_module_list_[index];
  return flag;
}

/*!
  */
template <typename Type> inline
UniqueBuffer<Type> VulkanDevice::makeBuffer(const BufferUsage usage_flag) noexcept
{
  using UniqueVulkanBuffer = zisc::UniqueMemoryPointer<VulkanBuffer<Type>>;
  UniqueBuffer<Type> buffer =
      UniqueVulkanBuffer::make(memoryResource(), this, usage_flag);
  return buffer;
}

/*!
  */
template <typename GroupType, std::size_t kDimension, typename ...ArgumentTypes>
inline
UniqueKernel<GroupType, kDimension, ArgumentTypes...> VulkanDevice::makeKernel(
    const uint32b module_index,
    const char* kernel_name) noexcept
{
  using UniqueVulkanKernel = zisc::UniqueMemoryPointer<VulkanKernel<
      GroupType,
      kDimension,
      ArgumentTypes...>>;
  UniqueKernel<GroupType, kDimension, ArgumentTypes...> kernel =
      UniqueVulkanKernel::make(memoryResource(), this, module_index, kernel_name);
  return kernel;
}

/*!
  */
template <typename Type> inline
void* VulkanDevice::mapMemory(const VulkanBuffer<Type>& buffer) const noexcept
{
  void* data = nullptr;
  const auto result = vmaMapMemory(allocator_, buffer.memory(), &data);
  ZISC_ASSERT(result == VK_SUCCESS, "Buffer memory map failed.");
  return data;
}

/*!
  */
inline
auto VulkanDevice::physicalDeviceInfo() const noexcept -> const PhysicalDeviceInfo&
{
  return device_info_;
}

/*!
  */
inline
void VulkanDevice::setShaderModule(const zisc::pmr::vector<uint32b>& spirv_code,
                                   const std::size_t index) noexcept
{
  if (shader_module_list_.size() <= index)
    shader_module_list_.resize(index + 1);
  else if (hasShaderModule(index))
    device_.destroyShaderModule(getShaderModule(index));

  const vk::ShaderModuleCreateInfo create_info{vk::ShaderModuleCreateFlags{},
                                               4 * spirv_code.size(),
                                               spirv_code.data()};
  auto [result, shader_module] = device_.createShaderModule(create_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Shader module creation failed.");
  shader_module_list_[index] = shader_module;
}

/*!
  */
inline
void VulkanDevice::submit(const vk::CommandBuffer& command) noexcept
{
  const vk::SubmitInfo info{0, nullptr, nullptr, 1, &command};
  auto q = device_.getQueue(queue_family_index_, 0);
  const auto result = q.submit(1, &info, nullptr);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Command submission failed.");
}

/*!
  */
template <typename Type> inline
void VulkanDevice::unmapMemory(const VulkanBuffer<Type>& buffer) const noexcept
{
  vmaUnmapMemory(allocator_, buffer.memory());
}

/*!
  */
inline
void VulkanDevice::waitForCompletion() noexcept
{
  auto q = device_.getQueue(queue_family_index_, 0);
  q.waitIdle();
}

/*!
  */
inline
VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDevice::debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* /* user_data */)
{
  constexpr std::size_t max_add_message_length = 1024;
  bool is_error = false;
  std::string message;

  {
    char prefix[64];
    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
      std::strcpy(prefix, "VERBOSE : ");
    }
    else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
      std::strcpy(prefix, "INFO : ");
    }
    else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      std::strcpy(prefix, "WARNING : ");
    }
    else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
      std::strcpy(prefix, "ERROR : ");
      is_error = true;
    }

    if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
      std::strcat(prefix, "GENERAL");
    else if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
      std::strcat(prefix, "VALIDATION");
    else if (message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
      std::strcat(prefix, "PERF");

    char tmp_message[max_add_message_length];
    std::sprintf(tmp_message, "%s - Message ID Number %d, Message ID Name %s :\n%s",
        prefix,
        callback_data->messageIdNumber,
        callback_data->pMessageIdName,
        callback_data->pMessage);
    message += tmp_message;
  }

  if (0 < callback_data->objectCount) {
    char tmp_message[max_add_message_length];
    std::sprintf(tmp_message, "\n Objects - %d\n", callback_data->objectCount);
    for (std::size_t object = 0; object < callback_data->objectCount; ++object) {
      std::sprintf(tmp_message, "%s Object[%d] - Type %s, Value %p, Name \"%s\"\n",
          tmp_message,
          zisc::cast<int>(object),
          vk::to_string(zisc::cast<vk::ObjectType>(callback_data->pObjects[object].objectType)).data(),
          reinterpret_cast<void*>(callback_data->pObjects[object].objectHandle),
          callback_data->pObjects[object].pObjectName);
    }
    message += tmp_message;
  }

  if (0 < callback_data->cmdBufLabelCount) {
    char tmp_message[max_add_message_length];
    std::sprintf(tmp_message, "\n Command Buffer Labels - %d\n",
        callback_data->cmdBufLabelCount);
    for (std::size_t label = 0; label < callback_data->cmdBufLabelCount; ++label) {
      std::sprintf(tmp_message, "%s Label[%d] - %s { %lf, %lf, %lf, %lf}\n",
          tmp_message,
          zisc::cast<int>(label),
          callback_data->pCmdBufLabels[label].pLabelName,
          zisc::cast<double>(callback_data->pCmdBufLabels[label].color[0]),
          zisc::cast<double>(callback_data->pCmdBufLabels[label].color[1]),
          zisc::cast<double>(callback_data->pCmdBufLabels[label].color[2]),
          zisc::cast<double>(callback_data->pCmdBufLabels[label].color[3]));
    }
    message += tmp_message;
  }

  std::ostream* output = (is_error) ? &std::cerr : &std::cout;
  (*output) << message << std::endl;

  return false;
}

/*!
  */
inline
uint32b VulkanDevice::findQueueFamilyForShader() const noexcept
{
  const auto& queue_family_list = device_info_.queue_family_list_;

  uint32b index = std::numeric_limits<uint32b>::max();
  bool is_found = false;

  auto hasFlag = [](const vk::QueueFamilyProperties& family,
                    const vk::QueueFlagBits& flag)
  {
    return (family.queueFlags & flag) == flag;
  };

  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
    const auto& family = queue_family_list[i];
    if (!hasFlag(family, vk::QueueFlagBits::eGraphics) &&
        hasFlag(family, vk::QueueFlagBits::eCompute)) {
      index = zisc::cast<uint32b>(i);
      is_found = true;
    }
  }

  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
    const auto& family = queue_family_list[i];
    if (hasFlag(family, vk::QueueFlagBits::eCompute)) {
      index = zisc::cast<uint32b>(i);
      is_found = true;
    }
  }

  return index;
}

/*!
  */
inline
auto VulkanDevice::getPhysicalDeviceInfo(const vk::PhysicalDevice& device) noexcept
    -> PhysicalDeviceInfo
{
  PhysicalDeviceInfo device_info;
  // Queue properties
  device_info.queue_family_list_ = device.getQueueFamilyProperties();
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
void VulkanDevice::initCommandPool() noexcept
{
  const vk::CommandPoolCreateInfo pool_info{
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      queue_family_index_};
  auto [result, command_pool] = device_.createCommandPool(pool_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Command pool creation failed.");
  command_pool_ = command_pool;
}

/*!
  */
inline
void VulkanDevice::initDebugMessenger() noexcept
{
  auto createDebugUtilsMessengerEXT =
      reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
          instance_.getProcAddr("vkCreateDebugUtilsMessengerEXT"));

  VkDebugUtilsMessengerCreateInfoEXT create_info;
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.pNext = nullptr;
  create_info.flags = 0;
  create_info.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = &VulkanDevice::debugMessengerCallback;
  create_info.pUserData = nullptr;

  vk::DebugUtilsMessengerEXT debug_messenger;
  createDebugUtilsMessengerEXT(
      zisc::cast<VkInstance>(instance_),
      &create_info,
      nullptr,
      reinterpret_cast<VkDebugUtilsMessengerEXT*>(&debug_messenger));
  debug_messenger_ = debug_messenger;
}

/*!
  */
inline
void VulkanDevice::initDevice() noexcept
{
  const std::array<const char*, 1> layers{{
    "VK_LAYER_LUNARG_standard_validation"
    }};
  const std::array<const char*, 4> extensions{{
      VK_KHR_8BIT_STORAGE_EXTENSION_NAME,
      VK_KHR_16BIT_STORAGE_EXTENSION_NAME,
      VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME,
      VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME}};

  const auto& device_info = physicalDeviceInfo();
  vk::PhysicalDeviceFeatures device_features;
  device_features.shaderFloat64 = device_info.features_.shaderFloat64;
  device_features.shaderInt64 = device_info.features_.shaderInt64;
  device_features.shaderInt16 = device_info.features_.shaderInt16;

  const float priority = 0.0f;
  const vk::DeviceQueueCreateInfo queue_create_info{vk::DeviceQueueCreateFlags{},
                                                    queue_family_index_,
                                                    1,
                                                    &priority};
  const vk::DeviceCreateInfo device_create_info{
      vk::DeviceCreateFlags{},
      1,
      &queue_create_info,
      zisc::cast<uint32b>(layers.size()),
      layers.data(),
      zisc::cast<uint32b>(extensions.size()),
      extensions.data(),
      &device_features};
  auto [result, device] = physical_device_.createDevice(device_create_info);
  ZISC_ASSERT(result == vk::Result::eSuccess, "Vulkan device creation failed.");
  device_ = device;
}

/*!
  */
inline
void VulkanDevice::initMemoryAllocator() noexcept
{
  VmaAllocatorCreateInfo allocator_create_info{};
  allocator_create_info.flags = 0;
  allocator_create_info.physicalDevice = physical_device_;
  allocator_create_info.device = device_;
  allocator_create_info.preferredLargeHeapBlockSize = 0;
  allocator_create_info.pAllocationCallbacks = nullptr;
  allocator_create_info.pDeviceMemoryCallbacks = nullptr;
  allocator_create_info.frameInUseCount = 0;
  allocator_create_info.pHeapSizeLimit = nullptr;
  allocator_create_info.pVulkanFunctions = nullptr;
  allocator_create_info.pRecordSettings = nullptr;

  auto result = vmaCreateAllocator(&allocator_create_info, &allocator_);
  ZISC_ASSERT(result == VK_SUCCESS, "Memory allocator creation failed.");

  VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                                 nullptr,
                                 0,
                                 sizeof(void*),
                                 VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                 VK_SHARING_MODE_EXCLUSIVE,
                                 1,
                                 &queue_family_index_};

  VmaAllocationCreateInfo alloc_info{0,
                                     VMA_MEMORY_USAGE_GPU_ONLY,
                                     0,
                                     0,
                                     0,
                                     VK_NULL_HANDLE,
                                     nullptr};

  result = vmaFindMemoryTypeIndexForBufferInfo(allocator_,
                                               &buffer_info,
                                               &alloc_info, 
                                               &memory_type_index_);
  ZISC_ASSERT(result == VK_SUCCESS, "Finding buffer info failed.");
}

/*!
  */
inline
void VulkanDevice::initialize(const DeviceOptions& options) noexcept
{
  app_info_ = makeApplicationInfo(options.app_name_,
                                  options.app_version_major_,
                                  options.app_version_minor_,
                                  options.app_version_patch_);
  instance_ = makeInstance(app_info_);
  ZISC_ASSERT(instance_, "Vulkan instance creation failed.");
#if Z_DEBUG_MODE
  initDebugMessenger();
#endif // Z_DEBUG_MODE
  initPhysicalDevice(options);
  queue_family_index_ = findQueueFamilyForShader();
  initDevice();
  initCommandPool();
  initMemoryAllocator();
}

/*!
  */
inline
void VulkanDevice::initPhysicalDevice(const DeviceOptions& options) noexcept
{
  auto [result, physical_device_list] = instance_.enumeratePhysicalDevices();
  ZISC_ASSERT(result == vk::Result::eSuccess,
              "Vulkan physical device enumeration failed.");
  ZISC_ASSERT(options.device_number_ < physical_device_list.size(),
              "The specified device doesn't exist.");
  physical_device_ = physical_device_list[options.device_number_];
  device_info_ = getPhysicalDeviceInfo(physical_device_);
}

/*!
  */
inline
vk::Instance VulkanDevice::makeInstance(const vk::ApplicationInfo& app_info)
    noexcept
{
  const std::array<const char*, 1> layers{{
    "VK_LAYER_LUNARG_standard_validation"
    }};
  const std::array<const char*, 2> extensions{{
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
  }};

  const vk::InstanceCreateInfo createInfo{vk::InstanceCreateFlags{},
                                          &app_info,
                                          zisc::cast<uint32b>(layers.size()),
                                          layers.data(),
                                          zisc::cast<uint32b>(extensions.size()),
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
  static constexpr char engine_name[] = "Zinvul";
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
