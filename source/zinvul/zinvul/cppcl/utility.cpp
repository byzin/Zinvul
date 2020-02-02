/*!
  \file utility.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "utility.hpp"
// Standard C++ library
#include <array>
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

namespace inner {

thread_local std::array<zinvul::uint32b, 3> WorkGroup::work_group_id_;
thread_local std::array<zinvul::uint32b, 3> WorkGroup::work_group_size_;

} // namespace inner

} // namespace cl

} // namespace zinvul
