/*!
  \file cpu_buffer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
template <BufferType kBufferType, typename T> inline
CpuBuffer<kBufferType, T>::CpuBuffer(CpuDevice* device,
                                     const BufferUsage usage_flag) noexcept :
    Buffer<kBufferType, T>(usage_flag),
    device_{device},
    buffer_{device->memoryResource()}
{
}

/*!
  */
template <BufferType kBufferType, typename T> inline
CpuBuffer<kBufferType, T>::CpuBuffer(CpuDevice* device,
                                     const BufferUsage usage_flag,
                                     const std::size_t size) noexcept :
    CpuBuffer(device, usage_flag)
{
  setSize(size);
}

/*!
  */
template <BufferType kBufferType, typename T> inline
auto CpuBuffer<kBufferType, T>::buffer() noexcept -> zisc::pmr::vector<Type>&
{
  return buffer_;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
auto CpuBuffer<kBufferType, T>::buffer() const noexcept
    -> const zisc::pmr::vector<Type>&
{
  return buffer_;
}

/*!
  */
template <BufferType kBufferType, typename T> template <BufferType kDstBufferType>
inline
void CpuBuffer<kBufferType, T>::copyTo(CpuBuffer<kDstBufferType, T>* dst,
                                       const std::size_t count,
                                       const std::size_t src_offset,
                                       const std::size_t dst_offset,
                                       const uint32b queue_index) const noexcept
{
  dst->write(data() + src_offset, count, dst_offset, queue_index);
}

/*!
  */
template <BufferType kBufferType, typename T> inline
auto CpuBuffer<kBufferType, T>::data() noexcept -> Pointer
{
  return buffer_.data();
}

/*!
  */
template <BufferType kBufferType, typename T> inline
auto CpuBuffer<kBufferType, T>::data() const noexcept -> ConstPointer
{
  return buffer_.data();
}

/*!
  */
template <BufferType kBufferType, typename T> inline
DeviceType CpuBuffer<kBufferType, T>::deviceType() const noexcept
{
  return DeviceType::kCpu;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool CpuBuffer<kBufferType, T>::isDeviceMemory() const noexcept
{
  return true;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool CpuBuffer<kBufferType, T>::isHostMemory() const noexcept
{
  return true;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
bool CpuBuffer<kBufferType, T>::isHostVisible() const noexcept
{
  return true;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
std::size_t CpuBuffer<kBufferType, T>::memoryUsage() const noexcept
{
  const std::size_t memory_usage = sizeof(Type) * size();
  return memory_usage;
}

/*!
  */
template <BufferType kBufferType, typename T> inline
void CpuBuffer<kBufferType, T>::read(Pointer host_data,
                                     const std::size_t count,
                                     const std::size_t offset,
                                     const uint32b) const noexcept
{
  const std::size_t s = sizeof(Type) * count;
  std::memcpy(host_data, data() + offset, s);
}

/*!
  */
template <BufferType kBufferType, typename T> inline
void CpuBuffer<kBufferType, T>::setSize(const std::size_t size) noexcept
{
  device_->deallocate(this);
  device_->allocate(size, this);
}

/*!
  */
template <BufferType kBufferType, typename T> inline
std::size_t CpuBuffer<kBufferType, T>::size() const noexcept
{
  return buffer_.size();
}

/*!
  */
template <BufferType kBufferType, typename T> inline
void CpuBuffer<kBufferType, T>::write(ConstPointer host_data,
                                      const std::size_t count,
                                      const std::size_t offset,
                                      const uint32b) noexcept
{
  const std::size_t s = sizeof(Type) * count;
  std::memcpy(data() + offset, host_data, s);
}

} // namespace zinvul

#endif // ZINVUL_CPU_BUFFER_INL_HPP
