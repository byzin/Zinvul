/*!
  \file hash_engine.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
  template <size_t kN>
  static ResultType hash(const char(&seed)[kN]) noexcept;

  //! Compute a hash value
  static ResultType hash(const int8b* seed, const size_t n) noexcept;

  //! Compute a hash value
  static ResultType hash(const uint8b* seed, const size_t n) noexcept;

  //! Compute a hash value
  template <typename UInteger>
  static ResultType hash(const UInteger seed) noexcept;
};

} // namespace zinvul

#include "hash_engine-inl.cl"

#endif // ZINVUL_HASH_ENGINE_CL
