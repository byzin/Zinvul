/*!
  \file hash_engine-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
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
#include "type_traits.cl"
#include "utility.cl"

namespace zinvul {

/*!
  */
template <typename HashClass, typename ResultType>
template <typename Integer> inline
ResultType HashEngine<HashClass, ResultType>::hash(const Integer seed) noexcept
{
  using ASpaceInfo = AddressSpaceInfo<Integer>;
  static_assert(!ASpaceInfo::isPointer(), "The Integer is pointer.");
  using IType = typename ASpaceInfo::DataType;

  // Make a seed array
  constexpr size_t n = sizeof(IType);
  Array<uint8b, n> seed_array;
  for (size_t i = 0; i < n; ++i) {
    constexpr auto mask = static_cast<IType>(NumericLimits<uint8b>::max());
    seed_array[i] = static_cast<uint8b>(mask & (seed >> (8u * i)));
  }
  // Hash the seed
  const auto result = HashClass::hashValue(seed_array.data(), n);
  return result;
}

/*!
  */
template <typename HashClass, typename ResultType>
template <typename Integer8Ptr> inline
ResultType HashEngine<HashClass, ResultType>::hash(
    const Integer8Ptr seed,
    const size_t n) noexcept
{
  using ASpaceInfo = AddressSpaceInfo<Integer8Ptr>;
  static_assert(ASpaceInfo::isPointer(), "The IntegerPtr isn't pointer.");
  using IType = typename ASpaceInfo::DataType;
  static_assert(sizeof(IType) == 1, "The IntegePtr isn't 8bit integer pointer.");
  // Hash the seed
  const auto result = HashClass::hashValue(seed, n);
  return result;
}

} // namespace zinvul

#endif // ZINVUL_HASH_ENGINE_INL_CL
