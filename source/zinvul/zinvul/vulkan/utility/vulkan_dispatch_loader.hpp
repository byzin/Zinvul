/*!
  \file vulkan_dispatch_loader.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_DISPATCH_LOADER_HPP
#define ZINVUL_VULKAN_DISPATCH_LOADER_HPP

// Standard C++ library
#include <memory>
#include <type_traits>
// Vulkan
#include <vulkan/vulkan.h>
// Zisc
#include "zisc/std_memory_resource.hpp"

namespace zinvulvk {

// Forward declaration
class DispatchLoaderDynamic;
class DynamicLoader;

} // namespace zinvulvk

namespace zinvul {

/*!
  \brief No brief description

  No detailed description.
  */
class VulkanDispatchLoader
{
 public:
  // Type aliases
  using LoaderImpl = zinvulvk::DispatchLoaderDynamic;
  using ConstLoaderImpl = std::add_const_t<LoaderImpl>;
  using LoaderImplPtr = std::add_pointer_t<LoaderImpl>;
  using ConstLoaderImplPtr = std::add_pointer_t<ConstLoaderImpl>;


  //! Initialize the dispatch loader
  VulkanDispatchLoader(zisc::pmr::memory_resource* mem_resource);

  //! Initialize the dispatch loader
  VulkanDispatchLoader(zisc::pmr::memory_resource* mem_resource,
                       PFN_vkGetInstanceProcAddr get_proc_addr);

  //! Copy a data
  VulkanDispatchLoader(const VulkanDispatchLoader& other) noexcept;

  //! Move a data
  VulkanDispatchLoader(VulkanDispatchLoader&& other) noexcept;

  //! Finalize the dispatch loader
  ~VulkanDispatchLoader() noexcept;


  //! Copy a data
  VulkanDispatchLoader& operator=(const VulkanDispatchLoader& other) noexcept;

  //! Move a data
  VulkanDispatchLoader& operator=(VulkanDispatchLoader&& other) noexcept;


  //! Check if the loader is available
  bool isAvailable() const noexcept;

  //! Return the underlying vulkan dispatch loader
  LoaderImplPtr loaderImpl() noexcept;

  //! Return the underlying vulkan dispatch loader
  ConstLoaderImplPtr loaderImpl() const noexcept;

  //! The dispatcher obtains the function pointers from the given device
  void set(const VkDevice& device);

  //! The dispatcher obtains the function pointers from the given instance
  void set(const VkInstance& instance);

 private:
  //! Copy a data
  void copy(const VulkanDispatchLoader& other) noexcept;

  //! Destroy all underlying data
  void destroy() noexcept;

  //! Initialize the dispatch loader with dynamic vulkan library
  void initialize();

  //! Initialize the dispatch loader
  void initialize(PFN_vkGetInstanceProcAddr get_proc_addr);

  //! Move a data
  void move(VulkanDispatchLoader& other) noexcept;


  zisc::pmr::memory_resource* mem_resource_ = nullptr;
  std::shared_ptr<zinvulvk::DynamicLoader> dynamic_loader_ = nullptr;
  LoaderImplPtr loader_impl_ = nullptr;
};

} // namespace zinvul

#include "vulkan_dispatch_loader-inl.hpp"

#endif // ZINVUL_VULKAN_DISPATCH_LOADER_HPP
