/*!
  \file array.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ARRAY_CL
#define ZINVUL_ARRAY_CL


namespace zinvul {

/*!
  \brief Represent an array
  */
template <typename T, size_t kN>
class Array
{
 public:
  // Type aliases
  using Type = T;
  using Reference = T&;
  using ConstReference = const T&;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Iterator = T*;
  using ConstIterator = const T*;


  //! Return an iterator to the first element of the container
   Iterator begin() noexcept;

  //! Return an iterator to the first element of the container
   ConstIterator begin() const noexcept;

  //! Return an iterator to the first element of the container
   ConstIterator cbegin() const noexcept;

  //! Return an iterator following the last element of the container
   Iterator end() noexcept;

  //! Return an iterator following the last element of the container
   ConstIterator end() const noexcept;

  //! Return an iterator following the last element of the container
   ConstIterator cend() const noexcept;


  //! Return the reference by index
   Reference operator[](const size_t index) noexcept;

  //! Return the reference by index
   ConstReference operator[](const size_t index) const noexcept;


  //! Fill the container with specified value
   void fill(ConstReference value) noexcept;

  //! Return the reference by index
   Reference get(const size_t index) noexcept;

  //! Return the reference by index
   ConstReference get(const size_t index) const noexcept;

  //! Return the largest element
   size_t getMaxIndex() const noexcept;

  //! Return the smallest element
   size_t getMinIndex() const noexcept;

  //! Check whether the array has the specified value
   bool hasValue(ConstReference value) const noexcept;

  //! Set value
   void set(const size_t index, ConstReference value) noexcept;

  //! Return the number of elements
  static  size_t size() noexcept;

 private:
  Type data_[kN];
};

} // namespace zinvul

#include "array-inl.cl"

#endif // ZINVUL_ARRAY_CL
