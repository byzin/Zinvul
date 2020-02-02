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
#include <cstddef>
#include <utility>
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
VulkanDeviceInfo::VulkanDeviceInfo(std::pmr::memory_resource* mem_resource)
    noexcept :
        extension_properties_list_{
          zisc::pmr::vector<ExtensionProperties>::allocator_type{mem_resource}},
        layer_properties_list_{
          zisc::pmr::vector<LayerProperties>::allocator_type{mem_resource}},
        queue_family_properties_list_{
          zisc::pmr::vector<QueueFamilyProperties>::allocator_type{mem_resource}},
        tool_properties_list_{
          zisc::pmr::vector<ToolProperties>::allocator_type{mem_resource}}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
VulkanDeviceInfo::VulkanDeviceInfo(VulkanDeviceInfo&& other) noexcept :
    extension_properties_list_{std::move(other.extension_properties_list_)},
    layer_properties_list_{std::move(other.layer_properties_list_)},
    queue_family_properties_list_{std::move(other.queue_family_properties_list_)},
    tool_properties_list_{std::move(other.tool_properties_list_)},
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
  extension_properties_list_ = std::move(other.extension_properties_list_);
  layer_properties_list_ = std::move(other.layer_properties_list_);
  queue_family_properties_list_ = std::move(other.queue_family_properties_list_);
  tool_properties_list_ = std::move(other.tool_properties_list_);
  properties_ = std::move(other.properties_);
  features_ = std::move(other.features_);
  memory_properties_ = std::move(other.memory_properties_);
  return *this;
}

///*!
//  */
//inline
//auto VulkanDeviceInfo::extensionPropertiesList() noexcept
//    -> zisc::pmr::vector<ExtensionProperties>&
//{
//  return extension_properties_list_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::extensionPropertiesList() const noexcept
//    -> const zisc::pmr::vector<ExtensionProperties>&
//{
//  return extension_properties_list_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::features() noexcept -> Features&
//{
//  return features_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::features() const noexcept -> const Features&
//{
//  return features_;
//}

/*!
  \details No detailed description

  \param [in] device No description.
  */
void VulkanDeviceInfo::fetch(const vk::PhysicalDevice& device) noexcept
{
  fetchExtensionProperties(device);
//  fetchLayerProperties(device);
//  fetchQueueFamilyProperties(device);
//  fetchProperties(device);
//  fetchFeatures(device);
//  fetchMemoryProperties(device);
}

///*!
//  */
//inline
//auto VulkanDeviceInfo::layerPropertiesList() noexcept
//    -> zisc::pmr::vector<LayerProperties>&
//{
//  return layer_properties_list_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::layerPropertiesList() const noexcept
//    -> const zisc::pmr::vector<LayerProperties>&
//{
//  return layer_properties_list_;
//}
//
///*!
//  */
//template <typename Type1, typename Type2, typename ...Types> inline
//void VulkanDeviceInfo::link(Type1&& value1,
//                                    Type2&& value2,
//                                    Types&&... values) noexcept
//{
//  constexpr std::size_t num_of_rests = 1 + sizeof...(Types);
//  value1.pNext = &value2;
//  if constexpr (1 < num_of_rests) {
//    link(value2, values...);
//  }
//  else {
//    value2.pNext = nullptr;
//  }
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::memoryProperties() noexcept
//    -> MemoryProperties&
//{
//  return memory_properties_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::memoryProperties() const noexcept
//    -> const MemoryProperties&
//{
//  return memory_properties_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::properties() noexcept -> Properties&
//{
//  return properties_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::properties() const noexcept -> const Properties&
//{
//  return properties_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::queueFamilyPropertiesList() noexcept
//    -> zisc::pmr::vector<QueueFamilyProperties>&
//{
//  return queue_family_properties_list_;
//}
//
///*!
//  */
//inline
//auto VulkanDeviceInfo::queueFamilyPropertiesList() const noexcept
//    -> const zisc::pmr::vector<QueueFamilyProperties>&
//{
//  return queue_family_properties_list_;
//}

/*!
  \details No detailed description

  \param [in] device No description.
  */
void VulkanDeviceInfo::fetchExtensionProperties(
    const vk::PhysicalDevice& device) noexcept
{
//  auto [result, properties] = device.enumerateDeviceExtensionProperties();
//  ZISC_ASSERT(result == vk::Result::eSuccess,
//              "Vulkan device failed to get extension properties.");
//  extension_properties_list_.resize(properties.size());
//  for (std::size_t i = 0; i < properties.size(); ++i) {
//    extension_properties_list_[i].properties1_ = properties[i];
//  }
}

