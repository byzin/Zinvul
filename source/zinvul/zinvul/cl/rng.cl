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

/*!
  */
float zMapTo01F(const uint32b x)
{
  const float f = (float)(x >> zExponentBitSizeF) * 5.960464478e-08f; // 2^(-24)
  return f;
}

// Correlated Multi-Jittered

/*!
  */
uint32b zCmjPermuteImpl(uint32b i, const uint32b p, const uint32b w)
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

/*!
  */
uint32b zCmjMakeWMask(uint32b w)
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
uint32b zCmjPermute(uint32b i, const uint32b p, const uint32b l)
{
  const int32b period_is_power_of_2 = (1u < l) && ((l & (l - 1u)) == 0u);
  if (period_is_power_of_2) {
    // fast case
    const uint32b w = l - 1u;
    i = zCmjPermuteImpl(i, p, w);
    i = (i + p) & w;
  }
  else {
    // slow case
    const uint32b w = zCmjMakeWMask(l - 1u);
    do {
      i = zCmjPermuteImpl(i, p, w);
    } while (l <= i);
    i = (i + p) % l;
  }
  return i;
}

/*!
  */
uint32b zCmjHash(uint32b i, const uint32b p)
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
float zCmjDraw1D(const uint32b s, const uint32b p, const uint32b root_n)
{
  const uint32b period = root_n * root_n;

  const uint32b sx = zCmjPermute(s, p * 0x68bc21ebu, period);
  float x = zMapTo01F(zCmjHash(s, p * 0x967a889bu));

  // Random jitter
  const float inv_n = 1.0f / (float)period;
  x = inv_n * ((float)sx + x);

  return x;
}

/*!
  */
float2 zCmjDraw2D(uint32b s, const uint32b p, const uint32b root_n)
{
  const uint32b period = root_n * root_n;

  s = zCmjPermute(s, p * 0x51633e2du, period);

  const uint32b s_div = s / root_n;
  const uint32b s_mod = s  - s_div * root_n;

  const uint32b sx = zCmjPermute(s_mod, p * 0x68bc21ebu, root_n);
  const uint32b sy = zCmjPermute(s_div, p * 0x02e5be93u, root_n);
  float x = zMapTo01F(zCmjHash(s, p * 0x967a889bu));
  float y = zMapTo01F(zCmjHash(s, p * 0x368cc8b7u));

  // Random jitter
  const float inv_rn = 1.0f / (float)root_n;
  x = inv_rn * ((float)sx + inv_rn * ((float)sy + x));
  y = (inv_rn * inv_rn) * ((float)s + y);

  return zMakeFloat2(x, y);
}
