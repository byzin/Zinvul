/*!
  \file limits-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_LIMITS_INL_CL
#define ZINVUL_LIMITS_INL_CL

#include "limits.cl"
// Zinvul
#include "types.cl"
#include "utility.cl"

namespace zinvul {

template <>
class NumericLimits<int8b>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr int8b min() noexcept
  {
    return SCHAR_MIN;
  }

  static constexpr int8b lowest() noexcept
  {
    return min();
  }

  static constexpr int8b max() noexcept
  {
    return SCHAR_MAX;
  }

  static constexpr int8b epsilon() noexcept
  {
    return 2;
  }

  static int8b infinity() noexcept
  {
    return max();
  }

  static int8b quietNan() noexcept
  {
    return 0;
  }
};

template <>
class NumericLimits<uint8b>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr uint8b min() noexcept
  {
    return 0;
  }

  static constexpr uint8b lowest() noexcept
  {
    return min();
  }

  static constexpr uint8b max() noexcept
  {
    return UCHAR_MAX;
  }

  static constexpr uint8b epsilon() noexcept
  {
    return 2;
  }

  static uint8b infinity() noexcept
  {
    return max();
  }

  static uint8b quietNan() noexcept
  {
    return 0;
  }
};

template <>
class NumericLimits<int16b>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr int16b min() noexcept
  {
    return SHRT_MIN;
  }

  static constexpr int16b lowest() noexcept
  {
    return min();
  }

  static constexpr int16b max() noexcept
  {
    return SHRT_MAX;
  }

  static constexpr int16b epsilon() noexcept
  {
    return 2;
  }

  static int16b infinity() noexcept
  {
    return max();
  }

  static int16b quietNan() noexcept
  {
    return 0;
  }
};

template <>
class NumericLimits<uint16b>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr uint16b min() noexcept
  {
    return 0;
  }

  static constexpr uint16b lowest() noexcept
  {
    return 0;
  }

  static constexpr uint16b max() noexcept
  {
    return USHRT_MAX;
  }

  static constexpr uint16b epsilon() noexcept
  {
    return 2;
  }

  static uint16b infinity() noexcept
  {
    return max();
  }

  static uint16b quietNan() noexcept
  {
    return 0;
  }
};

template <>
class NumericLimits<int32b>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr int32b min() noexcept
  {
    return INT_MIN;
  }

  static constexpr int32b lowest() noexcept
  {
    return min();
  }

  static constexpr int32b max() noexcept
  {
    return INT_MAX;
  }

  static constexpr int32b epsilon() noexcept
  {
    return 2;
  }

  static int32b infinity() noexcept
  {
    return max();
  }

  static int32b quietNan() noexcept
  {
    return 0;
  }
};

template <>
class NumericLimits<uint32b>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr uint32b min() noexcept
  {
    return 0;
  }

  static constexpr uint32b lowest() noexcept
  {
    return min();
  }

  static constexpr uint32b max() noexcept
  {
    return UINT_MAX;
  }

  static constexpr uint32b epsilon() noexcept
  {
    return 2;
  }

  static uint32b infinity() noexcept
  {
    return max();
  }

  static uint32b quietNan() noexcept
  {
    return 0;
  }
};

template <>
class NumericLimits<int64b>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr int64b max() noexcept
  {
    constexpr int64b v = static_cast<int64b>(9223372036854775807ll);
    return v;
  }

  static constexpr int64b min() noexcept
  {
    constexpr int64b v = static_cast<int64b>(-1) - max();
    return v;
  }

  static constexpr int64b lowest() noexcept
  {
    return min();
  }

  static constexpr int64b epsilon() noexcept
  {
    return 2;
  }

  static int64b infinity() noexcept
  {
    return max();
  }

  static int64b quietNan() noexcept
  {
    return 0;
  }
};

template <>
class NumericLimits<uint64b>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr uint64b min() noexcept
  {
    return 0;
  }

  static constexpr uint64b lowest() noexcept
  {
    return 0;
  }

  static constexpr uint64b max() noexcept
  {
    constexpr uint64b v = static_cast<uint64b>(18446744073709551615ull);
    return v;
  }

  static constexpr uint64b epsilon() noexcept
  {
    return 2;
  }

  static uint64b infinity() noexcept
  {
    return max();
  }

  static uint64b quietNan() noexcept
  {
    return 0;
  }
};

template <>
class NumericLimits<float>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr float min() noexcept
  {
    return FLT_MIN;
  }

  static constexpr float lowest() noexcept
  {
    return -max();
  }

  static constexpr float max() noexcept
  {
    return FLT_MAX;
  }

  static constexpr float epsilon() noexcept
  {
    return FLT_EPSILON;
  }

  static float infinity() noexcept
  {
    return INFINITY;
  }

  static float quietNan() noexcept
  {
    return NAN;
  }
};

template <>
class NumericLimits<double>
{
 public:
  static constexpr bool isSpecialized() noexcept
  {
    return true;
  }

  static constexpr double min() noexcept
  {
    return DBL_MIN;
  }

  static constexpr double lowest() noexcept
  {
    return -max();
  }

  static constexpr double max() noexcept
  {
    return DBL_MAX;
  }

  static constexpr double epsilon() noexcept
  {
    return DBL_EPSILON;
  }

  static double infinity() noexcept
  {
    const double v = HUGE_VAL;
    return v;
  }

  static double quietNan() noexcept
  {
    const int64b m = NumericLimits<int64b>::max();
    const double v = treatAs<double>(m);
    return v;
  }
};

} // namespace zinvul

#endif /* ZINVUL_LIMITS_INL_CL */
