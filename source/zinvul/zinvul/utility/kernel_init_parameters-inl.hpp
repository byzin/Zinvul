/*!
  \file kernel_init_parameters_inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_INIT_PARAMETERS_INL_HPP
#define ZINVUL_KERNEL_INIT_PARAMETERS_INL_HPP

#include "kernel_init_parameters.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <cstring>
#include <string_view>
// Zisc
#include "zisc/error.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <typename ...ArgTypes> inline
KernelInitParameters<ArgTypes...>::KernelInitParameters(Function ptr) noexcept :
    function_{ptr}
{
  initialize();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes> inline
auto KernelInitParameters<ArgTypes...>::func() const noexcept -> Function
{
  return function_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes> inline
std::string_view KernelInitParameters<ArgTypes...>::kernelName() const noexcept
{
  std::string_view name{kernel_name_.data()};
  return name;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename ...ArgTypes> inline
constexpr std::size_t KernelInitParameters<ArgTypes...>::maxKernelNameLength() noexcept
{
  return kMaxKernelNameLength;
}

/*!
  \details No detailed description

  \param [in] ptr No description.
  */
template <typename ...ArgTypes> inline
void KernelInitParameters<ArgTypes...>::setFunc(Function ptr) noexcept
{
  function_ = ptr;
}

/*!
  \details No detailed description

  \param [in] kernel_name No description.
  */
template <typename ...ArgTypes> inline
void KernelInitParameters<ArgTypes...>::setKernelName(std::string_view kernel_name) noexcept
{
  const std::size_t s = kernel_name.size();
  ZISC_ASSERT(s < maxKernelNameLength(),
              "The kernel '", kernel_name, "' exceed the limit of name length. ",
              maxKernelNameLength(), " < n=", s, ".");
  std::strncpy(kernel_name_.data(), kernel_name.data(), s);
  kernel_name_[s] = '\0';
}

/*!
  \details No detailed description
  */
template <typename ...ArgTypes> inline
void KernelInitParameters<ArgTypes...>::initialize() noexcept
{
  kernel_name_.fill('\0');
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_INIT_PARAMETERS_INL_HPP
