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
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/math.hpp"
// Test
#include "test.hpp"

#define ASSERT_FLOAT_NEAR(val1, val2, abs_error) ASSERT_NEAR(static_cast<double>(val1), static_cast<double>(val2), static_cast<double>(abs_error))
#define EXPECT_FLOAT_NEAR(val1, val2, abs_error) EXPECT_NEAR(static_cast<double>(val1), static_cast<double>(val2), static_cast<double>(abs_error))

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

    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i])
            << "The clz func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The clz func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The clz func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The clz func is wrong: " << x;
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

    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i])
            << "The clz func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The clz func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The clz func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The clz func is wrong: " << x;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZclzTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZclz, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i])
            << "The zClz func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zClz2 func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zClz3 func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zClz4 func is wrong: " << x;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZclzUTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZclzU, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::clz(x);
        ASSERT_EQ(expected, result[i])
            << "The zClzU func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zClzU2 func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zClzU3 func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zClzU4 func is wrong: " << x;
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

    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i])
            << "The popcount func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The popcount func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The popcount func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The popcount func is wrong: " << x;
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

    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i])
            << "The popcount func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The popcount func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The popcount func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The popcount func is wrong: " << x;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZpopcountTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZpopcount, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<int32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        int32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const int32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i])
            << "The zPopcount func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zPopcount2 func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zPopcount3 func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zPopcount4 func is wrong: " << x;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZpopcountUTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZpopcountU, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<uint32b, 32> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        uint32b x = 0;
        for (std::size_t j = 0; j < i; ++j)
          x = (x << 1) | 0b1;
        const uint32b expected = cl::popcount(x);
        ASSERT_EQ(expected, result[i])
            << "The zPopcountU func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zPopcountU2 func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zPopcountU3 func is wrong: " << x;
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
          ASSERT_EQ(expected, result[i][j])
              << "The zPopcountU4 func is wrong: " << x;
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

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      std::array<float, 18> resulti;
      result1i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        float expectedi = 0.0f;
        const float expected = cl::fract(x, &expectedi);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The fract func is wrong: " << std::scientific << x;
        ASSERT_FLOAT_EQ(expectedi, resulti[i])
            << "The fract func is wrong: " << std::scientific << x;
      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 5]))
//          << "The fract func is wrong.";
//      ASSERT_TRUE(std::isnan(result[result.size() - 3]))
//          << "The fract func is wrong.";
//      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
//          << "The fract func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The fract func is wrong.";
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j])
              << "The fract func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The fract func is wrong.";
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j])
              << "The fract func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The fract func is wrong.";
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j])
              << "The fract func is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZfractTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZfract, 1);
    kernel->run(*result1, *result1i, *result2, *result2i,
                *result3, *result3i, *result4, *result4i, {1}, 0);
    device->waitForCompletion();

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      std::array<float, 18> resulti;
      result1i->read(resulti.data(), resulti.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        float expectedi = 0.0f;
        const float expected = cl::fract(x, &expectedi);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The zFract func is wrong: " << std::scientific << x;
        ASSERT_FLOAT_EQ(expectedi, resulti[i])
            << "The zFract func is wrong: " << std::scientific << x;
      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 5]))
