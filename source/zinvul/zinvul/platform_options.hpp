/*!
  \file platform_options.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_PLATFORM_OPTIONS_HPP
#define ZINVUL_PLATFORM_OPTIONS_HPP

// Standard C++ library
#include <string>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul_config.hpp"

namespace zinvul {

/*!
  \brief No brief description

  No detailed description.
  */
class PlatformOptions : public zisc::NonCopyable<PlatformOptions>
{
 public:
  //! Create a platform options
  PlatformOptions(std::pmr::memory_resource* mem_resource) noexcept;

  //! Move option data
  PlatformOptions(PlatformOptions&& other) noexcept;


  //! Move option data
  PlatformOptions& operator=(PlatformOptions&& other) noexcept;


  //! Enable the debug mode
  void enableDebugMode(const bool debug_mode_enabled) noexcept;

  //! Enable the vulkan sub-platform
  void enableVulkanSubPlatform(const bool sub_platform_enabled) noexcept;

  //! Return the underlying memory resource
  std::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const std::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the platform name
  std::string_view platformName() const noexcept;

  //! Return the value of the major component of the platform version number
  uint32b platformVersionMajor() const noexcept;

  //! Return the value of the minor component of the platform version number
  uint32b platformVersionMinor() const noexcept;

  //! Return the value of the patch component of the platform version number
  uint32b platformVersionPatch() const noexcept;

  //! Check whether the debug mode is enabled
  bool debugModeEnabled() const noexcept;

  //! Set the platform name
  void setPlatformName(std::string_view name) noexcept;

  //! Set the value of the major component of the platform version number
  void setPlatformVersionMajor(const uint32b major) noexcept;

  //! Set the value of the minor component of the platform version number
  void setPlatformVersionMinor(const uint32b minor) noexcept;

  //! Set the value of the patch component of the platform version number
  void setPlatformVersionPatch(const uint32b patch) noexcept;

  //! Check whether the vulkan sub-platform is enabled
  bool vulkanSubPlatformEnabled() const noexcept;

 private:
  //! Initialize options
  void initialize() noexcept;


  std::pmr::memory_resource* mem_resource_;
  zisc::pmr::string platform_name_;
  uint32b platform_version_major_;
  uint32b platform_version_minor_;
  uint32b platform_version_patch_;
  uint32b vulkan_sub_platform_enabled_;
  uint32b debug_mode_enabled_; //!< Enable debugging in Zinvul
};

} // namespace zinvul

#include "platform_options-inl.hpp"

#endif // ZINVUL_PLATFORM_OPTIONS_HPP
