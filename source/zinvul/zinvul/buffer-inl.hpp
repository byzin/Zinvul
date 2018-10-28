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
Buffer<Type>::Buffer(const int usage_flags) noexcept :
    usage_flags_{zisc::cast<int32b>(usage_flags)}
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
  const bool flag = (usageFlags() & kDeviceToHost) == kDeviceToHost;
  return flag;
}

/*!
  */
template <typename Type> inline
bool Buffer<Type>::isHostWritable() const noexcept
{
  const bool flag = (usageFlags() & kHostToDevice) == kHostToDevice;
  return flag;
}

/*!
  */
template <typename Type> inline
int Buffer<Type>::usageFlags() const noexcept
{
  return zisc::cast<int>(usage_flags_);
}

} // namespace zinvul

#endif // ZINVUL_BUFFER_INL_HPP
