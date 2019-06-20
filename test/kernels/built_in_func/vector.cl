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
#include "zinvul/cl/types.cl"
#include "zinvul/cl/type_traits.cl"
#include "zinvul/cl/utility.cl"
#include "zinvul/cl/vector_data.cl"

using namespace zinvul;

// Forward declaration

/*!
  */
__kernel void testInt32bVectorData(ConstGlobalPtr<int32b> inputs,
    GlobalPtr<int32b> outputs,
    const uint32b resolution)
{
//  const uint32b index = getGlobalIdX();
//  if (index < resolution) {
//    size_t offset = 10u * index;
//    {
//      using VData = VectorData<int32b>;
//      int32b v = VData::load(offset, inputs);
//      v = 2 * v;
//      VData::store(v, offset, outputs);
//      offset += VData::size();
//    }
//    {
//      using VData = VectorData<int2>;
//      int2 v = VData::load(offset, inputs);
//      v = 2 * v;
//      VData::store(v, offset, outputs);
//      offset += VData::size();
//    }
//    {
//      using VData = VectorData<int3>;
//      int3 v = VData::load(offset, inputs);
//      v = 2 * v;
//      VData::store(v, offset, outputs);
//      offset += VData::size();
//    }
//    {
//      using VData = VectorData<int4>;
//      int4 v = VData::load(offset, inputs);
//      v = 2 * v;
//      VData::store(v, offset, outputs);
//      offset += VData::size();
//    }
//  }
}

#endif /* ZINVUL_BUILT_IN_FUNC_TEST_VECTOR_CL */
