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
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "platform_options.hpp"
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
SubPlatform::SubPlatform() noexcept
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
  mem_resource_ = nullptr;
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void SubPlatform::initialize(PlatformOptions& platform_options)
{
  // Clear the previous sub-platform data first
  destroy();

  mem_resource_ = platform_options.memoryResource();
  id_count_.store(0);

  initData(platform_options);
}

} // namespace zinvul
