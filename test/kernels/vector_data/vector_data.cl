/*!
  \file vector_data.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_VECTOR_DATA_TEST_VECTOR_DATA_CL
#define ZINVUL_VECTOR_DATA_TEST_VECTOR_DATA_CL

// Zinvul
#include "zinvul/cl/array.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/type_traits.cl"
#include "zinvul/cl/utility.cl"
#include "zinvul/cl/vector_data.cl"

using zinvul::int8b;
using zinvul::int16b;
using zinvul::int32b;
using zinvul::int64b;
using zinvul::uint8b;
using zinvul::uint16b;
using zinvul::uint32b;
using zinvul::uint64b;
using zinvul::GlobalPtr;
using zinvul::ConstGlobalPtr;
using zinvul::ConstantPtr;
using zinvul::ConstConstantPtr;
using zinvul::Local;
using zinvul::LocalPtr;
using zinvul::ConstLocalPtr;
using zinvul::Private;
using zinvul::PrivatePtr;
using zinvul::ConstPrivatePtr;
using zinvul::cast;
using zinvul::treatAs;

// Forward declaration
__kernel void testInt8bVectorData(ConstGlobalPtr<int8b> inputs,
    GlobalPtr<int8b> outputs,
    const uint32b resolution);
__kernel void testInt8bVectorDataLocal(ConstGlobalPtr<int8b> inputs,
    GlobalPtr<int8b> outputs,
    const uint32b resolution);
__kernel void testInt8bVectorDataPrivate(ConstGlobalPtr<int8b> inputs,
    GlobalPtr<int8b> outputs,
    const uint32b resolution);
__kernel void testInt16bVectorData(ConstGlobalPtr<int16b> inputs,
    GlobalPtr<int16b> outputs,
    const uint32b resolution);
__kernel void testInt16bVectorDataLocal(ConstGlobalPtr<int16b> inputs,
    GlobalPtr<int16b> outputs,
    const uint32b resolution);
__kernel void testInt16bVectorDataPrivate(ConstGlobalPtr<int16b> inputs,
    GlobalPtr<int16b> outputs,
    const uint32b resolution);
__kernel void testInt32bVectorData(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution);
__kernel void testInt32bVectorDataLocal(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution);
__kernel void testInt32bVectorDataPrivate(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution);
__kernel void testUInt32bVectorData(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> outputs,
    const uint32b resolution);
__kernel void testUInt32bVectorDataLocal(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> outputs,
    const uint32b resolution);
__kernel void testUInt32bVectorDataPrivate(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> outputs,
    const uint32b resolution);
__kernel void testFloatVectorData(ConstGlobalPtr<float> inputs,
    GlobalPtr<float> outputs,
    const uint32b resolution);
__kernel void testFloatVectorDataLocal(ConstGlobalPtr<float> inputs,
    GlobalPtr<float> outputs,
    const uint32b resolution);
__kernel void testFloatVectorDataPrivate(ConstGlobalPtr<float> inputs,
    GlobalPtr<float> outputs,
    const uint32b resolution);
__kernel void testHalfVectorData(ConstGlobalPtr<half> inputs,
    GlobalPtr<half> outputs,
    const uint32b resolution);
__kernel void testHalfUVectorData(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<float> outputs1,
    GlobalPtr<uint16b> outputs2,
    const uint32b resolution);
__kernel void testHalfUVectorDataLocal(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<float> outputs1,
    GlobalPtr<uint16b> outputs2,
    const uint32b resolution);
__kernel void testHalfUVectorDataPrivate(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<float> outputs1,
    GlobalPtr<uint16b> outputs2,
    const uint32b resolution);

namespace test {

constexpr zinvul::Constant<uint32b> kMaxLocalSize = 64u;

template <size_t kN, typename SrcType, typename DstType>
void copyArray(const SrcType src,
    const size_t src_offset, 
    DstType dst,
    const size_t dst_offset) noexcept;

template <size_t kN, typename SrcType, typename DstType>
void copyArray(const SrcType src,
    const size_t src_offset, 
    DstType dst,
    const size_t dst_offset) noexcept
{
  const size_t index = kN - 1;
  dst[index + dst_offset] = src[index + src_offset];
  if constexpr (1 < kN)
    copyArray<kN - 1>(src, src_offset, dst, dst_offset);
}

} // namespace test

__kernel void testInt8bVectorData(ConstGlobalPtr<int8b> inputs,
    GlobalPtr<int8b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr int8b shift = 1;
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      int8b v = zinvul::VectorData::load<1>(0, inputs + offset);
      v = static_cast<int8b>(v << shift);
      zinvul::VectorData::store(v, 0, outputs + offset);
      offset += 1;
    }
    {
      char2 v = zinvul::vload2(0, inputs + offset);
      v = v << shift;
      zinvul::vstore2(v, 0, outputs + offset);
      offset += 2;
    }
    {
      char3 v = zinvul::vload3(0, inputs + offset);
      v = v << shift;
      zinvul::vstore3(v, 0, outputs + offset);
      offset += 3;
    }
    {
      char4 v = zinvul::vload4(0, inputs + offset);
      v = v << shift;
      zinvul::vstore4(v, 0, outputs + offset);
      offset += 4;
    }
  }
}

/*!
  */
