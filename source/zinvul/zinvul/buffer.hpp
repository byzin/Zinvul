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

  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Check if a buffer memory is on device
  virtual bool isDeviceMemory() const noexcept = 0;

  //! Check if a buffer memory is on host
  virtual bool isHostMemory() const noexcept = 0;

  //! Check if a buffer is host visible
  virtual bool isHostVisible() const noexcept = 0;

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

  //! Return the buffer usage flag
  BufferUsage usage() const noexcept;

  //! Write a data to a buffer
  virtual void write(ConstPointer data,
                     const std::size_t count,
                     const std::size_t offset,
                     const uint32b queue_index) noexcept = 0;

 private:
  static_assert(!std::is_pointer_v<T>, "Pointer type is restricted.");


  //! Initialize a buffer
  void initialize() noexcept;


  BufferUsage usage_flag_;
};

//! Copy a src buffer to a dst buffer
template <BufferType kBufferType1, BufferType kBufferType2, typename Type>
void copy(const Buffer<kBufferType1, Type>& src,
          Buffer<kBufferType2, Type>* dst,
          const std::size_t count,
          const std::size_t src_offset,
          const std::size_t dst_offset,
          const uint32b queue_index) noexcept;

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
