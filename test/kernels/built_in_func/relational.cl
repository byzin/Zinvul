/*!
  \file relational.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL
#define ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL

// Zinvul
#include "zinvul/cl/math.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

///*!
//  */
//kernel void testRelational(global int32b* scalar_results,
//                           global int4* vector_results)
//{
//  const uint index = zGetGlobalIdX();
//  if (index == 0) {
//    // Scalar
//    uint scalar_index = 0;
//    scalar_results[scalar_index++] = isequal(1.0f, 1.0f);
//    scalar_results[scalar_index++] = isequal(1.0f, 2.0f);
//    scalar_results[scalar_index++] = isnotequal(1.0f, 1.0f);
//    scalar_results[scalar_index++] = isnotequal(1.0f, 2.0f);
//    scalar_results[scalar_index++] = isgreater(1.0f, 0.0f);
//    scalar_results[scalar_index++] = isgreater(1.0f, 1.0f);
//    scalar_results[scalar_index++] = isgreater(1.0f, 2.0f);
//    scalar_results[scalar_index++] = isgreaterequal(1.0f, 0.0f);
//    scalar_results[scalar_index++] = isgreaterequal(1.0f, 1.0f);
//    scalar_results[scalar_index++] = isgreaterequal(1.0f, 2.0f);
//    scalar_results[scalar_index++] = isless(1.0f, 0.0f);
//    scalar_results[scalar_index++] = isless(1.0f, 1.0f);
//    scalar_results[scalar_index++] = isless(1.0f, 2.0f);
//    scalar_results[scalar_index++] = islessequal(1.0f, 0.0f);
//    scalar_results[scalar_index++] = islessequal(1.0f, 1.0f);
//    scalar_results[scalar_index++] = islessequal(1.0f, 2.0f);
//    scalar_results[scalar_index++] = isinf(1.0f);
//    scalar_results[scalar_index++] = isinf(INFINITY);
//    scalar_results[scalar_index++] = isnan(1.0f);
//    scalar_results[scalar_index++] = isnan(NAN);
//    scalar_results[scalar_index++] = signbit(1.0f);
//    scalar_results[scalar_index++] = signbit(0.0f);
//    scalar_results[scalar_index++] = signbit(-0.0f);
//    scalar_results[scalar_index++] = signbit(-1.0f);
//    // Vector
//    uint vector_index = 0;
//    {
//      const float4 v0 = zMakeFloat4(0.0f, 1.0f, 2.0f, 3.0f);
//      const float4 v1 = zMakeFloat4(0.0f, 1.0f, 2.0f, 3.0f);
//      const float4 v2 = zMakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
//#if defined(Z_DEBUG_MODE)
//      vector_results[vector_index++] = isequal(v0, v1);
//#else // Z_DEBUG_MODE
//      vector_results[vector_index++] = zMakeInt4(~0, ~0, ~0, ~0);
//#endif // Z_DEBUG_MODE
//      vector_results[vector_index++] = isequal(v0, v2);
//      vector_results[vector_index++] = isnotequal(v0, v1);
//      vector_results[vector_index++] = isnotequal(v0, v2);
//      vector_results[vector_index++] = isgreater(v0, v2);
//      vector_results[vector_index++] = isgreaterequal(v0, v2);
//      vector_results[vector_index++] = isless(v0, v2);
//      vector_results[vector_index++] = islessequal(v0, v2);
//    }
//    {
//      const float4 v0 = zMakeFloat4(MAXFLOAT, -INFINITY, INFINITY, NAN);
//      vector_results[vector_index++] = isinf(v0);
//      vector_results[vector_index++] = isnan(v0);
//    }
//    {
//      const float4 v0 = zMakeFloat4(1.0f, 0.0f, -0.0f, -1.0f);
//      vector_results[vector_index++] = signbit(v0);
//    }
//  }
//}
//
//#define ZINVUL_TEST_SELECT(select1, select2, select3, select4, results1, results2, results3, results4) \
//  const uint index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    uint table_index = 0; \
//    { \
//      const int32b x = 5; \
//      results1[table_index++] = select1(x, -x, 0 <= x); \
//    } \
//    { \
//      const int32b x = 5; \
//      results1[table_index++] = select1(x, -x, x < 0); \
//    } \
//    { \
//      const int32b x = -5; \
//      results1[table_index++] = select1(x, -x, 0 <= x); \
//    } \
//    { \
//      const int32b x = -5; \
//      results1[table_index++] = select1(x, -x, x < 0); \
//    } \
//    table_index = 0; \
//    { \
//      const int2 x = zMakeInt2(5, -2); \
//      results2[table_index++] = select2(x, -x, 0 <= x); \
//    } \
//    { \
//      const int2 x = zMakeInt2(5, -2); \
//      results2[table_index++] = select2(x, -x, x < 0); \
//    } \
//    table_index = 0; \
//    { \
//      const int3 x = zMakeInt3(5, -2, 9); \
//      results3[table_index++] = select3(x, -x, 0 <= x); \
//    } \
//    { \
//      const int3 x = zMakeInt3(5, -2, 9); \
//      results3[table_index++] = select3(x, -x, x < 0); \
//    } \
//    table_index = 0; \
//    { \
//      const int4 x = zMakeInt4(5, -2, 9, -15); \
//      results4[table_index++] = select4(x, -x, 0 <= x); \
//    } \
//    { \
//      const int4 x = zMakeInt4(5, -2, 9, -15); \
//      results4[table_index++] = select4(x, -x, x < 0); \
//    } \
//  } \
//
//kernel void testSelect(global int32b* results1, global int2* results2,
//    global int3* results3, global int4* results4)
//{
//  ZINVUL_TEST_SELECT(select, select, select, select, results1, results2, results3, results4);
//}
//
//kernel void testzSelectImpl(global int32b* results1, global int2* results2,
//    global int3* results3, global int4* results4)
//{
//  ZINVUL_TEST_SELECT(zSelectImpl, zSelect2Impl, zSelect3Impl, zSelect4Impl, results1, results2, results3, results4);
//}
//
//kernel void testzSelect(global int32b* results1, global int2* results2,
//    global int3* results3, global int4* results4)
//{
//  ZINVUL_TEST_SELECT(zSelect, zSelect2, zSelect3, zSelect4, results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_SELECTU(select1, select2, select3, select4, results1, results2, results3, results4) \
//  const uint index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    uint table_index = 0; \
//    { \
//      const uint32b x = 5; \
//      results1[table_index++] = select1(x, 2u * x, x == 5u); \
//    } \
//    { \
//      const uint32b x = 5; \
//      results1[table_index++] = select1(x, 2u * x, x != 5u); \
//    } \
//    table_index = 0; \
//    { \
//      const uint2 x = zMakeUInt2(5, 2); \
//      results2[table_index++] = select2(x, 2u * x, x == 5u); \
//    } \
//    { \
//      const uint2 x = zMakeUInt2(5, 2); \
//      results2[table_index++] = select2(x, 2u * x, x != 5u); \
//    } \
//    table_index = 0; \
//    { \
//      const uint3 x = zMakeUInt3(5, 2, 9); \
//      results3[table_index++] = select3(x, 2u * x, x == 5u); \
//    } \
//    { \
//      const uint3 x = zMakeUInt3(5, 2, 9); \
//      results3[table_index++] = select3(x, 2u * x, x != 5u); \
//    } \
//    table_index = 0; \
//    { \
//      const uint4 x = zMakeUInt4(5, 2, 9, 15); \
//      results4[table_index++] = select4(x, 2u * x, x == 5u); \
//    } \
//    { \
//      const uint4 x = zMakeUInt4(5, 2, 9, 15); \
//      results4[table_index++] = select4(x, 2u * x, x != 5u); \
//    } \
//  } \
//
//kernel void testSelectU(global uint32b* results1, global uint2* results2,
//    global uint3* results3, global uint4* results4)
//{
//  ZINVUL_TEST_SELECTU(select, select, select, select, results1, results2, results3, results4);
//}
//
//kernel void testzSelectUImpl(global uint32b* results1, global uint2* results2,
//    global uint3* results3, global uint4* results4)
//{
//  ZINVUL_TEST_SELECTU(zSelectUImpl, zSelectU2Impl, zSelectU3Impl, zSelectU4Impl, results1, results2, results3, results4);
//}
//
//kernel void testzSelectU(global uint32b* results1, global uint2* results2,
//    global uint3* results3, global uint4* results4)
//{
//  ZINVUL_TEST_SELECTU(zSelectU, zSelectU2, zSelectU3, zSelectU4, results1, results2, results3, results4);
//}
//
//#define ZINVUL_TEST_SELECTF(select1, select2, select3, select4, results1, results2, results3, results4) \
//  const uint index = zGetGlobalIdX(); \
//  if (index == 0) { \
//    uint table_index = 0; \
//    { \
//      const float x = 5.0f; \
//      results1[table_index++] = select1(x, -x, 0.0f <= x); \
//    } \
//    { \
//      const float x = 5.0f; \
//      results1[table_index++] = select1(x, -x, x < 0.0f); \
//    } \
//    { \
//      const float x = -5.0f; \
//      results1[table_index++] = select1(x, -x, 0.0f <= x); \
//    } \
//    { \
//      const float x = -5.0f; \
//      results1[table_index++] = select1(x, -x, x < 0.0f); \
//    } \
//    table_index = 0; \
//    { \
//      const float2 x = zMakeFloat2(5.0f, -2.0f); \
//      results2[table_index++] = select2(x, -x, 0.0f <= x); \
//    } \
//    { \
//      const float2 x = zMakeFloat2(5.0f, -2.0f); \
//      results2[table_index++] = select2(x, -x, x < 0.0f); \
//    } \
//    table_index = 0; \
//    { \
//      const float3 x = zMakeFloat3(5.0f, -2.0f, 9.0f); \
//      results3[table_index++] = select3(x, -x, 0.0f <= x); \
//    } \
//    { \
//      const float3 x = zMakeFloat3(5.0f, -2.0f, 9.0f); \
//      results3[table_index++] = select3(x, -x, x < 0.0f); \
//    } \
//    table_index = 0; \
//    { \
//      const float4 x = zMakeFloat4(5.0f, -2.0f, 9.0f, -15.0f); \
//      results4[table_index++] = select4(x, -x, 0.0f <= x); \
//    } \
//    { \
//      const float4 x = zMakeFloat4(5.0f, -2.0f, 9.0f, -15.0f); \
//      results4[table_index++] = select4(x, -x, x < 0.0f); \
//    } \
//  } \
//
//kernel void testSelectF(global float* results1, global float2* results2,
//    global float3* results3, global float4* results4)
//{
//  ZINVUL_TEST_SELECTF(select, select, select, select, results1, results2, results3, results4);
//}
//
//kernel void testzSelectFImpl(global float* results1, global float2* results2,
//    global float3* results3, global float4* results4)
//{
//  ZINVUL_TEST_SELECTF(zSelectFImpl, zSelectF2Impl, zSelectF3Impl, zSelectF4Impl, results1, results2, results3, results4);
//}
//
//kernel void testzSelectF(global float* results1, global float2* results2,
//    global float3* results3, global float4* results4)
//{
//  ZINVUL_TEST_SELECTF(zSelectF, zSelectF2, zSelectF3, zSelectF4, results1, results2, results3, results4);
//}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL */
