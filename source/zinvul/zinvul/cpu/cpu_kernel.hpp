/*!
  \file cpu_kernel.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_KERNEL_HPP
#define ZINVUL_CPU_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
// Zinvul
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"
#include "zinvul/utility/kernel_init_parameters.hpp"

namespace zinvul {

// Forward declaration
template <typename Type> class Buffer;
class CpuDevice;
template <std::size_t kDimension, typename FuncArgTypes, typename ...ArgTypes>
class CpuKernel;

/*!
  \brief No brief description

  No detailed description.

  \tparam kDimension No description.
  \tparam FuncArgTypes No description.
  \tparam ArgTypes No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
class CpuKernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...> :
    public Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>
{
 public:
  // Type aliases
  using BaseKernel =
      Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>;
  using InitParameters = typename BaseKernel::InitParameters;
  using Function = typename InitParameters::Function;
  template <typename Type>
  using BufferRef = typename BaseKernel::BufferRef;
  using LaunchOptions = typename BaseKernel::LaunchOptions;


  //! Initialize the kernel
  CpuKernel(IdData&& id) noexcept;

  //! Finalize the kernel
  ~CpuKernel() noexcept override;


  //! Return the underlying kernel function
  Function kernel() const noexcept;

  //! Execute a kernel
  void run(BufferRef<ArgTypes>... args,
           const LaunchOptions& launch_options) override;

 protected:
  //! Clear the contents of the kernel
  void destroyData() noexcept override;

  //! Initialize the kernel
  void initData(const InitParameters& params) override;

 private:
  /*!
    \brief No brief description

    No detailed description.

    \tparam ArgType No description.
    \tparam RestTypes No description.
    */
  template <typename ArgType, typename ...RestTypes>
  class Launcher 
  {
   public:
    //! Launch the given function 
    template <typename Type, typename ...Types>
    static void exec(Function func,
                     const LaunchOptions& launch_options,
                     Type&& value,
                     Types&&... rest) noexcept;

   private:
    //! Invoke the given function 
    template <typename ...Types>
    static void invoke(Function func,
                       const LaunchOptions& launch_options,
                       Types&&... args) noexcept;
  };

  //! Expand the given work size to 3d work size array
  static std::array<uint32b, 3> expandTo3d(
    const std::array<uint32b, kDimension>& work_size) noexcept;

  //! Return the device
  CpuDevice& parentImpl() noexcept;

  //! Return the device
  const CpuDevice& parentImpl() const noexcept;


  Function kernel_ = nullptr;
};

} // namespace zinvul

#include "cpu_kernel-inl.hpp"

#endif // ZINVUL_CPU_KERNEL_HPP
