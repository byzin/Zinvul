/*!
  \file vulkan_device_info.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_DEVICE_INFO_HPP
#define ZINVUL_VULKAN_DEVICE_INFO_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul/device_info.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class VulkanDispatchLoader;

/*!
  \brief Properties and features of a vulkan device

  No detailed description.
  */
class VulkanDeviceInfo : public DeviceInfo
{
 public:
  //! The Vendor IDs of vulkan device
  enum class VendorId : uint32b
  {
    kAmd = 0x1002, // AMD
    kImgTec = 0x1010, // ImgTec
    kNvidia = 0x10de, // NVIDIA
    kArm = 0x13b5, // ARM
    kQualcomm = 0x5143, // Qualcomm
    kIntel = 0x8086 // INTEL
  };

  //! Properties of a device
  struct Properties
  {
    VkPhysicalDeviceProperties properties1_;
    VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT blend_operation_advanced_;
    VkPhysicalDeviceConservativeRasterizationPropertiesEXT conservative_rasterization_;
    VkPhysicalDeviceDepthStencilResolveProperties depth_stencil_resolve_;
    VkPhysicalDeviceDescriptorIndexingProperties descriptor_indexing_;
    VkPhysicalDeviceDiscardRectanglePropertiesEXT discard_rectangle_;
    VkPhysicalDeviceDriverProperties driver_;
    VkPhysicalDeviceExternalMemoryHostPropertiesEXT external_memory_host_;
    VkPhysicalDeviceFloatControlsProperties float_controls_;
    VkPhysicalDeviceFragmentDensityMapPropertiesEXT fragment_density_map_;
    VkPhysicalDeviceIDProperties id_properties_;
    VkPhysicalDeviceInlineUniformBlockPropertiesEXT inline_uniform_block_;
    VkPhysicalDeviceLineRasterizationPropertiesEXT line_rasterization_;
    VkPhysicalDeviceMaintenance3Properties maintenance3_;
    VkPhysicalDeviceMultiviewProperties multiview_;
    VkPhysicalDevicePCIBusInfoPropertiesEXT pci_bus_info_;
    VkPhysicalDevicePerformanceQueryPropertiesKHR performance_query_;
    VkPhysicalDevicePointClippingProperties point_clipping_;
    VkPhysicalDeviceProtectedMemoryProperties protected_memory_;
    VkPhysicalDevicePushDescriptorPropertiesKHR push_descriptor_;
    VkPhysicalDeviceSampleLocationsPropertiesEXT sample_locations_;
    VkPhysicalDeviceSamplerFilterMinmaxProperties sampler_filter_minmax_;
    VkPhysicalDeviceSubgroupProperties subgroup_;
    VkPhysicalDeviceSubgroupSizeControlPropertiesEXT subgroup_size_control_;
    VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT texel_buffer_alignment_;
    VkPhysicalDeviceTimelineSemaphoreProperties timeline_semaphore_;
    VkPhysicalDeviceTransformFeedbackPropertiesEXT transform_feedback_;
    VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT vertex_attribute_divisor_;
    VkPhysicalDeviceVulkan11Properties vulkan11_;
    VkPhysicalDeviceVulkan12Properties vulkan12_;
  };

