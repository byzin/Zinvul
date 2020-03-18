/*!
  \file device-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DEVICE_INL_HPP
#define ZINVUL_DEVICE_INL_HPP

#include "device.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "buffer.hpp"
#include "zinvul_config.hpp"

namespace zinvul {

// Forward declaration
template <typename T>
SharedBuffer<T> makeBuffer(Device* device, const BufferUsage flag);

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <typename T> inline
SharedBuffer<T> Device::makeBuffer(const BufferUsage flag)
{
  auto buffer = zinvul::makeBuffer<T>(this, flag);
  return buffer;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const DeviceInfo& Device::deviceInfo() const noexcept
{
  return *device_info_;
}

} // namespace zinvul

#endif // ZINVUL_DEVICE_INL_HPP
