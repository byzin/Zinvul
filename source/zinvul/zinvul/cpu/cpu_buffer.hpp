/*!
  \file cpu_buffer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_BUFFER_HPP
#define ZINVUL_CPU_BUFFER_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/memory_resource.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class CpuDevice;

/*!
  */
template <typename Type>
class CpuBuffer : public Buffer<Type>
{
 public:
  //! Create an empty buffer
  CpuBuffer(CpuDevice* device,
            const BufferUsage usage_flag) noexcept;

  //! Create a buffer
  CpuBuffer(CpuDevice* device,
            const BufferUsage usage_flag,
            const std::size_t size) noexcept;


  //! Return a data from a cpu buffer
  Type* data() noexcept;

  //! Return a data from a cpu buffer
  const Type* data() const noexcept;

  //! Return the device type
  DeviceType deviceType() const noexcept override;

  //! Return the memory usage
  std::size_t memoryUsage() const noexcept override;

  //! Read a data from a buffer
  void read(Type* data) const noexcept override;

  //! Set a size of a buffer
  void setSize(const std::size_t size) noexcept override;

  //! Return a size of a buffer
  std::size_t size() const noexcept override;

  //! Write a data to a buffer
  void write(const Type* data) noexcept override;

 private:
  zisc::pmr::vector<Type> buffer_;
};

} // namespace zinvul

#include "cpu_buffer-inl.hpp"

#endif // ZINVUL_CPU_BUFFER_HPP
