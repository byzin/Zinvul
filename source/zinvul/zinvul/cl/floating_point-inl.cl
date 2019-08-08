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
#include "math.cl"
#include "relational.cl"
#include "types.cl"
#include "type_traits.cl"
#include "utility.cl"

namespace zinvul {

/*!
  */
template <FloatingPointFormat kFormat>
template <FloatingPointFormat kDstFormat, size_t kN> inline
typename FloatingPoint<kDstFormat>::template BitVec<kN>
FloatingPoint<kFormat>::downscale(const BitVec<kN> x) noexcept
{
  using DstFloat = FloatingPoint<kDstFormat>;
  using DstBitType = typename DstFloat::BitType;
  using DstBitVec = typename DstFloat::template BitVec<kN>;
  using DstResult = ComparisonResultType<DstBitVec>;
  static_assert(sizeof(DstBitVec) < sizeof(BitVec<kN>),
                "The size of dst float should be greater than the size of src.");

  constexpr BitType exp_bias = exponentBias();
  constexpr BitType dst_exp_bias = DstFloat::exponentBias();
  constexpr size_t sig_size = significandBitSize();
  constexpr size_t dst_sig_size = DstFloat::significandBitSize();

  constexpr BitType lower_bound = ((exp_bias + 1) - dst_exp_bias) << sig_size;
  constexpr BitType upper_bound = ((exp_bias + 1) + dst_exp_bias) << sig_size;

  DstBitVec dst_bit = make<DstBitVec>(static_cast<DstBitType>(0));

  // Exponent bits
  {
    constexpr size_t sig_size_diff = sig_size - dst_sig_size;
    constexpr BitType exp_bias_diff = (exp_bias - dst_exp_bias) << dst_sig_size;
    BitVec<kN> src_exp_bit = x & exponentBitMask();
    src_exp_bit = src_exp_bit >> sig_size_diff; 
    src_exp_bit = src_exp_bit - exp_bias_diff;
    dst_bit = cast<DstBitVec>(src_exp_bit) & DstFloat::exponentBitMask();
#if defined(ZINVUL_VULKAN)
    //! \todo [Bug] MSB of exponent bits disappears, remove this macro in future
    constexpr DstBitType l_mask =
        ~static_cast<DstBitType>(DstFloat::signBitMask() >> 1);
    constexpr DstBitType h_mask =
        static_cast<DstBitType>(DstFloat::signBitMask() >> 1);
    constexpr BitType middle = 
        static_cast<BitType>(dst_exp_bias + 1) << dst_sig_size;
    const DstBitVec dst_bit_l = dst_bit & l_mask;
    const DstBitVec dst_bit_h = dst_bit | h_mask;
    const DstResult flag = cast<DstResult>(middle <= src_exp_bit);
    dst_bit = zinvul::select(dst_bit_l, dst_bit_h, flag); 
#endif
  }

  // Significand bits
  {
    constexpr size_t sig_size_diff = sig_size - dst_sig_size;
    const BitVec<kN> src_sig_bit = x & significandBitMask();
    const DstBitVec dst_sig_bit = cast<DstBitVec>(src_sig_bit >> sig_size_diff);
    dst_bit = dst_bit | dst_sig_bit;
  }

  // Rounding
  {
    constexpr size_t sig_size_diff = sig_size - dst_sig_size;
    const BitVec<kN> src_t_bit = cast<BitVec<kN>>(x << dst_sig_size) &
                                 significandBitMask();
    const DstBitVec truncated_bit = cast<DstBitVec>(src_t_bit >> sig_size_diff);
    dst_bit = DstFloat::template round<kN>(dst_bit, truncated_bit);
  }

  // Special case
  {
    const BitVec<kN> src_exp_bit = x & exponentBitMask();
    // Zero
    const auto zero_bit = make<DstBitVec>(DstFloat::zeroBit());
    DstResult flag = cast<DstResult>(src_exp_bit < lower_bound) | // Subnormal
                     cast<DstResult>(isZeroBit<kN>(x)); // Input is zero
    dst_bit = zinvul::select(dst_bit, zero_bit, flag);
    // Inf
    const auto inf_bit = make<DstBitVec>(DstFloat::infinityBit());
    flag = cast<DstResult>(upper_bound <= src_exp_bit) | // Overflow
           cast<DstResult>(isInfBit<kN>(x)); // Input is inf
    dst_bit = zinvul::select(dst_bit, inf_bit, flag);
    // Nan
    const auto nan_bit = make<DstBitVec>(DstFloat::quietNanBit());
    flag = cast<DstResult>(isNanBit<kN>(x));
    dst_bit = zinvul::select(dst_bit, nan_bit, flag);
  }

  // Sign bit
  {
    constexpr size_t bit_size = exponentBitSize() + sig_size + 1;
    constexpr size_t dst_bit_size = DstFloat::exponentBitSize() + dst_sig_size + 1;
    constexpr size_t bit_size_diff = bit_size - dst_bit_size;
    const BitVec<kN> src_sign_bit = x & signBitMask();
    const DstBitVec dst_sign_bit = cast<DstBitVec>(src_sign_bit >> bit_size_diff);
    dst_bit = dst_sign_bit | dst_bit;
  }

  return dst_bit;
}

/*!
  */
template <FloatingPointFormat kFormat> template <typename UVec> inline
auto FloatingPoint<kFormat>::expandToBit(const UVec x) noexcept
    -> BitVec<VectorType<UVec>::size()>
{
  using UVecData = VectorType<UVec>;
  using BVec = BitVec<UVecData::size()>;
  using BVecData = VectorType<BVec>;

  constexpr size_t int_size = sizeof(typename UVecData::ElementType);
  constexpr size_t bit_size = sizeof(typename BVecData::ElementType);
  if constexpr (int_size == bit_size) {
    return x;
  }
  else if constexpr (int_size < bit_size) {
    constexpr size_t diff = bit_size - int_size;
    const auto result = cast<BVec>(x) << (diff * 8);
    return result;
  }
  else {
    constexpr size_t diff = int_size - bit_size;
    const auto result = cast<BVec>(x >> (diff * 8));
    return result;
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
constexpr auto FloatingPoint<kFormat>::infinityBit() noexcept -> BitType
{
  const BitType value = exponentBitMask();
  return value;
}

/*!
  */
template <FloatingPointFormat kFormat> template <size_t kN> inline
auto FloatingPoint<kFormat>::isInfBit(const BitVec<kN> x) noexcept
    -> ComparisonResultType<BitVec<kN>>
{
  constexpr BitType zero = static_cast<BitType>(0);
  const auto exp_bit = x & exponentBitMask();
  const auto sig_bit = x & significandBitMask();
  const auto result = (exp_bit == exponentBitMask()) && (sig_bit == zero);
  return cast<ComparisonResultType<BitVec<kN>>>(result);
}

/*!
  */
template <FloatingPointFormat kFormat> template <size_t kN> inline
auto FloatingPoint<kFormat>::isNanBit(const BitVec<kN> x) noexcept
    -> ComparisonResultType<BitVec<kN>>
{
  constexpr BitType zero = static_cast<BitType>(0);
  const auto exp_bit = x & exponentBitMask();
  const auto sig_bit = x & significandBitMask();
  const auto result = (exp_bit == exponentBitMask()) && (zero < sig_bit);
  return cast<ComparisonResultType<BitVec<kN>>>(result);
}

/*!
  */
template <FloatingPointFormat kFormat> template <size_t kN> inline
auto FloatingPoint<kFormat>::isZeroBit(const BitVec<kN> x) noexcept
    -> ComparisonResultType<BitVec<kN>>
{
  constexpr BitType zero = static_cast<BitType>(0);
  constexpr BitType value_bit_mask = static_cast<BitType>(~signBitMask());
  const auto value_bit = x & value_bit_mask;
  const auto result = value_bit == zero;
  return cast<ComparisonResultType<BitVec<kN>>>(result);
}

/*!
  */
template <FloatingPointFormat kFormat> inline
void FloatingPoint<kFormat>::mapTo01(const BitType x,
                                     GenericPtr<FloatType> result) noexcept
{
  if constexpr (kFormat != FloatingPointFormat::kHalf)
    *result = mapTo01Impl<1>(x);
}

/*!
  */
template <FloatingPointFormat kFormat> inline
void FloatingPoint<kFormat>::mapTo01(const BitVec<2> x,
                                     GenericPtr<FloatVec<2>> result) noexcept
{
  if constexpr (kFormat != FloatingPointFormat::kHalf)
    *result = mapTo01Impl<2>(x);
}

/*!
  */
template <FloatingPointFormat kFormat> inline
void FloatingPoint<kFormat>::mapTo01(const BitVec<3> x,
                                     GenericPtr<FloatVec<3>> result) noexcept
{
  if constexpr (kFormat != FloatingPointFormat::kHalf)
    *result = mapTo01Impl<3>(x);
}

/*!
  */
template <FloatingPointFormat kFormat> inline
void FloatingPoint<kFormat>::mapTo01(const BitVec<4> x,
                                     GenericPtr<FloatVec<4>> result) noexcept
{
  if constexpr (kFormat != FloatingPointFormat::kHalf)
    *result = mapTo01Impl<4>(x);
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::oneBit() noexcept -> BitType
{
  const auto exp_bit = static_cast<BitType>(exponentBias() << significandBitSize());
  return exp_bit;
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::quietNanBit() noexcept -> BitType
{
  const BitType exp_bit = exponentBitMask();
  const BitType significand_bit = BitType{0b1u} << (significandBitSize() - 1);
  const BitType value = exp_bit | significand_bit;
  return value;
}

/*!
  */
template <FloatingPointFormat kFormat> template <size_t kN> inline
auto FloatingPoint<kFormat>::round(
    const BitVec<kN> bit,
    const BitVec<kN> truncated_bit) noexcept -> BitVec<kN>
{
  using CmpResult = ComparisonResultType<BitVec<kN>>;
  constexpr BitType one = BitType{0b01};
  constexpr BitType middle = one << (significandBitSize() - 1);
  const CmpResult flag = (cast<CmpResult>(truncated_bit == middle) & isOdd(bit)) |
                         cast<CmpResult>(middle < truncated_bit);
  const BitVec<kN> next = bit + one;
  const BitVec<kN> result = zinvul::select(bit, next, flag);
  return result;
}

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::signBitMask() noexcept -> BitType
{
  const BitType mask = BitType{1u} << (exponentBitSize() + significandBitSize());
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

/*!
  */
template <FloatingPointFormat kFormat> inline
constexpr auto FloatingPoint<kFormat>::zeroBit() noexcept -> BitType
{
  const BitType value = 0;
  return value;
}

/*!
  */
template <FloatingPointFormat kFormat>
template <FloatingPointFormat kDstFormat, size_t kN> inline
typename FloatingPoint<kDstFormat>::template BitVec<kN>
FloatingPoint<kFormat>::upscale(const BitVec<kN> x) noexcept
{
  using DstFloat = FloatingPoint<kDstFormat>;
  using DstBitType = typename DstFloat::BitType;
  using DstBitVec = typename DstFloat::template BitVec<kN>;
  using DstResult = ComparisonResultType<DstBitVec>;
  static_assert(sizeof(BitVec<kN>) < sizeof(DstBitVec),
                "The size of dst float should be greater than the size of src.");

  constexpr DstBitType exp_bias = exponentBias();
  constexpr DstBitType dst_exp_bias = DstFloat::exponentBias();
  constexpr size_t sig_size = significandBitSize();
  constexpr size_t dst_sig_size = DstFloat::significandBitSize();

  DstBitVec dst_bit = make<DstBitVec>(static_cast<DstBitType>(0));

  // Exponent bits
  {
    constexpr size_t sig_size_diff = dst_sig_size - sig_size;
    constexpr DstBitType exp_bias_diff = (dst_exp_bias - exp_bias) << dst_sig_size;
    const BitVec<kN> src_exp_bit = x & exponentBitMask();
    dst_bit = cast<DstBitVec>(src_exp_bit) << sig_size_diff;
    dst_bit = dst_bit + exp_bias_diff;
  }

  // Significand bits
  {
    constexpr size_t sig_size_diff = dst_sig_size - sig_size;
    const BitVec<kN> src_sig_bit = x & significandBitMask();
    const DstBitVec dst_sig_bit = cast<DstBitVec>(src_sig_bit) << sig_size_diff;
    dst_bit = dst_bit | dst_sig_bit;
  }

  // Special case
  {
    const BitVec<kN> src_exp_bit = x & exponentBitMask();
    // Zero
    const auto zero_bit = make<DstBitVec>(DstFloat::zeroBit());
    DstResult flag = cast<DstResult>(src_exp_bit == zeroBit()) | // Subnormal
                     cast<DstResult>(isZeroBit<kN>(x)); // Input is zero
    dst_bit = zinvul::select(dst_bit, zero_bit, flag);
    // Inf
    const auto inf_bit = make<DstBitVec>(DstFloat::infinityBit());
    flag = cast<DstResult>(isInfBit<kN>(x)); // Input is inf
    dst_bit = zinvul::select(dst_bit, inf_bit, flag);
    // Nan
    const auto nan_bit = make<DstBitVec>(DstFloat::quietNanBit());
    flag = cast<DstResult>(isNanBit<kN>(x));
    dst_bit = zinvul::select(dst_bit, nan_bit, flag);
  }

  // Sign bit
  {
    constexpr size_t bit_size = exponentBitSize() + sig_size + 1;
    constexpr size_t dst_bit_size = DstFloat::exponentBitSize() + dst_sig_size + 1;
    constexpr size_t bit_size_diff = dst_bit_size - bit_size;
    const BitVec<kN> src_sign_bit = x & signBitMask();
    const DstBitVec dst_sign_bit = cast<DstBitVec>(src_sign_bit) << bit_size_diff;
    dst_bit = dst_sign_bit | dst_bit;
  }

  return dst_bit;
}

/*!
  */
template <FloatingPointFormat kFormat> template <size_t kN> inline
auto FloatingPoint<kFormat>::mapTo01Impl(const BitVec<kN> x) noexcept
    -> FloatVec<kN>
{
  constexpr auto k =
      static_cast<FloatType>(1.0) /
      static_cast<FloatType>(static_cast<BitType>(1) << (significandBitSize() + 1));
  return k * cast<FloatVec<kN>>(x >> exponentBitSize());
}

} // namespace zinvul

#endif // ZINVUL_FLOATING_POINT_INL_CL
