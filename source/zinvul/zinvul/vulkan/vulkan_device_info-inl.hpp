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
#include <cstddef>
#include <vector>
// Vulkan
#include <vulkan/vulkan.hpp>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::extensionPropertiesList() noexcept
    -> zisc::pmr::vector<ExtensionProperties>&
{
  return extension_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::extensionPropertiesList() const noexcept
    -> const zisc::pmr::vector<ExtensionProperties>&
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

  \return No description
  */
inline
auto VulkanDeviceInfo::layerPropertiesList() noexcept
    -> zisc::pmr::vector<LayerProperties>&
{
  return layer_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::layerPropertiesList() const noexcept
    -> const zisc::pmr::vector<LayerProperties>&
{
  return layer_properties_list_;
}

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
auto VulkanDeviceInfo::toolPropertiesList() noexcept
    -> zisc::pmr::vector<ToolProperties>&
{
  return tool_properties_list_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDeviceInfo::toolPropertiesList() const noexcept
    -> const zisc::pmr::vector<ToolProperties>&
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
auto VulkanDeviceInfo::queueFamilyPropertiesList() const noexcept
    -> const zisc::pmr::vector<QueueFamilyProperties>&
{
  return queue_family_properties_list_;
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_DEVICE_INFO_INL_HPP
