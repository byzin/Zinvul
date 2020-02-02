/*!
  \file cpu_sub_platform.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_sub_platform.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
CpuSubPlatform::CpuSubPlatform() noexcept
{
}

/*!
  \details No detailed description
  */
CpuSubPlatform::~CpuSubPlatform() noexcept
{
}

/*!
  \details No detailed description

  \param [in,out] platform_options No description.
  */
void CpuSubPlatform::initialize(PlatformOptions& /* platform_options */)
{
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType CpuSubPlatform::type() const noexcept
{
  return SubPlatformType::kCpu;
}

} // namespace zinvul
