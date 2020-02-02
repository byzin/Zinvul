/*!
  \file sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_SUB_PLATFORM_HPP
#define ZINVUL_SUB_PLATFORM_HPP

// Standard C++ library
#include <memory>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class SubPlatform : private zisc::NonCopyable<SubPlatform>
{
 public:
  //! Create an empty sub-platform
  SubPlatform() noexcept;

  //! Finalize the platform
  virtual ~SubPlatform() noexcept;


  //! Initialize the sub-platform
  virtual void initialize(PlatformOptions& platform_options) = 0;

  //! Return the sub-platform type
  virtual SubPlatformType type() const noexcept = 0;
};

// Type aliases
using UniqueSubPlatform = zisc::pmr::unique_ptr<SubPlatform>;

} // namespace zinvul

#endif // ZINVUL_SUB_PLATFORM_HPP
