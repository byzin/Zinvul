/*!
  \file matrix.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATRIX_TEST_MATRIX_CL
#define ZINVUL_MATRIX_TEST_MATRIX_CL

#include "zinvul/cl/types.cl"
#include "zinvul/cl/matrix.cl"
#include "zinvul/cl/utility.cl"

//__kernel void testMatrix2x2Init(__global ZMatrix2x2* inputs,
//    __global ZMatrix2x2* outputs)
//{
//  const uint32b index = zGetGlobalIdX();
//  __local ZMatrix2x2 mat[2];
//  if (index == 0) {
//    const size_t n = 2;
//    // Setter, getter
//    // Mat1
//    {
//      ZMatrix2x2 m;
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          float v = zGetMat2x2ElemG(inputs, row, column);
//          v = 2.0f * v;
//          zSetMat2x2Elem(&m, row, column, v);
//        }
//      }
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          const float v = zGetMat2x2Elem(&m, row, column);
//          zSetMat2x2ElemG(&outputs[0], row, column, v);
//        }
//      }
//    }
//    // Mat2
//    {
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          float v = zGetMat2x2ElemG(inputs, row, column);
//          v = 2.0f * v;
//          zSetMat2x2ElemL(&mat[0], row, column, v);
//        }
//      }
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          const float v = zGetMat2x2ElemL(&mat[0], row, column);
//          zSetMat2x2ElemG(&outputs[1], row, column, v);
//        }
//      }
//    }
//
//    // Init, vector
//    // Mat3
//    {
//      ZMatrix2x2 m;
//      zInitMat2x2(&m, 0.0f, 1.0f,
//                      2.0f, 3.0f);
//      for (size_t row = 0; row < n; ++row) {
//        const float2 vector = zGetMat2x2RowVec(&m, row);
//        zSetMat2x2RowVecG(&outputs[2], row, vector);
//      }
//      for (size_t column = 0; column < n; ++column) {
//        const float2 vector = zGetMat2x2ColumnVec(&m, column);
//        zSetMat2x2ColumnVecG(&outputs[3], column, vector);
//      }
//    }
//    // Mat4
//    {
//      zInitMat2x2L(&mat[1], 0.0f, 1.0f,
//                            2.0f, 3.0f);
//      for (size_t row = 0; row < n; ++row) {
//        const float2 vector = zGetMat2x2RowVecL(&mat[1], row);
//        zSetMat2x2RowVecG(&outputs[4], row, vector);
//      }
//      for (size_t column = 0; column < n; ++column) {
//        const float2 vector = zGetMat2x2ColumnVecL(&mat[1], column);
//        zSetMat2x2ColumnVecG(&outputs[5], column, vector);
//      }
//    }
//  }
//}
//
//__kernel void testMatrix3x3Init(__global ZMatrix3x3* inputs,
//    __global ZMatrix3x3* outputs)
//{
//  const uint32b index = zGetGlobalIdX();
//  __local ZMatrix3x3 mat[2];
//  if (index == 0) {
//    const size_t n = 3;
//    // Setter, getter
//    // Mat1
//    {
//      ZMatrix3x3 m = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f);
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          float v = zGetMat3x3ElemG(inputs, row, column);
//          v = 2.0f * v;
//          zSetMat3x3Elem(&m, row, column, v);
//        }
//      }
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          const float v = zGetMat3x3Elem(&m, row, column);
//          zSetMat3x3ElemG(&outputs[0], row, column, v);
//        }
//      }
//    }
//    // Mat2
//    {
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          float v = zGetMat3x3ElemG(inputs, row, column);
//          v = 2.0f * v;
//          zSetMat3x3ElemL(&mat[0], row, column, v);
//        }
//      }
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          const float v = zGetMat3x3ElemL(&mat[0], row, column);
//          zSetMat3x3ElemG(&outputs[1], row, column, v);
//        }
//      }
//    }
//
//    // Init, vector
//    // Mat3
//    {
//      ZMatrix3x3 m = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f);
//      zInitMat3x3(&m, 0.0f, 1.0f, 2.0f,
//                      3.0f, 4.0f, 5.0f,
//                      6.0f, 7.0f, 8.0f);
//      for (size_t row = 0; row < n; ++row) {
//        const float3 vector = zGetMat3x3RowVec(&m, row);
//        zSetMat3x3RowVecG(&outputs[2], row, vector);
//      }
//      for (size_t column = 0; column < n; ++column) {
//        const float3 vector = zGetMat3x3ColumnVec(&m, column);
//        zSetMat3x3ColumnVecG(&outputs[3], column, vector);
//      }
//    }
//    // Mat4
//    {
//      zInitMat3x3L(&mat[1], 0.0f, 1.0f, 2.0f,
//                            3.0f, 4.0f, 5.0f,
//                            6.0f, 7.0f, 8.0f);
//      for (size_t row = 0; row < n; ++row) {
//        const float3 vector = zGetMat3x3RowVecL(&mat[1], row);
//        zSetMat3x3RowVecG(&outputs[4], row, vector);
//      }
//      for (size_t column = 0; column < n; ++column) {
//        const float3 vector = zGetMat3x3ColumnVecL(&mat[1], column);
//        zSetMat3x3ColumnVecG(&outputs[5], column, vector);
//      }
//    }
//  }
//}
//
//__kernel void testMatrix4x4Init(__global ZMatrix4x4* inputs,
//    __global ZMatrix4x4* outputs)
//{
//  const uint32b index = zGetGlobalIdX();
//  __local ZMatrix4x4 mat[2];
//  if (index == 0) {
//    const size_t n = 4;
//    // Setter, getter
//    // Mat1
//    {
//      ZMatrix4x4 m = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f, 0.0f);
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          float v = zGetMat4x4ElemG(inputs, row, column);
//          v = 2.0f * v;
//          zSetMat4x4Elem(&m, row, column, v);
//        }
//      }
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          const float v = zGetMat4x4Elem(&m, row, column);
//          zSetMat4x4ElemG(&outputs[0], row, column, v);
//        }
//      }
//    }
//    // Mat2
//    {
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          float v = zGetMat4x4ElemG(inputs, row, column);
//          v = 2.0f * v;
//          zSetMat4x4ElemL(&mat[0], row, column, v);
//        }
//      }
//      for (size_t row = 0; row < n; ++row) {
//        for (size_t column = 0; column < n; ++column) {
//          const float v = zGetMat4x4ElemL(&mat[0], row, column);
//          zSetMat4x4ElemG(&outputs[1], row, column, v);
//        }
//      }
//    }
//
//    // Init, vector
//    // Mat3
//    {
//      ZMatrix4x4 m = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f, 0.0f,
//                                 0.0f, 0.0f, 0.0f, 0.0f);
//      zInitMat4x4(&m, 0.0f, 1.0f, 2.0f, 3.0f,
//                      4.0f, 5.0f, 6.0f, 7.0f,
//                      8.0f, 9.0f, 10.0f, 11.0f,
//                      12.0f, 13.0f, 14.0f, 15.0f);
//      for (size_t row = 0; row < n; ++row) {
//        const float4 vector = zGetMat4x4RowVec(&m, row);
//        zSetMat4x4RowVecG(&outputs[2], row, vector);
//      }
//      for (size_t column = 0; column < n; ++column) {
//        const float4 vector = zGetMat4x4ColumnVec(&m, column);
//        zSetMat4x4ColumnVecG(&outputs[3], column, vector);
//      }
//    }
//    // Mat4
//    {
//      zInitMat4x4L(&mat[1], 0.0f, 1.0f, 2.0f, 3.0f,
//                            4.0f, 5.0f, 6.0f, 7.0f,
//                            8.0f, 9.0f, 10.0f, 11.0f,
//                            12.0f, 13.0f, 14.0f, 15.0f);
//      for (size_t row = 0; row < n; ++row) {
//        const float4 vector = zGetMat4x4RowVecL(&mat[1], row);
//        zSetMat4x4RowVecG(&outputs[4], row, vector);
//      }
//      for (size_t column = 0; column < n; ++column) {
//        const float4 vector = zGetMat4x4ColumnVecL(&mat[1], column);
//        zSetMat4x4ColumnVecG(&outputs[5], column, vector);
//      }
//    }
//  }
//}
//
//__kernel void testMatrix2x2Addition(__global ZMatrix2x2* inputs,
//    __global ZMatrix2x2* outputs)
//{
//  __local ZMatrix2x2 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zAddMat2x2(&lhs, &rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      zAddMat2x2L(&lhs, &rhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      zAddMat2x2G(&lhs, &rhs, &outputs[i++]);
//    }
//
//    {
//      zCopyMat2x2GL(&inputs[0], &mat[0]);
//      zCopyMat2x2GL(&inputs[1], &mat[1]);
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zAddLMat2x2(&mat[0], &mat[1], &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      zCopyMat2x2GL(&inputs[0], &mat[0]);
//      zCopyMat2x2GL(&inputs[1], &mat[1]);
//      zAddLMat2x2L(&mat[0], &mat[1], &mat[2]);
//      zCopyMat2x2LG(&mat[2], &outputs[i++]);
//    }
//    {
//      zCopyMat2x2GL(&inputs[0], &mat[0]);
//      zCopyMat2x2GL(&inputs[1], &mat[1]);
//      zAddLMat2x2G(&mat[0], &mat[1], &outputs[i++]);
//    }
//
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zAddGMat2x2(lhs, rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zAddGMat2x2L(lhs, rhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zAddGMat2x2G(lhs, rhs, &outputs[i++]);
//    }
//
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const __local ZMatrix2x2* rhs = &mat[1];
//      zAddPLMat2x2G(&lhs, rhs, &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zAddPGMat2x2G(&lhs, rhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix3x3Addition(__global ZMatrix3x3* inputs,
//    __global ZMatrix3x3* outputs)
//{
//  __local ZMatrix3x3 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zAddMat3x3(&lhs, &rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      zAddMat3x3L(&lhs, &rhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      zAddMat3x3G(&lhs, &rhs, &outputs[i++]);
//    }
//
//    {
//      zCopyMat3x3GL(&inputs[0], &mat[0]);
//      zCopyMat3x3GL(&inputs[1], &mat[1]);
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zAddLMat3x3(&mat[0], &mat[1], &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      zCopyMat3x3GL(&inputs[0], &mat[0]);
//      zCopyMat3x3GL(&inputs[1], &mat[1]);
//      zAddLMat3x3L(&mat[0], &mat[1], &mat[2]);
//      zCopyMat3x3LG(&mat[2], &outputs[i++]);
//    }
//    {
//      zCopyMat3x3GL(&inputs[0], &mat[0]);
//      zCopyMat3x3GL(&inputs[1], &mat[1]);
//      zAddLMat3x3G(&mat[0], &mat[1], &outputs[i++]);
//    }
//
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zAddGMat3x3(lhs, rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zAddGMat3x3L(lhs, rhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zAddGMat3x3G(lhs, rhs, &outputs[i++]);
//    }
//
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const __local ZMatrix3x3* rhs = &mat[1];
//      zAddPLMat3x3G(&lhs, rhs, &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zAddPGMat3x3G(&lhs, rhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix4x4Addition(__global ZMatrix4x4* inputs,
//    __global ZMatrix4x4* outputs)
//{
//  __local ZMatrix4x4 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zAddMat4x4(&lhs, &rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      zAddMat4x4L(&lhs, &rhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      zAddMat4x4G(&lhs, &rhs, &outputs[i++]);
//    }
//
//    {
//      zCopyMat4x4GL(&inputs[0], &mat[0]);
//      zCopyMat4x4GL(&inputs[1], &mat[1]);
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zAddLMat4x4(&mat[0], &mat[1], &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      zCopyMat4x4GL(&inputs[0], &mat[0]);
//      zCopyMat4x4GL(&inputs[1], &mat[1]);
//      zAddLMat4x4L(&mat[0], &mat[1], &mat[2]);
//      zCopyMat4x4LG(&mat[2], &outputs[i++]);
//    }
//    {
//      zCopyMat4x4GL(&inputs[0], &mat[0]);
//      zCopyMat4x4GL(&inputs[1], &mat[1]);
//      zAddLMat4x4G(&mat[0], &mat[1], &outputs[i++]);
//    }
//
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zAddGMat4x4(lhs, rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zAddGMat4x4L(lhs, rhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zAddGMat4x4G(lhs, rhs, &outputs[i++]);
//    }
//
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const __local ZMatrix4x4* rhs = &mat[1];
//      zAddPLMat4x4G(&lhs, rhs, &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zAddPGMat4x4G(&lhs, rhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix2x2Subtraction(__global ZMatrix2x2* inputs,
//    __global ZMatrix2x2* outputs)
//{
//  __local ZMatrix2x2 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zSubMat2x2(&lhs, &rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      zSubMat2x2L(&lhs, &rhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      zSubMat2x2G(&lhs, &rhs, &outputs[i++]);
//    }
//
//    {
//      zCopyMat2x2GL(&inputs[0], &mat[0]);
//      zCopyMat2x2GL(&inputs[1], &mat[1]);
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zSubLMat2x2(&mat[0], &mat[1], &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      zCopyMat2x2GL(&inputs[0], &mat[0]);
//      zCopyMat2x2GL(&inputs[1], &mat[1]);
//      zSubLMat2x2L(&mat[0], &mat[1], &mat[2]);
//      zCopyMat2x2LG(&mat[2], &outputs[i++]);
//    }
//    {
//      zCopyMat2x2GL(&inputs[0], &mat[0]);
//      zCopyMat2x2GL(&inputs[1], &mat[1]);
//      zSubLMat2x2G(&mat[0], &mat[1], &outputs[i++]);
//    }
//
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zSubGMat2x2(lhs, rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zSubGMat2x2L(lhs, rhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zSubGMat2x2G(lhs, rhs, &outputs[i++]);
//    }
//
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const __local ZMatrix2x2* rhs = &mat[1];
//      zSubPLMat2x2G(&lhs, rhs, &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zSubPGMat2x2G(&lhs, rhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix3x3Subtraction(__global ZMatrix3x3* inputs,
//    __global ZMatrix3x3* outputs)
//{
//  __local ZMatrix3x3 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zSubMat3x3(&lhs, &rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      zSubMat3x3L(&lhs, &rhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      zSubMat3x3G(&lhs, &rhs, &outputs[i++]);
//    }
//
//    {
//      zCopyMat3x3GL(&inputs[0], &mat[0]);
//      zCopyMat3x3GL(&inputs[1], &mat[1]);
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zSubLMat3x3(&mat[0], &mat[1], &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      zCopyMat3x3GL(&inputs[0], &mat[0]);
//      zCopyMat3x3GL(&inputs[1], &mat[1]);
//      zSubLMat3x3L(&mat[0], &mat[1], &mat[2]);
//      zCopyMat3x3LG(&mat[2], &outputs[i++]);
//    }
//    {
//      zCopyMat3x3GL(&inputs[0], &mat[0]);
//      zCopyMat3x3GL(&inputs[1], &mat[1]);
//      zSubLMat3x3G(&mat[0], &mat[1], &outputs[i++]);
//    }
//
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zSubGMat3x3(lhs, rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zSubGMat3x3L(lhs, rhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zSubGMat3x3G(lhs, rhs, &outputs[i++]);
//    }
//
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const __local ZMatrix3x3* rhs = &mat[1];
//      zSubPLMat3x3G(&lhs, rhs, &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zSubPGMat3x3G(&lhs, rhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix4x4Subtraction(__global ZMatrix4x4* inputs,
//    __global ZMatrix4x4* outputs)
//{
//  __local ZMatrix4x4 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zSubMat4x4(&lhs, &rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      zSubMat4x4L(&lhs, &rhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      zSubMat4x4G(&lhs, &rhs, &outputs[i++]);
//    }
//
//    {
//      zCopyMat4x4GL(&inputs[0], &mat[0]);
//      zCopyMat4x4GL(&inputs[1], &mat[1]);
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zSubLMat4x4(&mat[0], &mat[1], &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      zCopyMat4x4GL(&inputs[0], &mat[0]);
//      zCopyMat4x4GL(&inputs[1], &mat[1]);
//      zSubLMat4x4L(&mat[0], &mat[1], &mat[2]);
//      zCopyMat4x4LG(&mat[2], &outputs[i++]);
//    }
//    {
//      zCopyMat4x4GL(&inputs[0], &mat[0]);
//      zCopyMat4x4GL(&inputs[1], &mat[1]);
//      zSubLMat4x4G(&mat[0], &mat[1], &outputs[i++]);
//    }
//
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zSubGMat4x4(lhs, rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zSubGMat4x4L(lhs, rhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zSubGMat4x4G(lhs, rhs, &outputs[i++]);
//    }
//
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const __local ZMatrix4x4* rhs = &mat[1];
//      zSubPLMat4x4G(&lhs, rhs, &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zSubPGMat4x4G(&lhs, rhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix2x2Multiplication(const __global ZMatrix2x2* inputs,
//    __global ZMatrix2x2* outputs)
//{
//  __local ZMatrix2x2 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    //
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zMulMat2x2(&lhs, &rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      zMulMat2x2L(&lhs, &rhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const ZMatrix2x2 rhs = zLoadMat2x2G(&inputs[1]);
//      zMulMat2x2G(&lhs, &rhs, &outputs[i++]);
//    }
//    //
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      __local ZMatrix2x2* rhs = &mat[0];
//      zCopyMat2x2GL(&inputs[1], rhs);
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zMulPLMat2x2(&lhs, rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//      zMulLPMat2x2(rhs, &lhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      __local ZMatrix2x2* rhs = &mat[0];
//      zCopyMat2x2GL(&inputs[1], rhs);
//      zMulPLMat2x2L(&lhs, rhs, &mat[1]);
//      zCopyMat2x2LG(&mat[1], &outputs[i++]);
//      zMulLPMat2x2L(rhs, &lhs, &mat[1]);
//      zCopyMat2x2LG(&mat[1], &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      __local ZMatrix2x2* rhs = &mat[0];
//      zCopyMat2x2GL(&inputs[1], rhs);
//      zMulPLMat2x2G(&lhs, rhs, &outputs[i++]);
//      zMulLPMat2x2G(rhs, &lhs, &outputs[i++]);
//    }
//    //
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zMulPGMat2x2(&lhs, rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//      zMulGPMat2x2(rhs, &lhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zMulPGMat2x2L(&lhs, rhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//      zMulGPMat2x2L(rhs, &lhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zMulPGMat2x2G(&lhs, rhs, &outputs[i++]);
//      zMulGPMat2x2G(rhs, &lhs, &outputs[i++]);
//    }
//    //
//    {
//      __local ZMatrix2x2* lhs = &mat[0];
//      zCopyMat2x2GL(&inputs[0], lhs);
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zMulLGMat2x2(lhs, rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//      zMulGLMat2x2(rhs, lhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      __local ZMatrix2x2* lhs = &mat[0];
//      zCopyMat2x2GL(&inputs[0], lhs);
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zMulLGMat2x2L(lhs, rhs, &mat[1]);
//      zCopyMat2x2LG(&mat[1], &outputs[i++]);
//      zMulGLMat2x2L(rhs, lhs, &mat[1]);
//      zCopyMat2x2LG(&mat[1], &outputs[i++]);
//    }
//    {
//      __local ZMatrix2x2* lhs = &mat[0];
//      zCopyMat2x2GL(&inputs[0], lhs);
//      const __global ZMatrix2x2* rhs = &inputs[1];
//      zMulLGMat2x2G(lhs, rhs, &outputs[i++]);
//      zMulGLMat2x2G(rhs, lhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix2x2ScalarMultiplication(const __global ZMatrix2x2* inputs,
//    __global ZMatrix2x2* outputs)
//{
//  __local ZMatrix2x2 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    //
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const float rhs = 3.0f;
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zMulMat2x2S(&lhs, rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const float rhs = 3.0f;
//      zMulMat2x2SL(&lhs, rhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix2x2 lhs = zLoadMat2x2G(&inputs[0]);
//      const float rhs = 3.0f;
//      zMulMat2x2SG(&lhs, rhs, &outputs[i++]);
//    }
//    //
//    {
//      __local ZMatrix2x2* lhs = &mat[0];
//      zCopyMat2x2GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zMulLMat2x2S(lhs, rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      __local ZMatrix2x2* lhs = &mat[0];
//      zCopyMat2x2GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      zMulLMat2x2SL(lhs, rhs, &mat[1]);
//      zCopyMat2x2LG(&mat[1], &outputs[i++]);
//    }
//    {
//      __local ZMatrix2x2* lhs = &mat[0];
//      zCopyMat2x2GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      zMulLMat2x2SG(lhs, rhs, &outputs[i++]);
//    }
//    //
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      ZMatrix2x2 result = zMakeMat2x2(0.0f, 0.0f,
//                                      0.0f, 0.0f);
//      zMulGMat2x2S(lhs, rhs, &result);
//      zCopyMat2x2PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      zMulGMat2x2SL(lhs, rhs, &mat[0]);
//      zCopyMat2x2LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix2x2* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      zMulGMat2x2SG(lhs, rhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix3x3Multiplication(const __global ZMatrix3x3* inputs,
//    __global ZMatrix3x3* outputs)
//{
//  __local ZMatrix3x3 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    //
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zMulMat3x3(&lhs, &rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      zMulMat3x3L(&lhs, &rhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const ZMatrix3x3 rhs = zLoadMat3x3G(&inputs[1]);
//      zMulMat3x3G(&lhs, &rhs, &outputs[i++]);
//    }
//    //
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      __local ZMatrix3x3* rhs = &mat[0];
//      zCopyMat3x3GL(&inputs[1], rhs);
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zMulPLMat3x3(&lhs, rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//      zMulLPMat3x3(rhs, &lhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      __local ZMatrix3x3* rhs = &mat[0];
//      zCopyMat3x3GL(&inputs[1], rhs);
//      zMulPLMat3x3L(&lhs, rhs, &mat[1]);
//      zCopyMat3x3LG(&mat[1], &outputs[i++]);
//      zMulLPMat3x3L(rhs, &lhs, &mat[1]);
//      zCopyMat3x3LG(&mat[1], &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      __local ZMatrix3x3* rhs = &mat[0];
//      zCopyMat3x3GL(&inputs[1], rhs);
//      zMulPLMat3x3G(&lhs, rhs, &outputs[i++]);
//      zMulLPMat3x3G(rhs, &lhs, &outputs[i++]);
//    }
//    //
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zMulPGMat3x3(&lhs, rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//      zMulGPMat3x3(rhs, &lhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zMulPGMat3x3L(&lhs, rhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//      zMulGPMat3x3L(rhs, &lhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zMulPGMat3x3G(&lhs, rhs, &outputs[i++]);
//      zMulGPMat3x3G(rhs, &lhs, &outputs[i++]);
//    }
//    //
//    {
//      __local ZMatrix3x3* lhs = &mat[0];
//      zCopyMat3x3GL(&inputs[0], lhs);
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zMulLGMat3x3(lhs, rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//      zMulGLMat3x3(rhs, lhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      __local ZMatrix3x3* lhs = &mat[0];
//      zCopyMat3x3GL(&inputs[0], lhs);
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zMulLGMat3x3L(lhs, rhs, &mat[1]);
//      zCopyMat3x3LG(&mat[1], &outputs[i++]);
//      zMulGLMat3x3L(rhs, lhs, &mat[1]);
//      zCopyMat3x3LG(&mat[1], &outputs[i++]);
//    }
//    {
//      __local ZMatrix3x3* lhs = &mat[0];
//      zCopyMat3x3GL(&inputs[0], lhs);
//      const __global ZMatrix3x3* rhs = &inputs[1];
//      zMulLGMat3x3G(lhs, rhs, &outputs[i++]);
//      zMulGLMat3x3G(rhs, lhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix3x3ScalarMultiplication(const __global ZMatrix3x3* inputs,
//    __global ZMatrix3x3* outputs)
//{
//  __local ZMatrix3x3 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    //
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const float rhs = 3.0f;
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zMulMat3x3S(&lhs, rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const float rhs = 3.0f;
//      zMulMat3x3SL(&lhs, rhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix3x3 lhs = zLoadMat3x3G(&inputs[0]);
//      const float rhs = 3.0f;
//      zMulMat3x3SG(&lhs, rhs, &outputs[i++]);
//    }
//    //
//    {
//      __local ZMatrix3x3* lhs = &mat[0];
//      zCopyMat3x3GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zMulLMat3x3S(lhs, rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      __local ZMatrix3x3* lhs = &mat[0];
//      zCopyMat3x3GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      zMulLMat3x3SL(lhs, rhs, &mat[1]);
//      zCopyMat3x3LG(&mat[1], &outputs[i++]);
//    }
//    {
//      __local ZMatrix3x3* lhs = &mat[0];
//      zCopyMat3x3GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      zMulLMat3x3SG(lhs, rhs, &outputs[i++]);
//    }
//    //
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      ZMatrix3x3 result = zMakeMat3x3(0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f);
//      zMulGMat3x3S(lhs, rhs, &result);
//      zCopyMat3x3PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      zMulGMat3x3SL(lhs, rhs, &mat[0]);
//      zCopyMat3x3LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix3x3* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      zMulGMat3x3SG(lhs, rhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix4x4Multiplication(const __global ZMatrix4x4* inputs,
//    __global ZMatrix4x4* outputs)
//{
//  __local ZMatrix4x4 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    //
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zMulMat4x4(&lhs, &rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      zMulMat4x4L(&lhs, &rhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const ZMatrix4x4 rhs = zLoadMat4x4G(&inputs[1]);
//      zMulMat4x4G(&lhs, &rhs, &outputs[i++]);
//    }
//    //
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      __local ZMatrix4x4* rhs = &mat[0];
//      zCopyMat4x4GL(&inputs[1], rhs);
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zMulPLMat4x4(&lhs, rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//      zMulLPMat4x4(rhs, &lhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      __local ZMatrix4x4* rhs = &mat[0];
//      zCopyMat4x4GL(&inputs[1], rhs);
//      zMulPLMat4x4L(&lhs, rhs, &mat[1]);
//      zCopyMat4x4LG(&mat[1], &outputs[i++]);
//      zMulLPMat4x4L(rhs, &lhs, &mat[1]);
//      zCopyMat4x4LG(&mat[1], &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      __local ZMatrix4x4* rhs = &mat[0];
//      zCopyMat4x4GL(&inputs[1], rhs);
//      zMulPLMat4x4G(&lhs, rhs, &outputs[i++]);
//      zMulLPMat4x4G(rhs, &lhs, &outputs[i++]);
//    }
//    //
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zMulPGMat4x4(&lhs, rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//      zMulGPMat4x4(rhs, &lhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zMulPGMat4x4L(&lhs, rhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//      zMulGPMat4x4L(rhs, &lhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zMulPGMat4x4G(&lhs, rhs, &outputs[i++]);
//      zMulGPMat4x4G(rhs, &lhs, &outputs[i++]);
//    }
//    //
//    {
//      __local ZMatrix4x4* lhs = &mat[0];
//      zCopyMat4x4GL(&inputs[0], lhs);
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zMulLGMat4x4(lhs, rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//      zMulGLMat4x4(rhs, lhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      __local ZMatrix4x4* lhs = &mat[0];
//      zCopyMat4x4GL(&inputs[0], lhs);
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zMulLGMat4x4L(lhs, rhs, &mat[1]);
//      zCopyMat4x4LG(&mat[1], &outputs[i++]);
//      zMulGLMat4x4L(rhs, lhs, &mat[1]);
//      zCopyMat4x4LG(&mat[1], &outputs[i++]);
//    }
//    {
//      __local ZMatrix4x4* lhs = &mat[0];
//      zCopyMat4x4GL(&inputs[0], lhs);
//      const __global ZMatrix4x4* rhs = &inputs[1];
//      zMulLGMat4x4G(lhs, rhs, &outputs[i++]);
//      zMulGLMat4x4G(rhs, lhs, &outputs[i++]);
//    }
//  }
//}
//
//__kernel void testMatrix4x4ScalarMultiplication(const __global ZMatrix4x4* inputs,
//    __global ZMatrix4x4* outputs)
//{
//  __local ZMatrix4x4 mat[3];
//  const uint32b index = zGetGlobalIdX();
//  if (index == 0) {
//    size_t i = 0;
//    //
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const float rhs = 3.0f;
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zMulMat4x4S(&lhs, rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const float rhs = 3.0f;
//      zMulMat4x4SL(&lhs, rhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const ZMatrix4x4 lhs = zLoadMat4x4G(&inputs[0]);
//      const float rhs = 3.0f;
//      zMulMat4x4SG(&lhs, rhs, &outputs[i++]);
//    }
//    //
//    {
//      __local ZMatrix4x4* lhs = &mat[0];
//      zCopyMat4x4GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zMulLMat4x4S(lhs, rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      __local ZMatrix4x4* lhs = &mat[0];
//      zCopyMat4x4GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      zMulLMat4x4SL(lhs, rhs, &mat[1]);
//      zCopyMat4x4LG(&mat[1], &outputs[i++]);
//    }
//    {
//      __local ZMatrix4x4* lhs = &mat[0];
//      zCopyMat4x4GL(&inputs[0], lhs);
//      const float rhs = 3.0f;
//      zMulLMat4x4SG(lhs, rhs, &outputs[i++]);
//    }
//    //
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      ZMatrix4x4 result = zMakeMat4x4(0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f,
//                                      0.0f, 0.0f, 0.0f, 0.0f);
//      zMulGMat4x4S(lhs, rhs, &result);
//      zCopyMat4x4PG(&result, &outputs[i++]);
//    }
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      zMulGMat4x4SL(lhs, rhs, &mat[0]);
//      zCopyMat4x4LG(&mat[0], &outputs[i++]);
//    }
//    {
//      const __global ZMatrix4x4* lhs = &inputs[0];
//      const float rhs = 3.0f;
//      zMulGMat4x4SG(lhs, rhs, &outputs[i++]);
//    }
//  }
//}

#endif /* ZINVUL_MATRIX_TEST_MATRIX_CL */
