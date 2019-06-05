/*!
  \file buffer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUFFER_HPP
#define ZINVUL_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/non_copyable.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <BufferType kBufferType, typename T>
class Buffer : private zisc::NonCopyable<Buffer<kBufferType, T>>
{
  static_assert(!std::is_pointer_v<T>, "Pointer type is restricted.");
 public:
  //! The type of the buffer. "const", "volatile" and "reference" are removed
  using Type = std::remove_reference_t<std::remove_cv_t<T>>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<std::add_const_t<Type>>;


  //! Initialize a buffer
  Buffer(const BufferUsage usage_flag) noexcept;

  //!
  virtual ~Buffer() noexcept;


  //! Return the buffer type
  static constexpr BufferType bufferType() noexcept;

  //! Copy this buffer to a dst buffer
  virtual void copyTo(Buffer<kBufferType, T>* dst,
                      const std::size_t count,
                      const std::size_t src_offset,
                      const std::size_t dst_offset,
                      const uint32b queue_index) const noexcept = 0;

  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Return the expected memory usage
  std::size_t expectedMemoryUsage() const noexcept;

  //! Check if a buffer is transfer destination buffer
  bool isDestination() const noexcept;

  //! Check if a buffer memory is on device
  bool isDeviceBuffer() const noexcept;

  //! Check if a buffer memory is mappable on host
  bool isHostBuffer() const noexcept;

  //! Check if a buffer is readable from host
  bool isHostReadable() const noexcept;

  //! Check if a buffer is host visible
  bool isHostVisible() const noexcept;

  //! Check if a buffer is writable from host
  bool isHostWritable() const noexcept;

  //! Check if a buffer is transfer source buffer
  bool isSource() const noexcept;

  //! Return the memory usage
  virtual std::size_t memoryUsage() const noexcept = 0;

  //! Read a data from a buffer
  virtual void read(Pointer data,
                    const std::size_t count,
                    const std::size_t offset,
                    const uint32b queue_index) const noexcept = 0;

  //! Set a size of a buffer
  virtual void setSize(const std::size_t s) noexcept = 0;

  //! Return a size of a buffer
  virtual std::size_t size() const noexcept = 0;

  //! Convert a type of a buffer interface to DstType
  template <typename DstType>
  Buffer<kBufferType, DstType>* treatAs() noexcept;

  //! Convert a type of a buffer interface to DstType
  template <typename DstType>
  const Buffer<kBufferType, DstType>* treatAs() const noexcept;

  //! Return usage flag
  BufferUsage usage() const noexcept;

  //! Write a data to a buffer
  virtual void write(ConstPointer data,
                     const std::size_t count,
                     const std::size_t offset,
                     const uint32b queue_index) noexcept = 0;

 private:
  //! Initialize a buffer
  void initialize() noexcept;


  BufferUsage usage_flag_;
};

// Type aliases
template <typename Type>
using UniformBuffer = Buffer<BufferType::kUniform, Type>;
template <typename Type>
using StorageBuffer = Buffer<BufferType::kStorage, Type>;
template <BufferType kBufferType, typename Type>
using UniqueBuffer = zisc::UniqueMemoryPointer<Buffer<kBufferType, Type>>;

} // namespace zinvul

#include "buffer-inl.hpp"

#endif // ZINVUL_BUFFER_HPP
