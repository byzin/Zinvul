/*!
  \file cpu_kernel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_KERNEL_HPP
#define ZINVUL_CPU_KERNEL_HPP

// Zinvul
#include "zinvul/kernel.hpp"
#include "zinvul/kernel_group.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

// Forward declaration
class CpuDevice;
template <typename> class CpuBuffer;

/*!
  */
template <typename GroupType, typename ...ArgumentTypes>
class CpuKernel : public Kernel<GroupType, ArgumentTypes...>
{
  using KernelBase = Kernel<GroupType, ArgumentTypes...>;
  template <typename Type>
  using BufferRef = typename KernelBase::template BufferRef<Type>;

 public:
  using KernelGroupType = typename KernelBase::KernelGroupType;
  using KernelFunction = typename KernelBase::KernelFunction;


   //! Construct a kernel
  CpuKernel(CpuDevice* device, const KernelFunction kernel) noexcept;


  //! Return a assigned device
  CpuDevice* device() noexcept;

  //! Return a assigned device
  const CpuDevice* device() const noexcept;

  //! Return the device type
  DeviceType deviceType() const noexcept override;

  //! Return a kernel function
  KernelFunction kernel() const noexcept;

  //! Execute a kernel
  void run(BufferRef<ArgumentTypes>... args,
           const uint32b work_x_size,
           const uint32b work_y_size = 1,
           const uint32b work_z_size = 1) noexcept override;

  //! Set a kernel function
  void setKernel(const KernelFunction kernel) noexcept;

 private:
  //! Refer to a buffer
  template <typename Type>
  Type refer(BufferRef<Type> buffer) const noexcept;


  CpuDevice* device_ = nullptr;
  KernelFunction kernel_ = nullptr;
};

} // namespace zinvul

#include "cpu_kernel-inl.hpp"

#endif // ZINVUL_CPU_KERNEL_HPP
