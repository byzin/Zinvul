/*!
  \file vulkan_buffer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_BUFFER_HPP
#define ZINVUL_VULKAN_BUFFER_HPP

// Standard C++ library
#include <cstddef>
// Vulkan
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class VulkanDevice;

/*!
  */
template <BufferType kBufferType, typename T>
class VulkanBuffer : public Buffer<kBufferType, T>
{
 public:
  //! The type of the buffer. "const", "volatile" and "reference" are removed
  using Type = typename Buffer<kBufferType, T>::Type;
  using Pointer = typename Buffer<kBufferType, T>::Pointer;
  using ConstPointer = typename Buffer<kBufferType, T>::ConstPointer;


  //! Create an empty buffer
  VulkanBuffer(const VulkanDevice* device,
               const BufferUsage usage_flag) noexcept;

  //! Create a buffer
  VulkanBuffer(const VulkanDevice* device,
               const BufferUsage usage_flag,
               const std::size_t size) noexcept;

  //! Destroy a buffer
  ~VulkanBuffer() noexcept override;


  //! Return the allocation info
  VmaAllocationInfo& allocationInfo() noexcept;

  //! Return the allocation info
  const VmaAllocationInfo& allocationInfo() const noexcept;

  //! Return the buffer body
  vk::Buffer& buffer() noexcept;

  //! Return the buffer body
  const vk::Buffer& buffer() const noexcept;

  //! Copy this buffer to a dst buffer
  void copyTo(Buffer<kBufferType, T>* dst,
              const std::size_t count,
              const std::size_t src_offset,
              const std::size_t dst_offset,
              const uint32b queue_index) const noexcept override;

  //! Destroy a buffer
  void destroy() noexcept;

  //! Return the device type
  DeviceType deviceType() const noexcept override;

  //! Return the expected memory usage
  std::size_t expectedMemoryUsage() const noexcept;

  //! Return the memory allocation
  VmaAllocation& memory() noexcept;

  //! Return the memory allocation
  const VmaAllocation& memory() const noexcept;

  //! Return the memory usage
  std::size_t memoryUsage() const noexcept override;

  //! Read a data from a buffer
  void read(Pointer data,
            const std::size_t count,
            const std::size_t offset,
            const uint32b queue_index) const noexcept override;

  //! Set a size of a buffer
  void setSize(const std::size_t size) noexcept override;

  //! Return a size of a buffer
  std::size_t size() const noexcept override;

  //! Write a data to a buffer
  void write(ConstPointer data,
             const std::size_t count,
             const std::size_t offset,
             const uint32b queue_index) noexcept override;

 private:
  const VulkanDevice* device_;
  vk::Buffer buffer_;
  VmaAllocation memory_ = VK_NULL_HANDLE;
  VmaAllocationInfo alloc_info_;
  std::size_t size_ = 0;
};

// Type aliases
template <typename Type>
using VulkanUniformBuffer = VulkanBuffer<BufferType::kUniform, Type>;
template <typename Type>
using VulkanStorageBuffer = VulkanBuffer<BufferType::kStorage, Type>;

} // namespace zinvul

#include "vulkan_buffer-inl.hpp"

#endif // ZINVUL_VULKAN_BUFFER_HPP
