/*!
  \file types.cl
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPES_CL
#define ZINVUL_TYPES_CL

// Type aliases
#ifndef ZINVUL_CPP_CL
// Integer types
typedef char int8b;
typedef short int16b;
typedef int int32b;
typedef long int64b;
// Unsigned integer types
typedef unsigned char uint8b;
typedef unsigned short uint16b;
typedef unsigned int uint32b;
typedef unsigned long uint64b;
#endif // ZINVUL_CPP_CL

#endif /* ZINVUL_TYPES_CL */
