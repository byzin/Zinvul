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
  Buffer(const int usage_flags) noexcept;

  //!
  virtual ~Buffer() noexcept;


  //! Return the device type
  virtual DeviceType deviceType() const noexcept = 0;

  //! Check if a buffer is readable from host
  bool isHostReadable() const noexcept;

  //! Check if a buffer is writable from host
  bool isHostWritable() const noexcept;

  //! Read a data from a buffer
  virtual void read(Type* data) const noexcept = 0;

  //! Set a size of a buffer
  virtual void setSize(const std::size_t size) noexcept = 0;

  //! Return a size of a buffer
  virtual std::size_t size() const noexcept = 0;

  //! Return usage flags
  int usageFlags() const noexcept;

  //! Write a data to a buffer
  virtual void write(const Type* data) noexcept = 0;

 private:
  int32b usage_flags_;
};

// Type aliases
template <typename Type>
using UniqueBuffer = zisc::UniqueMemoryPointer<Buffer<Type>>;

} // namespace zinvul

#include "buffer-inl.hpp"

#endif // ZINVUL_BUFFER_HPP
