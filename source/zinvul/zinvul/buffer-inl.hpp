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
bool Buffer<T>::isHostReadable() const noexcept
{
  return usage() == BufferUsage::kDeviceToHost;
}

/*!
  */
template <typename T> inline
bool Buffer<T>::isHostWritable() const noexcept
{
  return usage() == BufferUsage::kHostToDevice;
}

/*!
  */
template <typename T> inline
BufferUsage Buffer<T>::usage() const noexcept
{
  return usage_flag_;
}

} // namespace zinvul

#endif // ZINVUL_BUFFER_INL_HPP
