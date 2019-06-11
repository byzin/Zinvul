/*!
  \file cpu_buffer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_BUFFER_HPP
#define ZINVUL_CPU_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
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
template <BufferType kBufferType, typename T>
class CpuBuffer : public Buffer<kBufferType, T>
{
 public:
  //! The type of the buffer. "const", "volatile" and "reference" are removed
  using Type = typename Buffer<kBufferType, T>::Type;
  using Pointer = typename Buffer<kBufferType, T>::Pointer;
  using ConstPointer = typename Buffer<kBufferType, T>::ConstPointer;


  //! Create an empty buffer
  CpuBuffer(CpuDevice* device,
            const BufferUsage usage_flag) noexcept;

  //! Create a buffer
  CpuBuffer(CpuDevice* device,
            const BufferUsage usage_flag,
            const std::size_t size) noexcept;


  //! Return the buffer body
  zisc::pmr::vector<Type>& buffer() noexcept;

  //! Return the buffer body
  const zisc::pmr::vector<Type>& buffer() const noexcept;

  //! Copy this buffer to a dst buffer
  template <BufferType kDstBufferType>
  void copyTo(CpuBuffer<kDstBufferType, T>* dst,
              const std::size_t count,
              const std::size_t src_offset,
              const std::size_t dst_offset,
              const uint32b queue_index) const noexcept;

  //! Return a data from a cpu buffer
  Pointer data() noexcept;

  //! Return a data from a cpu buffer
  ConstPointer data() const noexcept;

  //! Return the device type
  DeviceType deviceType() const noexcept override;

  //! Check if a buffer memory is on device
  bool isDeviceMemory() const noexcept override;

  //! Check if a buffer memory is on host
  bool isHostMemory() const noexcept override;

  //! Check if a buffer memory is host visible
  bool isHostVisible() const noexcept override;

  //! Return the memory usage
  std::size_t memoryUsage() const noexcept override;

  //! Read a data from a buffer
  void read(Pointer host_data,
            const std::size_t count,
            const std::size_t offset,
            const uint32b queue_index) const noexcept override;

  //! Set a size of a buffer
  void setSize(const std::size_t size) noexcept override;

  //! Return a size of a buffer
  std::size_t size() const noexcept override;

  //! Write a data to a buffer
  void write(ConstPointer host_data,
             const std::size_t count,
             const std::size_t offset,
             const uint32b queue_index) noexcept override;

 private:
  CpuDevice* device_;
  zisc::pmr::vector<Type> buffer_;
};

// Type aliases
template <typename Type>
using CpuUniformBuffer = CpuBuffer<BufferType::kUniform, Type>;
template <typename Type>
using CpuStorageBuffer = CpuBuffer<BufferType::kStorage, Type>;

} // namespace zinvul

#include "cpu_buffer-inl.hpp"

#endif // ZINVUL_CPU_BUFFER_HPP
