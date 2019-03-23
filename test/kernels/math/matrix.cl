/*!
  \file matrix.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATH_TEST_MATRIX_CL
#define ZINVUL_MATH_TEST_MATRIX_CL

#include "zinvul/cl/types.cl"
#include "zinvul/cl/math.cl"
#include "zinvul/cl/utility.cl"

__kernel void testMatrix2x2Init(__global ZMatrix2x2* inputs,
    __global ZMatrix2x2* outputs)
{
  const uint32b index = zGetGlobalIdX();
  __local ZMatrix2x2 mat[2];
  if (index == 0) {
    // Mat1
    for (size_t column = 0; column < 2; ++column) {
      for (size_t row = 0; row < 2; ++row) {
        float v = zGetMat2x2ElemG(inputs, row, column);
        v = 2.0f * v;
        zSetMat2x2ElemL(&mat[0], row, column, v);
      }
    }
    for (size_t column = 0; column < 2; ++column) {
      for (size_t row = 0; row < 2; ++row) {
        const float v = zGetMat2x2ElemL(&mat[0], row, column);
        zSetMat2x2ElemG(&outputs[0], row, column, v);
      }
    }
    // Mat2
    zInitMat2x2L(&mat[1], 0.0f, 1.0f,
                          2.0f, 3.0f);
    {
      const ZMatrix2x2 m = mat[1];
      for (size_t row = 0; row < 2; ++row) {
        const float2 vector = zGetMat2x2RowVec(&m, row);
        zSetMat2x2RowVecG(&outputs[1], row, vector);
      }
    }
    {
      for (size_t column = 0; column < 2; ++column) {
        const float2 vector = zGetMat2x2ColumnVecL(&mat[1], column);
        zSetMat2x2ColumnVecG(&outputs[2], column, vector);
      }
    }
  }
}

__kernel void testMatrix3x3Init(__global ZMatrix3x3* inputs,
    __global ZMatrix3x3* outputs)
{
  const uint32b index = zGetGlobalIdX();
  __local ZMatrix3x3 mat[2];
  if (index == 0) {
    // Mat1
    for (size_t column = 0; column < 3; ++column) {
      for (size_t row = 0; row < 3; ++row) {
        float v = zGetMat3x3ElemG(inputs, row, column);
        v = 2.0f * v;
        zSetMat3x3ElemL(&mat[0], row, column, v);
      }
    }
    for (size_t column = 0; column < 3; ++column) {
      for (size_t row = 0; row < 3; ++row) {
        const float v = zGetMat3x3ElemL(&mat[0], row, column);
        zSetMat3x3ElemG(&outputs[0], row, column, v);
      }
    }
    // Mat2
    zInitMat3x3L(&mat[1], 0.0f, 1.0f, 2.0f,
                          3.0f, 4.0f, 5.0f,
                          6.0f, 7.0f, 8.0f);
    {
      const ZMatrix3x3 m = mat[1];
      for (size_t row = 0; row < 3; ++row) {
        const float3 vector = zGetMat3x3RowVec(&m, row);
        zSetMat3x3RowVecG(&outputs[1], row, vector);
      }
    }
    {
      for (size_t column = 0; column < 3; ++column) {
        const float3 vector = zGetMat3x3ColumnVecL(&mat[1], column);
        zSetMat3x3ColumnVecG(&outputs[2], column, vector);
      }
    }
  }
}

__kernel void testMatrix4x4Init(__global ZMatrix4x4* inputs,
    __global ZMatrix4x4* outputs)
{
  const uint32b index = zGetGlobalIdX();
  __local ZMatrix4x4 mat[2];
  if (index == 0) {
    // Mat1
    for (size_t column = 0; column < 4; ++column) {
      for (size_t row = 0; row < 4; ++row) {
        float v = zGetMat4x4ElemG(inputs, row, column);
        v = 2.0f * v;
        zSetMat4x4ElemL(&mat[0], row, column, v);
      }
    }
    for (size_t column = 0; column < 4; ++column) {
      for (size_t row = 0; row < 4; ++row) {
        const float v = zGetMat4x4ElemL(&mat[0], row, column);
        zSetMat4x4ElemG(&outputs[0], row, column, v);
      }
    }
    // Mat2
    zInitMat4x4L(&mat[1], 0.0f, 1.0f, 2.0f, 3.0f,
                          4.0f, 5.0f, 6.0f, 7.0f,
                          8.0f, 9.0f, 10.0f, 11.0f,
                          12.0f, 13.0f, 14.0f, 15.0f);
    {
      const ZMatrix4x4 m = mat[1];
      for (size_t row = 0; row < 4; ++row) {
        const float4 vector = zGetMat4x4RowVec(&m, row);
        zSetMat4x4RowVecG(&outputs[1], row, vector);
      }
    }
    {
      for (size_t column = 0; column < 4; ++column) {
        const float4 vector = zGetMat4x4ColumnVecL(&mat[1], column);
        zSetMat4x4ColumnVecG(&outputs[2], column, vector);
      }
    }
  }
}

#endif /* ZINVUL_MATH_TEST_MATRIX_CL */
