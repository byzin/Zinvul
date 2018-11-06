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
template <typename Type> inline
Buffer<Type>::Buffer(const BufferUsage usage_flag) noexcept :
    usage_flag_{usage_flag}
{
}

/*!
  */
template <typename Type> inline
Buffer<Type>::~Buffer() noexcept
{
}

/*!
  */
template <typename Type> inline
bool Buffer<Type>::isHostReadable() const noexcept
{
  return usage() == BufferUsage::kDeviceToHost;
}

/*!
  */
template <typename Type> inline
bool Buffer<Type>::isHostWritable() const noexcept
{
  return usage() == BufferUsage::kHostToDevice;
}

/*!
  */
template <typename Type> inline
BufferUsage Buffer<Type>::usage() const noexcept
{
  return usage_flag_;
}

} // namespace zinvul

#endif // ZINVUL_BUFFER_INL_HPP
