/*!
  \file data_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
// Zisc
#include "zisc/floating_point.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/data.hpp"
// Test
#include "test.hpp"

TEST(DataTest, PointerTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 30;

    auto buffer0 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTDst);
    buffer0->setSize(5);
    {
      std::array<int32b, 5> inputs{{1, 2, 3, 4, 5}};
      buffer0->write(inputs.data(), inputs.size(), 0, 0);
    }
    auto buffer1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(n);

    auto kernel = makeZinvulKernel(device.get(), data, testPointer, 1);
    kernel->run(*buffer0, *buffer1, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "Pointer operation is wrong.";
    {
      std::array<int32b, n> results;
      buffer1->read(results.data(), results.size(), 0, 0);

      std::size_t i = 0;
      ASSERT_EQ(1, results[i++]) << error_message;
      ASSERT_EQ(2, results[i++]) << error_message;
      ASSERT_EQ(3, results[i++]) << error_message;
      ASSERT_EQ(4, results[i++]) << error_message;
      ASSERT_EQ(5, results[i++]) << error_message;

      ASSERT_EQ(3, results[i++]) << error_message;
      ASSERT_EQ(4, results[i++]) << error_message;
      ASSERT_EQ(4, results[i++]) << error_message;
      ASSERT_EQ(5, results[i++]) << error_message;
      ASSERT_EQ(3, results[i++]) << error_message;

      ASSERT_EQ(5, results[i++]) << error_message;
      ASSERT_EQ(4, results[i++]) << error_message;
      ASSERT_EQ(4, results[i++]) << error_message;
      ASSERT_EQ(3, results[i++]) << error_message;
      ASSERT_EQ(2, results[i++]) << error_message;
      ASSERT_EQ(4, results[i++]) << error_message;

      ASSERT_EQ(15, results[i++]) << error_message;
      ASSERT_EQ(5, results[i++]) << error_message;
      ASSERT_EQ(20, results[i++]) << error_message;
      ASSERT_EQ(10, results[i++]) << error_message;

      ASSERT_EQ(-10, results[i++]) << error_message;
      ASSERT_EQ(10, results[i++]) << error_message;

      ASSERT_FALSE(results[i++]) << error_message;
      ASSERT_TRUE(results[i++]) << error_message;
      ASSERT_FALSE(results[i++]) << error_message;
      ASSERT_FALSE(results[i++]) << error_message;
      ASSERT_TRUE(results[i++]) << error_message;
      ASSERT_TRUE(results[i++]) << error_message;

      ASSERT_FALSE(results[i++]) << error_message;
      ASSERT_TRUE(results[i++]) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, GlobalInstanceTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    using GlobalOption = cl::test::OptionTest;
    GlobalOption option;
    option.init();
    option.setValue1(5u);
    option.setValue3(15u);

    auto buffer0 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer0->setSize(2);

    auto buffer1 = makeBuffer<GlobalOption>(device.get(), BufferUsage::kDeviceTSrcDst);
    buffer1->setSize(1);
    buffer1->write(&option, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), data, testGlobalInstance, 1);
    kernel->run(*buffer0, *buffer1, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "Global option test failed.";
    {
      std::array<uint32b, 2> results;
      buffer0->read(results.data(), results.size(), 0, 0);
      GlobalOption result_option;
      buffer1->read(&result_option, 1, 0, 0);

      ASSERT_EQ(5u, results[0]) << error_message;
      ASSERT_EQ(10u, result_option.getValue2()) << error_message;
      ASSERT_EQ(15u, results[1]) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(DataTest, LocalInstanceTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    using GlobalOption = cl::test::OptionTest;
    GlobalOption option;
    option.init();
    option.setValue1(5u);
    option.setValue3(15u);

    auto buffer0 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer0->setSize(3);

    auto buffer1 = makeBuffer<GlobalOption>(device.get(), BufferUsage::kDeviceTDst);
    buffer1->setSize(1);
    buffer1->write(&option, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), data, testLocalInstance, 1);
    kernel->run(*buffer0, *buffer1, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "Global option test failed.";
    {
      std::array<uint32b, 3> results;
      buffer0->read(results.data(), results.size(), 0, 0);

      ASSERT_EQ(5u, results[0]) << error_message;
      ASSERT_EQ(10u, results[1]) << error_message;
      ASSERT_EQ(15u, results[2]) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

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

TEST(DataTest, TypeCastTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto iinputs = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTDst);
    iinputs->setSize(4);
    {
      std::array<int32b, 4> inputs{{1, 2, 3, 4}};
      iinputs->write(inputs.data(), inputs.size(), 0, 0);
    }
    auto uinputs = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    uinputs->setSize(4);
    {
      std::array<uint32b, 4> inputs{{1, 2, 3, 4}};
      uinputs->write(inputs.data(), inputs.size(), 0, 0);
    }
    auto finputs = makeBuffer<float>(device.get(), BufferUsage::kDeviceTDst);
    finputs->setSize(4);
    {
      std::array<float, 4> inputs{{1.0f, 2.0f, 3.0f, 4.0f}};
      finputs->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto ibuffer1 = makeBuffer<int8b>(device.get(), BufferUsage::kDeviceTSrc);
    ibuffer1->setSize(8);
    auto ibuffer2 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
    ibuffer2->setSize(8);
    auto ibuffer3 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
    ibuffer3->setSize(2);

    auto ubuffer1 = makeBuffer<uint8b>(device.get(), BufferUsage::kDeviceTSrc);
    ubuffer1->setSize(4);
    auto ubuffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    ubuffer2->setSize(4);
    auto ubuffer3 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
    ubuffer3->setSize(2);

    auto fbuffer1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    fbuffer1->setSize(8);
    auto fbuffer2 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
    fbuffer2->setSize(2);

    auto kernel = makeZinvulKernel(device.get(), data, testTypeCast, 1);
    kernel->run(*iinputs, *uinputs, *finputs,
        *ibuffer1, *ibuffer2, *ibuffer3,
        *ubuffer1, *ubuffer2, *ubuffer3,
        *fbuffer1, *fbuffer2,
        {1}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    {
      std::vector<int8b> results;
      results.resize(ibuffer1->size(), 0);
      ibuffer1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const int8b expected = zisc::cast<int8b>(i + 1);
        const int8b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'cast<int8b>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const int8b expected = -zisc::cast<int8b>(i + 1);
        const int8b result = results[i + 4];
        ASSERT_EQ(expected, result) << "The func 'cast<int8b>' failed.";
      }
    }
    {
      std::vector<int32b> results;
      results.resize(ibuffer2->size(), 0);
      ibuffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const int32b expected = zisc::cast<int32b>(i + 1);
        const int32b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'cast<int32b>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const int32b expected = -zisc::cast<int32b>(i + 1);
        const int32b result = results[i + 4];
        ASSERT_EQ(expected, result) << "The func 'cast<int32b>' failed.";
      }
    }
    {
      std::vector<cl::int4> results;
      results.resize(ibuffer3->size());
      ibuffer3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const int32b expected = zisc::cast<int32b>(i + 1);
        const int32b result = results[0][i];
        ASSERT_EQ(expected, result) << "The func 'cast<int4>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const int32b expected = -zisc::cast<int32b>(i + 1);
        const int32b result = results[1][i];
        ASSERT_EQ(expected, result) << "The func 'cast<int4>' failed.";
      }
    }

    {
      std::vector<uint8b> results;
      results.resize(ubuffer1->size(), 0);
      ubuffer1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const uint8b expected = zisc::cast<uint8b>(i + 1);
        const uint8b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'cast<uint8b>' failed.";
      }
    }
    {
      std::vector<uint32b> results;
      results.resize(ubuffer2->size(), 0);
      ubuffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const uint32b expected = zisc::cast<uint32b>(i + 1);
        const uint32b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'cast<uint32b>' failed.";
      }
    }
    {
      std::vector<cl::uint4> results;
      results.resize(ubuffer3->size());
      ubuffer3->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const uint32b expected = zisc::cast<uint32b>(i + 1);
        const uint32b result = results[0][i];
        ASSERT_EQ(expected, result) << "The func 'cast<uint4>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const uint32b expected = zisc::cast<uint32b>(i + 1);
        const uint32b result = results[1][i];
        ASSERT_EQ(expected, result) << "The func 'cast<uint4>' failed.";
      }
    }

    {
      std::vector<float> results;
      results.resize(fbuffer1->size(), 0);
      fbuffer1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'cast<float>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = -zisc::cast<float>(i + 1);
        const float result = results[i + 4];
        EXPECT_FLOAT_EQ(expected, result) << "The func 'cast<float>' failed.";
      }
    }
    {
      std::vector<cl::float4> results;
      results.resize(fbuffer2->size());
      fbuffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[0][i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'cast<float4>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = -zisc::cast<float>(i + 1);
        const float result = results[1][i];
        EXPECT_FLOAT_EQ(expected, result) << "The func 'cast<float4>' failed.";
      }
    }
  }
}

TEST(DataTest, TypeReinterpretingTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto uinputs1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    uinputs1->setSize(4);
    {
      std::array<uint32b, 4> inputs;
      for (std::size_t i = 0; i < 4; ++i) {
        const float v = zisc::cast<float>(i + 1);
        inputs[i] = *zisc::treatAs<const uint32b*>(&v);
      }
      uinputs1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto finputs1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTDst);
    finputs1->setSize(4);
    {
      std::array<float, 4> inputs{{1.0f, 2.0f, 3.0f, 4.0f}};
      finputs1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto ubuffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    ubuffer1->setSize(8);
    auto ubuffer2 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
    ubuffer2->setSize(2);

    auto fbuffer1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    fbuffer1->setSize(8);
    auto fbuffer2 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
    fbuffer2->setSize(2);

    auto kernel = makeZinvulKernel(device.get(), data, testTypeReinterpreting, 1);
    kernel->run(*uinputs1, *finputs1,
        *ubuffer1, *ubuffer2,
        *fbuffer1, *fbuffer2,
        {1}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    {
      std::vector<uint32b> results;
      results.resize(ubuffer1->size(), 0);
      ubuffer1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const float v = zisc::cast<float>(i + 1);
        const uint32b expected = *zisc::treatAs<const uint32b*>(&v);
        const uint32b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'treatAs<uint32b>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const float v = zisc::cast<float>(i + 1);
        const uint32b expected = *zisc::treatAs<const uint32b*>(&v);
        const uint32b result = results[i + 4];
        ASSERT_EQ(expected, result) << "The func 'treatAs<uint32b>' failed.";
      }
    }
    {
      std::vector<cl::uint4> results;
      results.resize(ubuffer2->size(), 0);
      ubuffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const float v = zisc::cast<float>(i + 1);
        const uint32b expected = *zisc::treatAs<const uint32b*>(&v);
        const uint32b result = results[0][i];
        ASSERT_EQ(expected, result) << "The func 'treatAs<uint4>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const float v = zisc::cast<float>(i + 1);
        const uint32b expected = *zisc::treatAs<const uint32b*>(&v);
        const uint32b result = results[1][i];
        ASSERT_EQ(expected, result) << "The func 'treatAs<uint4>' failed.";
      }
    }

    {
      std::vector<float> results;
      results.resize(fbuffer1->size(), 0);
      fbuffer1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'treatAs<float>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[i + 4];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'treatAs<float>' failed.";
      }
    }
    {
      std::vector<cl::float4> results;
      results.resize(fbuffer2->size(), 0);
      fbuffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[0][i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'treatAs<float4>' failed.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[1][i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'treatAs<float4>' failed.";
      }
    }
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
    auto buffer16 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
    buffer16->setSize(2);

    auto kernel = makeZinvulKernel(device.get(), data, testVectorOperations, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, *buffer5, *buffer6, *buffer7, *buffer8, *buffer9, *buffer10, *buffer11, *buffer12, *buffer13, *buffer14, *buffer15, *buffer16, {1}, 0);
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
      const cl::int2 expected{7, 7};
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
        for (std::size_t i = 0; i < expected.size(); ++i) {
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
        for (std::size_t i = 0; i < 3; ++i) {
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
    {
      std::array<cl::float4, 2> result;
      buffer16->read(result.data(), result.size(), 0, 0);
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        ASSERT_FLOAT_EQ(expected, result[0][i])
              << "The vector conditional operations is wrong.";
      }
      for (std::size_t i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 5);
        ASSERT_FLOAT_EQ(expected, result[1][i])
              << "The vector conditional operations is wrong.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

//TEST(DataTest, HalfLoadStoreTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto input_scalar =
//        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTDst);
//    input_scalar->setSize(3);
//    {
//      std::array<cl::half, 3> input;
//      input[0] = zisc::SingleFloat::fromFloat(1.0f);
//      input[1] = zisc::SingleFloat::fromFloat(2.0f);
//      input[2] = zisc::SingleFloat::fromFloat(4.0f);
//      input_scalar->write(input.data(), input.size(), 0, 0);
//    }
//    auto output_scalar =
//        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTSrc);
//    output_scalar->setSize(3);
//    auto input_vector2 =
//        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTDst);
//    input_vector2->setSize(4);
//    {
//      std::array<cl::half, 4> input;
//      input[0] = zisc::SingleFloat::fromFloat(1.0f);
//      input[1] = zisc::SingleFloat::fromFloat(2.0f);
//      input[2] = zisc::SingleFloat::fromFloat(4.0f);
//      input[3] = zisc::SingleFloat::fromFloat(8.0f);
//      input_vector2->write(input.data(), input.size(), 0, 0);
//    }
//    auto output_vector2 =
//        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTSrc);
//    output_vector2->setSize(4);
//    auto input_vector3 =
//        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTDst);
//    input_vector3->setSize(6);
//    {
//      std::array<cl::half, 6> input;
//      input[0] = zisc::SingleFloat::fromFloat(1.0f);
//      input[1] = zisc::SingleFloat::fromFloat(2.0f);
//      input[2] = zisc::SingleFloat::fromFloat(4.0f);
//      input[3] = zisc::SingleFloat::fromFloat(8.0f);
//      input[4] = zisc::SingleFloat::fromFloat(16.0f);
//      input[5] = zisc::SingleFloat::fromFloat(32.0f);
//      input_vector3->write(input.data(), input.size(), 0, 0);
//    }
//    auto output_vector3 =
//        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTSrc);
//    output_vector3->setSize(6);
//    auto input_vector4 =
//        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTDst);
//    input_vector4->setSize(8);
//    {
//      std::array<cl::half, 8> input;
//      input[0] = zisc::SingleFloat::fromFloat(1.0f);
//      input[1] = zisc::SingleFloat::fromFloat(2.0f);
//      input[2] = zisc::SingleFloat::fromFloat(4.0f);
//      input[3] = zisc::SingleFloat::fromFloat(8.0f);
//      input[4] = zisc::SingleFloat::fromFloat(16.0f);
//      input[5] = zisc::SingleFloat::fromFloat(32.0f);
//      input[6] = zisc::SingleFloat::fromFloat(64.0f);
//      input[7] = zisc::SingleFloat::fromFloat(128.0f);
//      input_vector4->write(input.data(), input.size(), 0, 0);
//    }
//    auto output_vector4 =
//        makeBuffer<cl::half>(device.get(), BufferUsage::kDeviceTSrc);
//    output_vector4->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), data, testHalfLoadStore, 1);
//    kernel->run(*input_scalar, *output_scalar, *input_vector2, *output_vector2,
//        *input_vector3, *output_vector3, *input_vector4, *output_vector4, {1}, 0);
//    device->waitForCompletion();
//
//    {
//      std::array<cl::half, 3> result;
//      output_scalar->read(result.data(), result.size(), 0, 0);
//      for (std::size_t i = 0; i < result.size(); ++i) {
//        const float expected = static_cast<float>(2u << i);
//        const auto r = zisc::SingleFloat{result[i]};
//        ASSERT_EQ(expected, r.toFloat()) << "Loading and storing half failed.";
//      }
//    }
//
////    {
////      std::array<cl::half, 4> result;
////      output_vector2->read(result.data(), result.size(), 0, 0);
////      for (std::size_t i = 0; i < result.size(); ++i) {
////        const float expected = static_cast<float>(2u << i);
////        const auto r = zisc::SingleFloat{result[i]};
////        ASSERT_EQ(expected, r.toFloat()) << "Loading and storing half2 failed.";
////      }
////    }
////
////    {
////      std::array<cl::half, 6> result;
////      output_vector3->read(result.data(), result.size(), 0, 0);
////      for (std::size_t i = 0; i < result.size(); ++i) {
////        const float expected = static_cast<float>(2u << i);
////        const auto r = zisc::SingleFloat{result[i]};
////        ASSERT_EQ(expected, r.toFloat()) << "Loading and storing half3 failed.";
////      }
////    }
//
//    {
//      std::array<cl::half, 8> result;
//      output_vector4->read(result.data(), result.size(), 0, 0);
//      for (std::size_t i = 0; i < result.size(); ++i) {
//        const float expected = static_cast<float>(2u << i);
//        const auto r = zisc::SingleFloat{result[i]};
//        ASSERT_EQ(expected, r.toFloat()) << "Loading and storing half4 failed.";
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//namespace {
//
//using Ray1 = zinvul::data::KernelGroup::Ray1;
//using Ray2 = zinvul::data::KernelGroup::Ray2;
//
//} // namespace
//
//TEST(DataTest, VariablePointerTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 16;
//
//    auto buffer0 = makeBuffer<::Ray1>(device.get(), BufferUsage::kDeviceTSrc);
//    buffer0->setSize(2 * resolution);
//
//    auto buffer1 = makeBuffer<::Ray2>(device.get(), BufferUsage::kDeviceTSrc);
//    buffer1->setSize(2 * resolution);
//
//    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    buffer2->setSize(1);
//    buffer2->write(&resolution, 1, 0, 0);
//
//    auto kernel = makeZinvulKernel(device.get(), data, testVariablePointer, 1);
//    kernel->run(*buffer0, *buffer1, *buffer2, {resolution}, 0);
//    device->waitForCompletion();
//
//    {
//      std::array<::Ray1, 2 * resolution> results;
//      buffer0->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < results.size(); ++i) {
//        const auto& result = results[i];
//        for (std::size_t j = 0; j < 4; ++j) {
//          ASSERT_FLOAT_EQ(zisc::cast<float>(j + 1), result.origin_[j])
//              << "The variable pointer for "
//              << (zisc::isOdd(j) ? "local" : "global") << " is wrong.";
//          ASSERT_FLOAT_EQ(1.0f, result.dir_[j])
//              << "The variable pointer for "
//              << (zisc::isOdd(j) ? "local" : "global") << " is wrong.";
//        }
//      }
//    }
//    {
//      std::array<::Ray2, 2 * resolution> results;
//      buffer1->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < results.size(); ++i) {
//        if (zisc::isOdd(i))
//          continue;
//        const auto& result = results[i];
//        for (std::size_t j = 0; j < 3; ++j) {
//          ASSERT_FLOAT_EQ(zisc::cast<float>(j + 1), result.origin_[j])
//              << "The variable pointer for "
//              << (zisc::isOdd(j) ? "local" : "global") << " is wrong.";
//          ASSERT_FLOAT_EQ(1.0f, result.dir_[j])
//              << "The variable pointer for "
//              << (zisc::isOdd(j) ? "local" : "global") << " is wrong.";
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}

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

//TEST(DataTest, CastToUint8bBufferTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 0b1u << (sizeof(uint8b) * 8);
//
//    auto buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    buffer->setSize(resolution / 4);
//
//    auto kernel = makeZinvulKernel(device.get(), data, testUint8bBuffer, 1);
//    kernel->run(*(buffer->treatAs<uint8b>()), {resolution}, 0);
//    device->waitForCompletion();
//
//    {
//      std::vector<float> results;
//      results.resize(resolution / 4, 0);
//      buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const uint8b expected = zisc::cast<uint8b>(i);
//        const uint8b* p = zisc::treatAs<const uint8b*>(results.data());
//        const uint8b result = p[i];
//        ASSERT_EQ(expected, result) << "Casting to uint8b buffer doesn't work";
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(DataTest, CastToUint16bBufferTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);
//
//    auto buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    buffer->setSize(resolution / 2);
//
//    auto kernel = makeZinvulKernel(device.get(), data, testUint16bBuffer, 1);
//    kernel->run(*(buffer->treatAs<uint16b>()), {resolution}, 0);
//    device->waitForCompletion();
//
//    {
//      std::vector<float> results;
//      results.resize(resolution / 2, 0);
//      buffer->read(results.data(), results.size(), 0, 0);
//      for (std::size_t i = 0; i < resolution; ++i) {
//        const uint16b expected = zisc::cast<uint16b>(i);
//        const uint16b* p = zisc::treatAs<const uint16b*>(results.data());
//        const uint16b result = p[i];
//        ASSERT_EQ(expected, result) << "Casting to uint16b buffer doesn't work";
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}

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

TEST(DataTest, NumericLimitsTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<int8b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(3);
    auto buffer2 = makeBuffer<uint8b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer2->setSize(3);
    auto buffer3 = makeBuffer<int16b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(3);
    auto buffer4 = makeBuffer<uint16b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer4->setSize(3);
    auto buffer5 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer5->setSize(3);
    auto buffer6 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer6->setSize(3);
    auto buffer7 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    buffer7->setSize(6);

    auto kernel = makeZinvulKernel(device.get(), data, testNumericLimits, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *buffer4, *buffer5, *buffer6, *buffer7, {1}, 0);
    device->waitForCompletion();

    {
      std::vector<int8b> results;
      results.resize(3);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(std::numeric_limits<int8b>::min(), results[i++])
        << "The NumericLimits<int8b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int8b>::lowest(), results[i++])
        << "The NumericLimits<int8b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int8b>::max(), results[i++])
        << "The NumericLimits<int8b> is wrong.";
    }

    {
      std::vector<uint8b> results;
      results.resize(3);
      buffer2->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(std::numeric_limits<uint8b>::min(), results[i++])
        << "The NumericLimits<uint8b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint8b>::lowest(), results[i++])
        << "The NumericLimits<uint8b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint8b>::max(), results[i++])
        << "The NumericLimits<uint8b> is wrong.";
    }

    {
      std::vector<int16b> results;
      results.resize(3);
      buffer3->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(std::numeric_limits<int16b>::min(), results[i++])
        << "The NumericLimits<int16b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int16b>::lowest(), results[i++])
        << "The NumericLimits<int16b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int16b>::max(), results[i++])
        << "The NumericLimits<int16b> is wrong.";
    }

    {
      std::vector<uint16b> results;
      results.resize(3);
      buffer4->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(std::numeric_limits<uint16b>::min(), results[i++])
        << "The NumericLimits<uint16b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint16b>::lowest(), results[i++])
        << "The NumericLimits<uint16b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint16b>::max(), results[i++])
        << "The NumericLimits<uint16b> is wrong.";
    }

    {
      std::vector<int32b> results;
      results.resize(3);
      buffer5->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(std::numeric_limits<int32b>::min(), results[i++])
        << "The NumericLimits<int32b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int32b>::lowest(), results[i++])
        << "The NumericLimits<int32b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int32b>::max(), results[i++])
        << "The NumericLimits<int32b> is wrong.";
    }

    {
      std::vector<uint32b> results;
      results.resize(3);
      buffer6->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(std::numeric_limits<uint32b>::min(), results[i++])
        << "The NumericLimits<uint32b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint32b>::lowest(), results[i++])
        << "The NumericLimits<uint32b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint32b>::max(), results[i++])
        << "The NumericLimits<uint32b> is wrong.";
    }

    {
      std::vector<float> results;
      results.resize(6);
      buffer7->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_FLOAT_EQ(std::numeric_limits<float>::min(), results[i++])
        << "The NumericLimits<float> is wrong.";
      ASSERT_FLOAT_EQ(std::numeric_limits<float>::lowest(), results[i++])
        << "The NumericLimits<float> is wrong.";
      ASSERT_FLOAT_EQ(std::numeric_limits<float>::max(), results[i++])
        << "The NumericLimits<float> is wrong.";
      ASSERT_FLOAT_EQ(std::numeric_limits<float>::epsilon(), results[i++])
        << "The NumericLimits<float> is wrong.";
      ASSERT_FLOAT_EQ(std::numeric_limits<float>::infinity(), results[i++])
        << "The NumericLimits<float> is wrong.";
      ASSERT_TRUE(std::isnan(results[i++]))
        << "The NumericLimits<float> is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }

}

#if !defined(Z_MAC)

TEST(DataTest, NumericLimits64Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<int64b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(3);
    auto buffer2 = makeBuffer<uint64b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer2->setSize(3);
    auto buffer3 = makeBuffer<double>(device.get(), BufferUsage::kDeviceTSrc);
    buffer3->setSize(6);

    auto kernel = makeZinvulKernel(device.get(), data, testNumericLimits64, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, {1}, 0);
    device->waitForCompletion();

    {
      std::vector<int64b> results;
      results.resize(3);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(std::numeric_limits<int64b>::min(), results[i++])
        << "The NumericLimits<int64b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int64b>::lowest(), results[i++])
        << "The NumericLimits<int64b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int64b>::max(), results[i++])
        << "The NumericLimits<int64b> is wrong.";
    }

    {
      std::vector<uint64b> results;
      results.resize(3);
      buffer2->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(std::numeric_limits<uint64b>::min(), results[i++])
        << "The NumericLimits<uint64b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint64b>::lowest(), results[i++])
        << "The NumericLimits<uint64b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint64b>::max(), results[i++])
        << "The NumericLimits<uint64b> is wrong.";
    }

    {
      std::vector<double> results;
      results.resize(6);
      buffer3->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_DOUBLE_EQ(std::numeric_limits<double>::min(), results[i++])
        << "The NumericLimits<double> is wrong.";
      ASSERT_DOUBLE_EQ(std::numeric_limits<double>::lowest(), results[i++])
        << "The NumericLimits<double> is wrong.";
      ASSERT_DOUBLE_EQ(std::numeric_limits<double>::max(), results[i++])
        << "The NumericLimits<double> is wrong.";
      ASSERT_DOUBLE_EQ(std::numeric_limits<double>::epsilon(), results[i++])
        << "The NumericLimits<double> is wrong.";
      ASSERT_DOUBLE_EQ(std::numeric_limits<double>::infinity(), results[i++])
        << "The NumericLimits<double> is wrong.";
      ASSERT_TRUE(std::isnan(results[i++]))
        << "The NumericLimits<double> is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

#endif // !Z_MAC

TEST(DataTest, ArrayTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(5);
    {
      std::array<uint32b, 5> src{{6, 7, 8, 9, 10}};
      buffer1->write(src.data(), src.size(), 0, 0);
    }
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer2->setSize(17);

    auto kernel = makeZinvulKernel(device.get(), data, testArray, 1);
    kernel->run(*buffer1, *buffer2, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The class 'Array' is wrong.";
    {
      std::vector<uint32b> results;
      results.resize(buffer2->size());
      buffer2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      for (std::size_t i = 0; i < 5; ++i) {
        const uint32b expected = i + 1;
        ASSERT_EQ(expected, results[index++]) << error_message;
      }
      for (std::size_t i = 0; i < 5; ++i) {
        const uint32b expected = 2;
        ASSERT_EQ(expected, results[index++]) << error_message;
      }
      for (std::size_t i = 0; i < 5; ++i) {
        const uint32b expected = i + 6;
        ASSERT_EQ(expected, results[index++]) << error_message;
      }
      ASSERT_EQ(0, results[index++]) << error_message;
      ASSERT_EQ(4, results[index++]) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
TEST(DataTest, Fnv1AHash32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(11);

    auto kernel = makeZinvulKernel(device.get(), data, testFnv1AHash32, 1);
    kernel->run(*buffer1, {1}, 0);
    device->waitForCompletion();

    {
      std::vector<uint32b> results;
      results.resize(11);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(0x811c9dc5, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0x811c9dc5, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0xe40c292c, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0xe40c292c, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0xe70c2de5, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0xe70c2de5, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0xbf9cf968, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0xbf9cf968, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0xe40c292c, results[i++])
        << "The Fnv1aHash32 is wrong.";
      ASSERT_EQ(0xe70c2de5, results[i++])
        << "The Fnv1aHash32 is wrong.";
      std::cout << "  hash32(123'456'789) = " << results[i++] << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

#if !defined(Z_MAC)

TEST(DataTest, Fnv1AHash64Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint64b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(11);

    auto kernel = makeZinvulKernel(device.get(), data, testFnv1AHash64, 1);
    kernel->run(*buffer1, {1}, 0);
    device->waitForCompletion();

    {
      std::vector<uint64b> results;
      results.resize(11);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::size_t i = 0;
      ASSERT_EQ(0xcbf29ce484222325, results[i++])
        << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0xcbf29ce484222325, results[i++])
       << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0xaf63dc4c8601ec8c, results[i++])
        << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0xaf63dc4c8601ec8c, results[i++])
        << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0xaf63df4c8601f1a5, results[i++])
        << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0xaf63df4c8601f1a5, results[i++])
        << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0x85944171f73967e8, results[i++])
        << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0x85944171f73967e8, results[i++])
        << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0xaf63dc4c8601ec8c, results[i++])
        << "The Fnv1aHash64 is wrong.";
      ASSERT_EQ(0xaf63df4c8601f1a5, results[i++])
        << "The Fnv1aHash64 is wrong.";
      std::cout << "  hash64(123'456'789) = " << results[i++] << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

#endif // !Z_MAC
