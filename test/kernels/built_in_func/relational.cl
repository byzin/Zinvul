///*!
//  \file relational.cl
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2018 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL
//#define ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL
//
//// Zinvul
//#include "zinvul/cl/relational.cl"
//#include "zinvul/cl/types.cl"
//#include "zinvul/cl/utility.cl"
//
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
///*!
//  */
//kernel void testSelect(global int32b* scalar_results,
//                       global float4* vector_results)
//{
//  const uint index = zGetGlobalIdX();
//  if (index == 0) {
//    // Scalar
//    uint scalar_index = 0;
//    {
//      int32b x = 2;
//      scalar_results[scalar_index++] = select(x, -x, x < 0);
//    }
//    {
//      int32b x = -2;
//      scalar_results[scalar_index++] = select(-x, x, x < 0);
//    }
//    // Vector
//    uint vector_index = 0;
//    {
//      {
//        const float4 v0 = zMakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
//        const int4 v1 = zMakeInt4(1, -1, 1, -1);
//        vector_results[vector_index++] = select(v0, -v0, v1 < 0);
//      }
//    }
//  }
//}
//
///*!
//  */
//kernel void testZSelect(global int32b* scalar_results,
//                        global float4* vector_results)
//{
//  const uint index = zGetGlobalIdX();
//  if (index == 0) {
//    // Scalar
//    uint scalar_index = 0;
//    {
//      int32b x = 2;
//      scalar_results[scalar_index++] = zSelect(x, -x, x < 0);
//    }
//    {
//      int32b x = -2;
//      scalar_results[scalar_index++] = zSelect(-x, x, x < 0);
//    }
//    // Vector
//    uint vector_index = 0;
//    {
//      {
//        const float4 v0 = zMakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
//        const int4 v1 = zMakeInt4(1, -1, 1, -1);
//        vector_results[vector_index++] = zSelectF4(v0, -v0, v1 < 0);
//      }
//    }
//  }
//}
//
//#endif /* ZINVUL_BUILT_IN_FUNC_TEST_RELATIONAL_CL */
