/*!
  \file platform.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_PLATFORM_HPP
#define ZINVUL_PLATFORM_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "sub_platform.hpp"
#include "zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class PlatformOptions;

/*!
  \brief No brief description

  No detailed description.
  */
class Platform : public zisc::NonCopyable<Platform>
{
 public:
  //! Create an empty platform
  Platform() noexcept;

  //! Move a data
  Platform(Platform&& other) noexcept;

  //! Finalize the platform
  ~Platform() noexcept;


  //! Move a data
  Platform& operator=(Platform&& other) noexcept;


  //! Destroy the platform
  void destroy() noexcept;

  //! Check if the platform has the sub-platform of the given type
  bool hasSubPlatform(const SubPlatformType type) const noexcept;

  //! Return the underlying memory resource
  std::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  const std::pmr::memory_resource* memoryResource() const noexcept;

  //! Initialize the platform
  void initialize(PlatformOptions& platform_options);

  //! Return the sub-platform of the given type
  SubPlatform* subPlatform(const SubPlatformType type) noexcept;

  //! Return the sub-platform of the given type
  const SubPlatform* subPlatform(const SubPlatformType type) const noexcept;

 private:
  //! Create a CPU sub-platform
  void initCpuSubPlatform(PlatformOptions& platform_options);

  //! Create a Vulkan sub-platform
  void initVulkanSubPlatform(PlatformOptions& platform_options);

  //! Set a sub-platform of the given type
  void setSubPlatform(const SubPlatformType type,
                      UniqueSubPlatform&& sub_platform) noexcept;


  static constexpr std::size_t kNumOfSubPlatforms = 2;


  std::pmr::memory_resource* mem_resource_ = nullptr;
  std::array<UniqueSubPlatform, kNumOfSubPlatforms> sub_platform_list_;
};

// Type aliases
using UniquePlatform = zisc::pmr::unique_ptr<Platform>;

} // namespace zinvul

#include "platform-inl.hpp"

#endif // ZINVUL_PLATFORM_HPP
