/*!
  \file vulkan_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_BUFFER_INL_HPP
#define ZINVUL_VULKAN_BUFFER_INL_HPP

#include "vulkan_buffer.hpp"
// Standard C++ library
#include <memory>
#include <utility>
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "vulkan_device.hpp"
#include "zinvul/buffer.hpp"
#include "zinvul/sub_platform.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"
#include "zinvul/utility/zinvul_object.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
template <typename T> inline
VulkanBuffer<T>::VulkanBuffer(IdData&& id) noexcept : Buffer<T>(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
VulkanBuffer<T>::~VulkanBuffer() noexcept
{
  Buffer<T>::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
VmaAllocation& VulkanBuffer<T>::allocation() noexcept
{
  return vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const VmaAllocation& VulkanBuffer<T>::allocation() const noexcept
{
  return vm_allocation_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const VmaAllocationInfo& VulkanBuffer<T>::allocationInfo() const noexcept
{
  return vm_alloc_info_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
VkBuffer& VulkanBuffer<T>::buffer() noexcept
{
  return buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const VkBuffer& VulkanBuffer<T>::buffer() const noexcept
{
  return buffer_;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <typename T> inline
void VulkanBuffer<T>::setSize(const std::size_t s)
{
  const std::size_t prev_size = size();
  if (s != prev_size) {
    Buffer<T>::clear();
    if (0 < s) {
      const std::size_t mem_size = sizeof(Type) * s;
      auto& device = parentImpl();
      device.allocateMemory(mem_size,
                            Buffer<T>::usage(),
                            std::addressof(Buffer<T>::id()),
                            std::addressof(buffer()),
                            std::addressof(allocation()),
                            std::addressof(vm_alloc_info_));
    }
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t VulkanBuffer<T>::size() const noexcept
{
  const auto& info = allocationInfo();
  const std::size_t s = info.size / sizeof(Type);
  return s;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void VulkanBuffer<T>::destroyData() noexcept
{
  if (buffer_ != VK_NULL_HANDLE) {
    auto& device = parentImpl();
    device.deallocateMemory(std::addressof(buffer()),
                            std::addressof(allocation()),
                            std::addressof(vm_alloc_info_));
    initData();
  }
}

/*!
  \details No detailed description
  */
