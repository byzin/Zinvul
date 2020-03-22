/*!
  \file kernel_init_parameters.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_INIT_PARAMETERS_HPP
#define ZINVUL_KERNEL_INIT_PARAMETERS_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <string_view>

namespace zinvul {

/*!
  \brief No brief description

  No detailed description.

  \tparam ArgTypes No description.
  */
template <typename ...ArgTypes>
class KernelInitParameters
{
 public:
  // Type aliases
  using Function = void (*)(ArgTypes...);


  //! Initialize parameters
  KernelInitParameters(Function ptr) noexcept;


  //! Return the underlying function
  Function func() const noexcept;

  //! Return the kernel name
  std::string_view kernelName() const noexcept;

  //! Return the maximum kernel name length
  static constexpr std::size_t maxKernelNameLength() noexcept;

  //! Set a function
  void setFunc(Function ptr) noexcept;

  //! Set a kernel name
  void setKernelName(std::string_view kernel_name) noexcept;

 private:
  static constexpr std::size_t kMaxKernelNameLength = 256;


  //! Initialize parameters
  void initialize() noexcept;


  Function function_;
  std::array<char, kMaxKernelNameLength> kernel_name_;
};

} // namespace zinvul

#include "kernel_init_parameters-inl.hpp"

#endif // ZINVUL_KERNEL_INIT_PARAMETERS_HPP
