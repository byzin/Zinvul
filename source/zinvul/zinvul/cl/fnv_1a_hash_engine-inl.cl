/*!
  \file fnv_1a_hash_engine-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_FNV_1A_HASH_ENGINE_INL_CL
#define ZINVUL_FNV_1A_HASH_ENGINE_INL_CL

#include "fnv_1a_hash_engine.cl"
// Zinvul
#include "hash_engine.cl"
#include "types.cl"

namespace zinvul {

namespace inner {

template <typename ResultType> struct Fnv1aHashEngineImpl;

template <>
struct Fnv1aHashEngineImpl<uint32b>
{
  static constexpr uint32b prime() noexcept
  {
    uint32b p = 16777619u;
    return p;
  }

  static constexpr uint32b offset() noexcept
  {
    uint32b o = 2166136261u;
    return o;
  }
};

template <>
struct Fnv1aHashEngineImpl<uint64b>
{
  static constexpr uint64b prime() noexcept
  {
    uint64b p = static_cast<uint64b>(1099511628211ull);
    return p;
  }

  static constexpr uint64b offset() noexcept
  {
    uint64b o = static_cast<uint64b>(14695981039346656037ull);
    return o;
  }
};

} // namespace inner

/*!
  */
template <typename ResultType> template <typename Int8> inline
ResultType Fnv1aHashEngine<ResultType>::hashValue(const Int8* inputs,
                                                  const size_t n) noexcept
{
  Private<ResultType> x = inner::Fnv1aHashEngineImpl<ResultType>::offset();
  for (size_t i = 0; i < n; ++i) {
    x = (x ^ cast<ResultType>(inputs[i])) *
        inner::Fnv1aHashEngineImpl<ResultType>::prime();
  }
  return x;
}

} // namespace zinvul

#endif // ZINVUL_FNV_1A_HASH_ENGINE_INL_CL
