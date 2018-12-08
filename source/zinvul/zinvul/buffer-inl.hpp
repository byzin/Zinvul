/*!
  \file buffer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUFFER_INL_HPP
#define ZINVUL_BUFFER_INL_HPP

#include "buffer.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename T> inline
Buffer<T>::Buffer(const BufferUsage usage_flag) noexcept :
    usage_flag_{usage_flag}
{
  initialize();
}

/*!
  */
template <typename T> inline
Buffer<T>::~Buffer() noexcept
{
}

/*!
  */
template <typename T> inline
bool Buffer<T>::isDestination() const noexcept
{
  return (usage_flag_ & BufferUsage::kDst) == BufferUsage::kDst;
}

/*!
  */
template <typename T> inline
bool Buffer<T>::isDeviceBuffer() const noexcept
{
  return (usage_flag_ & BufferUsage::kDevice) == BufferUsage::kDevice;
}

/*!
  */
template <typename T> inline
bool Buffer<T>::isHostBuffer() const noexcept
{
  return (usage_flag_ & BufferUsage::kHost) == BufferUsage::kHost;
}

/*!
  */
template <typename T> inline
bool Buffer<T>::isHostReadable() const noexcept
{
  return isHostVisible() || isSource();
}

/*!
  */
template <typename T> inline
bool Buffer<T>::isHostVisible() const noexcept
{
  return isHostBuffer();
}

/*!
  */
template <typename T> inline
bool Buffer<T>::isHostWritable() const noexcept
{
  return isHostVisible() || isDestination();
}

/*!
  */
template <typename T> inline
bool Buffer<T>::isSource() const noexcept
{
  return (usage_flag_ & BufferUsage::kSrc) == BufferUsage::kSrc;
}

/*!
  */
template <typename T> inline
BufferUsage Buffer<T>::usage() const noexcept
{
  return usage_flag_;
}

/*!
  */
template <typename T> inline
void Buffer<T>::initialize() noexcept
{
  ZISC_ASSERT(!(isHostBuffer() && isDeviceBuffer()),
      "The flags \"kDevice\" and \"kHost\" aren't able to set to a same buffer");
  ZISC_ASSERT(!(isSource() && isDestination()),
      "The flags \"kSrc\" and \"kDst\" aren't able to set to a same buffer");
}

} // namespace zinvul

#endif // ZINVUL_BUFFER_INL_HPP
