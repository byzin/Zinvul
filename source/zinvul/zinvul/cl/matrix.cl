/*!
  \file matrix.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_MATRIX_CL
#define ZINVUL_MATRIX_CL

#include "math.cl"
#include "types.cl"
#include "utility.cl"

/*!
  \brief 2x2 matrix
  */
typedef struct __ZMatrix2x2
{
  float m_[4];
} ZMatrix2x2;

/*!
  \brief 3x3 matrix
  */
typedef struct __ZMatrix3x3
{
  float m_[9];
} ZMatrix3x3;

/*!
  \brief 4x4 matrix
  */
typedef struct __ZMatrix4x4
{
  float m_[16];
} ZMatrix4x4;

//! Make a 2x2 matrix
ZMatrix2x2 zMakeMat2x2(
    const float m00, const float m01,
    const float m10, const float m11)
{
  const ZMatrix2x2 matrix = {{m00, m01,
                              m10, m11}};
  return matrix;
}

//! Make a 3x3 matrix
ZMatrix3x3 zMakeMat3x3(
    const float m00, const float m01, const float m02,
    const float m10, const float m11, const float m12,
    const float m20, const float m21, const float m22)
{
  const ZMatrix3x3 matrix = {{m00, m01, m02,
                              m10, m11, m12,
                              m20, m21, m22}};
  return matrix;
}

//! Make a 4x4 matrix
ZMatrix4x4 zMakeMat4x4(
    const float m00, const float m01, const float m02, const float m03,
    const float m10, const float m11, const float m12, const float m13,
    const float m20, const float m21, const float m22, const float m23,
    const float m30, const float m31, const float m32, const float m33)
{
  const ZMatrix4x4 matrix = {{m00, m01, m02, m03,
                              m10, m11, m12, m13,
                              m20, m21, m22, m23,
                              m30, m31, m32, m33}};
  return matrix;
}

//! Access to an element of a matrix
#define ZINVUL_ACCESS_MAT_ELEM_IMPL(matrix, row_size, row, column) \
    matrix->m_[row * row_size + column]

//! Access to an element of a 2x2 matrix
#define ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column) \
    ZINVUL_ACCESS_MAT_ELEM_IMPL(matrix, 2, row, column)

//! Return an element of a 2x2 matrix
float zGetMat2x2Elem(const __private ZMatrix2x2* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Return an element of a 2x2 matrix
float zGetMat2x2ElemL(const __local ZMatrix2x2* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Return an element of a 2x2 matrix
float zGetMat2x2ElemG(const __global ZMatrix2x2* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Assign a value to an element of a 2x2 matrix
void zSetMat2x2Elem(__private ZMatrix2x2* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column) = value;
}

//! Assign a value to an element of a 2x2 matrix
void zSetMat2x2ElemL(__local ZMatrix2x2* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column) = value;
}

//! Assign a value to an element of a 2x2 matrix
void zSetMat2x2ElemG(__global ZMatrix2x2* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, column) = value;
}

//! Return a row vector of a 2x2 matrix
#define ZINVUL_GET_MAT2x2_ROW_VEC(matrix, row) \
    zMakeFloat2(ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, 0), \
                ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, 1))

//! Return a row vector of a 2x2 matrix
float2 zGetMat2x2RowVec(const __private ZMatrix2x2* matrix,
    const size_t row)
{
  const float2 vector = ZINVUL_GET_MAT2x2_ROW_VEC(matrix, row);
  return vector;
}

//! Return a row vector of a 2x2 matrix
float2 zGetMat2x2RowVecL(const __local ZMatrix2x2* matrix,
    const size_t row)
{
  const float2 vector = ZINVUL_GET_MAT2x2_ROW_VEC(matrix, row);
  return vector;
}

//! Return a row vector of a 2x2 matrix
float2 zGetMat2x2RowVecG(const __global ZMatrix2x2* matrix,
    const size_t row)
{
  const float2 vector = ZINVUL_GET_MAT2x2_ROW_VEC(matrix, row);
  return vector;
}

