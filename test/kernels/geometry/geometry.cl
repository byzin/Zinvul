/*!
  \file geometry.cl
  \author Sho Ikeda
  */

#ifndef ZINVUL_GEOMETRY_TEST_GEOMETRY_CL
#define ZINVUL_GEOMETRY_TEST_GEOMETRY_CL

// Zinvul
#include "zinvul/cl/correlated_multi_jittered_engine.cl"
#include "zinvul/cl/geometry.cl"
#include "zinvul/cl/limits.cl"
#include "zinvul/cl/types.cl"
#include "zinvul/cl/utility.cl"

using zinvul::int8b;
using zinvul::int16b;
using zinvul::int32b;
using zinvul::int64b;
using zinvul::uint8b;
using zinvul::uint16b;
using zinvul::uint32b;
using zinvul::uint64b;
using zinvul::GlobalPtr;
using zinvul::ConstGlobalPtr;
using zinvul::ConstantPtr;
using zinvul::ConstConstantPtr;
using zinvul::Local;
using zinvul::LocalPtr;
using zinvul::ConstLocalPtr;
using zinvul::Private;
using zinvul::PrivatePtr;
using zinvul::ConstPrivatePtr;
using zinvul::cast;
using zinvul::treatAs;

// Forward declaration
__kernel void testCross3(GlobalPtr<float3> parameters,
    GlobalPtr<float3> results,
    const uint32b resolution);
__kernel void testCross4(GlobalPtr<float4> parameters,
    GlobalPtr<float4> results,
    const uint32b resolution);
