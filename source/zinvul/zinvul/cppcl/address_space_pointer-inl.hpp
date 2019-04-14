/*!
  \file address_space_pointer-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_ADDRESS_SPACE_POINTER_INL_HPP
#define ZINVUL_CL_ADDRESS_SPACE_POINTER_INL_HPP

#include "address_space_pointer.hpp"
// Standard C++ library
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
template <typename T, AddressSpaceType kAddressSpaceType> inline
AddressSpacePointer<T, kAddressSpaceType>::AddressSpacePointer() noexcept :
    data_{nullptr}
{
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
AddressSpacePointer<T, kAddressSpaceType>::AddressSpacePointer(
    Pointer data) noexcept :
        data_{data}
{
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator=(
    Pointer data) noexcept -> Reference
{
  data_ = data;
  return *this;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator*() noexcept
    -> Reference
{
  auto data = get();
  return *data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator*() const noexcept
    -> ConstReference
{
  auto data = get();
  return *data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator->() noexcept
    -> Pointer
{
  auto data = get();
  return data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator->() const noexcept
    -> ConstPointer
{
  auto data = get();
  return data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator[](
    const size_t index) noexcept -> Reference
{
  auto data = get();
  return data[index];
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator[](
    const size_t index) const noexcept -> ConstReference
{
  auto data = get();
  return data[index];
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator+=(
    const ptrdiff_t n) noexcept -> Reference
{
  auto data = get() + n;
  data_ = data;
  return *data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator-=(
    const ptrdiff_t n) noexcept -> Reference
{
  auto data = get() - n;
  data_ = data;
  return *data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator+() noexcept -> Pointer 
{
  auto data = get();
  return data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator++() noexcept
    -> Pointer 
{
  auto data = get() + 1;
  data_ = data;
  return data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator--() noexcept
    -> Pointer 
{
  auto data = get() - 1;
  data_ = data;
  return data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator++(int) noexcept
    -> Pointer 
{
  auto data = get();
  data_ = data + 1;
  return data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::operator--(int) noexcept
    -> Pointer 
{
  auto data = get();
  data_ = data - 1;
  return data;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::get() noexcept
    -> Pointer 
{
  return data_;
}

/*!
  */
template <typename T, AddressSpaceType kAddressSpaceType> inline
auto AddressSpacePointer<T, kAddressSpaceType>::get() const noexcept
    -> ConstPointer 
{
  return data_;
}

/*!
  */
template <typename Type, AddressSpaceType kAddressSpaceType> inline
auto operator+(AddressSpacePointer<Type, kAddressSpaceType>& p,
               const ptrdiff_t n) noexcept
{
  const auto result = p.get() + n;
  return AddressSpacePointer<Type, kAddressSpaceType>{result};
}

/*!
  */
template <typename Type, AddressSpaceType kAddressSpaceType> inline
auto operator+(const ptrdiff_t n,
               AddressSpacePointer<Type, kAddressSpaceType>& p) noexcept
{
  const auto result = p + n;
  return result;
}

/*!
  */
template <typename Type, AddressSpaceType kAddressSpaceType> inline
auto operator-(AddressSpacePointer<Type, kAddressSpaceType>& p,
               const ptrdiff_t n) noexcept
{
  const auto result = p.get() - n;
  return AddressSpacePointer<Type, kAddressSpaceType>{result};
}

/*!
  */
template <typename Type, AddressSpaceType kAddressSpaceType> inline
ptrdiff_t operator-(const AddressSpacePointer<Type, kAddressSpaceType>& lhs,
                    const AddressSpacePointer<Type, kAddressSpaceType>& rhs) noexcept
{
  const ptrdiff_t diff = zisc::cast<ptrdiff_t>(lhs.get() - rhs.get());
  return diff;
}

} // namespace cl

} // namespace zinvul

#endif // ZINVUL_CL_ADDRESS_SPACE_POINTER_INL_HPP
