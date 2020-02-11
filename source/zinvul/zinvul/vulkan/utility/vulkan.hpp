/*!
  \file vulkan.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_HPP
#define ZINVUL_VULKAN_HPP

// Zinvul
#include "zinvul/zinvul_config.hpp"

// Namespace
#define VULKAN_HPP_NAMESPACE zinvulvk 
// Disable the smart handler of the Vulkan-HPP
#define VULKAN_HPP_NO_SMART_HANDLE 1
// Dynamic loading
#define VK_NO_PROTOTYPES 1
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#if defined(ZINVUL_DYNAMIC_VULKAN_LOADING)
  #define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 1
#else // ZINVUL_DYNAMIC_VULKAN_LOADING
  #define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0
#endif // ZINVUL_DYNAMIC_VULKAN_LOADING
// Prohibit default dispatcher
#define VULKAN_HPP_DEFAULT_DISPATCHER int{0}
#define VULKAN_HPP_DEFAULT_DISPATCHER_TYPE int

// Vulkan
#include <vulkan/vulkan.hpp>

#endif // ZINVUL_VULKAN_HPP