//! Assign to a row vector of a 2x2 matrix
#define ZINVUL_SET_MAT2x2_ROW_VEC(matrix, row, vector) \
    { \
      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, 0) = vector.x; \
      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, row, 1) = vector.y; \
    }

//! Assign to a row vector of a 2x2 matrix
void zSetMat2x2RowVec(__private ZMatrix2x2* matrix,
    const size_t row,
    const float2 vector)
{
  ZINVUL_SET_MAT2x2_ROW_VEC(matrix, row, vector);
}

//! Assign to a row vector of a 2x2 matrix
void zSetMat2x2RowVecL(__local ZMatrix2x2* matrix,
    const size_t row,
    const float2 vector)
{
  ZINVUL_SET_MAT2x2_ROW_VEC(matrix, row, vector);
}

//! Assign to a row vector of a 2x2 matrix
void zSetMat2x2RowVecG(__global ZMatrix2x2* matrix,
    const size_t row,
    const float2 vector)
{
  ZINVUL_SET_MAT2x2_ROW_VEC(matrix, row, vector);
}

//! Return a column vector of a 2x2 matrix
#define ZINVUL_GET_MAT2x2_COLUMN_VEC(matrix, column) \
    zMakeFloat2(ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 0, column), \
                ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 1, column))

//! Return a column vector of a 2x2 matrix
float2 zGetMat2x2ColumnVec(const __private ZMatrix2x2* matrix,
    const size_t column)
{
  const float2 vector = ZINVUL_GET_MAT2x2_COLUMN_VEC(matrix, column);
  return vector;
}

//! Return a column vector of a 2x2 matrix
float2 zGetMat2x2ColumnVecL(const __local ZMatrix2x2* matrix,
    const size_t column)
{
  const float2 vector = ZINVUL_GET_MAT2x2_COLUMN_VEC(matrix, column);
  return vector;
}

//! Return a column vector of a 2x2 matrix
float2 zGetMat2x2ColumnVecG(const __global ZMatrix2x2* matrix,
    const size_t column)
{
  const float2 vector = ZINVUL_GET_MAT2x2_COLUMN_VEC(matrix, column);
  return vector;
}

//! Assign to a column vector of a 2x2 matrix
#define ZINVUL_SET_MAT2x2_COLUMN_VEC(matrix, column, vector) \
    { \
      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 0, column) = vector.x; \
      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 1, column) = vector.y; \
    }

//! Assign to a column vector of a 2x2 matrix
void zSetMat2x2ColumnVec(__private ZMatrix2x2* matrix,
    const size_t column,
    const float2 vector)
{
  ZINVUL_SET_MAT2x2_COLUMN_VEC(matrix, column, vector);
}

//! Assign to a column vector of a 2x2 matrix
void zSetMat2x2ColumnVecL(__local ZMatrix2x2* matrix,
    const size_t column,
    const float2 vector)
{
  ZINVUL_SET_MAT2x2_COLUMN_VEC(matrix, column, vector);
}

//! Assign to a column vector of a 2x2 matrix
void zSetMat2x2ColumnVecG(__global ZMatrix2x2* matrix,
    const size_t column,
    const float2 vector)
{
  ZINVUL_SET_MAT2x2_COLUMN_VEC(matrix, column, vector);
}

//! Access to an element of a 3x3 matrix
#define ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column) \
    ZINVUL_ACCESS_MAT_ELEM_IMPL(matrix, 3, row, column)

//! Return an element of a 3x3 matrix
float zGetMat3x3Elem(const __private ZMatrix3x3* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Return an element of a 3x3 matrix
float zGetMat3x3ElemL(const __local ZMatrix3x3* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Return an element of a 3x3 matrix
float zGetMat3x3ElemG(const __global ZMatrix3x3* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Assign a value to an element of a 3x3 matrix
void zSetMat3x3Elem(__private ZMatrix3x3* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column) = value;
}

//! Assign a value to an element of a 3x3 matrix
void zSetMat3x3ElemL(__local ZMatrix3x3* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column) = value;
}

//! Assign a value to an element of a 3x3 matrix
void zSetMat3x3ElemG(__global ZMatrix3x3* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, column) = value;
}