__kernel void testDot3(GlobalPtr<float3> parameters,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testDot4(GlobalPtr<float4> parameters,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testDistance3(GlobalPtr<float3> parameters,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testDistance4(GlobalPtr<float4> parameters,
    GlobalPtr<float> results,
    const uint32b resolution);
__kernel void testNormalize3(GlobalPtr<float3> parameters,
    GlobalPtr<float3> results,
    const uint32b resolution);
__kernel void testNormalize4(GlobalPtr<float4> parameters,
    GlobalPtr<float4> results,
    const uint32b resolution);

namespace test {

template <typename Float>
Float rng(const uint32b s, const uint32b p) noexcept;

template <typename Float>
Float rng(const uint32b s, const uint32b p) noexcept
{
  using Cmj = zinvul::CmjN256;
  constexpr Float t = zinvul::NumericLimits<Float>::epsilon();
  constexpr Float k = static_cast<Float>(100000.0);
  const auto u = Cmj::generate1D<Float>(s, p) - static_cast<Float>(0.5);
  const auto r = t + k * (u * u);
//  const auto r = t + k * u;
  return r;
}

template <typename Float, typename Float3>
void testCross3(GlobalPtr<Float3> parameters,
    GlobalPtr<Float3> results,
    const uint32b index) noexcept
{
  const uint32b p = 123'456'789u + index;
  const Float3 a{test::rng<Float>(0, p),
                 test::rng<Float>(1, p),
                 test::rng<Float>(2, p)};
  const Float3 b{test::rng<Float>(3, p),
                 test::rng<Float>(4, p),
                 test::rng<Float>(5, p)};
  parameters[2 * index    ] = a;
  parameters[2 * index + 1] = b;
  results[2 * index    ] = zinvul::cross(a, b);
  results[2 * index + 1] = zinvul::cross(b, a);
}

template <typename Float, typename Float4>
void testCross4(GlobalPtr<Float4> parameters,
    GlobalPtr<Float4> results,
    const uint32b index) noexcept
{
  const uint32b p = 123'456'789u + index;
  const Float4 a{test::rng<Float>(0, p),
                 test::rng<Float>(1, p),
                 test::rng<Float>(2, p),
                 test::rng<Float>(3, p)};
  const Float4 b{test::rng<Float>(4, p),
                 test::rng<Float>(5, p),
                 test::rng<Float>(6, p),
                 test::rng<Float>(7, p)};
  parameters[2 * index    ] = a;
  parameters[2 * index + 1] = b;
  results[2 * index    ] = zinvul::cross(a, b);
  results[2 * index + 1] = zinvul::cross(b, a);
}

template <typename Float, typename Float3>
void testDot3(GlobalPtr<Float3> parameters,
    GlobalPtr<Float> results,
    const uint32b index) noexcept
{
  const uint32b p = 123'456'789u + index;
  const Float3 a{test::rng<Float>(0, p),
                 test::rng<Float>(1, p),
                 test::rng<Float>(2, p)};
  const Float3 b{test::rng<Float>(3, p),
                 test::rng<Float>(4, p),
                 test::rng<Float>(5, p)};
  parameters[2 * index    ] = a;
  parameters[2 * index + 1] = b;
  results[2 * index    ] = zinvul::dot(a, b);
  results[2 * index + 1] = zinvul::Geometry::dot3(a, b);
}


template <typename Float, typename Float4>
void testDot4(GlobalPtr<Float4> parameters,
    GlobalPtr<Float> results,
    const uint32b index) noexcept
{
  const uint32b p = 123'456'789u + index;
  const Float4 a{test::rng<Float>(0, p),
                 test::rng<Float>(1, p),
                 test::rng<Float>(2, p),
                 test::rng<Float>(3, p)};
  const Float4 b{test::rng<Float>(4, p),
                 test::rng<Float>(5, p),
                 test::rng<Float>(6, p),
                 test::rng<Float>(7, p)};
  parameters[2 * index    ] = a;
  parameters[2 * index + 1] = b;
  results[2 * index    ] = zinvul::dot(a, b);
  results[2 * index + 1] = zinvul::Geometry::dot3(a, b);
}

template <typename Float, typename Float3>
void testDistance3(GlobalPtr<Float3> parameters,
    GlobalPtr<Float> results,
    const uint32b index) noexcept
{
  const uint32b p = 123'456'789u + index;
  const Float3 a{test::rng<Float>(0, p),
                 test::rng<Float>(1, p),
                 test::rng<Float>(2, p)};
  const Float3 b{test::rng<Float>(3, p),
                 test::rng<Float>(4, p),
                 test::rng<Float>(5, p)};
  parameters[2 * index    ] = a;
  parameters[2 * index + 1] = b;
  results[2 * index    ] = zinvul::distance(a, b);
  results[2 * index + 1] = zinvul::length(a - b);
}

template <typename Float, typename Float4>
void testDistance4(GlobalPtr<Float4> parameters,
    GlobalPtr<Float> results,
    const uint32b index) noexcept
{
  const uint32b p = 123'456'789u + index;
  const Float4 a{test::rng<Float>(0, p),
                 test::rng<Float>(1, p),
                 test::rng<Float>(2, p),
                 test::rng<Float>(3, p)};
  const Float4 b{test::rng<Float>(4, p),
                 test::rng<Float>(5, p),
                 test::rng<Float>(6, p),
                 test::rng<Float>(7, p)};
  parameters[2 * index    ] = a;
  parameters[2 * index + 1] = b;
  results[2 * index    ] = zinvul::distance(a, b);
  results[2 * index + 1] = zinvul::length(a - b);
}

template <typename Float, typename Float3>
void testNormalize3(GlobalPtr<Float3> parameters,
    GlobalPtr<Float3> results,
    const uint32b index) noexcept
{
  const uint32b p = 123'456'789u + index;
  const Float3 a{test::rng<Float>(0, p),
                 test::rng<Float>(1, p),
                 test::rng<Float>(2, p)};
  const Float3 b{test::rng<Float>(3, p),
                 test::rng<Float>(4, p),
                 test::rng<Float>(5, p)};
  parameters[2 * index    ] = a;
  parameters[2 * index + 1] = b;
  results[2 * index    ] = zinvul::normalize(a);
  results[2 * index + 1] = zinvul::Geometry::normalize3(b);
}

template <typename Float, typename Float4>
void testNormalize4(GlobalPtr<Float4> parameters,
    GlobalPtr<Float4> results,
    const uint32b index) noexcept
{
  const uint32b p = 123'456'789u + index;
  const Float4 a{test::rng<Float>(0, p),
                 test::rng<Float>(1, p),
                 test::rng<Float>(2, p),
                 test::rng<Float>(3, p)};
  const Float4 b{test::rng<Float>(4, p),
                 test::rng<Float>(5, p),
                 test::rng<Float>(6, p),
                 test::rng<Float>(7, p)};
  parameters[2 * index    ] = a;
  parameters[2 * index + 1] = b;
  results[2 * index    ] = zinvul::normalize(a);
  results[2 * index + 1] = zinvul::Geometry::normalize3(b);
}

} // namespace test

__kernel void testCross3(GlobalPtr<float3> parameters,
    GlobalPtr<float3> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    test::testCross3<float, float3>(parameters, results, index);
  }
}

__kernel void testCross4(GlobalPtr<float4> parameters,
    GlobalPtr<float4> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    test::testCross4<float, float4>(parameters, results, index);
  }
}

__kernel void testDot3(GlobalPtr<float3> parameters,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    test::testDot3<float, float3>(parameters, results, index);
  }
}

