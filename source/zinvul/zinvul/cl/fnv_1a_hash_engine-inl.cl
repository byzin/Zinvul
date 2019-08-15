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

/*!
  */
template <typename ResultType> template <typename Int8> inline
ResultType Fnv1aHashEngine<ResultType>::hashValue(ConstGenericPtr<Int8> inputs,
                                                  const size_t n) noexcept
{
  auto x = offset();
  for (size_t i = 0; i < n; ++i)
    x = (x ^ static_cast<ResultType>(inputs[i])) * prime();
  return x;
}

template <typename ResultType> inline
constexpr auto Fnv1aHashEngine<ResultType>::prime() noexcept -> ResultType
{
  if constexpr (sizeof(ResultType) == 4) {
    const ResultType p = static_cast<ResultType>(16777619u);
    return p;
  }
  else if constexpr (sizeof(ResultType) == 8) {
    const ResultType p = static_cast<ResultType>(1099511628211ull);
    return p;
  }
  else {
    static_assert(sizeof(ResultType) == 0, "Unsupported result type is specified.");
  }
}

template <typename ResultType> inline
constexpr auto Fnv1aHashEngine<ResultType>::offset() noexcept -> ResultType
{
  if constexpr (sizeof(ResultType) == 4) {
    const ResultType o = static_cast<ResultType>(2166136261u);
    return o;
  }
  else if constexpr (sizeof(ResultType) == 8) {
    const ResultType o = static_cast<ResultType>(14695981039346656037ull);
    return o;
  }
  else {
    static_assert(sizeof(ResultType) == 0, "Unsupported result type is specified.");
  }
}

} // namespace zinvul

#endif // ZINVUL_FNV_1A_HASH_ENGINE_INL_CL
