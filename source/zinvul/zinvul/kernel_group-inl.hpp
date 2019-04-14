/*!
  \file kernel_group-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_GROUP_INL_HPP
#define ZINVUL_KERNEL_GROUP_INL_HPP

#include "kernel_group.hpp"
// Standard C++ library
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
inline
KernelGroup::~KernelGroup() noexcept
{
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_GROUP_INL_HPP
