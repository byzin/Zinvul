/*!
  \file vector.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUILT_IN_FUNC_TEST_VECTOR_CL
#define ZINVUL_BUILT_IN_FUNC_TEST_VECTOR_CL

// Zinvul
#include "zinvul/cl/array.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/type_traits.cl"
#include "zinvul/cl/utility.cl"
#include "zinvul/cl/vector_data.cl"

using namespace zinvul;

// Forward declaration
__kernel void testInt8bVectorData(ConstGlobalPtr<int8b> inputs,
    GlobalPtr<int8b> outputs,
    const uint32b resolution);
__kernel void testInt16bVectorData(ConstGlobalPtr<int16b> inputs,
    GlobalPtr<int16b> outputs,
    const uint32b resolution);
__kernel void testInt32bVectorData(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution);
__kernel void testUInt32bVectorData(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> outputs,
    const uint32b resolution);
__kernel void testFloatVectorData(ConstGlobalPtr<float> inputs,
    GlobalPtr<float> outputs,
    const uint32b resolution);
__kernel void testHalfVectorData(ConstGlobalPtr<half> inputs,
    GlobalPtr<half> outputs,
    const uint32b resolution);
__kernel void testHalfUVectorData(ConstGlobalPtr<uint16b> inputs,
    GlobalPtr<float> outputs1,
    GlobalPtr<uint16b> outputs2,
    const uint32b resolution);


__kernel void testInt8bVectorData(ConstGlobalPtr<int8b> inputs,
    GlobalPtr<int8b> outputs,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    constexpr int8b shift = 1;
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      using VData = VectorData<int8b>;
      int8b v = VData::load(0, inputs + offset);
      v = v << shift;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<char2>;
      char2 v = VData::load(0, inputs + offset);
      v = v << shift;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<char3>;
      char3 v = VData::load(0, inputs + offset);
      v = v << shift;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<char4>;
      char4 v = VData::load(0, inputs + offset);
      v = v << shift;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
  }
}

__kernel void testInt16bVectorData(ConstGlobalPtr<int16b> inputs,
    GlobalPtr<int16b> outputs,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    constexpr int16b shift = 1;
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      using VData = VectorData<int16b>;
      int16b v = VData::load(0, inputs + offset);
      v = v << shift;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<short2>;
      short2 v = VData::load(0, inputs + offset);
      v = v << shift;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<short3>;
      short3 v = VData::load(0, inputs + offset);
      v = v << shift;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<short4>;
      short4 v = VData::load(0, inputs + offset);
      v = v << shift;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
  }
}

/*!
  */
__kernel void testInt32bVectorData(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      using VData = VectorData<int32b>;
      int32b v = VData::load(0, inputs + offset);
      v = 2 * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<int2>;
      int2 v = VData::load(0, inputs + offset);
      v = 2 * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<int3>;
      int3 v = VData::load(0, inputs + offset);
      v = 2 * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<int4>;
      int4 v = VData::load(0, inputs + offset);
      v = 2 * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
  }
}

/*!
  */
__kernel void testUInt32bVectorData(ConstGlobalPtr<uint32b> inputs,
    GlobalPtr<uint32b> outputs,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      using VData = VectorData<uint32b>;
      uint32b v = VData::load(0, inputs + offset);
      v = 2u * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<uint2>;
      uint2 v = VData::load(0, inputs + offset);
      v = 2u * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<uint3>;
      uint3 v = VData::load(0, inputs + offset);
      v = 2u * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<uint4>;
      uint4 v = VData::load(0, inputs + offset);
      v = 2u * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
  }
}

/*!
  */
__kernel void testFloatVectorData(ConstGlobalPtr<float> inputs,
    GlobalPtr<float> outputs,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      using VData = VectorData<float>;
      float v = VData::load(0, inputs + offset);
      v = 2.0f * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<float2>;
      float2 v = VData::load(0, inputs + offset);
      v = 2.0f * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<float3>;
      float3 v = VData::load(0, inputs + offset);
      v = 2.0f * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<float4>;
      float4 v = VData::load(0, inputs + offset);
      v = 2.0f * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
  }
}

/*!
  */
__kernel void testHalfVectorData(ConstGlobalPtr<half> inputs,
    GlobalPtr<half> outputs,
    const uint32b resolution)
{
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      using VData = VectorData<half>;
      float v = VData::load(0, inputs + offset);
      v = 3.0f * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<half2>;
      float2 v = VData::load(0, inputs + offset);
      v = 3.0f * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<half3>;
      float3 v = VData::load(0, inputs + offset);
      v = 3.0f * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
    }
    {
      using VData = VectorData<half4>;
      float4 v = VData::load(0, inputs + offset);
      v = 3.0f * v;
      VData::store(v, 0, outputs + offset);
      offset += VData::size();
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
  const uint32b index = getGlobalIdX();
  if (index < resolution) {
    constexpr size_t n = 10u;
    size_t offset = n * index;
    {
      using FData = VectorData<float>;
      using VData = VectorData<uint16b>;
      float v = VData::loadHalfU(0, inputs + offset);
      FData::store(v, 0, outputs1 + offset);
      v = 3.0f * v;
      VData::storeHalfU(v, 0, outputs2 + offset);
      offset += VData::size();
    }
    {
      using FData = VectorData<float2>;
      using VData = VectorData<ushort2>;
      float2 v = VData::loadHalfU(0, inputs + offset);
      FData::store(v, 0, outputs1 + offset);
      v = 3.0f * v;
      VData::storeHalfU(v, 0, outputs2 + offset);
      offset += VData::size();
    }
    {
      using FData = VectorData<float3>;
      using VData = VectorData<ushort3>;
      float3 v = VData::loadHalfU(0, inputs + offset);
      FData::store(v, 0, outputs1 + offset);
      v = 3.0f * v;
      VData::storeHalfU(v, 0, outputs2 + offset);
      offset += VData::size();
    }
    {
      using FData = VectorData<float4>;
      using VData = VectorData<ushort4>;
      float4 v = VData::loadHalfU(0, inputs + offset);
      FData::store(v, 0, outputs1 + offset);
      v = 3.0f * v;
      VData::storeHalfU(v, 0, outputs2 + offset);
      offset += VData::size();
      if (index == (resolution - 1)) {
        for (size_t j = 0; j < 2; ++j) {
          v = VData::loadHalfU(0, inputs + offset);
          VData::storeHalfU(v, 0, outputs2 + offset);
          offset += VData::size();
        }
      }
    }
  }
}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_VECTOR_CL */
