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
template <typename T> inline
CpuBuffer<T>::CpuBuffer(CpuDevice* device,
                        const BufferUsage usage_flag) noexcept :
    Buffer<T>(usage_flag),
    device_{device},
    buffer_{device->memoryResource()}
{
}

/*!
  */
template <typename T> inline
CpuBuffer<T>::CpuBuffer(CpuDevice* device,
                        const BufferUsage usage_flag,
                        const std::size_t size) noexcept :
    CpuBuffer(device, usage_flag)
{
  setSize(size);
}

/*!
  */
template <typename T> inline
auto CpuBuffer<T>::buffer() noexcept -> zisc::pmr::vector<Type>&
{
  return buffer_;
}

/*!
  */
template <typename T> inline
auto CpuBuffer<T>::buffer() const noexcept -> const zisc::pmr::vector<Type>&
{
  return buffer_;
}

/*!
  */
template <typename T> inline
void CpuBuffer<T>::copyTo(Buffer<Type>* dst,
                          const std::size_t count,
                          const std::size_t src_offset,
                          const std::size_t dst_offset,
                          const uint32b queue_index) const noexcept
{
  dst->write(data() + src_offset, count, dst_offset, queue_index);
}

/*!
  */
template <typename T> inline
auto CpuBuffer<T>::data() noexcept -> Type*
{
  return buffer_.data();
}

/*!
  */
template <typename T> inline
auto CpuBuffer<T>::data() const noexcept -> const Type*
{
  return buffer_.data();
}

/*!
  */
template <typename T> inline
DeviceType CpuBuffer<T>::deviceType() const noexcept
{
  return DeviceType::kCpu;
}

/*!
  */
template <typename T> inline
std::size_t CpuBuffer<T>::memoryUsage() const noexcept
{
  const std::size_t memory_usage = sizeof(Type) * size();
  return memory_usage;
}

/*!
  */
template <typename T> inline
void CpuBuffer<T>::read(Type* host_data,
                        const std::size_t count,
                        const std::size_t offset,
                        const uint32b) const noexcept
{
  ZISC_ASSERT(this->isHostReadable(), "The buffer isn't host readable.");
  const std::size_t s = sizeof(Type) * count;
  std::memcpy(host_data, data() + offset, s);
}

/*!
  */
template <typename T> inline
void CpuBuffer<T>::setSize(const std::size_t size) noexcept
{
  device_->deallocate(this);
  device_->allocate(size, this);
}

/*!
  */
template <typename T> inline
std::size_t CpuBuffer<T>::size() const noexcept
{
  return buffer_.size();
}

/*!
  */
template <typename T> inline
void CpuBuffer<T>::write(const Type* host_data,
                         const std::size_t count,
                         const std::size_t offset,
                         const uint32b) noexcept
{
  ZISC_ASSERT(this->isHostWritable(), "The buffer isn't host writable.");
  const std::size_t s = sizeof(Type) * count;
  std::memcpy(data() + offset, host_data, s);
}

} // namespace zinvul

#endif // ZINVUL_CPU_BUFFER_INL_HPP
