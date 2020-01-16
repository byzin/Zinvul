/*!
  \file matrix.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATRIX_CL
#define ZINVUL_MATRIX_CL

#include "math.cl"
#include "types.cl"
#include "utility.cl"

///*!
//  \brief 2x2 matrix
//  */
//typedef struct __ZMatrix2x2
//{
//  float m_[4];
//} ZMatrix2x2;
//
///*!
//  \brief 3x3 matrix
//  */
//typedef struct __ZMatrix3x3
//{
//  float m_[9];
//} ZMatrix3x3;
//
///*!
//  \brief 4x4 matrix
//  */
//typedef struct __ZMatrix4x4
//{
//  float m_[16];
//} ZMatrix4x4;
//
////! Make a 2x2 matrix
//ZMatrix2x2 zMakeMat2x2(
//    const float m00, const float m01,
//    const float m10, const float m11)
//{
//  const ZMatrix2x2 matrix = {{m00, m01,
//                              m10, m11}};
//  return matrix;
//}
//
////! Make a 3x3 matrix
//ZMatrix3x3 zMakeMat3x3(
//    const float m00, const float m01, const float m02,
//    const float m10, const float m11, const float m12,
//    const float m20, const float m21, const float m22)
//{
//  const ZMatrix3x3 matrix = {{m00, m01, m02,
//                              m10, m11, m12,
//                              m20, m21, m22}};
//  return matrix;
//}
//
////! Make a 4x4 matrix
//ZMatrix4x4 zMakeMat4x4(
//    const float m00, const float m01, const float m02, const float m03,
//    const float m10, const float m11, const float m12, const float m13,
//    const float m20, const float m21, const float m22, const float m23,
//    const float m30, const float m31, const float m32, const float m33)
//{
//  const ZMatrix4x4 matrix = {{m00, m01, m02, m03,
//                              m10, m11, m12, m13,
//                              m20, m21, m22, m23,
//                              m30, m31, m32, m33}};
//  return matrix;
//}
//
////! Access to an element of a matrix
//#define ZINVUL_ACCESS_MAT_ELEM_IMPL(matrix, n, row, column) \
//    matrix->m_[row * n + column]
//
////! Access to an element of a 2x2 matrix
//#define ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column) \
//    ZINVUL_ACCESS_MAT_ELEM_IMPL(matrix, 2, row, column)
//
////! Return an element of a 2x2 matrix
//float zGetMat2x2Elem(const __private ZMatrix2x2* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Return an element of a 2x2 matrix
//float zGetMat2x2ElemL(const __local ZMatrix2x2* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Return an element of a 2x2 matrix
//float zGetMat2x2ElemG(const __global ZMatrix2x2* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Assign a value to an element of a 2x2 matrix
//void zSetMat2x2Elem(__private ZMatrix2x2* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Assign a value to an element of a 2x2 matrix
//void zSetMat2x2ElemL(__local ZMatrix2x2* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Assign a value to an element of a 2x2 matrix
//void zSetMat2x2ElemG(__global ZMatrix2x2* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Return a row vector of a 2x2 matrix
//#define ZINVUL_GET_MAT2x2_ROW_VEC(matrix, row) \
//    zMakeFloat2(ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, 0), \
//                ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, 1))
//
////! Return a row vector of a 2x2 matrix
//float2 zGetMat2x2RowVec(const __private ZMatrix2x2* matrix,
//    const size_t row)
//{
//  const float2 vector = ZINVUL_GET_MAT2x2_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Return a row vector of a 2x2 matrix
//float2 zGetMat2x2RowVecL(const __local ZMatrix2x2* matrix,
//    const size_t row)
//{
//  const float2 vector = ZINVUL_GET_MAT2x2_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Return a row vector of a 2x2 matrix
//float2 zGetMat2x2RowVecG(const __global ZMatrix2x2* matrix,
//    const size_t row)
//{
//  const float2 vector = ZINVUL_GET_MAT2x2_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Assign to a row vector of a 2x2 matrix
//#define ZINVUL_SET_MAT2x2_ROW_VEC(matrix, row, vector) \
//    { \
//      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, 0) = vector.x; \
//      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, 1) = vector.y; \
//    }
//
////! Assign to a row vector of a 2x2 matrix
//void zSetMat2x2RowVec(__private ZMatrix2x2* matrix,
//    const size_t row,
//    const float2 vector)
//{
//  ZINVUL_SET_MAT2x2_ROW_VEC(matrix, row, vector);
//}
//
////! Assign to a row vector of a 2x2 matrix
//void zSetMat2x2RowVecL(__local ZMatrix2x2* matrix,
//    const size_t row,
//    const float2 vector)
//{
//  ZINVUL_SET_MAT2x2_ROW_VEC(matrix, row, vector);
//}
//
////! Assign to a row vector of a 2x2 matrix
//void zSetMat2x2RowVecG(__global ZMatrix2x2* matrix,
//    const size_t row,
//    const float2 vector)
//{
//  ZINVUL_SET_MAT2x2_ROW_VEC(matrix, row, vector);
//}
//
////! Return a column vector of a 2x2 matrix
//#define ZINVUL_GET_MAT2x2_COLUMN_VEC(matrix, column) \
//    zMakeFloat2(ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 0, column), \
//                ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 1, column))
//
////! Return a column vector of a 2x2 matrix
//float2 zGetMat2x2ColumnVec(const __private ZMatrix2x2* matrix,
//    const size_t column)
//{
//  const float2 vector = ZINVUL_GET_MAT2x2_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Return a column vector of a 2x2 matrix
//float2 zGetMat2x2ColumnVecL(const __local ZMatrix2x2* matrix,
//    const size_t column)
//{
//  const float2 vector = ZINVUL_GET_MAT2x2_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Return a column vector of a 2x2 matrix
//float2 zGetMat2x2ColumnVecG(const __global ZMatrix2x2* matrix,
//    const size_t column)
//{
//  const float2 vector = ZINVUL_GET_MAT2x2_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Assign to a column vector of a 2x2 matrix
//#define ZINVUL_SET_MAT2x2_COLUMN_VEC(matrix, column, vector) \
//    { \
//      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 0, column) = vector.x; \
//      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 1, column) = vector.y; \
//    }
//
////! Assign to a column vector of a 2x2 matrix
//void zSetMat2x2ColumnVec(__private ZMatrix2x2* matrix,
//    const size_t column,
//    const float2 vector)
//{
//  ZINVUL_SET_MAT2x2_COLUMN_VEC(matrix, column, vector);
//}
//
////! Assign to a column vector of a 2x2 matrix
//void zSetMat2x2ColumnVecL(__local ZMatrix2x2* matrix,
//    const size_t column,
//    const float2 vector)
//{
//  ZINVUL_SET_MAT2x2_COLUMN_VEC(matrix, column, vector);
//}
//
////! Assign to a column vector of a 2x2 matrix
//void zSetMat2x2ColumnVecG(__global ZMatrix2x2* matrix,
//    const size_t column,
//    const float2 vector)
//{
//  ZINVUL_SET_MAT2x2_COLUMN_VEC(matrix, column, vector);
//}
//
////! Access to an element of a 3x3 matrix
//#define ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column) \
//    ZINVUL_ACCESS_MAT_ELEM_IMPL(matrix, 3, row, column)
//
////! Return an element of a 3x3 matrix
//float zGetMat3x3Elem(const __private ZMatrix3x3* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Return an element of a 3x3 matrix
//float zGetMat3x3ElemL(const __local ZMatrix3x3* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Return an element of a 3x3 matrix
//float zGetMat3x3ElemG(const __global ZMatrix3x3* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Assign a value to an element of a 3x3 matrix
//void zSetMat3x3Elem(__private ZMatrix3x3* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Assign a value to an element of a 3x3 matrix
//void zSetMat3x3ElemL(__local ZMatrix3x3* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Assign a value to an element of a 3x3 matrix
//void zSetMat3x3ElemG(__global ZMatrix3x3* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Return a row vector of a 3x3 matrix
//#define ZINVUL_GET_MAT3x3_ROW_VEC(matrix, row) \
//    zMakeFloat3(ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 0), \
//                ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 1), \
//                ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 2))
//
////! Return a row vector of a 3x3 matrix
//float3 zGetMat3x3RowVec(const __private ZMatrix3x3* matrix,
//    const size_t row)
//{
//  const float3 vector = ZINVUL_GET_MAT3x3_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Return a row vector of a 3x3 matrix
//float3 zGetMat3x3RowVecL(const __local ZMatrix3x3* matrix,
//    const size_t row)
//{
//  const float3 vector = ZINVUL_GET_MAT3x3_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Return a row vector of a 3x3 matrix
//float3 zGetMat3x3RowVecG(const __global ZMatrix3x3* matrix,
//    const size_t row)
//{
//  const float3 vector = ZINVUL_GET_MAT3x3_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Assign to a row vector of a 3x3 matrix
//#define ZINVUL_SET_MAT3x3_ROW_VEC(matrix, row, vector) \
//    { \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 0) = vector.x; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 1) = vector.y; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 2) = vector.z; \
//    }
//
////! Assign to a row vector of a 3x3 matrix
//void zSetMat3x3RowVec(__private ZMatrix3x3* matrix,
//    const size_t row,
//    const float3 vector)
//{
//  ZINVUL_SET_MAT3x3_ROW_VEC(matrix, row, vector);
//}
//
////! Assign to a row vector of a 3x3 matrix
//void zSetMat3x3RowVecL(__local ZMatrix3x3* matrix,
//    const size_t row,
//    const float3 vector)
//{
//  ZINVUL_SET_MAT3x3_ROW_VEC(matrix, row, vector);
//}
//
////! Assign to a row vector of a 3x3 matrix
//void zSetMat3x3RowVecG(__global ZMatrix3x3* matrix,
//    const size_t row,
//    const float3 vector)
//{
//  ZINVUL_SET_MAT3x3_ROW_VEC(matrix, row, vector);
//}
//
////! Return a column vector of a 3x3 matrix
//#define ZINVUL_GET_MAT3x3_COLUMN_VEC(matrix, column) \
//    zMakeFloat3(ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, column), \
//                ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, column), \
//                ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, column))
//
////! Return a column vector of a 3x3 matrix
//float3 zGetMat3x3ColumnVec(const __private ZMatrix3x3* matrix,
//    const size_t column)
//{
//  const float3 vector = ZINVUL_GET_MAT3x3_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Return a column vector of a 3x3 matrix
//float3 zGetMat3x3ColumnVecL(const __local ZMatrix3x3* matrix,
//    const size_t column)
//{
//  const float3 vector = ZINVUL_GET_MAT3x3_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Return a column vector of a 3x3 matrix
//float3 zGetMat3x3ColumnVecG(const __global ZMatrix3x3* matrix,
//    const size_t column)
//{
//  const float3 vector = ZINVUL_GET_MAT3x3_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Assign to a column vector of a 3x3 matrix
//#define ZINVUL_SET_MAT3x3_COLUMN_VEC(matrix, column, vector) \
//    { \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, column) = vector.x; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, column) = vector.y; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, column) = vector.z; \
//    }
//
////! Assign to a column vector of a 3x3 matrix
//void zSetMat3x3ColumnVec(__private ZMatrix3x3* matrix,
//    const size_t column,
//    const float3 vector)
//{
//  ZINVUL_SET_MAT3x3_COLUMN_VEC(matrix, column, vector);
//}
//
////! Assign to a column vector of a 3x3 matrix
//void zSetMat3x3ColumnVecL(__local ZMatrix3x3* matrix,
//    const size_t column,
//    const float3 vector)
//{
//  ZINVUL_SET_MAT3x3_COLUMN_VEC(matrix, column, vector);
//}
//
////! Assign to a column vector of a 3x3 matrix
//void zSetMat3x3ColumnVecG(__global ZMatrix3x3* matrix,
//    const size_t column,
//    const float3 vector)
//{
//  ZINVUL_SET_MAT3x3_COLUMN_VEC(matrix, column, vector);
//}
//
////! Access to an element of a 3x3 matrix
//#define ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column) \
//    ZINVUL_ACCESS_MAT_ELEM_IMPL(matrix, 4, row, column)
//
////! Return an element of a 4x4 matrix
//float zGetMat4x4Elem(__private ZMatrix4x4* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Return an element of a 4x4 matrix
//float zGetMat4x4ElemL(__local ZMatrix4x4* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Return an element of a 4x4 matrix
//float zGetMat4x4ElemG(__global ZMatrix4x4* matrix,
//    const size_t row,
//    const size_t column)
//{
//  const float value = ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column);
//  return value;
//}
//
////! Assign a value to an element of a 4x4 matrix
//void zSetMat4x4Elem(__private ZMatrix4x4* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Assign a value to an element of a 4x4 matrix
//void zSetMat4x4ElemL(__local ZMatrix4x4* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Assign a value to an element of a 4x4 matrix
//void zSetMat4x4ElemG(__global ZMatrix4x4* matrix,
//    const size_t row,
//    const size_t column,
//    const float value)
//{
//  ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column) = value;
//}
//
////! Return a row vector of a 3x3 matrix
//#define ZINVUL_GET_MAT4x4_ROW_VEC(matrix, row) \
//    zMakeFloat4(ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 0), \
//                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 1), \
//                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 2), \
//                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 3))
//
////! Return a row vector of a 4x4 matrix
//float4 zGetMat4x4RowVec(const __private ZMatrix4x4* matrix,
//    const size_t row)
//{
//  const float4 vector = ZINVUL_GET_MAT4x4_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Return a row vector of a 4x4 matrix
//float4 zGetMat4x4RowVecL(const __local ZMatrix4x4* matrix,
//    const size_t row)
//{
//  const float4 vector = ZINVUL_GET_MAT4x4_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Return a row vector of a 4x4 matrix
//float4 zGetMat4x4RowVecG(const __global ZMatrix4x4* matrix,
//    const size_t row)
//{
//  const float4 vector = ZINVUL_GET_MAT4x4_ROW_VEC(matrix, row);
//  return vector;
//}
//
////! Assign to a row vector of a 4x4 matrix
//#define ZINVUL_SET_MAT4x4_ROW_VEC(matrix, row, vector) \
//    { \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 0) = vector.x; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 1) = vector.y; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 2) = vector.z; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 3) = vector.w; \
//    }
//
////! Assign to a row vector of a 4x4 matrix
//void zSetMat4x4RowVec(__private ZMatrix4x4* matrix,
//    const size_t row,
//    const float4 vector)
//{
//  ZINVUL_SET_MAT4x4_ROW_VEC(matrix, row, vector);
//}
//
////! Assign to a row vector of a 4x4 matrix
//void zSetMat4x4RowVecL(__local ZMatrix4x4* matrix,
//    const size_t row,
//    const float4 vector)
//{
//  ZINVUL_SET_MAT4x4_ROW_VEC(matrix, row, vector);
//}
//
////! Assign to a row vector of a 4x4 matrix
//void zSetMat4x4RowVecG(__global ZMatrix4x4* matrix,
//    const size_t row,
//    const float4 vector)
//{
//  ZINVUL_SET_MAT4x4_ROW_VEC(matrix, row, vector);
//}
//
////! Return a column vector of a 4x4 matrix
//#define ZINVUL_GET_MAT4x4_COLUMN_VEC(matrix, column) \
//    zMakeFloat4(ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, column), \
//                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, column), \
//                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, column), \
//                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, column))
//
////! Return a column vector of a 4x4 matrix
//float4 zGetMat4x4ColumnVec(const __private ZMatrix4x4* matrix,
//    const size_t column)
//{
//  const float4 vector = ZINVUL_GET_MAT4x4_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Return a column vector of a 4x4 matrix
//float4 zGetMat4x4ColumnVecL(const __local ZMatrix4x4* matrix,
//    const size_t column)
//{
//  const float4 vector = ZINVUL_GET_MAT4x4_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Return a column vector of a 4x4 matrix
//float4 zGetMat4x4ColumnVecG(const __global ZMatrix4x4* matrix,
//    const size_t column)
//{
//  const float4 vector = ZINVUL_GET_MAT4x4_COLUMN_VEC(matrix, column);
//  return vector;
//}
//
////! Assign to a column vector of a 4x4 matrix
//#define ZINVUL_SET_MAT4x4_COLUMN_VEC(matrix, column, vector) \
//    { \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, column) = vector.x; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, column) = vector.y; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, column) = vector.z; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, column) = vector.w; \
//    }
//
////! Assign to a column vector of a 4x4 matrix
//void zSetMat4x4ColumnVec(__private ZMatrix4x4* matrix,
//    const size_t column,
//    const float4 vector)
//{
//  ZINVUL_SET_MAT4x4_COLUMN_VEC(matrix, column, vector);
//}
//
////! Assign to a column vector of a 4x4 matrix
//void zSetMat4x4ColumnVecL(__local ZMatrix4x4* matrix,
//    const size_t column,
//    const float4 vector)
//{
//  ZINVUL_SET_MAT4x4_COLUMN_VEC(matrix, column, vector);
//}
//
////! Assign to a column vector of a 4x4 matrix
//void zSetMat4x4ColumnVecG(__global ZMatrix4x4* matrix,
//    const size_t column,
//    const float4 vector)
//{
//  ZINVUL_SET_MAT4x4_COLUMN_VEC(matrix, column, vector);
//}
//
////! Initialize a 2x2 matrix
//#define ZINVUL_INIT_MAT2x2_IMPL(matrix, m00, m01, \
//                                        m10, m11) \
//    { \
//      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 0, 0) = m00; \
//      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 0, 1) = m01; \
//      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 1, 0) = m10; \
//      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 1, 1) = m11; \
//    }
//
////! Initialize a 2x2 matrix
//void zInitMat2x2(__private ZMatrix2x2* matrix,
//    const float m00, const float m01,
//    const float m10, const float m11)
//{
//  ZINVUL_INIT_MAT2x2_IMPL(matrix, m00, m01,
//                                  m10, m11);
//}
//
////! Initialize a 2x2 matrix
//void zInitMat2x2L(__local ZMatrix2x2* matrix,
//    const float m00, const float m01,
//    const float m10, const float m11)
//{
//  ZINVUL_INIT_MAT2x2_IMPL(matrix, m00, m01,
//                                  m10, m11);
//}
//
////! Initialize a 2x2 matrix
//void zInitMat2x2G(__global ZMatrix2x2* matrix,
//    const float m00, const float m01,
//    const float m10, const float m11)
//{
//  ZINVUL_INIT_MAT2x2_IMPL(matrix, m00, m01,
//                                  m10, m11);
//}
//
////! Initialize a 3x3 matrix
//#define ZINVUL_INIT_MAT3x3_IMPL(matrix, m00, m01, m02, \
//                                        m10, m11, m12, \
//                                        m20, m21, m22) \
//    { \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, 0) = m00; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, 1) = m01; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, 2) = m02; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, 0) = m10; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, 1) = m11; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, 2) = m12; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, 0) = m20; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, 1) = m21; \
//      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, 2) = m22; \
//    }
//
////! Initialize a 3x3 matrix
//void zInitMat3x3(__private ZMatrix3x3* matrix,
//    const float m00, const float m01, const float m02,
//    const float m10, const float m11, const float m12,
//    const float m20, const float m21, const float m22)
//{
//  ZINVUL_INIT_MAT3x3_IMPL(matrix, m00, m01, m02,
//                                  m10, m11, m12,
//                                  m20, m21, m22);
//}
//
////! Initialize a 3x3 matrix
//void zInitMat3x3L(__local ZMatrix3x3* matrix,
//    const float m00, const float m01, const float m02,
//    const float m10, const float m11, const float m12,
//    const float m20, const float m21, const float m22)
//{
//  ZINVUL_INIT_MAT3x3_IMPL(matrix, m00, m01, m02,
//                                  m10, m11, m12,
//                                  m20, m21, m22);
//}
//
////! Initialize a 3x3 matrix
//void zInitMat3x3G(__global ZMatrix3x3* matrix,
//    const float m00, const float m01, const float m02,
//    const float m10, const float m11, const float m12,
//    const float m20, const float m21, const float m22)
//{
//  ZINVUL_INIT_MAT3x3_IMPL(matrix, m00, m01, m02,
//                                  m10, m11, m12,
//                                  m20, m21, m22);
//}
//
////! Initialize a 4x4 matrix
//#define ZINVUL_INIT_MAT4x4_IMPL(matrix, m00, m01, m02, m03, \
//                                        m10, m11, m12, m13, \
//                                        m20, m21, m22, m23, \
//                                        m30, m31, m32, m33) \
//    { \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, 0) = m00; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, 1) = m01; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, 2) = m02; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, 3) = m03; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, 0) = m10; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, 1) = m11; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, 2) = m12; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, 3) = m13; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, 0) = m20; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, 1) = m21; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, 2) = m22; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, 3) = m23; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, 0) = m30; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, 1) = m31; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, 2) = m32; \
//      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, 3) = m33; \
//    }
//
////! Initialize a 4x4 matrix
//void zInitMat4x4(__private ZMatrix4x4* matrix,
//    const float m00, const float m01, const float m02, const float m03,
//    const float m10, const float m11, const float m12, const float m13,
//    const float m20, const float m21, const float m22, const float m23,
//    const float m30, const float m31, const float m32, const float m33)
//{
//  ZINVUL_INIT_MAT4x4_IMPL(matrix, m00, m01, m02, m03,
//                                  m10, m11, m12, m13,
//                                  m20, m21, m22, m23,
//                                  m30, m31, m32, m33);
//}
//
////! Initialize a 4x4 matrix
//void zInitMat4x4L(__local ZMatrix4x4* matrix,
//    const float m00, const float m01, const float m02, const float m03,
//    const float m10, const float m11, const float m12, const float m13,
//    const float m20, const float m21, const float m22, const float m23,
//    const float m30, const float m31, const float m32, const float m33)
//{
//  ZINVUL_INIT_MAT4x4_IMPL(matrix, m00, m01, m02, m03,
//                                  m10, m11, m12, m13,
//                                  m20, m21, m22, m23,
//                                  m30, m31, m32, m33);
//}
//
////! Initialize a 4x4 matrix
//void zInitMat4x4G(__global ZMatrix4x4* matrix,
//    const float m00, const float m01, const float m02, const float m03,
//    const float m10, const float m11, const float m12, const float m13,
//    const float m20, const float m21, const float m22, const float m23,
//    const float m30, const float m31, const float m32, const float m33)
//{
//  ZINVUL_INIT_MAT4x4_IMPL(matrix, m00, m01, m02, m03,
//                                  m10, m11, m12, m13,
//                                  m20, m21, m22, m23,
//                                  m30, m31, m32, m33);
//}
//
////! Copy a matrix from src to dst
//#define ZINVUL_COPY_MAT_IMPL(n, src, dst) \
//  for (size_t i = 0; i < n; ++i) { \
//    dst->m_[i] = src->m_[i]; \
//  }
//
////! Copy a 2x2 matrix from src to dst
//#define ZINVUL_COPY_MAT2x2_IMPL(src, dst) ZINVUL_COPY_MAT_IMPL(4, src, dst)
//
////! Copy a matrix from src to dst
//void zCopyMat2x2(const __private ZMatrix2x2* src, __private ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat2x2PL(const __private ZMatrix2x2* src, __local ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat2x2LP(const __local ZMatrix2x2* src, __private ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat2x2PG(const __private ZMatrix2x2* src, __global ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat2x2GP(const __global ZMatrix2x2* src, __private ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat2x2L(const __local ZMatrix2x2* src, __local ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat2x2G(const __global ZMatrix2x2* src, __global ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat2x2LG(const __local ZMatrix2x2* src, __global ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat2x2GL(const __global ZMatrix2x2* src, __local ZMatrix2x2* dst)
//{
//  ZINVUL_COPY_MAT2x2_IMPL(src, dst);
//}
//
////! Load a 2x2 matrix
//ZMatrix2x2 zLoadMat2x2L(const __local ZMatrix2x2* src)
//{
//  const ZMatrix2x2 matrix = *src;
//  return matrix;
//}
//
////! Load a 2x2 matrix
//ZMatrix2x2 zLoadMat2x2G(const __global ZMatrix2x2* src)
//{
//  const ZMatrix2x2 matrix = *src;
//  return matrix;
//}
//
////! Copy a 3x3 matrix from src to dst
//#define ZINVUL_COPY_MAT3x3_IMPL(src, dst) ZINVUL_COPY_MAT_IMPL(9, src, dst)
//
////! Copy a matrix from src to dst
//void zCopyMat3x3(const __private ZMatrix3x3* src, __private ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat3x3PL(const __private ZMatrix3x3* src, __local ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat3x3LP(const __local ZMatrix3x3* src, __private ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat3x3PG(const __private ZMatrix3x3* src, __global ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat3x3GP(const __global ZMatrix3x3* src, __private ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat3x3L(const __local ZMatrix3x3* src, __local ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat3x3G(const __global ZMatrix3x3* src, __global ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat3x3LG(const __local ZMatrix3x3* src, __global ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat3x3GL(const __global ZMatrix3x3* src, __local ZMatrix3x3* dst)
//{
//  ZINVUL_COPY_MAT3x3_IMPL(src, dst);
//}
//
////! Load a 3x3 matrix
//ZMatrix3x3 zLoadMat3x3L(const __local ZMatrix3x3* src)
//{
//  const ZMatrix3x3 matrix = *src;
//  return matrix;
//}
//
////! Load a 3x3 matrix
//ZMatrix3x3 zLoadMat3x3G(const __global ZMatrix3x3* src)
//{
//  const ZMatrix3x3 matrix = *src;
//  return matrix;
//}
//
////! Copy a 4x4 matrix from src to dst
//#define ZINVUL_COPY_MAT4x4_IMPL(src, dst) ZINVUL_COPY_MAT_IMPL(16, src, dst)
//
////! Copy a matrix from src to dst
//void zCopyMat4x4(const __private ZMatrix4x4* src, __private ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat4x4PL(const __private ZMatrix4x4* src, __local ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat4x4LP(const __local ZMatrix4x4* src, __private ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat4x4PG(const __private ZMatrix4x4* src, __global ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat4x4GP(const __global ZMatrix4x4* src, __private ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat4x4L(const __local ZMatrix4x4* src, __local ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat4x4G(const __global ZMatrix4x4* src, __global ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat4x4LG(const __local ZMatrix4x4* src, __global ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Copy a matrix from src to dst
//void zCopyMat4x4GL(const __global ZMatrix4x4* src, __local ZMatrix4x4* dst)
//{
//  ZINVUL_COPY_MAT4x4_IMPL(src, dst);
//}
//
////! Load a 4x4 matrix
//ZMatrix4x4 zLoadMat4x4L(const __local ZMatrix4x4* src)
//{
//  const ZMatrix4x4 matrix = *src;
//  return matrix;
//}
//
////! Load a 4x4 matrix
//ZMatrix4x4 zLoadMat4x4G(const __global ZMatrix4x4* src)
//{
//  const ZMatrix4x4 matrix = *src;
//  return matrix;
//}
//
////! Perform matrix operations 
//#define ZINVUL_ADDSUB_MAT_IMPL(n, floatn, lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, op, lhs, rhs, dst) \
//    for (size_t row = 0; row < n; ++row) { \
//      const floatn l = lhs_row_vec_getter(lhs, row); \
//      const floatn r = rhs_row_vec_getter(rhs, row); \
//      const floatn d = l op r; \
//      dst_row_vec_setter(dst, row, d); \
//    }
//
////! Perform matrix addition
//#define ZINVUL_ADD_MAT2x2_IMPL(lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_ADDSUB_MAT_IMPL(2, float2, lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, +, lhs, rhs, dst)
//
////! Perform matrix addition
//void zAddMat2x2(const __private ZMatrix2x2* lhs,
//                const __private ZMatrix2x2* rhs,
//                __private ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVec, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddMat2x2L(const __private ZMatrix2x2* lhs,
//                 const __private ZMatrix2x2* rhs,
//                 __local ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVec, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddMat2x2G(const __private ZMatrix2x2* lhs,
//                 const __private ZMatrix2x2* rhs,
//                 __global ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVec, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat2x2(const __local ZMatrix2x2* lhs,
//                 const __local ZMatrix2x2* rhs,
//                 __private ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecL, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat2x2L(const __local ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __local ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecL, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat2x2G(const __local ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __global ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecL, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat2x2(const __private ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecL, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat2x2L(const __private ZMatrix2x2* lhs,
//                   const __local ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecL, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat2x2G(const __private ZMatrix2x2* lhs,
//                   const __local ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecL, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat2x2(const __global ZMatrix2x2* lhs,
//                 const __global ZMatrix2x2* rhs,
//                 __private ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecG, zGetMat2x2RowVecG, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat2x2L(const __global ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __local ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecG, zGetMat2x2RowVecG, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat2x2G(const __global ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __global ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecG, zGetMat2x2RowVecG, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat2x2(const __private ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecG, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat2x2L(const __private ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecG, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat2x2G(const __private ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecG, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat2x2(const __local ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecG, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat2x2L(const __local ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecG, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat2x2G(const __local ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_ADD_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecG, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//#define ZINVUL_SUB_MAT2x2_IMPL(lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_ADDSUB_MAT_IMPL(2, float2, lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, -, lhs, rhs, dst)
//
////! Perform matrix subtraction
//void zSubMat2x2(const __private ZMatrix2x2* lhs,
//                const __private ZMatrix2x2* rhs,
//                __private ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVec, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubMat2x2L(const __private ZMatrix2x2* lhs,
//                 const __private ZMatrix2x2* rhs,
//                 __local ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVec, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubMat2x2G(const __private ZMatrix2x2* lhs,
//                 const __private ZMatrix2x2* rhs,
//                 __global ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVec, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat2x2(const __local ZMatrix2x2* lhs,
//                 const __local ZMatrix2x2* rhs,
//                 __private ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecL, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat2x2L(const __local ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __local ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecL, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat2x2G(const __local ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __global ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecL, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat2x2(const __private ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecL, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat2x2L(const __private ZMatrix2x2* lhs,
//                   const __local ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecL, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat2x2G(const __private ZMatrix2x2* lhs,
//                   const __local ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecL, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat2x2(const __global ZMatrix2x2* lhs,
//                 const __global ZMatrix2x2* rhs,
//                 __private ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecG, zGetMat2x2RowVecG, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat2x2L(const __global ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __local ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecG, zGetMat2x2RowVecG, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat2x2G(const __global ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __global ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecG, zGetMat2x2RowVecG, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat2x2(const __private ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecG, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat2x2L(const __private ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecG, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat2x2G(const __private ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVec, zGetMat2x2RowVecG, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat2x2(const __local ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecG, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat2x2L(const __local ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecG, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat2x2G(const __local ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_SUB_MAT2x2_IMPL(zGetMat2x2RowVecL, zGetMat2x2RowVecG, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//#define ZINVUL_ADD_MAT3x3_IMPL(lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_ADDSUB_MAT_IMPL(3, float3, lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, +, lhs, rhs, dst)
//
////! Perform matrix addition
//void zAddMat3x3(const __private ZMatrix3x3* lhs,
//                const __private ZMatrix3x3* rhs,
//                __private ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVec, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddMat3x3L(const __private ZMatrix3x3* lhs,
//                 const __private ZMatrix3x3* rhs,
//                 __local ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVec, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddMat3x3G(const __private ZMatrix3x3* lhs,
//                 const __private ZMatrix3x3* rhs,
//                 __global ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVec, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat3x3(const __local ZMatrix3x3* lhs,
//                 const __local ZMatrix3x3* rhs,
//                 __private ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecL, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat3x3L(const __local ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __local ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecL, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat3x3G(const __local ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __global ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecL, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat3x3(const __private ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecL, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat3x3L(const __private ZMatrix3x3* lhs,
//                   const __local ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecL, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat3x3G(const __private ZMatrix3x3* lhs,
//                   const __local ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecL, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat3x3(const __global ZMatrix3x3* lhs,
//                 const __global ZMatrix3x3* rhs,
//                 __private ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecG, zGetMat3x3RowVecG, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat3x3L(const __global ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __local ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecG, zGetMat3x3RowVecG, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat3x3G(const __global ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __global ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecG, zGetMat3x3RowVecG, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat3x3(const __private ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecG, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat3x3L(const __private ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecG, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat3x3G(const __private ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecG, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat3x3(const __local ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecG, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat3x3L(const __local ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecG, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat3x3G(const __local ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_ADD_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecG, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//#define ZINVUL_SUB_MAT3x3_IMPL(lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_ADDSUB_MAT_IMPL(3, float3, lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, -, lhs, rhs, dst)
//
////! Perform matrix subtraction
//void zSubMat3x3(const __private ZMatrix3x3* lhs,
//                const __private ZMatrix3x3* rhs,
//                __private ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVec, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubMat3x3L(const __private ZMatrix3x3* lhs,
//                 const __private ZMatrix3x3* rhs,
//                 __local ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVec, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubMat3x3G(const __private ZMatrix3x3* lhs,
//                 const __private ZMatrix3x3* rhs,
//                 __global ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVec, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat3x3(const __local ZMatrix3x3* lhs,
//                 const __local ZMatrix3x3* rhs,
//                 __private ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecL, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat3x3L(const __local ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __local ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecL, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat3x3G(const __local ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __global ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecL, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat3x3(const __private ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecL, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat3x3L(const __private ZMatrix3x3* lhs,
//                   const __local ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecL, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat3x3G(const __private ZMatrix3x3* lhs,
//                   const __local ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecL, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat3x3(const __global ZMatrix3x3* lhs,
//                 const __global ZMatrix3x3* rhs,
//                 __private ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecG, zGetMat3x3RowVecG, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat3x3L(const __global ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __local ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecG, zGetMat3x3RowVecG, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat3x3G(const __global ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __global ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecG, zGetMat3x3RowVecG, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat3x3(const __private ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecG, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat3x3L(const __private ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecG, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat3x3G(const __private ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVec, zGetMat3x3RowVecG, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat3x3(const __local ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecG, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat3x3L(const __local ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecG, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat3x3G(const __local ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_SUB_MAT3x3_IMPL(zGetMat3x3RowVecL, zGetMat3x3RowVecG, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//#define ZINVUL_ADD_MAT4x4_IMPL(lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_ADDSUB_MAT_IMPL(4, float4, lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, +, lhs, rhs, dst)
//
////! Perform matrix addition
//void zAddMat4x4(const __private ZMatrix4x4* lhs,
//                const __private ZMatrix4x4* rhs,
//                __private ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVec, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddMat4x4L(const __private ZMatrix4x4* lhs,
//                 const __private ZMatrix4x4* rhs,
//                 __local ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVec, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddMat4x4G(const __private ZMatrix4x4* lhs,
//                 const __private ZMatrix4x4* rhs,
//                 __global ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVec, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat4x4(const __local ZMatrix4x4* lhs,
//                 const __local ZMatrix4x4* rhs,
//                 __private ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecL, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat4x4L(const __local ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __local ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecL, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLMat4x4G(const __local ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __global ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecL, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat4x4(const __private ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecL, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat4x4L(const __private ZMatrix4x4* lhs,
//                   const __local ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecL, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPLMat4x4G(const __private ZMatrix4x4* lhs,
//                   const __local ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecL, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat4x4(const __global ZMatrix4x4* lhs,
//                 const __global ZMatrix4x4* rhs,
//                 __private ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecG, zGetMat4x4RowVecG, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat4x4L(const __global ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __local ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecG, zGetMat4x4RowVecG, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddGMat4x4G(const __global ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __global ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecG, zGetMat4x4RowVecG, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat4x4(const __private ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecG, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat4x4L(const __private ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecG, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddPGMat4x4G(const __private ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecG, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat4x4(const __local ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecG, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat4x4L(const __local ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecG, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix addition
//void zAddLGMat4x4G(const __local ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_ADD_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecG, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//#define ZINVUL_SUB_MAT4x4_IMPL(lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_ADDSUB_MAT_IMPL(4, float4, lhs_row_vec_getter, rhs_row_vec_getter, dst_row_vec_setter, -, lhs, rhs, dst)
//
////! Perform matrix subtraction
//void zSubMat4x4(const __private ZMatrix4x4* lhs,
//                const __private ZMatrix4x4* rhs,
//                __private ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVec, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubMat4x4L(const __private ZMatrix4x4* lhs,
//                 const __private ZMatrix4x4* rhs,
//                 __local ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVec, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubMat4x4G(const __private ZMatrix4x4* lhs,
//                 const __private ZMatrix4x4* rhs,
//                 __global ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVec, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat4x4(const __local ZMatrix4x4* lhs,
//                 const __local ZMatrix4x4* rhs,
//                 __private ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecL, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat4x4L(const __local ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __local ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecL, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLMat4x4G(const __local ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __global ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecL, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat4x4(const __private ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecL, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat4x4L(const __private ZMatrix4x4* lhs,
//                   const __local ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecL, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPLMat4x4G(const __private ZMatrix4x4* lhs,
//                   const __local ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecL, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat4x4(const __global ZMatrix4x4* lhs,
//                 const __global ZMatrix4x4* rhs,
//                 __private ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecG, zGetMat4x4RowVecG, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat4x4L(const __global ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __local ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecG, zGetMat4x4RowVecG, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubGMat4x4G(const __global ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __global ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecG, zGetMat4x4RowVecG, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat4x4(const __private ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecG, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat4x4L(const __private ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecG, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubPGMat4x4G(const __private ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVec, zGetMat4x4RowVecG, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat4x4(const __local ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecG, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat4x4L(const __local ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecG, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix subtraction
//void zSubLGMat4x4G(const __local ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_SUB_MAT4x4_IMPL(zGetMat4x4RowVecL, zGetMat4x4RowVecG, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT_ROW_IMPL(n, floatn, lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, dotn, lhs, rhs, dst) \
//    for (size_t row = 0; row < n; ++row) { \
//      const floatn l = lhs_row_vec_getter(lhs, row); \
//      for (size_t column = 0; column < n; ++column) { \
//        const floatn r = rhs_column_vec_getter(rhs, column); \
//        const float d = dotn(l, r); \
//        dst_setter(dst, row, column, d); \
//      } \
//    }
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT_COLUMN_IMPL(n, floatn, lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, dotn, lhs, rhs, dst) \
//    for (size_t column = 0; column < n; ++column) { \
//      const floatn r = rhs_column_vec_getter(rhs, column); \
//      for (size_t row = 0; row < n; ++row) { \
//        const floatn l = lhs_row_vec_getter(lhs, row); \
//        const float d = dotn(l, r); \
//        dst_setter(dst, row, column, d); \
//      } \
//    }
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT_SCALAR_IMPL(n, floatn, lhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    for (size_t row = 0; row < n; ++row) { \
//      const floatn l = lhs_row_vec_getter(lhs, row); \
//      const floatn d = l * rhs; \
//      dst_row_vec_setter(dst, row, d); \
//    }
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT2x2_ROW_IMPL(lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_ROW_IMPL(2, float2, lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, zDot2, lhs, rhs, dst)
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT2x2_COLUMN_IMPL(lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_COLUMN_IMPL(2, float2, lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, zDot2, lhs, rhs, dst)
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT2x2_SCALAR_IMPL(lhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_SCALAR_IMPL(2, float2, lhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst)
//
////! Perform matrix multiplication
//void zMulMat2x2(const __private ZMatrix2x2* lhs,
//                const __private ZMatrix2x2* rhs,
//                __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVec, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat2x2L(const __private ZMatrix2x2* lhs,
//                 const __private ZMatrix2x2* rhs,
//                 __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVec, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat2x2G(const __private ZMatrix2x2* lhs,
//                 const __private ZMatrix2x2* rhs,
//                 __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVec, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat2x2(const __local ZMatrix2x2* lhs,
//                 const __local ZMatrix2x2* rhs,
//                 __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVecL, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat2x2L(const __local ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVecL, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat2x2G(const __local ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVecL, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat2x2(const __local ZMatrix2x2* lhs,
//                  const __private ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVec, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat2x2L(const __local ZMatrix2x2* lhs,
//                   const __private ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVec, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat2x2G(const __local ZMatrix2x2* lhs,
//                   const __private ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVec, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat2x2(const __private ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVecL, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat2x2L(const __private ZMatrix2x2* lhs,
//                   const __local ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVecL, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat2x2G(const __private ZMatrix2x2* lhs,
//                   const __local ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVecL, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat2x2(const __global ZMatrix2x2* lhs,
//                 const __global ZMatrix2x2* rhs,
//                 __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVecG, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat2x2L(const __global ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVecG, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat2x2G(const __global ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVecG, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat2x2(const __global ZMatrix2x2* lhs,
//                  const __private ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVec, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat2x2L(const __global ZMatrix2x2* lhs,
//                   const __private ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVec, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat2x2G(const __global ZMatrix2x2* lhs,
//                   const __private ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVec, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat2x2(const __private ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVecG, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat2x2L(const __private ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVecG, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat2x2G(const __private ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVec, zGetMat2x2ColumnVecG, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat2x2(const __global ZMatrix2x2* lhs,
//                  const __local ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVecL, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat2x2L(const __global ZMatrix2x2* lhs,
//                   const __local ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVecL, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat2x2G(const __global ZMatrix2x2* lhs,
//                   const __local ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_ROW_IMPL(zGetMat2x2RowVecG, zGetMat2x2ColumnVecL, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat2x2(const __local ZMatrix2x2* lhs,
//                  const __global ZMatrix2x2* rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVecG, zSetMat2x2Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat2x2L(const __local ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVecG, zSetMat2x2ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat2x2G(const __local ZMatrix2x2* lhs,
//                   const __global ZMatrix2x2* rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_COLUMN_IMPL(zGetMat2x2RowVecL, zGetMat2x2ColumnVecG, zSetMat2x2ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat2x2S(const __private ZMatrix2x2* lhs,
//                 const float rhs,
//                 __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVec, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat2x2SL(const __private ZMatrix2x2* lhs,
//                  const float rhs,
//                  __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVec, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat2x2SG(const __private ZMatrix2x2* lhs,
//                  const float rhs,
//                  __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVec, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat2x2S(const __local ZMatrix2x2* lhs,
//                  const float rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVecL, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat2x2SL(const __local ZMatrix2x2* lhs,
//                   const float rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVecL, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat2x2SG(const __local ZMatrix2x2* lhs,
//                   const float rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVecL, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat2x2S(const __global ZMatrix2x2* lhs,
//                  const float rhs,
//                  __private ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVecG, zSetMat2x2RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat2x2SL(const __global ZMatrix2x2* lhs,
//                   const float rhs,
//                   __local ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVecG, zSetMat2x2RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat2x2SG(const __global ZMatrix2x2* lhs,
//                   const float rhs,
//                   __global ZMatrix2x2* dst)
//{
//  ZINVUL_MUL_MAT2x2_SCALAR_IMPL(zGetMat2x2RowVecG, zSetMat2x2RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT3x3_ROW_IMPL(lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_ROW_IMPL(3, float3, lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, zDot3, lhs, rhs, dst)
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT3x3_COLUMN_IMPL(lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_COLUMN_IMPL(3, float3, lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, zDot3, lhs, rhs, dst)
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT3x3_SCALAR_IMPL(lhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_SCALAR_IMPL(3, float3, lhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst)
//
////! Perform matrix multiplication
//void zMulMat3x3(const __private ZMatrix3x3* lhs,
//                const __private ZMatrix3x3* rhs,
//                __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVec, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat3x3L(const __private ZMatrix3x3* lhs,
//                 const __private ZMatrix3x3* rhs,
//                 __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVec, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat3x3G(const __private ZMatrix3x3* lhs,
//                 const __private ZMatrix3x3* rhs,
//                 __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVec, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat3x3(const __local ZMatrix3x3* lhs,
//                 const __local ZMatrix3x3* rhs,
//                 __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVecL, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat3x3L(const __local ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVecL, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat3x3G(const __local ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVecL, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat3x3(const __local ZMatrix3x3* lhs,
//                  const __private ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVec, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat3x3L(const __local ZMatrix3x3* lhs,
//                   const __private ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVec, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat3x3G(const __local ZMatrix3x3* lhs,
//                   const __private ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVec, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat3x3(const __private ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVecL, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat3x3L(const __private ZMatrix3x3* lhs,
//                   const __local ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVecL, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat3x3G(const __private ZMatrix3x3* lhs,
//                   const __local ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVecL, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat3x3(const __global ZMatrix3x3* lhs,
//                 const __global ZMatrix3x3* rhs,
//                 __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVecG, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat3x3L(const __global ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVecG, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat3x3G(const __global ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVecG, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat3x3(const __global ZMatrix3x3* lhs,
//                  const __private ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVec, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat3x3L(const __global ZMatrix3x3* lhs,
//                   const __private ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVec, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat3x3G(const __global ZMatrix3x3* lhs,
//                   const __private ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVec, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat3x3(const __private ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVecG, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat3x3L(const __private ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVecG, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat3x3G(const __private ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVec, zGetMat3x3ColumnVecG, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat3x3(const __global ZMatrix3x3* lhs,
//                  const __local ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVecL, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat3x3L(const __global ZMatrix3x3* lhs,
//                   const __local ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVecL, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat3x3G(const __global ZMatrix3x3* lhs,
//                   const __local ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_ROW_IMPL(zGetMat3x3RowVecG, zGetMat3x3ColumnVecL, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat3x3(const __local ZMatrix3x3* lhs,
//                  const __global ZMatrix3x3* rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVecG, zSetMat3x3Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat3x3L(const __local ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVecG, zSetMat3x3ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat3x3G(const __local ZMatrix3x3* lhs,
//                   const __global ZMatrix3x3* rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_COLUMN_IMPL(zGetMat3x3RowVecL, zGetMat3x3ColumnVecG, zSetMat3x3ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat3x3S(const __private ZMatrix3x3* lhs,
//                 const float rhs,
//                 __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVec, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat3x3SL(const __private ZMatrix3x3* lhs,
//                  const float rhs,
//                  __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVec, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat3x3SG(const __private ZMatrix3x3* lhs,
//                  const float rhs,
//                  __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVec, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat3x3S(const __local ZMatrix3x3* lhs,
//                  const float rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVecL, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat3x3SL(const __local ZMatrix3x3* lhs,
//                   const float rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVecL, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat3x3SG(const __local ZMatrix3x3* lhs,
//                   const float rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVecL, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat3x3S(const __global ZMatrix3x3* lhs,
//                  const float rhs,
//                  __private ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVecG, zSetMat3x3RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat3x3SL(const __global ZMatrix3x3* lhs,
//                   const float rhs,
//                   __local ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVecG, zSetMat3x3RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat3x3SG(const __global ZMatrix3x3* lhs,
//                   const float rhs,
//                   __global ZMatrix3x3* dst)
//{
//  ZINVUL_MUL_MAT3x3_SCALAR_IMPL(zGetMat3x3RowVecG, zSetMat3x3RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT4x4_ROW_IMPL(lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_ROW_IMPL(4, float4, lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, zDot4, lhs, rhs, dst)
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT4x4_COLUMN_IMPL(lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_COLUMN_IMPL(4, float4, lhs_row_vec_getter, rhs_column_vec_getter, dst_setter, zDot4, lhs, rhs, dst)
//
////! Perform matrix multiplication
//#define ZINVUL_MUL_MAT4x4_SCALAR_IMPL(lhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst) \
//    ZINVUL_MUL_MAT_SCALAR_IMPL(4, float4, lhs_row_vec_getter, dst_row_vec_setter, lhs, rhs, dst)
//
////! Perform matrix multiplication
//void zMulMat4x4(const __private ZMatrix4x4* lhs,
//                const __private ZMatrix4x4* rhs,
//                __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVec, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat4x4L(const __private ZMatrix4x4* lhs,
//                 const __private ZMatrix4x4* rhs,
//                 __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVec, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat4x4G(const __private ZMatrix4x4* lhs,
//                 const __private ZMatrix4x4* rhs,
//                 __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVec, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat4x4(const __local ZMatrix4x4* lhs,
//                 const __local ZMatrix4x4* rhs,
//                 __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVecL, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat4x4L(const __local ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVecL, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat4x4G(const __local ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVecL, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat4x4(const __local ZMatrix4x4* lhs,
//                  const __private ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVec, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat4x4L(const __local ZMatrix4x4* lhs,
//                   const __private ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVec, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLPMat4x4G(const __local ZMatrix4x4* lhs,
//                   const __private ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVec, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat4x4(const __private ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVecL, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat4x4L(const __private ZMatrix4x4* lhs,
//                   const __local ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVecL, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPLMat4x4G(const __private ZMatrix4x4* lhs,
//                   const __local ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVecL, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat4x4(const __global ZMatrix4x4* lhs,
//                 const __global ZMatrix4x4* rhs,
//                 __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVecG, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat4x4L(const __global ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVecG, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat4x4G(const __global ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVecG, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat4x4(const __global ZMatrix4x4* lhs,
//                  const __private ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVec, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat4x4L(const __global ZMatrix4x4* lhs,
//                   const __private ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVec, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGPMat4x4G(const __global ZMatrix4x4* lhs,
//                   const __private ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVec, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat4x4(const __private ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVecG, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat4x4L(const __private ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVecG, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulPGMat4x4G(const __private ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVec, zGetMat4x4ColumnVecG, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat4x4(const __global ZMatrix4x4* lhs,
//                  const __local ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVecL, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat4x4L(const __global ZMatrix4x4* lhs,
//                   const __local ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVecL, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGLMat4x4G(const __global ZMatrix4x4* lhs,
//                   const __local ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_ROW_IMPL(zGetMat4x4RowVecG, zGetMat4x4ColumnVecL, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat4x4(const __local ZMatrix4x4* lhs,
//                  const __global ZMatrix4x4* rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVecG, zSetMat4x4Elem, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat4x4L(const __local ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVecG, zSetMat4x4ElemL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLGMat4x4G(const __local ZMatrix4x4* lhs,
//                   const __global ZMatrix4x4* rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_COLUMN_IMPL(zGetMat4x4RowVecL, zGetMat4x4ColumnVecG, zSetMat4x4ElemG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat4x4S(const __private ZMatrix4x4* lhs,
//                 const float rhs,
//                 __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVec, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat4x4SL(const __private ZMatrix4x4* lhs,
//                  const float rhs,
//                  __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVec, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulMat4x4SG(const __private ZMatrix4x4* lhs,
//                  const float rhs,
//                  __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVec, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat4x4S(const __local ZMatrix4x4* lhs,
//                  const float rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVecL, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat4x4SL(const __local ZMatrix4x4* lhs,
//                   const float rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVecL, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulLMat4x4SG(const __local ZMatrix4x4* lhs,
//                   const float rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVecL, zSetMat4x4RowVecG, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat4x4S(const __global ZMatrix4x4* lhs,
//                  const float rhs,
//                  __private ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVecG, zSetMat4x4RowVec, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat4x4SL(const __global ZMatrix4x4* lhs,
//                   const float rhs,
//                   __local ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVecG, zSetMat4x4RowVecL, lhs, rhs, dst);
//}
//
////! Perform matrix multiplication
//void zMulGMat4x4SG(const __global ZMatrix4x4* lhs,
//                   const float rhs,
//                   __global ZMatrix4x4* dst)
//{
//  ZINVUL_MUL_MAT4x4_SCALAR_IMPL(zGetMat4x4RowVecG, zSetMat4x4RowVecG, lhs, rhs, dst);
//}

#endif /* ZINVUL_MATRIX_CL */
