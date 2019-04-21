/*!
  \file vector.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DATA_TEST_VECTOR_CL
#define ZINVUL_DATA_TEST_VECTOR_CL

#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using namespace zinvul;

// Prototypes
__kernel void testVectorOperations(
    GlobalPtr<short2> buffer1, GlobalPtr<int3> buffer2, GlobalPtr<uint4> buffer3,
    GlobalPtr<float2> buffer4, GlobalPtr<int2> buffer5, GlobalPtr<int2> buffer6,
    GlobalPtr<int2> buffer7, GlobalPtr<int2> buffer8, GlobalPtr<int2> buffer9,
    GlobalPtr<int2> buffer10, GlobalPtr<int2> buffer11, GlobalPtr<int3> buffer12,
    GlobalPtr<int3> buffer13, GlobalPtr<int3> buffer14, GlobalPtr<int3> buffer15,
    GlobalPtr<float4> buffer16);
__kernel void testRelationalOperations(
    GlobalPtr<int32b> result1, GlobalPtr<int2> result2,
    GlobalPtr<int3> result3, GlobalPtr<int4> result4);

/*!
  */
__kernel void testVectorOperations(
    GlobalPtr<short2> buffer1, GlobalPtr<int3> buffer2, GlobalPtr<uint4> buffer3,
    GlobalPtr<float2> buffer4, GlobalPtr<int2> buffer5, GlobalPtr<int2> buffer6,
    GlobalPtr<int2> buffer7, GlobalPtr<int2> buffer8, GlobalPtr<int2> buffer9,
    GlobalPtr<int2> buffer10, GlobalPtr<int2> buffer11, GlobalPtr<int3> buffer12,
    GlobalPtr<int3> buffer13, GlobalPtr<int3> buffer14, GlobalPtr<int3> buffer15,
    GlobalPtr<float4> buffer16)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    // Addition operations
    {
      const short2 v0{1, 2};
      const short2 v1{3, 4};
      const short v2 = 5;
      const short2 v3{6, 7};
      const short v4 = 0;
      short2 result = v0 + v1;
      result = result + v2;
      result = v2 + result;
      result += v3;
      result += v4;
      buffer1[0] = result;
    }
    // Subtraction operatinos
    {
      const int3 v0{1, 2, 3};
      const int3 v1{4, 5, 6};
      const int v2 = 3;
      const int3 v3{7, 8, 9};
      const int v4 = -1;
      int3 result = v0 - v1;
      result = result - v2;
      result = v2 - result;
      result -= v3;
      result -= v4;
      buffer2[0] = result;
    }
    // Multiplication operations
    {
      const uint4 v0{1u, 2u, 3u, 4u};
      const uint4 v1{5u, 6u, 7u, 8u};
      const uint v2 = 2u;
      const uint4 v3{9u, 10u, 11u, 12u};
      const uint v4 = 3u;
      uint4 result = v0 * v1;
      result = result * v2;
      result = v2 * result;
      result *= v3;
      result *= v4;
      buffer3[0] = result;
    }
    // Division operations
    {
      const float2 v0{5.0f, 6.0f};
      const float2 v1{1.0f, 2.0f};
      const float v2 = 2.0f;
      const float2 v3{2.0f, 2.0f};
      const float v4 = 3.0f;
      buffer4[0] = v0 / v1;
      buffer4[1] = v0 / v2;
      buffer4[2] = v2 / v0;
      float2 result{1.0f, 1.0f};
      result /= v3;
      buffer4[3] = result;
      result = float2{1.0f, 1.0f};
      result /= v4;
      buffer4[4] = result;
    }
    // Increment and decrement
    {
      int2 v0{0, 1};
      buffer5[0] = ++v0;
      buffer5[1] = v0;
      buffer5[2] = v0++;
      buffer5[3] = v0;
      v0 = int2{0, 1};
      buffer5[4] = --v0;
      buffer5[5] = v0;
      buffer5[6] = v0--;
      buffer5[7] = v0;
    }
    // Reminder
    {
      const int2 v0{4, 4};
      const int2 v1{3, 3};
      const int v2 = 3;
      buffer6[0] = v0 % v1;
      buffer6[1] = v0 % v2;
      buffer6[2] = v2 % v0;
      int2 v3 = v0;
      v3 %= v1;
      buffer6[3] = v3;
      v3 = v0;
      v3 %= v2;
      buffer6[4] = v3;
    }
    // Bitwise AND
    {
      const int2 v0{3, 3};
      const int2 v1{-1, -1};
      const int v2 = -1;
      buffer7[0] = v0 & v1;
      buffer7[1] = v0 & v2;
      buffer7[2] = v2 & v0;
      int2 v3 = v0;
      v3 &= v1;
      buffer7[3] = v3;
      v3 = v0;
      v3 &= v2;
      buffer7[4] = v3;
    }
    // Bitwise OR 
    {
      const int2 v0{3, 3};
      const int2 v1{4, 4};
      const int32b v2 = 4;
      buffer8[0] = v0 | v1;
      buffer8[1] = v0 | v2;
      buffer8[2] = v2 | v0;
      int2 v3 = v0;
      v3 |= v1;
      buffer8[3] = v3;
      v3 = v0;
      v3 |= v2;
      buffer8[4] = v3;
    }
    // Bitwise XOR
    {
      const int2 v0{3, 3};
      const int2 v1{-1, -1};
      const int v2 = -1;
      buffer9[0] = v0 ^ v1;
      buffer9[1] = v0 ^ v2;
      buffer9[2] = v2 ^ v0;
      int2 v3 = v0;
      v3 ^= v1;
      buffer9[3] = v3;
      v3 = v0;
      v3 ^= v2;
      buffer9[4] = v3;
    }
    // Left shift
    {
      const int2 v0{1, 1};
      const int2 v1{2, 2};
      const int v2 = 2;
      buffer10[0] = v0 << v1;
      buffer10[1] = v0 << v2;
      int2 v3 = v0;
      v3 <<= v1;
      buffer10[2] = v3;
      v3 = v0;
      v3 <<= v2;
      buffer10[3] = v3;
    }
    // Right shift
    {
      const int2 v0{8, 8};
      const int2 v1{1, 1};
      const int v2 = 1;
      buffer11[0] = v0 >> v1;
      buffer11[1] = v0 >> v2;
      int2 v3 = v0;
      v3 >>= v1;
      buffer11[2] = v3;
      v3 = v0;
      v3 >>= v2;
      buffer11[3] = v3;
    }
    // Boolean AND
    {
      const int3 v0{0, 0, 1};
      const int3 v1{1, 0, 1};
      const int v2 = 1;
      buffer12[0] = v0 && v1;
      buffer12[1] = v0 && v2;
      buffer12[2] = v2 && v0;
    }
    // Boolean OR 
    {
      const int3 v0{0, 0, 1};
      const int3 v1{1, 0, 1};
      const int v2 = 1;
      buffer13[0] = v0 || v1;
      buffer13[1] = v0 || v2;
      buffer13[2] = v2 || v0;
    }
    // Equal operator and boolean opeartor
    {
      const uint3 v0{0u, 0u, 0u};
      const uint3 v1{0u, 1u, 0u};
      const uint v2 = 0u;
      const uint v3 = 1u;
      buffer14[0] = v0 == v0;
      buffer14[1] = v0 != v0;
      buffer14[2] = v0 == v1;
      buffer14[3] = v0 != v1;
      buffer14[4] = v0 == v2;
      buffer14[5] = v0 != v2;
      buffer14[6] = v2 == v0;
      buffer14[7] = v2 != v0;
      buffer14[8] = v1 == v3;
      buffer14[9] = v1 != v3;
      buffer14[10] = !(v1 != v3);
    }
    {
      const float3 v0{1.0f, 2.0f, 3.0f};
      const float3 v1{3.0f, 2.0f, 1.0f};
      const float v2 = 2.0f;
      buffer15[0] = v0 <= v0;
      buffer15[1] = v0 < v0;
      buffer15[2] = v0 >= v0;
      buffer15[3] = v0 > v0;
      buffer15[4] = v0 <= v1;
      buffer15[5] = v0 < v1;
      buffer15[6] = v0 >= v1;
      buffer15[7] = v0 > v1;
      buffer15[8] = v0 <= v2;
      buffer15[9] = v0 < v2;
      buffer15[10] = v0 >= v2;
      buffer15[11] = v0 > v2;
      buffer15[12] = v2 <= v0;
      buffer15[13] = v2 < v0;
      buffer15[14] = v2 >= v0;
      buffer15[15] = v2 > v0;
    }
    // Conditional operator
    {
      const float4 v0{1.0f, 2.0f, 3.0f, 4.0f};
      const float4 v1{5.0f, 6.0f, 7.0f, 8.0f};
      buffer16[0] = (buffer15[0].x == buffer15[2].x) ? v0 : v1;
      buffer16[1] = (buffer15[0].x == buffer15[1].x) ? v0 : v1;
    }
  }
}

