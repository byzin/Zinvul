/*!
  \file limits.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_LIMITS_CL
#define ZINVUL_LIMITS_CL

namespace zinvul {

/*!
  \brief Various properties of arithmetic types
  */
template <typename Type> class NumericLimits;

} // namespace zinvul

#include "limits-inl.cl"

namespace zinvul {

/*!
  */
template <typename Type>
class NumericLimits
{
 public:
  //! Check if the NumericLimits<Type> is specialized
  static constexpr bool isSpecialized() noexcept;

  //! Return the smallest finite value of the Type
  static constexpr Type min() noexcept;

  //! Return the lowest finite value of the Type
  static constexpr Type lowest() noexcept;

  //! Return the largest finite value of the Type
  static constexpr Type max() noexcept;

  //! Return the difference between 1.0 and the next representable value
  static constexpr Type epsilon() noexcept;

  //! Return the positive infinity value of the Type
  static constexpr Type infinity() noexcept;

  //! Return the positive infinity value of the Type
  static Type quietNan() noexcept;
};

} // namespace zinvul

#endif /* ZINVUL_LIMITS_CL */
