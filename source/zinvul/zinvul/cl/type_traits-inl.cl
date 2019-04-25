/*!
  \file type_traits-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPE_TRAITS_INL_CL
#define ZINVUL_TYPE_TRAITS_INL_CL

#include "type_traits.cl"

namespace zinvul {

template <typename T, typename F>
struct Conditional<true, T, F>
{
  using Type = T;
};

template <typename T, typename F>
struct Conditional<false, T, F>
{
  using Type = F;
};

} // namespace zinvul

#endif /* ZINVUL_TYPE_TRAITS_INL_CL */