__kernel void testInt8bVectorDataLocal(ConstGlobalPtr<int8b> inputs,
    GlobalPtr<int8b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();

  constexpr size_t n = 10u;
  Local<int8b> storage[n * test::kMaxLocalSize];

  if (index < resolution) {
    const size_t loffset = n * zinvul::getLocalIdX();
    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, loffset);

    constexpr int8b m = 2;
    size_t offset = 0;
    LocalPtr<int8b> storage_ptr = storage + loffset;
    {
      int8b v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = static_cast<int8b>(m * v);
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      char2 v = zinvul::vload2(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      char3 v = zinvul::vload3(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      char4 v = zinvul::vload4(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, loffset, outputs, goffset);
  }
}

/*!
  */
__kernel void testInt8bVectorDataPrivate(ConstGlobalPtr<int8b> inputs,
    GlobalPtr<int8b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    int8b storage[n];

    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, 0);

    constexpr int8b m = 2;
    size_t offset = 0;
    PrivatePtr<int8b> storage_ptr = storage;
    {
      int8b v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = static_cast<int8b>(m * v);
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      char2 v = zinvul::vload2(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      char3 v = zinvul::vload3(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      char4 v = zinvul::vload4(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, 0, outputs, goffset);
  }
}

__kernel void testInt16bVectorData(ConstGlobalPtr<int16b> inputs,
    GlobalPtr<int16b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr int16b shift = 1;
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      int16b v = zinvul::VectorData::load<1>(0, inputs + offset);
      v = static_cast<int16b>(v << shift);
      zinvul::VectorData::store(v, 0, outputs + offset);
      offset += 1;
    }
    {
      short2 v = zinvul::vload2(0, inputs + offset);
      v = v << shift;
      zinvul::vstore2(v, 0, outputs + offset);
      offset += 2;
    }
    {
      short3 v = zinvul::vload3(0, inputs + offset);
      v = v << shift;
      zinvul::vstore3(v, 0, outputs + offset);
      offset += 3;
    }
    {
      short4 v = zinvul::vload4(0, inputs + offset);
      v = v << shift;
      zinvul::vstore4(v, 0, outputs + offset);
      offset += 4;
    }
  }
}

/*!
  */
__kernel void testInt16bVectorDataLocal(ConstGlobalPtr<int16b> inputs,
    GlobalPtr<int16b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();

  constexpr size_t n = 10u;
  Local<int16b> storage[n * test::kMaxLocalSize];

  if (index < resolution) {
    const size_t loffset = n * zinvul::getLocalIdX();
    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, loffset);

    constexpr int16b m = 2;
    size_t offset = 0;
    LocalPtr<int16b> storage_ptr = storage + loffset;
    {
      int16b v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = static_cast<int16b>(m * v);
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      short2 v = zinvul::vload2(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      short3 v = zinvul::vload3(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      short4 v = zinvul::vload4(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, loffset, outputs, goffset);
  }
}

/*!
  */
__kernel void testInt16bVectorDataPrivate(ConstGlobalPtr<int16b> inputs,
    GlobalPtr<int16b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    int16b storage[n];

    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, 0);

    constexpr int16b m = 2;
    size_t offset = 0;
    PrivatePtr<int16b> storage_ptr = storage;
    {
      int16b v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = static_cast<int16b>(m * v);
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      short2 v = zinvul::vload2(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      short3 v = zinvul::vload3(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      short4 v = zinvul::vload4(0, storage_ptr + offset);
      v = m * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, 0, outputs, goffset);
  }
}

/*!
  */
__kernel void testInt32bVectorData(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      int32b v = zinvul::VectorData::load<1>(0, inputs + offset);
      v = 2 * v;
      zinvul::VectorData::store(v, 0, outputs + offset);
      offset += 1;
    }
    {
      int2 v = zinvul::vload2(0, inputs + offset);
      v = 2 * v;
      zinvul::vstore2(v, 0, outputs + offset);
      offset += 2;
    }
    {
      int3 v = zinvul::vload3(0, inputs + offset);
      v = 2 * v;
      zinvul::vstore3(v, 0, outputs + offset);
      offset += 3;
    }
    {
      int4 v = zinvul::vload4(0, inputs + offset);
      v = 2 * v;
      zinvul::vstore4(v, 0, outputs + offset);
      offset += 4; 
    }
  }
}

/*!
  */
__kernel void testInt32bVectorDataLocal(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();

  constexpr size_t n = 10u;
  Local<int32b> storage[n * test::kMaxLocalSize];

  if (index < resolution) {
    const size_t loffset = n * zinvul::getLocalIdX();
    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, loffset);

    size_t offset = 0;
    LocalPtr<int32b> storage_ptr = storage + loffset;
    {
      int32b v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = 2 * v;
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      int2 v = zinvul::vload2(0, storage_ptr + offset);
      v = 2 * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      int3 v = zinvul::vload3(0, storage_ptr + offset);
      v = 2 * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      int4 v = zinvul::vload4(0, storage_ptr + offset);
      v = 2 * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, loffset, outputs, goffset);
  }
}

