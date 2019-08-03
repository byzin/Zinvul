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

  static constexpr int32b digits() noexcept
  {
    const int32b d = CHAR_BIT - 1;
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    return 0;
  }

  static constexpr int8b min() noexcept
  {
    const int8b m = SCHAR_MIN;
    return m;
  }

  static constexpr int8b lowest() noexcept
  {
    const int8b l = min();
    return l;
  }

  static constexpr int8b max() noexcept
  {
    const int8b m = SCHAR_MAX;
    return m;
  }

  static constexpr int8b epsilon() noexcept
  {
    return 2;
  }

  static int8b infinity() noexcept
  {
    const int8b i = max();
    return i;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = CHAR_BIT;
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    return 0;
  }

  static constexpr uint8b min() noexcept
  {
    return 0;
  }

  static constexpr uint8b lowest() noexcept
  {
    const uint8b l = min();
    return l;
  }

  static constexpr uint8b max() noexcept
  {
    const uint8b m = UCHAR_MAX;
    return m;
  }

  static constexpr uint8b epsilon() noexcept
  {
    return 2;
  }

  static uint8b infinity() noexcept
  {
    const uint8b i = max();
    return i;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = CHAR_BIT * sizeof(int16b) - 1;
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    return 0;
  }

  static constexpr int16b min() noexcept
  {
    const int16b m = SHRT_MIN;
    return m;
  }

  static constexpr int16b lowest() noexcept
  {
    const int16b l = min();
    return l;
  }

  static constexpr int16b max() noexcept
  {
    const int16b m = SHRT_MAX;
    return m;
  }

  static constexpr int16b epsilon() noexcept
  {
    return 2;
  }

  static int16b infinity() noexcept
  {
    const int16b m = max();
    return m;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = CHAR_BIT * sizeof(int16b);
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    return 0;
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
    const uint16b m = USHRT_MAX;
    return m;
  }

  static constexpr uint16b epsilon() noexcept
  {
    return 2;
  }

  static uint16b infinity() noexcept
  {
    const uint16b i = max();
    return i;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = CHAR_BIT * sizeof(int32b) - 1;
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    return 0;
  }

  static constexpr int32b min() noexcept
  {
    const int32b m = INT_MIN;
    return m;
  }

  static constexpr int32b lowest() noexcept
  {
    const int32b l = min();
    return l;
  }

  static constexpr int32b max() noexcept
  {
    const int32b m = INT_MAX;
    return m;
  }

  static constexpr int32b epsilon() noexcept
  {
    return 2;
  }

  static int32b infinity() noexcept
  {
    const int32b i = max();
    return i;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = CHAR_BIT * sizeof(int32b);
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    return 0;
  }

  static constexpr uint32b min() noexcept
  {
    return 0;
  }

  static constexpr uint32b lowest() noexcept
  {
    const uint32b l = min();
    return l;
  }

  static constexpr uint32b max() noexcept
  {
    const uint32b m = UINT_MAX;
    return m;
  }

  static constexpr uint32b epsilon() noexcept
  {
    return 2;
  }

  static uint32b infinity() noexcept
  {
    const uint32b i = max();
    return i;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = CHAR_BIT * sizeof(int64b) - 1;
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    return 0;
  }

  static constexpr int64b max() noexcept
  {
    const int64b v = static_cast<int64b>(9223372036854775807ll);
    return v;
  }

  static constexpr int64b min() noexcept
  {
    const int64b v = static_cast<int64b>(-1) - max();
    return v;
  }

  static constexpr int64b lowest() noexcept
  {
    const int64b l = min();
    return l;
  }

  static constexpr int64b epsilon() noexcept
  {
    return 2;
  }

  static int64b infinity() noexcept
  {
    const int64b i = max();
    return i;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = CHAR_BIT * sizeof(int64b);
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    return 0;
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
    const uint64b v = static_cast<uint64b>(18446744073709551615ull);
    return v;
  }

  static constexpr uint64b epsilon() noexcept
  {
    return 2;
  }

  static uint64b infinity() noexcept
  {
    const uint64b i = max();
    return i;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = FLT_MANT_DIG;
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    const int32b d = FLT_DIG;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr float min() noexcept
  {
    const float m = FLT_MIN;
    return m;
  }

  static constexpr float lowest() noexcept
  {
    const float l = -max();
    return l;
  }

  static constexpr float max() noexcept
  {
    const float m = FLT_MAX;
    return m;
  }

  static constexpr float epsilon() noexcept
  {
    const float e = FLT_EPSILON;
    return e;
  }

  static float infinity() noexcept
  {
    const float i = INFINITY;
    return i;
  }

  static float quietNan() noexcept
  {
    const float n = NAN;
    return n;
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

  static constexpr int32b digits() noexcept
  {
    const int32b d = DBL_MANT_DIG;
    return d;
  }

  static constexpr int32b digits10() noexcept
  {
    const int32b d = DBL_DIG;
    return d;
  }

  static constexpr int32b maxDigits10() noexcept
  {
    //! \todo Implement
    const int32b d = 0;
    return d;
  }

  static constexpr double min() noexcept
  {
    const double m = DBL_MIN;
    return m;
  }

  static constexpr double lowest() noexcept
  {
    const double l = -max();
    return l;
  }

  static constexpr double max() noexcept
  {
    const double m = DBL_MAX;
    return m;
  }

  static constexpr double epsilon() noexcept
  {
    const double e = DBL_EPSILON;
    return e;
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