//! Return a row vector of a 3x3 matrix
#define ZINVUL_GET_MAT3x3_ROW_VEC(matrix, row) \
    zMakeFloat3(ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 0), \
                ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 1), \
                ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 2))

//! Return a row vector of a 3x3 matrix
float3 zGetMat3x3RowVec(const __private ZMatrix3x3* matrix,
    const size_t row)
{
  const float3 vector = ZINVUL_GET_MAT3x3_ROW_VEC(matrix, row);
  return vector;
}

//! Return a row vector of a 3x3 matrix
float3 zGetMat3x3RowVecL(const __local ZMatrix3x3* matrix,
    const size_t row)
{
  const float3 vector = ZINVUL_GET_MAT3x3_ROW_VEC(matrix, row);
  return vector;
}

//! Return a row vector of a 3x3 matrix
float3 zGetMat3x3RowVecG(const __global ZMatrix3x3* matrix,
    const size_t row)
{
  const float3 vector = ZINVUL_GET_MAT3x3_ROW_VEC(matrix, row);
  return vector;
}

//! Assign to a row vector of a 3x3 matrix
#define ZINVUL_SET_MAT3x3_ROW_VEC(matrix, row, vector) \
    { \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 0) = vector.x; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 1) = vector.y; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, row, 2) = vector.z; \
    }

//! Assign to a row vector of a 3x3 matrix
void zSetMat3x3RowVec(__private ZMatrix3x3* matrix,
    const size_t row,
    const float3 vector)
{
  ZINVUL_SET_MAT3x3_ROW_VEC(matrix, row, vector);
}

//! Assign to a row vector of a 3x3 matrix
void zSetMat3x3RowVecL(__local ZMatrix3x3* matrix,
    const size_t row,
    const float3 vector)
{
  ZINVUL_SET_MAT3x3_ROW_VEC(matrix, row, vector);
}

//! Assign to a row vector of a 3x3 matrix
void zSetMat3x3RowVecG(__global ZMatrix3x3* matrix,
    const size_t row,
    const float3 vector)
{
  ZINVUL_SET_MAT3x3_ROW_VEC(matrix, row, vector);
}

//! Return a column vector of a 3x3 matrix
#define ZINVUL_GET_MAT3x3_COLUMN_VEC(matrix, column) \
    zMakeFloat3(ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, column), \
                ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, column), \
                ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, column))

//! Return a column vector of a 3x3 matrix
float3 zGetMat3x3ColumnVec(const __private ZMatrix3x3* matrix,
    const size_t column)
{
  const float3 vector = ZINVUL_GET_MAT3x3_COLUMN_VEC(matrix, column);
  return vector;
}

//! Return a column vector of a 3x3 matrix
float3 zGetMat3x3ColumnVecL(const __local ZMatrix3x3* matrix,
    const size_t column)
{
  const float3 vector = ZINVUL_GET_MAT3x3_COLUMN_VEC(matrix, column);
  return vector;
}

//! Return a column vector of a 3x3 matrix
float3 zGetMat3x3ColumnVecG(const __global ZMatrix3x3* matrix,
    const size_t column)
{
  const float3 vector = ZINVUL_GET_MAT3x3_COLUMN_VEC(matrix, column);
  return vector;
}

//! Assign to a column vector of a 3x3 matrix
#define ZINVUL_SET_MAT3x3_COLUMN_VEC(matrix, column, vector) \
    { \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, column) = vector.x; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, column) = vector.y; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, column) = vector.z; \
    }