template <typename T> inline
void VulkanBuffer<T>::initData()
{
  buffer_ = VK_NULL_HANDLE;
  vm_allocation_ = VK_NULL_HANDLE;
  vm_alloc_info_.memoryType = 0;
  vm_alloc_info_.deviceMemory = VK_NULL_HANDLE;
  vm_alloc_info_.offset = 0;
  vm_alloc_info_.size = 0;
  vm_alloc_info_.pMappedData = nullptr;
  vm_alloc_info_.pUserData = nullptr;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
VulkanDevice& VulkanBuffer<T>::parentImpl() noexcept
{
  auto p = Buffer<T>::getParent();
  return *zisc::treatAs<VulkanDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const VulkanDevice& VulkanBuffer<T>::parentImpl() const noexcept
{
  const auto p = Buffer<T>::getParent();
  return *zisc::treatAs<const VulkanDevice*>(p);
}

// Device

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <typename T> inline
SharedBuffer<T> VulkanDevice::makeBuffer(const BufferUsage flag)
{
  using BufferType = VulkanBuffer<T>;
  zisc::pmr::polymorphic_allocator<BufferType> alloc{memoryResource()};
  SharedBuffer<T> buffer = std::allocate_shared<BufferType>(alloc, issueId());

  ZinvulObject::SharedPtr parent{getOwn()};
  WeakBuffer<T> own{buffer};
  buffer->initialize(std::move(parent), std::move(own), flag);

  return buffer;
}

} // namespace zinvul

#endif // ZINVUL_VULKAN_BUFFER_INL_HPP

///*!
//  \file vulkan_buffer-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZINVUL_VULKAN_BUFFER_INL_HPP
//#define ZINVUL_VULKAN_BUFFER_INL_HPP
//
//#include "vulkan_buffer.hpp"
//// Standard C++ library
//#include <cstddef>
//// Vulkan
//#include <vulkan/vulkan.hpp>
//#include "vk_mem_alloc.h"
//// Zisc
//#include "zisc/error.hpp"
//#include "zisc/utility.hpp"
//// Zinvul
//#include "vulkan_device.hpp"
//#include "zinvul/buffer.hpp"
//#include "zinvul/zinvul_config.hpp"
//
//namespace zinvul {
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VulkanBuffer<kDescriptor, T>::VulkanBuffer(const VulkanDevice* device,
//                                           const BufferUsage usage_flag) noexcept :
//    Buffer<kDescriptor, T>(usage_flag),
//    device_{device}
//{
//  initialize();
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VulkanBuffer<kDescriptor, T>::VulkanBuffer(const VulkanDevice* device,
//                                           const BufferUsage usage_flag,
//                                           const std::size_t size) noexcept :
//    VulkanBuffer(device, usage_flag)
//{
//  setSize(size);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VulkanBuffer<kDescriptor, T>::~VulkanBuffer() noexcept
//{
//  destroy();
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VmaAllocationInfo& VulkanBuffer<kDescriptor, T>::allocationInfo() noexcept
//{
//  return alloc_info_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//const VmaAllocationInfo& VulkanBuffer<kDescriptor, T>::allocationInfo() const noexcept
//{
//  return alloc_info_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//vk::Buffer& VulkanBuffer<kDescriptor, T>::buffer() noexcept
//{
//  return buffer_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//const vk::Buffer& VulkanBuffer<kDescriptor, T>::buffer() const noexcept
//{
//  return buffer_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> template <DescriptorType kDstDescriptor>
//inline
//void VulkanBuffer<kDescriptor, T>::copyTo(VulkanBuffer<kDstDescriptor, T>* dst,
//                                          const std::size_t count,
//                                          const std::size_t src_offset,
//                                          const std::size_t dst_offset,
//                                          const uint32b queue_index) const noexcept
//{
//  const std::size_t s = sizeof(Type) * count;
//  ZISC_ASSERT(s <= dst->memoryUsage(), "The size of dst buffer memory is less.");
//  const std::size_t src_offset_size = sizeof(Type) * src_offset;
//  const std::size_t dst_offset_size = sizeof(Type) * dst_offset;
//  const vk::BufferCopy copy_info{src_offset_size, dst_offset_size, s};
//
//  vk::CommandBufferBeginInfo begin_info{};
//  begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
//  copy_command_.begin(begin_info);
//
//  copy_command_.copyBuffer(buffer(), dst->buffer(), 1, &copy_info);
//
//  copy_command_.end();
//  device_->submit(QueueType::kTransfer, queue_index, copy_command_);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::destroy() noexcept
//{
//  if (buffer_) {
//    auto d = const_cast<VulkanDevice*>(device_);
//    d->deallocate(this);
//  }
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//SubPlatformType VulkanBuffer<kDescriptor, T>::SubPlatformType() const noexcept
//{
//  return SubPlatformType::kVulkan;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool VulkanBuffer<kDescriptor, T>::isDeviceMemory() const noexcept
//{
//  const auto& info = device_->physicalDeviceInfo();
//  const auto& memory_property = info.memoryProperties().properties1_;
//  const uint32b index = allocationInfo().memoryType;
//  const auto flag = memory_property.memoryTypes[index].propertyFlags;
//  const bool result = (flag & vk::MemoryPropertyFlagBits::eDeviceLocal) ==
//                      vk::MemoryPropertyFlagBits::eDeviceLocal;
//  return result;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool VulkanBuffer<kDescriptor, T>::isHostMemory() const noexcept
//{
//  const bool result = !isDeviceMemory();
//  return result;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool VulkanBuffer<kDescriptor, T>::isHostVisible() const noexcept
//{
//  const auto& info = device_->physicalDeviceInfo();
//  const auto& memory_property = info.memoryProperties().properties1_;
//  const uint32b index = allocationInfo().memoryType;
//  const auto flag = memory_property.memoryTypes[index].propertyFlags;
//  const bool result = (flag & vk::MemoryPropertyFlagBits::eHostVisible) ==
//                      vk::MemoryPropertyFlagBits::eHostVisible;
//  return result;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//VmaAllocation& VulkanBuffer<kDescriptor, T>::memory() noexcept
//{
//  return memory_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//const VmaAllocation& VulkanBuffer<kDescriptor, T>::memory() const noexcept
//{
//  return memory_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//std::size_t VulkanBuffer<kDescriptor, T>::memoryUsage() const noexcept
//{
//  const auto memory_usage = zisc::cast<std::size_t>(alloc_info_.size);
//  return memory_usage;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::read(Pointer data,
//                                        const std::size_t count,
//                                        const std::size_t offset,
//                                        const uint32b queue_index) const noexcept
//{
//  if (isHostVisible()) {
//    auto src = this->mapMemory();
//    const std::size_t s = sizeof(Type) * count;
//    std::memcpy(data, src.data() + offset, s);
//  }
//  else {
//    VulkanBuffer<kDescriptor, Type> dst{device_, BufferUsage::kHostOnly};
//    dst.setSize(count);
//    copyTo(&dst, count, offset, 0, queue_index);
//    device_->waitForCompletion(QueueType::kTransfer, queue_index);
//    dst.read(data, count, 0, queue_index);
//  }
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::setSize(const std::size_t size) noexcept
//{
//  destroy();
//  size_ = size;
//  auto d = const_cast<VulkanDevice*>(device_);
//  d->allocate(size, this);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//std::size_t VulkanBuffer<kDescriptor, T>::size() const noexcept
//{
//  return size_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::write(ConstPointer data,
//                                         const std::size_t count,
//                                         const std::size_t offset,
//                                         const uint32b queue_index) noexcept
//{
//  if (isHostVisible()) {
//    auto dst = this->mapMemory();
//    const std::size_t s = sizeof(Type) * count;
//    std::memcpy(dst.data() + offset, data, s);
//  }
//  else {
//    VulkanBuffer<kDescriptor, Type> src{device_, BufferUsage::kHostOnly};
//    src.setSize(count);
//    src.write(data, count, 0, queue_index);
//    src.copyTo(this, count, 0, offset, queue_index);
//    device_->waitForCompletion(QueueType::kTransfer, queue_index);
//  }
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::initialize() noexcept
//{
//  ZISC_ASSERT(device_ != nullptr, "The device is null.");
//  alloc_info_.size = 0;
//  // Initialize a copy command
//  const vk::CommandBufferAllocateInfo alloc_info{
//      device_->commandPool(QueueType::kTransfer),
//      vk::CommandBufferLevel::ePrimary,
//      1};
//  auto [r, copy_commands] = device_->device().allocateCommandBuffers(alloc_info);
//  ZISC_ASSERT(r == vk::Result::eSuccess, "Command buffer allocation failed.");
//  copy_command_ = copy_commands[0];
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto VulkanBuffer<kDescriptor, T>::mappedMemory() const noexcept -> Pointer
//{
//  void* d = nullptr;
//  const auto result = vmaMapMemory(device_->memoryAllocator(), memory_, &d);
//  ZISC_ASSERT(result == VK_SUCCESS, "Buffer memory map failed.");
//  (void)result;
//  return zisc::cast<Pointer>(d);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void VulkanBuffer<kDescriptor, T>::unmapMemory() const noexcept
//{
//  vmaUnmapMemory(device_->memoryAllocator(), memory_);
//}
//
//} // namespace zinvul
//
//#endif // ZINVUL_VULKAN_BUFFER_INL_HPP
