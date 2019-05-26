/*!
  \file cpu_kernel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_KERNEL_HPP
#define ZINVUL_CPU_KERNEL_HPP

// Standard C++ library
#include <array>
#include <cstddef>
// Zinvul
#include "zinvul/kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class CpuDevice;
template <typename> class CpuBuffer;

/*!
  */
template <std::size_t kDimension, typename ...ArgumentTypes>
class CpuKernel : public Kernel<kDimension, ArgumentTypes...>
{
  using KernelBase = Kernel<kDimension, ArgumentTypes...>;
  template <typename Type>
  using BufferRef = typename KernelBase::template BufferRef<Type>;

 public:
  using Function = typename KernelBase::Function;


   //! Construct a kernel
  CpuKernel(CpuDevice* device, const Function kernel) noexcept;


  //! Return an assigned device
  CpuDevice* device() noexcept;

  //! Return an assigned device
  const CpuDevice* device() const noexcept;

  //! Return the device type
  DeviceType deviceType() const noexcept override;

  //! Return a kernel function
  Function kernel() const noexcept;

  //! Check if this has a kernel
  bool hasKernel() const noexcept;

  //! Execute a kernel
  void run(BufferRef<ArgumentTypes>... args,
           const std::array<uint32b, kDimension> works,
           const uint32b /* queue_index*/) noexcept override
  {
    const auto command =[this, &args...]()
    {
      (*kernel())(refer<ArgumentTypes>(args)...);
    };
    device_->submit(works, CpuDevice::Command{command});
  }

  //! Set a kernel function
  void setKernel(const Function kernel) noexcept;

 private:
  //! Refer to a buffer
  template <typename Type>
  Type refer(BufferRef<Type> buffer) const noexcept;


  CpuDevice* device_ = nullptr;
  Function kernel_ = nullptr;
};

} // namespace zinvul

#include "cpu_kernel-inl.hpp"

#endif // ZINVUL_CPU_KERNEL_HPP
