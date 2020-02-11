/*!
  \file vulkan_device_info.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_device_info.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "utility/vulkan.hpp"
#include "utility/vulkan_dispatch_loader.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
VulkanDeviceInfo::VulkanDeviceInfo(zisc::pmr::memory_resource* mem_resource)
    noexcept :
        DeviceInfo(),
        extension_properties_list_{
          decltype(extension_properties_list_)::allocator_type{mem_resource}},
        layer_properties_list_{
          decltype(layer_properties_list_)::allocator_type{mem_resource}},
        queue_family_properties_list_{
          decltype(queue_family_properties_list_)::allocator_type{mem_resource}},
        tool_properties_list_{
          decltype(tool_properties_list_)::allocator_type{mem_resource}},
        device_local_index_list_{
          decltype(device_local_index_list_)::allocator_type{mem_resource}},
        vendor_name_{zisc::pmr::string::allocator_type{mem_resource}}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
VulkanDeviceInfo::VulkanDeviceInfo(VulkanDeviceInfo&& other) noexcept :
    DeviceInfo(std::move(other)),
    extension_properties_list_{std::move(other.extension_properties_list_)},
    layer_properties_list_{std::move(other.layer_properties_list_)},
    queue_family_properties_list_{std::move(other.queue_family_properties_list_)},
    tool_properties_list_{std::move(other.tool_properties_list_)},
    device_local_index_list_{std::move(other.device_local_index_list_)},
    vendor_name_{std::move(other.vendor_name_)},
    device_{other.device_},
    properties_{std::move(other.properties_)},
    features_{std::move(other.features_)},
    memory_properties_{std::move(other.memory_properties_)}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
VulkanDeviceInfo& VulkanDeviceInfo::operator=(VulkanDeviceInfo&& other) noexcept
{
  DeviceInfo::operator=(std::move(other));
  extension_properties_list_ = std::move(other.extension_properties_list_);
  layer_properties_list_ = std::move(other.layer_properties_list_);
  queue_family_properties_list_ = std::move(other.queue_family_properties_list_);
  tool_properties_list_ = std::move(other.tool_properties_list_);
  device_local_index_list_ = std::move(other.device_local_index_list_);
  vendor_name_ = std::move(other.vendor_name_);
  device_ = other.device_;
  properties_ = std::move(other.properties_);
  features_ = std::move(other.features_);
  memory_properties_ = std::move(other.memory_properties_);
  return *this;
}

/*!
  \details No detailed description
  */
VulkanDeviceInfo::~VulkanDeviceInfo() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDeviceInfo::availableMemory(const std::size_t heap_index) const noexcept
{
  const std::size_t index = device_local_index_list_[heap_index];
  const auto& budget = memoryProperties().budget_;
  const std::size_t mem = budget.heapBudget[index] - budget.heapUsage[index];
  return mem;
}

/*!
  \details No detailed description

  \param [in] vdevice No description.
  */
void VulkanDeviceInfo::fetch(const VkPhysicalDevice& vdevice,
                             const VulkanDispatchLoader& dispatcher)
{
  device_ = vdevice;
  // Fetch properties from the given device
  fetchExtensionProperties(dispatcher);
  fetchLayerProperties(dispatcher);
  fetchQueueFamilyProperties(dispatcher);
  fetchToolProperties(dispatcher);
  fetchProperties(dispatcher);
  fetchFeatures(dispatcher);
  fetchMemoryProperties(dispatcher);
  // Set device info
  setVendorNameFromId(properties().properties1_.vendorID);
  findDeviceLocalHeaps();
}
/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDeviceInfo::maxAllocationSize() const noexcept
{
  const Properties& props = properties();
  const std::size_t max_alloc = props.maintenance3_.maxMemoryAllocationSize;
  return max_alloc;
}

/*!
  \details No detailed description

  \return No description
  */
std::array<uint32b, 3> VulkanDeviceInfo::maxWorkGroupCount() const noexcept
{
  const Properties& props = properties();
  const uint32b* counts = props.properties1_.limits.maxComputeWorkGroupCount;
  const std::array<uint32b, 3> max_list{{counts[0], counts[1], counts[2]}};
  return max_list;
}

