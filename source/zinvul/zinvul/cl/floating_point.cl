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
  using Float2Type =
      ConditionalType<kFormat == FloatingPointFormat::kHalf, half2,
      ConditionalType<kFormat == FloatingPointFormat::kSingle, float2,
      ConditionalType<kFormat == FloatingPointFormat::kDouble, double2, void>>>;
  using BitType =
      ConditionalType<kFormat == FloatingPointFormat::kHalf, uint16b,
      ConditionalType<kFormat == FloatingPointFormat::kSingle, uint32b,
      ConditionalType<kFormat == FloatingPointFormat::kDouble, uint64b, void>>>;
  using Bit2Type =
      ConditionalType<kFormat == FloatingPointFormat::kHalf, ushort2,
      ConditionalType<kFormat == FloatingPointFormat::kSingle, uint2,
      ConditionalType<kFormat == FloatingPointFormat::kDouble, ulong2, void>>>;


  //! Expand an input value to the bittype size
  template <typename UInt>
  static BitType expandToBit(const UInt x) noexcept;

  //! Expand an input value to the bittype size
  template <typename UInt2>
  static Bit2Type expandToBit2(const UInt2 x) noexcept;

  //! Return the exponent bias
  static constexpr size_t exponentBias() noexcept;

  //! Return the exponent bit size
  static constexpr BitType exponentBitMask() noexcept;

  //! Return the exponent bit size
  static constexpr size_t exponentBitSize() noexcept;

  //! Map an unsigned integer into a [0, 1) float
  static FloatType mapTo01(const BitType x) noexcept;

  //! Map an unsigned integer into a [0, 1) float
  static Float2Type mapTo01(const Bit2Type x) noexcept;

  //! Return the sign bit mask
  static constexpr BitType signBitMask() noexcept;

  //! Return the significand bit mask
  static constexpr BitType significandBitMask() noexcept;

  //! Return the significand bit size
  static constexpr size_t significandBitSize() noexcept;
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
