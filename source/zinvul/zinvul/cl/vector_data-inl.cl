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
template <size_t kN, typename Type> inline
auto VectorData::load(const size_t offset, ConstGlobalPtr<Type> p) noexcept
    -> VecType<Type, kN>
{
  const auto data = loadImpl<Type, kN>(offset, p);
  return data;
}

/*!
  */
template <size_t kN, typename Type> inline
auto VectorData::load(const size_t offset, ConstLocalPtr<Type> p) noexcept
    -> VecType<Type, kN>
{
  const auto data = loadImpl<Type, kN>(offset, p);
  return data;
}

/*!
  */
template <size_t kN, typename Type> inline
auto VectorData::load(const size_t offset, ConstConstantPtr<Type> p) noexcept
    -> VecType<Type, kN>
{
  const auto data = loadImpl<Type, kN>(offset, p);
  return data;
}

/*!
  */
template <size_t kN, typename Type> inline
auto VectorData::load(const size_t offset, ConstPrivatePtr<Type> p) noexcept
    -> VecType<Type, kN>
{
  const auto data = loadImpl<Type, kN>(offset, p);
  return data;
}

/*!
  */
template <size_t kN, typename Type> inline
auto VectorData::loadHalf(const size_t offset, ConstGlobalPtr<Type> p) noexcept
    -> FloatVec<kN>
{
  const auto data = loadHalfImpl<Type, kN>(offset, p);
  return data;
}

/*!
  */
template <size_t kN, typename Type> inline
auto VectorData::loadHalf(const size_t offset, ConstLocalPtr<Type> p) noexcept
    -> FloatVec<kN>
{
  const auto data = loadHalfImpl<Type, kN>(offset, p);
  return data;
}

/*!
  */
template <size_t kN, typename Type> inline
auto VectorData::loadHalf(const size_t offset, ConstConstantPtr<Type> p) noexcept
    -> FloatVec<kN>
{
  const auto data = loadHalfImpl<Type, kN>(offset, p);
  return data;
}

/*!
  */
template <size_t kN, typename Type> inline
auto VectorData::loadHalf(const size_t offset, ConstPrivatePtr<Type> p) noexcept
    -> FloatVec<kN>
{
  const auto data = loadHalfImpl<Type, kN>(offset, p);
  return data;
}

/*!
  */