//! Assign to a column vector of a 3x3 matrix
void zSetMat3x3ColumnVec(__private ZMatrix3x3* matrix,
    const size_t column,
    const float3 vector)
{
  ZINVUL_SET_MAT3x3_COLUMN_VEC(matrix, column, vector);
}

//! Assign to a column vector of a 3x3 matrix
void zSetMat3x3ColumnVecL(__local ZMatrix3x3* matrix,
    const size_t column,
    const float3 vector)
{
  ZINVUL_SET_MAT3x3_COLUMN_VEC(matrix, column, vector);
}

//! Assign to a column vector of a 3x3 matrix
void zSetMat3x3ColumnVecG(__global ZMatrix3x3* matrix,
    const size_t column,
    const float3 vector)
{
  ZINVUL_SET_MAT3x3_COLUMN_VEC(matrix, column, vector);
}

//! Access to an element of a 3x3 matrix
#define ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column) \
    ZINVUL_ACCESS_MAT_ELEM_IMPL(matrix, 4, row, column)

//! Return an element of a 4x4 matrix
float zGetMat4x4Elem(__private ZMatrix4x4* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Return an element of a 4x4 matrix
float zGetMat4x4ElemL(__local ZMatrix4x4* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Return an element of a 4x4 matrix
float zGetMat4x4ElemG(__global ZMatrix4x4* matrix,
    const size_t row,
    const size_t column)
{
  const float value = ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column);
  return value;
}

//! Assign a value to an element of a 4x4 matrix
void zSetMat4x4Elem(__private ZMatrix4x4* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column) = value;
}

//! Assign a value to an element of a 4x4 matrix
void zSetMat4x4ElemL(__local ZMatrix4x4* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column) = value;
}

//! Assign a value to an element of a 4x4 matrix
void zSetMat4x4ElemG(__global ZMatrix4x4* matrix,
    const size_t row,
    const size_t column,
    const float value)
{
  ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, column) = value;
}

//! Return a row vector of a 3x3 matrix
#define ZINVUL_GET_MAT4x4_ROW_VEC(matrix, row) \
    zMakeFloat4(ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 0), \
                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 1), \
                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 2), \
                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 3))

//! Return a row vector of a 4x4 matrix
float4 zGetMat4x4RowVec(const __private ZMatrix4x4* matrix,
    const size_t row)
{
  const float4 vector = ZINVUL_GET_MAT4x4_ROW_VEC(matrix, row);
  return vector;
}

//! Return a row vector of a 4x4 matrix
float4 zGetMat4x4RowVecL(const __local ZMatrix4x4* matrix,
    const size_t row)
{
  const float4 vector = ZINVUL_GET_MAT4x4_ROW_VEC(matrix, row);
  return vector;
}

//! Return a row vector of a 4x4 matrix
float4 zGetMat4x4RowVecG(const __global ZMatrix4x4* matrix,
    const size_t row)
{
  const float4 vector = ZINVUL_GET_MAT4x4_ROW_VEC(matrix, row);
  return vector;
}

//! Assign to a row vector of a 4x4 matrix
#define ZINVUL_SET_MAT4x4_ROW_VEC(matrix, row, vector) \
    { \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 0) = vector.x; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 1) = vector.y; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 2) = vector.z; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, row, 3) = vector.w; \
    }

//! Assign to a row vector of a 4x4 matrix
void zSetMat4x4RowVec(__private ZMatrix4x4* matrix,
    const size_t row,
    const float4 vector)
{
  ZINVUL_SET_MAT4x4_ROW_VEC(matrix, row, vector);
}

//! Assign to a row vector of a 4x4 matrix
void zSetMat4x4RowVecL(__local ZMatrix4x4* matrix,
    const size_t row,
    const float4 vector)
{
  ZINVUL_SET_MAT4x4_ROW_VEC(matrix, row, vector);
}

//! Assign to a row vector of a 4x4 matrix
void zSetMat4x4RowVecG(__global ZMatrix4x4* matrix,
    const size_t row,
    const float4 vector)
{
  ZINVUL_SET_MAT4x4_ROW_VEC(matrix, row, vector);
}

