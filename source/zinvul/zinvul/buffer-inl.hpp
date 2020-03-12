/*!
  \file buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUFFER_INL_HPP
#define ZINVUL_BUFFER_INL_HPP

#include "buffer.hpp"
// Standard C++ library
#include <type_traits>
#include <utility>
// Zinvul
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
template <DescriptorType kDescType, typename T> inline
Buffer<kDescType, T>::Buffer(const BufferUsage buffer_usage,
                             IdData&& id_data) noexcept :
    buffer_usage_{buffer_usage},
    id_data_{std::move(id_data)}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
template <DescriptorType kDescType, typename T> inline
Buffer<kDescType, T>::Buffer(Buffer&& other) noexcept :
    buffer_usage_{other.buffer_usage_},
    id_data_{std::move(other.id_data_)}
{
}

/*!
  \details No detailed description
  */
template <DescriptorType kDescType, typename T> inline
Buffer<kDescType, T>::~Buffer() noexcept
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
template <DescriptorType kDescType, typename T> inline
auto Buffer<kDescType, T>::operator=(Buffer&& other) noexcept -> Buffer&
{
  buffer_usage_ = other.buffer_usage_;
  id_data_ = std::move(other.id_data_);
  return *this;
}

template <DescriptorType kDescType, typename T> inline
void Buffer<kDescType, T>::clear() noexcept
{
  clearData();
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescType, typename T> inline
constexpr DescriptorType Buffer<kDescType, T>::descriptorType() noexcept
{
  return kDescType;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescType, typename T> inline
IdData& Buffer<kDescType, T>::idData() noexcept
{
  return id_data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescType, typename T> inline
const IdData& Buffer<kDescType, T>::idData() const noexcept
{
  return id_data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescType, typename T> inline
BufferUsage Buffer<kDescType, T>::usage() const noexcept
{
  return buffer_usage_;
}

} // namespace zinvul

#endif // ZINVUL_BUFFER_INL_HPP

///*!
//  \file buffer-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZINVUL_BUFFER_INL_HPP
//#define ZINVUL_BUFFER_INL_HPP
//
//#include "buffer.hpp"
//// Zisc
//#include "zisc/error.hpp"
//#include "zisc/utility.hpp"
//// Zinvul
//#include "mapped_memory.hpp"
//#include "zinvul/zinvul_config.hpp"
//
//namespace zinvul {
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//Buffer<kDescriptor, T>::Buffer(const BufferUsage usage_flag) noexcept :
//    usage_flag_{usage_flag}
//{
//  initialize();
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//Buffer<kDescriptor, T>::~Buffer() noexcept
//{
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//constexpr DescriptorType Buffer<kDescriptor, T>::descriptorType() noexcept
//{
//  return kDescriptor;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto Buffer<kDescriptor, T>::mapMemory() noexcept
//    -> MappedMemory<kDescriptor, Type>
//{
//  using MappedMem = MappedMemory<kDescriptor, Type>;
//  typename MappedMem::ConstBufferP p = isHostVisible() ? this : nullptr;
//  MappedMem memory{p};
//  return memory;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto Buffer<kDescriptor, T>::mapMemory() const noexcept
//    -> MappedMemory<kDescriptor, ConstType>
//{
//  using MappedMem = MappedMemory<kDescriptor, ConstType>;
//  typename MappedMem::ConstBufferP p = isHostVisible() ? this : nullptr;
//  MappedMem memory{p};
//  return memory;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> template <typename DstType> inline
//Buffer<kDescriptor, DstType>* Buffer<kDescriptor, T>::treatAs() noexcept
//{
//  using DstBuffer = Buffer<kDescriptor, DstType>;
//  DstBuffer* dst = zisc::treatAs<DstBuffer*>(this);
//  return dst;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> template <typename DstType> inline
//const Buffer<kDescriptor, DstType>* Buffer<kDescriptor, T>::treatAs() const noexcept
//{
//  using DstBuffer = Buffer<kDescriptor, DstType>;
//  const DstBuffer* dst = zisc::treatAs<const DstBuffer*>(this);
//  return dst;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//BufferUsage Buffer<kDescriptor, T>::usage() const noexcept
//{
//  return usage_flag_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void Buffer<kDescriptor, T>::initialize() noexcept
//{
//}
//
//} // namespace zinvul
//
//#endif // ZINVUL_BUFFER_INL_HPP
