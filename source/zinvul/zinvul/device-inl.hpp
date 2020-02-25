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
template <DescriptorType kDescType, typename T>
UniqueBuffer<kDescType, T> makeBuffer(Device* device, const BufferUsage flag);

/*!
  \details No detailed description

  \tparam kDescType No description.
  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <DescriptorType kDescType, typename T> inline
UniqueBuffer<kDescType, T> Device::makeBuffer(const BufferUsage flag)
{
  auto buffer = zinvul::makeBuffer<kDescType, T>(this, flag);
  return buffer;
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <typename T> inline
UniqueStorageBuffer<T> Device::makeStorageBuffer(const BufferUsage flag)
{
  UniqueStorageBuffer<T> buffer = makeBuffer<DescriptorType::kStorage, T>(flag);
  return buffer;
}

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <typename T> inline
UniqueUniformBuffer<T> Device::makeUniformBuffer(const BufferUsage flag)
{
  UniqueUniformBuffer<T> buffer = makeBuffer<DescriptorType::kUniform, T>(flag);
  return buffer;
}

/*!
  \details No detailed description

  \return No description
  */
inline
zisc::pmr::memory_resource* Device::memoryResource() noexcept
{
  ZISC_ASSERT(mem_resource_ != nullptr, "The memory resource is null.");
  return mem_resource_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const zisc::pmr::memory_resource* Device::memoryResource() const noexcept
{
  ZISC_ASSERT(mem_resource_ != nullptr, "The memory resource is null.");
  return mem_resource_;
}

} // namespace zinvul

#endif // ZINVUL_DEVICE_INL_HPP
