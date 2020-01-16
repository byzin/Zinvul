/*!
  \file hash_engine.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_HASH_ENGINE_CL
#define ZINVUL_HASH_ENGINE_CL

// Zinvul
#include "types.cl"

namespace zinvul {

/*!
  */
template <typename HashClass, typename ResultType>
class HashEngine
{
 public:
  //! Compute a hash value
  template <typename Integer>
  static ResultType  hash(const Integer seed) noexcept;

  //! Compute a hash value
  template <typename IntegerPtr>
  static ResultType  hash(const IntegerPtr seed, const size_t n) noexcept;
};

} // namespace zinvul

#include "hash_engine-inl.cl"

#endif // ZINVUL_HASH_ENGINE_CL
