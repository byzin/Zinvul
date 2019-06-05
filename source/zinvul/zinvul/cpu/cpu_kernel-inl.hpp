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
#include <memory>
#include <array>
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/error.hpp"
#include "zisc/function_reference.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_buffer.hpp"
#include "cpu_device.hpp"
#include "zinvul/kernel.hpp"
#include "zinvul/kernel_arg_parser.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cppinner {

template <typename ...Types>
class CpuKernelRunnerImpl
{
 public:
  template <typename Function, typename ...ArgumentTypes>
  static void run(Function func, ArgumentTypes&&... args) noexcept
  {
    (*func)(std::forward<ArgumentTypes>(args)...);
  }
};

template <typename T, typename ...ArgumentTypes>
class CpuKernelRunnerImpl<T, ArgumentTypes...>
{
  using Parent = CpuKernelRunnerImpl<ArgumentTypes...>;
  using ArgInfo = KernelArgInfo<T>;

  template <typename Type>
  using BufferRef = std::add_lvalue_reference_t<Buffer<ArgInfo::kBufferType, Type>>;
  template <typename Type>
  using CpuBufferPtr = std::add_pointer_t<CpuBuffer<ArgInfo::kBufferType, Type>>;

  template <typename Type>
  static CpuBufferPtr<Type> getCpuBufferPtr(BufferRef<Type> buffer) noexcept
  {
    auto b = zisc::cast<CpuBufferPtr<Type>>(std::addressof(buffer));
    return b;
  }

  template <typename Function, typename Type, typename ...Types>
  static void runWithGlobal(Function func,
                            Type&& buffer,
                            Types&&... rest) noexcept
  {
    ZISC_ASSERT(buffer.deviceType() == DeviceType::kCpu,
                "The device type of the buffer isn't cpu.");
    ZISC_ASSERT(0 < buffer.size(), "The buffer is empty.");
    using ElementType = typename std::remove_reference_t<Type>::Type;
    using CpuBufferPtr = std::add_pointer_t<CpuBuffer<ArgInfo::kBufferType, 
                                                      ElementType>>;
    auto b = zisc::cast<CpuBufferPtr>(std::addressof(buffer));
    if constexpr (ArgInfo::kIsPod) {
      auto& argument = *(b->data());
      Parent::run(func, std::forward<Types>(rest)..., argument);
    }
    else {
      T argument{b->data()};
      Parent::run(func, std::forward<Types>(rest)..., argument);
    }
  }

  template <typename Function, typename ...Types>
  static void runWithLocal(Function func, Types&&... rest) noexcept
  {
    typename ArgInfo::ElementType local_storage{};
    T argument{std::addressof(local_storage)};
    Parent::run(func, std::forward<Types>(rest)..., argument);
  }

 public:
  template <typename Function, typename ...Types>
  static void run(Function func, Types&&... args) noexcept
  {
    if constexpr (ArgInfo::kIsGlobal)
      runWithGlobal(func, std::forward<Types>(args)...);
    else
      runWithLocal(func, std::forward<Types>(args)...);
  }
};

template <typename ...Types>
class CpuKernelRunner
{
  static_assert(0 <= sizeof...(Types), "The 'Function' type isn't kernel type.");
};

template <typename ...ArgumentTypes>
class CpuKernelRunner<void (*)(ArgumentTypes...)>
{
   using Function = void (*)(ArgumentTypes...);
 public:
   template <typename ...BufferArgs>
   static void run(Function func, BufferArgs&&... args) noexcept
   {
     CpuKernelRunnerImpl<ArgumentTypes...>::run(func,
                                                std::forward<BufferArgs>(args)...);
   }
};

} // namespace cppinner

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
CpuKernel<kDimension, Function, BufferArgs...>::CpuKernel(
    CpuDevice* device,
    const Function kernel_func) noexcept : device_{device}
{
  setKernel(kernel_func);
}

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
CpuDevice* CpuKernel<kDimension, Function, BufferArgs...>::device() noexcept
{
  ZISC_ASSERT(device_ != nullptr, "The assigned device is null.");
  return device_;
}

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
const CpuDevice* CpuKernel<kDimension, Function, BufferArgs...>::device()
    const noexcept
{
  ZISC_ASSERT(device_ != nullptr, "The assigned device is null.");
  return device_;
}

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
DeviceType CpuKernel<kDimension, Function, BufferArgs...>::deviceType()
    const noexcept
{
  return DeviceType::kCpu;
}

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
auto CpuKernel<kDimension, Function, BufferArgs...>::kernel() const noexcept
    -> Function
{
  ZISC_ASSERT(kernel_ != nullptr, "The kernel function is null.");
  return kernel_;
}

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
bool CpuKernel<kDimension, Function, BufferArgs...>::hasKernel() const noexcept
{
  const bool result = kernel_ != nullptr;
  return result;
}

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
void CpuKernel<kDimension, Function, BufferArgs...>::run(
    std::add_lvalue_reference_t<BufferArgs>... args,
    const std::array<uint32b, kDimension> works,
    const uint32b /* queue_index */) noexcept
{
  const auto command = [this, &args...]()
  {
    cppinner::CpuKernelRunner<Function>::run(kernel(), args...);
  };
  device()->submit(works, CpuDevice::Command{command});
}

/*!
  */
template <std::size_t kDimension, typename Function, typename ...BufferArgs> inline
void CpuKernel<kDimension, Function, BufferArgs...>::setKernel(
    const Function kernel_func) noexcept
{
  kernel_ = kernel_func;
}

} // namespace zinvul

#endif // ZINVUL_CPU_KERNEL_INL_HPP
