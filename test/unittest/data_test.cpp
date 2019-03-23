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
// Zisc
#include "zisc/floating_point.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/data.hpp"
// Test
#include "test.hpp"

TEST(DataTest, CopyBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 16;
    constexpr uint32b initializer = 100;

    std::array<uint32b, n> init;
    for (std::size_t i = 0; i < init.size(); ++i)
      init[i] = initializer;

    auto buffer0 = makeBuffer<uint32b>(device.get(), BufferUsage::kHostTSrc);
    buffer0->setSize(n);
    buffer0->write(init.data(), init.size(), 0, 0);

    std::array<uint32b, 12> input;
    for (std::size_t i = 0; i < input.size(); ++i)
      input[i] = zisc::cast<uint32b>(i + 1);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kHostTSrc);
    buffer1->setSize(n);
    buffer1->write(input.data(), input.size(), 2, 0);

    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    buffer2->setSize(n);
    buffer0->copyTo(buffer2.get(), n, 0, 0, 0);
    buffer1->copyTo(buffer2.get(), 12, 2, 2, 0);

    auto buffer3 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(n);

    auto kernel = makeZinvulKernel(device.get(), data, copyBufferTest, 1);
    kernel->run(*buffer2, *buffer3, {1}, 0);
    device->waitForCompletion();

    auto buffer4 = makeBuffer<uint32b>(device.get(), BufferUsage::kHostTDst);
    buffer4->setSize(n);
    buffer3->copyTo(buffer4.get(), n, 0, 0, 0);

    {
      std::array<uint32b, 2> result;
      buffer4->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(initializer, result[0]) << "The copyBuffer func is wrong.";
      EXPECT_EQ(initializer, result[1]) << "The copyBuffer func is wrong.";
    }
    {
      std::array<uint32b, 14> result;
      buffer4->read(result.data(), result.size(), 2, 0);
      for (std::size_t i = 0; i < 12; ++i)
        EXPECT_EQ(i + 1, result[i]) << "The copyBuffer func is wrong.";
      EXPECT_EQ(initializer, result[12]) << "The copyBuffer func is wrong.";
      EXPECT_EQ(initializer, result[13]) << "The copyBuffer func is wrong.";
    }

    constexpr uint32b resolution = 500;

    auto buffer5 = makeBuffer<int32b>(device.get(), BufferUsage::kHostTSrcDst);
    buffer5->setSize(resolution);
    {
      std::array<int32b, resolution> table;
      for (std::size_t i = 0; i < table.size(); ++i)
        table[i] = zisc::cast<int>(i);
      buffer5->write(table.data(), table.size(), 0, 0);
    }
    auto buffer6 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    buffer6->setSize(resolution);
    buffer5->copyTo(buffer6.get(), resolution, 0, 0, 0);
    auto buffer7 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    buffer7->setSize(1);
    buffer7->write(&resolution, 1, 0, 0);

    auto kernel2 = makeZinvulKernel(device.get(), data, multiplyBufferTest, 1);
    kernel2->run(*buffer6, *buffer7, {resolution}, 0);
    device->waitForCompletion();

    buffer6->copyTo(buffer5.get(), resolution, 0, 0, 0);
    {
      std::array<int32b, resolution> table;
      buffer5->read(table.data(), resolution, 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i) {
        const int expected = 2 * zisc::cast<int>(i);
        ASSERT_EQ(expected, table[i]) << "The buffer readwrite is wrong.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}


TEST(DataTest, TypeSizeTest)
{
  using namespace zinvul;

  constexpr std::size_t num_of_tables = 41;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(num_of_tables);

    auto kernel = makeZinvulKernel(device.get(), data, getTypeSize, 1);
    kernel->run(*buffer1, {1}, 0);
    device->waitForCompletion();

    std::vector<uint32b> result;
    result.resize(num_of_tables);
    buffer1->read(result.data(), result.size(), 0, 0);

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

    auto buffer1 = makeBuffer<int16b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(1);
    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer2->setSize(1);
    auto buffer3 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(1);
    auto buffer4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
    buffer4->setSize(1);
    auto buffer5 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
    buffer5->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), data, testDataConversion, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, *buffer5, {1}, 0);
    device->waitForCompletion();

    {
      constexpr int16b expected = 3;
      int16b result;
      buffer1->read(&result, 1, 0, 0);
      ASSERT_EQ(expected, result) << "The 'convert_short' func is wrong.";
    }
    {
      constexpr float expected = 3.0f;
      float result;
      buffer2->read(&result, 1, 0, 0);
      ASSERT_EQ(expected, result) << "The 'convert_float' func is wrong.";
    }
    {
      constexpr uint32b expected = 5;
      uint32b result;
      buffer3->read(&result, 1, 0, 0);
      ASSERT_EQ(expected, result) << "The 'convert_uint' func is wrong.";
    }
    {
      const cl::float4 expected{1.0f, 2.0f, 3.0f, 4.0f};
      cl::float4 result;
      buffer4->read(&result, 1, 0, 0);
      for (std::size_t i = 0; i < expected.size(); ++i)
        ASSERT_EQ(expected[i], result[i]) << "The 'convert_float4' func is wrong.";
    }
    {
      const cl::int4 expected{1, 2, 3, 4};
      cl::int4 result;
      buffer5->read(&result, 1, 0, 0);
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

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(1);
    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer2->setSize(1);
    auto buffer3 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(1);
    auto buffer4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
    buffer4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), data, testDataReinterpreting, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, {1}, 0);
    device->waitForCompletion();

    {
      constexpr uint32b expected = 0x3f800000u;
      uint32b result;
      buffer1->read(&result, 1, 0, 0);
      ASSERT_EQ(expected, result) << "The 'as_uint' func is wrong.";
    }
    {
      constexpr float expected = 1.0f;
      float result;
      buffer2->read(&result, 1, 0, 0);
      ASSERT_EQ(expected, result) << "The 'as_float' func is wrong.";
    }
    {
      const cl::uint4 expected{0x3f800000u, 0x40000000u, 0x40800000u, 0x41000000u};
      cl::uint4 result;
      buffer3->read(&result, 1, 0, 0);
      for (std::size_t i = 0; i < expected.size(); ++i)
        ASSERT_EQ(expected[i], result[i]) << "The 'as_uint4' func is wrong.";
    }
    {
      const cl::float4 expected{1.0f, 2.0f, 4.0f, 8.0f};
      cl::float4 result;
      buffer4->read(&result, 1, 0, 0);
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

    auto buffer1 = makeBuffer<cl::short2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(1);
    auto buffer2 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
    buffer2->setSize(1);
    auto buffer3 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(1);
    auto buffer4 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer4->setSize(5);
    auto buffer5 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer5->setSize(8);
    auto buffer6 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer6->setSize(5);
    auto buffer7 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer7->setSize(5);
    auto buffer8 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer8->setSize(5);
    auto buffer9 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer9->setSize(5);
    auto buffer10 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer10->setSize(5);
    auto buffer11 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer11->setSize(5);
    auto buffer12 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
    buffer12->setSize(3);
    auto buffer13 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
    buffer13->setSize(3);
    auto buffer14 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
    buffer14->setSize(11);
    auto buffer15 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
    buffer15->setSize(16);

    auto kernel = makeZinvulKernel(device.get(), data, testVectorOperations, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, *buffer5, *buffer6, *buffer7, *buffer8, *buffer9, *buffer10, *buffer11, *buffer12, *buffer13, *buffer14, *buffer15, {1}, 0);
    device->waitForCompletion();

    {
      const cl::short2 expected{20, 23};
      cl::short2 result;
      buffer1->read(&result, 1, 0, 0);
      for (std::size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector addition operations are wrong.";
      }
    }
    {
      const cl::int3 expected{3, 2, 1};
      cl::int3 result;
      buffer2->read(&result, 1, 0, 0);
      for (std::size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector subtraction operations are wrong.";
      }
    }
    {
      const cl::uint4 expected{540u, 1440u, 2772u, 4608u};
      cl::uint4 result;
      buffer3->read(&result, 1, 0, 0);
      for (std::size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i])
            << "The vector multiplication operations are wrong.";
      }
    }
    {
      std::array<cl::float2, 5> result;
      buffer4->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int2, 8> result;
      buffer5->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int2, 5> result;
      buffer6->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int2, 5> result;
      buffer7->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int2, 5> result;
      buffer8->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int2, 5> result;
      buffer9->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int2, 5> result;
      buffer10->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int2, 5> result;
      buffer11->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int3, 3> result;
      buffer12->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int3, 3> result;
      buffer13->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int3, 11> result;
      buffer14->read(result.data(), result.size(), 0, 0);
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
      std::array<cl::int3, 16> result;
      buffer15->read(result.data(), result.size(), 0, 0);
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

TEST(DataTest, HalfLoadStoreTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto input_scalar =
        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTDst);
    input_scalar->setSize(3);
    {
      std::array<cl::half, 3> input;
      input[0] = zisc::SingleFloat::fromFloat(1.0f);
      input[1] = zisc::SingleFloat::fromFloat(2.0f);
      input[2] = zisc::SingleFloat::fromFloat(4.0f);
      input_scalar->write(input.data(), input.size(), 0, 0);
    }
    auto output_scalar =
        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTSrc);
    output_scalar->setSize(3);
    auto input_vector2 =
        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTDst);
    input_vector2->setSize(4);
    {
      std::array<cl::half, 4> input;
      input[0] = zisc::SingleFloat::fromFloat(1.0f);
      input[1] = zisc::SingleFloat::fromFloat(2.0f);
      input[2] = zisc::SingleFloat::fromFloat(4.0f);
      input[3] = zisc::SingleFloat::fromFloat(8.0f);
      input_vector2->write(input.data(), input.size(), 0, 0);
    }
    auto output_vector2 =
        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTSrc);
    output_vector2->setSize(4);
    auto input_vector3 =
        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTDst);
    input_vector3->setSize(6);
    {
      std::array<cl::half, 6> input;
      input[0] = zisc::SingleFloat::fromFloat(1.0f);
      input[1] = zisc::SingleFloat::fromFloat(2.0f);
      input[2] = zisc::SingleFloat::fromFloat(4.0f);
      input[3] = zisc::SingleFloat::fromFloat(8.0f);
      input[4] = zisc::SingleFloat::fromFloat(16.0f);
      input[5] = zisc::SingleFloat::fromFloat(32.0f);
      input_vector3->write(input.data(), input.size(), 0, 0);
    }
    auto output_vector3 =
        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTSrc);
    output_vector3->setSize(6);
    auto input_vector4 =
        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTDst);
    input_vector4->setSize(8);
    {
      std::array<cl::half, 8> input;
      input[0] = zisc::SingleFloat::fromFloat(1.0f);
      input[1] = zisc::SingleFloat::fromFloat(2.0f);
      input[2] = zisc::SingleFloat::fromFloat(4.0f);
      input[3] = zisc::SingleFloat::fromFloat(8.0f);
      input[4] = zisc::SingleFloat::fromFloat(16.0f);
      input[5] = zisc::SingleFloat::fromFloat(32.0f);
      input[6] = zisc::SingleFloat::fromFloat(64.0f);
      input[7] = zisc::SingleFloat::fromFloat(128.0f);
      input_vector4->write(input.data(), input.size(), 0, 0);
    }
    auto output_vector4 =
        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTSrc);
    output_vector4->setSize(8);

    auto kernel = makeZinvulKernel(device.get(), data, testHalfLoadStore, 1);
    kernel->run(*input_scalar, *output_scalar, *input_vector2, *output_vector2,
        *input_vector3, *output_vector3, *input_vector4, *output_vector4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<cl::half, 3> result;
      output_scalar->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        const float expected = static_cast<float>(2u << i);
        const auto r = zisc::SingleFloat{result[i]};
        ASSERT_EQ(expected, r.toFloat()) << "Loading and storing half failed.";
      }
    }

