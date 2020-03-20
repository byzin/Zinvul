/*!
  \file cpu_buffer-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CPU_BUFFER_INL_HPP
#define ZINVUL_CPU_BUFFER_INL_HPP

#include "cpu_buffer.hpp"
// Standard C++ library
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "cpu_device.hpp"
#include "zinvul/buffer.hpp"
#include "zinvul/sub_platform.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/utility/id_data.hpp"
#include "zinvul/utility/zinvul_object.hpp"

namespace zinvul {

/*!
  \details No detailed description
  */
template <typename T> inline
CpuBuffer<T>::CpuBuffer(IdData&& id) noexcept : Buffer<T>(std::move(id))
{
}

/*!
  \details No detailed description
  */
template <typename T> inline
CpuBuffer<T>::~CpuBuffer() noexcept
{
  Buffer<T>::destroy();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto CpuBuffer<T>::buffer() noexcept -> zisc::pmr::vector<Type>&
{
  return *buffer_;
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
auto CpuBuffer<T>::buffer() const noexcept -> const zisc::pmr::vector<Type>&
{
  return *buffer_;
}

/*!
  \details No detailed description

  \param [in] s No description.
  */
template <typename T> inline
void CpuBuffer<T>::setSize(const std::size_t s)
{
  const std::size_t prev_size = size();
  if (s != prev_size) {
    prepareBuffer();
    const std::size_t prev_cap = buffer().capacity();
    buffer().resize(s);
    const std::size_t cap = buffer().capacity();
    if (cap != prev_cap) {
      auto& device = parentImpl();
      const std::size_t prev_mem_size = sizeof(Type) * prev_cap;
      device.notifyDeallocation(prev_mem_size);
      const std::size_t mem_size = sizeof(Type) * cap;
      device.notifyAllocation(mem_size);
    }
  }
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
std::size_t CpuBuffer<T>::size() const noexcept
{
  const std::size_t s = (buffer_) ? buffer().size() : 0;
  return s;
}

/*!
  \details No detailed description
  */
template <typename T> inline
void CpuBuffer<T>::destroyData() noexcept
{
  buffer_.reset();
}

/*!
  \details No detailed description
  */
template <typename T> inline
void CpuBuffer<T>::initData()
{
  prepareBuffer();
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
CpuDevice& CpuBuffer<T>::parentImpl() noexcept
{
  auto p = Buffer<T>::getParent();
  return *zisc::treatAs<CpuDevice*>(p);
}

/*!
  \details No detailed description

  \return No description
  */
template <typename T> inline
const CpuDevice& CpuBuffer<T>::parentImpl() const noexcept
{
  const auto p = Buffer<T>::getParent();
  return *zisc::treatAs<const CpuDevice*>(p);
}

/*!
  \details No detailed description
  */
template <typename T> inline
void CpuBuffer<T>::prepareBuffer() noexcept
{
  if (!buffer_) {
    auto mem_resource = Buffer<T>::memoryResource();
    using BufferImplType = typename decltype(buffer_)::element_type;
    typename BufferImplType::allocator_type alloc{mem_resource};
    BufferImplType buffer{alloc};
    buffer_ = zisc::pmr::allocateUnique<BufferImplType>(mem_resource,
                                                        std::move(buffer));
  }
}

// Device

/*!
  \details No detailed description

  \tparam T No description.
  \param [in] flag No description.
  \return No description
  */
template <typename T> inline
SharedBuffer<T> CpuDevice::makeBuffer(const BufferUsage flag) noexcept
{
  using BufferType = CpuBuffer<T>;
  zisc::pmr::polymorphic_allocator<BufferType> alloc{memoryResource()};
  SharedBuffer<T> buffer = std::allocate_shared<BufferType>(alloc, issueId());

  ZinvulObject::SharedPtr parent{getOwn()};
  WeakBuffer<T> own{buffer};
  buffer->initialize(std::move(parent), std::move(own), flag);

  return buffer;
}

} // namespace zinvul

#endif // ZINVUL_CPU_BUFFER_INL_HPP

///*!
//  \file cpu_buffer-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZINVUL_CPU_BUFFER_INL_HPP
//#define ZINVUL_CPU_BUFFER_INL_HPP
//
//#include "cpu_buffer.hpp"
//// Standard C++ library
//#include <cstddef>
//#include <cstring>
//// Zisc
//#include "zisc/error.hpp"
//#include "zisc/memory_resource.hpp"
//// Zinvul
//#include "cpu_device.hpp"
//#include "zinvul/buffer.hpp"
//
//namespace zinvul {
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//CpuBuffer<kDescriptor, T>::CpuBuffer(CpuDevice* device,
//                                     const BufferUsage usage_flag) noexcept :
//    Buffer<kDescriptor, T>(usage_flag),
//    device_{device},
//    buffer_{device->memoryResource()}
//{
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//CpuBuffer<kDescriptor, T>::CpuBuffer(CpuDevice* device,
//                                     const BufferUsage usage_flag,
//                                     const std::size_t size) noexcept :
//    CpuBuffer(device, usage_flag)
//{
//  setSize(size);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto CpuBuffer<kDescriptor, T>::buffer() noexcept -> zisc::pmr::vector<Type>&
//{
//  return buffer_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto CpuBuffer<kDescriptor, T>::buffer() const noexcept
//    -> const zisc::pmr::vector<Type>&
//{
//  return buffer_;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> template <DescriptorType kDstDescriptor>
//inline
//void CpuBuffer<kDescriptor, T>::copyTo(CpuBuffer<kDstDescriptor, T>* dst,
//                                       const std::size_t count,
//                                       const std::size_t src_offset,
//                                       const std::size_t dst_offset,
//                                       const uint32b queue_index) const noexcept
//{
//  dst->write(data() + src_offset, count, dst_offset, queue_index);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto CpuBuffer<kDescriptor, T>::data() noexcept -> Pointer
//{
//  return buffer_.data();
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto CpuBuffer<kDescriptor, T>::data() const noexcept -> ConstPointer
//{
//  return buffer_.data();
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//SubPlatformType CpuBuffer<kDescriptor, T>::SubPlatformType() const noexcept
//{
//  return SubPlatformType::kCpu;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool CpuBuffer<kDescriptor, T>::isDeviceMemory() const noexcept
//{
//  return true;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool CpuBuffer<kDescriptor, T>::isHostMemory() const noexcept
//{
//  return true;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//bool CpuBuffer<kDescriptor, T>::isHostVisible() const noexcept
//{
//  return true;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//std::size_t CpuBuffer<kDescriptor, T>::memoryUsage() const noexcept
//{
//  const std::size_t memory_usage = sizeof(Type) * size();
//  return memory_usage;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void CpuBuffer<kDescriptor, T>::read(Pointer host_data,
//                                     const std::size_t count,
//                                     const std::size_t offset,
//                                     const uint32b) const noexcept
//{
//  const std::size_t s = sizeof(Type) * count;
//  std::memcpy(host_data, data() + offset, s);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void CpuBuffer<kDescriptor, T>::setSize(const std::size_t size) noexcept
//{
//  device_->deallocate(this);
//  device_->allocate(size, this);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//std::size_t CpuBuffer<kDescriptor, T>::size() const noexcept
//{
//  return buffer_.size();
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void CpuBuffer<kDescriptor, T>::write(ConstPointer host_data,
//                                      const std::size_t count,
//                                      const std::size_t offset,
//                                      const uint32b) noexcept
//{
//  const std::size_t s = sizeof(Type) * count;
//  std::memcpy(data() + offset, host_data, s);
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//auto CpuBuffer<kDescriptor, T>::mappedMemory() const noexcept -> Pointer
//{
//  Pointer d = const_cast<Pointer>(data());
//  ZISC_ASSERT(d != nullptr, "The data is null.");
//  return d;
//}
//
///*!
//  */
//template <DescriptorType kDescriptor, typename T> inline
//void CpuBuffer<kDescriptor, T>::unmapMemory() const noexcept
//{
//}
//
//} // namespace zinvul
//
//#endif // ZINVUL_CPU_BUFFER_INL_HPP
