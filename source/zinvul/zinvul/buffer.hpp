/*!
  \file buffer.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUFFER_HPP
#define ZINVUL_BUFFER_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
#include <type_traits>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"

namespace zinvul {

/*!
  \brief No brief description

  No detailed description.

  \tparam kDescType No description.
  \tparam T No description.
  */
template <DescriptorType kDescType, typename T>
class Buffer : private zisc::NonCopyable<Buffer<kDescType, T>>
{
 public:
  // Type aliases
  using Type = std::remove_cv_t<std::remove_reference_t<T>>;
  using ConstType = std::add_const_t<Type>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<ConstType>;


  //! Initialize the buffer
  Buffer(const BufferUsage buffer_usage, IdData&& id_data) noexcept;

  //! Move a data
  Buffer(Buffer&& other) noexcept;

  //! Finalize the buffer
  virtual ~Buffer() noexcept;


  //! Move a data
  Buffer& operator=(Buffer&& other) noexcept;


  //! Clear the contents of the buffer
  void clear() noexcept;

  //! Return the descriptor type
  static constexpr DescriptorType descriptorType() noexcept;

  //! Return the underlying ID data
  IdData& idData() noexcept;

  //! Return the underlying ID data
  const IdData& idData() const noexcept;

  //! Change the number of elements
  virtual void setSize(const std::size_t s) = 0;

  //! Return the number of elements
  virtual std::size_t size() const noexcept = 0;

  //! Return the buffer usage flag
  BufferUsage usage() const noexcept;

  //! Return the sub-platform type
  virtual SubPlatformType type() const noexcept = 0;

 protected:
  //! Clear the contents of the buffer
  virtual void clearData() noexcept = 0;

 private:
  BufferUsage buffer_usage_;
  IdData id_data_;
};

// Type aliases
template <typename Type>
using UniformBuffer = Buffer<DescriptorType::kUniform, Type>;
template <typename Type>
using StorageBuffer = Buffer<DescriptorType::kStorage, Type>;
template <DescriptorType kDescType, typename Type>
using UniqueBuffer = zisc::pmr::unique_ptr<Buffer<kDescType, Type>>;
template <typename Type>
using UniqueUniformBuffer = UniqueBuffer<DescriptorType::kUniform, Type>;
template <typename Type>
using UniqueStorageBuffer = UniqueBuffer<DescriptorType::kStorage, Type>;

} // namespace zinvul

#include "buffer-inl.hpp"

#endif // ZINVUL_BUFFER_HPP

///*!
//  \file buffer.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZINVUL_BUFFER_HPP
//#define ZINVUL_BUFFER_HPP
//
//// Standard C++ library
//#include <cstddef>
//#include <type_traits>
//// Zisc
//#include "zisc/unique_memory_pointer.hpp"
//#include "zisc/non_copyable.hpp"
//// Zinvul
//#include "zinvul/mapped_memory.hpp"
//#include "zinvul/zinvul_config.hpp"
//
//namespace zinvul {
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T>
//class Buffer : private zisc::NonCopyable<Buffer<kDescriptor, T>>
//{
// public:
//  //! The type of the buffer. "const", "volatile" and "reference" are removed
//  using Type = std::remove_cv_t<std::remove_reference_t<T>>;
//  using ConstType = std::add_const_t<Type>;
//  using Pointer = std::add_pointer_t<Type>;
//  using ConstPointer = std::add_pointer_t<ConstType>;
//
//
//  //! Initialize a buffer
//  Buffer(const BufferUsage usage_flag) noexcept;
//
//  //!
//  virtual ~Buffer() noexcept;
//
//
//  //! Return the buffer type
//  static constexpr DescriptorType descriptorType() noexcept;
//
//  //! Return the device type
//  virtual SubPlatformType SubPlatformType() const noexcept = 0;
//
//  //! Check if a buffer memory is on device
//  virtual bool isDeviceMemory() const noexcept = 0;
//
//  //! Check if a buffer memory is on host
//  virtual bool isHostMemory() const noexcept = 0;
//
//  //! Check if a buffer is host visible
//  virtual bool isHostVisible() const noexcept = 0;
//
//  //! Map a buffer memory to a host
//  MappedMemory<kDescriptor, Type> mapMemory() noexcept;
//
//  //! Map a buffer memory to a host
//  MappedMemory<kDescriptor, ConstType> mapMemory() const noexcept;
//
//  //! Return the memory usage
//  virtual std::size_t memoryUsage() const noexcept = 0;
//
//  //! Read a data from a buffer
//  virtual void read(Pointer data,
//                    const std::size_t count,
//                    const std::size_t offset,
//                    const uint32b queue_index) const noexcept = 0;
//
//  //! Change the number of elements
//  virtual void setSize(const std::size_t s) noexcept = 0;
//
//  //! Return the number of elements
//  virtual std::size_t size() const noexcept = 0;
//
//  //! Convert a type of a buffer interface to DstType
//  template <typename DstType>
//  Buffer<kDescriptor, DstType>* treatAs() noexcept;
//
//  //! Convert a type of a buffer interface to DstType
//  template <typename DstType>
//  const Buffer<kDescriptor, DstType>* treatAs() const noexcept;
//
//  //! Return the buffer usage flag
//  BufferUsage usage() const noexcept;
//
//  //! Write a data to a buffer
//  virtual void write(ConstPointer data,
//                     const std::size_t count,
//                     const std::size_t offset,
//                     const uint32b queue_index) noexcept = 0;
//
// private:
//  static_assert(!std::is_pointer_v<T>, "Pointer type is restricted.");
//
//
//  friend MappedMemory<kDescriptor, Type>;
//  friend MappedMemory<kDescriptor, ConstType>;
//
//
//  //! Initialize a buffer
//  void initialize() noexcept;
//
//  //! Map a buffer memory to a host
//  virtual Pointer mappedMemory() const noexcept = 0;
//
//  //! Unmap a buffer memory
//  virtual void unmapMemory() const noexcept = 0;
//
//
//  BufferUsage usage_flag_;
//};
//
//// Type aliases
//template <typename Type>
//using UniformBuffer = Buffer<DescriptorType::kUniform, Type>;
//template <typename Type>
//using StorageBuffer = Buffer<DescriptorType::kStorage, Type>;
//template <DescriptorType kDescriptor, typename Type>
//using UniqueBuffer = zisc::UniqueMemoryPointer<Buffer<kDescriptor, Type>>;
//
//} // namespace zinvul
//
//#include "buffer-inl.hpp"
//
//#endif // ZINVUL_BUFFER_HPP