/*!
  */
__kernel void testInt32bVectorDataPrivate(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    int32b storage[n];

    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, 0);

    size_t offset = 0;
    PrivatePtr<int32b> storage_ptr = storage;
    {
      int32b v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = 2 * v;
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      int2 v = zinvul::vload2(0, storage_ptr + offset);
      v = 2 * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      int3 v = zinvul::vload3(0, storage_ptr + offset);
      v = 2 * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      int4 v = zinvul::vload4(0, storage_ptr + offset);
      v = 2 * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, 0, outputs, goffset);
  }
}

/*!
  */
__kernel void testUInt32bVectorData(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      uint32b v = zinvul::VectorData::load<1>(0, inputs + offset);
      v = 2u * v;
      zinvul::VectorData::store(v, 0, outputs + offset);
      offset += 1;
    }
    {
      uint2 v = zinvul::vload2(0, inputs + offset);
      v = 2u * v;
      zinvul::vstore2(v, 0, outputs + offset);
      offset += 2;
    }
    {
      uint3 v = zinvul::vload3(0, inputs + offset);
      v = 2u * v;
      zinvul::vstore3(v, 0, outputs + offset);
      offset += 3;
    }
    {
      uint4 v = zinvul::vload4(0, inputs + offset);
      v = 2u * v;
      zinvul::vstore4(v, 0, outputs + offset);
      offset += 4;
    }
  }
}

/*
  */
__kernel void testUInt32bVectorDataLocal(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();

  constexpr size_t n = 10u;
  Local<uint32b> storage[n * test::kMaxLocalSize];

  if (index < resolution) {
    const size_t loffset = n * zinvul::getLocalIdX();
    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, loffset);

    size_t offset = 0;
    LocalPtr<uint32b> storage_ptr = storage + loffset;
    {
      uint32b v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = 2u * v;
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      uint2 v = zinvul::vload2(0, storage_ptr + offset);
      v = 2u * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      uint3 v = zinvul::vload3(0, storage_ptr + offset);
      v = 2u * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      uint4 v = zinvul::vload4(0, storage_ptr + offset);
      v = 2u * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, loffset, outputs, goffset);
  }
}

/*!
  */
__kernel void testUInt32bVectorDataPrivate(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    uint32b storage[n];

    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, 0);

    size_t offset = 0;
    PrivatePtr<uint32b> storage_ptr = storage;
    {
      uint32b v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = 2u * v;
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      uint2 v = zinvul::vload2(0, storage_ptr + offset);
      v = 2u * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      uint3 v = zinvul::vload3(0, storage_ptr + offset);
      v = 2u * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      uint4 v = zinvul::vload4(0, storage_ptr + offset);
      v = 2u * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, 0, outputs, goffset);
  }
}

/*!
  */
