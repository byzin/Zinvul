/*!
  \file zinvul_initialization.hpp
  \author Sho Ikeda
  */

#ifndef ZINVUL_ZINVUL_INITIALIZATION_HPP
#define ZINVUL_ZINVUL_INITIALIZATION_HPP

// Standard C++ library
#include <array>
// Zisc
#include "zisc/spin_lock_mutex.hpp"
// Zinvul
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

namespace zinvul {

namespace cl {

extern thread_local std::array<uint32b, 3> __work_group_id;
thread_local std::array<uint32b, 3> __work_group_id;

extern thread_local std::array<uint32b, 3> __work_group_size;
thread_local std::array<uint32b, 3> __work_group_size;

extern thread_local zisc::SpinLockMutex* __mutex;
thread_local zisc::SpinLockMutex* __mutex;

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_ZINVUL_INITIALIZATION_HPP
