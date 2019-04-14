/*!
  \file rng.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "math.cl"
#include "types.cl"
#include "utility.cl"

///*!
//  */
//float zMapTo01F(const uint32b x)
//{
//  const float f = (float)(x >> zExponentBitSizeF) * 5.960464478e-08f; // 2^(-24)
//  return f;
//}
//
///*!
//  */
//float2 zMapTo01F2(uint2 x)
//{
//  x = x >> (uint32b)zExponentBitSizeF;
//  const float2 f = zMakeFloat2((float)x.x, (float)x.y) * 5.960464478e-08f;
//  return f;
//}
//
//// Correlated Multi-Jittered
//
//#define CMJ_PERMUTE_IMPL(i, p, w) \
//  i ^= p; \
//  i *= 0xe170893du; \
//  i ^= p >> 16u; \
//  i ^= (i & w) >> 4u; \
//  i ^= p >> 8u; \
//  i *= 0x0929eb3fu; \
//  i ^= p >> 23u; \
//  i ^= (i & w) >> 1u; \
//  i *= 1u | p >> 27u; \
//  i *= 0x6935fa69u; \
//  i ^= (i & w) >> 11u; \
//  i *= 0x74dcb303u; \
//  i ^= (i & w) >> 2u; \
//  i *= 0x9e501cc3u; \
//  i ^= (i & w) >> 2u; \
//  i *= 0xc860a3dfu; \
//  i &= w; \
//  i ^= i >> 5u
//
///*!
//  */
//uint32b zCmjMakeWMask(uint32b w)
//{
//  w |= w >> 1u;
//  w |= w >> 2u;
//  w |= w >> 4u;
//  w |= w >> 8u;
//  w |= w >> 16u;
//  return w;
//}
//
//#define CMJ_PERMUTE(i, p, l) \
//  zAssert((1u < l) && ((l & (l - 1u)) == 0u), "The l should be power of 2."); \
//  const uint32b w = l - 1u; \
//  i = CMJ_PERMUTE_IMPL(i, p, w); \
//  i = (i + p) & w
//
///*!
//  */
//uint32b zCmjPermute(uint32b i, const uint32b p, const uint32b l)
//{
//  CMJ_PERMUTE(i, p, l);
//  return i;
//}
//
///*!
//  */
//uint2 zCmjPermute2(uint2 i, const uint2 p, const uint32b l)
//{
//  CMJ_PERMUTE(i, p, l);
//  return i;
//}
//
//#define CMJ_HASH(i, p) \
//  i ^= p; \
//  i ^= i >> 17u; \
//  i ^= i >> 10u; \
//  i *= 0xb36534e5u; \
//  i ^= i >> 12u; \
//  i ^= i >> 21u; \
//  i *= 0x93fc4795u; \
//  i ^= 0xdf6e307fu; \
//  i ^= i >> 17u; \
//  i *= 1u | p >> 18u
//
///*!
//  */
//uint32b zCmjHash(uint32b i, const uint32b p)
//{
//  CMJ_HASH(i, p);
//  return i;
//}
//
///*!
//  */
//uint2 zCmjHash2(const uint32b i1, const uint2 p)
//{
//  uint2 i = zBroadcastU2(i1);
//  CMJ_HASH(i, p);
//  return i;
//}
//
///*!
//  */
//float zCmjDraw1D(const uint32b s, const uint32b p, const uint32b root_n)
//{
//  const uint32b period = root_n * root_n;
//
//  const uint32b sx = zCmjPermute(s, p * 0x68bc21ebu, period);
//  float x = zMapTo01F(zCmjHash(s, p * 0x967a889bu));
//
//  // Random jitter
//  const float inv_n = 1.0f / (float)period;
//  x = inv_n * ((float)sx + x);
//
//  return x;
//}
//
///*!
//  */
//float2 zCmjDraw2D(uint32b s, const uint32b p, const uint32b root_n)
//{
//  const uint32b period = root_n * root_n;
//
//  s = zCmjPermute(s, p * 0x51633e2du, period);
//  uint2 sx = zMakeUInt2(s % root_n, s / root_n);
//  sx = zCmjPermute2(sx, zMakeUInt2(p * 0x68bc21ebu, p * 0x02e5be93u), root_n);
//  float2 x = zMapTo01F2(zCmjHash2(s, zMakeUInt2(p * 0x967a889bu, p * 0x368cc8b7u)));
//
//  // Random jitter
//  const float inv_rn = 1.0f / (float)root_n;
//  x.x = inv_rn * ((float)sx.x + inv_rn * ((float)sx.y + x.x));
//  x.y = (inv_rn * inv_rn) * ((float)s + x.y);
//
//  return x;
//}
