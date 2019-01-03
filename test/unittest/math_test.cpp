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

TEST(MathTest, ConstantValueTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_int = 16;
    constexpr std::size_t n_float = 20;
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
      EXPECT_FLOAT_EQ(zisc::kE<float>, results[index++])
          << "The constant 'M_E_F' is wrong.";
      EXPECT_FLOAT_EQ(std::log2(zisc::kE<float>), results[index++])
          << "The constant 'M_LOG2E_F' is wrong.";
      EXPECT_FLOAT_EQ(std::log10(zisc::kE<float>), results[index++])
          << "The constant 'M_LOG10E_F' is wrong.";
      EXPECT_FLOAT_EQ(std::log(2.0f), results[index++])
          << "The constant 'M_LN2_F' is wrong.";
      EXPECT_FLOAT_EQ(std::log(10.0f), results[index++])
          << "The constant 'M_LN10_F' is wrong.";
      EXPECT_FLOAT_EQ(zisc::kPi<float>, results[index++])
          << "The constant 'M_PI_F' is wrong.";
      EXPECT_FLOAT_EQ(zisc::kPi<float> / 2.0f, results[index++])
          << "The constant 'M_PI_2_F' is wrong.";
      EXPECT_FLOAT_EQ(zisc::kPi<float> / 4.0f, results[index++])
          << "The constant 'M_PI_4_F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / zisc::kPi<float>, results[index++])
          << "The constant 'M_1_PI_F' is wrong.";
      EXPECT_FLOAT_EQ(2.0f / zisc::kPi<float>, results[index++])
          << "The constant 'M_2_PI_F' is wrong.";
      EXPECT_FLOAT_EQ(2.0f / std::sqrt(zisc::kPi<float>), results[index++])
          << "The constant 'M_2_SQRTPI_F' is wrong.";
      EXPECT_FLOAT_EQ(std::sqrt(2.0f), results[index++])
          << "The constant 'M_SQRT2_F' is wrong.";
      EXPECT_FLOAT_EQ(1.0f / std::sqrt(2.0f), results[index++])
          << "The constant 'M_SQRT1_2_F' is wrong.";
      EXPECT_FLOAT_EQ(zisc::kPi<float>, results[index++])
          << "The constant 'zPiF' is wrong.";
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
      EXPECT_EQ(1u, result[0]) << "The abs func is wrong.";
      EXPECT_EQ(1u, result[1]) << "The abs func is wrong.";
      EXPECT_EQ(1u, result[2]) << "The abs func is wrong.";
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
        EXPECT_EQ(i+1, result[1][i]) << "The abs func is wrong.";
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
        EXPECT_EQ(i+1, result[1][i]) << "The abs func is wrong.";
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
        EXPECT_EQ(i+1, result[1][i]) << "The abs func is wrong.";
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
      EXPECT_EQ(std::numeric_limits<int>::max(), result[index++]) << "The max func is wrong.";
      EXPECT_EQ(std::numeric_limits<int>::max(), result[index++]) << "The max func is wrong.";
      EXPECT_FALSE(result[index++]) << "The max func is wrong.";
      EXPECT_FALSE(result[index++]) << "The max func is wrong.";
      EXPECT_TRUE(result[index++]) << "The max func is wrong.";
      EXPECT_TRUE(result[index++]) << "The max func is wrong.";
      EXPECT_EQ(-1, result[index++]) << "The max func is wrong.";
      EXPECT_EQ(-1, result[index++]) << "The max func is wrong.";
      EXPECT_EQ(10, result[index++]) << "The max func is wrong.";
      EXPECT_EQ(10, result[index++]) << "The max func is wrong.";
    }
    {
      std::size_t index = 0;
      std::array<float, n> result;
      buffer2->read(result.data(), result.size(), 0, 0);
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), result[index++]) << "The max func is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), result[index++]) << "The max func is wrong.";
      index = 2;
      EXPECT_FLOAT_EQ(0.0f, result[index++]) << "The max func is wrong.";
      EXPECT_FLOAT_EQ(0.0f, result[index++]) << "The max func is wrong.";
      EXPECT_FLOAT_EQ(1.0f, result[index++]) << "The max func is wrong.";
      EXPECT_FLOAT_EQ(1.0f, result[index++]) << "The max func is wrong.";
      EXPECT_FLOAT_EQ(-1, result[index++]) << "The max func is wrong.";
      EXPECT_FLOAT_EQ(-1, result[index++]) << "The max func is wrong.";
      EXPECT_FLOAT_EQ(10, result[index++]) << "The max func is wrong.";
      EXPECT_FLOAT_EQ(10, result[index++]) << "The max func is wrong.";
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
      EXPECT_EQ(std::numeric_limits<int>::lowest(), result[index++]) << "The min func is wrong.";
      EXPECT_EQ(std::numeric_limits<int>::lowest(), result[index++]) << "The min func is wrong.";
      EXPECT_EQ(-1, result[index++]) << "The min func is wrong.";
      EXPECT_EQ(-1, result[index++]) << "The min func is wrong.";
      EXPECT_EQ(-1, result[index++]) << "The min func is wrong.";
      EXPECT_EQ(-1, result[index++]) << "The min func is wrong.";
      EXPECT_EQ(-10, result[index++]) << "The min func is wrong.";
      EXPECT_EQ(-10, result[index++]) << "The min func is wrong.";
      EXPECT_TRUE(result[index++]) << "The min func is wrong.";
      EXPECT_TRUE(result[index++]) << "The min func is wrong.";
    }
    {
      std::size_t index = 0;
      std::array<float, n> result;
      buffer2->read(result.data(), result.size(), 0, 0);
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::lowest(), result[index++]) << "The min func is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::lowest(), result[index++]) << "The min func is wrong.";
      index = 2;
      EXPECT_FLOAT_EQ(-1.0f, result[index++]) << "The min func is wrong.";
      EXPECT_FLOAT_EQ(-1.0f, result[index++]) << "The min func is wrong.";
      EXPECT_FLOAT_EQ(-1.0f, result[index++]) << "The min func is wrong.";
      EXPECT_FLOAT_EQ(-1.0f, result[index++]) << "The min func is wrong.";
      EXPECT_FLOAT_EQ(-10.0f, result[index++]) << "The min func is wrong.";
      EXPECT_FLOAT_EQ(-10.0f, result[index++]) << "The min func is wrong.";
      EXPECT_FLOAT_EQ(1.0f, result[index++]) << "The min func is wrong.";
      EXPECT_FLOAT_EQ(1.0f, result[index++]) << "The min func is wrong.";
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

    constexpr uint32b resolution = 10000u;

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
        if( i == result_exp.size() - 1)
          EXPECT_TRUE(std::isnan(result)) << "frexp(" << n << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "frexp(" << n << ") is wrong.";
        EXPECT_EQ(e, result_exp[i]) << "frexp(" << n << ") is wrong.";
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        if( i == result_exp.size() - 1)
          EXPECT_TRUE(std::isnan(result)) << "ldexp(" << m << "," << e << ") is wrong.";
        else
          ASSERT_FLOAT_EQ(expected, result) << "ldexp(" << m << "," << e << ") is wrong.";
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
        float expected = std::frexp(n[0], &e);
        cl::float2 result = results[3 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          EXPECT_FLOAT_EQ(expected, result[j]) << "frexp(" << n[0] << ") is wrong.";
          EXPECT_EQ(e, result_exp[i][j]) << "frexp(" << n[0] << ") is wrong.";
        }
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        for (std::size_t j = 0; j < result.size(); ++j) {
          ASSERT_FLOAT_EQ(expected, result[j]) << "ldexp(" << m << "," << e << ") is wrong.";
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
        float expected = std::frexp(n[0], &e);
        cl::float3 result = results[3 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          EXPECT_FLOAT_EQ(expected, result[j]) << "zrexp(" << n[0] << ") is wrong.";
          EXPECT_EQ(e, result_exp[i][j]) << "frexp(" << n[0] << ") is wrong.";
        }
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        for (std::size_t j = 0; j < result.size(); ++j) {
          ASSERT_FLOAT_EQ(expected, result[j]) << "ldexp(" << m << "," << e << ") is wrong.";
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
        float expected = std::frexp(n[0], &e);
        cl::float4 result = results[3 * i + 1];
        for (std::size_t j = 0; j < result.size(); ++j) {
          EXPECT_FLOAT_EQ(expected, result[j]) << "zrexp(" << n[0] << ") is wrong.";
          EXPECT_EQ(e, result_exp[i][j]) << "frexp(" << n[0] << ") is wrong.";
        }
        const float m = expected;
        expected = std::ldexp(m, e);
        result = results[3 * i + 2];
        for (std::size_t j = 0; j < result.size(); ++j) {
          ASSERT_FLOAT_EQ(expected, result[j]) << "ldexp(" << m << "," << e << ") is wrong.";
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

    constexpr uint32b resolution = 10000u;

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
          ASSERT_FLOAT_EQ(expected, result) << "zLdexp(" << m << "," << e << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result1[j]) << "zFrexp2(" << n[j] << ") is wrong.";
          EXPECT_EQ(e, result_exp[i][j]) << "zFrexp2(" << n[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result1[j]) << "zFrexp3(" << n[j] << ") is wrong.";
          EXPECT_EQ(e, result_exp[i][j]) << "zFrexp3(" << n[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result1[j]) << "zFrexp4(" << n[j] << ") is wrong.";
          EXPECT_EQ(e, result_exp[i][j]) << "zFrexp4(" << n[j] << ") is wrong.";
          const float m = expected;
          expected = std::ldexp(m, e);
          ASSERT_FLOAT_EQ(expected, result2[j]) << "zLdexp4(" << m << "," << e << ") is wrong.";
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

    constexpr uint32b resolution = 10000u;

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
        if (i == resolution + 1)
          EXPECT_TRUE(std::isnan(result)) << "sqrt(" << z << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "sqrt(" << z << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "sqrt(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "sqrt(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "sqrt(" << z[j] << ") is wrong.";
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

    constexpr uint32b resolution = 10000u;

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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zSqrt2(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zSqrt3(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zSqrt4(" << z[j] << ") is wrong.";
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

//! \todo Remove comment out. Subnormal doesn't work on Vulkan

//    // Scalar
//    {
//      std::vector<float> results;
//      results.resize(2 * resolution);
//      result1_buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const float z = results[2 * i];
//        const float expected = 1.0f / std::sqrt(z);
//        const float result = results[2 * i + 1];
//        EXPECT_FLOAT_EQ(expected, result) << "rsqrt(" << z << ") is wrong.";
//      }
//    }
//
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      result2_buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (std::size_t j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          EXPECT_FLOAT_EQ(expected, result[j]) << "rsqrt(" << z[j] << ") is wrong.";
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      result3_buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (std::size_t j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          EXPECT_FLOAT_EQ(expected, result[j]) << "rsqrt(" << z[j] << ") is wrong.";
//        }
//      }
//    }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      result4_buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (std::size_t j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          EXPECT_FLOAT_EQ(expected, result[j]) << "rsqrt(" << z[j] << ") is wrong.";
//        }
//      }
//    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
  FAIL();
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

//! \todo Remove comment out. Subnormal doesn't work on Vulkan

//    // Scalar
//    {
//      std::vector<float> results;
//      results.resize(2 * resolution);
//      result1_buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const float z = results[2 * i];
//        const float expected = 1.0f / std::sqrt(z);
//        const float result = results[2 * i + 1];
//        EXPECT_FLOAT_EQ(expected, result) << "zRsqrt(" << z << ") is wrong.";
//      }
//    }
//
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      result2_buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (std::size_t j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          EXPECT_FLOAT_EQ(expected, result[j]) << "zRsqrt2(" << z[j] << ") is wrong.";
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      result3_buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (std::size_t j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          EXPECT_FLOAT_EQ(expected, result[j]) << "zRsqrt3(" << z[j] << ") is wrong.";
//        }
//      }
//    }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      result4_buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (std::size_t j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          EXPECT_FLOAT_EQ(expected, result[j]) << "zRsqrt4(" << z[j] << ") is wrong.";
//        }
//      }
//    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
  FAIL();
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

    constexpr uint32b resolution = 10000u;

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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zCbrt2(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zCbrt3(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zCbrt4(" << z[j] << ") is wrong.";
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

    constexpr uint32b resolution = 10000u;

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
        if (i == resolution + 1)
          EXPECT_TRUE(std::isnan(result)) << "log(" << z << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "log(" << z << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "log(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "log(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "log(" << z[j] << ") is wrong.";
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

    constexpr uint32b resolution = 10000u;

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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zLogF2(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zLogF3(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zLogF4(" << z[j] << ") is wrong.";
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

    constexpr uint32b resolution = 10000u;

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
        if (i == resolution + 1)
          EXPECT_TRUE(std::isnan(result)) << "log2(" << z << ") is wrong.";
        else
          EXPECT_FLOAT_EQ(expected, result) << "log2(" << z << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "log2(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "log2(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "log2(" << z[j] << ") is wrong.";
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

    constexpr uint32b resolution = 10000u;

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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zLog2F2(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zLog2F3(" << z[j] << ") is wrong.";
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
          EXPECT_FLOAT_EQ(expected, result[j]) << "zLog2F4(" << z[j] << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