__kernel void testDot4(GlobalPtr<float4> parameters,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    test::testDot4<float, float4>(parameters, results, index);
  }
}

__kernel void testDistance3(GlobalPtr<float3> parameters,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    test::testDistance3<float, float3>(parameters, results, index);
  }
}

__kernel void testDistance4(GlobalPtr<float4> parameters,
    GlobalPtr<float> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    test::testDistance4<float, float4>(parameters, results, index);
  }
}

__kernel void testNormalize3(GlobalPtr<float3> parameters,
    GlobalPtr<float3> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    test::testNormalize3<float, float3>(parameters, results, index);
  }
}

__kernel void testNormalize4(GlobalPtr<float4> parameters,
    GlobalPtr<float4> results,
    const uint32b resolution)
{
  const uint32b index = zinvul::getGlobalIdX();
  if (index < resolution) {
    test::testNormalize4<float, float4>(parameters, results, index);
  }
}

#if !defined(Z_MAC)

//kernel void testCross4D(GlobalPtr<double4> parameters,
//    GlobalPtr<double4> results,
//    const uint32b resolution)
//{
//  const uint32b index = zinvul::getGlobalIdX();
//  if (index < resolution) {
//    test::testCross4<double, double4>(parameters, results, index);
//  }
//}
//
//__kernel void testDot3D(GlobalPtr<double3> parameters,
//    GlobalPtr<double> results,
//    const uint32b resolution)
//{
//  const uint32b index = zinvul::getGlobalIdX();
//  if (index < resolution) {
//    test::testDot3<double, double3>(parameters, results, index);
//  }
//}
//
//__kernel void testDot4D(GlobalPtr<double4> parameters,
//    GlobalPtr<double> results,
//    const uint32b resolution)
//{
//  const uint32b index = zinvul::getGlobalIdX();
//  if (index < resolution) {
//    test::testDot4<double, double4>(parameters, results, index);
//  }
//}
//
//__kernel void testDistance3D(GlobalPtr<double3> parameters,
//    GlobalPtr<double> results,
//    const uint32b resolution)
//{
//  const uint32b index = zinvul::getGlobalIdX();
//  if (index < resolution) {
//    test::testDistance3<double, double3>(parameters, results, index);
//  }
//}
//
//__kernel void testDistance4D(GlobalPtr<double4> parameters,
//    GlobalPtr<double> results,
//    const uint32b resolution)
//{
//  const uint32b index = zinvul::getGlobalIdX();
//  if (index < resolution) {
//    test::testDistance4<double, double4>(parameters, results, index);
//  }
//}
//
//__kernel void testNormalize3D(GlobalPtr<double3> parameters,
//    GlobalPtr<double3> results,
//    const uint32b resolution)
//{
//  const uint32b index = zinvul::getGlobalIdX();
//  if (index < resolution) {
//    test::testNormalize3<double, double3>(parameters, results, index);
//  }
//}
//
//__kernel void testNormalize4D(GlobalPtr<double4> parameters,
//    GlobalPtr<double4> results,
//    const uint32b resolution)
//{
//  const uint32b index = zinvul::getGlobalIdX();
//  if (index < resolution) {
//    test::testNormalize4<double, double4>(parameters, results, index);
//  }
//}

#endif // Z_MAC

#endif /* ZINVUL_GEOMETRY_TEST_GEOMETRY_CL */
