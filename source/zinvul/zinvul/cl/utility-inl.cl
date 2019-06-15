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
// Zinvul
#include "types.cl"
#include "type_traits.cl"

//#if defined(ZINVUL_CPU)
////! Write the result of the format to stdout
//#define zPrint(format, value) printf(format, value)
//#else
//#define zPrint(format, value)
//#endif
//
//#if defined(Z_DEBUG_MODE) && defined(ZINVUL_CPU)
//#define zAssert(condition, message) ZISC_ASSERT(condition, message)
//#define zAssert2(c, message) ZISC_ASSERT(!zHasFalse2(c), message)
//#define zAssert3(c, message) ZISC_ASSERT(!zHasFalse3(c), message)
//#define zAssert4(c, message) ZISC_ASSERT(!zHasFalse4(c), message)
//#else
//#define zAssert(condition, message)
//#define zAssert2(c, message)
//#define zAssert3(c, message)
//#define zAssert4(c, message)
//#endif

namespace zinvul {

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

////! Convert int2 to uint2
//uint2 zI2ToU2(const int2 value)
//{
//  const uint2 result = zMakeUInt2((uint32b)value.x, (uint32b)value.y);
//  return result;
//}
//
////! Convert int3 to uint3
//uint3 zI3ToU3(const int3 value)
//{
//  const uint3 result = zMakeUInt3((uint32b)value.x, (uint32b)value.y,
//                                  (uint32b)value.z);
//  return result;
//}
//
////! Convert int4 to uint4
//uint4 zI4ToU4(const int4 value)
//{
//  const uint4 result = zMakeUInt4((uint32b)value.x, (uint32b)value.y,
//                                  (uint32b)value.z, (uint32b)value.w);
//  return result;
//}
//
////! Convert int2 to float2
//float2 zI2ToF2(const int2 value)
//{
//  const float2 result = zMakeFloat2((float)value.x, (float)value.y);
//  return result;
//}
//
////! Convert int3 to float3
//float3 zI3ToF3(const int3 value)
//{
//  const float3 result = zMakeFloat3((float)value.x, (float)value.y,
//                                    (float)value.z);
//  return result;
//}
//
////! Convert int4 to float4
//float4 zI4ToF4(const int4 value)
//{
//  const float4 result = zMakeFloat4((float)value.x, (float)value.y,
//                                    (float)value.z, (float)value.w);
//  return result;
//}
//
////! Convert uint2 to int2
//int2 zU2ToI2(const uint2 value)
//{
//  const int2 result = zMakeInt2((int32b)value.x, (int32b)value.y);
//  return result;
//}
//
////! Convert uint3 to int3
//int3 zU3ToI3(const uint3 value)
//{
//  const int3 result = zMakeInt3((int32b)value.x, (int32b)value.y,
//                                (int32b)value.z);
//  return result;
//}
//
////! Convert uint4 to int4
//int4 zU4ToI4(const uint4 value)
//{
//  const int4 result = zMakeInt4((int32b)value.x, (int32b)value.y,
//                                (int32b)value.z, (int32b)value.w);
//  return result;
//}
//
////! Convert uint2 to float2
//float2 zU2ToF2(const uint2 value)
//{
//  const float2 result = zMakeFloat2((float)value.x, (float)value.y);
//  return result;
//}
//
////! Convert uint3 to float3
//float3 zU3ToF3(const uint3 value)
//{
//  const float3 result = zMakeFloat3((float)value.x, (float)value.y,
//                                    (float)value.z);
//  return result;
//}
//
////! Convert uint4 to float4
//float4 zU4ToF4(const uint4 value)
//{
//  const float4 result = zMakeFloat4((float)value.x, (float)value.y,
//                                    (float)value.z, (float)value.w);
//  return result;
//}
//
////! Convert float2 to int2
//int2 zF2ToI2(const float2 value)
//{
//  const int2 result = zMakeInt2((int32b)value.x, (int32b)value.y);
//  return result;
//}
//
////! Convert float3 to int3
//int3 zF3ToI3(const float3 value)
//{
//  const int3 result = zMakeInt3((int32b)value.x, (int32b)value.y,
//                                (int32b)value.z);
//  return result;
//}
//
////! Convert float4 to int4
//int4 zF4ToI4(const float4 value)
//{
//  const int4 result = zMakeInt4((int32b)value.x, (int32b)value.y,
//                                (int32b)value.z, (int32b)value.w);
//  return result;
//}
//
////! Convert float2 to uint2
//uint2 zF2ToU2(const float2 value)
//{
//  const uint2 result = zMakeUInt2((uint32b)value.x, (uint32b)value.y);
//  return result;
//}
//
////! Convert float3 to uint3
//uint3 zF3ToU3(const float3 value)
//{
//  const uint3 result = zMakeUInt3((uint32b)value.x, (uint32b)value.y,
//                                  (uint32b)value.z);
//  return result;
//}
//
////! Convert float4 to uint4
//uint4 zF4ToU4(const float4 value)
//{
//  const uint4 result = zMakeUInt4((uint32b)value.x, (uint32b)value.y,
//                                  (uint32b)value.z, (uint32b)value.w);
//  return result;
//}
//
////! Broadcast a int value
//int2 zBroadcast2(const int32b value)
//{
//  const int2 result = zMakeInt2(value, value);
//  return result;
//}
//
////! Broadcast a int value
//int3 zBroadcast3(const int32b value)
//{
//  const int3 result = zMakeInt3(value, value, value);
//  return result;
//}
//
////! Broadcast a int value
//int4 zBroadcast4(const int32b value)
//{
//  const int4 result = zMakeInt4(value, value, value, value);
//  return result;
//}
//
////! Broadcast a uint value
//uint2 zBroadcastU2(const uint32b value)
//{
//  const uint2 result = zMakeUInt2(value, value);
//  return result;
//}
//
////! Broadcast a uint value
//uint3 zBroadcastU3(const uint32b value)
//{
//  const uint3 result = zMakeUInt3(value, value, value);
//  return result;
//}
//
////! Broadcast a uint value
//uint4 zBroadcastU4(const uint32b value)
//{
//  const uint4 result = zMakeUInt4(value, value, value, value);
//  return result;
//}
//
////! Broadcast a float value
//float2 zBroadcastF2(const float value)
//{
//  const float2 result = zMakeFloat2(value, value);
//  return result;
//}
//
////! Broadcast a float value
//float3 zBroadcastF3(const float value)
//{
//  const float3 result = zMakeFloat3(value, value, value);
//  return result;
//}
//
////! Broadcast a float value
//float4 zBroadcastF4(const float value)
//{
//  const float4 result = zMakeFloat4(value, value, value, value);
//  return result;
//}
//
////! Check a condition has false
//int32b zHasFalse2(const int2 c)
//{
//  const int32b result = (c.x & c.y) == 0;
//  return result;
//}
//
////! Check a condition has false
//int32b zHasFalse3(const int3 c)
//{
//  const int32b result = (c.x & c.y & c.z) == 0;
//  return result;
//}
//
////! Check a condition has false
//int32b zHasFalse4(const int4 c)
//{
//  const int32b result = (c.x & c.y & c.z & c.w) == 0;
//  return result;
//}
//
////! Check a condition has true
//int32b zHasTrue2(const int2 c)
//{
//  const int32b result = (c.x | c.y) != 0;
//  return result;
//}
//
////! Check a condition has true
//int32b zHasTrue3(const int3 c)
//{
//  const int32b result = (c.x | c.y | c.z) != 0;
//  return result;
//}
//
////! Check a condition has true
//int32b zHasTrue4(const int4 c)
//{
//  const int32b result = (c.x | c.y | c.z | c.w) != 0;
//  return result;
//}

namespace inner {

/*!
  */
template <typename Type>
struct TypeConverter
{
  template <typename T>
  static Type cast(T&& value) noexcept
  {
    auto result = static_cast<Type>(value);
    return result;
  }

  template <typename T>
  static Type treatAs(T&& object) noexcept
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
    static Type cast(T&& value) noexcept \
    { \
      auto result = convert_ ## name (value); \
      return result; \
    } \
    template <typename T> \
    static Type treatAs(T&& object) noexcept \
    { \
      auto result = *reinterpret_cast< const Type* >(&object); \
      return result; \
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