/*!
  \details No detailed description

  \return No description
  */
std::string_view VulkanDeviceInfo::name() const noexcept
{
  std::string_view n{properties_.properties1_.deviceName};
  return n;
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDeviceInfo::numOfHeaps() const noexcept
{
  return device_local_index_list_.size();
}

/*!
  \details No detailed description

  \return No description
  */
std::size_t VulkanDeviceInfo::totalMemory(const std::size_t heap_index) const noexcept
{
  const std::size_t index = device_local_index_list_[heap_index];
  const auto& heap = memoryProperties().properties1_.memoryHeaps[index];
  const std::size_t mem = heap.size;
  return mem;
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType VulkanDeviceInfo::type() const noexcept
{
  return SubPlatformType::kVulkan;
}

/*!
  \details No detailed description

  \return No description
  */
std::string_view VulkanDeviceInfo::vendorName() const noexcept
{
  return vendor_name_;
}

/*!
  \details No detailed description

  \return No description
  */
uint32b VulkanDeviceInfo::workGroupSize() const noexcept
{
  const Properties& props = properties();
  return props.subgroup_.subgroupSize;
}

/*!
  \details No detailed description
  */
void VulkanDeviceInfo::fetchExtensionProperties(
    const VulkanDispatchLoader& dispatcher)
{
  using Props = zinvulvk::ExtensionProperties;
  static_assert(sizeof(VkExtensionProperties) == sizeof(Props),
                "The sizes of properties don't match.");

  const zinvulvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  using PropertiesList = zisc::pmr::vector<Props>;
  extension_properties_list_.clear();
  auto props_list = zisc::treatAs<PropertiesList*>(&extension_properties_list_);
  *props_list = d.enumerateDeviceExtensionProperties(nullptr, 
                                                     props_list->get_allocator(),
                                                     *loader);
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchFeatures(
    const VulkanDispatchLoader& dispatcher)
{
  const zinvulvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  zinvulvk::PhysicalDeviceFeatures2 p;
  auto& props = features_;
  link(p,
       initProp<zinvulvk::PhysicalDevice16BitStorageFeatures>(
          props.b16bit_storage_),
       initProp<zinvulvk::PhysicalDevice8BitStorageFeatures>(
          props.b8bit_storage_),
       initProp<zinvulvk::PhysicalDeviceASTCDecodeFeaturesEXT>(
          props.astc_decode_),
       initProp<zinvulvk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT>(
          props.blend_operation_advanced_),
       initProp<zinvulvk::PhysicalDeviceBufferDeviceAddressFeatures>(
          props.buffer_device_address_),
       initProp<zinvulvk::PhysicalDeviceConditionalRenderingFeaturesEXT>(
          props.conditional_rendering_),
       initProp<zinvulvk::PhysicalDeviceDepthClipEnableFeaturesEXT>(
          props.depth_clip_enabled_),
       initProp<zinvulvk::PhysicalDeviceDescriptorIndexingFeatures>(
          props.descriptor_indexing_),
       initProp<zinvulvk::PhysicalDeviceFragmentDensityMapFeaturesEXT>(
          props.fragment_density_map_),
       initProp<zinvulvk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT>(
          props.fragment_shader_inter_lock_),
       initProp<zinvulvk::PhysicalDeviceHostQueryResetFeatures>(
          props.host_query_reset_),
       initProp<zinvulvk::PhysicalDeviceImagelessFramebufferFeatures>(
          props.imageless_framebuffer_),
       initProp<zinvulvk::PhysicalDeviceIndexTypeUint8FeaturesEXT>(
          props.index_type_uint8_),
       initProp<zinvulvk::PhysicalDeviceInlineUniformBlockFeaturesEXT>(
          props.inline_uniform_block_),
       initProp<zinvulvk::PhysicalDeviceLineRasterizationFeaturesEXT>(
          props.line_rasterization_),
       initProp<zinvulvk::PhysicalDeviceMemoryPriorityFeaturesEXT>(
          props.memory_priority_features_),
       initProp<zinvulvk::PhysicalDeviceMultiviewFeatures>(
          props.multiview_),
       initProp<zinvulvk::PhysicalDevicePerformanceQueryFeaturesKHR>(
          props.performance_query_),
       initProp<zinvulvk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR>(
          props.pipeline_executable_properties_),
       initProp<zinvulvk::PhysicalDeviceProtectedMemoryFeatures>(
          props.protected_memory_),
       initProp<zinvulvk::PhysicalDeviceSamplerYcbcrConversionFeatures>(
          props.sampler_ycbcr_conversion_),
       initProp<zinvulvk::PhysicalDeviceScalarBlockLayoutFeatures>(
          props.scalar_block_layout_),
       initProp<zinvulvk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures>(
          props.depth_stencil_layouts_),
       initProp<zinvulvk::PhysicalDeviceShaderAtomicInt64Features>(
          props.shader_atomic_int64_),
       initProp<zinvulvk::PhysicalDeviceShaderClockFeaturesKHR>(
          props.shader_clock_),
       initProp<zinvulvk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT>(
          props.demote_to_helper_invocation_),
       initProp<zinvulvk::PhysicalDeviceShaderDrawParametersFeatures>(
          props.shader_draw_parameters_),
       initProp<zinvulvk::PhysicalDeviceShaderFloat16Int8FeaturesKHR>(
          props.shader_float16_int8_),
       initProp<zinvulvk::PhysicalDeviceShaderSubgroupExtendedTypesFeatures>(
          props.shader_subgroup_extended_types_),
       initProp<zinvulvk::PhysicalDeviceSubgroupSizeControlFeaturesEXT>(
          props.subgroup_size_control_),
       initProp<zinvulvk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT>(
          props.texel_buffer_alignment_),
       initProp<zinvulvk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT>(
          props.texture_compression_astchdr_),
       initProp<zinvulvk::PhysicalDeviceTimelineSemaphoreFeatures>(
          props.timeline_semaphore_),
       initProp<zinvulvk::PhysicalDeviceTransformFeedbackFeaturesEXT>(
          props.transform_feedback_),
       initProp<zinvulvk::PhysicalDeviceUniformBufferStandardLayoutFeatures>(
          props.uniform_buffer_standard_layout_),
       initProp<zinvulvk::PhysicalDeviceVariablePointerFeatures>(
          props.variable_pointers_),
       initProp<zinvulvk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT>(
          props.vertex_attribute_divisor_),
       initProp<zinvulvk::PhysicalDeviceVulkan11Features>(
          props.vulkan11_),
       initProp<zinvulvk::PhysicalDeviceVulkan12Features>(
          props.vulkan12_),
       initProp<zinvulvk::PhysicalDeviceVulkanMemoryModelFeatures>(
          props.vulkan_memory_mode_),
       initProp<zinvulvk::PhysicalDeviceYcbcrImageArraysFeaturesEXT>(
          props.ycbcr_image_arrays_)
       );
  d.getFeatures2(&p, *loader);
  props.features1_ = zisc::cast<VkPhysicalDeviceFeatures>(p.features);
}

/*!
  \details No detailed description
  */
void VulkanDeviceInfo::fetchLayerProperties(
    const VulkanDispatchLoader& dispatcher)
{
  using Props = zinvulvk::LayerProperties;
  static_assert(sizeof(VkLayerProperties) == sizeof(Props),
                "The sizes of properties don't match.");

  const zinvulvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  using PropertiesList = zisc::pmr::vector<Props>;
  layer_properties_list_.clear();
  auto props_list = zisc::treatAs<PropertiesList*>(&layer_properties_list_);
  *props_list = d.enumerateDeviceLayerProperties(props_list->get_allocator(),
                                                 *loader);
}

void VulkanDeviceInfo::fetchMemoryProperties(
    const VulkanDispatchLoader& dispatcher)
{
  const zinvulvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  zinvulvk::PhysicalDeviceMemoryProperties2 p;
  auto& props = memory_properties_;
  link(p,
       initProp<zinvulvk::PhysicalDeviceMemoryBudgetPropertiesEXT>(
          props.budget_)
       );
  d.getMemoryProperties2(&p, *loader);
  props.properties1_ =
      zisc::cast<VkPhysicalDeviceMemoryProperties>(p.memoryProperties);
}

/*!
  \details No detailed description

  \param [in] dispatcher No description.
  */
void VulkanDeviceInfo::fetchProperties(
    const VulkanDispatchLoader& dispatcher)
{
  const zinvulvk::PhysicalDevice d{device()};
  const auto loader = dispatcher.loaderImpl();

  zinvulvk::PhysicalDeviceProperties2 p;
  auto& props = properties_;
  link(p,
       initProp<zinvulvk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT>(
          props.blend_operation_advanced_),
       initProp<zinvulvk::PhysicalDeviceConservativeRasterizationPropertiesEXT>(
          props.conservative_rasterization_),
       initProp<zinvulvk::PhysicalDeviceDepthStencilResolveProperties>(
          props.depth_stencil_resolve_),
       initProp<zinvulvk::PhysicalDeviceDescriptorIndexingProperties>(
          props.descriptor_indexing_),
       initProp<zinvulvk::PhysicalDeviceDiscardRectanglePropertiesEXT>(
          props.discard_rectangle_),
       initProp<zinvulvk::PhysicalDeviceDriverProperties>(
          props.driver_),
       initProp<zinvulvk::PhysicalDeviceExternalMemoryHostPropertiesEXT>(
          props.external_memory_host_),
       initProp<zinvulvk::PhysicalDeviceFloatControlsProperties>(
          props.float_controls_),
       initProp<zinvulvk::PhysicalDeviceFragmentDensityMapPropertiesEXT>(
          props.fragment_density_map_),
       initProp<zinvulvk::PhysicalDeviceIDProperties>(
          props.id_properties_),
       initProp<zinvulvk::PhysicalDeviceInlineUniformBlockPropertiesEXT>(
          props.inline_uniform_block_),
       initProp<zinvulvk::PhysicalDeviceLineRasterizationPropertiesEXT>(
          props.line_rasterization_),
       initProp<zinvulvk::PhysicalDeviceMaintenance3Properties>(
          props.maintenance3_),
       initProp<zinvulvk::PhysicalDeviceMultiviewProperties>(
          props.multiview_),
       initProp<zinvulvk::PhysicalDevicePCIBusInfoPropertiesEXT>(
          props.pci_bus_info_),
       initProp<zinvulvk::PhysicalDevicePerformanceQueryPropertiesKHR>(
          props.performance_query_),
       initProp<zinvulvk::PhysicalDevicePointClippingProperties>(
          props.point_clipping_),
       initProp<zinvulvk::PhysicalDeviceProtectedMemoryProperties>(
          props.protected_memory_),
       initProp<zinvulvk::PhysicalDevicePushDescriptorPropertiesKHR>(
          props.push_descriptor_),
       initProp<zinvulvk::PhysicalDeviceSampleLocationsPropertiesEXT>(
          props.sample_locations_),
       initProp<zinvulvk::PhysicalDeviceSamplerFilterMinmaxProperties>(
          props.sampler_filter_minmax_),
       initProp<zinvulvk::PhysicalDeviceSubgroupProperties>(
          props.subgroup_),
       initProp<zinvulvk::PhysicalDeviceSubgroupSizeControlPropertiesEXT>(
          props.subgroup_size_control_),
       initProp<zinvulvk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT>(
          props.texel_buffer_alignment_),
       initProp<zinvulvk::PhysicalDeviceTimelineSemaphoreProperties>(
          props.timeline_semaphore_),
       initProp<zinvulvk::PhysicalDeviceTransformFeedbackPropertiesEXT>(
          props.transform_feedback_),
       initProp<zinvulvk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT>(
          props.vertex_attribute_divisor_),
       initProp<zinvulvk::PhysicalDeviceVulkan11Properties>(
          props.vulkan11_),
       initProp<zinvulvk::PhysicalDeviceVulkan12Properties>(
          props.vulkan12_)
       );
  d.getProperties2(&p, *loader);
  props.properties1_ = zisc::cast<VkPhysicalDeviceProperties>(p.properties);
}

/*!
  \details No detailed description
  */
void VulkanDeviceInfo::fetchToolProperties(
    const VulkanDispatchLoader& dispatcher)
{
  auto mem_resource = tool_properties_list_.get_allocator().resource();
  const zinvulvk::PhysicalDevice d{device()};

  // Tool properties
  {
    const auto loader = dispatcher.loaderImpl();

    // Get the number of properties
    uint32b n = 0;
    d.getToolPropertiesEXT(&n, nullptr, *loader);

    // Get properties
    using PropertiesList = zisc::pmr::vector<zinvulvk::PhysicalDeviceToolPropertiesEXT>;
    PropertiesList props_list{PropertiesList::allocator_type{mem_resource}};
    props_list.resize(n);
    d.getToolPropertiesEXT(&n, props_list.data(), *loader);

    //
    tool_properties_list_.clear();
    tool_properties_list_.resize(n);
    for (std::size_t i = 0; i < props_list.size(); ++i) {
      const auto& prop = props_list[i];
      tool_properties_list_[i].properties1_ =
          zisc::cast<VkPhysicalDeviceToolPropertiesEXT>(prop);
    }
  }
}

/*!
  \details No detailed description
  */
void VulkanDeviceInfo::fetchQueueFamilyProperties(
    const VulkanDispatchLoader& dispatcher)
{
  auto mem_resource = queue_family_properties_list_.get_allocator().resource();
  const zinvulvk::PhysicalDevice d{device()};

  // Queue family properties
  {
    const auto loader = dispatcher.loaderImpl();

    // Get the number of properties
    uint32b n = 0;
    d.getQueueFamilyProperties2(&n, nullptr, *loader);

    // Get properties
    using PropertiesList = zisc::pmr::vector<zinvulvk::QueueFamilyProperties2>;
    PropertiesList props_list{PropertiesList::allocator_type{mem_resource}};
    props_list.resize(n);
    d.getQueueFamilyProperties2(&n, props_list.data(), *loader);

    //
    queue_family_properties_list_.clear();
    queue_family_properties_list_.resize(n);
    for (std::size_t i = 0; i < props_list.size(); ++i) {
      const auto& prop = props_list[i];
      queue_family_properties_list_[i].properties1_ =
          zisc::cast<VkQueueFamilyProperties>(prop.queueFamilyProperties);
    }
  }
}

/*!
  \details No detailed description
  */
void VulkanDeviceInfo::findDeviceLocalHeaps() noexcept
{
  device_local_index_list_.clear();
  const auto& props = memoryProperties().properties1_;
  for (std::size_t i = 0; i < props.memoryHeapCount; ++i) {
    const auto& heap = props.memoryHeaps[i];
    const auto heap_flags = zisc::cast<zinvulvk::MemoryHeapFlags>(heap.flags);
    if(heap_flags & zinvulvk::MemoryHeapFlagBits::eDeviceLocal)
      device_local_index_list_.emplace_back(i);
  }
}

/*!
  \details No detailed description

  \param [in] vendor_id No description.
  */
void VulkanDeviceInfo::setVendorNameFromId(const uint32b vendor_id) noexcept
{
  using namespace std::string_literals;
  switch (vendor_id) {
   case zisc::cast<uint32b>(VendorId::kAmd): {
    vendor_name_ = "AMD"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kImgTec): {
    vendor_name_ = "ImgTec"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kNvidia): {
    vendor_name_ = "NVIDIA"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kArm): {
    vendor_name_ = "ARM"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kQualcomm): {
    vendor_name_ = "Qualcomm"s;
    break;
   }
   case zisc::cast<uint32b>(VendorId::kIntel): {
    vendor_name_ = "INTEL"s;
    break;
   }
   default: {
    vendor_name_ = "N/A"s;
    break;
   }
  }
}

} // namespace zinvul
