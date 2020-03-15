/*!
  \file vulkan_device_info-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_DEVICE_INFO_INL_HPP
#define ZINVUL_VULKAN_DEVICE_INFO_INL_HPP

#include "vulkan_device_info.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
const VkPhysicalDevice& VulkanDeviceInfo::device() const noexcept
{
  return device_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::vector<VkExtensionProperties>&
VulkanDeviceInfo::extensionPropertiesList() noexcept
{
  return extension_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::vector<VkExtensionProperties>&
VulkanDeviceInfo::extensionPropertiesList() const noexcept
{
  return extension_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::features() noexcept -> Features&
{
  return features_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::features() const noexcept -> const Features&
{
  return features_;
}

/*!
  \details No detailed description

  \param [in] number No description.
  \return No description
  */
inline
std::size_t VulkanDeviceInfo::getDeviceHeapIndex(const std::size_t number) const noexcept
{
  const std::size_t index = device_local_index_list_[number];
  return index;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
std::size_t VulkanDeviceInfo::getDeviceHeapNumber(const std::size_t index) const noexcept
{
  const auto it = std::find(device_local_index_list_.begin(),
                            device_local_index_list_.end(),
                            index);
  const std::size_t number = std::distance(device_local_index_list_.begin(),
                                           it);
  return number;
}

/*!
  \details No detailed description

  \tparam CppType No description.
  \tparam CType No description.
  \param [in] prop No description.
  \return No description
  */
template <typename CppType, typename CType> inline
CppType& VulkanDeviceInfo::initProp(CType& prop) noexcept
{
  static_assert(std::is_convertible_v<CppType, CType>,
                "The CppType isn't convertible to CType.");
  // Initialize
  CppType data{};
  prop = zisc::cast<CType>(data);
  //
  using CppTypePtr = std::add_pointer_t<CppType>;
  CppType& p = *zisc::treatAs<CppTypePtr>(std::addressof(prop));
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::vector<VkLayerProperties>&
VulkanDeviceInfo::layerPropertiesList() noexcept
{
  return layer_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::vector<VkLayerProperties>&
VulkanDeviceInfo::layerPropertiesList() const noexcept
{
  return layer_properties_list_;
}

/*!
  \details No detailed description

  \tparam Type1 No description.
  \tparam Type2 No description.
  \tparam Types No description.
  \param [in,out] value1 No description.
  \param [in,out] value2 No description.
  \param [in,out] values No description.
  */
template <typename Type1, typename Type2, typename ...Types> inline
void VulkanDeviceInfo::link(Type1&& value1,
                            Type2&& value2,
                            Types&&... values) noexcept
{
  constexpr std::size_t num_of_rests = 1 + sizeof...(Types);
  value1.pNext = std::addressof(value2);
  if constexpr (1 < num_of_rests) {
    link(value2, values...);
  }
  else {
    value2.pNext = nullptr;
  }
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::memoryProperties() noexcept
    -> MemoryProperties&
{
  return memory_properties_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::memoryProperties() const noexcept
    -> const MemoryProperties&
{
  return memory_properties_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::properties() noexcept -> Properties&
{
  return properties_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::properties() const noexcept -> const Properties&
{
  return properties_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::toolPropertiesList() noexcept ->
    zisc::pmr::vector<ToolProperties>&
{
  return tool_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::toolPropertiesList() const noexcept ->
const zisc::pmr::vector<ToolProperties>&
{
  return tool_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::queueFamilyPropertiesList() noexcept
    -> zisc::pmr::vector<QueueFamilyProperties>&
{
  return queue_family_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::vendorId() const noexcept -> VendorId
{
  return vendor_id_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::queueFamilyPropertiesList() const noexcept
    -> const zisc::pmr::vector<QueueFamilyProperties>&
{
  return queue_family_properties_list_;
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_DEVICE_INFO_INL_HPP
