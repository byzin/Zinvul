/*!
  \file cpu_buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_BUFFER_HPP
#define ZINVUL_CPU_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <vector>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul/buffer.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"

namespace zinvul {

// Forward declaration
class CpuDevice;

/*!
  \brief No brief description

  No detailed description.

  \tparam T No description.
  */
template <typename T>
class CpuBuffer : public Buffer<T>
{
 public:
  // Type aliases
  using Type = typename Buffer<T>::Type;
  using ConstType = typename Buffer<T>::ConstType;
  using Pointer = typename Buffer<T>::Pointer;
  using ConstPointer = typename Buffer<T>::ConstPointer;


  //! Initialize the buffer
  CpuBuffer(IdData&& id) noexcept;

  //! Finalize the buffer
  ~CpuBuffer() noexcept override;


  //! Return the buffer data
  zisc::pmr::vector<Type>& buffer() noexcept;

  //! Return the buffer data
  const zisc::pmr::vector<Type>& buffer() const noexcept;

  //! Check if the buffer is the most efficient for the device access
  bool isDeviceLocal() const noexcept override;

  //! Check if the buffer is cached on the host
  bool isHostCached() const noexcept override;

  //! Check if the buffer doesn't need to be unmapped
  bool isHostCoherent() const noexcept override;

  //! Check if the buffer can be mapped for the host access
  bool isHostVisible() const noexcept override;

  //! Change the number of elements
  void setSize(const std::size_t s) override;

  //! Return the number of elements
  std::size_t size() const noexcept override;

 protected:
  //! Clear the contents of the buffer
  void destroyData() noexcept override;

  //! Initialize the buffer
  void initData() override;

 private:
  //! Return the device
  CpuDevice& parentImpl() noexcept;

  //! Return the device
  const CpuDevice& parentImpl() const noexcept;

  //! Prepare a buffer for use
  void prepareBuffer() noexcept;


  zisc::pmr::unique_ptr<zisc::pmr::vector<Type>> buffer_;
};

} // namespace zinvul

#include "cpu_buffer-inl.hpp"

#endif // ZINVUL_CPU_BUFFER_HPP

///*!
//  \file cpu_buffer.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZINVUL_CPU_BUFFER_HPP
//#define ZINVUL_CPU_BUFFER_HPP
//
//// Standard C++ library
//#include <cstddef>
//#include <type_traits>
//// Zisc
//#include "zisc/memory_resource.hpp"
//// Zinvul
//#include "zinvul/buffer.hpp"
//#include "zinvul/zinvul_config.hpp"
//
//namespace zinvul {
//
//// Forward declaration
//class CpuDevice;
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T>
//class CpuBuffer : public Buffer<kDescriptor, T>
//{
// public:
//  //! The type of the buffer. "const", "volatile" and "reference" are removed
//  using Type = typename Buffer<kDescriptor, T>::Type;
//  using Pointer = typename Buffer<kDescriptor, T>::Pointer;
//  using ConstPointer = typename Buffer<kDescriptor, T>::ConstPointer;
//
//
//  //! Create an empty buffer
//  CpuBuffer(CpuDevice* device,
//            const BufferUsage usage_flag) noexcept;
//
//  //! Create a buffer
//  CpuBuffer(CpuDevice* device,
//            const BufferUsage usage_flag,
//            const std::size_t size) noexcept;
//
//
//  //! Return the buffer body
//  zisc::pmr::vector<Type>& buffer() noexcept;
//
//  //! Return the buffer body
//  const zisc::pmr::vector<Type>& buffer() const noexcept;
//
//  //! Copy this buffer to a dst buffer
//  template <DescriptorType kDstDescriptor>
//  void copyTo(CpuBuffer<kDstDescriptor, T>* dst,
//              const std::size_t count,
//              const std::size_t src_offset,
//              const std::size_t dst_offset,
//              const uint32b queue_index) const noexcept;
//
//  //! Return a data from a cpu buffer
//  Pointer data() noexcept;
//
//  //! Return a data from a cpu buffer
//  ConstPointer data() const noexcept;
//
//  //! Return the device type
//  SubPlatformType SubPlatformType() const noexcept override;
//
//  //! Check if a buffer memory is on device
//  bool isDeviceMemory() const noexcept override;
//
//  //! Check if a buffer memory is on host
//  bool isHostMemory() const noexcept override;
//
//  //! Check if a buffer memory is host visible
//  bool isHostVisible() const noexcept override;
//
//  //! Return the memory usage
//  std::size_t memoryUsage() const noexcept override;
//
//  //! Read a data from a buffer
//  void read(Pointer host_data,
//            const std::size_t count,
//            const std::size_t offset,
//            const uint32b queue_index) const noexcept override;
//
//  //! Set a size of a buffer
//  void setSize(const std::size_t size) noexcept override;
//
//  //! Return a size of a buffer
//  std::size_t size() const noexcept override;
//
//  //! Write a data to a buffer
//  void write(ConstPointer host_data,
//             const std::size_t count,
//             const std::size_t offset,
//             const uint32b queue_index) noexcept override;
//
// private:
//  //! Map a buffer memory to a host
//  Pointer mappedMemory() const noexcept override;
//
//  //! Unmap a buffer memory
//  void unmapMemory() const noexcept override;
//
//
//  CpuDevice* device_;
//  zisc::pmr::vector<Type> buffer_;
//};
//
//// Type aliases
//template <typename Type>
//using CpuUniformBuffer = CpuBuffer<DescriptorType::kUniform, Type>;
//template <typename Type>
//using CpuStorageBuffer = CpuBuffer<DescriptorType::kStorage, Type>;
//
//} // namespace zinvul
//
//#include "cpu_buffer-inl.hpp"
//
//#endif // ZINVUL_CPU_BUFFER_HPP
