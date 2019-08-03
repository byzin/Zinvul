/*!
  \file utility-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_UTILITY_INL_CL
#define ZINVUL_UTILITY_INL_CL

#include "utility.cl"
#if defined(ZINVUL_CPU)
#include <cstdio>
#include <cstdlib>
#endif // ZINVUL_CPU
// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"

/*!
  */
template <typename ...Types>
int32b print(ConstConstantPtr<char> format, const Types... args) noexcept
{
  int32b result = 0;
  constexpr size_t num_of_args = sizeof...(args);
#if defined(ZINVUL_CPU)
  if constexpr (0 < num_of_args)
    result = std::printf(format.get(), args...);
  else
    result = std::printf(format.get());
#endif // ZINVUL_CPU
  return result;
}

/*!
  */
template <typename ...Types> inline
void assertIfFalse(const int32b condition,
                   ConstConstantPtr<char> format,
                   const Types... args) noexcept
{
  constexpr size_t num_of_args = sizeof...(args);
  if (!condition) {
#if defined(ZINVUL_CPU)
    if constexpr (0 < num_of_args)
      std::fprintf(stderr, format.get(), args...);
    else
      std::fprintf(stderr, format.get());
    std::abort();
#endif // ZINVUL_CPU
  }
}

#pragma clang diagnostic pop
#pragma clang diagnostic pop
#pragma clang diagnostic pop

/*!
  */
inline
uint32b getGlobalSize(const uint32b dimension) noexcept
{
  const uint32b size = get_global_size(dimension);
  return size;
}

/*!
  */
inline
uint32b getGlobalSizeX() noexcept
{
  const uint32b size = getGlobalSize(0);
  return size;
}

/*!
  */
inline
uint32b getGlobalSizeY() noexcept
{
  const uint32b size = getGlobalSize(1);
  return size;
}

/*!
  */
inline
uint32b getGlobalSizeZ() noexcept
{
  const uint32b size = getGlobalSize(2);
  return size;
}

/*!
  */
inline
uint32b getGlobalId(const uint32b dimension) noexcept
{
  const uint32b id = get_global_id(dimension);
  return id;
}

/*!
  */
inline
uint32b getGlobalIdX() noexcept
{
  const uint32b id = getGlobalId(0);
  return id;
}

/*!
  */
inline
uint32b getGlobalIdY() noexcept
{
  const uint32b id = getGlobalId(1);
  return id;
}

/*!
  */
inline
uint32b getGlobalIdZ() noexcept
{
  const uint32b id = getGlobalId(2);
  return id;
}

/*!
  */
inline
uint32b getLocalSize(const uint32b dimension) noexcept
{
  const uint32b size = get_local_size(dimension);
  return size;
}

/*!
  */
inline
uint32b getLocalSizeX() noexcept
{
  const uint32b size = getLocalSize(0);
  return size;
}

/*!
  */
inline
uint32b getLocalSizeY() noexcept
{
  const uint32b size = getLocalSize(1);
  return size;
}

/*!
  */
inline
uint32b getLocalSizeZ() noexcept
{
  const uint32b size = getLocalSize(2);
  return size;
}

/*!
  */
inline
uint32b getLocalId(const uint32b dimension) noexcept
{
  const uint32b id = get_local_id(dimension);
  return id;
}

/*!
  */
inline
uint32b getLocalIdX() noexcept
{
  const uint32b id = getLocalId(0);
  return id;
}

/*!
  */
inline
uint32b getLocalIdY() noexcept
{
  const uint32b id = getLocalId(1);
  return id;
}

/*!
  */
inline
uint32b getLocalIdZ() noexcept
{
  const uint32b id = getLocalId(2);
  return id;
}

/*!
  */
inline
uint32b getNumGroups(const uint32b dimension) noexcept
{
  const uint32b size = get_num_groups(dimension);
  return size;
}

/*!
  */
inline
uint32b getNumGroupsX() noexcept
{
  const uint32b size = getNumGroups(0);
  return size;
}

/*!
  */
