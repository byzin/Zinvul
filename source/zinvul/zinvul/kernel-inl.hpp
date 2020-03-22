/*!
  \file kernel-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_INL_HPP
#define ZINVUL_KERNEL_INL_HPP

#include "kernel.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
// Zinvul
#include "utility/id_data.hpp"
#include "utility/zinvul_object.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::LaunchOptions() noexcept :
    queue_index_{0}
{
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::LaunchOptions(const std::array<uint32b, kDimension>& work_size) noexcept :
    work_size_{work_size},
    queue_index_{0}
{
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::LaunchOptions(const std::array<uint32b, kDimension>& work_size,
                             const uint32b queue_index) noexcept :
    work_size_{work_size},
    queue_index_{queue_index}
{
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
constexpr std::size_t
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::dimension() noexcept
{
  return kDimension;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
constexpr std::size_t
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::numOfArgs() noexcept
{
  return kNumOfArgs;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
uint32b
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::queueIndex() const noexcept
{
  return queue_index_;
}

/*!
  \details No detailed description

  \param [in] queue_index No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::setQueueIndex(const uint32b queue_index) noexcept
{
  queue_index_ = queue_index;
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  \param [in] dim No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::setWorkSize(const uint32b work_size,
                           const std::size_t dim) noexcept
{
  work_size_[dim] = work_size;
}

/*!
  \details No detailed description

  \param [in] work_size No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::setWorkSize(const std::array<uint32b, kDimension>& work_size) noexcept
{
  work_size_ = work_size;
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
const std::array<uint32b, kDimension>&
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
LaunchOptions::workSize() const noexcept
{
  return work_size_;
}

/*!
  \details No detailed description

  \param [in] id No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
Kernel(IdData&& id) noexcept :
    ZinvulObject(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
~Kernel() noexcept
{
}

/*!
  \details No detailed description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
destroy() noexcept
{
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
constexpr std::size_t 
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
dimension() noexcept
{
  return kDimension;
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  \param [in] params No description.
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
void
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
initialize(ZinvulObject::SharedPtr&& parent,
           WeakPtr&& own,
           const InitParameters& params)
{
  //! Clear the previous device data first
  destroy();

  initObject(std::move(parent), std::move(own));
  initData(params);
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
auto
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
makeOptions() const noexcept -> LaunchOptions
{
  return LaunchOptions{};
}

/*!
  \details No detailed description

  \return No description
  */
template <std::size_t kDimension, typename ...FuncArgTypes, typename ...ArgTypes>
inline
constexpr std::size_t 
Kernel<kDimension, KernelInitParameters<FuncArgTypes...>, ArgTypes...>::
numOfArgs() noexcept
{
  const std::size_t size = sizeof...(ArgTypes);
  return size;
}

} // namespace zinvul

#endif // ZINVUL_KERNEL_INL_HPP
