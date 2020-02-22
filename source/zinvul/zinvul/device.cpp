/*!
  \file device.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "device.hpp"
// Standard C++ library
#include <memory>
#include <utility>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in,out] mem_resource No description.
  */
Device::Device(zisc::pmr::memory_resource* mem_resource) noexcept :
    mem_resource_{mem_resource}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
Device::Device(Device&& other) noexcept :
    mem_resource_{nullptr}
{
  std::swap(mem_resource_, other.mem_resource_);
}

/*!
  \details No detailed description
  */
Device::~Device() noexcept
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
Device& Device::operator=(Device&& other) noexcept
{
  std::swap(mem_resource_, other.mem_resource_);
  return *this;
}

/*!
  \details No detailed description
  */
void Device::destroy() noexcept
{
  destroyData();
  mem_resource_ = nullptr;
}

} // namespace zinvul
