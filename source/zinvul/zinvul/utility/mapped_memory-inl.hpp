/*!
  \file mapped_memory-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MAPPED_MEMORY_INL_HPP
#define ZINVUL_MAPPED_MEMORY_INL_HPP

#include "mapped_memory.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "buffer.hpp"
#include "zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
template <DescriptorType kDescriptor, typename T> inline
MappedMemory<kDescriptor, T>::MappedMemory() noexcept
{
}

/*!
  \details No detailed description

  \param [in] buffer No description.
  */
template <DescriptorType kDescriptor, typename T> inline
MappedMemory<kDescriptor, T>::MappedMemory(ConstBufferP buffer) noexcept :
    data_{buffer ? buffer->mappedMemory() : nullptr},
    buffer_{buffer}
{
  if (buffer_) {
    ZISC_ASSERT(buffer_->isHostVisible(), "The buffer isn't host visible.");
  }
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  */
template <DescriptorType kDescriptor, typename T> inline
MappedMemory<kDescriptor, T>::MappedMemory(MappedMemory&& other) noexcept
{
  zisc::swap(data_, other.data_);
  zisc::swap(buffer_, other.buffer_);
}

/*!
  \details No detailed description
  */
template <DescriptorType kDescriptor, typename T> inline
MappedMemory<kDescriptor, T>::~MappedMemory() noexcept
{
  unmap();
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::begin() noexcept -> Iterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::begin() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::cbegin() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::end() noexcept -> Iterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  ite = ite + size();
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::end() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  ite = ite + size();
  return ite;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::cend() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  ite = ite + size();
  return ite;
}

/*!
  \details No detailed description

  \param [in,out] other No description.
  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::operator=(MappedMemory&& other) noexcept
    -> MappedMemory&
{
  zisc::swap(data_, other.data_);
  zisc::swap(buffer_, other.buffer_);
  return *this;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::operator[](const std::size_t index) noexcept
    -> Reference
{
  return get(index);
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::operator[](const std::size_t index) const noexcept
    -> ConstReference
{
  return get(index);
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::data() noexcept -> Pointer
{
  return data_;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::data() const noexcept -> ConstPointer
{
  return data_;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::get(const std::size_t index) noexcept
    -> Reference
{
  auto d = data();
  ZISC_ASSERT(d != nullptr, "The data is null.");
  ZISC_ASSERT(index < size(), "The index is out of bounds");
  Reference value = d[index];
  return value;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::get(const std::size_t index) const noexcept
    -> ConstReference
{
  auto d = data();
  ZISC_ASSERT(d != nullptr, "The data is null.");
  ZISC_ASSERT(index < size(), "The index is out of bounds");
  ConstReference value = d[index];
  return value;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \param [in] value No description.
  */
template <DescriptorType kDescriptor, typename T> inline
void MappedMemory<kDescriptor, T>::set(const std::size_t index,
                                       ConstReference value) noexcept
{
  auto d = data();
  ZISC_ASSERT(d != nullptr, "The data is null.");
  ZISC_ASSERT(index < size(), "The index is out of bounds");
  d[index] = value;
}

/*!
  \details No detailed description

  \return No description
  */
template <DescriptorType kDescriptor, typename T> inline
std::size_t MappedMemory<kDescriptor, T>::size() const noexcept
{
  const std::size_t s = (buffer_ != nullptr) ? buffer_->size() : 0;
  return s;
}

/*!
  \details No detailed description
  */
template <DescriptorType kDescriptor, typename T> inline
void MappedMemory<kDescriptor, T>::unmap() noexcept
{
  if (buffer_ != nullptr)
    buffer_->unmapMemory();
  data_ = nullptr;
  buffer_ = nullptr;
}

} // namespace zinvul

#endif // ZINVUL_MAPPED_MEMORY_INL_HPP
