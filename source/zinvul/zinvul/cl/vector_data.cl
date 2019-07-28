/*!
  \file vector_data.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VECTOR_DATA_CL
#define ZINVUL_VECTOR_DATA_CL

// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

/*!
  \brief Reading and writing functions of vector data
  */
template <typename Type>
class VectorData
{
  using VecType = VectorType<Type>;

 public:
  using ValueType = ConditionalType<!kIsHalf<Type>, Type,
      ConditionalType<VecType::size() == 1, float,
      ConditionalType<VecType::size() == 2, float2,
      ConditionalType<VecType::size() == 3, float3,
      ConditionalType<VecType::size() == 4, float4, void>>>>>;
  using DataType = typename VecType::ElementType;


  //! Return a data
  static ValueType load(const size_t offset, ConstGlobalPtr<DataType> p) noexcept;

  //! Return a data
  static ValueType load(const size_t offset, ConstLocalPtr<DataType> p) noexcept;

  //! Return a data
  static ValueType load(const size_t offset, ConstConstantPtr<DataType> p) noexcept;

  //! Return a data
  static ValueType load(const size_t offset, ConstPrivatePtr<DataType> p) noexcept;

  //! Return the number of elements of the type
  static constexpr size_t size() noexcept;

  //! Write a scalar data
  static void store(const ValueType data,
                    const size_t offset,
                    GlobalPtr<DataType> p) noexcept;

  //! Write a scalar data
  static void store(const ValueType data,
                    const size_t offset,
                    LocalPtr<DataType> p) noexcept;

  //! Write a scalar data
  static void store(const ValueType data,
                    const size_t offset,
                    PrivatePtr<DataType> p) noexcept;
 private:
  //! Return a data
  template <typename AddressType>
  static ValueType loadImpl(const size_t offset, AddressType p) noexcept;

  //! Return a data
  template <typename AddressType>
  static ValueType loadHalfImpl(const size_t offset, AddressType p) noexcept;

  //! Write a data
  template <typename AddressType>
  static void storeImpl(const ValueType data,
                        const size_t offset,
                        AddressType p) noexcept;

  //! Write a data
  template <typename AddressType>
  static void storeHalfImpl(const ValueType data,
                            const size_t offset,
                            AddressType p) noexcept;
};

} // namespace zinvul

#include "vector_data-inl.cl"

#endif /* ZINVUL_VECTOR_DATA_CL */
