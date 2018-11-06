/*!
  \file vulkan_buffer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
template <typename Type> inline
VulkanBuffer<Type>::VulkanBuffer(VulkanDevice* device,
                                 const BufferUsage usage_flag) noexcept :
    Buffer<Type>(usage_flag),
    device_{device}
{
  ZISC_ASSERT(device_ != nullptr, "The device is null.");
  alloc_info_.size = 0;
}

/*!
  */
template <typename Type> inline
VulkanBuffer<Type>::VulkanBuffer(VulkanDevice* device,
                                 const BufferUsage usage_flag,
                                 const std::size_t size) noexcept :
    VulkanBuffer(device, usage_flag)
{
  setSize(size);
}

/*!
  */
template <typename Type> inline
VulkanBuffer<Type>::~VulkanBuffer() noexcept
{
  destroy();
}

/*!
  */
template <typename Type> inline
VmaAllocationInfo& VulkanBuffer<Type>::allocationInfo() noexcept
{
  return alloc_info_;
}

/*!
  */
template <typename Type> inline
const VmaAllocationInfo& VulkanBuffer<Type>::allocationInfo() const noexcept
{
  return alloc_info_;
}

/*!
  */
template <typename Type> inline
vk::Buffer& VulkanBuffer<Type>::buffer() noexcept
{
  return buffer_;
}

/*!
  */
template <typename Type> inline
const vk::Buffer& VulkanBuffer<Type>::buffer() const noexcept
{
  return buffer_;
}

/*!
  */
template <typename Type> inline
void VulkanBuffer<Type>::destroy() noexcept
{
  if (buffer_)
    device_->deallocate(this);
}

/*!
  */
template <typename Type> inline
DeviceType VulkanBuffer<Type>::deviceType() const noexcept
{
  return DeviceType::kVulkan;
}

/*!
  */
template <typename Type> inline
std::size_t VulkanBuffer<Type>::expectedMemoryUsage() const noexcept
{
  const std::size_t memory_usage = sizeof(Type) * size();
  return memory_usage;
}

/*!
  */
template <typename Type> inline
VmaAllocation& VulkanBuffer<Type>::memory() noexcept
{
  return memory_;
}

/*!
  */
template <typename Type> inline
const VmaAllocation& VulkanBuffer<Type>::memory() const noexcept
{
  return memory_;
}

/*!
  */
template <typename Type> inline
std::size_t VulkanBuffer<Type>::memoryUsage() const noexcept
{
  const auto memory_usage = zisc::cast<std::size_t>(alloc_info_.size);
  return memory_usage;
}

/*!
  */
template <typename Type> inline
void VulkanBuffer<Type>::read(Type* data) const noexcept
{
  ZISC_ASSERT(this->isHostReadable(), "The buffer isn't readable.");
  void* source = device_->mapMemory(*this);
  std::memcpy(data, source, expectedMemoryUsage());
  device_->unmapMemory(*this);
}

/*!
  */
template <typename Type> inline
void VulkanBuffer<Type>::setSize(const std::size_t size) noexcept
{
  destroy();
  size_ = size;
  device_->allocate(size, this);
}

/*!
  */
template <typename Type> inline
std::size_t VulkanBuffer<Type>::size() const noexcept
{
  return size_;
}

/*!
  */
template <typename Type> inline
void VulkanBuffer<Type>::write(const Type* data) noexcept
{
  ZISC_ASSERT(this->isHostWritable(), "The buffer isn't writable.");
  void* dest = device_->mapMemory(*this);
  std::memcpy(dest, data, expectedMemoryUsage());
  device_->unmapMemory(*this);
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_BUFFER_INL_HPP
