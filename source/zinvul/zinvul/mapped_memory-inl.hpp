/*!
  \file mapped_memory-inl.hpp
  \author Sho Ikeda
  */

#ifndef ZINVUL_MAPPED_MEMORY_INL_HPP
#define ZINVUL_MAPPED_MEMORY_INL_HPP

#include "mapped_memory.hpp"
// Zisc
#include "zisc/error.hpp"
// Zinvul
#include "buffer.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
MappedMemory<kDescriptor, T>::MappedMemory() noexcept
{
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
MappedMemory<kDescriptor, T>::MappedMemory(ConstBufferP buffer) noexcept :
    data_{(buffer != nullptr) ? buffer->mappedMemory() : nullptr},
    buffer_{buffer}
{
  if (buffer_) {
    ZISC_ASSERT(buffer_->isHostVisible(), "The buffer isn't host visible.");
  }
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
MappedMemory<kDescriptor, T>::MappedMemory(MappedMemory&& other) noexcept :
    data_{other.data_},
    buffer_{other.buffer_}
{
  other.data_ = nullptr;
  other.buffer_ = nullptr;
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
MappedMemory<kDescriptor, T>::~MappedMemory() noexcept
{
  unmap();
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::begin() noexcept -> Iterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::begin() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::cbegin() const noexcept -> ConstIterator
{
  auto ite = data();
  ZISC_ASSERT(ite != nullptr, "The data is null.");
  return ite;
}

/*!
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
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::operator=(MappedMemory&& other) noexcept
    -> MappedMemory&
{
  data_ = other.data_;
  buffer_ = other.buffer_;
  other.data_ = nullptr;
  other.buffer_ = nullptr;
  return *this;
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::operator[](const std::size_t index) noexcept
    -> Reference
{
  return get(index);
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::operator[](const std::size_t index) const noexcept
    -> ConstReference
{
  return get(index);
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::data() noexcept -> Pointer
{
  return data_;
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::data() const noexcept -> ConstPointer
{
  return data_;
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::get(const std::size_t index) noexcept
    -> Reference
{
  auto d = data();
  ZISC_ASSERT(d != nullptr, "The data is null.");
  ZISC_ASSERT(index < size(), "The index is out of bounds");
  return d[index];
}

/*!
  */
template <DescriptorType kDescriptor, typename T> inline
auto MappedMemory<kDescriptor, T>::get(const std::size_t index) const noexcept
    -> ConstReference
{
  auto d = data();
  ZISC_ASSERT(d != nullptr, "The data is null.");
  ZISC_ASSERT(index < size(), "The index is out of bounds");
  return d[index];
}

/*!
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
  */
template <DescriptorType kDescriptor, typename T> inline
std::size_t MappedMemory<kDescriptor, T>::size() const noexcept
{
  const std::size_t s = (buffer_ != nullptr) ? buffer_->size() : 0;
  return s;
}

/*!
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
