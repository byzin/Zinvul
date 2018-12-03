/*!
  \file vector.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_DATA_TEST_VECTOR_CL
#define ZINVUL_DATA_TEST_VECTOR_CL

#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

/*!
  */
kernel void testVectorOperations(
    global short2* buffer1, global int3* buffer2, global uint4* buffer3,
    global float2* buffer4, global int2* buffer5, global int2* buffer6,
    global int2* buffer7, global int2* buffer8, global int2* buffer9,
    global int2* buffer10, global int2* buffer11, global int3* buffer12,
    global int3* buffer13, global int3* buffer14, global int3* buffer15)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    // Addition operations
    {
      const short2 v0 = zMakeShort2(1, 2);
      const short2 v1 = zMakeShort2(3, 4);
      const short v2 = 5;
      const short2 v3 = zMakeShort2(6, 7);
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
      const int3 v0 = zMakeInt3(1, 2, 3);
      const int3 v1 = zMakeInt3(4, 5, 6);
      const int v2 = 3;
      const int3 v3 = zMakeInt3(7, 8, 9);
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
      const uint4 v0 = zMakeUInt4(1u, 2u, 3u, 4u);
      const uint4 v1 = zMakeUInt4(5u, 6u, 7u, 8u);
      const uint v2 = 2u;
      const uint4 v3 = zMakeUInt4(9u, 10u, 11u, 12u);
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
      const float2 v0 = zMakeFloat2(5.0f, 6.0f);
      const float2 v1 = zMakeFloat2(1.0f, 2.0f);
      const float v2 = 2.0f;
      const float2 v3 = zMakeFloat2(2.0f, 2.0f);
      const float v4 = 3.0f;
      buffer4[0] = v0 / v1;
      buffer4[1] = v0 / v2;
      buffer4[2] = v2 / v0;
      float2 result = zMakeFloat2(1.0f, 1.0f);
      result /= v3;
      buffer4[3] = result;
      result = zMakeFloat2(1.0f, 1.0f);
      result /= v4;
      buffer4[4] = result;
    }
    // Increment and decrement
    {
      int2 v0 = zMakeInt2(0, 1);
      buffer5[0] = ++v0;
      buffer5[1] = v0;
      buffer5[2] = v0++;
      buffer5[3] = v0;
      v0 = zMakeInt2(0, 1);
      buffer5[4] = --v0;
      buffer5[5] = v0;
      buffer5[6] = v0--;
      buffer5[7] = v0;
    }
    // Reminder
    {
      const int2 v0 = zMakeInt2(4, 4);
      const int2 v1 = zMakeInt2(3, 3);
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
      const int2 v0 = zMakeInt2(3, 3);
      const int2 v1 = zMakeInt2(~0, ~0);
      const int v2 = ~0;
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
      const int2 v0 = zMakeInt2(3, 3);
      const int2 v1 = zMakeInt2(~0, ~0);
      const int v2 = ~0;
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
      const int2 v0 = zMakeInt2(3, 3);
      const int2 v1 = zMakeInt2(~0, ~0);
      const int v2 = ~0;
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
      const int2 v0 = zMakeInt2(1, 1);
      const int2 v1 = zMakeInt2(2, 2);
      const int v2 = 2;
      buffer10[0] = v0 << v1;
      buffer10[1] = v0 << v2;
//      buffer10[2] = v2 << v0;
      int2 v3 = v0;
      v3 <<= v1;
      buffer10[3] = v3;
      v3 = v0;
      v3 <<= v2;
      buffer10[4] = v3;
    }
    // Left shift
    {
      const int2 v0 = zMakeInt2(8, 8);
      const int2 v1 = zMakeInt2(1, 1);
      const int v2 = 1;
      buffer11[0] = v0 >> v1;
      buffer11[1] = v0 >> v2;
//      buffer11[2] = v2 >> v0;
      int2 v3 = v0;
      v3 >>= v1;
      buffer11[3] = v3;
      v3 = v0;
      v3 >>= v2;
      buffer11[4] = v3;
    }
    // Boolean AND
    {
      const int3 v0 = zMakeInt3(0, 0, 1);
      const int3 v1 = zMakeInt3(1, 0, 1);
      const int v2 = 1;
      buffer12[0] = v0 && v1;
      buffer12[1] = v0 && v2;
      buffer12[2] = v2 && v0;
    }
    // Boolean OR 
    {
      const int3 v0 = zMakeInt3(0, 0, 1);
      const int3 v1 = zMakeInt3(1, 0, 1);
      const int v2 = 1;
      buffer13[0] = v0 || v1;
      buffer13[1] = v0 || v2;
      buffer13[2] = v2 || v0;
    }
    // Equal operator and boolean opeartor
    {
      const float3 v0 = zMakeFloat3(0.0f, 0.0f, 0.0f);
      const float3 v1 = zMakeFloat3(0.0f, 1.0f, 0.0f);
      const float v2 = 0.0f;
      const float v3 = 1.0f;
//      buffer14[0] = v0 == v0;
//      buffer14[1] = v0 != v0;
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
      const float3 v0 = zMakeFloat3(1.0f, 2.0f, 3.0f);
      const float3 v1 = zMakeFloat3(3.0f, 2.0f, 1.0f);
      const float v2 = 2.0f;
//      buffer15[0] = v0 <= v0;
//      buffer15[1] = v0 < v0;
//      buffer15[2] = v0 >= v0;
//      buffer15[3] = v0 > v0;
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
  }
}

/*!
  */
kernel void testHalfLoadStore(
    global const half* input_scalar, global half* output_scalar,
    global const half* input_vector2, global half* output_vector2,
    global const half* input_vector3, global half* output_vector3,
    global const half* input_vector4, global half* output_vector4)
{
  const uint32b index = zGetGlobalIdX();
  if (index == 0) {
    // scalar
    {
      const float v0 = vload_half(0, input_scalar);
      const float v1 = vload_half(1, input_scalar);
      const float v2 = vload_half(2, input_scalar);
      vstore_half(2.0f * v0, 0, output_scalar);
      vstore_half(2.0f * v1, 1, output_scalar);
      vstore_half(2.0f * v2, 2, output_scalar);
    }
    // vector2
    {
//      const float2 v0 = vload_half2(0, input_vector2);
//      const float2 v1 = vload_half2(1, input_vector2);
//      vstore_half2(2.0f * v0, 0, output_vector2);
//      vstore_half2(2.0f * v1, 1, output_vector2);
      (void)input_vector2;
      (void)output_vector2;
    }
    // vector3
    {
//      const float3 v0 = vload_half3(0, input_vector3);
//      const float3 v1 = vload_half3(1, input_vector3);
//      vstore_half3(2.0f * v0, 0, output_vector3);
//      vstore_half3(2.0f * v1, 1, output_vector3);
      (void)input_vector3;
      (void)output_vector3;
    }
    // vector4
    {
      const float4 v0 = vload_half4(0, input_vector4);
      const float4 v1 = vload_half4(1, input_vector4);
      vstore_half4(2.0f * v0, 0, output_vector4);
      vstore_half4(2.0f * v1, 1, output_vector4);
    }
  }
}

#endif /* ZINVUL_DATA_TEST_VECTOR_CL */
