/*!
  \file floating_point.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_FLOATING_POINT_CL
#define ZINVUL_FLOATING_POINT_CL

// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  */
enum class FloatingPointFormat
{
  kHalf,
  kSingle,
  kDouble
};

/*!
  \brief Replesent a floating point based on IEEE 754
  */
template <FloatingPointFormat kFormat>
class FloatingPoint
{
 public:
  using FloatType =
      ConditionalType<kFormat == FloatingPointFormat::kHalf, half,
      ConditionalType<kFormat == FloatingPointFormat::kSingle, float,
      ConditionalType<kFormat == FloatingPointFormat::kDouble, double, void>>>;
  using BitType =
      ConditionalType<kFormat == FloatingPointFormat::kHalf, uint16b,
      ConditionalType<kFormat == FloatingPointFormat::kSingle, uint32b,
      ConditionalType<kFormat == FloatingPointFormat::kDouble, uint64b, void>>>;
  template <size_t kN>
  using FloatVec = VectorTypeFromElems<FloatType, kN>;
  template <size_t kN>
  using BitVec = VectorTypeFromElems<BitType, kN>;


  //! Return the upscaled bit
  template <FloatingPointFormat kDstFormat, size_t kN>
  static typename FloatingPoint<kDstFormat>::template BitVec<kN>
  downscale(const BitVec<kN> x) noexcept;

  //! Expand an input value to the bittype size
  template <typename UVec>
  static BitVec<VectorTypeInfo<UVec>::size()> expandToBit(const UVec x) noexcept;

  //! Return the exponent bias
  static constexpr size_t exponentBias() noexcept;

  //! Return the exponent bit size
  static constexpr BitType exponentBitMask() noexcept;

  //! Return the exponent bit size
  static constexpr size_t exponentBitSize() noexcept;

  //! Return the positive 1/2 bit value
  static constexpr BitType halfBit() noexcept;

  //! Return the positive infinity value
  static constexpr BitType infinityBit() noexcept;

  //! Check if the given value is infinity
  template <size_t kN>
  static ComparisonResultType<BitVec<kN>> isInfBit(const BitVec<kN> x) noexcept;

  //! Check if the given value is NaN
  template <size_t kN>
  static ComparisonResultType<BitVec<kN>> isNanBit(const BitVec<kN> x) noexcept;

  //! Check if the given value is zero
  template <size_t kN>
  static ComparisonResultType<BitVec<kN>> isZeroBit(const BitVec<kN> x) noexcept;

  //! Map an unsigned integer into a [0, 1) float
  static auto mapTo01(const BitType x) noexcept;

  //! Map an unsigned integer into a [0, 1) float
  static auto mapTo01(const BitVec<2> x) noexcept;

  //! Map an unsigned integer into a [0, 1) float
  static auto mapTo01(const BitVec<3> x) noexcept;

  //! Map an unsigned integer into a [0, 1) float
  static auto mapTo01(const BitVec<4> x) noexcept;

  //! Return the positive one value
  static constexpr BitType oneBit() noexcept;

  //! Return a quiet NaN
  static constexpr BitType quietNanBit() noexcept;

  //! Round to nearest, where ties round to the nearest
  template <size_t kN>
  static BitVec<kN> round(const BitVec<kN> bit,
                          const BitVec<kN> truncated_bit) noexcept;

  //! Return the sign bit mask
  static constexpr BitType signBitMask() noexcept;

  //! Return the significand bit mask
  static constexpr BitType significandBitMask() noexcept;

  //! Return the significand bit size
  static constexpr size_t significandBitSize() noexcept;

  //! Return the upscaled bit
  template <FloatingPointFormat kDstFormat, size_t kN>
  static typename FloatingPoint<kDstFormat>::template BitVec<kN>
  upscale(const BitVec<kN> x) noexcept;

  //! Return the positive zero value
  static constexpr BitType zeroBit() noexcept;

 private:
  //! Map an unsigned integer into a [0, 1) float
  template <size_t kN>
  static auto mapTo01Impl(const BitVec<kN> x) noexcept;
};

// Type alias
using HalfFloat = FloatingPoint<FloatingPointFormat::kHalf>;
using SingleFloat = FloatingPoint<FloatingPointFormat::kSingle>;
using DoubleFloat = FloatingPoint<FloatingPointFormat::kDouble>;
template <size_t kBytes>
using FloatingPointFromBytes =
    ConditionalType<kBytes == sizeof(half), HalfFloat,
    ConditionalType<kBytes == sizeof(float), SingleFloat,
    ConditionalType<kBytes == sizeof(double), DoubleFloat, void>>>;

} // namespace zinvul

#include "floating_point-inl.cl"

#endif // ZINVUL_FLOATING_POINT_CL
