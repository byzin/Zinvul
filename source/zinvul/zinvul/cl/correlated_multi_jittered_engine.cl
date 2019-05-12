/*!
  \file correlated_multi_jittered_engine.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CORRELATED_MULTI_JITTERED_ENGINE_CL
#define ZINVUL_CORRELATED_MULTI_JITTERED_ENGINE_CL

// Zinvul
#include "types.cl"
#include "utility.cl"

namespace zinvul {

/*!
  */
template <uint32b kRootN>
class CorrelatedMultiJitteredEngine
{
 public:
  //! Generate a [0, 1) float random number
  template <typename Float>
  static Float generate1D(const uint32b s, const uint32b p) noexcept;

  //! Generate a [0, 1) float random numbers
  template <typename Float2>
  static Float2 generate2D(uint32b s, const uint32b p) noexcept;

  //! Return the period
  static constexpr size_t getPeriod() noexcept;

  //! Return the root of the period
  static constexpr size_t getRootPeriod() noexcept;

  //! Check if a specified sample (0 base count) is the end of period
  static constexpr bool isEndOfPeriod(const uint32b sample) noexcept;

 private:
  static_assert(1u < kRootN, "The kRootN must be greater than 1.");
  static_assert((kRootN & (kRootN - 1)) == 0, "The kRootN must be power of 2.");


  //! Hash the i value
  template <typename UInteger>
  static UInteger hash(UInteger i, const UInteger p) noexcept;

  //! Permutation
  template <uint32b l, typename UInteger>
  static UInteger permute(UInteger i, const UInteger p) noexcept;

  //! The implementation of permutation
  template <uint32b w, typename UInteger>
  static UInteger permuteImpl(UInteger i, const UInteger p) noexcept;
};

// Type aliases
using CmjN16 = CorrelatedMultiJitteredEngine<4>;
using CmjN64 = CorrelatedMultiJitteredEngine<8>;
using CmjN81 = CorrelatedMultiJitteredEngine<9>;
using CmjN256 = CorrelatedMultiJitteredEngine<16>;

} // namespace zinvul

#include "correlated_multi_jittered_engine-inl.cl"

#endif /* ZINVUL_CORRELATED_MULTI_JITTERED_ENGINE_CL */
