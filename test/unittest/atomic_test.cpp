/*!
  \file atomic_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <cstddef>
#include <iostream>
#include <limits>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/atomic.hpp"
// Test
#include "test.hpp"

TEST(AtomicTest, LocalMemoryFenceTest)
{
  using namespace zinvul;

  constexpr uint resolution = 1024;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto table_buf = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buf->setSize(resolution);
    auto results_buf = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    results_buf->setSize(resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testLocalMemoryFence));
    kernel->run(*table_buf, *results_buf, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "Local memory fence isn't working.";
    const uint32b local_size = device->subgroupSize();
    {
      std::vector<uint32b> table;
      table.resize(resolution, 0);
      table_buf->read(table.data(), table.size(), 0, 0);

      for (std::size_t i = 0; i < resolution; ++i) {
        const uint32b expected = (i % local_size) + 1;
        ASSERT_EQ(expected, table[i]) << error_message;
      }
    }
    {
      std::vector<uint32b> results;
      results.resize(resolution, 0);
      results_buf->read(results.data(), results.size(), 0, 0);

      uint32b expected = 0;
      for (uint32b i = 0; i < local_size; ++i) {
        expected += i + 1;
      }
      for (std::size_t i = 0; i < resolution; ++i) {
        ASSERT_EQ(expected, results[i]) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicAddGlobalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicAddGlobalPositive));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_add' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result = 0;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicAddLocalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicAddLocalPositive));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_add' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result = 0;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicAddGlobalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicAddGlobalNegative));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_add' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicAddLocalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicAddLocalNegative));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_add' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicAddGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicAddGlobalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_add' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicAddLocalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicAddLocalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_add' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicSubGlobalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicSubGlobalPositive));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_sub' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicSubLocalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicSubLocalPositive));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_sub' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicSubGlobalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicSubGlobalNegative));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_sub' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicSubLocalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicSubLocalNegative));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_sub' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicSubGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicSubGlobalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_sub' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicSubLocalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicSubLocalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_sub' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicXchgGlobalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicXchgGlobalPositive));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_xchg' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      int32b last_one = static_cast<int32b>(resolution);
      for (std::size_t i = 0; i < table.size(); ++i) {
        if (!table[i]) {
          if (last_one == static_cast<int32b>(resolution))
            last_one = zisc::cast<int32b>(i);
          else
            FAIL();
        }
      }

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_EQ(last_one, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicXchgGlobalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicXchgGlobalNegative));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_xchg' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      int32b last_one = static_cast<int32b>(resolution);
      for (std::size_t i = 0; i < table.size(); ++i) {
        if (!table[i]) {
          if (last_one == static_cast<int32b>(resolution))
            last_one = zisc::cast<int32b>(i);
          else
            FAIL();
        }
      }
      last_one = -last_one;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_EQ(last_one, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicXchgGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicXchgGlobalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_xchg' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      uint32b last_one = resolution;
      for (std::size_t i = 0; i < table.size(); ++i) {
        if (!table[i]) {
          if (last_one == resolution)
            last_one = zisc::cast<uint32b>(i);
          else
            FAIL();
        }
      }

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_EQ(last_one, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicIncGlobalTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicIncGlobal));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_inc' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicIncLocalTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicIncLocal));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_inc' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicIncGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicIncGlobalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_inc' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicIncLocalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicIncLocalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_inc' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicDecGlobalTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicDecGlobal));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_dec' for gloabl storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicDecLocalTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicDecLocal));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_dec' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicDecGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicDecGlobalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_dec' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicDecLocalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicDecLocalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_dec' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicCmpxchgGlobalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicCmpxchgGlobalPositive));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_cmpxchg' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicCmpxchgLocalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicCmpxchgLocalPositive));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_cmpxchg' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicCmpxchgGlobalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicCmpxchgGlobalNegative));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_cmpxchg' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicCmpxchgLocalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicCmpxchgLocalNegative));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_cmpxchg' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message << ": [" << i << "]";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicCmpxchgGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicCmpxchgGlobalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_cmpxchg' for global storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicCmpxchgLocalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicCmpxchgLocalUint));
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_cmpxchg' for local storage is wrong.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMinGlobalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMinGlobalPositive));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_min' for global storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = 0;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMinLocalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMinLocalPositive));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_min' for local storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = 0;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMinGlobalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMinGlobalNegative));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_min' for global storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution - 1);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMinLocalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMinLocalNegative));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_min' for local storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution - 1);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMinGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMinGlobalUint));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_min' for global storage is wrong.";
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = 0;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMinLocalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMinLocalUint));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_min' for local storage is wrong.";
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = 0;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMaxGlobalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMaxGlobalPositive));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_max' for global storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution - 1);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMaxLocalPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMaxLocalPositive));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_max' for local storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution - 1);
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMaxGlobalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::min();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMaxGlobalNegative));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_max' for global storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = 0;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMaxLocalNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::min();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMaxLocalNegative));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_max' for local storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = 0;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMaxGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMaxGlobalUint));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_max' for global storage is wrong.";
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution - 1;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicMaxLocalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicMaxLocalUint));
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_max' for local storage is wrong.";
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution - 1;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicAndGlobalTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicAndGlobal));
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_and' for global storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicAndGlobalUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicAndGlobalUint));
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_and' for global storage is wrong.";
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicOrGlobalTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicOrGlobal));
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_and' for global storage is wrong.";
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -1;
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicOrUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicOrGlobalUint));
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'atomic_and' for global storage is wrong.";
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = std::numeric_limits<uint32b>::max();
      EXPECT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AtomicTest, AtomicFloatIncGlobalTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(1);
    {
      const float init = 0.0f;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(atomic, testAtomicFloatIncGlobal));
    kernel->run(*(result_buff->treatAs<uint32b>()), *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "Atomic float increment failed.";
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << error_message;

      float result;
      result_buff->read(&result, 1, 0, 0);
      const float expected = zisc::cast<float>(resolution);
      EXPECT_FLOAT_EQ(expected, result) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
