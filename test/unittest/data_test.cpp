/*!
  \file data_test.cpp
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
#include "zinvul/kernel_set/data.hpp"
#include "zinvul/zinvul_config.hpp"
// Test
#include "test.hpp"

TEST(DataTest, PointerTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 30;

    auto buffer0 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer0->setSize(5);
    {
      std::array<int32b, 5> inputs{{1, 2, 3, 4, 5}};
      buffer0->write(inputs.data(), inputs.size(), 0, 0);
    }
    auto buffer1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testPointer));
    kernel->run(*buffer0, *buffer1, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "Pointer operation is wrong.";
    {
      std::array<int32b, n> results;
      buffer1->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
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

TEST(DataTest, MemoryMapTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    using MemoryMapT = cl::data::test::MemoryMapTest;

    constexpr uint32b resolution = 100;
    const char* error_message = "Memory mapping operation is wrong.";

    auto buffer0 = makeStorageBuffer<MemoryMapT>(device.get(), BufferUsage::kDeviceToHost);
    buffer0->setSize(resolution);
    {
      auto memory = buffer0->mapMemory();
      ASSERT_TRUE(memory) << error_message;
      {
        MemoryMapT data = {0, 0.0f};
        for (auto ite = memory.begin(); ite < memory.end(); ++ite) {
          *ite = data;
          data.u_++;
        }
      }
      for (uint32b i = 0; i < memory.size(); ++i) {
        auto data = memory.get(i);
        data.f_ = zisc::cast<float>(i);
        memory.set(i, data);
      }
      {
        auto data = memory.data();
        for (uint32b i = 0; i < memory.size(); ++i) {
          const uint32b uexpected = zisc::cast<uint32b>(i);
          const float fexpected = zisc::cast<float>(i);
          const auto result = data[i];
          ASSERT_EQ(uexpected, result.u_) << error_message;
          ASSERT_FLOAT_EQ(fexpected, result.f_) << error_message;
        }
      }
    }

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    {
      res_buff->write(&resolution, 1, 0, 0);
    }


    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testMemoryMap));
    kernel->run(*buffer0, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      auto memory = buffer0->mapMemory();

      for (uint32b i = 0; i < memory.size(); ++i) {
        const uint32b uexpected = 100u - zisc::cast<uint32b>(i);
        const float fexpected = 100.0f - zisc::cast<float>(i);
        const auto result = memory[i];
        ASSERT_EQ(uexpected, result.u_) << error_message;
        ASSERT_FLOAT_EQ(fexpected, result.f_) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }

}

TEST(DataTest, LocalInputTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 100;

    auto buffer0 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer0->setSize(resolution);
    {
      std::array<uint32b, resolution> inputs;
      for (uint32b i = 0; i < resolution; ++i)
        inputs[i] = resolution - (i + 1);
      buffer0->write(inputs.data(), inputs.size(), 0, 0);
    }
    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(resolution);
    {
      std::array<uint32b, resolution> inputs;
      for (uint32b i = 0; i < resolution; ++i)
        inputs[i] = i;
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    {
      res_buff->write(&resolution, 1, 0, 0);
    }

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testLocalInput));
    kernel->run(*buffer0, *buffer1, *buffer2, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "Local input kernel is wrong.";
    {
      std::array<uint32b, resolution> results;
      buffer2->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < resolution; ++i)
      {
        const uint32b expected = resolution - 1;
        const uint32b result = results[i];
        ASSERT_EQ(expected, result) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}


TEST(DataTest, GlobalInstanceTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    using GlobalOption = cl::data::test::OptionTest;
    GlobalOption option;
    option.init();
    option.setValue1(5u);
    option.setValue3(15u);

    auto buffer0 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer0->setSize(2);

    auto buffer1 = makeStorageBuffer<GlobalOption>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(1);
    buffer1->write(&option, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testGlobalInstance));
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    using GlobalOption = cl::data::test::OptionTest;
    GlobalOption option;
    option.init();
    option.setValue1(5u);
    option.setValue3(15u);

    auto buffer0 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer0->setSize(3);

    auto buffer1 = makeStorageBuffer<GlobalOption>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(1);
    buffer1->write(&option, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testLocalInstance));
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

TEST(DataTest, ConstantArgTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 100;

    auto buffer0 = makeUniformBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    buffer0->setSize(resolution);
    {
      std::array<cl::uint4, resolution> inputs;
      for (uint32b i = 0; i < resolution; ++i) {
        const uint32b u = zisc::cast<uint32b>(i);
        const cl::uint4 v{u, u + 1u, u + 2u, u + 3u};
        inputs[i] = v;
      }
      buffer0->write(inputs.data(), resolution, 0, 0);
    }

    auto buffer1 = makeUniformBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(resolution);
    {
      std::array<cl::float4, resolution> inputs;
      for (uint32b i = 0; i < resolution; ++i) {
        const float u = zisc::cast<float>(i);
        const cl::float4 v{u, u + 1.0f, u + 2.0f, u + 3.0f};
        inputs[i] = v;
      }
      buffer1->write(inputs.data(), resolution, 0, 0);
    }

    auto buffer2 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(resolution);

    auto buffer3 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testConstantArg));
    kernel->run(*buffer0, *buffer1, *buffer2, *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "Constant arg test failed.";
    {
      std::array<cl::uint4, resolution> results;
      buffer2->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < results.size(); ++i) {
        const auto result = results[i];
        for (uint32b j = 0; j < 4; ++j) {
          const uint32b expected = zisc::cast<uint32b>(i + j);
          ASSERT_EQ(expected, result[j]) << error_message;
        }
      }
    }
    {
      std::array<cl::float4, resolution> results;
      buffer3->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < results.size(); ++i) {
        const auto result = results[i];
        for (uint32b j = 0; j < 4; ++j) {
          const float expected = zisc::cast<float>(i + j);
          ASSERT_FLOAT_EQ(expected, result[j]) << error_message;
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
TEST(DataTest, CopyBufferTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 16;
    constexpr uint32b initializer = 100;

    std::array<uint32b, n> init;
    for (uint32b i = 0; i < init.size(); ++i)
      init[i] = initializer;

    auto buffer0 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kHostOnly);
    buffer0->setSize(n);
    buffer0->write(init.data(), init.size(), 0, 0);

    std::array<uint32b, 12> input;
    for (uint32b i = 0; i < input.size(); ++i)
      input[i] = zisc::cast<uint32b>(i + 1);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kHostOnly);
    buffer1->setSize(n);
    buffer1->write(input.data(), input.size(), 2, 0);

    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(n);
    zinvul::copy(*buffer0, buffer2.get(), n, 0, 0, 0);
    zinvul::copy(*buffer1, buffer2.get(), 12, 2, 2, 0);
    device->waitForCompletion(QueueType::kTransfer);

    auto buffer3 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, copyBufferTest));
    kernel->run(*buffer2, *buffer3, {1}, 0);
    device->waitForCompletion();

    auto buffer4 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceToHost);
    buffer4->setSize(n);
    zinvul::copy(*buffer3, buffer4.get(), n, 0, 0, 0);
    device->waitForCompletion(QueueType::kTransfer);

    {
      std::array<uint32b, 2> result;
      buffer4->read(result.data(), result.size(), 0, 0);
      EXPECT_EQ(initializer, result[0]) << "The copyBuffer func is wrong.";
      EXPECT_EQ(initializer, result[1]) << "The copyBuffer func is wrong.";
    }
    {
      std::array<uint32b, 14> result;
      buffer4->read(result.data(), result.size(), 2, 0);
      for (uint32b i = 0; i < 12; ++i)
        EXPECT_EQ(i + 1, result[i]) << "The copyBuffer func is wrong.";
      EXPECT_EQ(initializer, result[12]) << "The copyBuffer func is wrong.";
      EXPECT_EQ(initializer, result[13]) << "The copyBuffer func is wrong.";
    }

    constexpr uint32b resolution = 500;

    auto buffer5 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kHostOnly);
    buffer5->setSize(resolution);
    {
      std::array<int32b, resolution> table;
      for (uint32b i = 0; i < table.size(); ++i)
        table[i] = zisc::cast<int>(i);
      buffer5->write(table.data(), table.size(), 0, 0);
    }
    auto buffer6 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer6->setSize(resolution);
    zinvul::copy(*buffer5, buffer6.get(), resolution, 0, 0, 0);
    device->waitForCompletion(QueueType::kTransfer);
    auto buffer7 = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer7->setSize(1);
    buffer7->write(&resolution, 1, 0, 0);

    auto kernel2 = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, multiplyBufferTest));
    kernel2->run(*buffer6, *buffer7, {resolution}, 0);
    device->waitForCompletion();

    zinvul::copy(*buffer6, buffer5.get(), resolution, 0, 0, 0);
    device->waitForCompletion(QueueType::kTransfer);
    {
      std::array<int32b, resolution> table;
      buffer5->read(table.data(), resolution, 0, 0);
      for (uint32b i = 0; i < table.size(); ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto iinputs = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    iinputs->setSize(4);
    {
      std::array<int32b, 4> inputs{{1, 2, 3, 4}};
      iinputs->write(inputs.data(), inputs.size(), 0, 0);
    }
    auto uinputs = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    uinputs->setSize(4);
    {
      std::array<uint32b, 4> inputs{{1, 2, 3, 4}};
      uinputs->write(inputs.data(), inputs.size(), 0, 0);
    }
    auto finputs = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    finputs->setSize(4);
    {
      std::array<float, 4> inputs{{1.0f, 2.0f, 3.0f, 4.0f}};
      finputs->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto ibuffer1 = makeStorageBuffer<int8b>(device.get(), BufferUsage::kDeviceOnly);
    ibuffer1->setSize(8);
    auto ibuffer2 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    ibuffer2->setSize(8);
    auto ibuffer3 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceOnly);
    ibuffer3->setSize(2);

    auto ubuffer1 = makeStorageBuffer<uint8b>(device.get(), BufferUsage::kDeviceOnly);
    ubuffer1->setSize(4);
    auto ubuffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    ubuffer2->setSize(4);
    auto ubuffer3 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    ubuffer3->setSize(2);

    auto fbuffer1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    fbuffer1->setSize(8);
    auto fbuffer2 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    fbuffer2->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testTypeCast));
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
      for (uint32b i = 0; i < 4; ++i) {
        const int8b expected = zisc::cast<int8b>(i + 1);
        const int8b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'cast<int8b>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
        const int8b expected = -zisc::cast<int8b>(i + 1);
        const int8b result = results[i + 4];
        ASSERT_EQ(expected, result) << "The func 'cast<int8b>' failed.";
      }
    }
    {
      std::vector<int32b> results;
      results.resize(ibuffer2->size(), 0);
      ibuffer2->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const int32b expected = zisc::cast<int32b>(i + 1);
        const int32b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'cast<int32b>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
        const int32b expected = -zisc::cast<int32b>(i + 1);
        const int32b result = results[i + 4];
        ASSERT_EQ(expected, result) << "The func 'cast<int32b>' failed.";
      }
    }
    {
      std::vector<cl::int4> results;
      results.resize(ibuffer3->size());
      ibuffer3->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const int32b expected = zisc::cast<int32b>(i + 1);
        const int32b result = results[0][i];
        ASSERT_EQ(expected, result) << "The func 'cast<int4>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
        const int32b expected = -zisc::cast<int32b>(i + 1);
        const int32b result = results[1][i];
        ASSERT_EQ(expected, result) << "The func 'cast<int4>' failed.";
      }
    }

    {
      std::vector<uint8b> results;
      results.resize(ubuffer1->size(), 0);
      ubuffer1->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const uint8b expected = zisc::cast<uint8b>(i + 1);
        const uint8b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'cast<uint8b>' failed.";
      }
    }
    {
      std::vector<uint32b> results;
      results.resize(ubuffer2->size(), 0);
      ubuffer2->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const uint32b expected = zisc::cast<uint32b>(i + 1);
        const uint32b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'cast<uint32b>' failed.";
      }
    }
    {
      std::vector<cl::uint4> results;
      results.resize(ubuffer3->size());
      ubuffer3->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const uint32b expected = zisc::cast<uint32b>(i + 1);
        const uint32b result = results[0][i];
        ASSERT_EQ(expected, result) << "The func 'cast<uint4>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
        const uint32b expected = zisc::cast<uint32b>(i + 1);
        const uint32b result = results[1][i];
        ASSERT_EQ(expected, result) << "The func 'cast<uint4>' failed.";
      }
    }

    {
      std::vector<float> results;
      results.resize(fbuffer1->size(), 0);
      fbuffer1->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'cast<float>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = -zisc::cast<float>(i + 1);
        const float result = results[i + 4];
        EXPECT_FLOAT_EQ(expected, result) << "The func 'cast<float>' failed.";
      }
    }
    {
      std::vector<cl::float4> results;
      results.resize(fbuffer2->size());
      fbuffer2->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[0][i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'cast<float4>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto uinputs1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    uinputs1->setSize(4);
    {
      std::array<uint32b, 4> inputs;
      for (uint32b i = 0; i < 4; ++i) {
        const float v = zisc::cast<float>(i + 1);
        inputs[i] = *zisc::treatAs<const uint32b*>(&v);
      }
      uinputs1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto finputs1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    finputs1->setSize(4);
    {
      std::array<float, 4> inputs{{1.0f, 2.0f, 3.0f, 4.0f}};
      finputs1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto ubuffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    ubuffer1->setSize(8);
    auto ubuffer2 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    ubuffer2->setSize(2);

    auto fbuffer1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    fbuffer1->setSize(8);
    auto fbuffer2 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    fbuffer2->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testTypeReinterpreting));
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
      for (uint32b i = 0; i < 4; ++i) {
        const float v = zisc::cast<float>(i + 1);
        const uint32b expected = *zisc::treatAs<const uint32b*>(&v);
        const uint32b result = results[i];
        ASSERT_EQ(expected, result) << "The func 'treatAs<uint32b>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
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
      for (uint32b i = 0; i < 4; ++i) {
        const float v = zisc::cast<float>(i + 1);
        const uint32b expected = *zisc::treatAs<const uint32b*>(&v);
        const uint32b result = results[0][i];
        ASSERT_EQ(expected, result) << "The func 'treatAs<uint4>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
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
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'treatAs<float>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[i + 4];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'treatAs<float>' failed.";
      }
    }
    {
      std::vector<cl::float4> results;
      results.resize(fbuffer2->size(), 0);
      fbuffer2->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[0][i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'treatAs<float4>' failed.";
      }
      for (uint32b i = 0; i < 4; ++i) {
        const float expected = zisc::cast<float>(i + 1);
        const float result = results[1][i];
        ASSERT_FLOAT_EQ(expected, result) << "The func 'treatAs<float4>' failed.";
      }
    }
  }
}

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
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 16;
//
//    auto buffer0 = makeStorageBuffer<::Ray1>(device.get(), BufferUsage::kDeviceOnly);
//    buffer0->setSize(2 * resolution);
//
//    auto buffer1 = makeStorageBuffer<::Ray2>(device.get(), BufferUsage::kDeviceOnly);
//    buffer1->setSize(2 * resolution);
//
//    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
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
//      for (uint32b i = 0; i < results.size(); ++i) {
//        const auto& result = results[i];
//        for (uint32b j = 0; j < 4; ++j) {
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
//      for (uint32b i = 0; i < results.size(); ++i) {
//        if (zisc::isOdd(i))
//          continue;
//        const auto& result = results[i];
//        for (uint32b j = 0; j < 3; ++j) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint8b) * 8);

    auto buffer = makeStorageBuffer<int8b>(device.get(), BufferUsage::kDeviceOnly);
    buffer->setSize(resolution);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testInt8bBuffer));
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<int8b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < resolution; ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint8b) * 8);

    auto buffer = makeStorageBuffer<uint8b>(device.get(), BufferUsage::kDeviceOnly);
    buffer->setSize(resolution);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testUint8bBuffer));
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<uint8b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < resolution; ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);

    auto buffer = makeStorageBuffer<int16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer->setSize(resolution);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testInt16bBuffer));
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<int16b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < resolution; ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);

    auto buffer = makeStorageBuffer<uint16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer->setSize(resolution);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testUint16bBuffer));
    kernel->run(*buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<uint16b> results;
      results.resize(resolution, 0);
      buffer->read(results.data(), results.size(), 0, 0);
      for (uint32b i = 0; i < resolution; ++i) {
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
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 0b1u << (sizeof(uint8b) * 8);
//
//    auto buffer = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
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
//      for (uint32b i = 0; i < resolution; ++i) {
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
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 0b1u << (sizeof(uint16b) * 8);
//
//    auto buffer = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
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
//      for (uint32b i = 0; i < resolution; ++i) {
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

TEST(DataTest, CastUint8bToFloat)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 100000;

    auto buffer1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(resolution);

    auto buffer2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(resolution);
    {
      std::vector<float> inputs;
      inputs.resize(resolution);
      for (uint32b i = 0; i < resolution; ++i)
        inputs[i] = zisc::cast<float>(i);
      buffer2->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testCastUint8bToFloat));
    kernel->run(*(buffer1->treatAs<uint8b>()), *(buffer2->treatAs<uint8b>()), *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> wresults;
      wresults.resize(resolution);
      buffer1->read(wresults.data(), wresults.size(), 0, 0);
      std::vector<float> rresults;
      rresults.resize(resolution);
      buffer3->read(rresults.data(), rresults.size(), 0, 0);
      for (uint32b i = 0; i < wresults.size(); ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 100000;

    auto buffer1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(resolution);

    auto buffer2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(resolution);
    {
      std::vector<float> inputs;
      inputs.resize(resolution);
      for (uint32b i = 0; i < resolution; ++i)
        inputs[i] = zisc::cast<float>(i);
      buffer2->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testCastUint16bToFloat));
    kernel->run(*(buffer1->treatAs<uint16b>()), *(buffer2->treatAs<uint16b>()), *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> wresults;
      wresults.resize(resolution);
      buffer1->read(wresults.data(), wresults.size(), 0, 0);
      std::vector<float> rresults;
      rresults.resize(resolution);
      buffer3->read(rresults.data(), rresults.size(), 0, 0);
      for (uint32b i = 0; i < wresults.size(); ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 100000;

    auto buffer1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(resolution);

    auto buffer2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(resolution);
    {
      std::vector<float> inputs;
      inputs.resize(resolution);
      for (uint32b i = 0; i < resolution; ++i)
        inputs[i] = zisc::cast<float>(i);
      buffer2->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testCastUint32bToFloat));
    kernel->run(*(buffer1->treatAs<uint32b>()), *(buffer2->treatAs<uint32b>()), *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> wresults;
      wresults.resize(resolution);
      buffer1->read(wresults.data(), wresults.size(), 0, 0);
      std::vector<float> rresults;
      rresults.resize(resolution);
      buffer3->read(rresults.data(), rresults.size(), 0, 0);
      for (uint32b i = 0; i < wresults.size(); ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer0 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer0->setSize(3 * 7);

    auto buffer1 = makeStorageBuffer<int8b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(3);
    auto buffer2 = makeStorageBuffer<uint8b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(3);
    auto buffer3 = makeStorageBuffer<int16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(3);
    auto buffer4 = makeStorageBuffer<uint16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer4->setSize(3);
    auto buffer5 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer5->setSize(3);
    auto buffer6 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer6->setSize(3);
    auto buffer7 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer7->setSize(6);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testNumericLimits));
    kernel->run(*buffer0, *buffer1, *buffer2, *buffer3, *buffer4, *buffer5, *buffer6, *buffer7, {1}, 0);
    device->waitForCompletion();

    // Digits
    {
      std::vector<int32b> results;
      results.resize(buffer0->size());
      buffer0->read(results.data(), results.size(), 0, 0);
      uint32b i = 0;
      ASSERT_EQ(std::numeric_limits<int8b>::digits, results[i++])
        << "The NumericLimits<int8b> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<int8b>::digits10, results[i++])
//        << "The NumericLimits<int8b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int8b>::max_digits10, results[i++])
        << "The NumericLimits<int8b> is wrong.";

      ASSERT_EQ(std::numeric_limits<uint8b>::digits, results[i++])
        << "The NumericLimits<uint8b> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<uint8b>::digits10, results[i++])
//        << "The NumericLimits<uint8b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint8b>::max_digits10, results[i++])
        << "The NumericLimits<uint8b> is wrong.";

      ASSERT_EQ(std::numeric_limits<int16b>::digits, results[i++])
        << "The NumericLimits<int16b> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<int16b>::digits10, results[i++])
//        << "The NumericLimits<int16b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int16b>::max_digits10, results[i++])
        << "The NumericLimits<int16b> is wrong.";

      ASSERT_EQ(std::numeric_limits<uint16b>::digits, results[i++])
        << "The NumericLimits<uint16b> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<uint16b>::digits10, results[i++])
//        << "The NumericLimits<uint16b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint16b>::max_digits10, results[i++])
        << "The NumericLimits<uint16b> is wrong.";

      ASSERT_EQ(std::numeric_limits<int32b>::digits, results[i++])
        << "The NumericLimits<int32b> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<int32b>::digits10, results[i++])
//        << "The NumericLimits<int32b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int32b>::max_digits10, results[i++])
        << "The NumericLimits<int32b> is wrong.";

      ASSERT_EQ(std::numeric_limits<uint32b>::digits, results[i++])
        << "The NumericLimits<uint32b> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<uint32b>::digits10, results[i++])
//        << "The NumericLimits<uint32b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint32b>::max_digits10, results[i++])
        << "The NumericLimits<uint32b> is wrong.";

      ASSERT_EQ(std::numeric_limits<float>::digits, results[i++])
        << "The NumericLimits<float> is wrong.";
      ASSERT_EQ(std::numeric_limits<float>::digits10, results[i++])
        << "The NumericLimits<float> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<float>::max_digits10, results[i++])
//        << "The NumericLimits<float> is wrong.";
    }

    {
      std::vector<int8b> results;
      results.resize(3);
      buffer1->read(results.data(), results.size(), 0, 0);
      uint32b i = 0;
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
      uint32b i = 0;
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
      uint32b i = 0;
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
      uint32b i = 0;
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
      uint32b i = 0;
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
      uint32b i = 0;
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
      uint32b i = 0;
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer0 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer0->setSize(3 * 7);
    auto buffer1 = makeStorageBuffer<int64b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(3);
    auto buffer2 = makeStorageBuffer<uint64b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(3);
    auto buffer3 = makeStorageBuffer<double>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(6);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testNumericLimits64));
    kernel->run(*buffer0, *buffer1, *buffer2, *buffer3, {1}, 0);
    device->waitForCompletion();

    // Digits
    {
      std::vector<int32b> results;
      results.resize(buffer0->size());
      buffer0->read(results.data(), results.size(), 0, 0);
      uint32b i = 0;
      ASSERT_EQ(std::numeric_limits<int64b>::digits, results[i++])
        << "The NumericLimits<int64b> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<int64b>::digits10, results[i++])
//        << "The NumericLimits<int64b> is wrong.";
      ASSERT_EQ(std::numeric_limits<int64b>::max_digits10, results[i++])
        << "The NumericLimits<int64b> is wrong.";

      ASSERT_EQ(std::numeric_limits<uint64b>::digits, results[i++])
        << "The NumericLimits<uint64b> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<uint64b>::digits10, results[i++])
//        << "The NumericLimits<uint64b> is wrong.";
      ASSERT_EQ(std::numeric_limits<uint64b>::max_digits10, results[i++])
        << "The NumericLimits<uint64b> is wrong.";

      ASSERT_EQ(std::numeric_limits<double>::digits, results[i++])
        << "The NumericLimits<double> is wrong.";
      ASSERT_EQ(std::numeric_limits<double>::digits10, results[i++])
        << "The NumericLimits<double> is wrong.";
      ++i;
//      ASSERT_EQ(std::numeric_limits<double>::max_digits10, results[i++])
//        << "The NumericLimits<double> is wrong.";
    }
    {
      std::vector<int64b> results;
      results.resize(3);
      buffer1->read(results.data(), results.size(), 0, 0);
      uint32b i = 0;
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
      uint32b i = 0;
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
      uint32b i = 0;
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(5);
    {
      std::array<uint32b, 5> src{{6, 7, 8, 9, 10}};
      buffer1->write(src.data(), src.size(), 0, 0);
    }
    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(17);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testArray));
    kernel->run(*buffer1, *buffer2, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The class 'Array' is wrong.";
    {
      std::vector<uint32b> results;
      results.resize(buffer2->size());
      buffer2->read(results.data(), results.size(), 0, 0);
      uint32b index = 0;
      for (uint32b i = 0; i < 5; ++i) {
        const uint32b expected = zisc::cast<uint32b>(i + 1);
        ASSERT_EQ(expected, results[index++]) << error_message;
      }
      for (uint32b i = 0; i < 5; ++i) {
        const uint32b expected = 2;
        ASSERT_EQ(expected, results[index++]) << error_message;
      }
      for (uint32b i = 0; i < 5; ++i) {
        const uint32b expected = zisc::cast<uint32b>(i + 6);
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(11);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testFnv1AHash32));
    kernel->run(*buffer1, {1}, 0);
    device->waitForCompletion();

    {
      std::vector<uint32b> results;
      results.resize(11);
      buffer1->read(results.data(), results.size(), 0, 0);
      uint32b i = 0;
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint64b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(11);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(data, testFnv1AHash64));
    kernel->run(*buffer1, {1}, 0);
    device->waitForCompletion();

    {
      std::vector<uint64b> results;
      results.resize(11);
      buffer1->read(results.data(), results.size(), 0, 0);
      uint32b i = 0;
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
