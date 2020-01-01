/*!
  \file zinvul_initialization.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_INITIALIZATION_HPP
#define ZINVUL_ZINVUL_INITIALIZATION_HPP

// Standard C++ library
#include <array>
// Zinvul
#include "zinvul/cppcl/atomic.hpp"
#include "zinvul/cppcl/utility.hpp"
#include "zinvul/zinvul_config.hpp"

// Vulkan initialization

#if defined(ZINVUL_ENABLE_VULKAN_BACKEND)

#if defined(VMA_IMPLEMENTATION)
static_assert(false, "The macro 'VMA_IMPLEMENTATION' is already defined.");
#endif // VMA_IMPLEMENTATION

#if defined(AMD_VULKAN_MEMORY_ALLOCATOR_H)
static_assert(false, "The header 'vk_mem_alloc.h' is already loaded.");
#endif // AMD_VULKAN_MEMORY_ALLOCATOR_H

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#endif // ZINVUL_ENABLE_VULKAN_BACKEND

thread_local std::array<zinvul::uint32b, 3> zinvul::cl::clinner::WorkGroup::work_group_id_;
thread_local std::array<zinvul::uint32b, 3> zinvul::cl::clinner::WorkGroup::work_group_size_;

#endif // ZINVUL_ZINVUL_INITIALIZATION_HPP
