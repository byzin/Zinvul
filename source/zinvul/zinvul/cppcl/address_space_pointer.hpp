/*!
  \file address_space_pointer.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_ADDRESS_SPACE_POINTER_HPP
#define ZINVUL_CL_ADDRESS_SPACE_POINTER_HPP

// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zinvul
#include "types.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

/*!
  \brief Represent address space type
 */
enum class AddressSpaceType: uint32b
{
  kGlobal = 0,
  kLocal,
  kConstant,
  kPrivate
};

/*!
 \brief Represent a address space pointer
 */
template <AddressSpaceType kAddressSpaceType, typename T>
class AddressSpacePointer
{
  static_assert(!std::is_pointer_v<T>, "The 'T' is pointer type.");
  static_assert(!std::is_reference_v<T>, "The 'T' is reference type.");
 public:
  using Type = std::remove_volatile_t<T>;
  using PlainType = std::remove_const_t<Type>;
  using ASpacePointerRef = std::add_lvalue_reference_t<AddressSpacePointer>;
  using ConstASpacePointerRef = std::add_const_t<ASpacePointerRef>;
  using Reference = std::add_lvalue_reference_t<Type>;
  using ConstReference = std::add_const_t<Reference>;
  using Pointer = std::add_pointer_t<Type>;
  using ConstPointer = std::add_pointer_t<std::add_const_t<Type>>;


  //! Initialize a pointer by nullptr
  AddressSpacePointer() noexcept;

  //! Initialize a pointer
  AddressSpacePointer(Pointer data) noexcept;

  //! Initialize a pointer by other pointer
  AddressSpacePointer(AddressSpacePointer<kAddressSpaceType, PlainType>& other)
      noexcept;

  //! Assign a pointer
  ASpacePointerRef operator=(Pointer data) noexcept;

  //! Assign a pointer
  ASpacePointerRef operator=(AddressSpacePointer<kAddressSpaceType, PlainType>& other) noexcept;

  //! Check whether this owns an object
  explicit operator bool() const noexcept
  {
    return data_ != nullptr;
  }

  //! Dereference pointer to the managed object
  Reference operator*() noexcept;

  //! Dereference pointer to the managed object
  ConstReference operator*() const noexcept;

  //! Dereference pointer to the managed object
  Pointer operator->() noexcept;

  //! Dereference pointer to the managed object
  ConstPointer operator->() const noexcept;

  //! Return the reference by index
  Reference operator[](const size_t index) noexcept;

  //! Return the reference by index
  ConstReference operator[](const size_t index) const noexcept;

  //! Get a pointer to n-th element and assign it
  ASpacePointerRef operator+=(const ptrdiff_t n) noexcept;

  //! Get a pointer to n-th element and assign it
  ASpacePointerRef operator+=(const size_t n) noexcept;

  //! Get a pointer to -n-th element and assign it
  ASpacePointerRef operator-=(const ptrdiff_t n) noexcept;

  //! Get a pointer to -n-th element and assign it
  ASpacePointerRef operator-=(const size_t n) noexcept;

  //! Return a pointer to the managed object
  Pointer operator+() noexcept;

  //! Pre-increment a pointer address
  Pointer operator++() noexcept;

  //! Pre-decrement a pointer address
  Pointer operator--() noexcept;

  //! Post-increment a pointer address
  Pointer operator++(int) noexcept;

  //! Post-decrement a pointer address
  Pointer operator--(int) noexcept;


  //! Return a pointer to the managed object
  Pointer get() noexcept;

  //! Return a pointer to the managed object
  ConstPointer get() const noexcept;

 private:
  Pointer data_ = nullptr;
};

//! Compute a pointer to n-th element
template <AddressSpaceType kAddressSpaceType, typename Type>
AddressSpacePointer<kAddressSpaceType, Type> operator+(
    AddressSpacePointer<kAddressSpaceType, Type>& p,
    const ptrdiff_t n) noexcept;

//! Compute a pointer to n-th element
template <AddressSpaceType kAddressSpaceType, typename Type>
AddressSpacePointer<kAddressSpaceType, Type> operator+(
    AddressSpacePointer<kAddressSpaceType, Type>& p,
    const size_t n) noexcept;

//! Compute a pointer to n-th element
template <AddressSpaceType kAddressSpaceType, typename Type>
AddressSpacePointer<kAddressSpaceType, Type> operator+(
    const ptrdiff_t n,
    AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Compute a pointer to n-th element
template <AddressSpaceType kAddressSpaceType, typename Type>
AddressSpacePointer<kAddressSpaceType, Type> operator+(
    const size_t n,
    AddressSpacePointer<kAddressSpaceType, Type>& p) noexcept;

//! Compute a pointer to -n-th element
template <AddressSpaceType kAddressSpaceType, typename Type>
AddressSpacePointer<kAddressSpaceType, Type> operator-(
    AddressSpacePointer<kAddressSpaceType, Type>& p,
    const ptrdiff_t n) noexcept;

//! Compute a pointer to -n-th element
template <AddressSpaceType kAddressSpaceType, typename Type>
AddressSpacePointer<kAddressSpaceType, Type> operator-(
    AddressSpacePointer<kAddressSpaceType, Type>& p,
    const size_t n) noexcept;

//! Compute the distance between lhs and rhs
template <AddressSpaceType kAddressSpaceType, typename Type>
ptrdiff_t operator-(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

//! Return true if lhs is equal to rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator==(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

//! Return true if lhs is equal to rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator==(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const std::nullptr_t rhs) noexcept;

//! Return true if lhs is equal to rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator==(
    const std::nullptr_t lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

//! Return true if lhs is not equal to rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator!=(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

//! Return true if lhs is not equal to rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator!=(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const std::nullptr_t rhs) noexcept;

//! Return true if lhs is not equal to rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator!=(
    const std::nullptr_t lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

//! Return true if lhs is less than rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator<(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

//! Return true if lhs is less than or equal rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator<=(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

//! Return true if lhs is greater than rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator>(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

//! Return true if lhs is greater than or equal rhs, false otherwise
template <AddressSpaceType kAddressSpaceType, typename Type>
bool operator>=(
    const AddressSpacePointer<kAddressSpaceType, Type>& lhs,
    const AddressSpacePointer<kAddressSpaceType, Type>& rhs) noexcept;

} // namespace cl

} // namespace zinvul

#include "address_space_pointer-inl.hpp"

#endif // ZINVUL_CL_ADDRESS_SPACE_POINTER_HPP
