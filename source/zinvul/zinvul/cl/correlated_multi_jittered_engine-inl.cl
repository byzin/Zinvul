/*!
  \file correlated_multi_jittered_engine-inl.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_CORRELATED_MULTI_JITTERED_ENGINE_INL_CL
#define ZINVUL_CORRELATED_MULTI_JITTERED_ENGINE_INL_CL

#include "correlated_multi_jittered_engine.cl"
// Zinvul
#include "floating_point.cl"
#include "limits.cl"
#include "types.cl"
#include "utility.cl"

namespace zinvul {

/*!
  */
template <uint32b kRootN> template <typename Float> inline
Float CorrelatedMultiJitteredEngine<kRootN>::generate1D(
    const uint32b s,
    const uint32b p) noexcept
{
  using FPoint = FloatingPointFromBytes<sizeof(Float)>;

  constexpr uint32b n = getPeriod();

  const uint32b sx = permute<n>(s, p * 0x68bc21ebu);
  const uint32b h = hash(s, p * 0x967a889bu);
  auto x = FPoint::mapTo01(h);
//
//  // Random jitter
//  constexpr auto inv_n = static_cast<Float>(1.0) / static_cast<Float>(n);
//  x = inv_n * (static_cast<Float>(sx) + x);

  return x;
}

/*!
  */
template <uint32b kRootN> template <typename Float2> inline
Float2 CorrelatedMultiJitteredEngine<kRootN>::generate2D(
    uint32b s,
    const uint32b p) noexcept
{
  return float2{0.0f, 0.0f};
//  using Float = decltype(Float2{}.x);
//  using FPoint = FloatingPointFromBytes<sizeof(Float)>;
//
//  constexpr uint32b n = getPeriod();
//
//  s = permute<n>(s, p * 0x51633e2du);
//
//  const uint2 s2{s / kRootN, s % kRootN};
//  const uint2 sx = permute<kRootN>(s2, uint2{p * 0x68bc21ebu, p * 0x02e5be93u});
//  auto x = FPoint::mapTo01(hash(uint2{s, s},
//                                uint2{p * 0x967a889bu, p * 0x368cc8b7u}));
//
//  // Random jitter
//  constexpr auto inv_n = static_cast<Float>(1.0) / static_cast<Float>(n);
//  constexpr auto inv_root_n = static_cast<Float>(1.0) / static_cast<Float>(kRootN);
//  x.x = inv_root_n *
//        (static_cast<Float>(sx.x) + inv_root_n * (static_cast<Float>(sx.y) + x));
//  x.y = inv_n * (static_cast<Float>(s) + x.y);
//
//  return x;
}

/*!
  */
template <uint32b kRootN> inline
constexpr size_t CorrelatedMultiJitteredEngine<kRootN>::getPeriod() noexcept
{
  const size_t period = kRootN * kRootN;
  return period;
}

/*!
  */
template <uint32b kRootN> inline
constexpr bool CorrelatedMultiJitteredEngine<kRootN>::isEndOfPeriod(
    const uint32b sample) noexcept
{
  const uint32b end_of_period = getPeriod() - 1u;
  return sample == end_of_period;
}

/*!
  */
template <uint32b kRootN> template <typename UInteger> inline
UInteger CorrelatedMultiJitteredEngine<kRootN>::hash(UInteger i,
                                                     const UInteger p) noexcept
{
  i ^= p;
  i ^= i >> 17u;
  i ^= i >> 10u;
  i *= 0xb36534e5u;
  i ^= i >> 12u;
  i ^= i >> 21u;
  i *= 0x93fc4795u;
  i ^= 0xdf6e307fu;
  i ^= i >> 17u;
  i *= 1u | p >> 18u;
  return i;
}

/*!
  */
template <uint32b kRootN> template <typename UInteger> inline
constexpr UInteger CorrelatedMultiJitteredEngine<kRootN>::makeWMask(
    UInteger w) noexcept
{
  w |= w >> 1u;
  w |= w >> 2u;
  w |= w >> 4u;
  w |= w >> 8u;
  w |= w >> 16u;
  return w;
}

/*!
  */
template <uint32b kRootN> template <uint32b l, typename UInteger> inline
UInteger CorrelatedMultiJitteredEngine<kRootN>::permute(
    UInteger i,
    const UInteger p) noexcept
{
  constexpr bool is_power_of_2 = (1u < l) && (0u < (l & (l - 1u)));
  constexpr uint32b w = makeWMask(l - 1u);
  if (is_power_of_2) {
    // fast case
    i = permuteImpl<w>(i, p);
    i = (i + p) & w;
  }
  else {
    // slow case
    do {
      i = permuteImpl<w>(i, p);
    } while (l <= i);
    i = (i + p) % l;
  }
  return i;
}

/*!
  */
template <uint32b kRootN> template <uint32b w, typename UInteger> inline
UInteger CorrelatedMultiJitteredEngine<kRootN>::permuteImpl(
    UInteger i,
    const UInteger p) noexcept
{
  i ^= p;
  i *= 0xe170893du;
  i ^= p >> 16u;
  i ^= (i & w) >> 4u;
  i ^= p >> 8u;
  i *= 0x0929eb3fu;
  i ^= p >> 23u;
  i ^= (i & w) >> 1u;
  i *= 1u | p >> 27u;
  i *= 0x6935fa69u;
  i ^= (i & w) >> 11u;
  i *= 0x74dcb303u;
  i ^= (i & w) >> 2u;
  i *= 0x9e501cc3u;
  i ^= (i & w) >> 2u;
  i *= 0xc860a3dfu;
  i &= w;
  i ^= i >> 5u;
  return i;
}

} // namespace zinvul

#endif /* ZINVUL_CORRELATED_MULTI_JITTERED_ENGINE_INL_CL */
