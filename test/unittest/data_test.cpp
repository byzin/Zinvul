/*!
  \file data_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <cstddef>
#include <iostream>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/data.hpp"
// Test
#include "test.hpp"

TEST(DataTest, TypeSizeTest)
{
  using namespace zinvul;

  constexpr std::size_t num_of_tables = 41;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceToHost);
    buffer1->setSize(num_of_tables);

    auto kernel = makeZinvulKernel(device.get(), data, getTypeSize, 1);
    kernel->run(*buffer1, {1}, 0);
    device->waitForCompletion();

    std::vector<uint32b> result;
    result.resize(num_of_tables);
    buffer1->read(result.data());

    std::size_t table_index = 0;
    EXPECT_EQ(sizeof(int8b), result[table_index++])
        << "Using 'char' isn't recommended.";
    EXPECT_EQ(sizeof(int8b), result[table_index++])
        << "The size of 'int8b' is wrong.";
    EXPECT_EQ(sizeof(int16b), result[table_index++])
        << "Using 'short' isn't recommended.";
    EXPECT_EQ(sizeof(int16b), result[table_index++])
        << "The size of 'int16b' is wrong.";
    EXPECT_EQ(sizeof(int32b), result[table_index++])
        << "Using 'int' isn't recommended.";
    EXPECT_EQ(sizeof(int32b), result[table_index++])
        << "The size of 'int32b' is wrong.";
    EXPECT_EQ(sizeof(int64b), result[table_index++])
        << "Using 'long' isn't recommended.";
    EXPECT_EQ(sizeof(int64b), result[table_index++])
        << "The size of 'int64b' is wrong.";
    EXPECT_EQ(sizeof(uint8b), result[table_index++])
        << "Using 'unsigned char' isn't recommended.";
    EXPECT_EQ(sizeof(uint8b), result[table_index++])
        << "Using 'uchar' isn't recommended.";
    EXPECT_EQ(sizeof(uint8b), result[table_index++])
        << "The size of 'uint8b' is wrong.";
    EXPECT_EQ(sizeof(uint16b), result[table_index++])
        << "Using 'unsigned short' isn't recommended.";
    EXPECT_EQ(sizeof(uint16b), result[table_index++])
        << "Using 'ushort' isn't recommended.";
    EXPECT_EQ(sizeof(uint16b), result[table_index++])
        << "The size of 'uint16b' is wrong.";
    EXPECT_EQ(sizeof(uint32b), result[table_index++])
        << "Using 'unsigned int' isn't recommended.";
    EXPECT_EQ(sizeof(uint32b), result[table_index++])
        << "Using 'uint' isn't recommended.";
    EXPECT_EQ(sizeof(uint32b), result[table_index++])
        << "The size of 'uint32b' is wrong.";
    EXPECT_EQ(sizeof(uint64b), result[table_index++])
        << "Using 'unsigned long' isn't recommended.";
    EXPECT_EQ(sizeof(uint64b), result[table_index++])
        << "Using 'ulong' isn't recommended.";
    EXPECT_EQ(sizeof(uint64b), result[table_index++])
        << "The size of 'uint64b' is wrong.";
    EXPECT_EQ(sizeof(cl::half), result[table_index++])
        << "The size of 'half' is wrong.";
    EXPECT_EQ(sizeof(float), result[table_index++])
        << "The size of 'float' is wrong.";
    EXPECT_EQ(sizeof(cl::char2), result[table_index++])
        << "The size of 'char2' is wrong.";
    EXPECT_EQ(sizeof(cl::char3), result[table_index++])
        << "The size of 'char3' is wrong.";
    EXPECT_EQ(sizeof(cl::char4), result[table_index++])
        << "The size of 'char4' is wrong.";
    EXPECT_EQ(sizeof(cl::short2), result[table_index++])
        << "The size of 'short2' is wrong.";
    EXPECT_EQ(sizeof(cl::short3), result[table_index++])
        << "The size of 'short3' is wrong.";
    EXPECT_EQ(sizeof(cl::short4), result[table_index++])
        << "The size of 'short4' is wrong.";
    EXPECT_EQ(sizeof(cl::int2), result[table_index++])
        << "The size of 'int2' is wrong.";
    EXPECT_EQ(sizeof(cl::int3), result[table_index++])
        << "The size of 'int3' is wrong.";
    EXPECT_EQ(sizeof(cl::int4), result[table_index++])
        << "The size of 'int4' is wrong.";
    EXPECT_EQ(sizeof(cl::long2), result[table_index++])
        << "The size of 'long2' is wrong.";
    EXPECT_EQ(sizeof(cl::long3), result[table_index++])
        << "The size of 'long3' is wrong.";
    EXPECT_EQ(sizeof(cl::long4), result[table_index++])
        << "The size of 'long4' is wrong.";
    EXPECT_EQ(sizeof(cl::half2), result[table_index++])
        << "The size of 'half2' is wrong.";
    EXPECT_EQ(sizeof(cl::half3), result[table_index++])
        << "The size of 'half3' is wrong.";
    EXPECT_EQ(sizeof(cl::half4), result[table_index++])
        << "The size of 'half4' is wrong.";
    EXPECT_EQ(sizeof(cl::float2), result[table_index++])
        << "The size of 'float2' is wrong.";
    EXPECT_EQ(sizeof(cl::float3), result[table_index++])
        << "The size of 'float3' is wrong.";
    EXPECT_EQ(sizeof(cl::float4), result[table_index++])
        << "The size of 'float4' is wrong.";
    constexpr std::size_t num_of_types = num_of_tables - 1;
    EXPECT_EQ(num_of_types, result[table_index]);

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, DataConversionTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<int16b>(device.get(), BufferUsage::kDeviceToHost);
    buffer1->setSize(1);
    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceToHost);
    buffer2->setSize(1);
    auto buffer3 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceToHost);
    buffer3->setSize(1);
    auto buffer4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceToHost);
    buffer4->setSize(1);
    auto buffer5 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceToHost);
    buffer5->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), data, testDataConversion, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, *buffer5, {1}, 0);
    device->waitForCompletion();

    {
      constexpr int16b expected = 3;
      int16b result;
      buffer1->read(&result);
      ASSERT_EQ(expected, result) << "The 'convert_short' func is wrong.";
    }
    {
      constexpr float expected = 3.0f;
      float result;
      buffer2->read(&result);
      ASSERT_EQ(expected, result) << "The 'convert_float' func is wrong.";
    }
    {
      constexpr uint32b expected = 5;
      uint32b result;
      buffer3->read(&result);
      ASSERT_EQ(expected, result) << "The 'convert_uint' func is wrong.";
    }
    {
      const cl::float4 expected{1.0f, 2.0f, 3.0f, 4.0f};
      cl::float4 result;
      buffer4->read(&result);
      for (std::size_t i = 0; i < expected.size(); ++i)
        ASSERT_EQ(expected[i], result[i]) << "The 'convert_float4' func is wrong.";
    }
    {
      const cl::int4 expected{1, 2, 3, 4};
      cl::int4 result;
      buffer5->read(&result);
      for (std::size_t i = 0; i < expected.size(); ++i)
        ASSERT_EQ(expected[i], result[i]) << "The 'convert_int4' func is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, DataReinterpretingTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceToHost);
    buffer1->setSize(1);
    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceToHost);
    buffer2->setSize(1);
    auto buffer3 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceToHost);
    buffer3->setSize(1);
    auto buffer4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceToHost);
    buffer4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), data, testDataReinterpreting, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, {1}, 0);
    device->waitForCompletion();

    {
      constexpr uint32b expected = 0x3f800000u;
      uint32b result;
      buffer1->read(&result);
      ASSERT_EQ(expected, result) << "The 'as_uint' func is wrong.";
    }
    {
      constexpr float expected = 1.0f;
      float result;
      buffer2->read(&result);
      ASSERT_EQ(expected, result) << "The 'as_float' func is wrong.";
    }
    {
      const cl::uint4 expected{0x3f800000u, 0x40000000u, 0x40800000u, 0x41000000u};
      cl::uint4 result;
      buffer3->read(&result);
      for (std::size_t i = 0; i < expected.size(); ++i)
        ASSERT_EQ(expected[i], result[i]) << "The 'as_uint4' func is wrong.";
    }
    {
      const cl::float4 expected{1.0f, 2.0f, 4.0f, 8.0f};
      cl::float4 result;
      buffer4->read(&result);
      for (std::size_t i = 0; i < expected.size(); ++i)
        ASSERT_EQ(expected[i], result[i]) << "The 'as_float4' func is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, VectorOperationsTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<cl::short2>(device.get(), BufferUsage::kDeviceToHost);
    buffer1->setSize(1);
    auto buffer2 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceToHost);
    buffer2->setSize(1);
    auto buffer3 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceToHost);
    buffer3->setSize(1);
    auto buffer4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceToHost);
    buffer4->setSize(5);
    auto buffer5 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceToHost);
    buffer5->setSize(8);
    auto buffer6 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceToHost);
    buffer6->setSize(5);
    auto buffer7 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceToHost);
    buffer7->setSize(5);
    auto buffer8 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceToHost);
    buffer8->setSize(5);
    auto buffer9 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceToHost);
    buffer9->setSize(5);
    auto buffer10 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceToHost);
    buffer10->setSize(5);
    auto buffer11 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceToHost);
    buffer11->setSize(5);
    auto buffer12 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceToHost);
    buffer12->setSize(3);
    auto buffer13 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceToHost);
    buffer13->setSize(3);
    auto buffer14 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceToHost);
    buffer14->setSize(11);
    auto buffer15 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceToHost);
    buffer15->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), data, testVectorOperations, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, *buffer5, *buffer6, *buffer7, *buffer8, *buffer9, *buffer10, *buffer11, *buffer12, *buffer13, *buffer14, *buffer15, {1}, 0);
    device->waitForCompletion();

    {
      const cl::short2 expected{20, 23};
      cl::short2 result;
      buffer1->read(&result);
      for (std::size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector addition operations are wrong.";
      }
    }
    {
      const cl::int3 expected{3, 2, 1};
      cl::int3 result;
      buffer2->read(&result);
      for (std::size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector subtraction operations are wrong.";
      }
    }
    {
      const cl::uint4 expected{540u, 1440u, 2772u, 4608u};
      cl::uint4 result;
      buffer3->read(&result);
      for (std::size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector multiplication operations are wrong.";
      }
    }
    {
      cl::float2 result[5];
      buffer4->read(result);
      {
        const cl::float2 expected{5.0f, 3.0f};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[0][i])
              << "The vector division operations are wrong.";
        }
      }
      {
        const cl::float2 expected{2.5f, 3.0f};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[1][i])
              << "The vector division operations are wrong.";
        }
      }
      {
        const cl::float2 expected{2.0f / 5.0f, 2.0f / 6.0f};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[2][i])
              << "The vector division operations are wrong.";
        }
      }
      {
        const cl::float2 expected{0.5f, 0.5f};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[3][i])
              << "The vector division operations are wrong.";
        }
      }
      {
        const cl::float2 expected{1.0f / 3.0f, 1.0f / 3.0f};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          EXPECT_EQ(expected[i], result[4][i])
              << "The vector division operations are wrong.";
        }
      }
    }
    {
      cl::int2 result[8];
      buffer5->read(result);
      {
        const cl::int2 expected{1, 2};
        for (std::size_t i = 0; i < expected.size(); ++i) {
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
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[3][i])
              << "The vector pre-increment is wrong.";
        }
      }
      {
        const cl::int2 expected{-1, 0};
        for (std::size_t i = 0; i < expected.size(); ++i) {
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
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[7][i])
              << "The vector pre-decrement is wrong.";
        }
      }
    }
    {
      cl::int2 result[5];
      buffer6->read(result);
      {
        const cl::int2 expected{1, 1};
        for (std::size_t i = 0; i < expected.size(); ++i) {
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
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[2][i]) <<
              "The vector reminder operations are wrong.";
        }
      }
    }
    {
      cl::int2 result[5];
      buffer7->read(result);
      const cl::int2 expected{3, 3};
      for (std::size_t i = 0; i < expected.size(); ++i) {
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
      cl::int2 result[5];
      buffer8->read(result);
      const cl::int2 expected{~0, ~0};
      for (std::size_t i = 0; i < expected.size(); ++i) {
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
      cl::int2 result[5];
      buffer9->read(result);
      const cl::int2 expected{~0 - 3, ~0 - 3};
      for (std::size_t i = 0; i < expected.size(); ++i) {
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
      cl::int2 result[5];
      buffer10->read(result);
      const cl::int2 expected{4, 4};
      for (std::size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[0][i]) <<
            "The vector left shift operations are wrong.";
        ASSERT_EQ(expected[i], result[1][i]) <<
            "The vector left shift operations are wrong.";
//        ASSERT_EQ(expected[i], result[2][i]) <<
//            "The vector left shift operations are wrong.";
        ASSERT_EQ(expected[i], result[3][i]) <<
            "The vector left shift operations are wrong.";
        ASSERT_EQ(expected[i], result[4][i]) <<
            "The vector left shift operations are wrong.";
      }
    }
    {
      cl::int2 result[5];
      buffer11->read(result);
      {
        const cl::int2 expected{4, 4};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[0][i]) <<
              "The vector right shift operations are wrong.";
          ASSERT_EQ(expected[i], result[1][i]) <<
              "The vector right shift operations are wrong.";
          ASSERT_EQ(expected[i], result[3][i]) <<
              "The vector right shift operations are wrong.";
          ASSERT_EQ(expected[i], result[4][i]) <<
              "The vector right shift operations are wrong.";
        }
      }
//      {
//        const cl::int2 expected{0, 0};
//        ASSERT_EQ(expected, result[2]) <<
//            "The vector right shift operations are wrong.";
//      }
    }
    {
      cl::int3 result[3];
      buffer12->read(result);
      {
        const cl::int3 expected{kVecFalse, kVecFalse, kVecTrue};
        for (std::size_t i = 0; i < expected.size(); ++i) {
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
      cl::int3 result[3];
      buffer13->read(result);
      {
        const cl::int3 expected{kVecTrue, kVecFalse, kVecTrue};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[0][i]) <<
              "The vector right boolean OR are wrong.";
        }
      }
      {
        const cl::int3 expected{kVecTrue, kVecTrue, kVecTrue};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[1][i]) <<
              "The vector right boolean OR are wrong.";
          ASSERT_EQ(expected[i], result[2][i]) <<
              "The vector right boolean OR are wrong.";
        }
      }
    }
    {
      cl::int3 result[11];
      buffer14->read(result);
      {
        const cl::int3 expected{kVecTrue, kVecFalse, kVecTrue};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[2][i])
              << "The vector equal operation is wrong.";
          ASSERT_NE(expected[i], result[3][i])
              << "The vector equal operation is wrong.";
        }
      }
      {
        const cl::int3 expected{kVecTrue, kVecTrue, kVecTrue};
        for (std::size_t i = 0; i < expected.size(); ++i) {
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
        const cl::int3 expected{kVecFalse, kVecTrue, kVecFalse};
        for (std::size_t i = 0; i < expected.size(); ++i) {
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
      cl::int3 result[16];
      buffer15->read(result);
      {
        const cl::int3 expected{kVecTrue, kVecTrue, kVecFalse};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[4][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[8][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[14][i])
              << "The vector relation operations is wrong.";
        }
      }
      {
        const cl::int3 expected{kVecTrue, kVecFalse, kVecFalse};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[5][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[9][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[15][i])
              << "The vector relation operations is wrong.";
        }
      }
      {
        const cl::int3 expected{kVecFalse, kVecTrue, kVecTrue};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[6][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[10][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[12][i])
              << "The vector relation operations is wrong.";
        }
      }
      {
        const cl::int3 expected{kVecFalse, kVecFalse, kVecTrue};
        for (std::size_t i = 0; i < expected.size(); ++i) {
          ASSERT_EQ(expected[i], result[7][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[11][i])
              << "The vector relation operations is wrong.";
          ASSERT_EQ(expected[i], result[13][i])
              << "The vector relation operations is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }

}