//void VulkanDeviceInfo::fetchFeatures(
//    const vk::PhysicalDevice& device) noexcept
//{
//  vk::PhysicalDeviceFeatures2 p;
//  auto& props = features_;
//  link(p,
//       props.b16bit_storage_,
//       props.b8bit_storage_,
//       props.astc_decode_,
//       props.blend_operation_advanced_,
//       props.buffer_device_address_,
//       props.conditional_rendering_,
//       props.depth_clip_enabled_,
//       props.descriptor_indexing_,
//       props.float16_int8_,
//       props.fragment_density_map_,
////       props.fragment_shader_inter_lock_,
//       props.host_query_reset_,
//       props.inline_uniform_block_,
//       props.memory_priority_features_,
//       props.multiview_,
//       props.protected_memory_,
//       props.sampler_ycbcr_conversion_,
//       props.scalar_block_layout_,
//       props.shader_atomic_int64_,
//       props.shader_draw_parameters_,
//       props.transform_feedback_,
//       props.uniform_buffer_standard_layout_,
//       props.variable_pointers_,
//       props.vertex_attribute_divisor_,
//       props.vulkan_memory_mode_,
//       props.ycbcr_image_arrays_
//       );
//  device.getFeatures2(&p);
//  props.features1_ = p.features;
//}
//
///*!
//  */
//inline
//void VulkanDeviceInfo::fetchLayerProperties(
//    const vk::PhysicalDevice& device) noexcept
//{
//  auto [result, properties] = device.enumerateDeviceLayerProperties();
//  ZISC_ASSERT(result == vk::Result::eSuccess,
//              "Vulkan device failed to get layer properties.");
//  layer_properties_list_.resize(properties.size());
//  for (std::size_t i = 0; i < properties.size(); ++i) {
//    layer_properties_list_[i].properties1_ = properties[i];
//  }
//}
//
///*!
//  */
//inline
//void VulkanDeviceInfo::fetchMemoryProperties(
//    const vk::PhysicalDevice& device) noexcept
//{
//  vk::PhysicalDeviceMemoryProperties2 p;
//  auto& props = memory_properties_;
//  link(p,
//       props.budget_
//       );
//  device.getMemoryProperties2(&p);
//  props.properties1_ = p.memoryProperties;
//}
//
///*!
//  */
//inline
//void VulkanDeviceInfo::fetchProperties(
//    const vk::PhysicalDevice& device) noexcept
//{
//  vk::PhysicalDeviceProperties2 p;
//  auto& props = properties_;
//  link(p,
//       props.blend_operation_advanced_,
//       props.conservative_rasterization_,
//       props.depth_stencil_resolve_,
//       props.descriptor_indexing_,
//       props.discard_rectangle_,
//       props.driver_,
//       props.external_memory_host_,
//       props.float_controls_,
//       props.fragment_density_map_,
//       props.id_properties_,
//       props.inline_uniform_block_,
//       props.maintenance3_,
//       props.multiview_,
//       props.pci_bus_info_,
//       props.point_clipping_,
//       props.protected_memory_,
//       props.push_descriptor_,
//       props.sample_locations_,
//       props.sampler_filter_minmax_,
//       props.subgroup_,
//       props.transform_feedback_,
//       props.vertex_attribute_divisor_
//       );
//  device.getProperties2(&p);
//  props.properties1_ = p.properties;
//}
//
///*!
//  */
//inline
//void VulkanDeviceInfo::fetchQueueFamilyProperties(
//    const vk::PhysicalDevice& device) noexcept
//{
//  // Query queue family properties
//  {
//    uint32b n = 0;
//    std::vector<vk::QueueFamilyProperties2> properties_list;
//    device.getQueueFamilyProperties2(&n, properties_list.data());
//
//    queue_family_properties_list_.resize(zisc::cast<std::size_t>(n));
//    properties_list.resize(zisc::cast<std::size_t>(n));
//    device.getQueueFamilyProperties2(&n, properties_list.data());
//
//    for (std::size_t i = 0; i < properties_list.size(); ++i) {
//      const auto& src = properties_list[i];
//      auto& dst = queue_family_properties_list_[i];
//      dst.properties1_ = src.queueFamilyProperties;
//    }
//  }
//}

} // namespace zinvul