//    {
//      std::array<cl::half, 4> result;
//      output_vector2->read(result.data(), result.size(), 0, 0);
//      for (std::size_t i = 0; i < result.size(); ++i) {
//        const float expected = static_cast<float>(2u << i);
//        const auto r = zisc::SingleFloat{result[i]};
//        ASSERT_EQ(expected, r.toFloat()) << "Loading and storing half2 failed.";
//      }
//    }
//
//    {
//      std::array<cl::half, 6> result;
//      output_vector3->read(result.data(), result.size(), 0, 0);
//      for (std::size_t i = 0; i < result.size(); ++i) {
//        const float expected = static_cast<float>(2u << i);
//        const auto r = zisc::SingleFloat{result[i]};
//        ASSERT_EQ(expected, r.toFloat()) << "Loading and storing half3 failed.";
//      }
//    }

    {
      std::array<cl::half, 8> result;
      output_vector4->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < result.size(); ++i) {
        const float expected = static_cast<float>(2u << i);
        const auto r = zisc::SingleFloat{result[i]};
        ASSERT_EQ(expected, r.toFloat()) << "Loading and storing half4 failed.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

namespace {

using Ray1 = zinvul::data::KernelGroup::Ray1;
using Ray2 = zinvul::data::KernelGroup::Ray2;

} // namespace

TEST(DataTest, VariablePointerTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 16;

    auto buffer0 = makeBuffer<::Ray1>(device.get(), BufferUsage::kDeviceTSrc);
    buffer0->setSize(2 * resolution);

    auto buffer1 = makeBuffer<::Ray2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(2 * resolution);

    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    buffer2->setSize(1);
    buffer2->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), data, testVariablePointer, 1);
    kernel->run(*buffer0, *buffer1, *buffer2, {resolution}, 0);
    device->waitForCompletion();

    {
      std::array<::Ray1, 2 * resolution> results;
      buffer0->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        for (std::size_t j = 0; j < 4; ++j) {
          ASSERT_FLOAT_EQ(zisc::cast<float>(j + 1), result.origin_[j])
              << "The variable pointer for "
              << (zisc::isOdd(j) ? "local" : "global") << " is wrong.";
          ASSERT_FLOAT_EQ(1.0f, result.dir_[j])
              << "The variable pointer for "
              << (zisc::isOdd(j) ? "local" : "global") << " is wrong.";
        }
      }
    }
    {
      std::array<::Ray2, 2 * resolution> results;
      buffer1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i) {
        if (zisc::isOdd(i))
          continue;
        const auto& result = results[i];
        for (std::size_t j = 0; j < 3; ++j) {
          ASSERT_FLOAT_EQ(zisc::cast<float>(j + 1), result.origin_[j])
              << "The variable pointer for "
              << (zisc::isOdd(j) ? "local" : "global") << " is wrong.";
          ASSERT_FLOAT_EQ(1.0f, result.dir_[j])
              << "The variable pointer for "
              << (zisc::isOdd(j) ? "local" : "global") << " is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, Int8bBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint8b) * 8);

    auto buffer = makeBuffer<int8b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer->setSize(resolution);

    auto kernel = makeZinvulKernel(device.get(), data, testInt8bBuffer, 1);
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<int8b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const int32b e = zisc::cast<int32b>(std::numeric_limits<int8b>::min()) +
            zisc::cast<int32b>(i);
        const int8b expected = zisc::cast<int8b>(e);
        const int8b result = results[i];
        ASSERT_EQ(expected, result) << "Int8b buffer doesn't work";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, Uint8bBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint8b) * 8);

    auto buffer = makeBuffer<uint8b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer->setSize(resolution);

    auto kernel = makeZinvulKernel(device.get(), data, testUint8bBuffer, 1);
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<uint8b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const uint8b expected = zisc::cast<uint8b>(i);
        const uint8b result = results[i];
        ASSERT_EQ(expected, result) << "Uint8b buffer doesn't work";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, Int16bBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);

    auto buffer = makeBuffer<int16b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer->setSize(resolution);

    auto kernel = makeZinvulKernel(device.get(), data, testInt16bBuffer, 1);
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<int16b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const int32b e = zisc::cast<int32b>(std::numeric_limits<int16b>::min()) +
            zisc::cast<int32b>(i);
        const int16b expected = zisc::cast<int16b>(e);
        const int16b result = results[i];
        ASSERT_EQ(expected, result) << "Int16b buffer doesn't work";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, Uint16bBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);

    auto buffer = makeBuffer<uint16b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer->setSize(resolution);

    auto kernel = makeZinvulKernel(device.get(), data, testUint16bBuffer, 1);
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<uint16b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const uint16b expected = zisc::cast<uint16b>(i);
        const uint16b result = results[i];
        ASSERT_EQ(expected, result) << "Uint16b buffer doesn't work";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, Int64bBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);

    auto buffer = makeBuffer<int64b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer->setSize(resolution);

    auto kernel = makeZinvulKernel(device.get(), data, testInt64bBuffer, 1);
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<int64b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        int64b e = zisc::cast<int64b>(std::numeric_limits<int16b>::min()) +
            zisc::cast<int64b>(i);
        e = e << 32l;
        const int64b expected = e;
        const int64b result = results[i];
        ASSERT_EQ(expected, result) << "Int64b buffer doesn't work";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, Uint64bBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);

    auto buffer = makeBuffer<uint64b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer->setSize(resolution);

    auto kernel = makeZinvulKernel(device.get(), data, testUint64bBuffer, 1);
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<uint64b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const uint64b expected = zisc::cast<uint64b>(i) << 32ul;
        const uint64b result = results[i];
        ASSERT_EQ(expected, result) << "Uint64b buffer doesn't work";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
