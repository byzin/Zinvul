/*!
  \file relational_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/relational.hpp"
// Test
#include "test.hpp"

TEST(RelationalTest, RelationalF1Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 24;

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(relational, testRelationalF1));
    kernel->run(*result_buff, {1}, 0);
    device->waitForCompletion();

//    const char* error_message = "Relational f1 test failed.";
    {
      std::vector<int32b> results;
      results.resize(n);
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b index = 0;
      ASSERT_TRUE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_FALSE(results[index++]);
      ASSERT_TRUE(results[index++]);
      ASSERT_FALSE(results[index++]);
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(RelationalTest, RelationalF4Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 12;

    auto result_buff = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(relational, testRelationalF4));
    kernel->run(*result_buff, {1}, 0);
    device->waitForCompletion();

//    const char* error_message = "Relational f4 test failed.";
    {
      std::vector<cl::int4> results;
      results.resize(n);
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b index = 0;
      // isEqual
      {
        const auto result = results[index++];
        ASSERT_FALSE(result[0]);
        ASSERT_TRUE(result[1]);
        ASSERT_FALSE(result[2]);
        ASSERT_FALSE(result[3]);
      }
      // isNotEqual
      {
        const auto result = results[index++];
        ASSERT_TRUE(result[0]);
        ASSERT_FALSE(result[1]);
        ASSERT_TRUE(result[2]);
        ASSERT_TRUE(result[3]);
      }
      // isGreater
      {
        const auto result = results[index++];
        ASSERT_FALSE(result[0]);
        ASSERT_FALSE(result[1]);
        ASSERT_TRUE(result[2]);
        ASSERT_TRUE(result[3]);
      }
      // isGreater
      {
        const auto result = results[index++];
        ASSERT_TRUE(result[0]);
        ASSERT_TRUE(result[1]);
        ASSERT_FALSE(result[2]);
        ASSERT_TRUE(result[3]);
      }
      // isGreaterEqual
      {
        const auto result = results[index++];
        ASSERT_FALSE(result[0]);
        ASSERT_TRUE(result[1]);
        ASSERT_TRUE(result[2]);
        ASSERT_TRUE(result[3]);
      }
      // isLess
      {
        const auto result = results[index++];
        ASSERT_TRUE(result[0]);
        ASSERT_FALSE(result[1]);
        ASSERT_FALSE(result[2]);
        ASSERT_FALSE(result[3]);
      }
      // isLess
      {
        const auto result = results[index++];
        ASSERT_FALSE(result[0]);
        ASSERT_FALSE(result[1]);
        ASSERT_TRUE(result[2]);
        ASSERT_FALSE(result[3]);
      }
      // isLessEqual
      {
        const auto result = results[index++];
        ASSERT_TRUE(result[0]);
        ASSERT_TRUE(result[1]);
        ASSERT_FALSE(result[2]);
        ASSERT_FALSE(result[3]);
      }
      // isInf
      {
        const auto result = results[index++];
        ASSERT_FALSE(result[0]);
        ASSERT_FALSE(result[1]);
        ASSERT_TRUE(result[2]);
        ASSERT_TRUE(result[3]);
      }
      // isNan
      {
        const auto result = results[index++];
        ASSERT_FALSE(result[0]);
        ASSERT_FALSE(result[1]);
        ASSERT_FALSE(result[2]);
        ASSERT_TRUE(result[3]);
      }
      // isSignBitSet
      {
        const auto result = results[index++];
        EXPECT_FALSE(result[0]);
        EXPECT_FALSE(result[1]);
        EXPECT_TRUE(result[2]);
        EXPECT_FALSE(result[3]);
      }
      // isSignBitSet
      {
        const auto result = results[index++];
        EXPECT_FALSE(result[0]);
        EXPECT_TRUE(result[1]);
        EXPECT_FALSE(result[2]);
        EXPECT_TRUE(result[3]);
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(RelationalTest, BitselectUTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 1;

    auto result1_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(n);
    auto result2_buff = makeStorageBuffer<cl::uchar2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(n);
    auto result3_buff = makeStorageBuffer<cl::ushort3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(n);
    auto result4_buff = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(relational, testBitselectU));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

//    const char* error_message = "The 'selectBit' func for uint test failed.";
    {
      std::array<uint32b, n> results;
      result1_buff->read(results.data(), results.size(), 0, 0);

//      {
//        const uint32b expected = 0b010110101'010110101'01011010'01011010u;
//        EXPECT_EQ(expected, results[0]) << error_message;
//      }
    }
    {
      std::array<cl::uchar2, n> results;
      result2_buff->read(results.data(), results.size(), 0, 0);

//      {
//        const cl::uchar2 expected{0, 0};
//        for (uint32b i = 0; i < expected.size(); ++i)
//          EXPECT_EQ(expected[i], results[0][i]) << error_message;
//      }
    }
    {
      std::array<cl::ushort3, n> results;
      result3_buff->read(results.data(), results.size(), 0, 0);

//      {
//        const cl::ushort3 expected{0, 0, 0};
//        for (uint32b i = 0; i < expected.size(); ++i)
//          EXPECT_EQ(expected[i], results[0][i]) << error_message;
//      }
    }
    {
      std::array<cl::uint4, n> results;
      result4_buff->read(results.data(), results.size(), 0, 0);

//      {
//        const cl::uint4 expected{0, 0, 0, 0};
//        for (uint32b i = 0; i < expected.size(); ++i)
//          EXPECT_EQ(expected[i], results[0][i]) << error_message;
//      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(RelationalTest, SelectITest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 2;

    auto result1_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(n);
    auto result2_buff = makeStorageBuffer<cl::char2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(n);
    auto result3_buff = makeStorageBuffer<cl::short3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(n);
    auto result4_buff = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(relational, testSelectI));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'testSelect' func for int test failed.";
    {
      std::array<int32b, n> results;
      result1_buff->read(results.data(), results.size(), 0, 0);

      {
        const int32b expected = 1;
        ASSERT_EQ(expected, results[0]) << error_message;
      }
      {
        const int32b expected = -1;
        ASSERT_EQ(expected, results[1]) << error_message;
      }
    }
    {
      std::array<cl::char2, n> results;
      result2_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::char2 expected{1, -2};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::char2 expected{-1, 2};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::short3, n> results;
      result3_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::short3 expected{1, -2, 3};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::short3 expected{-1, 2, -3};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::int4, n> results;
      result4_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::int4 expected{1, -2, 3, -4};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::int4 expected{-1, 2, -3, 4};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(RelationalTest, SelectUTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 2;

    auto result1_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(n);
    auto result2_buff = makeStorageBuffer<cl::uchar2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(n);
    auto result3_buff = makeStorageBuffer<cl::ushort3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(n);
    auto result4_buff = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(relational, testSelectU));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'testSelect' func for uint test failed.";
    {
      std::array<uint32b, n> results;
      result1_buff->read(results.data(), results.size(), 0, 0);

      {
        const uint32b expected = 1;
        ASSERT_EQ(expected, results[0]) << error_message;
      }
      {
        const uint32b expected = 2;
        ASSERT_EQ(expected, results[1]) << error_message;
      }
    }
    {
      std::array<cl::uchar2, n> results;
      result2_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::uchar2 expected{1, 4};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::uchar2 expected{3, 2};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::ushort3, n> results;
      result3_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::ushort3 expected{1, 5, 3};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::ushort3 expected{4, 2, 6};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::uint4, n> results;
      result4_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::uint4 expected{1, 6, 3, 8};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::uint4 expected{5, 2, 7, 4};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(RelationalTest, SelectFTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 2;

    auto result1_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(n);
    auto result2_buff = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(n);
    auto result3_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(n);
    auto result4_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(relational, testSelectF));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'testSelect' func for float test failed.";
    {
      std::array<float, n> results;
      result1_buff->read(results.data(), results.size(), 0, 0);

      {
        const float expected = 1.0f;
        ASSERT_FLOAT_EQ(expected, results[0]) << error_message;
      }
      {
        const float expected = -1.0f;
        ASSERT_FLOAT_EQ(expected, results[1]) << error_message;
      }
    }
    {
      std::array<cl::float2, n> results;
      result2_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::float2 expected{1.0f, -2.0f};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_FLOAT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::float2 expected{-1.0f, 2.0f};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_FLOAT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::float3, n> results;
      result3_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::float3 expected{1.0f, -2.0f, 3.0f};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::float3 expected{-1.0f, 2.0f, -3.0f};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::float4, n> results;
      result4_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::float4 expected{1.0f, -2.0f, 3.0f, -4.0f};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::float4 expected{-1.0f, 2.0f, -3.0f, 4.0f};
        for (uint32b i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