__kernel void testFloatVectorData(ConstGlobalPtr<float> inputs,
    GlobalPtr<float> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      float v = zinvul::VectorData::load<1>(0, inputs + offset);
      v = 2.0f * v;
      zinvul::VectorData::store(v, 0, outputs + offset);
      offset += 1;
    }
    {
      float2 v = zinvul::vload2(0, inputs + offset);
      v = 2.0f * v;
      zinvul::vstore2(v, 0, outputs + offset);
      offset += 2;
    }
    {
      float3 v = zinvul::vload3(0, inputs + offset);
      v = 2.0f * v;
      zinvul::vstore3(v, 0, outputs + offset);
      offset += 3;
    }
    {
      float4 v = zinvul::vload4(0, inputs + offset);
      v = 2.0f * v;
      zinvul::vstore4(v, 0, outputs + offset);
      offset += 4;
    }
  }
}

/*!
  */
__kernel void testFloatVectorDataLocal(ConstGlobalPtr<float> inputs,
    GlobalPtr<float> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();

  constexpr size_t n = 10u;
  Local<float> storage[n * test::kMaxLocalSize];

  if (index < resolution) {
    const size_t loffset = n * zinvul::getLocalIdX();
    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, loffset);

    size_t offset = 0;
    LocalPtr<float> storage_ptr = storage + loffset;
    {
      float v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = 2.0f * v;
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      float2 v = zinvul::vload2(0, storage_ptr + offset);
      v = 2.0f * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      float3 v = zinvul::vload3(0, storage_ptr + offset);
      v = 2.0f * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      float4 v = zinvul::vload4(0, storage_ptr + offset);
      v = 2.0f * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, loffset, outputs, goffset);
  }
}

/*!
  */
__kernel void testFloatVectorDataPrivate(ConstGlobalPtr<float> inputs,
    GlobalPtr<float> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    float storage[n];

    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, 0);

    size_t offset = 0;
    PrivatePtr<float> storage_ptr = storage;
    {
      float v = zinvul::VectorData::load<1>(0, storage_ptr + offset);
      v = 2.0f * v;
      zinvul::VectorData::store(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      float2 v = zinvul::vload2(0, storage_ptr + offset);
      v = 2.0f * v;
      zinvul::vstore2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      float3 v = zinvul::vload3(0, storage_ptr + offset);
      v = 2.0f * v;
      zinvul::vstore3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      float4 v = zinvul::vload4(0, storage_ptr + offset);
      v = 2.0f * v;
      zinvul::vstore4(v, 0, storage_ptr + offset);
      offset += 4; 
    }

    test::copyArray<n>(storage, 0, outputs, goffset);
  }
}

/*!
  */
__kernel void testHalfVectorData(ConstGlobalPtr<half> inputs,
    GlobalPtr<half> outputs,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      float v = zinvul::vload_half(0, inputs + offset);
      v = 3.0f * v;
      zinvul::vstore_half(v, 0, outputs + offset);
      offset += 1;
    }
    {
      float2 v = zinvul::vload_half2(0, inputs + offset);
      v = 3.0f * v;
      zinvul::vstore_half2(v, 0, outputs + offset);
      offset += 2;
    }
    {
      float3 v = zinvul::vload_half3(0, inputs + offset);
      v = 3.0f * v;
      zinvul::vstore_half3(v, 0, outputs + offset);
      offset += 3;
    }
    {
      float4 v = zinvul::vload_half4(0, inputs + offset);
      v = 3.0f * v;
      zinvul::vstore_half4(v, 0, outputs + offset);
      offset += 4;
    }
  }
}

/*!
  */
__kernel void testHalfUVectorData(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<float> outputs1,
    GlobalPtr<uint16b> outputs2,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      float v = zinvul::vload_half(0, inputs + offset);
      zinvul::VectorData::store(v, 0, outputs1 + offset);
      v = 3.0f * v;
      zinvul::vstore_half(v, 0, outputs2 + offset);
      offset += 1;
    }
    {
      float2 v = zinvul::vload_half2(0, inputs + offset);
      zinvul::vstore2(v, 0, outputs1 + offset);
      v = 3.0f * v;
      zinvul::vstore_half2(v, 0, outputs2 + offset);
      offset += 2;
    }
    {
      float3 v = zinvul::vload_half3(0, inputs + offset);
      zinvul::vstore3(v, 0, outputs1 + offset);
      v = 3.0f * v;
      zinvul::vstore_half3(v, 0, outputs2 + offset);
      offset += 3;
    }
    {
      float4 v = zinvul::vload_half4(0, inputs + offset);
      zinvul::vstore4(v, 0, outputs1 + offset);
      v = 3.0f * v;
      zinvul::vstore_half4(v, 0, outputs2 + offset);
      offset += 4;

      // Special case
      if (index == (resolution - 1)) {
        for (size_t j = 0; j < 2; ++j) {
          v = zinvul::vload_half4(0, inputs + offset);
          zinvul::vstore_half4(v, 0, outputs2 + offset);
          offset += 4;
        }
      }
    }
  }
}

