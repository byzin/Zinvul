/*!
  \file buffer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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
template <typename Type>
class Buffer : private zisc::NonCopyable<Buffer<Type>>
{
 public:
  //! Initialize a buffer
  Buffer(const BufferUsage usage_flag) noexcept;

  //!
  virtual ~Buffer() noexcept;


  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Return the expected memory usage
  std::size_t expectedMemoryUsage() const noexcept;

  //! Check if a buffer is readable from host
  bool isHostReadable() const noexcept;

  //! Check if a buffer is writable from host
  bool isHostWritable() const noexcept;

  //! Return the memory usage
  virtual std::size_t memoryUsage() const noexcept = 0;

  //! Read a data from a buffer
  virtual void read(Type* data) const noexcept = 0;

  //! Set a size of a buffer
  virtual void setSize(const std::size_t s) noexcept = 0;

  //! Return a size of a buffer
  virtual std::size_t size() const noexcept = 0;

  //! Return usage flag
  BufferUsage usage() const noexcept;

  //! Write a data to a buffer
  virtual void write(const Type* data) noexcept = 0;

 private:
  //! Map a device buffer memory to a host
  void mapMemory(void** data) const noexcept;

  //! Map a device buffer memory to a host
  void unmapMemory() const noexcept;


  BufferUsage usage_flag_;
};

// Type aliases
template <typename Type>
using UniqueBuffer = zisc::UniqueMemoryPointer<Buffer<Type>>;

} // namespace zinvul

#include "buffer-inl.hpp"

#endif // ZINVUL_BUFFER_HPP
