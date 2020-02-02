/*!
  \file vulkan_sub_platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VULKAN_SUB_PLATFORM_HPP
#define ZINVUL_VULKAN_SUB_PLATFORM_HPP

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
class VulkanSubPlatform : public SubPlatform
{
 public:
  //! Create an empty vulkan sub-platform
  VulkanSubPlatform() noexcept;

  //! Finalize the sub-platform
  ~VulkanSubPlatform() noexcept override;


  //! Initialize the sub-platform
  void initialize(PlatformOptions& platform_options) override;

  //! Return the sub-platform type
  SubPlatformType type() const noexcept override;

 private:
};

} // namespace zinvul

#endif // ZINVUL_VULKAN_SUB_PLATFORM_HPP
