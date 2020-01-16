/*!
  \file vector_data.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
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
class VectorData
{
  template <typename Type, size_t kN>
  using VecType = VectorTypeFromElems<RemoveCvType<Type>, kN>;

  template <size_t kN>
  using FloatVec = Float32VecType<kN>;

 public:
  //! Return a vector data read from the given address p
  template <size_t kN, typename AddressSpaceType>
  static auto load(const size_t offset, AddressSpaceType p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename AddressSpaceType>
  static auto loadHalf(const size_t offset, AddressSpaceType p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename AddressSpaceType>
  static void store(const VectorType data,
                    const size_t offset,
                    AddressSpaceType p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename AddressSpaceType>
  static void storeHalf(const VectorType data,
                        const size_t offset,
                        AddressSpaceType p) noexcept;

 private:
  //! Return a vector data read from the given address p
  template <size_t kN, typename AddressSpaceType>
  static auto loadImpl(const size_t offset, AddressSpaceType p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename AddressSpaceType>
  static auto loadHalfImpl(const size_t offset, AddressSpaceType p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename AddressSpaceType>
  static auto loadHalfUImpl(const size_t offset, AddressSpaceType p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename AddressSpaceType>
  static void storeImpl(const VectorType data,
                        const size_t offset,
                        AddressSpaceType p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename AddressSpaceType>
  static void storeHalfImpl(const VectorType data,
                            const size_t offset,
                            AddressSpaceType p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename AddressSpaceType>
  static void storeHalfUImpl(const VectorType fdata,
                             const size_t offset,
                             AddressSpaceType p) noexcept;
};

// OpenCL function aliases

//! Return a vector data read from the given address p
template <typename AddressSpaceType>
auto vload2(const size_t offset, AddressSpaceType p) noexcept;

//! Return a vector data read from the given address p
template <typename AddressSpaceType>
auto vload3(const size_t offset, AddressSpaceType p) noexcept;

//! Return a vector data read from the given address p
template <typename AddressSpaceType>
auto vload4(const size_t offset, AddressSpaceType p) noexcept;

//! Return a data read from p
template <typename AddressSpaceType>
auto vload_half(const size_t offset, AddressSpaceType p) noexcept;

//! Return a data read from p
template <typename AddressSpaceType>
auto vload_half2(const size_t offset, AddressSpaceType p) noexcept;

//! Return a data read from p
template <typename AddressSpaceType>
auto vload_half3(const size_t offset, AddressSpaceType p) noexcept;

//! Return a data read from p
template <typename AddressSpaceType>
auto vload_half4(const size_t offset, AddressSpaceType p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename AddressSpaceType>
void vstore2(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename AddressSpaceType>
void vstore3(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename AddressSpaceType>
void vstore4(const VectorType data,
             const size_t offset,
             AddressSpaceType p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename AddressSpaceType>
void vstore_half(const VectorType data,
                 const size_t offset,
                 AddressSpaceType p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename AddressSpaceType>
void vstore_half2(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename AddressSpaceType>
void vstore_half3(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename AddressSpaceType>
void vstore_half4(const VectorType data,
                  const size_t offset,
                  AddressSpaceType p) noexcept;

} // namespace zinvul

#include "vector_data-inl.cl"

#endif /* ZINVUL_VECTOR_DATA_CL */