  //! Features of a device
  struct Features
  {
    VkPhysicalDeviceFeatures features1_;
    VkPhysicalDevice16BitStorageFeatures b16bit_storage_;
    VkPhysicalDevice8BitStorageFeatures b8bit_storage_;
    VkPhysicalDeviceASTCDecodeFeaturesEXT astc_decode_;
    VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT blend_operation_advanced_;
    VkPhysicalDeviceBufferDeviceAddressFeatures buffer_device_address_;
    VkPhysicalDeviceConditionalRenderingFeaturesEXT conditional_rendering_;
    VkPhysicalDeviceDepthClipEnableFeaturesEXT depth_clip_enabled_;
    VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_;
    VkPhysicalDeviceFragmentDensityMapFeaturesEXT fragment_density_map_;
    VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT fragment_shader_inter_lock_;
    VkPhysicalDeviceHostQueryResetFeatures host_query_reset_;
    VkPhysicalDeviceImagelessFramebufferFeatures imageless_framebuffer_;
    VkPhysicalDeviceIndexTypeUint8FeaturesEXT index_type_uint8_;
    VkPhysicalDeviceInlineUniformBlockFeaturesEXT inline_uniform_block_;
    VkPhysicalDeviceLineRasterizationFeaturesEXT line_rasterization_;
    VkPhysicalDeviceMemoryPriorityFeaturesEXT memory_priority_features_;
    VkPhysicalDeviceMultiviewFeatures multiview_;
    VkPhysicalDevicePerformanceQueryFeaturesKHR performance_query_;
    VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR pipeline_executable_properties_;
    VkPhysicalDeviceProtectedMemoryFeatures protected_memory_;
    VkPhysicalDeviceSamplerYcbcrConversionFeatures sampler_ycbcr_conversion_;
    VkPhysicalDeviceScalarBlockLayoutFeaturesEXT scalar_block_layout_;
    VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures depth_stencil_layouts_;
    VkPhysicalDeviceShaderAtomicInt64FeaturesKHR shader_atomic_int64_;
    VkPhysicalDeviceShaderClockFeaturesKHR shader_clock_;
    VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT demote_to_helper_invocation_;
    VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_;
    VkPhysicalDeviceShaderFloat16Int8FeaturesKHR shader_float16_int8_;
    VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures shader_subgroup_extended_types_;
    VkPhysicalDeviceSubgroupSizeControlFeaturesEXT subgroup_size_control_;
    VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT texel_buffer_alignment_;
    VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT texture_compression_astchdr_;
    VkPhysicalDeviceTimelineSemaphoreFeatures timeline_semaphore_;
    VkPhysicalDeviceTransformFeedbackFeaturesEXT transform_feedback_;
    VkPhysicalDeviceUniformBufferStandardLayoutFeatures uniform_buffer_standard_layout_;
    VkPhysicalDeviceVariablePointersFeatures variable_pointers_;
    VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT vertex_attribute_divisor_;
    VkPhysicalDeviceVulkan11Features vulkan11_;
    VkPhysicalDeviceVulkan12Features vulkan12_;
    VkPhysicalDeviceVulkanMemoryModelFeatures vulkan_memory_mode_;
    VkPhysicalDeviceYcbcrImageArraysFeaturesEXT ycbcr_image_arrays_;
  };

  //! Memory properties of a device
  struct MemoryProperties
  {
    VkPhysicalDeviceMemoryProperties properties1_;
    VkPhysicalDeviceMemoryBudgetPropertiesEXT budget_;
  };

  //! Queue family properties of a device
  struct QueueFamilyProperties
  {
    VkQueueFamilyProperties properties1_;
  };

  //! Device tool properties
  struct ToolProperties
  {
    VkPhysicalDeviceToolPropertiesEXT properties1_;
  };


  //! Initialize the device info
  VulkanDeviceInfo(zisc::pmr::memory_resource* mem_resource) noexcept;

  //! Move a data
  VulkanDeviceInfo(VulkanDeviceInfo&& other) noexcept;

  //! Finalize the device info
  ~VulkanDeviceInfo() noexcept override;


  //! Move a data
  VulkanDeviceInfo& operator=(VulkanDeviceInfo&& other) noexcept;


  //! Return the amount of actual device memory currently available in bytes
  std::size_t availableMemory(const std::size_t heap_index) const noexcept override;

  //! Return the underlying vulkan physical device
  const VkPhysicalDevice& device() const noexcept;

  //! Return extension properties list of the device
  zisc::pmr::vector<VkExtensionProperties>& extensionPropertiesList() noexcept;