/*!
  */
__kernel void testHalfUVectorDataLocal(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<float> outputs1,
    GlobalPtr<uint16b> outputs2,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();

  constexpr size_t n = 10u;
  Local<uint16b> storage[n * test::kMaxLocalSize];

  if (index < resolution) {
    const size_t loffset = n * zinvul::getLocalIdX();
    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, loffset);

    size_t offset = 0;
    LocalPtr<uint16b> storage_ptr = storage + loffset;
    {
      float v = zinvul::vload_half(0, storage_ptr + offset);
      zinvul::VectorData::store(v, 0, outputs1 + (goffset + offset));
      v = 3.0f * v;
      zinvul::vstore_half(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      float2 v = zinvul::vload_half2(0, storage_ptr + offset);
      zinvul::vstore2(v, 0, outputs1 + (goffset + offset));
      v = 3.0f * v;
      zinvul::vstore_half2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      float3 v = zinvul::vload_half3(0, storage_ptr + offset);
      zinvul::vstore3(v, 0, outputs1 + (goffset + offset));
      v = 3.0f * v;
      zinvul::vstore_half3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      float4 v = zinvul::vload_half4(0, storage_ptr + offset);
      zinvul::vstore4(v, 0, outputs1 + (goffset + offset));
      v = 3.0f * v;
      zinvul::vstore_half4(v, 0, storage_ptr + offset);
      offset += 4; 

      // Special case
      if (index == (resolution - 1)) {
        for (size_t j = 0; j < 2; ++j) {
          v = zinvul::vload_half4(0, inputs + (goffset + offset));
          zinvul::vstore_half4(v, 0, outputs2 + (goffset + offset));
          offset += 4;
        }
      }
    }

    test::copyArray<n>(storage, loffset, outputs2, goffset);
  }
}

/*!
  */
__kernel void testHalfUVectorDataPrivate(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<float> outputs1,
    GlobalPtr<uint16b> outputs2,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    uint16b storage[n];

    const size_t goffset = n * index;
    test::copyArray<n>(inputs, goffset, storage, 0);

    size_t offset = 0;
    PrivatePtr<uint16b> storage_ptr = storage;
    {
      float v = zinvul::vload_half(0, storage_ptr + offset);
      zinvul::VectorData::store(v, 0, outputs1 + (goffset + offset));
      v = 3.0f * v;
      zinvul::vstore_half(v, 0, storage_ptr + offset);
      offset += 1;
    }
    {
      float2 v = zinvul::vload_half2(0, storage_ptr + offset);
      zinvul::vstore2(v, 0, outputs1 + (goffset + offset));
      v = 3.0f * v;
      zinvul::vstore_half2(v, 0, storage_ptr + offset);
      offset += 2;
    }
    {
      float3 v = zinvul::vload_half3(0, storage_ptr + offset);
      zinvul::vstore3(v, 0, outputs1 + (goffset + offset));
      v = 3.0f * v;
      zinvul::vstore_half3(v, 0, storage_ptr + offset);
      offset += 3;
    }
    {
      float4 v = zinvul::vload_half4(0, storage_ptr + offset);
      zinvul::vstore4(v, 0, outputs1 + (goffset + offset));
      v = 3.0f * v;
      zinvul::vstore_half4(v, 0, storage_ptr + offset);
      offset += 4; 

      // Special case
      if (index == (resolution - 1)) {
        for (size_t j = 0; j < 2; ++j) {
          v = zinvul::vload_half4(0, inputs + (goffset + offset));
          zinvul::vstore_half4(v, 0, outputs2 + (goffset + offset));
          offset += 4;
        }
      }
    }

    test::copyArray<n>(storage, 0, outputs2, goffset);
  }
}

#endif /* ZINVUL_VECTOR_DATA_TEST_VECTOR_DATA_CL */
