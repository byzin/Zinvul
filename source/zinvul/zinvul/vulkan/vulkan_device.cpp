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
#include <array>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
//#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_device_info.hpp"
//#include "vulkan_buffer.hpp"
//#include "vulkan_kernel.hpp"
//#include "zinvul/buffer.hpp"
//#include "zinvul/device.hpp"
//#include "zinvul/kernel.hpp"
#include "zinvul/device.hpp"
#include "zinvul/device_options.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] options No description.
  */
VulkanDevice::VulkanDevice(DeviceOptions& options) noexcept :
    Device(options),
    id_count_{0}
//    device_info_{memoryResource()},
//    shader_module_list_{memoryResource()},
//    command_pool_list_{memoryResource()},
//    queue_family_index_list_{memoryResource()},
//    queue_family_index_ref_list_{{std::numeric_limits<uint32b>::max(),
//                                  std::numeric_limits<uint32b>::max()}}
{
  initialize(options);
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
//  destroy();
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
//
///*!
//  */
//inline
//void VulkanDevice::destroy() noexcept
//{
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
//}
//
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
SubPlatformType VulkanDevice::subPlatformType() const noexcept
{
  return SubPlatformType::kVulkan;
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  \return No description
  */
zisc::pmr::vector<VulkanDeviceInfo> VulkanDevice::getDeviceInfoList(
    zisc::pmr::memory_resource* mem_resource)
{
  const auto app_info = makeApplicationInfo("getDeviceInfoList",
                                            Config::versionMajor(),
                                            Config::versionMinor(),
                                            Config::versionPatch());
  constexpr bool enable_debug =
#if defined(Z_DEBUG_MODE)
    true;
#else // Z_DEBUG_MODE
    false;
#endif
  auto alloc = makeAllocator(mem_resource);
  auto instance = makeInstance(app_info, enable_debug, &alloc);

  using DeviceInfoList = zisc::pmr::vector<VulkanDeviceInfo>;
  DeviceInfoList device_info_list{DeviceInfoList::allocator_type{mem_resource}};

  if (instance) {
//    using ListAllocator = zisc::pmr::vector<vk::PhysicalDevice>::allocator_type;
//    ListAllocator list_alloc{mem_resource};
//    VULKAN_HPP_DEFAULT_DISPATCHER_TYPE dispatch{};
//    auto device_list = instance.enumeratePhysicalDevices(list_alloc, dispatch);
//    device_info_list.reserve(device_list.size());
//    for (const auto& device : device_list) {
//      device_info_list.emplace_back(mem_resource);
//      device_info_list.back().fetch(device);
//    }
  }
  instance.destroy(alloc);
  return device_info_list;
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
//
///*!
//  */
//inline
//std::string VulkanDevice::getVendorName(const uint32b id) noexcept
//{
//  using namespace std::string_literals;
//  std::string vendor_name;
//  switch (id) {
//   case zisc::cast<uint32b>(VendorId::kAmd): {
//    vendor_name = "AMD"s;
//    break;
//   }
//   case zisc::cast<uint32b>(VendorId::kImgTec): {
//    vendor_name = "ImgTec"s;
//    break;
//   }
//   case zisc::cast<uint32b>(VendorId::kNvidia): {
//    vendor_name = "NVIDIA"s;
//    break;
//   }
//   case zisc::cast<uint32b>(VendorId::kArm): {
//    vendor_name = "ARM"s;
//    break;
//   }
//   case zisc::cast<uint32b>(VendorId::kQualcomm): {
//    vendor_name = "Qualcomm"s;
//    break;
//   }
//   case zisc::cast<uint32b>(VendorId::kIntel): {
//    vendor_name = "INTEL"s;
//    break;
//   }
//   default: {
//    vendor_name = "N/A"s;
//    break;
//   }
//  }
//  return vendor_name;
//}
//
///*!
//  */
//inline
//uint32b VulkanDevice::getVendorSubgroupSize(const uint32b id) noexcept
//{
//  // Common subgroup size: 32 (NVidia, Intel), 64 (AMD)
//  uint32b subgroup_size = 1;
//  switch (id) {
//   case zisc::cast<uint32b>(VendorId::kAmd): {
//    subgroup_size = 64;
//    break;
//   }
//   case zisc::cast<uint32b>(VendorId::kNvidia):
//   case zisc::cast<uint32b>(VendorId::kIntel): {
//    subgroup_size = 32;
//    break;
//   }
//   default: {
//    break;
//   }
//  }
//  return subgroup_size;
//}
//
///*!
//  */
//inline
//bool VulkanDevice::hasShaderModule(const std::size_t index) const noexcept
//{
//  const bool flag = (index < shader_module_list_.size()) &&
//                    shader_module_list_[index];
//  return flag;
//}
//
///*!
//  */
//inline
//void VulkanDevice::initLocalWorkSize(const uint32b subgroup_size) noexcept
//{
//  for (uint32b dim = 1; dim <= local_work_size_list_.size(); ++dim) {
//    std::array<uint32b, 3> local_work_size{{1, 1, 1}};
//    const auto product = [](const std::array<uint32b, 3>& s)
//    {
//      return std::accumulate(s.begin(), s.end(), 1u, std::multiplies<uint32b>());
//    };
//    for (uint32b i = 0; product(local_work_size) < subgroup_size; i = (i + 1) % dim)
//      local_work_size[i] *= 2;
//    ZISC_ASSERT(product(local_work_size) == subgroup_size,
//                "The subgroup size should be power of 2: ", subgroup_size);
//    local_work_size_list_[dim - 1] = local_work_size;
//  }
//}

/*!
  \details No detailed description

  \return No description
  */
IdData VulkanDevice::issueId() noexcept
{
  const uint32b id = id_count_++;
  IdData id_data{id};
  return id_data;
}

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

/*!
  \details No detailed description

  \return No description
  */
std::string_view VulkanDevice::name() const noexcept
{
//  const auto& device_info = physicalDeviceInfo();
//  std::string_view device_name{device_info.properties().properties1_.deviceName};
//  return device_name;
  return std::string_view{""};
}

///*!
//  */
//inline
//auto VulkanDevice::physicalDeviceInfo() const noexcept
//    -> const VulkanPhysicalDeviceInfo&
//{
//  return device_info_;
//}
//
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

/*!
  \details No detailed description

  \return No description
  */
uint32b VulkanDevice::subgroupSize() const noexcept
{
//  return local_work_size_list_[0][0];
  return 0;
}

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

/*!
  \details No detailed description

  \return No description
  */
std::string_view VulkanDevice::vendorName() const noexcept
{
//  std::string_view vendor_name{vendor_name_};
//  return vendor_name;
  return std::string_view{""};
}

/*!
  \details No detailed description
  */
void VulkanDevice::waitForCompletion() const noexcept
{
//  const auto result = device_.waitIdle();
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Device waiting failed.");
//  (void)result;
}

/*!
  \details No detailed description

  \param [in] queue_type No description.
  */
void VulkanDevice::waitForCompletion(const QueueType queue_type) const noexcept
{
//  const uint32b family_index = queueFamilyIndex(queue_type);
//  const auto& info = physicalDeviceInfo();
//  const auto& family_info_list = info.queueFamilyPropertiesList();
//  const auto& family_info = family_info_list[family_index].properties1_;
//  for (uint32b i = 0; i < family_info.queueCount; ++i)
//    waitForCompletion(queue_type, i);
}

/*!
  \details No detailed description

  \param [in] queue_type No description.
  \param [in] queue_index No description.
  */
void VulkanDevice::waitForCompletion(const QueueType queue_type,
                                     const uint32b queue_index) const noexcept
{
//  auto q = getQueue(queue_type, queue_index);
//  const auto result = q.waitIdle();
//  ZISC_ASSERT(result == vk::Result::eSuccess, "Queue waiting failed.");
//  (void)result;
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in] size No description.
  \param [in] alignment No description.
  \param [in] scope No description.
  \return No description
  */
auto VulkanDevice::Callbacks::allocateMemory(
    void* user_data,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope /* scope */) -> AllocationReturnType
{
  ZISC_ASSERT(user_data != nullptr, "The user data is null.");
  auto mem_resource = zisc::cast<zisc::pmr::memory_resource*>(user_data);
  void* memory = mem_resource->allocate(size, alignment);
  return memory;
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in,out] memory No description.
  */
void VulkanDevice::Callbacks::freeMemory(
    void* user_data,
    void* memory)
{
  ZISC_ASSERT(user_data != nullptr, "The user data is null.");
  auto mem_resource = zisc::cast<zisc::pmr::memory_resource*>(user_data);
  if (memory)
    mem_resource->deallocate(memory, 0, 0);
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in] size No description.
  \param [in] type No description.
  \param [in] scope No description.
  */
void VulkanDevice::Callbacks::notifyOfMemoryAllocation(
    void* /* user_data */,
    size_t /* size */,
    VkInternalAllocationType /* type */,
    VkSystemAllocationScope /* scope */)
{
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in] size No description.
  \param [in] type No description.
  \param [in] scope No description.
  */
void VulkanDevice::Callbacks::notifyOfMemoryFreeing(
    void* /* user_data */,
    size_t /* size */,
    VkInternalAllocationType /* type */,
    VkSystemAllocationScope /* scope */)
{
}

/*!
  \details No detailed description

  \param [in] severity_flags No description.
  \param [in] message_types No description.
  \param [in] callback_data No description.
  \param [in] user_data No description.
  \return No description
  */
auto VulkanDevice::Callbacks::printDebugMessage(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity_flags,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data) -> DebugMessengerReturnType
{
  constexpr std::size_t max_add_message_length = 2048;
  bool is_error = false;
  std::string message;

  if (user_data) {
    char tmp[max_add_message_length];
    const IdData* data = zisc::cast<const IdData*>(user_data);
    std::sprintf(tmp, "ID[%u] -", data->id());
    if (data->hasName()) {
      std::sprintf(tmp, "%s Name '%s'", tmp, data->name().data());
    }
    if (data->hasFileInfo()) {
      const std::string_view file_name = data->fileName();
      const uint32b line = data->lineNumber();
      std::sprintf(tmp, "%s at line %u in '%s'", tmp, line, file_name.data());
    }
    std::sprintf(tmp, "%s\n", tmp);
    message += tmp;
  }

  {
    char prefix[64] = "";
    if (severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
      std::strcat(prefix, "VERBOSE : ");
    }
    else if (severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
      std::strcat(prefix, "INFO    : ");
    }
    else if (severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      std::strcat(prefix, "WARNING : ");
    }
    else if (severity_flags & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
      std::strcat(prefix, "ERROR   : ");
      is_error = true;
    }

    if (message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
      std::strcat(prefix, "GENERAL");
    }
    else if (message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
      std::strcat(prefix, "VALIDATION");
    }
    else if (message_types & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
      std::strcat(prefix, "PERF");
    }

    char tmp[max_add_message_length];
    std::sprintf(tmp, "%s - Message ID Number %d, Message ID Name %s :\n%s",
        prefix,
        callback_data->messageIdNumber,
        callback_data->pMessageIdName,
        callback_data->pMessage);
    message += tmp;
  }
  const char indent[] = "          ";

  if (0 < callback_data->queueLabelCount) {
    char tmp[max_add_message_length];
    std::sprintf(tmp, "\n%sQueue Labels - %d\n",
        indent,
        zisc::cast<int>(callback_data->queueLabelCount));
    for (std::size_t id = 0; id < callback_data->queueLabelCount; ++id) {
      const auto& label = callback_data->pQueueLabels[id];
      std::sprintf(tmp, "%s%s  * Label[%d] - %s {%lf, %lf, %lf, %lf}\n",
          tmp,
          indent,
          zisc::cast<int>(id),
          label.pLabelName,
          zisc::cast<double>(label.color[0]),
          zisc::cast<double>(label.color[1]),
          zisc::cast<double>(label.color[2]),
          zisc::cast<double>(label.color[3]));
    }
    message += tmp;
  }

  if (0 < callback_data->cmdBufLabelCount) {
    char tmp[max_add_message_length];
    std::sprintf(tmp, "\n%sCommand Buffer Labels - %d\n",
        indent,
        zisc::cast<int>(callback_data->cmdBufLabelCount));
    for (std::size_t id = 0; id < callback_data->cmdBufLabelCount; ++id) {
      const auto& label = callback_data->pCmdBufLabels[id];
      std::sprintf(tmp, "%s%s  * Label[%d] - %s {%lf, %lf, %lf, %lf}\n",
          tmp,
          indent,
          zisc::cast<int>(id),
          label.pLabelName,
          zisc::cast<double>(label.color[0]),
          zisc::cast<double>(label.color[1]),
          zisc::cast<double>(label.color[2]),
          zisc::cast<double>(label.color[3]));
    }
    message += tmp;
  }

  if (0 < callback_data->objectCount) {
    char tmp[max_add_message_length];
    std::sprintf(tmp, "\n%sObjects - %d\n",
        indent,
        zisc::cast<int>(callback_data->objectCount));
    for (std::size_t id = 0; id < callback_data->objectCount; ++id) {
      const auto& object = callback_data->pObjects[id];
      const auto obj_type_name = vk::to_string(
          zisc::cast<vk::ObjectType>(object.objectType));
      std::sprintf(tmp, "%s%s  * Object[%d] - Type '%s', Value %lu, Name '%s'\n",
          tmp,
          indent,
          zisc::cast<int>(id),
          obj_type_name.c_str(),
          object.objectHandle,
          object.pObjectName);
    }
    message += tmp;
  }

  message += '\n';
  std::ostream* output = (is_error) ? &std::cerr : &std::cout;
  (*output) << message;

  return VK_FALSE;
}

/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in,out] original_memory No description.
  \param [in] size No description.
  \param [in] alignment No description.
  \param [in] scope No description.
  \return No description
  */
auto VulkanDevice::Callbacks::reallocateMemory(
    void* user_data,
    void* original_memory,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope /* scope */) -> ReallocationReturnType
{
  ZISC_ASSERT(user_data != nullptr, "The user data is null.");
  auto mem_resource = zisc::cast<zisc::pmr::memory_resource*>(user_data);
  // Allocate new memory block
  void* memory = nullptr;
  if (0 < size)
    memory = mem_resource->allocate(size, alignment);
  // Copy data
  if (original_memory && memory)
    std::cerr << "Warning: copying data in the reallocation isn't operated.";
  // Deallocate the original memory
  if (original_memory)
    mem_resource->deallocate(original_memory, 0, 0);
  return memory;
}

///*!
//  */
//inline
//uint32b VulkanDevice::findQueueFamily(const QueueType queue_type) const noexcept
//{
//  const auto& queue_family_list = physicalDeviceInfo().queueFamilyPropertiesList();
//
//  uint32b index = std::numeric_limits<uint32b>::max();
//  bool is_found = false;
//
//  const vk::QueueFlagBits target = (queue_type == QueueType::kCompute)
//      ? vk::QueueFlagBits::eCompute
//      : vk::QueueFlagBits::eTransfer;
//  const vk::QueueFlagBits other = (queue_type == QueueType::kCompute)
//      ? vk::QueueFlagBits::eTransfer
//      : vk::QueueFlagBits::eCompute;
//
//  auto has_flag = [](const vk::QueueFamilyProperties& family,
//                     const vk::QueueFlagBits flag)
//  {
//    return (family.queueFlags & flag) == flag;
//  };
//
//  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
//    const auto& family = queue_family_list[i].properties1_;
//    if (!has_flag(family, vk::QueueFlagBits::eGraphics) &&
//        !has_flag(family, other) && has_flag(family, target)) {
//      index = zisc::cast<uint32b>(i);
//      is_found = true;
//    }
//  }
//
//  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
//    const auto& family = queue_family_list[i].properties1_;
//    if (!has_flag(family, other) && has_flag(family, target)) {
//      index = zisc::cast<uint32b>(i);
//      is_found = true;
//    }
//  }
//
//  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
//    const auto& family = queue_family_list[i].properties1_;
//    if (!has_flag(family, vk::QueueFlagBits::eGraphics) &&
//        has_flag(family, target)) {
//      index = zisc::cast<uint32b>(i);
//      is_found = true;
//    }
//  }
//
//  for (std::size_t i = 0; !is_found && (i < queue_family_list.size()); ++i) {
//    const auto& family = queue_family_list[i].properties1_;
//    if (has_flag(family, target)) {
//      index = zisc::cast<uint32b>(i);
//      is_found = true;
//    }
//  }
//
//  return index;
//}
//
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
//
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

/*!
  \details No detailed description

  \param [in] options No description.
  */
void VulkanDevice::initialize(const DeviceOptions& options) noexcept
{
  app_info_ = makeApplicationInfo(options.applicationName(),
                                  options.applicationVersionMajor(),
                                  options.applicationVersionMinor(),
                                  options.applicationVersionPatch());
//  instance_ = makeInstance(app_info_, options.enable_debug_);
//  ZISC_ASSERT(instance_, "Vulkan instance creation failed.");
//  if (options.enable_debug_)
//    initDebugMessenger();
//  initPhysicalDevice(options);
//  initQueueFamilyIndexList();
//
//  {
//    const auto& info = physicalDeviceInfo();
//    uint32b subgroup_size = info.properties().subgroup_.subgroupSize;
//    subgroup_size = zisc::isInClosedBounds(subgroup_size, 1u, 128u)
//        ? subgroup_size
//        : getVendorSubgroupSize(info.properties().properties1_.vendorID);
//    vendor_name_ = getVendorName(info.properties().properties1_.vendorID);
//    initLocalWorkSize(subgroup_size);
//  }
//
//  initDevice(options);
//  initCommandPool();
//  initMemoryAllocator();
}

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
//
///*!
//  */
//inline
//void VulkanDevice::initQueueFamilyIndexList() noexcept
//{
//  std::array<uint32b, 2> index_list{{findQueueFamily(QueueType::kCompute),
//                                     findQueueFamily(QueueType::kTransfer)}};
//  queue_family_index_list_.reserve(index_list.size());
//  for (std::size_t i = 0; i < index_list.size(); ++i) {
//    const auto family_index = index_list[i];
//    auto ite = std::find(queue_family_index_list_.begin(),
//                         queue_family_index_list_.end(),
//                         family_index);
//    if (ite != queue_family_index_list_.end()) {
//      const auto ref_index = std::distance(queue_family_index_list_.begin(), ite);
//      queue_family_index_ref_list_[i] = zisc::cast<std::size_t>(ref_index);
//    }
//    else {
//      queue_family_index_list_.emplace_back(family_index);
//      queue_family_index_ref_list_[i] = i;
//    }
//  }
//}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  \return No description
  */
vk::AllocationCallbacks VulkanDevice::makeAllocator(
    zisc::pmr::memory_resource* mem_resource) noexcept
{
  vk::AllocationCallbacks alloc{mem_resource,
                                Callbacks::allocateMemory,
                                Callbacks::reallocateMemory,
                                Callbacks::freeMemory,
                                Callbacks::notifyOfMemoryAllocation,
                                Callbacks::notifyOfMemoryFreeing};
  return alloc;
}

/*!
  \details No detailed description

  \param [in] app_name No description.
  \param [in] app_version_major No description.
  \param [in] app_version_minor No description.
  \param [in] app_version_patch No description.
  \return No description
  */
vk::ApplicationInfo VulkanDevice::makeApplicationInfo(
    const std::string_view app_name,
    const uint32b app_version_major,
    const uint32b app_version_minor,
    const uint32b app_version_patch) noexcept
{
  const uint32b app_version = VK_MAKE_VERSION(app_version_major,
                                              app_version_minor,
                                              app_version_patch);
  const std::string_view engine_name{"Zinvul"};
  constexpr uint32b engine_version = VK_MAKE_VERSION(Config::versionMajor(),
                                                     Config::versionMinor(),
                                                     Config::versionPatch());
  constexpr uint32b api_version = VK_API_VERSION_1_2;
  const vk::ApplicationInfo data{app_name.data(),
                                 app_version,
                                 engine_name.data(),
                                 engine_version,
                                 api_version};
  return data;
}

/*!
  \details No detailed description

  \param [in] app_info No description.
  \param [in] enable_debug No description.
  \param [in,out] alloc No description.
  \return No description
  */
vk::Instance VulkanDevice::makeInstance(const vk::ApplicationInfo& app_info,
                                        const bool enable_debug,
                                        vk::AllocationCallbacks* alloc)
{
  auto mem_resource = zisc::cast<zisc::pmr::memory_resource*>(alloc->pUserData);

  zisc::pmr::vector<const char*>::allocator_type layer_alloc{mem_resource};
  zisc::pmr::vector<const char*> layers{layer_alloc};
  zisc::pmr::vector<const char*> extensions{layer_alloc};

  extensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  if (enable_debug) {
    layers.emplace_back("VK_LAYER_KHRONOS_validation");
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  // Debug utils extension
  const auto severity_flags =
//      vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
  const auto message_type_flags =
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
  PFN_vkDebugUtilsMessengerCallbackEXT callback = Callbacks::printDebugMessage;
  vk::DebugUtilsMessengerCreateInfoEXT debug_utils_create_info{
      vk::DebugUtilsMessengerCreateFlagsEXT{},
      severity_flags,
      message_type_flags,
      callback};

  // Validation features
  std::array<vk::ValidationFeatureEnableEXT, 3> validation_features_list{{
      vk::ValidationFeatureEnableEXT::eGpuAssisted,
      vk::ValidationFeatureEnableEXT::eGpuAssistedReserveBindingSlot,
      vk::ValidationFeatureEnableEXT::eBestPractices}};
  vk::ValidationFeaturesEXT validation_features{
      zisc::cast<uint32b>(validation_features_list.size()),
      validation_features_list.data(),
      0,
      nullptr};

  vk::InstanceCreateInfo createInfo{vk::InstanceCreateFlags{},
                                    &app_info,
                                    zisc::cast<uint32b>(layers.size()),
                                    layers.data(),
                                    zisc::cast<uint32b>(extensions.size()),
                                    extensions.data()};
  if (enable_debug) {
    createInfo.setPNext(std::addressof(debug_utils_create_info));
    debug_utils_create_info.setPNext(std::addressof(validation_features));
  }

  auto instance = vk::createInstance(createInfo, *alloc);
  return instance;
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