  //! Return extension properties list of the device
  const zisc::pmr::vector<VkExtensionProperties>& extensionPropertiesList() const noexcept;

  //! Return features of the device
  Features& features() noexcept;

  //! Return features of the device
  const Features& features() const noexcept;

  //! Fetch device info from a physical device
  void fetch(const VkPhysicalDevice& vdevice,
             const VulkanDispatchLoader& dispatcher);

  //! Initialize a property with the given type
  template <typename CppType, typename CType>
  static CppType& initProp(CType& prop) noexcept;

  //! Return layer properties list of the device
  zisc::pmr::vector<VkLayerProperties>& layerPropertiesList() noexcept;

  //! Return layer properties list of the device
  const zisc::pmr::vector<VkLayerProperties>& layerPropertiesList() const noexcept;

  template <typename Type1, typename Type2, typename ...Types>
  static void link(Type1&& value1, Type2&& value2, Types&&... values) noexcept;

  //! Return the maximum size of an allocation
  std::size_t maxAllocationSize() const noexcept override;

  //! Return the maximum work group count
  std::array<uint32b, 3> maxWorkGroupCount() const noexcept override;

  //! Return memory properties of the device
  MemoryProperties& memoryProperties() noexcept;

  //! Return memory properties of the device
  const MemoryProperties& memoryProperties() const noexcept;

  //! Return the device name
  std::string_view name() const noexcept override;

  //! Return the number of heaps of the device local
  std::size_t numOfHeaps() const noexcept override;

  //! Return properties of the device
  Properties& properties() noexcept;

  //! Return properties of the device
  const Properties& properties() const noexcept;

  //! Return tool properties list of the device
  zisc::pmr::vector<ToolProperties>& toolPropertiesList() noexcept;

  //! Return tool properties list of the device
  const zisc::pmr::vector<ToolProperties>& toolPropertiesList() const noexcept;

  //! Return the amount of actual device memory in bytes
  std::size_t totalMemory(const std::size_t heap_index) const noexcept override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

  //! Return queue family properties list of the device
  zisc::pmr::vector<QueueFamilyProperties>& queueFamilyPropertiesList() noexcept;

  //! Return queue family properties list of the device
  const zisc::pmr::vector<QueueFamilyProperties>& queueFamilyPropertiesList() const noexcept;

  //! Return the vendor name
  std::string_view vendorName() const noexcept override;

  //! Return the local work group size of the device
  uint32b workGroupSize() const noexcept override;

 private:
  //! Fetch extension properties from a physical device
  void fetchExtensionProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch features from a physical device
  void fetchFeatures(const VulkanDispatchLoader& dispatcher);

  //! Fetch layer properties from a physical device
  void fetchLayerProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch memory properties from a physical device
  void fetchMemoryProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch properties from a physical device
  void fetchProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch tool properties from a physical device
  void fetchToolProperties(const VulkanDispatchLoader& dispatcher);

  //! Fetch queue family properties from a physical device
  void fetchQueueFamilyProperties(const VulkanDispatchLoader& dispatcher);

  //! Find the indices of device load heaps
  void findDeviceLocalHeaps() noexcept;

  //! Set the vendor name with the given vendor ID
  void setVendorNameFromId(const uint32b vendor_id) noexcept;


  zisc::pmr::vector<VkExtensionProperties> extension_properties_list_;
  zisc::pmr::vector<VkLayerProperties> layer_properties_list_;
  zisc::pmr::vector<QueueFamilyProperties> queue_family_properties_list_;
  zisc::pmr::vector<ToolProperties> tool_properties_list_;
  zisc::pmr::vector<std::size_t> device_local_index_list_;
  zisc::pmr::string vendor_name_;
  VkPhysicalDevice device_;
  Properties properties_;
  Features features_;
  MemoryProperties memory_properties_;
};

} // namespace zinvul

#include "vulkan_device_info-inl.hpp"

#endif // ZINVUL_VULKAN_DEVICE_INFO_HPP
