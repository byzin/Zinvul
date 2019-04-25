/*!
  \file type_traits.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPE_TRAITS_CL
#define ZINVUL_TYPE_TRAITS_CL

namespace zinvul {

/*!
  \brief Choose one type or another based on compile-time boolean
  */
template <bool kCondition, typename T, typename F> struct Conditional;

template <bool kCondition, typename T, typename F>
using ConditionalType = typename Conditional<kCondition, T, F>::Type;

} // namespace zinvul

#include "type_traits-inl.cl"

#endif /* ZINVUL_TYPE_TRAITS_CL */