//! Return a column vector of a 4x4 matrix
#define ZINVUL_GET_MAT4x4_COLUMN_VEC(matrix, column) \
    zMakeFloat4(ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, column), \
                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, column), \
                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, column), \
                ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, column))

//! Return a column vector of a 4x4 matrix
float4 zGetMat4x4ColumnVec(const __private ZMatrix4x4* matrix,
    const size_t column)
{
  const float4 vector = ZINVUL_GET_MAT4x4_COLUMN_VEC(matrix, column);
  return vector;
}

//! Return a column vector of a 4x4 matrix
float4 zGetMat4x4ColumnVecL(const __local ZMatrix4x4* matrix,
    const size_t column)
{
  const float4 vector = ZINVUL_GET_MAT4x4_COLUMN_VEC(matrix, column);
  return vector;
}

//! Return a column vector of a 4x4 matrix
float4 zGetMat4x4ColumnVecG(const __global ZMatrix4x4* matrix,
    const size_t column)
{
  const float4 vector = ZINVUL_GET_MAT4x4_COLUMN_VEC(matrix, column);
  return vector;
}

//! Assign to a column vector of a 4x4 matrix
#define ZINVUL_SET_MAT4x4_COLUMN_VEC(matrix, column, vector) \
    { \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, column) = vector.x; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, column) = vector.y; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, column) = vector.z; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, column) = vector.w; \
    }

//! Assign to a column vector of a 4x4 matrix
void zSetMat4x4ColumnVec(__private ZMatrix4x4* matrix,
    const size_t column,
    const float4 vector)
{
  ZINVUL_SET_MAT4x4_COLUMN_VEC(matrix, column, vector);
}

//! Assign to a column vector of a 4x4 matrix
void zSetMat4x4ColumnVecL(__local ZMatrix4x4* matrix,
    const size_t column,
    const float4 vector)
{
  ZINVUL_SET_MAT4x4_COLUMN_VEC(matrix, column, vector);
}

//! Assign to a column vector of a 4x4 matrix
void zSetMat4x4ColumnVecG(__global ZMatrix4x4* matrix,
    const size_t column,
    const float4 vector)
{
  ZINVUL_SET_MAT4x4_COLUMN_VEC(matrix, column, vector);
}

//! Initialize a 2x2 matrix
#define ZINVUL_INIT_MAT2x2_IMPL(matrix, m00, m01, \
                                        m10, m11) \
    { \
      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 0, 0) = m00; \
      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 0, 1) = m01; \
      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 1, 0) = m10; \
      ZINVUL_ACCESS_MAT2x2_ELEM_IMPL(matrix, 1, 1) = m11; \
    }

//! Initialize a 2x2 matrix
void zInitMat2x2(__private ZMatrix2x2* matrix,
    const float m00, const float m01,
    const float m10, const float m11)
{
  ZINVUL_INIT_MAT2x2_IMPL(matrix, m00, m01,
                                  m10, m11);
}

//! Initialize a 2x2 matrix
void zInitMat2x2L(__local ZMatrix2x2* matrix,
    const float m00, const float m01,
    const float m10, const float m11)
{
  ZINVUL_INIT_MAT2x2_IMPL(matrix, m00, m01,
                                  m10, m11);
}

//! Initialize a 2x2 matrix
void zInitMat2x2G(__global ZMatrix2x2* matrix,
    const float m00, const float m01,
    const float m10, const float m11)
{
  ZINVUL_INIT_MAT2x2_IMPL(matrix, m00, m01,
                                  m10, m11);
}

//! Initialize a 3x3 matrix
#define ZINVUL_INIT_MAT3x3_IMPL(matrix, m00, m01, m02, \
                                        m10, m11, m12, \
                                        m20, m21, m22) \
    { \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, 0) = m00; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, 1) = m01; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 0, 2) = m02; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, 0) = m10; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, 1) = m11; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 1, 2) = m12; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, 0) = m20; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, 1) = m21; \
      ZINVUL_ACCESS_MAT3x3_ELEM_IMPL(matrix, 2, 2) = m22; \
    }

