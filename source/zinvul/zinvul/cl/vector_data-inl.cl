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
template <typename Type> template <typename AddressType> inline
auto VectorData<Type>::loadImpl(const size_t offset, AddressType p)
    noexcept -> ValueType
{
  if constexpr (kIsHalfType<Type>) {
    const auto data = loadHalfImpl(offset, p);
    return data;
  }
  else {
    auto ptr = getPtr(p);
    if constexpr (size() == 1) {
      const auto data = ptr[offset];
      return data;
    }
    else if constexpr (size() == 2) {
      const auto data = vload2(offset, ptr);
      return data;
    }
    else if constexpr (size() == 3) {
      const auto data = vload3(offset, ptr);
      return data;
    }
    else if constexpr (size() == 4) {
      const auto data = vload4(offset, ptr);
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
  static_assert(!kIsHalfType<Type>, "The loadHalf isn't implemented yet.");
//  using UVecData = VectorData<uint16b>;
//
//  const auto ptr = toConstData(p);
//  const auto udata = UVecData::load(offset, ptr);
//
//  return ValueType{};
}

/*!
  */
template <typename Type> template <typename AddressType> inline
void VectorData<Type>::storeImpl(const ValueType data,
                                 const size_t offset,
                                 AddressType p) noexcept
{
  if constexpr (kIsHalfType<Type>) {
    storeHalfImpl(data, offset, p);
  }
  else {
    auto ptr = getPtr(p);
    if constexpr (size() == 1) {
      ptr[offset] = data;
    }
    else if constexpr (size() == 2) {
      vstore2(data, offset, ptr);
    }
    else if constexpr (size() == 3) {
      vstore3(data, offset, ptr);
    }
    else if constexpr (size() == 4) {
      vstore4(data, offset, ptr);
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
  static_assert(!kIsHalfType<Type>, "The storeHalf isn't implemented yet.");
//  using UVecData = VectorData<uint16b>;
//
//  const UVecData::ValueType udata{};
//  auto ptr = toData(p);
////  UVecData::store(udata, offset, ptr);
//  if constexpr (size() == 1) {
//    ptr[0] = udata;
//  }
}

/*!
  */
//template <typename Type> inline
//ConstGlobalPtr<uint16b> VectorData<Type>::toConstData(ConstGlobalPtr<half> p)
//    noexcept
//{
//  auto ptr = treatAs<ConstGlobalPtr<uint16b>>(p);
//  return ptr;
//}
//
///*!
//  */
//template <typename Type> inline
//ConstLocalPtr<uint16b> VectorData<Type>::toConstData(ConstLocalPtr<half> p)
//    noexcept
//{
//  auto ptr = treatAs<ConstLocalPtr<uint16b>>(p);
//  return ptr;
//}
//
///*!
//  */
//template <typename Type> inline
//ConstConstantPtr<uint16b> VectorData<Type>::toConstData(ConstConstantPtr<half> p)
//    noexcept
//{
//  auto ptr = treatAs<ConstConstantPtr<uint16b>>(p);
//  return ptr;
//}
//
///*!
//  */
//template <typename Type> inline
//ConstPrivatePtr<uint16b> VectorData<Type>::toConstData(ConstPrivatePtr<half> p)
//    noexcept
//{
//  auto ptr = treatAs<ConstPrivatePtr<uint16b>>(p);
//  return ptr;
//}
//
///*!
//  */
//template <typename Type> inline
//GlobalPtr<uint16b> VectorData<Type>::toData(GlobalPtr<half> p) noexcept
//{
//  auto ptr = treatAs<GlobalPtr<uint16b>>(p);
//  return ptr;
//}
//
///*!
//  */
//template <typename Type> inline
//LocalPtr<uint16b> VectorData<Type>::toData(LocalPtr<half> p) noexcept
//{
//  auto ptr = treatAs<LocalPtr<uint16b>>(p);
//  return ptr;
//}
//
///*!
//  */
//template <typename Type> inline
//PrivatePtr<uint16b> VectorData<Type>::toData(PrivatePtr<half> p) noexcept
//{
//  auto ptr = treatAs<PrivatePtr<uint16b>>(p);
//  return ptr;
//}

} // namespace zinvul

#endif /* ZINVUL_VECTOR_DATA_INL_CL */
