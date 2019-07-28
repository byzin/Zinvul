/*!
  \file hash_engine-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_HASH_ENGINE_INL_CL
#define ZINVUL_HASH_ENGINE_INL_CL

#include "hash_engine.cl"
// Zinvul
#include "array.cl"
#include "limits.cl"
#include "types.cl"
#include "utility.cl"

namespace zinvul {

template <typename HashClass, typename ResultType> template <size_t kN> inline
ResultType HashEngine<HashClass, ResultType>::hash(
    const char(&seed)[kN]) noexcept
{
  return HashClass::hashValue(seed, kN - 1);
}

/*!
  */
template <typename HashClass, typename ResultType> inline
ResultType HashEngine<HashClass, ResultType>::hash(const int8b* seed,
                                                   const size_t n) noexcept
{
  return HashClass::hashValue(seed, n);
}

/*!
  */
template <typename HashClass, typename ResultType> inline
ResultType HashEngine<HashClass, ResultType>::hash(const uint8b* seed,
                                                   const size_t n) noexcept
{
  return HashClass::hashValue(seed, n);
}

/*!
  */
template <typename HashClass, typename ResultType>
template <typename UInteger> inline
ResultType HashEngine<HashClass, ResultType>::hash(
    const UInteger seed) noexcept
{
  // Make a seed array
  constexpr size_t n = sizeof(UInteger);
  Array<uint8b, n> seed_array;
  for (size_t i = 0; i < n; ++i) {
    constexpr auto mask = static_cast<UInteger>(NumericLimits<uint8b>::max());
    seed_array[i] = cast<uint8b>(mask & (seed >> (8u * i)));
  }
  // Hash the seed
  return hash(seed_array.data(), n);
}

} // namespace zinvul

#endif // ZINVUL_HASH_ENGINE_INL_CL
