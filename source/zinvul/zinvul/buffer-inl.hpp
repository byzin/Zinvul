/*!
  \file buffer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
template <BufferType kBufferType, typename T> inline
Buffer<kBufferType, T>::Buffer(const BufferUsage usage_flag) noexcept :
    usage_flag_{usage_flag}
{
  initialize();
}

/*!
  */
template <BufferType kBufferType, typename T> inline
Buffer<kBufferType, T>::~Buffer() noexcept
{
}

/*!
  */
template <BufferType kBufferType, typename T> inline
constexpr BufferType Buffer<kBufferType, T>::bufferType() noexcept
{
  return kBufferType;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool Buffer<kBufferType, T>::isDestination() const noexcept
{
  return (usage_flag_ & BufferUsage::kTDst) == BufferUsage::kTDst;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool Buffer<kBufferType, T>::isDeviceBuffer() const noexcept
{
  return (usage_flag_ & BufferUsage::kDevice) == BufferUsage::kDevice;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool Buffer<kBufferType, T>::isHostBuffer() const noexcept
{
  return (usage_flag_ & BufferUsage::kHost) == BufferUsage::kHost;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool Buffer<kBufferType, T>::isHostReadable() const noexcept
{
  return isHostVisible() || isSource();
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool Buffer<kBufferType, T>::isHostVisible() const noexcept
{
  return isHostBuffer();
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool Buffer<kBufferType, T>::isHostWritable() const noexcept
{
  return isHostVisible() || isDestination();
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool Buffer<kBufferType, T>::isSource() const noexcept
{
  return (usage_flag_ & BufferUsage::kTSrc) == BufferUsage::kTSrc;
}

/*!
  */
template <BufferType kBufferType, typename T> template <typename DstType> inline
Buffer<kBufferType, DstType>* Buffer<kBufferType, T>::treatAs() noexcept
{
  using DstBuffer = Buffer<kBufferType, DstType>;
  DstBuffer* dst = zisc::treatAs<DstBuffer*>(this);
  return dst;
}

/*!
  */
template <BufferType kBufferType, typename T> template <typename DstType> inline
const Buffer<kBufferType, DstType>* Buffer<kBufferType, T>::treatAs() const noexcept
{
  using DstBuffer = Buffer<kBufferType, DstType>;
  const DstBuffer* dst = zisc::treatAs<const DstBuffer*>(this);
  return dst;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
BufferUsage Buffer<kBufferType, T>::usage() const noexcept
{
  return usage_flag_;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
void Buffer<kBufferType, T>::initialize() noexcept
{
  ZISC_ASSERT(!(isHostBuffer() && isDeviceBuffer()),
      "The flags \"kDevice\" and \"kHost\" aren't able to set to a same buffer");
}

} // namespace zinvul

#endif // ZINVUL_BUFFER_INL_HPP
