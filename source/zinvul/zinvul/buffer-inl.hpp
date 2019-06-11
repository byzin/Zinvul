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
}

} // namespace zinvul

#endif // ZINVUL_BUFFER_INL_HPP
