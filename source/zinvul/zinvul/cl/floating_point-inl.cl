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

namespace zinvul {

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
  BitType mask = 0;
  for (size_t bit = 0; bit < exponentBitSize(); ++bit) {
    const BitType one = 1u;
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
  const auto f = static_cast<FloatType>(x >> exponentBitSize()) *
                 5.960464478e-08f; // 2^(-24)
  return f;
}

/*!
  */
//template <FloatingPointFormat kFormat> inline
//auto FloatingPoint<kFormat>::mapTo01(const Bit2Type x) noexcept -> Float2Type
//{
//  const auto f = static_cast<Float2Type>(x >> exponentBitSize()) *
//                 5.960464478e-08f; // 2^(-24)
//  return f;
//}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::signBitMask() noexcept -> BitType
{
  const auto mask = BitType{1u} << (exponentBitSize() + significandBitSize());
  return mask;
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::significandBitMask() noexcept -> BitType
{
  BitType mask = 0;
  for (size_t bit = 0; bit < significandBitSize(); ++bit) {
    const BitType one = 1u;
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
