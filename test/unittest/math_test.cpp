/*!
  \file math_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/compensated_summation.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/math.hpp"
// Test
#include "test.hpp"

TEST(MathTest, ConstantValueTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_int = 16;
    constexpr std::size_t n_float = 44;
    auto int_values = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    int_values->setSize(n_int);
    auto float_values = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    float_values->setSize(n_float);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testConstantValues, 1);
    kernel->run(*int_values, *float_values, {1}, 0);
    device->waitForCompletion();

    {
      std::array<int32b, n_int> results;
      int_values->read(results.data(), n_int, 0, 0);
      std::size_t index = 0;
      EXPECT_EQ(FLT_DIG, results[index++])
          << "The constant 'FLT_DIG' is wrong.";
      EXPECT_EQ(FLT_MANT_DIG, results[index++])
          << "The constant 'FLT_MANT_DIG' is wrong.";
      EXPECT_EQ(FLT_MAX_10_EXP, results[index++])
          << "The constant 'FLT_MAX_10_EXP' is wrong.";
      EXPECT_EQ(FLT_MAX_EXP, results[index++])
          << "The constant 'FLT_MAX_EXP' is wrong.";
      EXPECT_EQ(FLT_MIN_10_EXP, results[index++])
          << "The constant 'FLT_MIN_10_EXP' is wrong.";
      EXPECT_EQ(FLT_MIN_EXP, results[index++])
          << "The constant 'FLT_MIN_EXP' is wrong.";
      EXPECT_EQ(FLT_RADIX, results[index++])
          << "The constant 'FLT_RADIX' is wrong.";
      EXPECT_EQ(CHAR_BIT, results[index++])
          << "The constant 'CHAR_BIT' is wrong.";
      EXPECT_EQ(SCHAR_MAX, results[index++])
          << "The constant 'SCHAR_MAX' is wrong.";
      EXPECT_EQ(SCHAR_MIN, results[index++])
          << "The constant 'SCHAR_MIN' is wrong.";
      EXPECT_EQ(CHAR_MAX, results[index++])
          << "The constant 'CHAR_MAX' is wrong.";
      EXPECT_EQ(CHAR_MIN, results[index++])
          << "The constant 'CHAR_MIN' is wrong.";
      EXPECT_EQ(SHRT_MAX, results[index++])
          << "The constant 'SHRT_MAX' is wrong.";
      EXPECT_EQ(SHRT_MIN, results[index++])
          << "The constant 'SHRT_MIN' is wrong.";
      EXPECT_EQ(INT_MAX, results[index++])
          << "The constant 'INT_MAX' is wrong.";
      EXPECT_EQ(INT_MIN, results[index++])
          << "The constant 'INT_MIN' is wrong.";
    }

    {
      std::array<float, n_float> results;
      float_values->read(results.data(), n_float, 0, 0);
      std::size_t index = 0;

      const float e = std::exp(1.0f);
      const float log2e = std::log2(e);
      const float log10e = std::log10(e);
      const float ln2 = std::log(2.0f);
      const float ln10 = std::log(10.0f);
      const float pi = 4.0f * std::atan(1.0f);

      EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), results[index++])
          << "The constant 'MAXFLOAT' is wrong.";
      EXPECT_TRUE(std::isinf(results[index++])) 
          << "The constant 'INFINITY' is wrong.";
      EXPECT_TRUE(std::isnan(results[index++])) 
          << "The constant 'NAN' is wrong.";
      EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), results[index++])
          << "The constant 'FLT_MAX' is wrong.";
      EXPECT_FLOAT_EQ(std::numeric_limits<float>::min(), results[index++])
          << "The constant 'FLT_MIN' is wrong.";
      EXPECT_FLOAT_EQ(std::numeric_limits<float>::epsilon(), results[index++])
          << "The constant 'FLT_EPSILON' is wrong.";
      EXPECT_FLOAT_EQ(e, results[index++])
          << "The constant 'M_E_F' is wrong.";
      EXPECT_FLOAT_EQ(log2e, results[index++])
          << "The constant 'M_LOG2E_F' is wrong.";
      EXPECT_FLOAT_EQ(log10e, results[index++])
          << "The constant 'M_LOG10E_F' is wrong.";
      EXPECT_FLOAT_EQ(ln2, results[index++])
          << "The constant 'M_LN2_F' is wrong.";
      EXPECT_FLOAT_EQ(ln10, results[index++])
          << "The constant 'M_LN10_F' is wrong.";
      EXPECT_FLOAT_EQ(pi, results[index++])
          << "The constant 'M_PI_F' is wrong.";
      EXPECT_FLOAT_EQ(pi / 2.0f, results[index++])
          << "The constant 'M_PI_2_F' is wrong.";
      EXPECT_FLOAT_EQ(pi / 4.0f, results[index++])
          << "The constant 'M_PI_4_F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / pi, results[index++])
          << "The constant 'M_1_PI_F' is wrong.";
      EXPECT_FLOAT_EQ(2.0f / pi, results[index++])
          << "The constant 'M_2_PI_F' is wrong.";
      EXPECT_FLOAT_EQ(2.0f / std::sqrt(pi), results[index++])
          << "The constant 'M_2_SQRTPI_F' is wrong.";
      EXPECT_FLOAT_EQ(std::sqrt(2.0f), results[index++])
          << "The constant 'M_SQRT2_F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / std::sqrt(2.0f), results[index++])
          << "The constant 'M_SQRT1_2_F' is wrong.";

      EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), results[index++])
          << "The constant 'zFloatMax' is wrong.";
      EXPECT_FLOAT_EQ(std::numeric_limits<float>::min(), results[index++])
          << "The constant 'zFloatMin' is wrong.";
      EXPECT_FLOAT_EQ(std::numeric_limits<float>::epsilon(), results[index++])
          << "The constant 'zFloatEpsilon' is wrong.";
      EXPECT_FLOAT_EQ(std::numeric_limits<float>::min() - std::numeric_limits<float>::denorm_min(), results[index++])
          << "The constant 'zFloatDenormMax' is wrong.";
      EXPECT_FLOAT_EQ(std::numeric_limits<float>::denorm_min(), results[index++])
          << "The constant 'zFloatDenormMin' is wrong.";
      EXPECT_FLOAT_EQ(e, results[index++])
          << "The constant 'zEF' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / e, results[index++])
          << "The constant 'zInvEF' is wrong.";
      EXPECT_FLOAT_EQ(log2e, results[index++])
          << "The constant 'zLog2EF' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / log2e, results[index++])
          << "The constant 'zInvLog2EF' is wrong.";
      EXPECT_FLOAT_EQ(log10e, results[index++])
          << "The constant 'zLog10EF' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / log10e, results[index++])
          << "The constant 'zInvLog10EF' is wrong.";
      EXPECT_FLOAT_EQ(ln2, results[index++])
          << "The constant 'zLn2F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / ln2, results[index++])
          << "The constant 'zInvLn2F' is wrong.";
      EXPECT_FLOAT_EQ(ln10, results[index++])
          << "The constant 'zLn10F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / ln10, results[index++])
          << "The constant 'zInvLn10F' is wrong.";
      EXPECT_FLOAT_EQ(pi, results[index++])
          << "The constant 'zPiF' is wrong.";
      EXPECT_FLOAT_EQ(pi / 2.0f, results[index++])
          << "The constant 'zPi2F' is wrong.";
      EXPECT_FLOAT_EQ(pi / 4.0f, results[index++])
          << "The constant 'zPi4F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / pi, results[index++])
          << "The constant 'zInvPi' is wrong.";
      EXPECT_FLOAT_EQ(2.0f / pi, results[index++])
          << "The constant 'z2InvPi' is wrong.";
      EXPECT_FLOAT_EQ(2.0f / std::sqrt(pi), results[index++])
          << "The constant 'z2InvSqrtPiF' is wrong.";
      EXPECT_FLOAT_EQ(std::sqrt(2.0f), results[index++])
          << "The constant 'zSqrt2F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / std::sqrt(2.0f), results[index++])
          << "The constant 'zInvSqrt2F' is wrong.";
      EXPECT_FLOAT_EQ(std::sqrt(3.0f), results[index++])
          << "The constant 'zSqrt3F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / std::sqrt(3.0f), results[index++])
          << "The constant 'zInvSqrt3F' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CommonTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(3);
    auto abs_result2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    abs_result2->setSize(2);
    auto abs_result3 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    abs_result3->setSize(2);
    auto abs_result4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    abs_result4->setSize(1);
    auto abs_result5 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    abs_result5->setSize(2);
    auto abs_result6 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    abs_result6->setSize(1);
    auto abs_result7 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    abs_result7->setSize(2);
    auto abs_result8 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    abs_result8->setSize(1);
    auto clamp_result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result1->setSize(9);
    auto clamp_result2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result2->setSize(11);
    auto clamp_result3 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result3->setSize(6);
    auto clamp_result4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result4->setSize(8);
    auto clamp_result5 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result5->setSize(3);
    auto clamp_result6 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result6->setSize(5);
    auto clamp_result7 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result7->setSize(3);
    auto clamp_result8 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result8->setSize(5);

    auto kernel = makeZinvulKernel(device.get(), math, testCommon, 1);
    kernel->run(*buffer1, *abs_result2, *abs_result3, *abs_result4,
        *abs_result5, *abs_result6, *abs_result7, *abs_result8,
        *clamp_result1, *clamp_result2, *clamp_result3, *clamp_result4,
        *clamp_result5, *clamp_result6, *clamp_result7, *clamp_result8, {1}, 0);
    device->waitForCompletion();

    {
      std::array<uint32b, 3> result;
      buffer1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(1u, result[0]) << "The abs func is wrong.";
      ASSERT_EQ(1u, result[1]) << "The abs func is wrong.";
      ASSERT_EQ(1u, result[2]) << "The abs func is wrong.";
    }
    {
      std::array<float, 2> result;
      abs_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(1.0f, result[0]) << "The fabs func is wrong.";
      ASSERT_EQ(1.0f, result[1]) << "The fabs func is wrong.";
    }
    {
      std::array<cl::uint2, 2> result;
      abs_result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(i+1, result[0][i]) << "The abs func is wrong.";
        ASSERT_EQ(i+1, result[1][i]) << "The abs func is wrong.";
      }
    }
    {
      std::array<cl::float2, 1> result;
      abs_result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 2; ++i)
        ASSERT_EQ(zisc::cast<float>(i+1), result[0][i]) << "The fabs func is wrong.";
    }
    {
      std::array<cl::uint3, 2> result;
      abs_result5->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 3; ++i) {
        ASSERT_EQ(i+1, result[0][i]) << "The abs func is wrong.";
        ASSERT_EQ(i+1, result[1][i]) << "The abs func is wrong.";
      }
    }
    {
      std::array<cl::float3, 1> result;
      abs_result6->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 3; ++i)
        ASSERT_EQ(zisc::cast<float>(i+1), result[0][i]) << "The fabs func is wrong.";
    }
    {
      std::array<cl::uint4, 2> result;
      abs_result7->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        ASSERT_EQ(i+1, result[0][i]) << "The abs func is wrong.";
        ASSERT_EQ(i+1, result[1][i]) << "The abs func is wrong.";
      }
    }
    {
      std::array<cl::float4, 1> result;
      abs_result8->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i)
        ASSERT_EQ(zisc::cast<float>(i+1), result[0][i]) << "The fabs func is wrong.";
    }

    {
      std::array<int32b, 9> result;
      clamp_result1->read(result.data(), result.size(), 0 ,0);
      ASSERT_FALSE(result[0]) << "The max func is wrong.";
      ASSERT_FALSE(result[1]) << "The min func is wrong.";
      ASSERT_FALSE(result[2]) << "The clamp func is wrong.";
      ASSERT_EQ(10, result[3]) << "The max func is wrong.";
      ASSERT_TRUE(result[4]) << "The min func is wrong.";
      ASSERT_TRUE(result[5]) << "The clamp func is wrong.";
      ASSERT_EQ(-1, result[6]) << "The max func is wrong.";
      ASSERT_EQ(-10, result[7]) << "The min func is wrong.";
      ASSERT_EQ(-1, result[8]) << "The clamp func is wrong.";
    }
    {
      std::array<float, 11> result;
      clamp_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(0.0f, result[0]) << "The max func is wrong.";
      ASSERT_EQ(0.0f, result[1]) << "The min func is wrong.";
      ASSERT_EQ(0.0f, result[2]) << "The clamp func is wrong.";
      ASSERT_EQ(10.0f, result[3]) << "The max func is wrong.";
      ASSERT_EQ(1.0f, result[4]) << "The min func is wrong.";
      ASSERT_EQ(1.0f, result[5]) << "The clamp func is wrong.";
      ASSERT_EQ(-1.0f, result[6]) << "The max func is wrong.";
      ASSERT_EQ(-10.0f, result[7]) << "The min func is wrong.";
      ASSERT_EQ(-1.0f, result[8]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::int2, 6> result;
      clamp_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0][0]) << "The max func is wrong.";
      ASSERT_EQ(10, result[0][1]) << "The max func is wrong.";
      ASSERT_FALSE(result[1][0]) << "The min func is wrong.";
      ASSERT_EQ(2, result[1][1]) << "The min func is wrong.";
      ASSERT_FALSE(result[2][0]) << "The clamp func is wrong.";
      ASSERT_EQ(2, result[2][1]) << "The clamp func is wrong.";
      ASSERT_FALSE(result[3][0]) << "The max func is wrong.";
      ASSERT_EQ(-2, result[3][1]) << "The max func is wrong.";
      ASSERT_FALSE(result[4][0]) << "The min func is wrong.";
      ASSERT_EQ(-2, result[4][1]) << "The min func is wrong.";
      ASSERT_FALSE(result[5][0]) << "The clamp func is wrong.";
      ASSERT_EQ(-2, result[5][1]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float2, 8> result;
      clamp_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(0.0f, result[0][0]) << "The max func is wrong.";
      ASSERT_EQ(10.0f, result[0][1]) << "The max func is wrong.";
      ASSERT_EQ(0.0f, result[1][0]) << "The min func is wrong.";
      ASSERT_EQ(2.0f, result[1][1]) << "The min func is wrong.";
      ASSERT_EQ(0.0f, result[2][0]) << "The clamp func is wrong.";
      ASSERT_EQ(2.0f, result[2][1]) << "The clamp func is wrong.";
      ASSERT_EQ(0.0f, result[3][0]) << "The max func is wrong.";
      ASSERT_EQ(-2.0f, result[3][1]) << "The max func is wrong.";
      ASSERT_EQ(0.0f, result[4][0]) << "The min func is wrong.";
      ASSERT_EQ(-2.0f, result[4][1]) << "The min func is wrong.";
      ASSERT_EQ(0.0f, result[5][0]) << "The clamp func is wrong.";
      ASSERT_EQ(-2.0f, result[5][1]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::int3, 3> result;
      clamp_result5->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0][0]) << "The max func is wrong.";
      ASSERT_EQ(10, result[0][1]) << "The max func is wrong.";
      ASSERT_EQ(-3, result[0][2]) << "The max func is wrong.";
      ASSERT_FALSE(result[1][0]) << "The min func is wrong.";
      ASSERT_EQ(2, result[1][1]) << "The min func is wrong.";
      ASSERT_EQ(-3, result[1][2]) << "The min func is wrong.";
      ASSERT_FALSE(result[2][0]) << "The clamp func is wrong.";
      ASSERT_EQ(2, result[2][1]) << "The clamp func is wrong.";
      ASSERT_EQ(-3, result[2][2]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float3, 5> result;
      clamp_result6->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(0.0f, result[0][0]) << "The max func is wrong.";
      ASSERT_EQ(10.0f, result[0][1]) << "The max func is wrong.";
      ASSERT_EQ(-3.0f, result[0][2]) << "The max func is wrong.";
      ASSERT_EQ(0.0f, result[1][0]) << "The min func is wrong.";
      ASSERT_EQ(2.0f, result[1][1]) << "The min func is wrong.";
      ASSERT_EQ(-3.0f, result[1][2]) << "The min func is wrong.";
      ASSERT_EQ(0.0f, result[2][0]) << "The clamp func is wrong.";
      ASSERT_EQ(2.0f, result[2][1]) << "The clamp func is wrong.";
      ASSERT_EQ(-3.0f, result[2][2]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::int4, 3> result;
      clamp_result7->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0][0]) << "The max func is wrong.";
      ASSERT_EQ(10, result[0][1]) << "The max func is wrong.";
      ASSERT_EQ(-3, result[0][2]) << "The max func is wrong.";
      ASSERT_EQ(100, result[0][3]) << "The max func is wrong.";
      ASSERT_FALSE(result[1][0]) << "The min func is wrong.";
      ASSERT_EQ(2, result[1][1]) << "The min func is wrong.";
      ASSERT_EQ(-3, result[1][2]) << "The min func is wrong.";
      ASSERT_EQ(4, result[1][3]) << "The min func is wrong.";
      ASSERT_FALSE(result[2][0]) << "The clamp func is wrong.";
      ASSERT_EQ(2, result[2][1]) << "The clamp func is wrong.";
      ASSERT_EQ(-3, result[2][2]) << "The clamp func is wrong.";
      ASSERT_EQ(4, result[2][3]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float4, 5> result;
      clamp_result8->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(0.0f, result[0][0]) << "The max func is wrong.";
      ASSERT_EQ(10.0f, result[0][1]) << "The max func is wrong.";
      ASSERT_EQ(-3.0f, result[0][2]) << "The max func is wrong.";
      ASSERT_EQ(100.0f, result[0][3]) << "The max func is wrong.";
      ASSERT_EQ(0.0f, result[1][0]) << "The min func is wrong.";
      ASSERT_EQ(2.0f, result[1][1]) << "The min func is wrong.";
      ASSERT_EQ(-3.0f, result[1][2]) << "The min func is wrong.";
      ASSERT_EQ(4.0f, result[1][3]) << "The min func is wrong.";
      ASSERT_EQ(0.0f, result[2][0]) << "The clamp func is wrong.";
      ASSERT_EQ(2.0f, result[2][1]) << "The clamp func is wrong.";
      ASSERT_EQ(-3.0f, result[2][2]) << "The clamp func is wrong.";
      ASSERT_EQ(4.0f, result[2][3]) << "The clamp func is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZCommonTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(3);
    auto abs_result2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    abs_result2->setSize(2);
    auto abs_result3 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    abs_result3->setSize(2);
    auto abs_result4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    abs_result4->setSize(1);
    auto abs_result5 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    abs_result5->setSize(2);
    auto abs_result6 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    abs_result6->setSize(1);
    auto abs_result7 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    abs_result7->setSize(2);
    auto abs_result8 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    abs_result8->setSize(1);
    auto clamp_result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result1->setSize(9);
    auto clamp_result2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result2->setSize(11);
    auto clamp_result3 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result3->setSize(6);
    auto clamp_result4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result4->setSize(8);
    auto clamp_result5 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result5->setSize(3);
    auto clamp_result6 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result6->setSize(5);
    auto clamp_result7 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result7->setSize(3);
    auto clamp_result8 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result8->setSize(5);

    auto kernel = makeZinvulKernel(device.get(), math, testZCommon, 1);
    kernel->run(*buffer1, *abs_result2, *abs_result3, *abs_result4,
        *abs_result5, *abs_result6, *abs_result7, *abs_result8,
        *clamp_result1, *clamp_result2, *clamp_result3, *clamp_result4,
        *clamp_result5, *clamp_result6, *clamp_result7, *clamp_result8, {1}, 0);
    device->waitForCompletion();

    {
      std::array<uint32b, 3> result;
      buffer1->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(1u, result[0]) << "The abs func is wrong.";
      EXPECT_EQ(1u, result[1]) << "The abs func is wrong.";
    }
    {
      std::array<float, 2> result;
      abs_result2->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(1.0f, result[0]) << "The fabs func is wrong.";
      EXPECT_EQ(1.0f, result[1]) << "The fabs func is wrong.";
    }
    {
      std::array<cl::uint2, 2> result;
      abs_result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 2; ++i) {
        EXPECT_EQ(i+1, result[0][i]) << "The abs func is wrong.";
      }
    }
    {
      std::array<cl::float2, 1> result;
      abs_result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 2; ++i)
        EXPECT_EQ(zisc::cast<float>(i+1), result[0][i]) << "The fabs func is wrong.";
    }
    {
      std::array<cl::uint3, 2> result;
      abs_result5->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(i+1, result[0][i]) << "The abs func is wrong.";
      }
    }
    {
      std::array<cl::float3, 1> result;
      abs_result6->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 3; ++i)
        EXPECT_EQ(zisc::cast<float>(i+1), result[0][i]) << "The fabs func is wrong.";
    }
    {
      std::array<cl::uint4, 2> result;
      abs_result7->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(i+1, result[0][i]) << "The abs func is wrong.";
      }
    }
    {
      std::array<cl::float4, 1> result;
      abs_result8->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i)
        EXPECT_EQ(zisc::cast<float>(i+1), result[0][i]) << "The fabs func is wrong.";
    }

    {
      std::array<int32b, 9> result;
      clamp_result1->read(result.data(), result.size(), 0 ,0);
      EXPECT_FALSE(result[0]) << "The max func is wrong.";
      EXPECT_FALSE(result[1]) << "The min func is wrong.";
      EXPECT_FALSE(result[2]) << "The clamp func is wrong.";
      EXPECT_EQ(10, result[3]) << "The max func is wrong.";
      EXPECT_TRUE(result[4]) << "The min func is wrong.";
      EXPECT_TRUE(result[5]) << "The clamp func is wrong.";
      EXPECT_EQ(-1, result[6]) << "The max func is wrong.";
      EXPECT_EQ(-10, result[7]) << "The min func is wrong.";
      EXPECT_EQ(-1, result[8]) << "The clamp func is wrong.";
    }
    {
      std::array<float, 11> result;
      clamp_result2->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0]) << "The max func is wrong.";
      EXPECT_EQ(0.0f, result[1]) << "The min func is wrong.";
      EXPECT_EQ(0.0f, result[2]) << "The clamp func is wrong.";
      EXPECT_EQ(10.0f, result[3]) << "The max func is wrong.";
      EXPECT_EQ(1.0f, result[4]) << "The min func is wrong.";
      EXPECT_EQ(1.0f, result[5]) << "The clamp func is wrong.";
      EXPECT_EQ(-1.0f, result[6]) << "The max func is wrong.";
      EXPECT_EQ(-10.0f, result[7]) << "The min func is wrong.";
      EXPECT_EQ(-1.0f, result[8]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::int2, 6> result;
      clamp_result3->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The max func is wrong.";
      EXPECT_EQ(10, result[0][1]) << "The max func is wrong.";
      EXPECT_FALSE(result[1][0]) << "The min func is wrong.";
      EXPECT_EQ(2, result[1][1]) << "The min func is wrong.";
      EXPECT_FALSE(result[2][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[2][1]) << "The clamp func is wrong.";
      EXPECT_FALSE(result[3][0]) << "The max func is wrong.";
      EXPECT_EQ(-2, result[3][1]) << "The max func is wrong.";
      EXPECT_FALSE(result[4][0]) << "The min func is wrong.";
      EXPECT_EQ(-2, result[4][1]) << "The min func is wrong.";
      EXPECT_FALSE(result[5][0]) << "The clamp func is wrong.";
      EXPECT_EQ(-2, result[5][1]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float2, 8> result;
      clamp_result4->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The max func is wrong.";
      EXPECT_EQ(10.0f, result[0][1]) << "The max func is wrong.";
      EXPECT_EQ(0.0f, result[1][0]) << "The min func is wrong.";
      EXPECT_EQ(2.0f, result[1][1]) << "The min func is wrong.";
      EXPECT_EQ(0.0f, result[2][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[2][1]) << "The clamp func is wrong.";
      EXPECT_EQ(0.0f, result[3][0]) << "The max func is wrong.";
      EXPECT_EQ(-2.0f, result[3][1]) << "The max func is wrong.";
      EXPECT_EQ(0.0f, result[4][0]) << "The min func is wrong.";
      EXPECT_EQ(-2.0f, result[4][1]) << "The min func is wrong.";
      EXPECT_EQ(0.0f, result[5][0]) << "The clamp func is wrong.";
      EXPECT_EQ(-2.0f, result[5][1]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::int3, 3> result;
      clamp_result5->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The max func is wrong.";
      EXPECT_EQ(10, result[0][1]) << "The max func is wrong.";
      EXPECT_EQ(-3, result[0][2]) << "The max func is wrong.";
      EXPECT_FALSE(result[1][0]) << "The min func is wrong.";
      EXPECT_EQ(2, result[1][1]) << "The min func is wrong.";
      EXPECT_EQ(-3, result[1][2]) << "The min func is wrong.";
      EXPECT_FALSE(result[2][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[2][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3, result[2][2]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float3, 5> result;
      clamp_result6->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The max func is wrong.";
      EXPECT_EQ(10.0f, result[0][1]) << "The max func is wrong.";
      EXPECT_EQ(-3.0f, result[0][2]) << "The max func is wrong.";
      EXPECT_EQ(0.0f, result[1][0]) << "The min func is wrong.";
      EXPECT_EQ(2.0f, result[1][1]) << "The min func is wrong.";
      EXPECT_EQ(-3.0f, result[1][2]) << "The min func is wrong.";
      EXPECT_EQ(0.0f, result[2][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[2][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3.0f, result[2][2]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::int4, 3> result;
      clamp_result7->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The max func is wrong.";
      EXPECT_EQ(10, result[0][1]) << "The max func is wrong.";
      EXPECT_EQ(-3, result[0][2]) << "The max func is wrong.";
      EXPECT_EQ(100, result[0][3]) << "The max func is wrong.";
      EXPECT_FALSE(result[1][0]) << "The min func is wrong.";
      EXPECT_EQ(2, result[1][1]) << "The min func is wrong.";
      EXPECT_EQ(-3, result[1][2]) << "The min func is wrong.";
      EXPECT_EQ(4, result[1][3]) << "The min func is wrong.";
      EXPECT_FALSE(result[2][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[2][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3, result[2][2]) << "The clamp func is wrong.";
      EXPECT_EQ(4, result[2][3]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float4, 5> result;
      clamp_result8->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The max func is wrong.";
      EXPECT_EQ(10.0f, result[0][1]) << "The max func is wrong.";
      EXPECT_EQ(-3.0f, result[0][2]) << "The max func is wrong.";
      EXPECT_EQ(100.0f, result[0][3]) << "The max func is wrong.";
      EXPECT_EQ(0.0f, result[1][0]) << "The min func is wrong.";
      EXPECT_EQ(2.0f, result[1][1]) << "The min func is wrong.";
      EXPECT_EQ(-3.0f, result[1][2]) << "The min func is wrong.";
      EXPECT_EQ(4.0f, result[1][3]) << "The min func is wrong.";
      EXPECT_EQ(0.0f, result[2][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[2][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3.0f, result[2][2]) << "The clamp func is wrong.";
      EXPECT_EQ(4.0f, result[2][3]) << "The clamp func is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CeilTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(36);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(16);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(16);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testCeil, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'ceil' func is wrong.";
    {
      std::array<float, 36> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::ceil(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zCeilImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(36);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(16);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(16);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzCeilImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zCeilImpl' func is wrong.";
    {
      std::array<float, 36> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::ceil(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zCeilTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(36);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(16);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(16);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzCeil, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zCeil' func is wrong.";
    {
      std::array<float, 36> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::ceil(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FloorTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testFloor, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'floor' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::floor(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zFloorImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzFloorImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zFloorImpl' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::floor(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zFloorTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzFloor, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zFloor' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::floor(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, TruncTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testTrunc, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'trunc' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::trunc(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zTruncImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzTruncImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zTruncImpl' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::trunc(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zTruncTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzTrunc, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zTrunc' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::trunc(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RoundTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testRound, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'round' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::round(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zRoundImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzRoundImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zRoundImpl' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::round(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zRoundTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzRound, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zRound' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::round(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AbsTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(2);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testAbs, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'abs' func is wrong.";
    {
      std::array<uint32b, 2> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0]) << error_message;
      ASSERT_EQ(5, result[1]) << error_message;
    }
    {
      std::array<cl::uint2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
    }
    {
      std::array<cl::uint3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
      ASSERT_EQ(9, result[0].z) << error_message;
    }
    {
      std::array<cl::uint4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
      ASSERT_EQ(9, result[0].z) << error_message;
      ASSERT_EQ(15, result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zAbsImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(2);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testzAbsImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zAbsImpl' func is wrong.";
    {
      std::array<uint32b, 2> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0]) << error_message;
      ASSERT_EQ(5, result[1]) << error_message;
    }
    {
      std::array<cl::uint2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
    }
    {
      std::array<cl::uint3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
      ASSERT_EQ(9, result[0].z) << error_message;
    }
    {
      std::array<cl::uint4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
      ASSERT_EQ(9, result[0].z) << error_message;
      ASSERT_EQ(15, result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zAbsTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(2);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testzAbs, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zAbs' func is wrong.";
    {
      std::array<uint32b, 2> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0]) << error_message;
      ASSERT_EQ(5, result[1]) << error_message;
    }
    {
      std::array<cl::uint2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
    }
    {
      std::array<cl::uint3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
      ASSERT_EQ(9, result[0].z) << error_message;
    }
    {
      std::array<cl::uint4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5, result[0].x) << error_message;
      ASSERT_EQ(2, result[0].y) << error_message;
      ASSERT_EQ(9, result[0].z) << error_message;
      ASSERT_EQ(15, result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AbsFTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(2);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testAbsF, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'fabs' func is wrong.";
    {
      std::array<float, 2> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0]) << error_message;
      ASSERT_EQ(5.0f, result[1]) << error_message;
    }
    {
      std::array<cl::float2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
    }
    {
      std::array<cl::float3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
      ASSERT_EQ(9.0f, result[0].z) << error_message;
    }
    {
      std::array<cl::float4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
      ASSERT_EQ(9.0f, result[0].z) << error_message;
      ASSERT_EQ(15.0f, result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zAbsFImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(2);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testzAbsFImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zAbsFImpl' func is wrong.";
    {
      std::array<float, 2> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0]) << error_message;
      ASSERT_EQ(5.0f, result[1]) << error_message;
    }
    {
      std::array<cl::float2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
    }
    {
      std::array<cl::float3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
      ASSERT_EQ(9.0f, result[0].z) << error_message;
    }
    {
      std::array<cl::float4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
      ASSERT_EQ(9.0f, result[0].z) << error_message;
      ASSERT_EQ(15.0f, result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zAbsFTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(2);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testzAbsF, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zAbsF' func is wrong.";
    {
      std::array<float, 2> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0]) << error_message;
      ASSERT_EQ(5.0f, result[1]) << error_message;
    }
    {
      std::array<cl::float2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
    }
    {
      std::array<cl::float3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
      ASSERT_EQ(9.0f, result[0].z) << error_message;
    }
    {
      std::array<cl::float4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(5.0f, result[0].x) << error_message;
      ASSERT_EQ(2.0f, result[0].y) << error_message;
      ASSERT_EQ(9.0f, result[0].z) << error_message;
      ASSERT_EQ(15.0f, result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zIsAlmostSameTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(6);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testzIsAlmostSame, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zIsAlmostSame' func is wrong.";
    {
      std::array<int32b, 6> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_TRUE(result[0]) << error_message;
      ASSERT_TRUE(result[1]) << error_message;
      ASSERT_FALSE(result[2]) << error_message;
      ASSERT_FALSE(result[3]) << error_message;
      ASSERT_TRUE(result[4]) << error_message;
      ASSERT_FALSE(result[5]) << error_message;
    }
    {
      std::array<cl::int2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_TRUE(result[0].x) << error_message;
      ASSERT_TRUE(result[0].y) << error_message;
    }
    {
      std::array<cl::int3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_TRUE(result[0].x) << error_message;
      ASSERT_TRUE(result[0].y) << error_message;
      ASSERT_TRUE(result[0].z) << error_message;
    }
    {
      std::array<cl::int4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_TRUE(result[0].x) << error_message;
      ASSERT_TRUE(result[0].y) << error_message;
      ASSERT_TRUE(result[0].z) << error_message;
      ASSERT_TRUE(result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zIsOddTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(5);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testzIsOdd, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zIsOdd' func is wrong.";
    {
      std::array<int32b, 5> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0]) << error_message;
      ASSERT_TRUE(result[1]) << error_message;
      ASSERT_FALSE(result[2]) << error_message;
      ASSERT_TRUE(result[3]) << error_message;
      ASSERT_FALSE(result[4]) << error_message;
    }
    {
      std::array<cl::int2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_TRUE(result[0].x) << error_message;
      ASSERT_FALSE(result[0].y) << error_message;
    }
    {
      std::array<cl::int3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0].x) << error_message;
      ASSERT_TRUE(result[0].y) << error_message;
      ASSERT_FALSE(result[0].z) << error_message;
    }
    {
      std::array<cl::int4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_TRUE(result[0].x) << error_message;
      ASSERT_FALSE(result[0].y) << error_message;
      ASSERT_TRUE(result[0].z) << error_message;
      ASSERT_FALSE(result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zIsOddUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(5);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(1);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(1);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), math, testzIsOddU, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zIsOddU' func is wrong.";
    {
      std::array<int32b, 5> result;
      result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0]) << error_message;
      ASSERT_TRUE(result[1]) << error_message;
      ASSERT_FALSE(result[2]) << error_message;
    }
    {
      std::array<cl::int2, 1> result;
      result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0].x) << error_message;
      ASSERT_TRUE(result[0].y) << error_message;
    }
    {
      std::array<cl::int3, 1> result;
      result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0].x) << error_message;
      ASSERT_TRUE(result[0].y) << error_message;
      ASSERT_FALSE(result[0].z) << error_message;
    }
    {
      std::array<cl::int4, 1> result;
      result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FALSE(result[0].x) << error_message;
      ASSERT_TRUE(result[0].y) << error_message;
      ASSERT_FALSE(result[0].z) << error_message;
      ASSERT_TRUE(result[0].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, MaxTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 30;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testMax, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const int32b lhs = result[3 * i];
        const int32b rhs = result[3 * i + 1];
        const int32b expected = std::max(lhs, rhs);
        const int32b m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::int2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMaxImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 30;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMaxImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMaxImpl' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const int32b lhs = result[3 * i];
        const int32b rhs = result[3 * i + 1];
        const int32b expected = std::max(lhs, rhs);
        const int32b m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::int2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMaxTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 30;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMax, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMax' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const int32b lhs = result[3 * i];
        const int32b rhs = result[3 * i + 1];
        const int32b expected = std::max(lhs, rhs);
        const int32b m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::int2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = std::max(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, MaxUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 18;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testMaxU, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::max(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::uint2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMaxUImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 18;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMaxUImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMaxUImpl' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::max(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::uint2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMaxUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 18;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMaxU, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMaxU' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::max(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::uint2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, MinTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 30;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testMin, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'min' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const int32b lhs = result[3 * i];
        const int32b rhs = result[3 * i + 1];
        const int32b expected = std::min(lhs, rhs);
        const int32b m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::int2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMinImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 30;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMinImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMinImpl' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const int32b lhs = result[3 * i];
        const int32b rhs = result[3 * i + 1];
        const int32b expected = std::min(lhs, rhs);
        const int32b m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::int2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMinTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 30;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMin, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMin' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const int32b lhs = result[3 * i];
        const int32b rhs = result[3 * i + 1];
        const int32b expected = std::min(lhs, rhs);
        const int32b m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::int2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = std::min(lhs[j], rhs[j]);
          const int32b m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, MinUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 18;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testMinU, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'min' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::min(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::uint2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMinUImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 18;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMinUImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMinUImpl' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::min(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::uint2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMinUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 18;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMinU, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMinU' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::min(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::uint2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, MaxFTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 48;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testMaxF, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::max(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::float2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMaxFImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 48;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMaxFImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMaxFImpl' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::max(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::float2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMaxFTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 48;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMaxF, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMaxF' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::max(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::float2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::max(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, MinFTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 48;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testMinF, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'min' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::min(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::float2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMinFImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 48;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMinFImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMinFImpl' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::min(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::float2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMinFTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 48;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testzMinF, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMinF' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        const auto expected = std::min(lhs, rhs);
        const auto m = result[3 * i + 2];
        ASSERT_EQ(expected, m) << error_message;
      }
    }
    {
      std::array<cl::float2, 3> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 3> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 3> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 3; ++i) {
        const auto lhs = result[3 * i];
        const auto rhs = result[3 * i + 1];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::min(lhs[j], rhs[j]);
          const auto m = result[3 * i + 2][j];
          ASSERT_EQ(expected, m) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, MixTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto mix_result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    mix_result1->setSize(3);
    auto mix_result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    mix_result2->setSize(6);
    auto mix_result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    mix_result3->setSize(6);
    auto mix_result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    mix_result4->setSize(6);

    auto kernel = makeZinvulKernel(device.get(), math, testMix, 1);
    kernel->run(*mix_result1, *mix_result2, *mix_result3, *mix_result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'mix' func is wrong.";
    {
      std::array<float, 3> result;
      mix_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2]) << error_message;
    }
    {
      std::array<cl::float2, 6> result;
      mix_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
    }
    {
      std::array<cl::float3, 6> result;
      mix_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
    }
    {
      std::array<cl::float4, 6> result;
      mix_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
      ASSERT_FLOAT_EQ(-4.0f, result[0][3]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][3]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
      ASSERT_FLOAT_EQ(4.0f, result[2][3]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
      ASSERT_FLOAT_EQ(-4.0f, result[3][3]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][3]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
      ASSERT_FLOAT_EQ(4.0f, result[5][3]) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMixImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto mix_result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    mix_result1->setSize(3);
    auto mix_result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    mix_result2->setSize(6);
    auto mix_result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    mix_result3->setSize(6);
    auto mix_result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    mix_result4->setSize(6);

    auto kernel = makeZinvulKernel(device.get(), math, testzMixImpl, 1);
    kernel->run(*mix_result1, *mix_result2, *mix_result3, *mix_result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMixImpl' func is wrong.";
    {
      std::array<float, 3> result;
      mix_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2]) << error_message;
    }
    {
      std::array<cl::float2, 6> result;
      mix_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
    }
    {
      std::array<cl::float3, 6> result;
      mix_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
    }
    {
      std::array<cl::float4, 6> result;
      mix_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
      ASSERT_FLOAT_EQ(-4.0f, result[0][3]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][3]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
      ASSERT_FLOAT_EQ(4.0f, result[2][3]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
      ASSERT_FLOAT_EQ(-4.0f, result[3][3]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][3]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
      ASSERT_FLOAT_EQ(4.0f, result[5][3]) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zMixTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto mix_result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    mix_result1->setSize(3);
    auto mix_result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    mix_result2->setSize(6);
    auto mix_result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    mix_result3->setSize(6);
    auto mix_result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    mix_result4->setSize(6);

    auto kernel = makeZinvulKernel(device.get(), math, testzMix, 1);
    kernel->run(*mix_result1, *mix_result2, *mix_result3, *mix_result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMix' func is wrong.";
    {
      std::array<float, 3> result;
      mix_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2]) << error_message;
    }
    {
      std::array<cl::float2, 6> result;
      mix_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
    }
    {
      std::array<cl::float3, 6> result;
      mix_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
    }
    {
      std::array<cl::float4, 6> result;
      mix_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
      ASSERT_FLOAT_EQ(-4.0f, result[0][3]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[1][3]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
      ASSERT_FLOAT_EQ(4.0f, result[2][3]) << error_message;
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
      ASSERT_FLOAT_EQ(-4.0f, result[3][3]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[4][3]) << error_message;
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
      ASSERT_FLOAT_EQ(4.0f, result[5][3]) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ClampTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 15 * 4;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(8);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(8);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testClamp, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clamp' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::int2, 8> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 8> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 8> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClampImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 15 * 4;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(8);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(8);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testzClampImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClampImpl' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::int2, 8> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 8> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 8> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClampTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 15 * 4;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(8);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(8);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testzClamp, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClamp' func is wrong.";
    {
      std::array<int32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::int2, 8> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 8> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 8> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ClampUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 12 * 4;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(8);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(8);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testClampU, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clamp' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::uint2, 8> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 8> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 8> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClampUImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 12 * 4;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(8);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(8);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testzClampUImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClampUImpl' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::uint2, 8> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 8> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 8> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClampUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 12 * 4;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(8);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(8);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testzClampU, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClampU' func is wrong.";
    {
      std::array<uint32b, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::uint2, 8> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 8> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 8> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ClampFTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 21 * 4;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(16);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(16);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testClampF, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clamp' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::float2, 16> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClampFImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 21 * 4;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(16);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(16);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzClampFImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClampFImpl' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::float2, 16> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClampFTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    const std::size_t n = 21 * 4;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(16);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(16);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzClampF, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClampF' func is wrong.";
    {
      std::array<float, n> result;
      results1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        const auto expected = std::clamp(x, minval, maxval);
        const auto c = result[4 * i + 3];
        ASSERT_EQ(expected, c) << error_message;
      }
    }
    {
      std::array<cl::float2, 16> result;
      results2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 2; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      results3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 3; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      results4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 4; ++i) {
        const auto x = result[4 * i];
        const auto minval = result[4 * i + 1];
        const auto maxval = result[4 * i + 2];
        for (std::size_t j = 0; j < 4; ++j) {
          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
          const auto c = result[4 * i + 3][j];
          ASSERT_EQ(expected, c) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ClzTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testClz, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clz' func is wrong.";
    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::int2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClzImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testzClzImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClzImpl' func is wrong.";
    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::int2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClzTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testzClz, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClz' func is wrong.";
    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::int2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ClzUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testClzU, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clz' func is wrong.";
    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::uint2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClzUImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testzClzUImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClzUImpl' func is wrong.";
    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::uint2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zClzUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testzClzU, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zClzU' func is wrong.";
    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::uint2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const uint32b expected = cl::clz(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, PopcountTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testPopcount, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'popcount' func is wrong.";
    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::int2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zPopcountImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testzPopcountImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zPopcountImpl' func is wrong.";
    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::int2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zPopcountTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testzPopcount, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zPopcount' func is wrong.";
    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::int2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::int4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr int32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const int32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, PopcountUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testPopcountU, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'popcount' func is wrong.";
    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::uint2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zPopcountUImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testzPopcountUImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zPopcountUImpl' func is wrong.";
    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::uint2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zPopcountUTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(32);
    auto result2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(32);
    auto result3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(32);
    auto result4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(32);

    auto kernel = makeZinvulKernel(device.get(), math, testzPopcountU, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zPopcountU' func is wrong.";
    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i]) << error_message;
      }
    }
    {
      std::array<cl::uint2, 32> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 2; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint3, 32> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 3; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::uint4, 32> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        constexpr uint32b k[] = {1, 2, 3, 5};
        for (std::size_t j = 0; j < 4; ++j) {
          const uint32b expected = cl::popcount(x / k[j]);
          ASSERT_EQ(expected, result[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
TEST(MathTest, FractTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result1i = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1i->setSize(18);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result2i = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2i->setSize(8);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result3i = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3i->setSize(8);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);
    auto result4i = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4i->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testFract, 1);
    kernel->run(*result1, *result1i, *result2, *result2i,
                *result3, *result3i, *result4, *result4i, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'fract' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      std::array<float, 18> resulti;
      result1i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        float expectedi = 0.0f;
        const float expected = cl::fract(x, &expectedi);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
        ASSERT_FLOAT_EQ(expectedi, resulti[i]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      std::array<cl::float2, 8> resulti;
      result2i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      std::array<cl::float3, 8> resulti;
      result3i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      std::array<cl::float4, 8> resulti;
      result4i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zFractImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result1i = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1i->setSize(18);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result2i = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2i->setSize(8);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result3i = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3i->setSize(8);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);
    auto result4i = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4i->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testzFractImpl, 1);
    kernel->run(*result1, *result1i, *result2, *result2i,
                *result3, *result3i, *result4, *result4i, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zFractImpl' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      std::array<float, 18> resulti;
      result1i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        float expectedi = 0.0f;
        const float expected = cl::fract(x, &expectedi);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
        ASSERT_FLOAT_EQ(expectedi, resulti[i]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      std::array<cl::float2, 8> resulti;
      result2i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      std::array<cl::float3, 8> resulti;
      result3i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      std::array<cl::float4, 8> resulti;
      result4i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zFractTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result1i = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1i->setSize(18);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result2i = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2i->setSize(8);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result3i = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3i->setSize(8);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);
    auto result4i = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4i->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), math, testzFract, 1);
    kernel->run(*result1, *result1i, *result2, *result2i,
                *result3, *result3i, *result4, *result4i, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zFract' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      std::array<float, 18> resulti;
      result1i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        float expectedi = 0.0f;
        const float expected = cl::fract(x, &expectedi);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
        ASSERT_FLOAT_EQ(expectedi, resulti[i]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      std::array<cl::float2, 8> resulti;
      result2i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      std::array<cl::float3, 8> resulti;
      result3i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      std::array<cl::float4, 8> resulti;
      result4i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          float expectedi = 0.0f;
          const float expected = cl::fract(x[j], &expectedi);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ModTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testMod, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'fmod' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        const float y = 1.0f;
        const float expected = std::fmod(x, y);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zModImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzModImpl, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zModImpl' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        const float y = 1.0f;
        const float expected = std::fmod(x, y);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zModTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testzMod, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zMod' func is wrong.";
    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        const float y = 1.0f;
        const float expected = std::fmod(x, y);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RadianTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto radian_result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    radian_result1->setSize(2);
    auto radian_result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    radian_result2->setSize(2);
    auto radian_result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    radian_result3->setSize(2);
    auto radian_result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    radian_result4->setSize(2);

    auto kernel = makeZinvulKernel(device.get(), math, testRadian, 1);
    kernel->run(*radian_result1, *radian_result2, *radian_result3, *radian_result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message1 = "The 'degrees' func is wrong.";
    const char* error_message2 = "The 'radians' func is wrong.";
    constexpr float pi = zisc::kPi<float>;
    {
      std::array<float, 2> result;
      radian_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1]) << error_message2;
    }
    {
      std::array<cl::float2, 2> result;
      radian_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
    }
    {
      std::array<cl::float3, 2> result;
      radian_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
    }
    {
      std::array<cl::float4, 2> result;
      radian_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) <<error_message1; 
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
      ASSERT_FLOAT_EQ(0.0f, result[0][3]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
      ASSERT_FLOAT_EQ(0.0f * pi, result[1][3]) << error_message2;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zRadianImplTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto radian_result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    radian_result1->setSize(2);
    auto radian_result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    radian_result2->setSize(2);
    auto radian_result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    radian_result3->setSize(2);
    auto radian_result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    radian_result4->setSize(2);

    auto kernel = makeZinvulKernel(device.get(), math, testzRadianImpl, 1);
    kernel->run(*radian_result1, *radian_result2, *radian_result3, *radian_result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message1 = "The 'zDegreesImpl' func is wrong.";
    const char* error_message2 = "The 'zRadiansImpl' func is wrong.";
    constexpr float pi = zisc::kPi<float>;
    {
      std::array<float, 2> result;
      radian_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1]) << error_message2;
    }
    {
      std::array<cl::float2, 2> result;
      radian_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
    }
    {
      std::array<cl::float3, 2> result;
      radian_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
    }
    {
      std::array<cl::float4, 2> result;
      radian_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) <<error_message1; 
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
      ASSERT_FLOAT_EQ(0.0f, result[0][3]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
      ASSERT_FLOAT_EQ(0.0f * pi, result[1][3]) << error_message2;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zRadianTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto radian_result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    radian_result1->setSize(2);
    auto radian_result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    radian_result2->setSize(2);
    auto radian_result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    radian_result3->setSize(2);
    auto radian_result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    radian_result4->setSize(2);

    auto kernel = makeZinvulKernel(device.get(), math, testzRadian, 1);
    kernel->run(*radian_result1, *radian_result2, *radian_result3, *radian_result4, {1}, 0);
    device->waitForCompletion();

    const char* error_message1 = "The 'zDegrees' func is wrong.";
    const char* error_message2 = "The 'zRadians' func is wrong.";
    constexpr float pi = zisc::kPi<float>;
    {
      std::array<float, 2> result;
      radian_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1]) << error_message2;
    }
    {
      std::array<cl::float2, 2> result;
      radian_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
    }
    {
      std::array<cl::float3, 2> result;
      radian_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
    }
    {
      std::array<cl::float4, 2> result;
      radian_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) <<error_message1; 
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
      ASSERT_FLOAT_EQ(0.0f, result[0][3]) << error_message1;
      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
      ASSERT_FLOAT_EQ(0.0f * pi, result[1][3]) << error_message2;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FrLdexpTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(3 * (resolution + 2));
    auto results_exp1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results_exp1->setSize(resolution + 2);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3 * resolution);
    auto results_exp2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results_exp2->setSize(resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3 * resolution);
    auto results_exp3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results_exp3->setSize(resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3 * resolution);
    auto results_exp4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results_exp4->setSize(resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testFrLdexp, 1);
    kernel->run(*results1, *results_exp1,
                *results2, *results_exp2,
                *results3, *results_exp3,
                *results4, *results_exp4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message1 = "The 'frexp' func is wrong.";
    const char* error_message2 = "The 'ldexp' func is wrong.";
    // Scalar
    {
      std::vector<float> results;
      results.resize(3 * (resolution + 2));
      std::vector<int32b> results_exp;
      results_exp.resize(resolution + 2);
      results1->read(results.data(), results.size(), 0, 0);
      results_exp1->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const float n = results[3 * i];
        int e = 0;
        float expected = std::frexp(n, &e);
        float result = results[3 * i + 1];
        if (i == results_exp.size() - 1)
          ASSERT_TRUE(std::isnan(result)) << error_message1;
        else
          ASSERT_FLOAT_EQ(expected, result) << error_message1;
        if (i < resolution)
          ASSERT_EQ(e, results_exp[i]) << error_message1;
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        if (i == results_exp.size() - 1)
          ASSERT_TRUE(std::isnan(result)) << error_message2;
        else
          ASSERT_FLOAT_EQ(expected, result) << error_message2;
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(3 * resolution);
      std::vector<cl::int2> results_exp;
      results_exp.resize(resolution);
      results2->read(results.data(), results.size(), 0, 0);
      results_exp2->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float2 n = results[3 * i];
        int e = 0;
        cl::float2 result1 = results[3 * i + 1];
        cl::float2 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(3 * resolution);
      std::vector<cl::int3> results_exp;
      results_exp.resize(resolution);
      results3->read(results.data(), results.size(), 0, 0);
      results_exp3->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float3 n = results[3 * i];
        int e = 0;
        cl::float3 result1 = results[3 * i + 1];
        cl::float3 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
  }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(3 * resolution);
      std::vector<cl::int4> results_exp;
      results_exp.resize(resolution);
      results4->read(results.data(), results.size(), 0, 0);
      results_exp4->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float4 n = results[3 * i];
        int e = 0;
        cl::float4 result1 = results[3 * i + 1];
        cl::float4 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zFrLdexpImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(3 * (resolution + 2));
    auto results_exp1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results_exp1->setSize(resolution + 2);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3 * resolution);
    auto results_exp2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results_exp2->setSize(resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3 * resolution);
    auto results_exp3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results_exp3->setSize(resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3 * resolution);
    auto results_exp4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results_exp4->setSize(resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzFrLdexpImpl, 1);
    kernel->run(*results1, *results_exp1,
                *results2, *results_exp2,
                *results3, *results_exp3,
                *results4, *results_exp4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message1 = "The 'zFrexpImpl' func is wrong.";
    const char* error_message2 = "The 'zLdexpImpl' func is wrong.";
    // Scalar
    {
      std::vector<float> results;
      results.resize(3 * (resolution + 2));
      std::vector<int32b> results_exp;
      results_exp.resize(resolution + 2);
      results1->read(results.data(), results.size(), 0, 0);
      results_exp1->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const float n = results[3 * i];
        int e = 0;
        float expected = std::frexp(n, &e);
        float result = results[3 * i + 1];
        if (i == results_exp.size() - 1)
          ASSERT_TRUE(std::isnan(result)) << error_message1;
        else
          ASSERT_FLOAT_EQ(expected, result) << error_message1;
        if (i < resolution)
          ASSERT_EQ(e, results_exp[i]) << error_message1;
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        if (i == results_exp.size() - 1)
          ASSERT_TRUE(std::isnan(result)) << error_message2;
        else
          ASSERT_FLOAT_EQ(expected, result) << error_message2;
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(3 * resolution);
      std::vector<cl::int2> results_exp;
      results_exp.resize(resolution);
      results2->read(results.data(), results.size(), 0, 0);
      results_exp2->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float2 n = results[3 * i];
        int e = 0;
        cl::float2 result1 = results[3 * i + 1];
        cl::float2 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(3 * resolution);
      std::vector<cl::int3> results_exp;
      results_exp.resize(resolution);
      results3->read(results.data(), results.size(), 0, 0);
      results_exp3->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float3 n = results[3 * i];
        int e = 0;
        cl::float3 result1 = results[3 * i + 1];
        cl::float3 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
  }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(3 * resolution);
      std::vector<cl::int4> results_exp;
      results_exp.resize(resolution);
      results4->read(results.data(), results.size(), 0, 0);
      results_exp4->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float4 n = results[3 * i];
        int e = 0;
        cl::float4 result1 = results[3 * i + 1];
        cl::float4 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zFrLdexpTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(3 * (resolution + 2));
    auto results_exp1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results_exp1->setSize(resolution + 2);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3 * resolution);
    auto results_exp2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results_exp2->setSize(resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3 * resolution);
    auto results_exp3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results_exp3->setSize(resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3 * resolution);
    auto results_exp4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results_exp4->setSize(resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzFrLdexp, 1);
    kernel->run(*results1, *results_exp1,
                *results2, *results_exp2,
                *results3, *results_exp3,
                *results4, *results_exp4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message1 = "The 'zFrexp' func is wrong.";
    const char* error_message2 = "The 'zLdexp' func is wrong.";
    // Scalar
    {
      std::vector<float> results;
      results.resize(3 * (resolution + 2));
      std::vector<int32b> results_exp;
      results_exp.resize(resolution + 2);
      results1->read(results.data(), results.size(), 0, 0);
      results_exp1->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const float n = results[3 * i];
        int e = 0;
        float expected = std::frexp(n, &e);
        float result = results[3 * i + 1];
        if (i == results_exp.size() - 1)
          ASSERT_TRUE(std::isnan(result)) << error_message1;
        else
          ASSERT_FLOAT_EQ(expected, result) << error_message1;
        if (i < resolution)
          ASSERT_EQ(e, results_exp[i]) << error_message1;
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        if (i == results_exp.size() - 1)
          ASSERT_TRUE(std::isnan(result)) << error_message2;
        else
          ASSERT_FLOAT_EQ(expected, result) << error_message2;
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(3 * resolution);
      std::vector<cl::int2> results_exp;
      results_exp.resize(resolution);
      results2->read(results.data(), results.size(), 0, 0);
      results_exp2->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float2 n = results[3 * i];
        int e = 0;
        cl::float2 result1 = results[3 * i + 1];
        cl::float2 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(3 * resolution);
      std::vector<cl::int3> results_exp;
      results_exp.resize(resolution);
      results3->read(results.data(), results.size(), 0, 0);
      results_exp3->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float3 n = results[3 * i];
        int e = 0;
        cl::float3 result1 = results[3 * i + 1];
        cl::float3 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
  }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(3 * resolution);
      std::vector<cl::int4> results_exp;
      results_exp.resize(resolution);
      results4->read(results.data(), results.size(), 0, 0);
      results_exp4->read(results_exp.data(), results_exp.size(), 0, 0);
      for (std::size_t i = 0; i < results_exp.size(); ++i) {
        const cl::float4 n = results[3 * i];
        int e = 0;
        cl::float4 result1 = results[3 * i + 1];
        cl::float4 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

namespace  {

template <std::size_t kAllowableUlps = 2, std::size_t kCautionedUlps = 5> 
void testFloat(const char* func_name,
    const float x,
    const float expected,
    const float result,
    std::size_t& num_of_trials,
    std::size_t& num_of_failures,
    zisc::CompensatedSummation<float>& sum_ulps,
    float& max_ulps,
    const bool print_inf_value = true,
    const bool print_nan_value = true)
{
  char name[256] = "";
  std::sprintf(name, func_name, zisc::cast<double>(x), zisc::cast<double>(result));
  if (std::isfinite(result)) {
    ++num_of_trials;
    if (!zisc::constant::isAlmostEqual<kAllowableUlps>(expected, result)) {
      ++num_of_failures;
      const float ulps = calcUlps(expected, result);
      sum_ulps.add(ulps);
      max_ulps = std::max(max_ulps, ulps);
      if (!isfinite(ulps) || (zisc::cast<float>(kCautionedUlps) <= ulps)) {
        std::cout << "  - " << name << " ulps: " << std::scientific << ulps << std::endl;
      }
    }
  }
  else if (std::isinf(result)) {
    if (print_inf_value)
      std::cout << "  - " << name;
    if (!std::isinf(expected)) {
      ++num_of_trials;
      ++num_of_failures;
      sum_ulps.add(4.0f);
      if (print_inf_value)
        std::cout << " is unexpected.";
    }
    if (print_inf_value)
      std::cout << std::endl;
  }
  else if (print_nan_value && std::isnan(result)) {
    if (print_inf_value)
    std::cout << "  - " << name;
    if (!std::isnan(expected)) {
      ++num_of_trials;
      ++num_of_failures;
      sum_ulps.add(4.0f);
      if (print_inf_value)
        std::cout << " is unexpected.";
    }
    if (print_inf_value)
      std::cout << std::endl;
  }
}

} // namespace 

TEST(MathTest, MakeNormalTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(4 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testMakeNormal, 1);
    kernel->run(*results1, *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'makeNormal' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(4 * resolution);
      results1->read(results.data(), results.size(), 0, 0);
      zinvul::math::KernelGroup k;
      for (std::size_t i = 0; i < resolution; ++i) {
        {
          const float x = cast<float>(2 * i) / cast<float>(resolution) - 1.0f;
          const float expected = k.makeNormal(x);
          const float result = results[4 * i + 1];
          char func_name[256] = "makeNormal(%g)=%g";
          ::testFloat<1>(func_name, x, expected, result,
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
        {
          const float x = cast<float>(i + 1) / cast<float>(resolution);
          const float expected = k.makeNormal(x);
          const float result = results[4 * i + 3];
          char func_name[256] = "makeNormal(%g)=%g";
          ::testFloat<1>(func_name, x, expected, result,
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, ExpTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testExp, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'exp' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::exp(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "exp(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "exp(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "exp(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "exp(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zExpImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzExpImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zExpImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::exp(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zExpImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "zExpImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "zExpImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "zExpImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zExpTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzExp, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zExp' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::exp(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zExp(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "zExp(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "zExp(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          char func_name[256] = "zExp(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, LogTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testLog, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'log' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "log(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "log(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "log(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "log(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zLogImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLogImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zLogImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zLogImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "zLogImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "zLogImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "zLogImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zLogTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLog, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zLog' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zLog(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "zLog(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "zLog(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          char func_name[256] = "zLog(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, Log2Test)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testLog2, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'log2' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log2(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "log2(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "log2(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "log2(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "log2(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zLog2ImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLog2Impl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zLo2Impl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log2(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zLog2Impl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "zLog2Impl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "zLog2Impl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "zLog2Impl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zLog2Test)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLog2, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zLo2' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log2(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zLog2(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "zLog2(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "zLog2(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          char func_name[256] = "zLog2(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, PowTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(9 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testPow, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(9 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.0f, z);
        const float result = results[9 * i + 1];
        char func_name[256] = "pow(0, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(0,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(1.0f, z);
        const float result = results[9 * i + 2];
        char func_name[256] = "pow(1, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(1,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      //! \todo pow(-1, exp) should be checked.
//      for (std::size_t i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-1.0f, z);
//        const float result = results[9 * i + 3];
//        char func_name[256] = "pow(-1, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(-1,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(2.0f, z);
        const float result = results[9 * i + 4];
        char func_name[256] = "pow(2, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.5f, z);
        const float result = results[9 * i + 5];
        char func_name[256] = "pow(0.5, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(0.5,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 6];
        char func_name[256] = "pow(inf, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(inf,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

//      for (std::size_t i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 7];
//        char func_name[256] = "pow(-inf, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(-inf,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
        const float result = results[9 * i + 8];
        char func_name[256] = "pow(NaN, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, true, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(NaN,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "pow(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "pow(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "pow(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'pow(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zPowImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(9 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzPowImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(9 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.0f, z);
        const float result = results[9 * i + 1];
        char func_name[256] = "zPowImpl(0, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(0,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(1.0f, z);
        const float result = results[9 * i + 2];
        char func_name[256] = "zPowImpl(1, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(1,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      //! \todo zPowImpl(-1, exp) should be checked.
//      for (std::size_t i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-1.0f, z);
//        const float result = results[9 * i + 3];
//        char func_name[256] = "zPowImpl(-1, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(-1,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(2.0f, z);
        const float result = results[9 * i + 4];
        char func_name[256] = "zPowImpl(2, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.5f, z);
        const float result = results[9 * i + 5];
        char func_name[256] = "zPowImpl(0.5, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(0.5,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 6];
        char func_name[256] = "zPowImpl(inf, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(inf,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

//      for (std::size_t i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 7];
//        char func_name[256] = "zPowImpl(-inf, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(-inf,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
        const float result = results[9 * i + 8];
        char func_name[256] = "zPowImpl(NaN, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, true, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(NaN,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPowImpl(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPowImpl(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPowImpl(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPowImpl(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zPowTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(9 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzPow, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(9 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.0f, z);
        const float result = results[9 * i + 1];
        char func_name[256] = "zPow(0, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(0,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(1.0f, z);
        const float result = results[9 * i + 2];
        char func_name[256] = "zPow(1, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(1,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      //! \todo zPow(-1, exp) should be checked.
//      for (std::size_t i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-1.0f, z);
//        const float result = results[9 * i + 3];
//        char func_name[256] = "zPow(-1, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(-1,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(2.0f, z);
        const float result = results[9 * i + 4];
        char func_name[256] = "zPow(2, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.5f, z);
        const float result = results[9 * i + 5];
        char func_name[256] = "zPow(0.5, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(0.5,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 6];
        char func_name[256] = "zPow(inf, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(inf,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

//      for (std::size_t i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 7];
//        char func_name[256] = "zPow(-inf, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(-inf,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
        const float result = results[9 * i + 8];
        char func_name[256] = "zPow(NaN, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, true, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(NaN,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPow(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPow(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPow(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPow(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zPownTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 32u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(9 * resolution);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzPown, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(9 * resolution);
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.0f, z);
        const float result = results[9 * i + 1];
        char func_name[256] = "zPown(0, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(0,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(1.0f, z);
        const float result = results[9 * i + 2];
        char func_name[256] = "zPown(1, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(1,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(-1.0f, z);
        const float result = results[9 * i + 3];
        char func_name[256] = "zPown(-1, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(-1,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(2.0f, z);
        const float result = results[9 * i + 4];
        char func_name[256] = "zPown(2, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.5f, z);
        const float result = results[9 * i + 5];
        char func_name[256] = "zPown(0.5, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(0.5,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 6];
        char func_name[256] = "zPown(inf, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false, true);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(inf,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      //! \todo zPow(-inf, exp) should be checked.
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 7];
        char func_name[256] = "zPown(-inf, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false, true);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(-inf,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));

      num_of_trials = 0;
      num_of_failures = 0;
      sum_ulps.set(0.0f);
      max_ulps = 0.0f;

      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
        const float result = results[9 * i + 8];
        char func_name[256] = "zPown(NaN, %g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps, true, false);
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(NaN,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPown(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPown(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          char func_name[256] = "zPown(2, %g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << "The 'zPown(2,x)' func is wrong." << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, RsqrtTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * resolution);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testRsqrt, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'rsqrt' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * resolution);
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[2 * i];
        const float expected = 1.0f / std::sqrt(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "rsqrt(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "rsqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "rsqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "rsqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zRsqrtImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * resolution);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzRsqrtImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zRsqrtImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * resolution);
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[2 * i];
        const float expected = 1.0f / std::sqrt(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zRsqrtImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "zRsqrtImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "zRsqrtImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "zRsqrtImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zRsqrtTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * resolution);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzRsqrt, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zRsqrt' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * resolution);
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[2 * i];
        const float expected = 1.0f / std::sqrt(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zRsqrt(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "zRsqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "zRsqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          char func_name[256] = "zRsqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, SqrtTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testSqrt, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'sqrt' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sqrt(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "sqrt(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "sqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "sqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "sqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zSqrtImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSqrtImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zSqrtImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sqrt(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zSqrtImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "zSqrtImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "zSqrtImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "zSqrtImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zSqrtTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSqrt, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zSqrt' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sqrt(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zSqrt(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "zSqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "zSqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          char func_name[256] = "zSqrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zCbrtTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzCbrt, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zCbrt' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::cbrt(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zCbrt(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cbrt(z[j]);
          char func_name[256] = "zCbrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cbrt(z[j]);
          char func_name[256] = "zCbrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cbrt(z[j]);
          char func_name[256] = "zCbrt(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SinTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testSin, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'sin' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sin(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "sin(%g)=%g";
        ::testFloat<2, 100000>(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "sin(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "sin(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "sin(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zSinImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSinImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zSinImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sin(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zSinImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "zSinImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "zSinImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "zSinImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zSinTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSinImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zSin' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sin(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zSin(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "zSin(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "zSin(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          char func_name[256] = "zSin(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, CosTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testCos, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'cos' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::cos(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "cos(%g)=%g";
        ::testFloat<2, 100000>(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "cos(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "cos(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "cos(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zCosImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzCosImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zCosImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::cos(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zCosImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "zCosImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "zCosImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "zCosImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zCosTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzCos, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zCos' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::cos(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zCos(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "zCos(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "zCos(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          char func_name[256] = "zCos(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zSincosImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(3 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSincosImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zSincosImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(3 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[3 * i];
        {
          const float expected = std::sin(z);
          const float result = results[3 * i + 1];
          char func_name[256] = "zSincosImpl(%g)=%g";
          ::testFloat(func_name, z, expected, result,
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
        {
          const float expected = std::cos(z);
          const float result = results[3 * i + 2];
          char func_name[256] = "zSincosImpl(%g)=%g";
          ::testFloat(func_name, z, expected, result,
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(3 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            char func_name[256] = "zSincosImpl(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            char func_name[256] = "zSincosImpl(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(3 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            char func_name[256] = "zSincosImpl(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            char func_name[256] = "zSincosImpl(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(3 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            char func_name[256] = "zSincosImpl(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            char func_name[256] = "zSincosImpl(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, zSincosTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(3 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(3 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(3 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(3 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSincos, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zSincos' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(3 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[3 * i];
        {
          const float expected = std::sin(z);
          const float result = results[3 * i + 1];
          char func_name[256] = "zSincos(%g)=%g";
          ::testFloat(func_name, z, expected, result,
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
        {
          const float expected = std::cos(z);
          const float result = results[3 * i + 2];
          char func_name[256] = "zSincos(%g)=%g";
          ::testFloat(func_name, z, expected, result,
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(3 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            char func_name[256] = "zSincos(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            char func_name[256] = "zSincos(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(3 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            char func_name[256] = "zSincos(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            char func_name[256] = "zSincos(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(3 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            char func_name[256] = "zSincos(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            char func_name[256] = "zSincos(%g)=%g";
            ::testFloat(func_name, z[j], expected, result[j],
                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
          }
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, TanTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testTan, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'tan' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::tan(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "tan(%g)=%g";
        ::testFloat<2, 100000>(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "tan(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "tan(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "tan(%g)=%g";
          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zTanImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzTanImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zTanImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::tan(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zTanImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "zTanImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "zTanImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "zTanImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zTanTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzTan, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zTan' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::tan(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zTan(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "zTan(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "zTan(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          char func_name[256] = "zTan(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, AsinTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAsin, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'asin' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::asin(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "asin(%g)=%g";
        ::testFloat<2, 10000>(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "asin(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "asin(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "asin(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zAsinImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAsinImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zAsinImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::asin(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zAsinImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "zAsinImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "zAsinImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "zAsinImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zAsinTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAsin, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zAsin' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::asin(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zAsin(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "zAsin(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "zAsin(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          char func_name[256] = "zAsin(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, AcosTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAcos, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'acos' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::acos(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "acos(%g)=%g";
        ::testFloat<2, 10000>(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "acos(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "acos(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "acos(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zAcosImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAcosImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zAcosImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::acos(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zAcosImpl(%g)=%g";
        ::testFloat<2, 10000>(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "zAcosImpl(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "zAcosImpl(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "zAcosImpl(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zAcosTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 2));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAcos, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zAcos' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::acos(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zAcos(%g)=%g";
        ::testFloat<2, 10000>(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "zAcos(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "zAcos(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          char func_name[256] = "zAcos(%g)=%g";
          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, AtanTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAtan, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'atan' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::atan(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "atan(%g)=%g";
        ::testFloat<2, 100>(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "atan(%g)=%g";
          ::testFloat<2, 100>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "atan(%g)=%g";
          ::testFloat<2, 100>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "atan(%g)=%g";
          ::testFloat<2, 100>(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zAtanImplTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAtanImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zAtanImpl' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::atan(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zAtanImpl(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "zAtanImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "zAtanImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "zAtanImpl(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}

TEST(MathTest, zAtanTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(2 * (resolution + 3));
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(2 * resolution);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(2 * resolution);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAtan, 1);
    kernel->run(*results1, *results2, *results3, *results4,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    const char* error_message = "The 'zAtan' func is wrong.";
    // Scalar
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      results1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::atan(z);
        const float result = results[2 * i + 1];
        char func_name[256] = "zAtan(%g)=%g";
        ::testFloat(func_name, z, expected, result,
                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector2
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      results2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "zAtan(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector3
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      results3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "zAtan(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    // Vector4
    {
      std::size_t num_of_trials = 0;
      std::size_t num_of_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps{0.0f};
      float max_ulps = 0.0f;

      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      results4->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          char func_name[256] = "zAtan(%g)=%g";
          ::testFloat(func_name, z[j], expected, result[j],
                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
        }
      }
      EXPECT_FALSE(num_of_failures)
          << error_message << std::endl
          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
          << "maxulps=" << std::scientific << max_ulps << ", "
          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
    }

    std::cout << std::endl;
  }
}