//! Initialize a 3x3 matrix
void zInitMat3x3(__private ZMatrix3x3* matrix,
    const float m00, const float m01, const float m02,
    const float m10, const float m11, const float m12,
    const float m20, const float m21, const float m22)
{
  ZINVUL_INIT_MAT3x3_IMPL(matrix, m00, m01, m02,
                                  m10, m11, m12,
                                  m20, m21, m22);
}

//! Initialize a 3x3 matrix
void zInitMat3x3L(__local ZMatrix3x3* matrix,
    const float m00, const float m01, const float m02,
    const float m10, const float m11, const float m12,
    const float m20, const float m21, const float m22)
{
  ZINVUL_INIT_MAT3x3_IMPL(matrix, m00, m01, m02,
                                  m10, m11, m12,
                                  m20, m21, m22);
}

//! Initialize a 3x3 matrix
void zInitMat3x3G(__global ZMatrix3x3* matrix,
    const float m00, const float m01, const float m02,
    const float m10, const float m11, const float m12,
    const float m20, const float m21, const float m22)
{
  ZINVUL_INIT_MAT3x3_IMPL(matrix, m00, m01, m02,
                                  m10, m11, m12,
                                  m20, m21, m22);
}

//! Initialize a 4x4 matrix
#define ZINVUL_INIT_MAT4x4_IMPL(matrix, m00, m01, m02, m03, \
                                        m10, m11, m12, m13, \
                                        m20, m21, m22, m23, \
                                        m30, m31, m32, m33) \
    { \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, 0) = m00; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, 1) = m01; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, 2) = m02; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 0, 3) = m03; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, 0) = m10; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, 1) = m11; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, 2) = m12; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 1, 3) = m13; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, 0) = m20; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, 1) = m21; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, 2) = m22; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 2, 3) = m23; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, 0) = m30; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, 1) = m31; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, 2) = m32; \
      ZINVUL_ACCESS_MAT4x4_ELEM_IMPL(matrix, 3, 3) = m33; \
    }

//! Initialize a 4x4 matrix
void zInitMat4x4(__private ZMatrix4x4* matrix,
    const float m00, const float m01, const float m02, const float m03,
    const float m10, const float m11, const float m12, const float m13,
    const float m20, const float m21, const float m22, const float m23,
    const float m30, const float m31, const float m32, const float m33)
{
  ZINVUL_INIT_MAT4x4_IMPL(matrix, m00, m01, m02, m03,
                                  m10, m11, m12, m13,
                                  m20, m21, m22, m23,
                                  m30, m31, m32, m33);
}

//! Initialize a 4x4 matrix
void zInitMat4x4L(__local ZMatrix4x4* matrix,
    const float m00, const float m01, const float m02, const float m03,
    const float m10, const float m11, const float m12, const float m13,
    const float m20, const float m21, const float m22, const float m23,
    const float m30, const float m31, const float m32, const float m33)
{
  ZINVUL_INIT_MAT4x4_IMPL(matrix, m00, m01, m02, m03,
                                  m10, m11, m12, m13,
                                  m20, m21, m22, m23,
                                  m30, m31, m32, m33);
}

//! Initialize a 4x4 matrix
void zInitMat4x4G(__global ZMatrix4x4* matrix,
    const float m00, const float m01, const float m02, const float m03,
    const float m10, const float m11, const float m12, const float m13,
    const float m20, const float m21, const float m22, const float m23,
    const float m30, const float m31, const float m32, const float m33)
{
  ZINVUL_INIT_MAT4x4_IMPL(matrix, m00, m01, m02, m03,
                                  m10, m11, m12, m13,
                                  m20, m21, m22, m23,
                                  m30, m31, m32, m33);
}

#endif /* ZINVUL_MATRIX_CL */
