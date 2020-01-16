/*!
  \file vector_data-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
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
template <size_t kN, typename AddressSpaceType> inline
auto VectorData::load(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = loadImpl<kN>(offset, p);
  return data;
}

/*!
  */
template <size_t kN, typename AddressSpaceType> inline
auto VectorData::loadHalf(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = loadHalfImpl<kN>(offset, p);
  return data;
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void VectorData::store(const VectorType data,
                       const size_t offset,
                       AddressSpaceType p) noexcept
{
  storeImpl(data, offset, p);
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void VectorData::storeHalf(const VectorType data,
                           const size_t offset,
                           AddressSpaceType p) noexcept
{
  storeHalfImpl(data, offset, p);
}

/*!
  */
template <size_t kN, typename AddressSpaceType> inline
auto VectorData::loadImpl(const size_t offset, AddressSpaceType p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

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
    return 0;
  }
}

/*!
  */
template <size_t kN, typename AddressSpaceType> inline
auto VectorData::loadHalfImpl(const size_t offset, AddressSpaceType p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");
  using ElemType = typename ASpaceInfo::DataType;

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
    const auto data = loadHalfUImpl<kN>(offset, p);
    return data;
  }
  else {
    static_assert(sizeof(ElemType) == 0, "The type isn't half type.");
  }
}

/*!
  */
template <size_t kN, typename AddressSpaceType> inline
auto VectorData::loadHalfUImpl(const size_t offset, AddressSpaceType p) noexcept
{
  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");
  using ElemType = typename ASpaceInfo::DataType;
  static_assert(kIsSame<uint16b, ElemType>, "The Type isn't uint16b.");

  const auto data = load<kN>(offset, p);
  const auto udata = HalfFloat::upscale<FloatingPointFormat::kSingle, kN>(data);
  const auto fdata = treatAs<FloatVec<kN>>(udata);
  return fdata;
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void VectorData::storeImpl(const VectorType data,
                           const size_t offset,
                           AddressSpaceType p) noexcept
{
  // Source type check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");

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
template <typename VectorType, typename AddressSpaceType> inline
void VectorData::storeHalfImpl(const VectorType data,
                               const size_t offset,
                               AddressSpaceType p) noexcept
{
  // Source type check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  using FloatType = typename VecInfo::ElementType;
  static_assert(kIsSingleFloat<FloatType>, "The VectorType isn't float type.");
  constexpr size_t n = VecInfo::size();

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");
  using ElemType = typename ASpaceInfo::DataType;

  // Destination type
  if constexpr (kIsHalf<ElemType>) {
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
  else if constexpr (kIsSame<uint16b, ElemType>) {
    storeHalfUImpl(data, offset, p);
  }
  else {
    static_assert(sizeof(ElemType) == 0, "The input type isn't half type.");
  }
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void VectorData::storeHalfUImpl(const VectorType fdata,
                                const size_t offset,
                                AddressSpaceType p) noexcept
{
  // Source type check
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  using FloatType = typename VecInfo::ElementType;
  static_assert(kIsSingleFloat<FloatType>, "The VectorType isn't float type.");
  constexpr size_t n = VecInfo::size();

  // Address space type check
  using ASpaceInfo = AddressSpaceInfo<AddressSpaceType>;
  static_assert(ASpaceInfo::isPointer(), "The p isn't pointer type.");
  using ElemType = typename ASpaceInfo::DataType;
  static_assert(kIsSame<uint16b, ElemType>, "The Type isn't uint16b.");

  using BitVec = SingleFloat::BitVec<n>;
  const auto udata = treatAs<BitVec>(fdata);
  const auto data = SingleFloat::downscale<FloatingPointFormat::kHalf, n>(udata);
  store(data, offset, p);
}

/*!
  */
template <typename AddressSpaceType> inline
auto vload2(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<2>(offset, p);
  return data;
}

/*!
  */
template <typename AddressSpaceType> inline
auto vload3(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<3>(offset, p);
  return data;
}

/*!
  */
template <typename AddressSpaceType> inline
auto vload4(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::load<4>(offset, p);
  return data;
}

/*!
  */
template <typename AddressSpaceType> inline
auto vload_half(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::loadHalf<1>(offset, p);
  return data;
}

/*!
  */
template <typename AddressSpaceType> inline
auto vload_half2(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::loadHalf<2>(offset, p);
  return data;
}

/*!
  */
template <typename AddressSpaceType> inline
auto vload_half3(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::loadHalf<3>(offset, p);
  return data;
}

/*!
  */
template <typename AddressSpaceType> inline
auto vload_half4(const size_t offset, AddressSpaceType p) noexcept
{
  const auto data = VectorData::loadHalf<4>(offset, p);
  return data;
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore2(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore3(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore4(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::store(data, offset, p);
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore_half(const VectorType data,
                 const size_t offset,
                 AddressSpaceType p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 1, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore_half2(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 2, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore_half3(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 3, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

/*!
  */
template <typename VectorType, typename AddressSpaceType> inline
void vstore_half4(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept
{
  using VecInfo = VectorTypeInfo<RemoveCvType<VectorType>>;
  constexpr size_t n = VecInfo::size();
  static_assert(n == 4, "The size of VectorType is wrong.");

  VectorData::storeHalf(data, offset, p);
}

} // namespace zinvul

#endif /* ZINVUL_VECTOR_DATA_INL_CL */
