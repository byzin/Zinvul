/*!
  \file vector_data-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VECTOR_DATA_INL_CL
#define ZINVUL_VECTOR_DATA_INL_CL

#include "vector_data.cl"
// Zinvul
#include "floating_point.cl"
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  */
template <typename Type> inline
auto VectorData<Type>::load(const size_t offset, ConstGlobalPtr<DataType> p)
    noexcept -> ValueType
{
  return loadImpl(offset, p);
}

/*!
  */
template <typename Type> inline
auto VectorData<Type>::load(const size_t offset, ConstLocalPtr<DataType> p)
    noexcept -> ValueType
{
  return loadImpl(offset, p);
}

/*!
  */
template <typename Type> inline
auto VectorData<Type>::load(const size_t offset, ConstConstantPtr<DataType> p)
    noexcept -> ValueType
{
  return loadImpl(offset, p);
}

/*!
  */
template <typename Type> inline
auto VectorData<Type>::load(const size_t offset, ConstPrivatePtr<DataType> p)
    noexcept -> ValueType
{
  return loadImpl(offset, p);
}

/*!
  */
template <typename Type> inline
auto VectorData<Type>::loadHalfU(const size_t offset, ConstGlobalPtr<DataType> p)
    noexcept -> FloatType
{
  static_assert(kIsSame<uint16b, DataType>, "The DataType isn't uint16b.");
  return loadHalfUImpl(offset, p);
}

/*!
  */
template <typename Type> inline
auto VectorData<Type>::loadHalfU(const size_t offset, ConstLocalPtr<DataType> p)
    noexcept -> FloatType
{
  static_assert(kIsSame<uint16b, DataType>, "The DataType isn't uint16b.");
  return loadHalfUImpl(offset, p);
}

/*!
  */
template <typename Type> inline
auto VectorData<Type>::loadHalfU(const size_t offset, ConstConstantPtr<DataType> p)
    noexcept -> FloatType
{
  static_assert(kIsSame<uint16b, DataType>, "The DataType isn't uint16b.");
  return loadHalfUImpl(offset, p);
}

/*!
  */
template <typename Type> inline
auto VectorData<Type>::loadHalfU(const size_t offset, ConstPrivatePtr<DataType> p)
    noexcept -> FloatType
{
  static_assert(kIsSame<uint16b, DataType>, "The DataType isn't uint16b.");
  return loadHalfUImpl(offset, p);
}

/*!
  */
template <typename Type> inline
constexpr size_t VectorData<Type>::size() noexcept
{
  return VecType::size();
}

/*!
  */
template <typename Type> inline
void VectorData<Type>::store(const ValueType data,
                             const size_t offset,
                             GlobalPtr<DataType> p) noexcept
{
  storeImpl(data, offset, p);
}

/*!
  */
template <typename Type> inline
void VectorData<Type>::store(const ValueType data,
                             const size_t offset,
                             LocalPtr<DataType> p) noexcept
{
  storeImpl(data, offset, p);
}

/*!
  */
template <typename Type> inline
void VectorData<Type>::store(const ValueType data,
                             const size_t offset,
                             PrivatePtr<DataType> p) noexcept
{
  storeImpl(data, offset, p);
}

/*!
  */
template <typename Type> inline
void VectorData<Type>::storeHalfU(const FloatType data,
                                  const size_t offset,
                                  GlobalPtr<DataType> p) noexcept
{
  storeHalfUImpl(data, offset, p);
}

/*!
  */
template <typename Type> inline
void VectorData<Type>::storeHalfU(const FloatType data,
                                  const size_t offset,
                                  LocalPtr<DataType> p) noexcept
{
  storeHalfUImpl(data, offset, p);
}

/*!
  */
template <typename Type> inline
void VectorData<Type>::storeHalfU(const FloatType data,
                                  const size_t offset,
                                  PrivatePtr<DataType> p) noexcept
{
  storeHalfUImpl(data, offset, p);
}

/*!
  */
