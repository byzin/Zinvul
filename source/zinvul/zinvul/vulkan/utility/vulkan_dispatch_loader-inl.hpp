/*!
  \file vulkan_dispatch_loader-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_DISPATCH_LOADER_INL_HPP
#define ZINVUL_VULKAN_DISPATCH_LOADER_INL_HPP

#include "vulkan_dispatch_loader.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
bool VulkanDispatchLoader::isAvailable() const noexcept
{
  const bool result = loaderImpl() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDispatchLoader::loaderImpl() noexcept -> LoaderImplPtr
{
  return loader_impl_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto VulkanDispatchLoader::loaderImpl() const noexcept -> ConstLoaderImplPtr
{
  return loader_impl_;
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_DISPATCH_LOADER_INL_HPP
