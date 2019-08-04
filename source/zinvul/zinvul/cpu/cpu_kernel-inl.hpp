/*!
  \file cpu_kernel-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_KERNEL_INL_HPP
#define ZINVUL_CPU_KERNEL_INL_HPP

#include "cpu_kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_buffer.hpp"
#include "cpu_device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/kernel_arg_parser.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::CpuKernel(
    CpuDevice* device,
    const Function kernel_func) noexcept : device_{device}
{
  setKernel(kernel_func);
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
auto CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::device()
    noexcept -> CpuDevice*
{
  ZISC_ASSERT(device_ != nullptr, "The assigned device is null.");
  return device_;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
auto CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::device()
    const noexcept -> const CpuDevice*
{
  ZISC_ASSERT(device_ != nullptr, "The assigned device is null.");
  return device_;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
auto CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::deviceType()
    const noexcept -> DeviceType
{
  return DeviceType::kCpu;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
auto CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::kernel()
    const noexcept -> Function
{
  ZISC_ASSERT(kernel_ != nullptr, "The kernel function is null.");
  return kernel_;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
bool CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::hasKernel()
    const noexcept
{
  const bool result = kernel_ != nullptr;
  return result;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::run(
    std::add_lvalue_reference_t<BufferArgs>... args,
    const std::array<uint32b, kDimension> works,
    const uint32b /* queue_index */) noexcept
{
  const auto command = [this, &args...]() noexcept
  {
    using ArgPack = std::tuple<ArgumentTypes...>;
    runFunc<ArgPack>(args...);
  };
  device()->submit(works, CpuDevice::Command{command});
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
inline
void CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::setKernel(
    const Function kernel_func) noexcept
{
  kernel_ = kernel_func;
}

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
template <typename ArgPack, typename Type, typename ...Types> inline
void CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...>::runFunc(
    Type&& argument,
    Types&&... rest) noexcept
{
  using ArgPackInfo = ArgumentPackInfo<ArgPack>;
  if constexpr (ArgPackInfo::kHasArgument) {
    using ArgInfo = KernelArgInfo<typename ArgPackInfo::Type>;
    using RestArgPack = typename ArgPackInfo::RestArgPack;
    if constexpr (ArgInfo::kIsGlobal) {
      // Process a global argument
      using ElementType = typename std::remove_reference_t<Type>::Type;
      using CpuBufferT = CpuBuffer<ArgInfo::kDescriptor, ElementType>;
      using CpuBufferPtr = std::add_pointer_t<CpuBufferT>;

      auto& buffer = argument;
      ZISC_ASSERT(buffer.deviceType() == DeviceType::kCpu,
                  "The device type of the buffer isn't cpu.");
      ZISC_ASSERT(0 < buffer.size(), "The buffer is empty.");
      auto b = zisc::cast<CpuBufferPtr>(std::addressof(buffer));
      if constexpr (ArgInfo::kIsPod) {
        auto& clargument = *(b->data());
        runFunc<RestArgPack>(std::forward<Types>(rest)..., clargument);
      }
      else {
        typename ArgPackInfo::Type clargument{b->data()};
        runFunc<RestArgPack>(std::forward<Types>(rest)..., clargument);
      }
    }
    else {
      // Process a local argument
      typename ArgInfo::ElementType local_storage{};
      typename ArgPackInfo::Type clargument{std::addressof(local_storage)};
      runFunc<RestArgPack>(std::forward<Type>(argument),
                           std::forward<Types>(rest)...,
                           clargument);
    }
  }
  else {
    // All buffers are processed, run a kernel
    (*kernel())(std::forward<Type>(argument), std::forward<Types>(rest)...);
  }
}

} // namespace zinvul

#endif // ZINVUL_CPU_KERNEL_INL_HPP