/*!
  */
//__kernel void testHalfLoadStore(
//    ConstGlobalPtr<half> input_scalar,  GlobalPtr<half> output_scalar,
//    ConstGlobalPtr<half> input_vector2, GlobalPtr<half> output_vector2,
//    ConstGlobalPtr<half> input_vector3, GlobalPtr<half> output_vector3,
//    ConstGlobalPtr<half> input_vector4, GlobalPtr<half> output_vector4)
//{
//  const uint32b index = getGlobalIdX();
//  if (index == 0) {
//    // scalar
//    {
//      const float v0 = vload_half(0, input_scalar);
//      const float v1 = vload_half(1, input_scalar);
//      const float v2 = vload_half(2, input_scalar);
//      vstore_half(2.0f * v0, 0, output_scalar);
//      vstore_half(2.0f * v1, 1, output_scalar);
//      vstore_half(2.0f * v2, 2, output_scalar);
//    }
//    // vector2
//    {
//      const float2 v0 = vload_half2(0, input_vector2);
//      const float2 v1 = vload_half2(1, input_vector2);
//      vstore_half2(2.0f * v0, 0, output_vector2);
//      vstore_half2(2.0f * v1, 1, output_vector2);
//      (void)input_vector2;
//      (void)output_vector2;
//    }
//    // vector3
//    {
//      const float3 v0 = vload_half3(0, input_vector3);
//      const float3 v1 = vload_half3(1, input_vector3);
//      vstore_half3(2.0f * v0, 0, output_vector3);
//      vstore_half3(2.0f * v1, 1, output_vector3);
//      (void)input_vector3;
//      (void)output_vector3;
//    }
//    // vector4
//    {
//      const float4 v0 = vload_half4(0, input_vector4);
//      const float4 v1 = vload_half4(1, input_vector4);
//      vstore_half4(2.0f * v0, 0, output_vector4);
//      vstore_half4(2.0f * v1, 1, output_vector4);
//    }
//  }
//}