//          << "The zFract func is wrong.";
//      ASSERT_TRUE(std::isnan(result[result.size() - 3]))
//          << "The zFract func is wrong.";
//      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
//          << "The zFract func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zFract func is wrong.";
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j])
              << "The zFract func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zFract func is wrong.";
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j])
              << "The zFract func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zFract func is wrong.";
          ASSERT_FLOAT_EQ(expectedi, resulti[i][j])
              << "The zFract func is wrong.";
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

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        const float y = 1.0f;
        const float expected = std::fmod(x, y);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The fmod func is wrong.";
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 5]))
          << "The fmod func is wrong.";
      ASSERT_TRUE(std::isnan(result[result.size() - 3]))
          << "The fmod func is wrong.";
      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
          << "The fmod func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The fmod func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The fmod func is wrong: " << std::scientific << x[j];
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The fmod func is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZmodTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZmod, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 3; ++i) {
        const float x = result[2 * i];
        const float y = 1.0f;
        const float expected = std::fmod(x, y);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The zmod func is wrong.";
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 5]))
          << "The zmod func is wrong.";
      ASSERT_TRUE(std::isnan(result[result.size() - 3]))
          << "The zmod func is wrong.";
      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
          << "The zmod func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float y = zisc::cast<float>(j + 1);
          const float expected = std::fmod(x[j], y);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zmod func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zmod func is wrong: " << std::scientific << x[j];
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zmod func is wrong.";
        }
      }
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

    auto result1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1->setSize(36);
    auto result2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2->setSize(16);
    auto result3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3->setSize(16);
    auto result4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), math, testCeil, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::ceil(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The ceil func is wrong.";
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
          << "The ceil func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The ceil func is wrong.";
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The ceil func is wrong: " << std::scientific << x[j];
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The ceil func is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZceilTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZceil, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::ceil(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The zCeil func is wrong.";
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
          << "The Zceil func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The Zceil2 func is wrong.";
        }
      }
    }
    {
      std::array<cl::float3, 16> result;
      result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 3; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zCeil3 func is wrong: " << std::scientific << x[j];
        }
      }
    }
    {
      std::array<cl::float4, 16> result;
      result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 4; ++j) {
          const float expected = std::ceil(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zCeil4 func is wrong.";
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

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::floor(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The floor func is wrong.";
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
          << "The floor func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The floor func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The floor func is wrong: " << std::scientific << x[j];
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The floor func is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZfloorTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZfloor, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::floor(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The zTrunc func is wrong.";
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
          << "The zFloor func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::floor(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zFloor2 func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zFloor3 func is wrong: " << std::scientific << x[j];
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zFloor4 func is wrong.";
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

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::trunc(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The trunc func is wrong.";
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
          << "The trunc func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The trunc func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The trunc func is wrong: " << std::scientific << x[j];
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The trunc func is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZtruncTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZtrunc, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::trunc(x);
        EXPECT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The zTrunc func is wrong: " << std::scientific << x;
      }
      EXPECT_TRUE(std::isnan(result[result.size() - 1]))
          << "The Ztrunc func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::trunc(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The Ztrunc2 func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zTrunc3 func is wrong: " << std::scientific << x[j];
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zTrunc4 func is wrong.";
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

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::round(x);
        ASSERT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The round func is wrong: " << std::scientific << x;
      }
      ASSERT_TRUE(std::isnan(result[result.size() - 1]))
          << "The round func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The round func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The round func is wrong: " << std::scientific << x[j];
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The round func is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZroundTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZround, 1);
    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<float, 36> result;
      result1->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < (result.size() / 2) - 1; ++i) {
        const float x = result[2 * i];
        const float expected = std::round(x);
        EXPECT_FLOAT_EQ(expected, result[2 * i + 1])
            << "The zTrunc func is wrong: " << std::scientific << x;
      }
      EXPECT_TRUE(std::isnan(result[result.size() - 1]))
          << "The zRound func is wrong.";
    }
    {
      std::array<cl::float2, 16> result;
      result2->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size() / 2; ++i) {
        const auto x = result[2 * i];
        for (std::size_t j = 0; j < 2; ++j) {
          const float expected = std::round(x[j]);
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zRound2 func is wrong.";
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zRound3 func is wrong: " << std::scientific << x[j];
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
          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j])
              << "The zRound4 func is wrong.";
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

    {
      std::array<float, 3> result;
      mix_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[2]) << "The mix func is wrong.";
    }
    {
      std::array<cl::float2, 6> result;
      mix_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[3][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << "The mix func is wrong.";
    }
    {
      std::array<cl::float3, 6> result;
      mix_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << "The mix func is wrong.";
    }
    {
      std::array<cl::float4, 6> result;
      mix_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-4.0f, result[0][3]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][3]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(4.0f, result[2][3]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-4.0f, result[3][3]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[4][3]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << "The mix func is wrong.";
      ASSERT_FLOAT_EQ(4.0f, result[5][3]) << "The mix func is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZmixTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZmix, 1);
    kernel->run(*mix_result1, *mix_result2, *mix_result3, *mix_result4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<float, 3> result;
      mix_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[2]) << "The zMix is wrong.";
    }
    {
      std::array<cl::float2, 6> result;
      mix_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[3][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << "The zMix is wrong.";
    }
    {
      std::array<cl::float3, 6> result;
      mix_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << "The zMix is wrong.";
    }
    {
      std::array<cl::float4, 6> result;
      mix_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-4.0f, result[0][3]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[1][3]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(4.0f, result[2][3]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-4.0f, result[3][3]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[4][3]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << "The zMix is wrong.";
      ASSERT_FLOAT_EQ(4.0f, result[5][3]) << "The zMix is wrong.";
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

    constexpr float pi = zisc::kPi<float>;
    {
      std::array<float, 2> result;
      radian_result1->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(pi, result[1]) << "The radians func is wrong.";
    }
    {
      std::array<cl::float2, 2> result;
      radian_result2->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(pi, result[1][0]) << "The radians func is wrong.";
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << "The radians func is wrong.";
    }
    {
      std::array<cl::float3, 2> result;
      radian_result3->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(pi, result[1][0]) << "The radians func is wrong.";
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << "The radians func is wrong.";
      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << "The radians func is wrong.";
    }
    {
      std::array<cl::float4, 2> result;
      radian_result4->read(result.data(), result.size(), 0, 0);
      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[0][3]) << "The degrees func is wrong.";
      ASSERT_FLOAT_EQ(pi, result[1][0]) << "The radians func is wrong.";
      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << "The radians func is wrong.";
      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << "The radians func is wrong.";
      ASSERT_FLOAT_EQ(0.0f * pi, result[1][3]) << "The radians func is wrong.";
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

TEST(MathTest, ZRadianTest)
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

    auto kernel = makeZinvulKernel(device.get(), math, testZRadian, 1);
    kernel->run(*radian_result1, *radian_result2, *radian_result3, *radian_result4, {1}, 0);
    device->waitForCompletion();

    constexpr float pi = zisc::kPi<float>;
    {
      std::array<float, 2> result;
      radian_result1->read(result.data(), result.size(), 0, 0);
      EXPECT_FLOAT_EQ(180.0f, result[0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[1]) << "The radians func is wrong.";
    }
    {
      std::array<cl::float2, 2> result;
      radian_result2->read(result.data(), result.size(), 0, 0);
      EXPECT_FLOAT_EQ(180.0f, result[0][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[0][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[1][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[1][1]) << "The radians func is wrong.";
    }
    {
      std::array<cl::float3, 2> result;
      radian_result3->read(result.data(), result.size(), 0, 0);
      EXPECT_FLOAT_EQ(180.0f, result[0][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[0][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(45.0f, result[0][2]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[1][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[1][1]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.25f * pi, result[1][2]) << "The radians func is wrong.";
    }
    {
      std::array<cl::float4, 2> result;
      radian_result4->read(result.data(), result.size(), 0, 0);
      EXPECT_FLOAT_EQ(180.0f, result[0][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[0][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(45.0f, result[0][2]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(0.0f, result[0][3]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[1][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[1][1]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.25f * pi, result[1][2]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.0f * pi, result[1][3]) << "The radians func is wrong.";
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

    constexpr std::size_t n = 10;
    auto buffer1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(n);
    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    buffer2->setSize(n);

    auto kernel = makeZinvulKernel(device.get(), math, testMaxFunction, 1);
    kernel->run(*buffer1, *buffer2, {1}, 0);
    device->waitForCompletion();

    {
      std::size_t index = 0;
      std::array<int32b, n> result;
      buffer1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(std::numeric_limits<int>::max(), result[index++]) << "The max func is wrong.";
      ASSERT_EQ(std::numeric_limits<int>::max(), result[index++]) << "The max func is wrong.";
      ASSERT_FALSE(result[index++]) << "The max func is wrong.";
      ASSERT_FALSE(result[index++]) << "The max func is wrong.";
      ASSERT_TRUE(result[index++]) << "The max func is wrong.";
      ASSERT_TRUE(result[index++]) << "The max func is wrong.";
      ASSERT_EQ(-1, result[index++]) << "The max func is wrong.";
      ASSERT_EQ(-1, result[index++]) << "The max func is wrong.";
      ASSERT_EQ(10, result[index++]) << "The max func is wrong.";
      ASSERT_EQ(10, result[index++]) << "The max func is wrong.";
    }
    {
      std::size_t index = 0;
      std::array<float, n> result;
      buffer2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(std::numeric_limits<float>::max(), result[index++]) << "The max func is wrong.";
//      ASSERT_FLOAT_EQ(std::numeric_limits<float>::max(), result[index++]) << "The max func is wrong.";
      index = 2;
      ASSERT_FLOAT_EQ(0.0f, result[index++]) << "The max func is wrong.";
      ASSERT_FLOAT_EQ(0.0f, result[index++]) << "The max func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[index++]) << "The max func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[index++]) << "The max func is wrong.";
      ASSERT_FLOAT_EQ(-1, result[index++]) << "The max func is wrong.";
      ASSERT_FLOAT_EQ(-1, result[index++]) << "The max func is wrong.";
      ASSERT_FLOAT_EQ(10, result[index++]) << "The max func is wrong.";
      ASSERT_FLOAT_EQ(10, result[index++]) << "The max func is wrong.";
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

    constexpr std::size_t n = 10;
    auto buffer1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(n);
    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    buffer2->setSize(n);

    auto kernel = makeZinvulKernel(device.get(), math, testMinFunction, 1);
    kernel->run(*buffer1, *buffer2, {1}, 0);
    device->waitForCompletion();

    {
      std::size_t index = 0;
      std::array<int32b, n> result;
      buffer1->read(result.data(), result.size(), 0, 0);
      ASSERT_EQ(std::numeric_limits<int>::lowest(), result[index++]) << "The min func is wrong.";
      ASSERT_EQ(std::numeric_limits<int>::lowest(), result[index++]) << "The min func is wrong.";
      ASSERT_EQ(-1, result[index++]) << "The min func is wrong.";
      ASSERT_EQ(-1, result[index++]) << "The min func is wrong.";
      ASSERT_EQ(-1, result[index++]) << "The min func is wrong.";
      ASSERT_EQ(-1, result[index++]) << "The min func is wrong.";
      ASSERT_EQ(-10, result[index++]) << "The min func is wrong.";
      ASSERT_EQ(-10, result[index++]) << "The min func is wrong.";
      ASSERT_TRUE(result[index++]) << "The min func is wrong.";
      ASSERT_TRUE(result[index++]) << "The min func is wrong.";
    }
    {
      std::size_t index = 0;
      std::array<float, n> result;
      buffer2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(std::numeric_limits<float>::lowest(), result[index++]) << "The min func is wrong.";
//      ASSERT_FLOAT_EQ(std::numeric_limits<float>::lowest(), result[index++]) << "The min func is wrong.";
      index = 2;
      ASSERT_FLOAT_EQ(-1.0f, result[index++]) << "The min func is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[index++]) << "The min func is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[index++]) << "The min func is wrong.";
      ASSERT_FLOAT_EQ(-1.0f, result[index++]) << "The min func is wrong.";
      ASSERT_FLOAT_EQ(-10.0f, result[index++]) << "The min func is wrong.";
      ASSERT_FLOAT_EQ(-10.0f, result[index++]) << "The min func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[index++]) << "The min func is wrong.";
      ASSERT_FLOAT_EQ(1.0f, result[index++]) << "The min func is wrong.";
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(3 * (resolution + 2));
    auto result_exp1_buffer = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result_exp1_buffer->setSize(resolution + 2);
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(3 * resolution);
    auto result_exp2_buffer = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result_exp2_buffer->setSize(resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(3 * resolution);
    auto result_exp3_buffer = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result_exp3_buffer->setSize(resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(3 * resolution);
    auto result_exp4_buffer = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result_exp4_buffer->setSize(resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testFrLdexp, 1);
    kernel->run(*result1_buffer, *result_exp1_buffer,
                *result2_buffer, *result_exp2_buffer,
                *result3_buffer, *result_exp3_buffer,
                *result4_buffer, *result_exp4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    FAIL();

    // Scalar
    {
      std::vector<float> results;
      results.resize(3 * (resolution + 2));
      std::vector<int32b> result_exp;
      result_exp.resize(resolution + 2);
      result1_buffer->read(results.data(), results.size(), 0, 0);
      result_exp1_buffer->read(result_exp.data(), result_exp.size(), 0, 0);
      for (std::size_t i = 0; i < result_exp.size(); ++i) {
        const float n = results[3 * i];
        int e = 0;
        float expected = std::frexp(n, &e);
        float result = results[3 * i + 1];
        if (i == result_exp.size() - 1) {
          EXPECT_TRUE(std::isnan(result))
              << "frexp(" << n << ") isn't recommended to use of.";
        }
        else {
          EXPECT_FLOAT_EQ(expected, result)
              << "frexp(" << n << ") isn't recommended to use of.";
        }
        if (i < resolution) {
          EXPECT_EQ(e, result_exp[i])
              << "frexp(" << n << ") isn't recommended to use of.";
        }
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        if (i == result_exp.size() - 1) {
          EXPECT_TRUE(std::isnan(result))
              << "ldexp(" << m << "," << e << ") isn't recommended to use of.";
        }
        else {
          EXPECT_FLOAT_EQ(expected, result)
              << "ldexp(" << m << "," << e << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(3 * resolution);
      std::vector<cl::int2> result_exp;
      result_exp.resize(resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      result_exp2_buffer->read(result_exp.data(), result_exp.size(), 0, 0);
      for (std::size_t i = 0; i < result_exp.size(); ++i) {
        const cl::float2 n = results[3 * i];
        int e = 0;
        cl::float2 result1 = results[3 * i + 1];
        cl::float2 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j])
              << "frexp(" << n[j] << ") isn't recommended to use of.";
          ASSERT_EQ(e, result_exp[i][j])
              << "frexp(" << n[j] << ") isn't recommended to use of.";
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j])
              << "ldexp(" << m << "," << e << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(3 * resolution);
      std::vector<cl::int3> result_exp;
      result_exp.resize(resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      result_exp3_buffer->read(result_exp.data(), result_exp.size(), 0, 0);
      for (std::size_t i = 0; i < result_exp.size(); ++i) {
        const cl::float3 n = results[3 * i];
        int e = 0;
        cl::float3 result1 = results[3 * i + 1];
        cl::float3 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j])
              << "frexp(" << n[j] << ") isn't recommended to use of.";
          ASSERT_EQ(e, result_exp[i][j])
              << "frexp(" << n[j] << ") isn't recommended to use of.";
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j])
              << "ldexp(" << m << "," << e << ") isn't recommended to use of.";
        }
      }
  }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(3 * resolution);
      std::vector<cl::int4> result_exp;
      result_exp.resize(resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      result_exp4_buffer->read(result_exp.data(), result_exp.size(), 0, 0);
      for (std::size_t i = 0; i < result_exp.size(); ++i) {
        const cl::float4 n = results[3 * i];
        int e = 0;
        cl::float4 result1 = results[3 * i + 1];
        cl::float4 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j])
              << "frexp(" << n[j] << ") isn't recommended to use of.";
          ASSERT_EQ(e, result_exp[i][j])
              << "frexp(" << n[j] << ") isn't recommended to use of.";
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j])
              << "ldexp(" << m << "," << e << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZFrLdexpTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(3 * (resolution + 2));
    auto result_exp1_buffer = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    result_exp1_buffer->setSize(resolution + 2);
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(3 * resolution);
    auto result_exp2_buffer = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    result_exp2_buffer->setSize(resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(3 * resolution);
    auto result_exp3_buffer = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    result_exp3_buffer->setSize(resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(3 * resolution);
    auto result_exp4_buffer = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    result_exp4_buffer->setSize(resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZFrLdexp, 1);
    kernel->run(*result1_buffer, *result_exp1_buffer,
                *result2_buffer, *result_exp2_buffer,
                *result3_buffer, *result_exp3_buffer,
                *result4_buffer, *result_exp4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(3 * (resolution + 2));
      std::vector<int32b> result_exp;
      result_exp.resize(resolution + 2);
      result1_buffer->read(results.data(), results.size(), 0, 0);
      result_exp1_buffer->read(result_exp.data(), result_exp.size(), 0, 0);
      for (std::size_t i = 0; i < result_exp.size(); ++i) {
        const float n = results[3 * i];
        int e = 0;
        float expected = std::frexp(n, &e);
        float result = results[3 * i + 1];
        if (i == result_exp.size() - 1)
          EXPECT_TRUE(std::isnan(result)) << "zFrexp(" << n << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "zFrexp(" << n << ") is wrong.";
        if (i < resolution)
          EXPECT_EQ(e, result_exp[i]) << "zFrexp(" << n << ") is wrong.";
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        if (i == result_exp.size() - 1)
          EXPECT_TRUE(std::isnan(result)) << "zLdexp(" << m << "," << e << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "zLdexp(" << m << "," << e << ") is wrong.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(3 * resolution);
      std::vector<cl::int2> result_exp;
      result_exp.resize(resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      result_exp2_buffer->read(result_exp.data(), result_exp.size(), 0, 0);
      for (std::size_t i = 0; i < result_exp.size(); ++i) {
        const cl::float2 n = results[3 * i];
        int e = 0;
        cl::float2 result1 = results[3 * i + 1];
        cl::float2 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << "zFrexp2(" << n[j] << ") is wrong.";
          ASSERT_EQ(e, result_exp[i][j]) << "zFrexp2(" << n[j] << ") is wrong.";
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << "zLdexp2(" << m << "," << e << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(3 * resolution);
      std::vector<cl::int3> result_exp;
      result_exp.resize(resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      result_exp3_buffer->read(result_exp.data(), result_exp.size(), 0, 0);
      for (std::size_t i = 0; i < result_exp.size(); ++i) {
        const cl::float3 n = results[3 * i];
        int e = 0;
        cl::float3 result1 = results[3 * i + 1];
        cl::float3 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << "zFrexp3(" << n[j] << ") is wrong.";
          ASSERT_EQ(e, result_exp[i][j]) << "zFrexp3(" << n[j] << ") is wrong.";
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << "zLdexp3(" << m << "," << e << ") is wrong.";
        }
      }
  }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(3 * resolution);
      std::vector<cl::int4> result_exp;
      result_exp.resize(resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      result_exp4_buffer->read(result_exp.data(), result_exp.size(), 0, 0);
      for (std::size_t i = 0; i < result_exp.size(); ++i) {
        const cl::float4 n = results[3 * i];
        int e = 0;
        cl::float4 result1 = results[3 * i + 1];
        cl::float4 result2 = results[3 * i + 2];
        for (std::size_t j = 0; j < result1.size(); ++j) {
          float expected = std::frexp(n[j], &e);
          ASSERT_FLOAT_EQ(expected, result1[j]) << "zFrexp4(" << n[j] << ") is wrong.";
          ASSERT_EQ(e, result_exp[i][j]) << "zFrexp4(" << n[j] << ") is wrong.";
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << "zLdexp4(" << m << "," << e << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(9 * (resolution + 3));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testPow, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(9 * (resolution + 3));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.0f, z);
        const float result = results[9 * i + 1];
        if (i == resolution + 2) {
          ASSERT_TRUE(std::isnan(expected))
              << "The reference of pow(0," << z << ") is wrong.";
          ASSERT_TRUE(std::isnan(result))
              << "pow(0," << z << ") isn't recommended to use of .";
        }
        else {
          ASSERT_FLOAT_EQ(expected, result)
              << "pow(0," << z << ") isn't recommended to use of .";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(1.0f, z);
        const float result = results[9 * i + 2];
        ASSERT_FLOAT_EQ(expected, result)
            << "pow(1," << z << ") isn't recommended to use of .";
      }
      //! \todo pow(-1, exp) should be checked.
//      for (std::size_t i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-1.0f, z);
//        const float result = results[9 * i + 3];
//        if ((zisc::cast<float>(zisc::cast<int>(z)) == z) || std::isinf(z)) {
//          ASSERT_FLOAT_EQ(expected, result)
//              << "pow(-1," << z << ") isn't recommended to use of .";
//        }
//        else {
//          ASSERT_TRUE(std::isnan(expected))
//              << "The reference of pow(-1," << z << ") is wrong.";
//          ASSERT_TRUE(std::isnan(result))
//              << "pow(-1," << z << ") isn't recommended to use of .";
//        }
//      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(2.0f, z);
        const float result = results[9 * i + 4];
        if (i == resolution + 2) {
          ASSERT_TRUE(std::isnan(expected))
              << "The reference of pow(2," << z << ") is wrong.";
          ASSERT_TRUE(std::isnan(result))
              << "pow(2," << z << ") isn't recommended to use of .";
        }
        else {
          ASSERT_FLOAT_EQ(expected, result)
              << "pow(2," << z << ") isn't recommended to use of .";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.5f, z);
        const float result = results[9 * i + 5];
        if (i == resolution + 2) {
          ASSERT_TRUE(std::isnan(expected))
              << "The reference of pow(0.5," << z << ") is wrong.";
          ASSERT_TRUE(std::isnan(result))
              << "pow(0.5," << z << ") isn't recommended to use of .";
        }
        else {
          ASSERT_FLOAT_EQ(expected, result)
              << "pow(0.5," << z << ") isn't recommended to use of .";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 6];
        if (i == resolution + 2) {
          ASSERT_TRUE(std::isnan(expected))
              << "The reference of pow(inf," << z << ") is wrong.";
          ASSERT_TRUE(std::isnan(result))
              << "pow(inf," << z << ") isn't recommended to use of .";
        }
        else {
          ASSERT_FLOAT_EQ(expected, result)
              << "pow(inf," << z << ") isn't recommended to use of .";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 7];
        if (i == resolution + 2) {
          ASSERT_TRUE(std::isnan(expected))
              << "The reference of pow(-inf," << z << ") is wrong.";
          ASSERT_TRUE(std::isnan(result))
              << "pow(-inf," << z << ") isn't recommended to use of .";
        }
        else {
          //! \todo pow(-inf, exp) should be checked.
//          ASSERT_FLOAT_EQ(expected, result)
//              << "pow(-inf," << z << ") isn't recommended to use of .";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
        const float result = results[9 * i + 8];
        if (z == 0.0f) {
          ASSERT_FALSE(std::isnan(expected))
              << "The reference of pow(NaN," << z << ") is wrong.";
          ASSERT_FALSE(std::isnan(result))
              << "pow(NaN," << z << ") isn't recommended to use of .";
        }
        else {
          ASSERT_TRUE(std::isnan(expected))
              << "The reference of pow(NaN," << z << ") is wrong.";
          ASSERT_TRUE(std::isnan(result))
              << "pow(NaN," << z << ") isn't recommended to use of .";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "pow(2," << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "pow(2," << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "pow(2," << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZpowTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(9 * (resolution + 3));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZpow, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(9 * (resolution + 3));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.0f, z);
        const float result = results[9 * i + 1];
        if (i == resolution + 2) {
          EXPECT_TRUE(std::isnan(expected))
              << "The reference of pow(0," << z << ") is wrong.";
          EXPECT_TRUE(std::isnan(result))
              << "zPow(0," << z << ") is wrong.";
        }
        else {
          EXPECT_FLOAT_EQ(expected, result)
              << "zPow(0," << z << ") is wrong.";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(1.0f, z);
        const float result = results[9 * i + 2];
        EXPECT_FLOAT_EQ(expected, result)
            << "zPow(1," << z << ") is wrong.";
      }
      //! \todo pow(-1, exp) should be checked.
//      for (std::size_t i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-1.0f, z);
//        const float result = results[9 * i + 3];
//        if ((zisc::cast<float>(zisc::cast<int>(z)) == z) || std::isinf(z)) {
//          EXPECT_FLOAT_EQ(expected, result)
//              << "zPow(-1," << z << ") is wrong.";
//        }
//        else {
//          EXPECT_TRUE(std::isnan(expected))
//              << "The reference of pow(-1," << z << ") is wrong.";
//          EXPECT_TRUE(std::isnan(result))
//              << "zPow(-1," << z << ") is wrong.";
//        }
//      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(2.0f, z);
        const float result = results[9 * i + 4];
        if (i == resolution + 2) {
          EXPECT_TRUE(std::isnan(expected))
              << "The reference of pow(2," << z << ") is wrong.";
          EXPECT_TRUE(std::isnan(result))
              << "zPow(2," << z << ") is wrong.";
        }
        else {
          EXPECT_FLOAT_EQ(expected, result)
              << "zPow(2," << z << ") is wrong.";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.5f, z);
        const float result = results[9 * i + 5];
        if (i == resolution + 2) {
          EXPECT_TRUE(std::isnan(expected))
              << "The reference of pow(0.5," << z << ") is wrong.";
          EXPECT_TRUE(std::isnan(result))
              << "zPow(0.5," << z << ") is wrong.";
        }
        else {
          EXPECT_FLOAT_EQ(expected, result)
              << "zPow(0.5," << z << ") is wrong.";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 6];
        if (i == resolution + 2) {
          EXPECT_TRUE(std::isnan(expected))
              << "The reference of pow(inf," << z << ") is wrong.";
          EXPECT_TRUE(std::isnan(result))
              << "zPow(inf," << z << ") is wrong.";
        }
        else {
          EXPECT_FLOAT_EQ(expected, result)
              << "zPow(inf," << z << ") is wrong.";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 7];
        if (i == resolution + 2) {
          EXPECT_TRUE(std::isnan(expected))
              << "The reference of pow(-inf," << z << ") is wrong.";
          EXPECT_TRUE(std::isnan(result))
              << "zPow(-inf," << z << ") is wrong.";
        }
        else {
          //! \todo zPow(-inf, exp) should be checked.
//          EXPECT_FLOAT_EQ(expected, result)
//              << "zPow(-inf," << z << ") is wrong.";
        }
      }
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
        const float result = results[9 * i + 8];
        if (z == 0.0f) {
          EXPECT_FALSE(std::isnan(expected))
              << "The reference of pow(NaN," << z << ") is wrong.";
          EXPECT_FALSE(std::isnan(result))
              << "zPow(NaN," << z << ") is wrong.";
        }
        else {
          EXPECT_TRUE(std::isnan(expected))
              << "The reference of pow(NaN," << z << ") is wrong.";
          EXPECT_TRUE(std::isnan(result))
              << "zPow(NaN," << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "zPow(2," << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "zPow(2," << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "zPow(2," << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZpownTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 32u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(9 * resolution);
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZpown, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(9 * resolution);
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.0f, z);
        const float result = results[9 * i + 1];
        EXPECT_FLOAT_EQ(expected, result) << "zPown(0," << z << ") is wrong.";
      }
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(1.0f, z);
        const float result = results[9 * i + 2];
        EXPECT_FLOAT_EQ(expected, result) << "zPown(1," << z << ") is wrong.";
      }
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(-1.0f, z);
        const float result = results[9 * i + 3];
        EXPECT_FLOAT_EQ(expected, result) << "zPown(-1," << z << ") is wrong.";
      }
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(2.0f, z);
        const float result = results[9 * i + 4];
        EXPECT_FLOAT_EQ(expected, result) << "zPown(2," << z << ") is wrong.";
      }
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(0.5f, z);
        const float result = results[9 * i + 5];
        EXPECT_FLOAT_EQ(expected, result) << "zPown(0.5," << z << ") is wrong.";
      }
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
        const float result = results[9 * i + 6];
        EXPECT_FLOAT_EQ(expected, result) << "zPown(inf," << z << ") is wrong.";
      }
      //! \todo zPow(-inf, exp) should be checked.
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 7];
//        EXPECT_FLOAT_EQ(expected, result) << "zPown(-inf," << z << ") is wrong.";
//      }
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[9 * i];
        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
        const float result = results[9 * i + 8];
        if (z == 0) {
          EXPECT_FLOAT_EQ(expected, result) << "zPown(NaN," << z << ") is wrong.";
        }
        else {
          EXPECT_TRUE(std::isnan(expected))
              << "The reference of pow(NaN," << z << ") is wrong.";
          EXPECT_TRUE(std::isnan(result))
              << "zPown(NaN," << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "zPown2(2," << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "zPown3(2," << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::pow(2.0f, z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "zPown4(2," << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testSqrt, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sqrt(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 1) {
          ASSERT_TRUE(std::isnan(result))
              << "sqrt(" << z << ") isn't recommended to use of.";
        }
        else {
          ASSERT_FLOAT_EQ(expected, result)
              << "sqrt(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "sqrt(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "sqrt(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "sqrt(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZsqrtTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZsqrt, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sqrt(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 1)
          EXPECT_TRUE(std::isnan(result)) << "zSqrt(" << z << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "zSqrt(" << z << ") is wrong.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zSqrt2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zSqrt3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zSqrt4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * resolution);
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testRsqrt, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * resolution);
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[2 * i];
        const float expected = 1.0f / std::sqrt(z);
        const float result = results[2 * i + 1];
        ASSERT_FLOAT_EQ(expected, result)
            << "rsqrt(" << z << ") isn't recommended to use of.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
            << "rsqrt(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
            << "rsqrt(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
            << "rsqrt(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RsqrtSubnormalTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 10000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * resolution);
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testRsqrtSubnormal, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    //! \todo Remove the failure code. Subnormal doesn't work on Vulkan
    FAIL();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * resolution);
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[2 * i];
        const float expected = 1.0f / std::sqrt(z);
        const float result = results[2 * i + 1];
        EXPECT_FLOAT_EQ(expected, result) << "rsqrt(" << z << ") is wrong.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          EXPECT_FLOAT_EQ(expected, result[j]) << "rsqrt(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          EXPECT_FLOAT_EQ(expected, result[j]) << "rsqrt(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          EXPECT_FLOAT_EQ(expected, result[j]) << "rsqrt(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZrsqrtTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * resolution);
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZrsqrt, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * resolution);
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[2 * i];
        const float expected = 1.0f / std::sqrt(z);
        const float result = results[2 * i + 1];
        EXPECT_FLOAT_EQ(expected, result) << "zRsqrt(" << z << ") is wrong.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zRsqrt2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zRsqrt3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zRsqrt4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZrsqrtSubnormalTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 10000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * resolution);
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZrsqrtSubnormal, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    //! \todo Remove the failure code. Subnormal doesn't work on Vulkan
    FAIL();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * resolution);
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const float z = results[2 * i];
        const float expected = 1.0f / std::sqrt(z);
        const float result = results[2 * i + 1];
        EXPECT_FLOAT_EQ(expected, result) << "zRsqrt(" << z << ") is wrong.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          EXPECT_FLOAT_EQ(expected, result[j]) << "zRsqrt2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          EXPECT_FLOAT_EQ(expected, result[j]) << "zRsqrt3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = 1.0f / std::sqrt(z[j]);
          EXPECT_FLOAT_EQ(expected, result[j]) << "zRsqrt4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZcbrtTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZcbrt, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::cbrt(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 1)
          EXPECT_TRUE(std::isnan(result)) << "zCbrt(" << z << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "zCbrt(" << z << ") is wrong.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cbrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zCbrt2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cbrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zCbrt3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cbrt(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zCbrt4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 3));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testExp, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::exp(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 2) {
          ASSERT_TRUE(std::isnan(result))
              << "exp(" << z << ") isn't recommended to use of.";
        }
        else {
          ASSERT_FLOAT_EQ(expected, result)
              << "exp(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "exp(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "exp(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "exp(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZexpTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 3));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZexp, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::exp(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 2) {
          EXPECT_TRUE(std::isnan(result)) << "zExp(" << z << ") is wrong.";
        }
        else {
          EXPECT_FLOAT_EQ(expected, result) << "zExp(" << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zExp2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zExp3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::exp(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zExp4(" << z[j] << ") is wrong";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testLog, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 1) {
          ASSERT_TRUE(std::isnan(result))
              << "log(" << z << ") isn't recommended to use of.";
        }
        else {
          ASSERT_FLOAT_EQ(expected, result)
              << "log(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "log(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "log(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "log(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZlogTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZlog, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 1)
          EXPECT_TRUE(std::isnan(result)) << "zLog(" << z << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "zLog(" << z << ") is wrong.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zLogF2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zLogF3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zLogF4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testLog2, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log2(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 1) {
          ASSERT_TRUE(std::isnan(result))
              << "log2(" << z << ") isn't recommended to use of.";
        }
        else {
          ASSERT_FLOAT_EQ(expected, result)
              << "log2(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "log2(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "log2(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "log2(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, Zlog2Test)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZlog2, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::log2(z);
        const float result = results[2 * i + 1];
        if (i == resolution + 1)
          EXPECT_TRUE(std::isnan(result)) << "zLog2(" << z << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "zLog2(" << z << ") is wrong.";
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zLog2F2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zLog2F3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::log2(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zLog2F4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

namespace {

std::string getArgString(const float x) noexcept
{
  if (std::isfinite(x)) {
    constexpr float pi = zisc::constant::pi<float>();
    const float s = (x < 0.0f) ? -1.0f : 1.0f;
    const float z = std::abs(x);
    const std::size_t q = zisc::cast<std::size_t>(z / pi);
    const float t = s * (z - zisc::cast<float>(q) * pi);
    if (x < 0.0f)
      return std::to_string(t) + " - " + std::to_string(q) + "*pi";
    else
      return std::to_string(t) + " + " + std::to_string(q) + "*pi";
  }
  else {
    return std::to_string(x);
  }
}

} // namespace 

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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testSin, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sin(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          ASSERT_FLOAT_EQ(expected, result)
              << "sin(" << ::getArgString(z) << ") isn't recommended to use of.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "sin(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "sin(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "sin(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "sin(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZsinTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZsin, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::sin(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          EXPECT_FLOAT_EQ(expected, result) << "zSin(" << ::getArgString(z) << ") is wrong.";
        }
        else {
          EXPECT_TRUE(std::isnan(result)) << "zSin(" << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zSin2(" << ::getArgString(z[j]) << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zSin3(" << ::getArgString(z[j]) << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::sin(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zSin4(" << ::getArgString(z[j]) << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testCos, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::cos(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          ASSERT_FLOAT_EQ(expected, result)
              << "cos(" << ::getArgString(z) << ") isn't recommended to use of.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "cos(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "cos(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "cos(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "cos(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZcosTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZcos, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::cos(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          EXPECT_FLOAT_EQ(expected, result) << "zCos(" << ::getArgString(z) << ") is wrong.";
        }
        else {
          EXPECT_TRUE(std::isnan(result)) << "zCos(" << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zCos2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zCos3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::cos(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zCos4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZsincosTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(3 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(3 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(3 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(3 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZsincos, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(3 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[3 * i];
        {
          const float expected = std::sin(z);
          const float result = results[3 * i + 1];
          if (i < resolution) {
            ASSERT_FLOAT_EQ(expected, result)
                << "zSincos(" << ::getArgString(z) << ") isn't recommended to use of.";
          }
          else {
            ASSERT_TRUE(std::isnan(result))
                << "zSincos(" << z << ") isn't recommended to use of.";
          }
        }
        {
          const float expected = std::cos(z);
          const float result = results[3 * i + 2];
          if (i < resolution) {
            ASSERT_FLOAT_EQ(expected, result)
                << "zSincos(" << ::getArgString(z) << ") isn't recommended to use of.";
          }
          else {
            ASSERT_TRUE(std::isnan(result))
                << "zSincos(" << z << ") isn't recommended to use of.";
          }
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(3 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            ASSERT_FLOAT_EQ(expected, result[j])
                << "zSincos(" << z[j] << ") isn't recommended to use of.";
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            ASSERT_FLOAT_EQ(expected, result[j])
                << "zSincos(" << z[j] << ") isn't recommended to use of.";
          }
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(3 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            ASSERT_FLOAT_EQ(expected, result[j])
                << "zSincos(" << z[j] << ") isn't recommended to use of.";
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            ASSERT_FLOAT_EQ(expected, result[j])
                << "zSincos(" << z[j] << ") isn't recommended to use of.";
          }
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(3 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[3 * i];
        {
          const auto& result = results[3 * i + 1];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::sin(z[j]);
            ASSERT_FLOAT_EQ(expected, result[j])
                << "zSincos(" << z[j] << ") isn't recommended to use of.";
          }
        }
        {
          const auto& result = results[3 * i + 2];
          for (std::size_t j = 0; j < result.size(); ++j) {
            float expected = std::cos(z[j]);
            ASSERT_FLOAT_EQ(expected, result[j])
                << "zSincos(" << z[j] << ") isn't recommended to use of.";
          }
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testTan, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::tan(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          const float error = zisc::constant::getUlps(expected + result);
          ASSERT_FLOAT_NEAR(expected, result, error)
              << "tan(" << ::getArgString(z) << ") isn't recommended to use of.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "tan(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "tan(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "tan(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "tan(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZtanTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZtan, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::tan(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          const float error = zisc::constant::getUlps(expected + result);
          EXPECT_FLOAT_NEAR(expected, result, error) << "zTan(" << ::getArgString(z) << ") is wrong.";
        }
        else {
          EXPECT_TRUE(std::isnan(result)) << "zTan(" << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zTan2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zTan3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::tan(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zTan4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAsin, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::asin(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          const float error = zisc::constant::getUlps<3>(expected + result);
          ASSERT_FLOAT_NEAR(expected, result, error)
              << "asin(" << z << ") isn't recommended to use of.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "asin(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          const float error = zisc::constant::getUlps<3>(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "asin(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          const float error = zisc::constant::getUlps<3>(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "asin(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          const float error = zisc::constant::getUlps<3>(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "asin(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZasinTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZasin, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::asin(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          const float error = zisc::constant::getUlps<3>(expected + result);
          EXPECT_FLOAT_NEAR(expected, result, error) << "zAsin(" << z << ") is wrong.";
        }
        else {
          EXPECT_TRUE(std::isnan(result)) << "zAsin(" << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          const float error = zisc::constant::getUlps<3>(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zAsin2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          const float error = zisc::constant::getUlps<3>(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zAsin3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::asin(z[j]);
          const float error = zisc::constant::getUlps<3>(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zAsin4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAcos, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::acos(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          const float error = zisc::constant::getUlps(expected + result);
          ASSERT_FLOAT_NEAR(expected, result, error)
              << "acos(" << z << ") isn't recommended to use of.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "acos(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "acos(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "acos(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error)
              << "acos(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZacosTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 2));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZacos, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 2));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 2; ++i) {
        const float z = results[2 * i];
        const float expected = std::acos(z);
        const float result = results[2 * i + 1];
        if (i < resolution) {
          const float error = zisc::constant::getUlps(expected + result);
          EXPECT_FLOAT_NEAR(expected, result, error) << "zAcos(" << z << ") is wrong.";
        }
        else {
          EXPECT_TRUE(std::isnan(result)) << "zAcos(" << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zAcos2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zAcos3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::acos(z[j]);
          const float error = zisc::constant::getUlps(expected + result[j]);
          ASSERT_FLOAT_NEAR(expected, result[j], error) << "zAcos4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
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

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 3));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAtan, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::atan(z);
        const float result = results[2 * i + 1];
        if (i < resolution + 2) {
          ASSERT_FLOAT_EQ(expected, result)
              << "atan(" << z << ") isn't recommended to use of.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "atan(" << z << ") isn't recommended to use of.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "atan(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "atan(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j])
              << "atan(" << z[j] << ") isn't recommended to use of.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ZatanTest)
{
  using zisc::cast;
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1000000u;

    auto result1_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    result1_buffer->setSize(2 * (resolution + 3));
    auto result2_buffer = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    result2_buffer->setSize(2 * resolution);
    auto result3_buffer = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    result3_buffer->setSize(2 * resolution);
    auto result4_buffer = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    result4_buffer->setSize(2 * resolution);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testZatan, 1);
    kernel->run(*result1_buffer, *result2_buffer, *result3_buffer, *result4_buffer,
                *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    // Scalar
    {
      std::vector<float> results;
      results.resize(2 * (resolution + 3));
      result1_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution + 3; ++i) {
        const float z = results[2 * i];
        const float expected = std::atan(z);
        const float result = results[2 * i + 1];
        if (i < resolution + 2) {
          EXPECT_FLOAT_EQ(expected, result) << "zAtan(" << z << ") is wrong.";
        }
        else {
          EXPECT_TRUE(std::isnan(result)) << "zAtan(" << z << ") is wrong.";
        }
      }
    }

    // Vector2
    {
      std::vector<cl::float2> results;
      results.resize(2 * resolution);
      result2_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zAtan2(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector3
    {
      std::vector<cl::float3> results;
      results.resize(2 * resolution);
      result3_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zAtan3(" << z[j] << ") is wrong.";
        }
      }
    }

    // Vector4
    {
      std::vector<cl::float4> results;
      results.resize(2 * resolution);
      result4_buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const auto& z = results[2 * i];
        const auto& result = results[2 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          float expected = std::atan(z[j]);
          ASSERT_FLOAT_EQ(expected, result[j]) << "zAtan4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
