/*!
  \file floating_point-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_FLOATING_POINT_INL_CL
#define ZINVUL_FLOATING_POINT_INL_CL

#include "floating_point.cl"
// Zinvul
#include "types.cl"
#include "utility.cl"

namespace zinvul {

/*!
  */
template <FloatingPointFormat kFormat> template <typename UInt> inline
auto FloatingPoint<kFormat>::expandToBit(const UInt x) noexcept -> BitType
{
  constexpr size_t bit_size = sizeof(BitType);
  constexpr size_t int_size = sizeof(UInt);
  if constexpr (int_size == bit_size) {
    return x;
  }
  else if constexpr (int_size < bit_size) {
    constexpr size_t diff = bit_size - int_size;
    return cast<BitType>(x) << (diff * 8);
  }
  else {
    constexpr size_t diff = int_size - bit_size;
    return cast<BitType>(x >> (diff * 8));
  }
}

/*!
  */
template <FloatingPointFormat kFormat> template <typename UInt2> inline
auto FloatingPoint<kFormat>::expandToBit2(const UInt2 x) noexcept -> Bit2Type
{
  constexpr size_t bit_size = sizeof(Bit2Type) / 2;
  constexpr size_t int_size = sizeof(UInt2) / 2;
  if constexpr (int_size == bit_size) {
    return x;
  }
  else if constexpr (int_size < bit_size) {
    constexpr size_t diff = bit_size - int_size;
    Private<Bit2Type> y{cast<BitType>(x.x), cast<BitType>(x.y)};
    return y << (diff * 8);
  }
  else {
    constexpr size_t diff = int_size - bit_size;
    Private<Bit2Type> y{cast<BitType>(x.x) >> (diff * 8),
                        cast<BitType>(x.y) >> (diff * 8)};
    return y;
  }
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr size_t FloatingPoint<kFormat>::exponentBias() noexcept
{
  const size_t bias = (1u << (exponentBitSize() - 1)) - 1u;
  return bias;
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::exponentBitMask() noexcept -> BitType
{
  Private<BitType> mask = 0;
  for (size_t bit = 0; bit < exponentBitSize(); ++bit) {
    const Private<BitType> one = 1u;
    mask = mask | static_cast<BitType>(one << (significandBitSize() + bit));
  }
  return mask;
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr size_t FloatingPoint<kFormat>::exponentBitSize() noexcept
{
  const size_t size = (kFormat == FloatingPointFormat::kHalf) ? 5 :
                      (kFormat == FloatingPointFormat::kSingle) ? 8 :
                      (kFormat == FloatingPointFormat::kDouble) ? 11 : 1;
  return size;
}

/*!
  */
template <FloatingPointFormat kFormat> inline
auto FloatingPoint<kFormat>::mapTo01(const BitType x) noexcept -> FloatType
{
  constexpr auto k =
      static_cast<FloatType>(1.0) /
      static_cast<FloatType>(static_cast<BitType>(1) << (significandBitSize() + 1));
  return k * cast<FloatType>(x >> exponentBitSize());
}

/*!
  */
template <FloatingPointFormat kFormat> inline
auto FloatingPoint<kFormat>::mapTo01(const Bit2Type x) noexcept -> Float2Type
{
  constexpr auto k =
      static_cast<FloatType>(1.0) /
      static_cast<FloatType>(static_cast<BitType>(1) << (significandBitSize() + 1));
  return k * cast<Float2Type>(x >> exponentBitSize());
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::signBitMask() noexcept -> BitType
{
  const Private<BitType> mask = BitType{1u} <<
                                (exponentBitSize() + significandBitSize());
  return mask;
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::significandBitMask() noexcept -> BitType
{
  Private<BitType> mask = 0;
  for (size_t bit = 0; bit < significandBitSize(); ++bit) {
    const Private<BitType> one = 1u;
    mask = mask | static_cast<BitType>(one << bit);
  }
  return mask;
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr size_t FloatingPoint<kFormat>::significandBitSize() noexcept
{
  const size_t size = (kFormat == FloatingPointFormat::kHalf) ? 10 :
                      (kFormat == FloatingPointFormat::kSingle) ? 23 :
                      (kFormat == FloatingPointFormat::kDouble) ? 52 : 1;
  return size;
}

} // namespace zinvul

#endif // ZINVUL_FLOATING_POINT_INL_CL
