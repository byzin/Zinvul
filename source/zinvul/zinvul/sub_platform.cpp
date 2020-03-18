/*!
  \file sub_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "sub_platform.hpp"
// Standard C++ library
#include <atomic>
#include <memory>
#include <utility>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "platform.hpp"
#include "platform_options.hpp"
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
SubPlatform::SubPlatform(Platform* platform) noexcept :
    ZinvulObject(platform->issueId()),
    platform_{platform}
{
}

/*!
  \details No detailed description
  */
SubPlatform::~SubPlatform() noexcept
{
}

/*!
  \details No detailed description
  */
void SubPlatform::destroy() noexcept
{
  destroyData();
  destroyObject();
}

/*!
  \details No detailed description

  \param [in,out] own No description.
  \param [in,out] platform_options No description.
  */
void SubPlatform::initialize(WeakPtr&& own, PlatformOptions& platform_options)
{
  // Clear the previous sub-platform data first
  destroy();

  initObject(nullptr, std::move(own));
  initData(platform_options);
}

/*!
  \details No detailed description

  \return No description
  */
bool SubPlatform::isDebugMode() const noexcept
{
  const bool mode = platform_->isDebugMode();
  return mode;
}

/*!
  \details No detailed description

  \return No description
  */
IdData SubPlatform::issueId() noexcept
{
  IdData id = platform_->issueId();
  return id;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* SubPlatform::memoryResource() noexcept
{
  zisc::pmr::memory_resource* mem_resource = platform_->memoryResource();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* SubPlatform::memoryResource() const noexcept
{
  const zisc::pmr::memory_resource* mem_resource = platform_->memoryResource();
  return mem_resource;
}

} // namespace zinvul
