/*!
  \file cpu_buffer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_BUFFER_INL_HPP
#define ZINVUL_CPU_BUFFER_INL_HPP

#include "cpu_buffer.hpp"
// Standard C++ library
#include <cstddef>
#include <cstring>
// Zisc
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
// Zinvul
#include "cpu_device.hpp"
#include "zinvul/buffer.hpp"

namespace zinvul {

/*!
  */
template <typename Type> inline
CpuBuffer<Type>::CpuBuffer(CpuDevice* device,
                           const BufferUsage usage_flag) noexcept :
    Buffer<Type>(usage_flag),
    buffer_{device->memoryResource()}
{
}

/*!
  */
template <typename Type> inline
CpuBuffer<Type>::CpuBuffer(CpuDevice* device,
                           const BufferUsage usage_flag,
                           const std::size_t size) noexcept :
    CpuBuffer(device, usage_flag)
{
  setSize(size);
}

/*!
  */
template <typename Type> inline
Type* CpuBuffer<Type>::data() noexcept
{
  return buffer_.data();
}

/*!
  */
template <typename Type> inline
const Type* CpuBuffer<Type>::data() const noexcept
{
  return buffer_.data();
}

/*!
  */
template <typename Type> inline
DeviceType CpuBuffer<Type>::deviceType() const noexcept
{
  return DeviceType::kCpu;
}

/*!
  */
template <typename Type> inline
std::size_t CpuBuffer<Type>::memoryUsage() const noexcept
{
  const std::size_t memory_usage = sizeof(Type) * size();
  return memory_usage;
}

/*!
  */
template <typename Type> inline
void CpuBuffer<Type>::read(Type* host_data) const noexcept
{
  ZISC_ASSERT(this->isHostReadable(), "The buffer isn't host readable.");
  const std::size_t count = sizeof(Type) * size();
  std::memcpy(host_data, data(), count);
}

/*!
  */
template <typename Type> inline
void CpuBuffer<Type>::setSize(const std::size_t size) noexcept
{
  buffer_.resize(size);
}

/*!
  */
template <typename Type> inline
std::size_t CpuBuffer<Type>::size() const noexcept
{
  return buffer_.size();
}

/*!
  */
template <typename Type> inline
void CpuBuffer<Type>::write(const Type* host_data) noexcept
{
  ZISC_ASSERT(this->isHostWritable(), "The buffer isn't host writable.");
  const std::size_t count = sizeof(Type) * size();
  std::memcpy(data(), host_data, count);
}

} // namespace zinvul

#endif // ZINVUL_CPU_BUFFER_INL_HPP
