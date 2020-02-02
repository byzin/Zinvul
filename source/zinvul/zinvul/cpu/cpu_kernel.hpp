/*!
  \file cpu_kernel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_KERNEL_HPP
#define ZINVUL_CPU_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
// Zinvul
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class CpuDevice;

template <std::size_t kDimension, typename Function, typename ...BufferArgs>
class CpuKernel;

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes, typename ...BufferArgs>
class CpuKernel<kDimension, void (*)(ArgumentTypes...), BufferArgs...> :
    public Kernel<kDimension, BufferArgs...>
{
 public:
  using Function = void (*)(ArgumentTypes...);


   //! Construct a kernel
  CpuKernel(CpuDevice* device, const Function kernel) noexcept;


  //! Return an assigned device
  CpuDevice* device() noexcept;

  //! Return an assigned device
  const CpuDevice* device() const noexcept;

  //! Return the device type
  SubPlatformType SubPlatformType() const noexcept override;

  //! Return a kernel function
  Function kernel() const noexcept;

  //! Check if this has a kernel
  bool hasKernel() const noexcept;

  //! Execute a kernel
  void run(std::add_lvalue_reference_t<BufferArgs>... args,
           const std::array<uint32b, kDimension> works,
           const uint32b queue_index) noexcept override;

  //! Set a kernel function
  void setKernel(const Function kernel) noexcept;

 private:
  template <typename ...Types>
  struct ArgumentPackInfo
  {
    static constexpr bool kHasArgument = false;
  };

  template <typename T, typename ...Types>
  struct ArgumentPackInfo<std::tuple<T, Types...>>
  {
    using Type = T;
    using RestArgPack = std::tuple<Types...>;
    static constexpr bool kHasArgument = true;
  };

  //! Execute a kernel
  template <typename ArgPack, typename Type, typename ...Types>
  void runFunc(Type&& argument, Types&&... args) noexcept;


  CpuDevice* device_ = nullptr;
  Function kernel_ = nullptr;
};

} // namespace zinvul

#include "cpu_kernel-inl.hpp"

#endif // ZINVUL_CPU_KERNEL_HPP
