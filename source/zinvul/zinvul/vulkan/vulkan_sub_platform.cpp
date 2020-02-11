/*!
  \file vulkan_sub_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_sub_platform.hpp"
// Standard C++ library
#include <cstddef>
#include <iostream>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_device_info.hpp"
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zinvul/platform_options.hpp"
#include "zinvul/sub_platform.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
VulkanSubPlatform::VulkanSubPlatform() noexcept
{
}

/*!
  \details No detailed description
  */
VulkanSubPlatform::~VulkanSubPlatform() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in,out] device_info_list No description.
  */
void VulkanSubPlatform::getDeviceInfoList(
    zisc::pmr::vector<const DeviceInfo*>& device_info_list) const noexcept
{
  for (const auto& device_info : *device_info_list_)
    device_info_list.emplace_back(std::addressof(device_info));
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanSubPlatform::numOfDevices() const noexcept
{
  const std::size_t n = device_list_->size();
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType VulkanSubPlatform::type() const noexcept
{
  return SubPlatformType::kVulkan;
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::updateDeviceInfoList() noexcept
{
  device_info_list_->clear();
  device_info_list_->reserve(numOfDevices());
  for (const auto& device : deviceList()) {
    device_info_list_->emplace_back(memoryResource());
    device_info_list_->back().fetch(device, dispatcher());
  }
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::destroyData() noexcept
{
  device_info_list_.reset();
  device_list_.reset();

  zinvulvk::Instance ins{instance()};
  if (ins) {
    zinvulvk::AllocationCallbacks alloc{makeAllocator()};
    const auto loader = dispatcher().loaderImpl();
    ins.destroy(alloc, *loader);
    instance_ = nullptr;
  }

  dispatcher_.reset();
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void VulkanSubPlatform::initData(PlatformOptions& platform_options)
{
  initDispatcher();
  initInstance(platform_options);
  initDeviceList();
  initDeviceInfoList();

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


/*!
  \details No detailed description

  \param [in,out] user_data No description.
  \param [in] size No description.
  \param [in] alignment No description.
  \param [in] scope No description.
  \return No description
  */
auto VulkanSubPlatform::Callbacks::allocateMemory(
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
void VulkanSubPlatform::Callbacks::freeMemory(
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
void VulkanSubPlatform::Callbacks::notifyOfMemoryAllocation(
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
void VulkanSubPlatform::Callbacks::notifyOfMemoryFreeing(
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
auto VulkanSubPlatform::Callbacks::printDebugMessage(
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

  const char indent[] = "          ";
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
    std::sprintf(tmp, "%s - Message ID Number %d, Message ID Name %s : %s\n",
        prefix,
        callback_data->messageIdNumber,
        callback_data->pMessageIdName,
        callback_data->pMessage);
    message += tmp;
  }

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
      const auto obj_type_name = zinvulvk::to_string(
          zisc::cast<zinvulvk::ObjectType>(object.objectType));
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
auto VulkanSubPlatform::Callbacks::reallocateMemory(
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
    std::cerr << "Warning: copying data in the reallocation isn't operated."
              << std::endl;
  // Deallocate the original memory
  if (original_memory)
    mem_resource->deallocate(original_memory, 0, 0);
  return memory;
}

/*!
  \details No detailed description

  \return No description
  */
VkAllocationCallbacks VulkanSubPlatform::makeAllocator() noexcept
{
  zinvulvk::AllocationCallbacks alloc{memoryResource(),
                                      Callbacks::allocateMemory,
                                      Callbacks::reallocateMemory,
                                      Callbacks::freeMemory,
                                      Callbacks::notifyOfMemoryAllocation,
                                      Callbacks::notifyOfMemoryFreeing};
  return zisc::cast<VkAllocationCallbacks>(alloc);
}

/*!
  \details No detailed description

  \param [in] app_name No description.
  \param [in] app_version_major No description.
  \param [in] app_version_minor No description.
  \param [in] app_version_patch No description.
  \return No description
  */
VkApplicationInfo VulkanSubPlatform::makeApplicationInfo(
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
  const zinvulvk::ApplicationInfo data{app_name.data(),
                                       app_version,
                                       engine_name.data(),
                                       engine_version,
                                       api_version};
  return zisc::cast<VkApplicationInfo>(data);
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void VulkanSubPlatform::initInstance(PlatformOptions& platform_options)
{
  zisc::pmr::vector<const char*>::allocator_type layer_alloc{memoryResource()};
  zisc::pmr::vector<const char*> layers{layer_alloc};
  zisc::pmr::vector<const char*> extensions{layer_alloc};

  extensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  if (platform_options.debugModeEnabled()) {
    layers.emplace_back("VK_LAYER_KHRONOS_validation");
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  // Debug utils extension
  const auto severity_flags =
//      zinvulvk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
      zinvulvk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      zinvulvk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
  const auto message_type_flags =
      zinvulvk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      zinvulvk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      zinvulvk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
  PFN_vkDebugUtilsMessengerCallbackEXT callback = Callbacks::printDebugMessage;
  zinvulvk::DebugUtilsMessengerCreateInfoEXT debug_utils_create_info{
      zinvulvk::DebugUtilsMessengerCreateFlagsEXT{},
      severity_flags,
      message_type_flags,
      callback};

  // Validation features
  std::array<zinvulvk::ValidationFeatureEnableEXT, 3> validation_features_list{{
      zinvulvk::ValidationFeatureEnableEXT::eGpuAssisted,
      zinvulvk::ValidationFeatureEnableEXT::eGpuAssistedReserveBindingSlot,
      zinvulvk::ValidationFeatureEnableEXT::eBestPractices}};
  zinvulvk::ValidationFeaturesEXT validation_features{
      zisc::cast<uint32b>(validation_features_list.size()),
      validation_features_list.data(),
      0,
      nullptr};

  const zinvulvk::ApplicationInfo app_info{makeApplicationInfo(
      platform_options.platformName(),
      platform_options.platformVersionMajor(),
      platform_options.platformVersionMinor(),
      platform_options.platformVersionPatch())};
  zinvulvk::InstanceCreateInfo createInfo{zinvulvk::InstanceCreateFlags{},
                                          &app_info,
                                          zisc::cast<uint32b>(layers.size()),
                                          layers.data(),
                                          zisc::cast<uint32b>(extensions.size()),
                                          extensions.data()};
  if (platform_options.debugModeEnabled()) {
    createInfo.setPNext(std::addressof(debug_utils_create_info));
    debug_utils_create_info.setPNext(std::addressof(validation_features));
  }

  zinvulvk::AllocationCallbacks alloc{makeAllocator()};
  const auto loader = dispatcher().loaderImpl();
  zinvulvk::Instance ins = zinvulvk::createInstance(createInfo, alloc, *loader);
  instance_ = zisc::cast<VkInstance>(ins);
  dispatcher_->set(instance());
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initDeviceList()
{
  const zinvulvk::Instance ins{instance()};

  using DeviceList = zisc::pmr::vector<zinvulvk::PhysicalDevice>;
  const DeviceList::allocator_type alloc{memoryResource()};
  const auto loader = dispatcher().loaderImpl();
  auto device_list = ins.enumeratePhysicalDevices(alloc, *loader);

  using DstDeviceList = decltype(device_list_)::element_type;
  auto list_ptr = zisc::treatAs<DstDeviceList*>(std::addressof(device_list));
  device_list_ = zisc::pmr::allocateUnique<DstDeviceList>(
      memoryResource(),
      std::move(*list_ptr));
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initDeviceInfoList() noexcept
{
  using DeviceInfoList = decltype(device_info_list_)::element_type;
  DeviceInfoList info_list{DeviceInfoList::allocator_type{memoryResource()}};
  device_info_list_ = zisc::pmr::allocateUnique<DeviceInfoList>(
      memoryResource(),
      std::move(info_list));
}

/*!
  \details No detailed description
  */
void VulkanSubPlatform::initDispatcher()
{
  auto mem_resource = memoryResource();
  dispatcher_ = zisc::pmr::allocateUnique<VulkanDispatchLoader>(mem_resource,
                                                                mem_resource);
}

} // namespace zinvul
