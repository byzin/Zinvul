/*!
  \file vulkan_buffer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_BUFFER_INL_HPP
#define ZINVUL_VULKAN_BUFFER_INL_HPP

#include "vulkan_buffer.hpp"
// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_device.hpp"
#include "zinvul/buffer.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <typename T> inline
VulkanBuffer<T>::VulkanBuffer(const VulkanDevice* device,
                              const BufferUsage usage_flag) noexcept :
    Buffer<T>(usage_flag),
    device_{device}
{
  ZISC_ASSERT(device_ != nullptr, "The device is null.");
  alloc_info_.size = 0;
}

/*!
  */
template <typename T> inline
VulkanBuffer<T>::VulkanBuffer(const VulkanDevice* device,
                              const BufferUsage usage_flag,
                              const std::size_t size) noexcept :
    VulkanBuffer(device, usage_flag)
{
  setSize(size);
}

/*!
  */
template <typename T> inline
VulkanBuffer<T>::~VulkanBuffer() noexcept
{
  destroy();
}

/*!
  */
template <typename T> inline
VmaAllocationInfo& VulkanBuffer<T>::allocationInfo() noexcept
{
  return alloc_info_;
}

/*!
  */
template <typename T> inline
const VmaAllocationInfo& VulkanBuffer<T>::allocationInfo() const noexcept
{
  return alloc_info_;
}

/*!
  */
template <typename T> inline
vk::Buffer& VulkanBuffer<T>::buffer() noexcept
{
  return buffer_;
}

/*!
  */
template <typename T> inline
const vk::Buffer& VulkanBuffer<T>::buffer() const noexcept
{
  return buffer_;
}

/*!
  */
template <typename T> inline
void VulkanBuffer<T>::copyTo(Buffer<Type>* dst,
                             const std::size_t count,
                             const std::size_t src_offset,
                             const std::size_t dst_offset,
                             const uint32b queue_index) const noexcept
{
  ZISC_ASSERT(this->isSource(), "This buffer isn't source.");
  ZISC_ASSERT(dst->isDestination(), "The buffer 'dst' isn't destination.");
  const std::size_t s = sizeof(Type) * count;
  ZISC_ASSERT(s <= dst->memoryUsage(), "The size of dst buffer memory is less.");
  auto d = zisc::cast<VulkanBuffer<Type>*>(dst);
  const std::size_t src_offset_size = sizeof(Type) * src_offset;
  const std::size_t dst_offset_size = sizeof(Type) * dst_offset;
  const vk::BufferCopy copy_info{src_offset_size, dst_offset_size, s};
  device_->copyBuffer(*this, d, copy_info, queue_index);
}

/*!
  */
template <typename T> inline
void VulkanBuffer<T>::destroy() noexcept
{
  if (buffer_) {
    auto d = const_cast<VulkanDevice*>(device_);
    d->deallocate(this);
  }
}

/*!
  */
template <typename T> inline
DeviceType VulkanBuffer<T>::deviceType() const noexcept
{
  return DeviceType::kVulkan;
}

/*!
  */
template <typename T> inline
std::size_t VulkanBuffer<T>::expectedMemoryUsage() const noexcept
{
  const std::size_t memory_usage = sizeof(Type) * size();
  return memory_usage;
}

/*!
  */
template <typename T> inline
VmaAllocation& VulkanBuffer<T>::memory() noexcept
{
  return memory_;
}

/*!
  */
template <typename T> inline
const VmaAllocation& VulkanBuffer<T>::memory() const noexcept
{
  return memory_;
}

/*!
  */
template <typename T> inline
std::size_t VulkanBuffer<T>::memoryUsage() const noexcept
{
  const auto memory_usage = zisc::cast<std::size_t>(alloc_info_.size);
  return memory_usage;
}

/*!
  */
template <typename T> inline
void VulkanBuffer<T>::read(Pointer data,
                           const std::size_t count,
                           const std::size_t offset,
                           const uint32b queue_index) const noexcept
{
  ZISC_ASSERT(this->isHostReadable(), "The buffer isn't readable.");
  if (this->isHostBuffer()) {
    ConstPointer source = zisc::cast<ConstPointer>(device_->mapMemory(*this));
    const std::size_t s = sizeof(Type) * count;
    std::memcpy(data, source + offset, s);
    device_->unmapMemory(*this);
  }
  else {
    VulkanBuffer<Type> dst{device_, BufferUsage::kHost | BufferUsage::kTDst};
    dst.setSize(count);
    copyTo(&dst, count, offset, 0, queue_index);
    dst.read(data, count, 0, queue_index);
  }
}

/*!
  */
template <typename T> inline
void VulkanBuffer<T>::setSize(const std::size_t size) noexcept
{
  destroy();
  size_ = size;
  auto d = const_cast<VulkanDevice*>(device_);
  d->allocate(size, this);
}

/*!
  */
template <typename T> inline
std::size_t VulkanBuffer<T>::size() const noexcept
{
  return size_;
}

/*!
  */
template <typename T> inline
void VulkanBuffer<T>::write(ConstPointer data,
                            const std::size_t count,
                            const std::size_t offset,
                            const uint32b queue_index) noexcept
{
  ZISC_ASSERT(this->isHostWritable(), "The buffer isn't writable.");
  if (this->isHostBuffer()) {
    Pointer dest = zisc::cast<Pointer>(device_->mapMemory(*this));
    const std::size_t s = sizeof(Type) * count;
    std::memcpy(dest + offset, data, s);
    device_->unmapMemory(*this);
  }
  else {
    VulkanBuffer<Type> src{device_, BufferUsage::kHost | BufferUsage::kTSrc};
    src.setSize(count);
    src.write(data, count, 0, queue_index);
    src.copyTo(this, count, 0, offset, queue_index);
  }
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_BUFFER_INL_HPP
