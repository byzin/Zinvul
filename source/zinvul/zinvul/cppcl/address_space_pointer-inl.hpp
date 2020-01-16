/*!
  \file address_space_pointer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_ADDRESS_SPACE_POINTER_INL_HPP
#define ZINVUL_CL_ADDRESS_SPACE_POINTER_INL_HPP

#include "address_space_pointer.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/utility.hpp"
// Zinvul
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
AddressSpacePointer<kAddressSpaceType, T>::AddressSpacePointer() noexcept :
    data_{nullptr}
{
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
AddressSpacePointer<kAddressSpaceType, T>::AddressSpacePointer(
    Pointer data) noexcept :
        data_{data}
{
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
AddressSpacePointer<kAddressSpaceType, T>::AddressSpacePointer(
    AddressSpacePointer<kAddressSpaceType, PlainType>& other) noexcept :
        data_{other.get()}
{
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator=(
    Pointer data) noexcept -> ASpacePointerRef
{
  data_ = data;
  return *this;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator=(
    AddressSpacePointer<kAddressSpaceType, PlainType>& other) noexcept -> ASpacePointerRef
{
  data_ = other.get();
  return *this;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator*() noexcept
    -> Reference
{
  auto data = get();
  return *data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator*() const noexcept
    -> ConstReference
{
  auto data = get();
  return *data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator->() noexcept
    -> Pointer
{
  auto data = get();
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator->() const noexcept
    -> ConstPointer
{
  auto data = get();
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator[](
    const size_t index) noexcept -> Reference
{
  auto data = get();
  return data[index];
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator[](
    const size_t index) const noexcept -> ConstReference
{
  auto data = get();
  return data[index];
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator+=(
    const ptrdiff_t n) noexcept -> ASpacePointerRef
{
  auto data = get() + n;
  data_ = data;
  return *this;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator+=(
    const size_t n) noexcept -> ASpacePointerRef
{
  ASpacePointerRef data = (*this += zisc::cast<ptrdiff_t>(n));
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator-=(
    const ptrdiff_t n) noexcept -> ASpacePointerRef
{
  auto data = get() - n;
  data_ = data;
  return *this;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator-=(
    const size_t n) noexcept -> ASpacePointerRef
{
  ASpacePointerRef data = (*this -= zisc::cast<ptrdiff_t>(n));
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator+() noexcept -> Pointer 
{
  auto data = get();
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator++() noexcept
    -> Pointer 
{
  auto data = get() + 1;
  data_ = data;
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator--() noexcept
    -> Pointer 
{
  auto data = get() - 1;
  data_ = data;
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator++(int) noexcept
    -> Pointer 
{
  auto data = get();
  data_ = data + 1;
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::operator--(int) noexcept
    -> Pointer 
{
  auto data = get();
  data_ = data - 1;
  return data;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::get() noexcept
    -> Pointer 
{
  return data_;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename T> inline
auto AddressSpacePointer<kAddressSpaceType, T>::get() const noexcept
    -> ConstPointer 
{
  return data_;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto operator+(AddressSpacePointer<kAddressSpaceType, Type>& p,
               const ptrdiff_t n) noexcept
    -> AddressSpacePointer<kAddressSpaceType, Type>
{
  const auto result = p.get() + n;
  return AddressSpacePointer<kAddressSpaceType, Type>{result};
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto operator+(AddressSpacePointer<kAddressSpaceType, Type>& p,
               const size_t n) noexcept
    -> AddressSpacePointer<kAddressSpaceType, Type>
{
  auto result = p + zisc::cast<ptrdiff_t>(n);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto operator+(const ptrdiff_t n,
               AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
    -> AddressSpacePointer<kAddressSpaceType, Type>
{
  const auto result = p + n;
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto operator+(const size_t n,
               AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept
    -> AddressSpacePointer<kAddressSpaceType, Type>
{
  auto result = zisc::cast<ptrdiff_t>(n) + p;
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto operator-(AddressSpacePointer<kAddressSpaceType, Type>& p,
               const ptrdiff_t n) noexcept
    -> AddressSpacePointer<kAddressSpaceType, Type>
{
  const auto result = p.get() - n;
  return AddressSpacePointer<kAddressSpaceType, Type>{result};
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
auto operator-(AddressSpacePointer<kAddressSpaceType, Type>& p,
               const size_t n) noexcept
    -> AddressSpacePointer<kAddressSpaceType, Type>
{
  auto result = p - zisc::cast<ptrdiff_t>(n);
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
ptrdiff_t operator-(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
                    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const ptrdiff_t diff = zisc::cast<ptrdiff_t>(lhs.get() - rhs.get());
  return diff;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator==(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
                const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() == rhs.get();
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator==(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
                const std::nullptr_t rhs) noexcept
{
  const bool result = lhs.get() == rhs;
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator==(const std::nullptr_t lhs,
                const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const bool result = lhs == rhs.get();
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator!=(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
                const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() != rhs.get();
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator!=(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
                const std::nullptr_t rhs) noexcept
{
  const bool result = lhs.get() != rhs;
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator!=(const std::nullptr_t lhs,
                const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const bool result = lhs != rhs.get();
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator<(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
               const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() < rhs.get();
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator<=(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
                const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() <= rhs.get();
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator>(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
               const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() > rhs.get();
  return result;
}

/*!
  */
template <AddressSpaceType kAddressSpaceType, typename Type> inline
bool operator>=(const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
                const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept
{
  const bool result = lhs.get() >= rhs.get();
  return result;
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_ADDRESS_SPACE_POINTER_INL_HPP
