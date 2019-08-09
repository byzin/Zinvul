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
class VectorData
{
 public:
  template <typename Type, size_t kN>
  using VecType = VectorTypeFromElems<RemoveCvType<Type>, kN>;

  template <size_t kN>
  using FloatVec = Float32VecType<kN>;


  //! Return a vector data read from the given address p
  template <size_t kN, typename Type>
  static VecType<Type, kN> load(const size_t offset,
                                ConstGlobalPtr<Type> p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename Type>
  static VecType<Type, kN> load(const size_t offset,
                                ConstLocalPtr<Type> p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename Type>
  static VecType<Type, kN> load(const size_t offset,
                                ConstConstantPtr<Type> p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename Type>
  static VecType<Type, kN> load(const size_t offset,
                                ConstPrivatePtr<Type> p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename Type>
  static FloatVec<kN> loadHalf(const size_t offset,
                               ConstGlobalPtr<Type> p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename Type>
  static FloatVec<kN> loadHalf(const size_t offset,
                               ConstLocalPtr<Type> p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename Type>
  static FloatVec<kN> loadHalf(const size_t offset,
                               ConstConstantPtr<Type> p) noexcept;

  //! Return a vector data read from the given address p
  template <size_t kN, typename Type>
  static FloatVec<kN> loadHalf(const size_t offset,
                               ConstPrivatePtr<Type> p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename Type>
  static void store(const VectorType data,
                    const size_t offset,
                    GlobalPtr<Type> p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename Type>
  static void store(const VectorType data,
                    const size_t offset,
                    LocalPtr<Type> p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename Type>
  static void store(const VectorType data,
                    const size_t offset,
                    PrivatePtr<Type> p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename Type>
  static void storeHalf(const VectorType data,
                        const size_t offset,
                        GlobalPtr<Type> p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename Type>
  static void storeHalf(const VectorType data,
                        const size_t offset,
                        LocalPtr<Type> p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename Type>
  static void storeHalf(const VectorType data,
                        const size_t offset,
                        PrivatePtr<Type> p) noexcept;

 private:
  //! Return a vector data read from the given address p
  template <typename Type, size_t kN, typename AddressType>
  static VecType<Type, kN> loadImpl(const size_t offset, AddressType p) noexcept;

  //! Return a vector data read from the given address p
  template <typename Type, size_t kN, typename AddressType>
  static FloatVec<kN> loadHalfImpl(const size_t offset, AddressType p) noexcept;

  //! Return a vector data read from the given address p
  template <typename Type, size_t kN, typename AddressType>
  static FloatVec<kN> loadHalfUImpl(const size_t offset, AddressType p) noexcept;

  //! Write a vector data to the given address p
  template <typename VectorType, typename AddressType>
  static void storeImpl(const VectorType data,
                        const size_t offset,
                        AddressType p) noexcept;

  //! Write a vector data to the given address p
  template <typename Type, typename VectorType, typename AddressType>
  static void storeHalfImpl(const VectorType data,
                            const size_t offset,
                            AddressType p) noexcept;

  //! Write a vector data to the given address p
  template <typename Type, typename VectorType, typename AddressType>
  static void storeHalfUImpl(const VectorType fdata,
                             const size_t offset,
                             AddressType p) noexcept;
};

// OpenCL function aliases

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 2> vload2(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 3> vload3(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 4> vload4(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 2> vload2(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 3> vload3(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 4> vload4(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 2> vload2(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 3> vload3(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 4> vload4(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 2> vload2(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 3> vload3(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept;

//! Return a vector data read from the given address p
template <typename Type>
VectorData::VecType<Type, 4> vload4(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<1> vload_half(const size_t offset,
                                   ConstGlobalPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<2> vload_half2(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<3> vload_half3(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<4> vload_half4(const size_t offset,
                                    ConstGlobalPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<1> vload_half(const size_t offset,
                                   ConstLocalPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<2> vload_half2(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<3> vload_half3(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<4> vload_half4(const size_t offset,
                                    ConstLocalPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<1> vload_half(const size_t offset,
                                   ConstConstantPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<2> vload_half2(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<3> vload_half3(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<4> vload_half4(const size_t offset,
                                    ConstConstantPtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<1> vload_half(const size_t offset,
                                   ConstPrivatePtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<2> vload_half2(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<3> vload_half3(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept;

//! Return a data read from p
template <typename Type>
VectorData::FloatVec<4> vload_half4(const size_t offset,
                                    ConstPrivatePtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore2(const VectorType data,
             const size_t offset,
             GlobalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore3(const VectorType data,
             const size_t offset,
             GlobalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore4(const VectorType data,
             const size_t offset,
             GlobalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore2(const VectorType data,
             const size_t offset,
             LocalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore3(const VectorType data,
             const size_t offset,
             LocalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore4(const VectorType data,
             const size_t offset,
             LocalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore2(const VectorType data,
             const size_t offset,
             PrivatePtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore3(const VectorType data,
             const size_t offset,
             PrivatePtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore4(const VectorType data,
             const size_t offset,
             PrivatePtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half(const VectorType data,
                 const size_t offset,
                 GlobalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half2(const VectorType data,
                  const size_t offset,
                  GlobalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half3(const VectorType data,
                  const size_t offset,
                  GlobalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half4(const VectorType data,
                  const size_t offset,
                  GlobalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half(const VectorType data,
                 const size_t offset,
                 LocalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half2(const VectorType data,
                  const size_t offset,
                  LocalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half3(const VectorType data,
                  const size_t offset,
                  LocalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half4(const VectorType data,
                  const size_t offset,
                  LocalPtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half(const VectorType data,
                 const size_t offset,
                 PrivatePtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half2(const VectorType data,
                  const size_t offset,
                  PrivatePtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half3(const VectorType data,
                  const size_t offset,
                  PrivatePtr<Type> p) noexcept;

//! Write a vector data to the given address p
template <typename VectorType, typename Type>
void vstore_half4(const VectorType data,
                  const size_t offset,
                  PrivatePtr<Type> p) noexcept;

} // namespace zinvul

#include "vector_data-inl.cl"

#endif /* ZINVUL_VECTOR_DATA_CL */