inline
uint32b getNumGroupsY() noexcept
{
  const uint32b size = getNumGroups(1);
  return size;
}

/*!
  */
inline
uint32b getNumGroupsZ() noexcept
{
  const uint32b size = getNumGroups(2);
  return size;
}

/*!
  */
inline
uint32b getGroupId(const uint32b dimension) noexcept
{
  const uint32b id = get_group_id(dimension);
  return id;
}

/*!
  */
inline
uint32b getGroupIdX() noexcept
{
  const uint32b id = getGroupId(0);
  return id;
}

/*!
  */
inline
uint32b getGroupIdY() noexcept
{
  const uint32b id = getGroupId(1);
  return id;
}

/*!
  */
inline
uint32b getGroupIdZ() noexcept
{
  const uint32b id = getGroupId(2);
  return id;
}

namespace inner {

template <typename Type>
struct TypeValueMaker
{
  //! Create a value
  static Type make(const Type v) noexcept
  {
    return v;
  }
};

} // namespace inner

#define ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(Type, ElemType, name) \
  namespace inner { \
    template <> \
    struct TypeValueMaker< Type > \
    { \
      static Type make(const ElemType v) noexcept \
      { \
        const Type data{v, v}; \
        return data; \
      } \
      static Type make(const ElemType x, const ElemType y) noexcept \
      { \
        const Type data{x, y}; \
        return data; \
      } \
    }; \
  } \
  inline Type make ## name (const ElemType v) noexcept \
  { \
    const auto data = inner::TypeValueMaker< Type >::make(v); \
    return data; \
  } \
  inline Type make ## name (const ElemType x, const ElemType y) noexcept \
  { \
    const auto data = inner::TypeValueMaker< Type >::make(x, y); \
    return data; \
  }

#define ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(Type, ElemType, name) \
  namespace inner { \
    template <> \
    struct TypeValueMaker< Type > \
    { \
      static Type make(const ElemType v) noexcept \
      { \
        const Type data{v, v, v}; \
        return data; \
      } \
      static Type make(const ElemType x, const ElemType y, \
                       const ElemType z) noexcept \
      { \
        const Type data{x, y, z}; \
        return data; \
      } \
    }; \
  } \
  inline Type make ## name (const ElemType v) noexcept \
  { \
    const auto data = inner::TypeValueMaker< Type >::make(v); \
    return data; \
  } \
  inline Type make ## name (const ElemType x, const ElemType y, \
                     const ElemType z) noexcept \
  { \
    const auto data = inner::TypeValueMaker< Type >::make(x, y, z); \
    return data; \
  }

