/*!
  \file cpu_kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_KERNEL_INL_HPP
#define ZINVUL_CPU_KERNEL_INL_HPP

#include "cpu_kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_buffer.hpp"
#include "cpu_device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"
#include "zinvul/utility/kernel_arg_parser.hpp"
#include "zinvul/utility/kernel_init_parameters.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
CpuKernel(IdData&& id) noexcept : BaseKernel(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
~CpuKernel() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
auto
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
kernel() const noexcept -> Function
{
  return kernel_;
}

/*!
  \details No detailed description

  \param [in] args No description.
  \param [in] launch_options No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
run(BufferRef<ArgTypes>... args, const LaunchOptions& launch_options)
{
  auto& device = parentImpl();
  const auto work_size = expandTo3d(launch_options.workSize());
  using LauncherType = Launcher<FuncArgTypes...>;
  auto command = [func = kernel(), &args..., &launch_options]() noexcept
  {
    LauncherType::exec(func, launch_options, args...);
  };
  device.submit(work_size, command);
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
destroyData() noexcept
{
  kernel_ = nullptr;
}

/*!
  \details No detailed description

  \param [in] params No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
initData(const InitParameters& params)
{
  kernel_ = params.func();
}

/*!
  \details No detailed description

  \tparam Type No description.
  \tparam Types No description.
  \param [in] func No description.
  \param [in] value No description.
  \param [in] rest No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
template <typename ArgType, typename ...RestTypes>
template <typename Type, typename ...Types>
inline
void
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
Launcher<ArgType, RestTypes...>::exec(Function func,
                                      const LaunchOptions& launch_options,
                                      Type&& value,
                                      Types&&... rest) noexcept
{
//  constexpr std::size_t rest_size = sizeof...(RestTypes);
  using ArgInfo = KernelArgInfo<ArgType>;
  using ArgT = std::remove_volatile_t<ArgType>;
  if constexpr (ArgInfo::kIsLocal) { // Process a local argument
    typename ArgInfo::ElementType storage{};
    ArgT cl_arg{std::addressof(storage)};
    invoke(func,
           launch_options,
           std::forward<Type>(value),
           std::forward<Types>(rest)...,
           cl_arg);
  }
  else {
    // Process a global argument
    using BufferT = std::remove_reference_t<Type>;
    using CpuBufferT = CpuBuffer<typename BufferT::Type>;
    using CpuBufferPtr = std::add_pointer_t<CpuBufferT>;
    auto buffer = zisc::cast<CpuBufferPtr>(std::addressof(value));
    if constexpr (ArgInfo::kIsPod) {
      auto& cl_arg = *(buffer->data());
      invoke(func,
             launch_options,
             std::forward<Types>(rest)...,
             cl_arg);
    }
    else {
      ArgT cl_arg{buffer->data()};
      invoke(func,
             launch_options,
             std::forward<Types>(rest)...,
             cl_arg);
    }
  }
}

/*!
  \details No detailed description

  \tparam Types No description.
  \param [in] func No description.
  \param [in] args No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
template <typename ArgType, typename ...RestTypes>
template <typename ...Types>
inline
void
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
Launcher<ArgType, RestTypes...>::invoke(Function func,
                                        const LaunchOptions& launch_options,
                                        Types&&... args) noexcept
{
  constexpr std::size_t rest_size = sizeof...(RestTypes);
  if (0 < rest_size) {
    using LauncherType = Launcher<RestTypes...>;
    LauncherType::exec(func, launch_options, std::forward<Types>(args)...);
  }
  else {
    std::invoke(func, std::forward<Types>(args)...);
  }
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
std::array<uint32b, 3>
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
expandTo3d(const std::array<uint32b, kDimension>& work_size) noexcept
{
  std::array<uint32b, 3> work_size_3d{{1, 1, 1}};
  for (std::size_t i = 0; i < kDimension; ++i)
    work_size_3d[i] = work_size[i];
  return work_size_3d;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
CpuDevice&
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
parentImpl() noexcept
{
  auto p = BaseKernel::getParent();
  return *zisc::treatAs<CpuDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
const CpuDevice&
CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
parentImpl() const noexcept
{
  const auto p = BaseKernel::getParent();
  return *zisc::treatAs<const CpuDevice*>(p);
}

} // namespace zinvul

#endif // ZINVUL_CPU_KERNEL_INL_HPP
