/*!
  \file utility.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_UTILITY_CL
#define ZINVUL_UTILITY_CL

#include "types.cl"

/*!
  */
uint getGlobalId(const uint dimension)
{
  const uint id = get_global_id(dimension);
  return id;
}

/*!
  */
uint getGlobalIdX()
{
  const uint id = get_global_id(0);
  return id;
}

/*!
  */
uint getGlobalIdY()
{
  const uint id = get_global_id(1);
  return id;
}

/*!
  */
uint getGlobalIdZ()
{
  const uint id = get_global_id(2);
  return id;
}

#endif /* ZINVUL_UTILITY_CL */
