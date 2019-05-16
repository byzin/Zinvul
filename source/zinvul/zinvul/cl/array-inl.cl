/*!
  \file array-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ARRAY_INL_CL
#define ZINVUL_ARRAY_INL_CL

#include "array.cl"
// Zinvul
#include "types.cl"

namespace zinvul {

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::begin() noexcept -> Iterator
{
  auto iterator = data();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::begin() const noexcept -> ConstIterator
{
  auto iterator = data();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::cbegin() const noexcept -> ConstIterator
{
  auto iterator = data();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::end() noexcept -> Iterator
{
  auto iterator = begin() + size();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::end() const noexcept -> ConstIterator
{
  auto iterator = begin() + size();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::cend() const noexcept -> ConstIterator
{
  auto iterator = cbegin() + size();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::operator[](const size_t index) noexcept
    -> Reference
{
  return get(index);
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::operator[](const size_t index) const noexcept
    -> ConstReference
{
  return get(index);
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::data() noexcept -> Pointer
{
  auto p = data_;
  return p;
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::data() const noexcept -> ConstPointer
{
  auto p = data_;
  return p;
}

/*!
  */
template <typename T, size_t kN> inline
void Array<T, kN>::fill(ConstReference value) noexcept
{
  for (size_t index = 0; index < size(); ++index)
    set(index, value);
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::get(const size_t index) noexcept
    -> Reference
{
  return data_[index];
}

/*!
  */
template <typename T, size_t kN> inline
auto Array<T, kN>::get(const size_t index) const noexcept
    -> ConstReference
{
  return data_[index];
}

/*!
  */
template <typename T, size_t kN> inline
bool Array<T, kN>::hasValue(ConstReference value) const noexcept
{
  bool result = false;
  for (size_t index = 0; !result && (index < size()); ++index)
    result = get(index) == value;
  return result;
}

/*!
  */
template <typename T, size_t kN> inline
size_t Array<T, kN>::getMaxIndex() const noexcept
{
  size_t i = 0;
  for (size_t index = 1; index < size(); ++index)
    i = (get(i) < get(index)) ? index : i;
  return i;
}

/*!
  */
template <typename T, size_t kN> inline
size_t Array<T, kN>::getMinIndex() const noexcept
{
  size_t i = 0;
  for (size_t index = 1; index < size(); ++index)
    i = (get(index) < get(i)) ? index : i;
  return i;
}

/*!
  */
template <typename T, size_t kN> inline
void Array<T, kN>::set(const size_t index, ConstReference value) noexcept
{
  data_[index] = value;
}

/*!
  */
template <typename T, size_t kN> inline
constexpr size_t Array<T, kN>::size() noexcept
{
  return kN;
}

} // namespace zinvul

#endif // ZINVUL_ARRAY_INL_CL