template <typename Type> template <typename AddressType> inline
auto VectorData<Type>::loadImpl(const size_t offset, AddressType p)
    noexcept -> ValueType
{
  if constexpr (kIsHalf<Type>) {
    const auto data = loadHalfImpl(offset, p);
    return data;
  }
  else {
    if constexpr (size() == 1) {
      const auto data = p[offset];
      return data;
    }
    else if constexpr (size() == 2) {
      const auto data = vload2(offset, p);
      return data;
    }
    else if constexpr (size() == 3) {
      const auto data = vload3(offset, p);
      return data;
    }
    else if constexpr (size() == 4) {
      const auto data = vload4(offset, p);
      return data;
    }
    else {
      static_assert(0 < size(), "The size of vector is wrong.");
    }
  }
}

/*!
  */
template <typename Type> template <typename AddressType> inline
auto VectorData<Type>::loadHalfImpl(const size_t offset, AddressType p)
    noexcept -> ValueType
{
#if defined(ZINVUL_CPU)
  if constexpr (size() == 1) {
    const auto data = vload_half(offset, p);
    return data;
  }
  else if constexpr (size() == 2) {
    const auto data = vload_half2(offset, p);
    return data;
  }
  else if constexpr (size() == 3) {
    const auto data = vload_half3(offset, p);
    return data;
  }
  else if constexpr (size() == 4) {
    const auto data = vload_half4(offset, p);
    return data;
  }
  else {
    static_assert(0 < size(), "The size of vector is wrong.");
  }
#else // ZINVUL_CPU
  (void)offset;
  (void)p;
  return ValueType{};
#endif // ZINVUL_CPU
}

/*!
  */
template <typename Type> template <typename AddressType> inline
auto VectorData<Type>::loadHalfUImpl(const size_t offset, AddressType p)
    noexcept -> FloatType
{
  static_assert(kIsSame<uint16b, DataType>, "The DataType isn't uint16b.");
  constexpr size_t n = size();

  const auto data = load(offset, p);
  const auto udata = HalfFloat::upscale<FloatingPointFormat::kSingle, n>(data);
  const auto fdata = treatAs<FloatType>(udata);
  return fdata;
}

/*!
  */
template <typename Type> template <typename AddressType> inline
void VectorData<Type>::storeImpl(const ValueType data,
                                 const size_t offset,
                                 AddressType p) noexcept
{
  if constexpr (kIsHalf<Type>) {
    storeHalfImpl(data, offset, p);
  }
  else {
    if constexpr (size() == 1) {
      p[offset] = data;
    }
    else if constexpr (size() == 2) {
      vstore2(data, offset, p);
    }
    else if constexpr (size() == 3) {
      vstore3(data, offset, p);
    }
    else if constexpr (size() == 4) {
      vstore4(data, offset, p);
    }
    else {
      static_assert(0 < size(), "The size of vector is wrong.");
    }
  }
}

/*!
  */
template <typename Type> template <typename AddressType> inline
void VectorData<Type>::storeHalfImpl(const ValueType data,
                                     const size_t offset,
                                     AddressType p) noexcept
{
#if defined(ZINVUL_CPU)
  if constexpr (size() == 1) {
    vstore_half(data, offset, p);
  }
  else if constexpr (size() == 2) {
    vstore_half2(data, offset, p);
  }
  else if constexpr (size() == 3) {
    vstore_half3(data, offset, p);
  }
  else if constexpr (size() == 4) {
    vstore_half4(data, offset, p);
  }
  else {
    static_assert(0 < size(), "The size of vector is wrong.");
  }
#else // ZINVUL_CPU
  (void)data;
  (void)offset;
  (void)p;
#endif // ZINVUL_CPU
}

/*!
  */
template <typename Type> template <typename AddressType> inline
void VectorData<Type>::storeHalfUImpl(const FloatType fdata,
                                      const size_t offset,
                                      AddressType p) noexcept
{
  static_assert(kIsSame<uint16b, DataType>, "The DataType isn't uint16b.");
  constexpr size_t n = size();
  using BitType = SingleFloat::BitVec<n>;

  const auto udata = treatAs<BitType>(fdata);
  const auto data = SingleFloat::downscale<FloatingPointFormat::kHalf, n>(udata);
  store(data, offset, p);
}

} // namespace zinvul

#endif /* ZINVUL_VECTOR_DATA_INL_CL */
