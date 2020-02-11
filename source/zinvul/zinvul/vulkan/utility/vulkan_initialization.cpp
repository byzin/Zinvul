/*!
  \file vulkan_initialization.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Vulkan
#if defined(ZINVUL_ENABLE_VMA_IMPLEMENTATION_BUILDING)
#define VMA_IMPLEMENTATION
#endif // ZINVUL_ENABLE_VMA_IMPLEMENTATION_BUILDING
#include "vk_mem_alloc.h"
// Zinvul
#include "zinvul/zinvul_config.hpp"
