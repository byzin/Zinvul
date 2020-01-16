/*!
  \file array-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
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
typename Array<T, kN>::Iterator
Array<T, kN>::begin() noexcept
{
  Iterator iterator = data();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::ConstIterator
Array<T, kN>::begin() const noexcept
{
  auto iterator = data();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::ConstIterator
Array<T, kN>::cbegin() const noexcept
{
  auto iterator = data();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::Iterator
Array<T, kN>::end() noexcept
{
  auto iterator = begin() + size();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::ConstIterator
Array<T, kN>::end() const noexcept
{
  auto iterator = begin() + size();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::ConstIterator
Array<T, kN>::cend() const noexcept
{
  auto iterator = cbegin() + size();
  return iterator;
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::Reference
Array<T, kN>::operator[](const size_t index) noexcept
{
  return get(index);
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::ConstReference
Array<T, kN>::operator[](const size_t index) const noexcept
{
  return get(index);
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::Pointer
Array<T, kN>::data() noexcept
{
  auto p = data_;
  return p;
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::ConstPointer
Array<T, kN>::data() const noexcept
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
typename Array<T, kN>::Reference
Array<T, kN>::get(const size_t index) noexcept
{
  return data_[index];
}

/*!
  */
template <typename T, size_t kN> inline
typename Array<T, kN>::ConstReference
Array<T, kN>::get(const size_t index) const noexcept
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
