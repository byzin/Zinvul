/*!
  \file vector_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/vector.hpp"
#include "zinvul/zinvul_config.hpp"
// Test
#include "test.hpp"

TEST(VectorTest, VectorOperationsTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<cl::short2>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(1);
    auto buffer2 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(1);
    auto buffer3 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(1);
    auto buffer4 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    buffer4->setSize(5);
    auto buffer5 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    buffer5->setSize(8);
    auto buffer6 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    buffer6->setSize(5);
    auto buffer7 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    buffer7->setSize(5);
    auto buffer8 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    buffer8->setSize(5);
    auto buffer9 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    buffer9->setSize(5);
    auto buffer10 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    buffer10->setSize(5);
    auto buffer11 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    buffer11->setSize(5);
    auto buffer12 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceOnly);
    buffer12->setSize(3);
    auto buffer13 = makeStorageBuffer<cl::char3>(device.get(), BufferUsage::kDeviceOnly);
    buffer13->setSize(3);
    auto buffer14 = makeStorageBuffer<cl::short3>(device.get(), BufferUsage::kDeviceOnly);
    buffer14->setSize(11);
    auto buffer15 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceOnly);
    buffer15->setSize(16);
    auto buffer16 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    buffer16->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector, testVectorOperations));
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, *buffer5, *buffer6, *buffer7, *buffer8, *buffer9, *buffer10, *buffer11, *buffer12, *buffer13, *buffer14, *buffer15, *buffer16, {1}, 0);
    device->waitForCompletion();

    {
      const cl::short2 expected{20, 23};
      cl::short2 result;
      buffer1->read(&result, 1, 0, 0);
      for (uint32b i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector addition operations are wrong.";
      }
    }
    {
      const cl::int3 expected{3, 2, 1};
      cl::int3 result;
      buffer2->read(&result, 1, 0, 0);
      for (uint32b i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector subtraction operations are wrong.";
      }
    }
    {
      const cl::uint4 expected{540u, 1440u, 2772u, 4608u};
      cl::uint4 result;
      buffer3->read(&result, 1, 0, 0);
      for (uint32b i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector multiplication operations are wrong.";
      }
    }
    {
      std::array<cl::float2, 5> result;
      buffer4->read(result.data(), result.size(), 0, 0);
      {
        const cl::float2 expected{5.0f, 3.0f};
        for (uint32b i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[0][i])
              << "The vector division operations are wrong.";
        }
      }
      {
        const cl::float2 expected{2.5f, 3.0f};
        for (uint32b i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[1][i])
              << "The vector division operations are wrong.";
        }
      }
      {
        const cl::float2 expected{2.0f / 5.0f, 2.0f / 6.0f};
        for (uint32b i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[2][i])
              << "The vector division operations are wrong.";
        }
      }
      {
        const cl::float2 expected{0.5f, 0.5f};
        for (uint32b i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[3][i])
              << "The vector division operations are wrong.";
        }
      }
      {
        const cl::float2 expected{1.0f / 3.0f, 1.0f / 3.0f};
        for (uint32b i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[4][i])
              << "The vector division operations are wrong.";
        }
      }
    }
    {
      std::array<cl::int2, 8> result;
      buffer5->read(result.data(), result.size(), 0, 0);
      {
        const cl::int2 expected{1, 2};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[0][i])
              << "The vector pre-increment is wrong.";
          ASSERT_EQ(expected[i], result[1][i])
              << "The vector pre-increment is wrong.";
          ASSERT_EQ(expected[i], result[2][i])
              << "The vector post-increment is wrong.";
        }
      }
      {
        const cl::int2 expected{2, 3};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[3][i])
              << "The vector pre-increment is wrong.";
        }
      }
      {
        const cl::int2 expected{-1, 0};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[4][i])
              << "The vector pre-decrement is wrong.";
          ASSERT_EQ(expected[i], result[5][i])
              << "The vector pre-decrement is wrong.";
          ASSERT_EQ(expected[i], result[6][i])
              << "The vector post-decrement is wrong.";
        }
      }
      {
        const cl::int2 expected{-2, -1};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[7][i])
              << "The vector pre-decrement is wrong.";
        }
      }
    }
    {
      std::array<cl::int2, 5> result;
      buffer6->read(result.data(), result.size(), 0, 0);
      {
        const cl::int2 expected{1, 1};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[0][i]) <<
              "The vector reminder operations are wrong.";
          ASSERT_EQ(expected[i], result[1][i]) <<
              "The vector reminder operations are wrong.";
          ASSERT_EQ(expected[i], result[3][i]) <<
              "The vector reminder operations are wrong.";
          ASSERT_EQ(expected[i], result[4][i]) <<
              "The vector reminder operations are wrong.";
        }
      }
      {
        const cl::int2 expected{3, 3};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[2][i]) <<
              "The vector reminder operations are wrong.";
        }
      }
    }
    {
      std::array<cl::int2, 5> result;
      buffer7->read(result.data(), result.size(), 0, 0);
      const cl::int2 expected{3, 3};
      for (uint32b i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[0][i]) <<
            "The vector bitwize AND operations are wrong.";
        ASSERT_EQ(expected[i], result[1][i]) <<
            "The vector bitwize AND operations are wrong.";
        ASSERT_EQ(expected[i], result[2][i]) <<
            "The vector bitwize AND operations are wrong.";
        ASSERT_EQ(expected[i], result[3][i]) <<
            "The vector bitwize AND operations are wrong.";
        ASSERT_EQ(expected[i], result[4][i]) <<
            "The vector bitwize AND operations are wrong.";
      }
    }
    {
      std::array<cl::int2, 5> result;
      buffer8->read(result.data(), result.size(), 0, 0);
      const cl::int2 expected{7, 7};
      for (uint32b i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[0][i]) <<
            "The vector bitwize OR operations are wrong.";
        ASSERT_EQ(expected[i], result[1][i]) <<
            "The vector bitwize OR operations are wrong.";
        ASSERT_EQ(expected[i], result[2][i]) <<
            "The vector bitwize OR operations are wrong.";
        ASSERT_EQ(expected[i], result[3][i]) <<
            "The vector bitwize OR operations are wrong.";
        ASSERT_EQ(expected[i], result[4][i]) <<
            "The vector bitwize OR operations are wrong.";
      }
    }
    {
      std::array<cl::int2, 5> result;
      buffer9->read(result.data(), result.size(), 0, 0);
      const cl::int2 expected{~0 - 3, ~0 - 3};
      for (uint32b i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[0][i]) <<
            "The vector bitwize XOR operations are wrong.";
        ASSERT_EQ(expected[i], result[1][i]) <<
            "The vector bitwize XOR operations are wrong.";
        ASSERT_EQ(expected[i], result[2][i]) <<
            "The vector bitwize XOR operations are wrong.";
        ASSERT_EQ(expected[i], result[3][i]) <<
            "The vector bitwize XOR operations are wrong.";
        ASSERT_EQ(expected[i], result[4][i]) <<
            "The vector bitwize XOR operations are wrong.";
      }
    }
    {
      std::array<cl::int2, 5> result;
      buffer10->read(result.data(), result.size(), 0, 0);
      const cl::int2 expected{4, 4};
      for (uint32b i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[0][i]) <<
            "The vector left shift operations are wrong.";
        ASSERT_EQ(expected[i], result[1][i]) <<
            "The vector left shift operations are wrong.";
        ASSERT_EQ(expected[i], result[2][i]) <<
            "The vector left shift operations are wrong.";
        ASSERT_EQ(expected[i], result[3][i]) <<
            "The vector left shift operations are wrong.";
      }
    }
    {
      std::array<cl::int2, 5> result;
      buffer11->read(result.data(), result.size(), 0, 0);
      {
        const cl::int2 expected{4, 4};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[0][i]) <<
              "The vector right shift operations are wrong.";
          ASSERT_EQ(expected[i], result[1][i]) <<
              "The vector right shift operations are wrong.";
          ASSERT_EQ(expected[i], result[2][i]) <<
              "The vector right shift operations are wrong.";
          ASSERT_EQ(expected[i], result[3][i]) <<
              "The vector right shift operations are wrong.";
        }
      }
    }
    {
      std::array<cl::int3, 3> result;
      buffer12->read(result.data(), result.size(), 0, 0);
      {
        const cl::int3 expected{Config::vecResultFalse<int32b>(),
                                Config::vecResultFalse<int32b>(),
                                Config::vecResultTrue<int32b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[0][i]) <<
              "The vector right boolean AND are wrong.";
          ASSERT_EQ(expected[i], result[1][i]) <<
              "The vector right boolean AND are wrong.";
          ASSERT_EQ(expected[i], result[2][i]) <<
              "The vector right boolean AND are wrong.";
        }
      }
    }
    {
      std::array<cl::char3, 3> result;
      buffer13->read(result.data(), result.size(), 0, 0);
      {
        const cl::char3 expected{Config::vecResultTrue<int8b>(),
                                 Config::vecResultFalse<int8b>(),
                                 Config::vecResultTrue<int8b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[0][i]) <<
              "The vector right boolean OR are wrong.";
        }
      }
      {
        const cl::char3 expected{Config::vecResultTrue<int8b>(),
                                 Config::vecResultTrue<int8b>(),
                                 Config::vecResultTrue<int8b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[1][i]) <<
              "The vector right boolean OR are wrong.";
          ASSERT_EQ(expected[i], result[2][i]) <<
              "The vector right boolean OR are wrong.";
        }
      }
    }
    {
      std::array<cl::short3, 11> result;
      buffer14->read(result.data(), result.size(), 0, 0);
      {
        const cl::short3 expected{Config::vecResultTrue<int16b>(),
                                  Config::vecResultFalse<int16b>(),
                                  Config::vecResultTrue<int16b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_TRUE(result[0][i])
              << "The vector equal operation is wrong.";
          ASSERT_FALSE(result[1][i])
              << "The vector equal operation is wrong.";
          ASSERT_EQ(expected[i], result[2][i])
              << "The vector equal operation is wrong.";
          ASSERT_NE(expected[i], result[3][i])
              << "The vector equal operation is wrong.";
        }
      }
      {
        const cl::short3 expected{Config::vecResultTrue<int16b>(),
                                  Config::vecResultTrue<int16b>(),
                                  Config::vecResultTrue<int16b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[4][i])
              << "The vector equal operation is wrong.";
          ASSERT_NE(expected[i], result[5][i])
              << "The vector equal operation is wrong.";
          ASSERT_EQ(expected[i], result[6][i])
              << "The vector equal operation is wrong.";
          ASSERT_NE(expected[i], result[7][i])
              << "The vector equal operation is wrong.";
        }
      }
      {
        const cl::short3 expected{Config::vecResultFalse<int16b>(),
                                  Config::vecResultTrue<int16b>(),
                                  Config::vecResultFalse<int16b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[8][i])
              << "The vector equal operation is wrong.";
          ASSERT_NE(expected[i], result[9][i])
              << "The vector equal operation is wrong.";
          ASSERT_EQ(expected[i], result[10][i])
              << "The vector equal operation is wrong.";
        }
      }
    }
    {
      std::array<cl::int3, 16> result;
      buffer15->read(result.data(), result.size(), 0, 0);
      {
        for (uint32b i = 0; i < 3; ++i) {
          ASSERT_TRUE(result[0][i])
              << "The vector equal operation is wrong.";
          ASSERT_FALSE(result[1][i])
              << "The vector equal operation is wrong.";
          ASSERT_TRUE(result[2][i])
              << "The vector equal operation is wrong.";
          ASSERT_FALSE(result[3][i])
              << "The vector equal operation is wrong.";
        }
      }
      {
        const cl::int3 expected{Config::vecResultTrue<int32b>(),
                                Config::vecResultTrue<int32b>(),
                                Config::vecResultFalse<int32b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[4][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[8][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[14][i])
              << "The vector relation operations is wrong.";
        }
      }
      {
        const cl::int3 expected{Config::vecResultTrue<int32b>(),
                                Config::vecResultFalse<int32b>(),
                                Config::vecResultFalse<int32b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[5][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[9][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[15][i])
              << "The vector relation operations is wrong.";
        }
      }
      {
        const cl::int3 expected{Config::vecResultFalse<int32b>(),
                                Config::vecResultTrue<int32b>(),
                                Config::vecResultTrue<int32b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[6][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[10][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[12][i])
              << "The vector relation operations is wrong.";
        }
      }
      {
        const cl::int3 expected{Config::vecResultFalse<int32b>(),
                                Config::vecResultFalse<int32b>(),
                                Config::vecResultTrue<int32b>()};
        for (uint32b i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[7][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[11][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[13][i])
              << "The vector relation operations is wrong.";
        }
      }
    }
    {
      std::array<cl::float4, 2> result;
      buffer16->read(result.data(), result.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        ASSERT_FLOAT_EQ(expected, result[0][i])
              << "The vector conditional operations is wrong.";
      }
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 5);
        ASSERT_FLOAT_EQ(expected, result[1][i])
              << "The vector conditional operations is wrong.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(VectorTest, RelationalOperationsTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b num_scalars = 32;
    auto buffer1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(num_scalars);
    auto buffer2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(1);
    auto buffer3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(1);
    auto buffer4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceOnly);
    buffer4->setSize(1);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector, testRelationalOperations));
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<int32b, num_scalars> results;
      buffer1->read(results.data(), num_scalars, 0, 0);
      uint32b scalar_index = 0;
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";

      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";

      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";

      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";

      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";

      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";

      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
