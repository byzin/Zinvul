/*!
  \file limits.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_LIMITS_CL
#define ZINVUL_LIMITS_CL

// Zinvul
#include "types.cl"

namespace zinvul {

/*!
  \brief Various properties of arithmetic types
  */
template <typename Type>
class NumericLimits
{
 public:
  //! Check if the NumericLimits<Type> is specialized
  static constexpr bool isSpecialized() noexcept;

  //! Return the number of radix digits that can be represented without change
  static constexpr int32b digits() noexcept;

  //! Return the number of decimal digits that can be represented without change
  static constexpr int32b digits10() noexcept;

  //! Return the number of decimal necessary to differentiate all values of type
  static constexpr int32b maxDigits10() noexcept;

  //! Return the smallest finite value of the Type
  static constexpr Type min() noexcept;

  //! Return the lowest finite value of the Type
  static constexpr Type lowest() noexcept;

  //! Return the largest finite value of the Type
  static constexpr Type max() noexcept;

  //! Return the difference between 1.0 and the next representable value
  static constexpr Type epsilon() noexcept;

  //! Return the positive infinity value of the Type
  static Type infinity() noexcept;

  //! Return the positive infinity value of the Type
  static Type quietNan() noexcept;
};

} // namespace zinvul

#include "limits-inl.cl"

#endif /* ZINVUL_LIMITS_CL */
