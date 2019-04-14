/*!
  \file utility.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CL_UTILITY_HPP
#define ZINVUL_CL_UTILITY_HPP

// Standard C++ library
#include <array>
// Zinvul
#include "types.hpp"
#include "vector.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

namespace cl {

//! Set the work group ID
void __setWorkGroupId(const uint32b id) noexcept;

//! Set the work group size
void __setWorkGroupSize(const std::array<uint32b, 3>& size) noexcept;

// OpenCL functions

//! Write the result of the format to stdout
template <typename Type>
void printf(const char* format, const Type& value) noexcept;

// Work-Item functions

//! Return the global work-item ID
size_t get_global_id(const uint32b dimension) noexcept;

//! Return the offset values
constexpr size_t get_global_offset(const uint32b dimension) noexcept;

//! Return the number of global work-items
size_t get_global_size(const uint32b dimension) noexcept;

//! Return the work-group ID
size_t get_group_id(const uint32b dimension) noexcept;

//! Return the unique local work-item ID
constexpr size_t get_local_id(const uint32b dimension) noexcept;

//! Return the number of local work-item
constexpr size_t get_local_size(const uint32b dimension) noexcept;

//! Return the number of work-groups that will execute a kernel
size_t get_num_groups(const uint32b dimension) noexcept;

//! Return the number of dimensions in use
constexpr uint32b get_work_dim() noexcept;

//! Convert to a char value
template <typename Type>
int8b convert_char(Type&& value) noexcept;

//! Convert to a char2 value
template <typename Type>
char2 convert_char2(const Vector<Type, 2>& value) noexcept;

//! Convert to a char3 value
template <typename Type>
char3 convert_char3(const Vector<Type, 3>& value) noexcept;

//! Convert to a char4 value
template <typename Type>
char4 convert_char4(const Vector<Type, 4>& value) noexcept;

//! Convert to a uchar value
template <typename Type>
uint8b convert_uchar(Type&& value) noexcept;

//! Convert to a uchar2 value
template <typename Type>
uchar2 convert_uchar2(const Vector<Type, 2>& value) noexcept;

//! Convert to a uchar3 value
template <typename Type>
uchar3 convert_uchar3(const Vector<Type, 3>& value) noexcept;

//! Convert to a uchar4 value
template <typename Type>
uchar4 convert_uchar4(const Vector<Type, 4>& value) noexcept;

//! Convert to a short value
template <typename Type>
int16b convert_short(Type&& value) noexcept;

//! Convert to a short2 value
template <typename Type>
short2 convert_short2(const Vector<Type, 2>& value) noexcept;

//! Convert to a short3 value
template <typename Type>
short3 convert_short3(const Vector<Type, 3>& value) noexcept;

//! Convert to a short4 value
template <typename Type>
short4 convert_short4(const Vector<Type, 4>& value) noexcept;

//! Convert to a ushort value
template <typename Type>
uint16b convert_ushort(Type&& value) noexcept;

//! Convert to a ushort2 value
template <typename Type>
ushort2 convert_ushort2(const Vector<Type, 2>& value) noexcept;

//! Convert to a ushort3 value
template <typename Type>
ushort3 convert_ushort3(const Vector<Type, 3>& value) noexcept;

//! Convert to a ushort4 value
template <typename Type>
ushort4 convert_ushort4(const Vector<Type, 4>& value) noexcept;

//! Convert to a int value
template <typename Type>
int32b convert_int(Type&& value) noexcept;

//! Convert to a int2 value
template <typename Type>
int2 convert_int2(const Vector<Type, 2>& value) noexcept;

//! Convert to a int3 value
template <typename Type>
int3 convert_int3(const Vector<Type, 3>& value) noexcept;

//! Convert to a int4 value
template <typename Type>
int4 convert_int4(const Vector<Type, 4>& value) noexcept;

//! Convert to a uint value
template <typename Type>
uint32b convert_uint(Type&& value) noexcept;

//! Convert to a uint2 value
template <typename Type>
uint2 convert_uint2(const Vector<Type, 2>& value) noexcept;

//! Convert to a uint3 value
template <typename Type>
uint3 convert_uint3(const Vector<Type, 3>& value) noexcept;

//! Convert to a uint4 value
template <typename Type>
uint4 convert_uint4(const Vector<Type, 4>& value) noexcept;

//! Convert to a long value
template <typename Type>
int64b convert_long(Type&& value) noexcept;

//! Convert to a long2 value
template <typename Type>
long2 convert_long2(const Vector<Type, 2>& value) noexcept;

//! Convert to a long3 value
template <typename Type>
long3 convert_long3(const Vector<Type, 3>& value) noexcept;

//! Convert to a long4 value
template <typename Type>
long4 convert_long4(const Vector<Type, 4>& value) noexcept;

//! Convert to a ulong value
template <typename Type>
uint64b convert_ulong(Type&& value) noexcept;

//! Convert to a ulong2 value
template <typename Type>
ulong2 convert_ulong2(const Vector<Type, 2>& value) noexcept;

//! Convert to a ulong3 value
template <typename Type>
ulong3 convert_ulong3(const Vector<Type, 3>& value) noexcept;

//! Convert to a ulong4 value
template <typename Type>
ulong4 convert_ulong4(const Vector<Type, 4>& value) noexcept;

//! Convert to a float value
template <typename Type>
float convert_float(Type&& value) noexcept;

//! Convert to a float2 value
template <typename Type>
float2 convert_float2(const Vector<Type, 2>& value) noexcept;

//! Convert to a float3 value
template <typename Type>
float3 convert_float3(const Vector<Type, 3>& value) noexcept;

//! Convert to a float4 value
template <typename Type>
float4 convert_float4(const Vector<Type, 4>& value) noexcept;

//! A data is reinterpreted as a char data
template <typename Type>
int8b as_char(Type&& value) noexcept;

//! A data is reinterpreted as a char2 data
template <typename Type>
char2 as_char2(Type&& value) noexcept;

//! A data is reinterpreted as a char3 data
template <typename Type>
char3 as_char3(Type&& value) noexcept;

//! A data is reinterpreted as a char4 data
template <typename Type>
char4 as_char4(Type&& value) noexcept;

//! A data is reinterpreted as a uchar data
template <typename Type>
uint8b as_uchar(Type&& value) noexcept;

//! A data is reinterpreted as a uchar2 data
template <typename Type>
uchar2 as_uchar2(Type&& value) noexcept;

//! A data is reinterpreted as a uchar3 data
template <typename Type>
uchar3 as_uchar3(Type&& value) noexcept;

//! A data is reinterpreted as a uchar4 data
template <typename Type>
uchar4 as_uchar4(Type&& value) noexcept;

//! A data is reinterpreted as a short data
template <typename Type>
int16b as_short(Type&& value) noexcept;

//! A data is reinterpreted as a short2 data
template <typename Type>
short2 as_short2(Type&& value) noexcept;

//! A data is reinterpreted as a short3 data
template <typename Type>
short3 as_short3(Type&& value) noexcept;

//! A data is reinterpreted as a short4 data
template <typename Type>
short4 as_short4(Type&& value) noexcept;

//! A data is reinterpreted as a ushort data
template <typename Type>
uint16b as_ushort(Type&& value) noexcept;

//! A data is reinterpreted as a ushort2 data
template <typename Type>
ushort2 as_ushort2(Type&& value) noexcept;

//! A data is reinterpreted as a ushort3 data
template <typename Type>
ushort3 as_ushort3(Type&& value) noexcept;

//! A data is reinterpreted as a ushort4 data
template <typename Type>
ushort4 as_ushort4(Type&& value) noexcept;

//! A data is reinterpreted as a int data
template <typename Type>
int32b as_int(Type&& value) noexcept;

//! A data is reinterpreted as a int2 data
template <typename Type>
int2 as_int2(Type&& value) noexcept;

//! A data is reinterpreted as a int3 data
template <typename Type>
int3 as_int3(Type&& value) noexcept;

//! A data is reinterpreted as a int4 data
template <typename Type>
int4 as_int4(Type&& value) noexcept;

//! A data is reinterpreted as a uint data
template <typename Type>
uint32b as_uint(Type&& value) noexcept;

//! A data is reinterpreted as a uint2 data
template <typename Type>
uint2 as_uint2(Type&& value) noexcept;

//! A data is reinterpreted as a uint3 data
template <typename Type>
uint3 as_uint3(Type&& value) noexcept;

//! A data is reinterpreted as a uint4 data
template <typename Type>
uint4 as_uint4(Type&& value) noexcept;

//! A data is reinterpreted as a long data
template <typename Type>
int64b as_long(Type&& value) noexcept;

//! A data is reinterpreted as a long2 data
template <typename Type>
long2 as_long2(Type&& value) noexcept;

//! A data is reinterpreted as a long3 data
template <typename Type>
long3 as_long3(Type&& value) noexcept;

//! A data is reinterpreted as a long4 data
template <typename Type>
long4 as_long4(Type&& value) noexcept;

//! A data is reinterpreted as a ulong data
template <typename Type>
uint64b as_ulong(Type&& value) noexcept;

//! A data is reinterpreted as a ulong2 data
template <typename Type>
ulong2 as_ulong2(Type&& value) noexcept;

//! A data is reinterpreted as a ulong3 data
template <typename Type>
ulong3 as_ulong3(Type&& value) noexcept;

//! A data is reinterpreted as a ulong4 data
template <typename Type>
ulong4 as_ulong4(Type&& value) noexcept;

//! A data is reinterpreted as a float data
template <typename Type>
float as_float(Type&& value) noexcept;

//! A data is reinterpreted as a float2 data
template <typename Type>
float2 as_float2(Type&& value) noexcept;

//! A data is reinterpreted as a float3 data
template <typename Type>
float3 as_float3(Type&& value) noexcept;

//! A data is reinterpreted as a float4 data
template <typename Type>
float4 as_float4(Type&& value) noexcept;

} // namespace cl

} // namespace zinvul

#include "utility-inl.hpp"

#endif // ZINVUL_CL_UTILITY_HPP
