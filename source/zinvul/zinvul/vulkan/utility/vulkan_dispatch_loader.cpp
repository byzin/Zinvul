/*!
  \file vulkan_dispatch_loader.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "vulkan_dispatch_loader.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "vulkan.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
VulkanDispatchLoader::VulkanDispatchLoader(
    zisc::pmr::memory_resource* mem_resource) :
        mem_resource_{mem_resource}
{
  initialize();
}

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  \param [in] get_proc_addr No description.
  */
VulkanDispatchLoader::VulkanDispatchLoader(
    zisc::pmr::memory_resource* mem_resource,
    PFN_vkGetInstanceProcAddr get_proc_addr) :
        mem_resource_{mem_resource}
{
  initialize(get_proc_addr);
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
VulkanDispatchLoader::VulkanDispatchLoader(const VulkanDispatchLoader& other) noexcept
{
  copy(other);
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
VulkanDispatchLoader::VulkanDispatchLoader(VulkanDispatchLoader&& other) noexcept
{
  move(other);
}

/*!
  \details No detailed description
  */
VulkanDispatchLoader::~VulkanDispatchLoader() noexcept
{
  destroy();
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
VulkanDispatchLoader& VulkanDispatchLoader::operator=(
    const VulkanDispatchLoader& other) noexcept
{
  copy(other);
  return *this;
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
VulkanDispatchLoader& VulkanDispatchLoader::operator=(
    VulkanDispatchLoader&& other) noexcept
{
  move(other);
  return *this;
}

/*!
  \details No detailed description

  \param [in] device No description.
  */
void VulkanDispatchLoader::set(const VkDevice& device)
{
  LoaderImplPtr impl = loaderImpl();
  if (impl)
    impl->init(zinvulvk::Device{device});
}

/*!
  \details No detailed description

  \param [in] instance No description.
  */
void VulkanDispatchLoader::set(const VkInstance& instance)
{
  LoaderImplPtr impl = loaderImpl();
  if (impl)
    impl->init(zinvulvk::Instance{instance});
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
void VulkanDispatchLoader::copy(const VulkanDispatchLoader& other) noexcept
{
  destroy();
  mem_resource_ = const_cast<zisc::pmr::memory_resource*>(other.mem_resource_);
  dynamic_loader_ = other.dynamic_loader_;
  if (other.isAvailable())
  {
    zisc::pmr::polymorphic_allocator<LoaderImpl> alloc{mem_resource_};
    loader_impl_ = alloc.allocate(1);
    alloc.construct(loader_impl_, *other.loader_impl_);
  }
}

/*!
  \details No detailed description
  */
void VulkanDispatchLoader::destroy() noexcept
{
  if (isAvailable()) {
    std::destroy_at(loader_impl_);
    zisc::pmr::polymorphic_allocator<LoaderImpl> alloc{mem_resource_};
    alloc.deallocate(loader_impl_, 1);
  }
  loader_impl_ = nullptr;
  dynamic_loader_.reset();
  mem_resource_ = nullptr;
}

/*!
  \details No detailed description
  */
void VulkanDispatchLoader::initialize()
{
  PFN_vkGetInstanceProcAddr get_proc_addr = nullptr;
#if defined(ZINVUL_DYNAMIC_VULKAN_LOADING)
  ZISC_ASSERT(mem_resource_ != nullptr, "The memory resource is null.");
  {
    using Loader = zinvulvk::DynamicLoader;
    zisc::pmr::polymorphic_allocator<Loader> alloc{mem_resource_};
    dynamic_loader_ = std::allocate_shared<Loader>(alloc);
  }
  if (!dynamic_loader_->success()) {
    //! \todo Throw an exception
    ZISC_ASSERT(!dynamic_loader_->success(), "Vulkan library loading failed.");
  }
  ZISC_ASSERT(!dynamic_loader_->success(), "Debug.");
#else // ZINVUL_DYNAMIC_VULKAN_LOADING
  get_proc_addr = ::vkGetInstanceProcAddr;
#endif // ZINVUL_DYNAMIC_VULKAN_LOADING
  initialize(get_proc_addr);
}

/*!
  \details No detailed description

  \param [in] get_proc_addr No description.
  */
void VulkanDispatchLoader::initialize(PFN_vkGetInstanceProcAddr get_proc_addr)
{
  ZISC_ASSERT(mem_resource_ != nullptr, "The memory resource is null.");
  if (get_proc_addr != nullptr) {
    zisc::pmr::polymorphic_allocator<LoaderImpl> alloc{mem_resource_};
    loader_impl_ = alloc.allocate(1);
    alloc.construct(loader_impl_, get_proc_addr);
  }
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
void VulkanDispatchLoader::move(VulkanDispatchLoader& other) noexcept
{
  std::swap(mem_resource_, other.mem_resource_);
  dynamic_loader_ = std::move(other.dynamic_loader_);
  std::swap(loader_impl_, other.loader_impl_);
}

} // namespace zinvul
