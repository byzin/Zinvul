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
#include "sub_platform.hpp"
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"
#include "utility/zinvul_object.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
Device::Device(IdData&& id) noexcept : ZinvulObject{std::move(id)}
{
}

/*!
  \details No detailed description
  */
Device::~Device() noexcept
{
}

/*!
  \details No detailed description
  */
void Device::destroy() noexcept
{
  destroyData();
  device_info_ = nullptr;
  destroyObject();
}

/*!
  \details No detailed description

  \param [in,out] parent No description.
  \param [in,out] own No description.
  */
void Device::initialize(ZinvulObject::SharedPtr&& parent,
                        WeakPtr&& own,
                        const DeviceInfo& device_info)
{
  //! Clear the previous device data first
  destroy();

  initObject(std::move(parent), std::move(own));
  device_info_ = std::addressof(device_info);
  initData();
}

} // namespace zinvul