/*!
  */
__kernel void testRelationalOperations(
    GlobalPtr<int32b> result1, GlobalPtr<int2> result2,
    GlobalPtr<int3> result3, GlobalPtr<int4> result4)
{
  const uint32b index = getGlobalIdX();
  if (index == 0) {
    // Scalar
    size_t scalar_index = 0;
    {
      const int32b lhs = INT_MIN;
      const int32b rhs = INT_MAX;
      result1[scalar_index++] = lhs < rhs;
    }
    {
      const int32b lhs = INT_MIN;
      const int32b rhs = INT_MAX;
      result1[scalar_index++] = lhs <= rhs;
    }
    {
      const int32b lhs = INT_MIN;
      const int32b rhs = INT_MAX;
      result1[scalar_index++] = lhs > rhs;
    }
    {
      const int32b lhs = INT_MIN;
      const int32b rhs = INT_MAX;
      result1[scalar_index++] = lhs >= rhs;
    }

    {
      const int32b lhs = INT_MAX;
      const int32b rhs = INT_MIN;
      result1[scalar_index++] = lhs < rhs;
    }
    {
      const int32b lhs = INT_MAX;
      const int32b rhs = INT_MIN;
      result1[scalar_index++] = lhs <= rhs;
    }
    {
      const int32b lhs = INT_MAX;
      const int32b rhs = INT_MIN;
      result1[scalar_index++] = lhs > rhs;
    }
    {
      const int32b lhs = INT_MAX;
      const int32b rhs = INT_MIN;
      result1[scalar_index++] = lhs >= rhs;
    }

    {
      const int32b lhs = INT_MAX - 1;
      const int32b rhs = INT_MAX - 1;
      result1[scalar_index++] = lhs == rhs;
    }
    {
      const int32b lhs = INT_MAX - 1;
      const int32b rhs = INT_MAX - 1;
      result1[scalar_index++] = lhs < rhs;
    }
    {
      const int32b lhs = INT_MIN + 1;
      const int32b rhs = INT_MIN + 1;
      result1[scalar_index++] = lhs == rhs;
    }
    {
      const int32b lhs = INT_MIN + 1;
      const int32b rhs = INT_MIN + 1;
      result1[scalar_index++] = lhs < rhs;
    }

    {
      const int32b lhs = -1;
      const int32b rhs = 0;
      result1[scalar_index++] = lhs == rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 0;
      result1[scalar_index++] = lhs < rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 0;
      result1[scalar_index++] = lhs <= rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 0;
      result1[scalar_index++] = lhs > rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 0;
      result1[scalar_index++] = lhs >= rhs;
    }

    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      result1[scalar_index++] = lhs == rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      result1[scalar_index++] = lhs < rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      result1[scalar_index++] = lhs <= rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      result1[scalar_index++] = lhs > rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = 1;
      result1[scalar_index++] = lhs >= rhs;
    }

    {
      const int32b lhs = -1;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs == rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs < rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs <= rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs > rhs;
    }
    {
      const int32b lhs = -1;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs >= rhs;
    }

    {
      const int32b lhs = -2;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs == rhs;
    }
    {
      const int32b lhs = -2;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs < rhs;
    }
    {
      const int32b lhs = -2;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs <= rhs;
    }
    {
      const int32b lhs = -2;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs > rhs;
    }
    {
      const int32b lhs = -2;
      const int32b rhs = -1;
      result1[scalar_index++] = lhs >= rhs;
    }

    // vector2
    (void)result2;
    // vector3
    (void)result3;
    // vector4
    (void)result4;
  }
}

#endif /* ZINVUL_DATA_TEST_VECTOR_CL */
