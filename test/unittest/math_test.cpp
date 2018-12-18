/*!
  \file math_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/math.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/math.hpp"
// Test
#include "test.hpp"

TEST(MathTest, ConstantValueTest)
{
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    auto pi_buffer = zinvul::makeBuffer<float>(device.get(),
                                               zinvul::BufferUsage::kDeviceSrc);
    pi_buffer->setSize(1);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testConstantValues, 1);
    kernel->run(*pi_buffer, {1}, 0);
    device->waitForCompletion();

    {
      float pi = 0.0f;
      pi_buffer->read(&pi, 1, 0, 0);
      EXPECT_FLOAT_EQ(zisc::kPi<float>, pi)
          << "The pi values are different on between host and device.";
    }
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

    auto abs_result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    abs_result1->setSize(3);
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
    clamp_result1->setSize(3);
    auto clamp_result2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result2->setSize(5);
    auto clamp_result3 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result3->setSize(2);
    auto clamp_result4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result4->setSize(4);
    auto clamp_result5 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result5->setSize(1);
    auto clamp_result6 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result6->setSize(3);
    auto clamp_result7 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result7->setSize(1);
    auto clamp_result8 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result8->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testCommon, 1);
    kernel->run(*abs_result1, *abs_result2, *abs_result3, *abs_result4,
        *abs_result5, *abs_result6, *abs_result7, *abs_result8,
        *clamp_result1, *clamp_result2, *clamp_result3, *clamp_result4,
        *clamp_result5, *clamp_result6, *clamp_result7, *clamp_result8, {1}, 0);
    device->waitForCompletion();

    {
      std::array<uint32b, 3> result;
      abs_result1->read(result.data(), result.size(), 0, 0);
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

    constexpr float pi = zisc::kPi<float>;
    {
      std::array<int32b, 3> result;
      clamp_result1->read(result.data(), result.size(), 0 ,0);
      EXPECT_FALSE(result[0]) << "The clamp func is wrong.";
      EXPECT_TRUE(result[1]) << "The clamp func is wrong.";
      EXPECT_EQ(-1, result[2]) << "The clamp func is wrong.";
    }
    {
      std::array<float, 5> result;
      clamp_result2->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0]) << "The clamp func is wrong.";
      EXPECT_EQ(1.0f, result[1]) << "The clamp func is wrong.";
      EXPECT_EQ(-1.0f, result[2]) << "The clamp func is wrong.";
      // radian
      EXPECT_FLOAT_EQ(180.0f, result[3]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[4]) << "The radians func is wrong.";
    }
    {
      std::array<cl::int2, 2> result;
      clamp_result3->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[0][1]) << "The clamp func is wrong.";
      EXPECT_FALSE(result[1][0]) << "The clamp func is wrong.";
      EXPECT_EQ(-2, result[1][1]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float2, 4> result;
      clamp_result4->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(0.0f, result[1][0]) << "The clamp func is wrong.";
      EXPECT_EQ(-2.0f, result[1][1]) << "The clamp func is wrong.";
      // radian
      EXPECT_FLOAT_EQ(180.0f, result[2][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[2][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[3][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[3][1]) << "The radians func is wrong.";
    }
    {
      std::array<cl::int3, 2> result;
      clamp_result5->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3, result[0][2]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float3, 3> result;
      clamp_result6->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3.0f, result[0][2]) << "The clamp func is wrong.";
      // radian
      EXPECT_FLOAT_EQ(180.0f, result[1][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[1][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(45.0f, result[1][2]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[2][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[2][1]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.25f * pi, result[2][2]) << "The radians func is wrong.";
    }
    {
      std::array<cl::int4, 2> result;
      clamp_result7->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3, result[0][2]) << "The clamp func is wrong.";
      EXPECT_EQ(4, result[0][3]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float4, 3> result;
      clamp_result8->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3.0f, result[0][2]) << "The clamp func is wrong.";
      EXPECT_EQ(4.0f, result[0][3]) << "The clamp func is wrong.";
      // radian
      EXPECT_FLOAT_EQ(180.0f, result[1][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[1][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(45.0f, result[1][2]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(0.0f, result[1][3]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[2][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[2][1]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.25f * pi, result[2][2]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.0f * pi, result[2][3]) << "The radians func is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CommonTest2)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto abs_result1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    abs_result1->setSize(2);
    auto abs_result2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    abs_result2->setSize(2);
    auto abs_result3 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    abs_result3->setSize(1);
    auto abs_result4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    abs_result4->setSize(1);
    auto abs_result5 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    abs_result5->setSize(1);
    auto abs_result6 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    abs_result6->setSize(1);
    auto abs_result7 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    abs_result7->setSize(1);
    auto abs_result8 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    abs_result8->setSize(1);
    auto clamp_result1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result1->setSize(3);
    auto clamp_result2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result2->setSize(5);
    auto clamp_result3 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result3->setSize(2);
    auto clamp_result4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result4->setSize(4);
    auto clamp_result5 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result5->setSize(1);
    auto clamp_result6 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result6->setSize(3);
    auto clamp_result7 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result7->setSize(1);
    auto clamp_result8 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    clamp_result8->setSize(3);

    auto kernel = makeZinvulKernel(device.get(), math, testCommon2, 1);
    kernel->run(*abs_result1, *abs_result2, *abs_result3, *abs_result4,
        *abs_result5, *abs_result6, *abs_result7, *abs_result8,
        *clamp_result1, *clamp_result2, *clamp_result3, *clamp_result4,
        *clamp_result5, *clamp_result6, *clamp_result7, *clamp_result8, {1}, 0);
    device->waitForCompletion();

    {
      std::array<uint32b, 2> result;
      abs_result1->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::uint2, 1> result;
      abs_result3->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 2; ++i)
        EXPECT_EQ(i+1, result[0][i]) << "The abs func is wrong.";
    }
    {
      std::array<cl::float2, 1> result;
      abs_result4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 2; ++i)
        EXPECT_EQ(zisc::cast<float>(i+1), result[0][i]) << "The fabs func is wrong.";
    }
    {
      std::array<cl::uint3, 1> result;
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
      std::array<cl::uint4, 1> result;
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

    constexpr float pi = zisc::kPi<float>;
    {
      std::array<int32b, 3> result;
      clamp_result1->read(result.data(), result.size(), 0 ,0);
      EXPECT_FALSE(result[0]) << "The clamp func is wrong.";
      EXPECT_TRUE(result[1]) << "The clamp func is wrong.";
      EXPECT_EQ(-1, result[2]) << "The clamp func is wrong.";
    }
    {
      std::array<float, 5> result;
      clamp_result2->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0]) << "The clamp func is wrong.";
      EXPECT_EQ(1.0f, result[1]) << "The clamp func is wrong.";
      EXPECT_EQ(-1.0f, result[2]) << "The clamp func is wrong.";
      // radian
      EXPECT_FLOAT_EQ(180.0f, result[3]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[4]) << "The radians func is wrong.";
    }
    {
      std::array<cl::int2, 2> result;
      clamp_result3->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[0][1]) << "The clamp func is wrong.";
      EXPECT_FALSE(result[1][0]) << "The clamp func is wrong.";
      EXPECT_EQ(-2, result[1][1]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float2, 4> result;
      clamp_result4->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(0.0f, result[1][0]) << "The clamp func is wrong.";
      EXPECT_EQ(-2.0f, result[1][1]) << "The clamp func is wrong.";
      // radian
      EXPECT_FLOAT_EQ(180.0f, result[2][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[2][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[3][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[3][1]) << "The radians func is wrong.";
    }
    {
      std::array<cl::int3, 2> result;
      clamp_result5->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3, result[0][2]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float3, 3> result;
      clamp_result6->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3.0f, result[0][2]) << "The clamp func is wrong.";
      // radian
      EXPECT_FLOAT_EQ(180.0f, result[1][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[1][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(45.0f, result[1][2]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[2][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[2][1]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.25f * pi, result[2][2]) << "The radians func is wrong.";
    }
    {
      std::array<cl::int4, 2> result;
      clamp_result7->read(result.data(), result.size(), 0, 0);
      EXPECT_FALSE(result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3, result[0][2]) << "The clamp func is wrong.";
      EXPECT_EQ(4, result[0][3]) << "The clamp func is wrong.";
    }
    {
      std::array<cl::float4, 3> result;
      clamp_result8->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(0.0f, result[0][0]) << "The clamp func is wrong.";
      EXPECT_EQ(2.0f, result[0][1]) << "The clamp func is wrong.";
      EXPECT_EQ(-3.0f, result[0][2]) << "The clamp func is wrong.";
      EXPECT_EQ(4.0f, result[0][3]) << "The clamp func is wrong.";
      // radian
      EXPECT_FLOAT_EQ(180.0f, result[1][0]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(90.0f, result[1][1]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(45.0f, result[1][2]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(0.0f, result[1][3]) << "The degrees func is wrong.";
      EXPECT_FLOAT_EQ(pi, result[2][0]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.5f * pi, result[2][1]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.25f * pi, result[2][2]) << "The radians func is wrong.";
      EXPECT_FLOAT_EQ(0.0f * pi, result[2][3]) << "The radians func is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
