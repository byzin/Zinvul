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
#include <memory>
#include <type_traits>
#include <utility>
// Zisc
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"
#include "utility/id_data.hpp"
#include "utility/zinvul_object.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
template <typename T> inline
Buffer<T>::Buffer(IdData&& id) noexcept : ZinvulObject(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
Buffer<T>::~Buffer() noexcept
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
void Buffer<T>::destroy() noexcept
{
  clear();
  destroyObject();
}

/*!
  \details No detailed description
  */
template <typename T> inline
void Buffer<T>::clear() noexcept
{
  destroyData();
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] buffer_usage No description.
  */
template <typename T> inline
void Buffer<T>::initialize(ZinvulObject::SharedPtr&& parent,
                           WeakPtr&& own,
                           const BufferUsage buffer_usage)
{
  //! Clear the previous device data first
  destroy();

  initObject(std::move(parent), std::move(own));
  buffer_usage_ = buffer_usage;
  initData();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
BufferUsage Buffer<T>::usage() const noexcept
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
