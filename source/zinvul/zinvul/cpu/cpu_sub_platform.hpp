/*!
  \file cpu_sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_SUB_PLATFORM_HPP
#define ZINVUL_CPU_SUB_PLATFORM_HPP

// Zinvul
#include "zinvul/sub_platform.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class CpuSubPlatform : public SubPlatform
{
 public:
  //! Create an empty cpu sub-platform
  CpuSubPlatform() noexcept;

  //! Finalize the sub-platform
  ~CpuSubPlatform() noexcept override;


  //! Initialize the sub-platform
  void initialize(PlatformOptions& platform_options) override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

 private:
};

} // namespace zinvul

#endif // ZINVUL_CPU_SUB_PLATFORM_HPP