#define ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(Type, ElemType, name) \
  namespace inner { \
    template <> \
    struct TypeValueMaker< Type > \
    { \
      static Type make(const ElemType v) noexcept \
      { \
        const Type data{v, v, v, v}; \
        return data; \
      } \
      static Type make(const ElemType x, const ElemType y, \
                       const ElemType z, const ElemType w) noexcept \
      { \
        const Type data{x, y, z, w}; \
        return data; \
      } \
    }; \
  } \
  inline Type make ## name (const ElemType v) noexcept \
  { \
    const auto data = inner::TypeValueMaker< Type >::make(v); \
    return data; \
  } \
  inline Type make ## name (const ElemType x, const ElemType y, \
                     const ElemType z, const ElemType w) noexcept \
  { \
    const auto data = inner::TypeValueMaker< Type >::make(x, y, z, w); \
    return data; \
  }

ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(char2, int8b, Char2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(uchar2, uint8b, UChar2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(short2, int16b, Short2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(ushort2, uint16b, UShort2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(int2, int32b, Int2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(uint2, uint32b, UInt2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(long2, int64b, Long2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(ulong2, uint64b, ULong2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(float2, float, Float2)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL2(double2, double, Double2)

ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(char3, int8b, Char3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(uchar3, uint8b, UChar3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(short3, int16b, Short3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(ushort3, uint16b, UShort3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(int3, int32b, Int3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(uint3, uint32b, UInt3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(long3, int64b, Long3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(ulong3, uint64b, ULong3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(float3, float, Float3)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL3(double3, double, Double3)

ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(char4, int8b, Char4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(uchar4, uint8b, UChar4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(short4, int16b, Short4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(ushort4, uint16b, UShort4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(int4, int32b, Int4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(uint4, uint32b, UInt4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(long4, int64b, Long4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(ulong4, uint64b, ULong4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(float4, float, Float4)
ZINVUL_TYPE_VALUE_MAKER_TEMPLATE_SPECIALIZATION_IMPL4(double4, double, Double4)

/*!
  */
template <typename Type, typename ...ArgumentTypes> inline
Type make(const ArgumentTypes... args) noexcept
{
  const auto data = inner::TypeValueMaker<Type>::make(args...);
  return data;
}

namespace inner {

/*!
  */
template <typename Type>
struct TypeConverter
{
  template <typename T>
  static Type cast(T value) noexcept
  {
    auto result = static_cast<Type>(value);
    return result;
  }

  template <typename T>
  static Type treatAs(T object) noexcept
  {
    auto result = reinterpret_cast<Type>(object);
    return result;
  }
};

#define ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(Type) \
    ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(Type, Type)

#define ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(Type, name) \
  template <> \
  struct TypeConverter< Type > \
  { \
    template <typename T> \
    static Type cast(T value) noexcept \
    { \
      auto result = convert_ ## name (value); \
      return result; \
    } \
    template <typename T> \
    static Type treatAs(T object) noexcept \
    { \
      static_assert(sizeof(T) == sizeof(Type), \
                    "The size of T doesn't match the size of Type."); \
      union Data \
      { \
        T src_; \
        Type result_; \
      }; \
      Data data = {object}; \
      return data.result_; \
    } \
  }

ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(int8b, char);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(char2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(char3);
//ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(char4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(uint8b, uchar);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(uchar2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(uchar3);
//ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(uchar4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(int16b, short);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(short2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(short3);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(short4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(uint16b, ushort);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(ushort2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(ushort3);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(ushort4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(int32b, int);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(int2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(int3);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(int4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(uint32b, uint);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(uint2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(uint3);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(uint4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(int64b, long);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(long2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(long3);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(long4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL2(uint64b, ulong);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(ulong2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(ulong3);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(ulong4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(float);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(float2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(float3);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(float4);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(double);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(double2);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(double3);
ZINVUL_TYPE_CONVERTER_TEMPLATE_SPECIALIZATION_IMPL(double4);

#if defined(ZINVUL_CPU)

template <AddressSpaceType kASpaceType, typename Type>
struct TypeConverter<AddressSpacePointer<kASpaceType, Type>>
{
  using ASpacePointer = AddressSpacePointer<kASpaceType, Type>;

  template <typename T>
  static ASpacePointer cast(AddressSpacePointer<kASpaceType, T> value) noexcept
  {
    return treatAs(value);
  }

  template <typename T>
  static ASpacePointer treatAs(AddressSpacePointer<kASpaceType, T> object) noexcept
  {
    auto data = reinterpret_cast<typename ASpacePointer::Pointer>(object.get());
    return ASpacePointer{data};
  }
};

#endif // ZINVUL_CPU

} // namespace inner

/*!
  */
template <typename Type, typename T> inline
Type cast(T value) noexcept
{
  auto result = inner::TypeConverter<Type>::cast(value);
  return result;
}

/*!
  */
template <typename Type, typename T> inline
Type treatAs(T object) noexcept
{
  auto result = inner::TypeConverter<Type>::treatAs(object);
  return result;
}

/*!
  */
template <typename Type> inline
Type&& forward(RemoveReferenceType<Type>& t) noexcept
{
  return static_cast<Type&&>(t);
}

/*!
  */
template <typename Type> inline
Type&& forward(RemoveReferenceType<Type>&& t) noexcept
{
  static_assert(!kIsLValueReference<Type>, "The Type is lvalue reference.");
  return static_cast<Type&&>(t);
}

} // namespace zinvul

#endif /* ZINVUL_UTILITY_INL_CL */