TEST(DataTest, CastToUint8bBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint8b) * 8);

    auto buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer->setSize(resolution / 4);

    auto kernel = makeZinvulKernel(device.get(), data, testUint8bBuffer, 1);
    kernel->run(*(buffer->treatAs<uint8b>()), {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> results;
      results.resize(resolution / 4, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const uint8b expected = zisc::cast<uint8b>(i);
        const uint8b* p = zisc::treatAs<const uint8b*>(results.data());
        const uint8b result = p[i];
        ASSERT_EQ(expected, result) << "Casting to uint8b buffer doesn't work";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, CastToUint16bBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);

    auto buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer->setSize(resolution / 2);

    auto kernel = makeZinvulKernel(device.get(), data, testUint16bBuffer, 1);
    kernel->run(*(buffer->treatAs<uint16b>()), {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> results;
      results.resize(resolution / 2, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < resolution; ++i) {
        const uint16b expected = zisc::cast<uint16b>(i);
        const uint16b* p = zisc::treatAs<const uint16b*>(results.data());
        const uint16b result = p[i];
        ASSERT_EQ(expected, result) << "Casting to uint16b buffer doesn't work";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, RelationalOperationsTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t num_scalars = 32;
    auto buffer1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(num_scalars);
    auto buffer2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
    buffer2->setSize(1);
    auto buffer3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(1);
    auto buffer4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
    buffer4->setSize(1);

    auto kernel = makeZinvulKernel(device.get(), data, testRelationalOperations, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, {1}, 0);
    device->waitForCompletion();

    {
      std::array<int32b, num_scalars> results;
      buffer1->read(results.data(), num_scalars, 0, 0);
      std::size_t scalar_index = 0;
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_TRUE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";

      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
//      EXPECT_FALSE(results[scalar_index++]) << "Relational operation is wrong.";
      scalar_index++;
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

TEST(DataTest, CastUint8bToFloat)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 100000;

    auto buffer1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(resolution);

    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTDst);
    buffer2->setSize(resolution);
    {
      std::vector<float> inputs;
      inputs.resize(resolution);
      for (std::size_t i = 0; i < resolution; ++i)
        inputs[i] = zisc::cast<float>(i);
      buffer2->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer3 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(resolution);

    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), data, testCastUint8bToFloat, 1);
    kernel->run(*(buffer1->treatAs<uint8b>()), *(buffer2->treatAs<uint8b>()), *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> wresults;
      wresults.resize(resolution);
      buffer1->read(wresults.data(), wresults.size(), 0, 0);
      std::vector<float> rresults;
      rresults.resize(resolution);
      buffer3->read(rresults.data(), rresults.size(), 0, 0);
      for (std::size_t i = 0; i < wresults.size(); ++i) {
        {
          const float expected = zisc::cast<float>(i);
          const auto result = wresults[i];
          ASSERT_EQ(expected, result) << "Casting uint8b* to float* failed.";
        }
        {
          const float expected = zisc::cast<float>(i);
          const auto result = rresults[i];
          ASSERT_EQ(expected, result) << "Casting uint8b* to float* failed.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, CastUint16bToFloat)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 100000;

    auto buffer1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(resolution);

    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTDst);
    buffer2->setSize(resolution);
    {
      std::vector<float> inputs;
      inputs.resize(resolution);
      for (std::size_t i = 0; i < resolution; ++i)
        inputs[i] = zisc::cast<float>(i);
      buffer2->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer3 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(resolution);

    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), data, testCastUint16bToFloat, 1);
    kernel->run(*(buffer1->treatAs<uint16b>()), *(buffer2->treatAs<uint16b>()), *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> wresults;
      wresults.resize(resolution);
      buffer1->read(wresults.data(), wresults.size(), 0, 0);
      std::vector<float> rresults;
      rresults.resize(resolution);
      buffer3->read(rresults.data(), rresults.size(), 0, 0);
      for (std::size_t i = 0; i < wresults.size(); ++i) {
        {
          const float expected = zisc::cast<float>(i);
          const auto result = wresults[i];
          ASSERT_EQ(expected, result) << "Casting uint16b* to float* failed.";
        }
        {
          const float expected = zisc::cast<float>(i);
          const auto result = rresults[i];
          ASSERT_EQ(expected, result) << "Casting uint16b* to float* failed.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, CastUint32bToFloat)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 100000;

    auto buffer1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(resolution);

    auto buffer2 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTDst);
    buffer2->setSize(resolution);
    {
      std::vector<float> inputs;
      inputs.resize(resolution);
      for (std::size_t i = 0; i < resolution; ++i)
        inputs[i] = zisc::cast<float>(i);
      buffer2->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer3 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(resolution);

    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), data, testCastUint32bToFloat, 1);
    kernel->run(*(buffer1->treatAs<uint32b>()), *(buffer2->treatAs<uint32b>()), *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> wresults;
      wresults.resize(resolution);
      buffer1->read(wresults.data(), wresults.size(), 0, 0);
      std::vector<float> rresults;
      rresults.resize(resolution);
      buffer3->read(rresults.data(), rresults.size(), 0, 0);
      for (std::size_t i = 0; i < wresults.size(); ++i) {
        {
          const float expected = zisc::cast<float>(i);
          const auto result = wresults[i];
          ASSERT_EQ(expected, result) << "Casting uint32b* to float* failed.";
        }
        {
          const float expected = zisc::cast<float>(i);
          const auto result = rresults[i];
          ASSERT_EQ(expected, result) << "Casting uint32b* to float* failed.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