template <typename VectorType, typename Type> inline
void VectorData::store(const VectorType data,
                       const size_t offset,
                       GlobalPtr<Type> p) noexcept
{
  storeImpl(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void VectorData::store(const VectorType data,
                       const size_t offset,
                       LocalPtr<Type> p) noexcept
{
  storeImpl(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void VectorData::store(const VectorType data,
                       const size_t offset,
                       PrivatePtr<Type> p) noexcept
{
  storeImpl(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void VectorData::storeHalf(const VectorType data,
                           const size_t offset,
                           GlobalPtr<Type> p) noexcept
{
  storeHalfImpl<Type>(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void VectorData::storeHalf(const VectorType data,
                           const size_t offset,
                           LocalPtr<Type> p) noexcept
{
  storeHalfImpl<Type>(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void VectorData::storeHalf(const VectorType data,
                           const size_t offset,
                           PrivatePtr<Type> p) noexcept
{
  storeHalfImpl<Type>(data, offset, p);
}

/*!
  */
template <typename Type, size_t kN, typename AddressType> inline
auto VectorData::loadImpl(const size_t offset, AddressType p) noexcept
    -> VecType<Type, kN>
{
  if constexpr (kN == 1) {
    const auto data = p[offset];
    return data;
  }
  else if constexpr (kN == 2) {
    const auto data = ZINVUL_GLOBAL_NAMESPACE::vload2(offset, p);
    return data;
  }
  else if constexpr (kN == 3) {
    const auto data = ZINVUL_GLOBAL_NAMESPACE::vload3(offset, p);
    return data;
  }
  else if constexpr (kN == 4) {
    const auto data = ZINVUL_GLOBAL_NAMESPACE::vload4(offset, p);
    return data;
  }
  else {
    static_assert(0 < kN, "The size of vector is wrong.");
  }
}

/*!
  */
template <typename Type, size_t kN, typename AddressType> inline
auto VectorData::loadHalfImpl(const size_t offset, AddressType p) noexcept
    -> FloatVec<kN>
{
  using TypeInfo = VectorTypeInfo<RemoveCvType<Type>>;
  static_assert(TypeInfo::size() == 1, "The Type isn't scalar type.");
  using ElemType = typename TypeInfo::ElementType;
  if constexpr (kIsHalf<ElemType>) {
#if defined(ZINVUL_CPU)
    if constexpr (kN == 1) {
      const auto data = ZINVUL_GLOBAL_NAMESPACE::vload_half(offset, p);
      return data;
    }
    else if constexpr (kN == 2) {
      const auto data = ZINVUL_GLOBAL_NAMESPACE::vload_half2(offset, p);
      return data;
    }
    else if constexpr (kN == 3) {
      const auto data = ZINVUL_GLOBAL_NAMESPACE::vload_half3(offset, p);
      return data;
    }
    else if constexpr (kN == 4) {
      const auto data = ZINVUL_GLOBAL_NAMESPACE::vload_half4(offset, p);
      return data;
    }
    else {
      static_assert(0 < kN, "The size of vector is wrong.");
    }
#else // ZINVUL_CPU
    (void)offset;
    (void)p;
    const auto data = make<FloatVec<kN>>(0.0f);
    return data;
#endif // ZINVUL_CPU
  }
  else if constexpr (kIsSame<uint16b, ElemType>) {
    const auto data = loadHalfUImpl<Type, kN>(offset, p);
    return data;
  }
  else {
    static_assert(sizeof(ElemType) == 0, "The input type is unsupported.");
  }
}

/*!
  */
template <typename Type, size_t kN, typename AddressType> inline
auto VectorData::loadHalfUImpl(const size_t offset, AddressType p) noexcept
    -> FloatVec<kN>
{
  using TypeInfo = VectorTypeInfo<RemoveCvType<Type>>;
  static_assert(TypeInfo::size() == 1, "The Type isn't scalar type.");
  using ElemType = typename TypeInfo::ElementType;
  static_assert(kIsSame<uint16b, ElemType>, "The Type isn't uint16b.");

  const auto data = load<kN, Type>(offset, p);
  const auto udata = HalfFloat::upscale<FloatingPointFormat::kSingle, kN>(data);
  const auto fdata = treatAs<FloatVec<kN>>(udata);
  return fdata;
}

/*!
  */
template <typename VectorType, typename AddressType> inline
void VectorData::storeImpl(const VectorType data,
                           const size_t offset,
                           AddressType p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  if constexpr (n == 1) {
    p[offset] = data;
  }
  else if constexpr (n == 2) {
    ZINVUL_GLOBAL_NAMESPACE::vstore2(data, offset, p);
  }
  else if constexpr (n == 3) {
    ZINVUL_GLOBAL_NAMESPACE::vstore3(data, offset, p);
  }
  else if constexpr (n == 4) {
    ZINVUL_GLOBAL_NAMESPACE::vstore4(data, offset, p);
  }
  else {
    static_assert(0 < n, "The size of vector is wrong.");
  }
}

/*!
  */
template <typename Type, typename VectorType, typename AddressType> inline
void VectorData::storeHalfImpl(const VectorType data,
                               const size_t offset,
                               AddressType p) noexcept
{
  // Source type
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  using ElemType = typename VecInfo::ElementType;
  static_assert(kIsSingleFloat<ElemType>, "The VectorType isn't float type.");
  constexpr size_t n = VecInfo::size();
  // Destination type
  if constexpr (kIsHalf<RemoveCvType<Type>>) {
#if defined(ZINVUL_CPU)
    if constexpr (n == 1) {
      ZINVUL_GLOBAL_NAMESPACE::vstore_half(data, offset, p);
    }
    else if constexpr (n == 2) {
      ZINVUL_GLOBAL_NAMESPACE::vstore_half2(data, offset, p);
    }
    else if constexpr (n == 3) {
      ZINVUL_GLOBAL_NAMESPACE::vstore_half3(data, offset, p);
    }
    else if constexpr (n == 4) {
      ZINVUL_GLOBAL_NAMESPACE::vstore_half4(data, offset, p);
    }
    else {
      static_assert(0 < n, "The size of vector is wrong.");
    }
#else // ZINVUL_CPU
    (void)data;
    (void)offset;
    (void)p;
#endif // ZINVUL_CPU
  }
  else if constexpr (kIsSame<uint16b, RemoveCvType<Type>>) {
    storeHalfUImpl<Type>(data, offset, p);
  }
  else {
    static_assert(sizeof(ElemType) == 0, "The input type is unsupported.");
  }
}

/*!
  */
template <typename Type, typename VectorType, typename AddressType> inline
void VectorData::storeHalfUImpl(const VectorType fdata,
                                const size_t offset,
                                AddressType p) noexcept
{
  // Source type
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  using ElemType = typename VecInfo::ElementType;
  static_assert(kIsSingleFloat<ElemType>, "The VectorType isn't float type.");
  constexpr size_t n = VecInfo::size();
  // Destination type
  static_assert(kIsSame<uint16b, RemoveCvType<Type>>, "The Type isn't uint16b.");
  using BitVec = SingleFloat::BitVec<n>;

  const auto udata = treatAs<BitVec>(fdata);
  const auto data = SingleFloat::downscale<FloatingPointFormat::kHalf, n>(udata);
  store(data, offset, p);
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 2> vload2(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept
{
  const auto data = VectorData::load<2>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 3> vload3(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept
{
  const auto data = VectorData::load<3>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 4> vload4(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept
{
  const auto data = VectorData::load<4>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 2> vload2(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept
{
  const auto data = VectorData::load<2>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 3> vload3(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept
{
  const auto data = VectorData::load<3>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 4> vload4(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept
{
  const auto data = VectorData::load<4>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 2> vload2(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept
{
  const auto data = VectorData::load<2>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 3> vload3(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept
{
  const auto data = VectorData::load<3>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 4> vload4(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept
{
  const auto data = VectorData::load<4>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 2> vload2(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept
{
  const auto data = VectorData::load<2>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 3> vload3(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept
{
  const auto data = VectorData::load<3>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::VecType<Type, 4> vload4(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept
{
  const auto data = VectorData::load<4>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<1> vload_half(const size_t offset,
                                   ConstGlobalPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<1>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<2> vload_half2(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<2>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<3> vload_half3(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<3>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<4> vload_half4(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<4>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<1> vload_half(const size_t offset,
                                   ConstLocalPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<1>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<2> vload_half2(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<2>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<3> vload_half3(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<3>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<4> vload_half4(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<4>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<1> vload_half(const size_t offset,
                                   ConstConstantPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<1>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<2> vload_half2(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<2>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<3> vload_half3(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<3>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<4> vload_half4(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<4>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<1> vload_half(const size_t offset,
                                   ConstPrivatePtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<1>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<2> vload_half2(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<2>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<3> vload_half3(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<3>(offset, p);
  return data;
}

/*!
  */
template <typename Type> inline
VectorData::FloatVec<4> vload_half4(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept
{
  const auto data = VectorData::loadHalf<4>(offset, p);
  return data;
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore2(const VectorType data,
             const size_t offset,
             GlobalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore3(const VectorType data,
             const size_t offset,
             GlobalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore4(const VectorType data,
             const size_t offset,
             GlobalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore2(const VectorType data,
             const size_t offset,
             LocalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore3(const VectorType data,
             const size_t offset,
             LocalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore4(const VectorType data,
             const size_t offset,
             LocalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore2(const VectorType data,
             const size_t offset,
             PrivatePtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore3(const VectorType data,
             const size_t offset,
             PrivatePtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore4(const VectorType data,
             const size_t offset,
             PrivatePtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half(const VectorType data,
                 const size_t offset,
                 GlobalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 1, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half2(const VectorType data,
                  const size_t offset,
                  GlobalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half3(const VectorType data,
                  const size_t offset,
                  GlobalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half4(const VectorType data,
                  const size_t offset,
                  GlobalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half(const VectorType data,
                 const size_t offset,
                 LocalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 1, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half2(const VectorType data,
                  const size_t offset,
                  LocalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half3(const VectorType data,
                  const size_t offset,
                  LocalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half4(const VectorType data,
                  const size_t offset,
                  LocalPtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half(const VectorType data,
                 const size_t offset,
                 PrivatePtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 1, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half2(const VectorType data,
                  const size_t offset,
                  PrivatePtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half3(const VectorType data,
                  const size_t offset,
                  PrivatePtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename Type> inline
void vstore_half4(const VectorType data,
                  const size_t offset,
                  PrivatePtr<Type> p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

} // namespace zinvul

#endif /* ZINVUL_VECTOR_DATA_INL_CL */
