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

// VMA
#if defined(ZINVUL_ENABLE_VMA_IMPLEMENTATION)
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_IMPLEMENTATION 1

// VMA Debug
#if defined(Z_DEBUG_MODE)
#define VMA_DEBUG_INITIALIZE_ALLOCATIONS 1
#define VMA_DEBUG_MARGIN 32
#define VMA_DEBUG_DETECT_CORRUPTION 1
#else
static_assert(false);
#endif // Z_DEBUG_MODE

#endif // ZINVUL_ENABLE_VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
// Zinvul
#include "zinvul/zinvul_config.hpp"
