/*!
  \file zinvul_config.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_CONFIG_CL
#define ZINVUL_ZINVUL_CONFIG_CL

namespace zinvul {

/*!
  \brief Configuration of zinvul
  */
class Config
{
 public:
  // Math library config

  //! Check if built-in math library is used
  static constexpr bool isBuiltinMathUsed() noexcept;

  //! Check if built-in 'frexp' is used
  static constexpr bool isBuiltinMathFrexpUsed() noexcept;

  //! Check if built-in 'ldexp' is used
  static constexpr bool isBuiltinMathLdexpUsed() noexcept;

  //! Check if built-in 'round' is used
  static constexpr bool isBuiltinMathRoundUsed() noexcept;

  //! Check if built-in 'fract' is used
  static constexpr bool isBuiltinMathFractUsed() noexcept;

  //! Check if built-in 'fmod' is used
  static constexpr bool isBuiltinMathFmodUsed() noexcept;

  //! Check if built-in 'exp' is used
  static constexpr bool isBuiltinMathExpUsed() noexcept;

  //! Check if built-in 'log', 'log2' and 'log10' are used
  static constexpr bool isBuiltinMathLogUsed() noexcept;

  //! Check if built-in 'pow' is used
  static constexpr bool isBuiltinMathPowUsed() noexcept;

  //! Check if built-in 'sqrt' is used
  static constexpr bool isBuiltinMathSqrtUsed() noexcept;

  //! Check if built-in 'cbrt' is used
  static constexpr bool isBuiltinMathCbrtUsed() noexcept;

  //! Check if built-in 'sin', 'cos' and 'tan' are used
  static constexpr bool isBuiltinMathTrigonometricUsed() noexcept;

  //! Check if built-in 'asin', 'acos' and 'atan' are used
  static constexpr bool isBuiltinMathInvTrigonometricUsed() noexcept;
};

} // namespace zinvul

#include "zinvul_config-inl.cl"

#endif /* ZINVUL_ZINVUL_CONFIG_CL */
