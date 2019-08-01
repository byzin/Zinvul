/*!
  \file built_in_func_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <iostream>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/floating_point.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/built_in_func.hpp"
// Test
#include "test.hpp"

TEST(BuiltInFuncTest, WorkItem1DTest)
{
  using namespace zinvul;

  constexpr uint resolution = 1000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(9);
    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(resolution);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(resolution);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testWorkItem1D));

    kernel->run(*buffer1, *buffer2, *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The work-item functions are wrong.";
    {
      std::cout << "  resolutions: " <<  resolution << std::endl;

      std::vector<uint32b> results;
      results.resize(9, 0u);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::cout << "  group size[0]: " << results[0] << std::endl;
      std::cout << "  group size[1]: " << results[1] << std::endl;
      std::cout << "  group size[2]: " << results[2] << std::endl;
      std::cout << "  local size[0]: " << results[3] << std::endl;
      std::cout << "  local size[1]: " << results[4] << std::endl;
      std::cout << "  local size[2]: " << results[5] << std::endl;
      std::cout << "  global size[0]: " << results[6] << std::endl;
      std::cout << "  global size[1]: " << results[7] << std::endl;
      std::cout << "  global size[2]: " << results[8] << std::endl;

      const std::size_t num_of_works = results[6] * results[7] * results[8];
      std::cout << "  num of works: " << num_of_works << std::endl;
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]) << error_message;
    }

    {
      uint32b c = 0;
      buffer3->read(&c, 1, 0, 0);
      std::cout << "  out of range: " << c << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, WorkItem2DTest)
{
  using namespace zinvul;

  const cl::uint2 resolution{40, 25};
  const std::size_t res = resolution.x * resolution.y;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(9);
    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(res);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(res);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<cl::uint2>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<2>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testWorkItem2D));
    kernel->run(*buffer1, *buffer2, *buffer3, *res_buff, {resolution.x, resolution.y}, 0);
    device->waitForCompletion();

    const char* error_message = "The work-item functions are wrong.";
    {
      std::cout << "  resolutions: " <<  res << std::endl;

      std::vector<uint32b> results;
      results.resize(9, 0u);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::cout << "  group size[0]: " << results[0] << std::endl;
      std::cout << "  group size[1]: " << results[1] << std::endl;
      std::cout << "  group size[2]: " << results[2] << std::endl;
      std::cout << "  local size[0]: " << results[3] << std::endl;
      std::cout << "  local size[1]: " << results[4] << std::endl;
      std::cout << "  local size[2]: " << results[5] << std::endl;
      std::cout << "  global size[0]: " << results[6] << std::endl;
      std::cout << "  global size[1]: " << results[7] << std::endl;
      std::cout << "  global size[2]: " << results[8] << std::endl;

      const std::size_t num_of_works = results[6] * results[7] * results[8];
      std::cout << "  num of works: " << num_of_works << std::endl;
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]) << error_message;
    }

    {
      uint32b c = 0;
      buffer3->read(&c, 1, 0, 0);
      std::cout << "  out of range: " << c << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, WorkItem3DTest)
{
  using namespace zinvul;

  const cl::uint3 resolution{10, 10, 10};
  const std::size_t res = resolution.x * resolution.y * resolution.z;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(9);
    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(res);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(res);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<cl::uint3>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<3>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testWorkItem3D));
    kernel->run(*buffer1, *buffer2, *buffer3, *res_buff, {resolution.x, resolution.y, resolution.z}, 0);
    device->waitForCompletion();

    const char* error_message = "The work-item functions are wrong.";
    {
      std::cout << "  resolutions: " <<  res << std::endl;

      std::vector<uint32b> results;
      results.resize(9, 0u);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::cout << "  group size[0]: " << results[0] << std::endl;
      std::cout << "  group size[1]: " << results[1] << std::endl;
      std::cout << "  group size[2]: " << results[2] << std::endl;
      std::cout << "  local size[0]: " << results[3] << std::endl;
      std::cout << "  local size[1]: " << results[4] << std::endl;
      std::cout << "  local size[2]: " << results[5] << std::endl;
      std::cout << "  global size[0]: " << results[6] << std::endl;
      std::cout << "  global size[1]: " << results[7] << std::endl;
      std::cout << "  global size[2]: " << results[8] << std::endl;

      const std::size_t num_of_works = results[6] * results[7] * results[8];
      std::cout << "  num of works: " << num_of_works << std::endl;
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]) << error_message;
    }

    {
      uint32b c = 0;
      buffer3->read(&c, 1, 0, 0);
      std::cout << "  out of range: " << c << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, LocalMemoryFenceTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testLocalMemoryFence));
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

TEST(BuiltInFuncTest, AtomicAddGlobalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicAddGlobalPositive));
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

TEST(BuiltInFuncTest, AtomicAddLocalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicAddLocalPositive));
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

TEST(BuiltInFuncTest, AtomicAddGlobalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicAddGlobalNegative));
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

TEST(BuiltInFuncTest, AtomicAddLocalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicAddLocalNegative));
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

TEST(BuiltInFuncTest, AtomicAddGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicAddGlobalUint));
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

TEST(BuiltInFuncTest, AtomicAddLocalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicAddLocalUint));
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

TEST(BuiltInFuncTest, AtomicSubGlobalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicSubGlobalPositive));
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

TEST(BuiltInFuncTest, AtomicSubLocalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicSubLocalPositive));
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

TEST(BuiltInFuncTest, AtomicSubGlobalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicSubGlobalNegative));
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

TEST(BuiltInFuncTest, AtomicSubLocalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicSubLocalNegative));
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

TEST(BuiltInFuncTest, AtomicSubGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicSubGlobalUint));
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

TEST(BuiltInFuncTest, AtomicSubLocalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicSubLocalUint));
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

TEST(BuiltInFuncTest, AtomicXchgGlobalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicXchgGlobalPositive));
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

TEST(BuiltInFuncTest, AtomicXchgGlobalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicXchgGlobalNegative));
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

TEST(BuiltInFuncTest, AtomicXchgGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicXchgGlobalUint));
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

TEST(BuiltInFuncTest, AtomicIncGlobalTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicIncGlobal));
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

TEST(BuiltInFuncTest, AtomicIncLocalTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicIncLocal));
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

TEST(BuiltInFuncTest, AtomicIncGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicIncGlobalUint));
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

TEST(BuiltInFuncTest, AtomicIncLocalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicIncLocalUint));
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

TEST(BuiltInFuncTest, AtomicDecGlobalTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicDecGlobal));
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

TEST(BuiltInFuncTest, AtomicDecLocalTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicDecLocal));
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

TEST(BuiltInFuncTest, AtomicDecGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicDecGlobalUint));
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

TEST(BuiltInFuncTest, AtomicDecLocalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicDecLocalUint));
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

TEST(BuiltInFuncTest, AtomicCmpxchgGlobalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicCmpxchgGlobalPositive));
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

TEST(BuiltInFuncTest, AtomicCmpxchgLocalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicCmpxchgLocalPositive));
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

TEST(BuiltInFuncTest, AtomicCmpxchgGlobalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicCmpxchgGlobalNegative));
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

TEST(BuiltInFuncTest, AtomicCmpxchgLocalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicCmpxchgLocalNegative));
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

TEST(BuiltInFuncTest, AtomicCmpxchgGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicCmpxchgGlobalUint));
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

TEST(BuiltInFuncTest, AtomicCmpxchgLocalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicCmpxchgLocalUint));
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

TEST(BuiltInFuncTest, AtomicMinGlobalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMinGlobalPositive));
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

TEST(BuiltInFuncTest, AtomicMinLocalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMinLocalPositive));
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

TEST(BuiltInFuncTest, AtomicMinGlobalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMinGlobalNegative));
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

TEST(BuiltInFuncTest, AtomicMinLocalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMinLocalNegative));
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

TEST(BuiltInFuncTest, AtomicMinGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMinGlobalUint));
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

TEST(BuiltInFuncTest, AtomicMinLocalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMinLocalUint));
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

TEST(BuiltInFuncTest, AtomicMaxGlobalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMaxGlobalPositive));
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

TEST(BuiltInFuncTest, AtomicMaxLocalPositiveTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMaxLocalPositive));
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

TEST(BuiltInFuncTest, AtomicMaxGlobalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMaxGlobalNegative));
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

TEST(BuiltInFuncTest, AtomicMaxLocalNegativeTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMaxLocalNegative));
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

TEST(BuiltInFuncTest, AtomicMaxGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMaxGlobalUint));
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

TEST(BuiltInFuncTest, AtomicMaxLocalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicMaxLocalUint));
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

TEST(BuiltInFuncTest, AtomicAndGlobalTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicAndGlobal));
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

TEST(BuiltInFuncTest, AtomicAndGlobalUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicAndGlobalUint));
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

TEST(BuiltInFuncTest, AtomicOrGlobalTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicOrGlobal));
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

TEST(BuiltInFuncTest, AtomicOrUintTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicOrGlobalUint));
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

TEST(BuiltInFuncTest, AtomicFloatIncGlobalTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testAtomicFloatIncGlobal));
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

TEST(BuiltInFuncTest, RelationalF1Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 24;

    auto result_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testRelationalF1));
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

TEST(BuiltInFuncTest, RelationalF4Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b n = 12;

    auto result_buff = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testRelationalF4));
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

TEST(BuiltInFuncTest, BitselectUTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testBitselectU));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'selectBit' func for uint test failed.";
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
//        for (std::size_t i = 0; i < expected.size(); ++i)
//          EXPECT_EQ(expected[i], results[0][i]) << error_message;
//      }
    }
    {
      std::array<cl::ushort3, n> results;
      result3_buff->read(results.data(), results.size(), 0, 0);

//      {
//        const cl::ushort3 expected{0, 0, 0};
//        for (std::size_t i = 0; i < expected.size(); ++i)
//          EXPECT_EQ(expected[i], results[0][i]) << error_message;
//      }
    }
    {
      std::array<cl::uint4, n> results;
      result4_buff->read(results.data(), results.size(), 0, 0);

//      {
//        const cl::uint4 expected{0, 0, 0, 0};
//        for (std::size_t i = 0; i < expected.size(); ++i)
//          EXPECT_EQ(expected[i], results[0][i]) << error_message;
//      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, SelectITest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testSelectI));
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
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::char2 expected{-1, 2};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::short3, n> results;
      result3_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::short3 expected{1, -2, 3};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::short3 expected{-1, 2, -3};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::int4, n> results;
      result4_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::int4 expected{1, -2, 3, -4};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::int4 expected{-1, 2, -3, 4};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, SelectUTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testSelectU));
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
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::uchar2 expected{3, 2};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::ushort3, n> results;
      result3_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::ushort3 expected{1, 5, 3};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::ushort3 expected{4, 2, 6};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::uint4, n> results;
      result4_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::uint4 expected{1, 6, 3, 8};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::uint4 expected{5, 2, 7, 4};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, SelectFTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testSelectF));
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
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_FLOAT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::float2 expected{-1.0f, 2.0f};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_FLOAT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::float3, n> results;
      result3_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::float3 expected{1.0f, -2.0f, 3.0f};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::float3 expected{-1.0f, 2.0f, -3.0f};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }
    {
      std::array<cl::float4, n> results;
      result4_buff->read(results.data(), results.size(), 0, 0);

      {
        const cl::float4 expected{1.0f, -2.0f, 3.0f, -4.0f};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[0][i]) << error_message;
      }
      {
        const cl::float4 expected{-1.0f, 2.0f, -3.0f, 4.0f};
        for (std::size_t i = 0; i < expected.size(); ++i)
          ASSERT_EQ(expected[i], results[1][i]) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, Int8bVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 10;
    constexpr uint32b resolution = 2;

    auto buffer1 = makeStorageBuffer<int8b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n * resolution);
    {
      std::vector<int8b> inputs;
      inputs.resize(n * resolution);
      for (std::size_t i = 0; i < inputs.size(); ++i)
        inputs[i] = zisc::cast<int8b>(i);
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<int8b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(n * resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testInt8bVectorData));
    kernel->run(*buffer1, *buffer2, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The load and store funcs of int8b are wrong.";
    {
      std::vector<int8b> results;
      results.resize(n * resolution);
      buffer2->read(results.data(), results.size(), 0, 0);

      for (std::size_t i = 0; i < results.size(); ++i) {
        const int8b expected = zisc::cast<int8b>(2 * i);
        const int8b result = results[i];
        ASSERT_EQ(expected, result) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, Int16bVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 10;
    constexpr uint32b resolution = 2;

    auto buffer1 = makeStorageBuffer<int16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n * resolution);
    {
      std::vector<int16b> inputs;
      inputs.resize(n * resolution);
      for (std::size_t i = 0; i < inputs.size(); ++i)
        inputs[i] = zisc::cast<int16b>(i);
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<int16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(n * resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testInt16bVectorData));
    kernel->run(*buffer1, *buffer2, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The load and store funcs of int16b are wrong.";
    {
      std::vector<int16b> results;
      results.resize(n * resolution);
      buffer2->read(results.data(), results.size(), 0, 0);

      for (std::size_t i = 0; i < results.size(); ++i) {
        const int16b expected = zisc::cast<int16b>(2 * i);
        const int16b result = results[i];
        ASSERT_EQ(expected, result) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, Int32bVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 10;
    constexpr uint32b resolution = 2;

    auto buffer1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n * resolution);
    {
      std::vector<int32b> inputs;
      inputs.resize(n * resolution);
      for (std::size_t i = 0; i < inputs.size(); ++i)
        inputs[i] = zisc::cast<int32b>(i);
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(n * resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testInt32bVectorData));
    kernel->run(*buffer1, *buffer2, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The load and store funcs of int32b are wrong.";
    {
      std::vector<int32b> results;
      results.resize(n * resolution);
      buffer2->read(results.data(), results.size(), 0, 0);

      for (std::size_t i = 0; i < results.size(); ++i) {
        const int32b expected = zisc::cast<int32b>(2 * i);
        const int32b result = results[i];
        ASSERT_EQ(expected, result) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, UInt32bVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 10;
    constexpr uint32b resolution = 2;

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n * resolution);
    {
      std::vector<uint32b> inputs;
      inputs.resize(n * resolution);
      for (std::size_t i = 0; i < inputs.size(); ++i)
        inputs[i] = zisc::cast<uint32b>(i);
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(n * resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testUInt32bVectorData));
    kernel->run(*buffer1, *buffer2, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The load and store funcs of uint32b are wrong.";
    {
      std::vector<uint32b> results;
      results.resize(n * resolution);
      buffer2->read(results.data(), results.size(), 0, 0);

      for (std::size_t i = 0; i < results.size(); ++i) {
        const uint32b expected = zisc::cast<uint32b>(2 * i);
        const uint32b result = results[i];
        ASSERT_EQ(expected, result) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, FloatVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 10;
    constexpr uint32b resolution = 2;

    auto buffer1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n * resolution);
    {
      std::vector<float> inputs;
      inputs.resize(n * resolution);
      for (std::size_t i = 0; i < inputs.size(); ++i)
        inputs[i] = zisc::cast<float>(i);
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(n * resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testFloatVectorData));
    kernel->run(*buffer1, *buffer2, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The load and store funcs of float are wrong.";
    {
      std::vector<float> results;
      results.resize(n * resolution);
      buffer2->read(results.data(), results.size(), 0, 0);

      for (std::size_t i = 0; i < results.size(); ++i) {
        const float expected = zisc::cast<float>(2 * i);
        const float result = results[i];
        ASSERT_FLOAT_EQ(expected, result) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, HalfVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 10;
    constexpr uint32b resolution = 2;

    auto buffer1 = makeStorageBuffer<cl::half>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n * resolution);
    {
      std::vector<cl::half> inputs;
      inputs.resize(n * resolution);
      for (std::size_t i = 0; i < inputs.size(); ++i) {
        const float t = zisc::cast<float>(i);
        inputs[i] = zisc::SingleFloat::fromFloat(t);
      }
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<cl::half>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(n * resolution);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testHalfVectorData));
    kernel->run(*buffer1, *buffer2, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The load and store funcs of half are wrong.";
    {
      std::vector<cl::half> results;
      results.resize(n * resolution);
      buffer2->read(results.data(), results.size(), 0, 0);

      for (std::size_t i = 0; i < results.size(); ++i) {
        const float expected = zisc::cast<float>(3 * i);
        const zisc::SingleFloat t = results[i];
        const float result = t.toFloat();
        ASSERT_FLOAT_EQ(expected, result) << error_message;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    std::cout << "## Warning: only CPU test is done." << std::endl;
    break;
  }
}

TEST(BuiltInFuncTest, HalfUVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 10;
    constexpr std::size_t n_special = 5;
    constexpr uint32b resolution = 1024;

    auto buffer1 = makeStorageBuffer<uint16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n * resolution + n_special);
    {
      std::vector<uint16b> inputs;
      inputs.resize(buffer1->size());
      std::size_t index = 0;
      for (std::size_t i = 0; i < resolution; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
          const std::size_t u = (n * (i / 4)) + j;
          std::size_t shift = 0;
          {
            std::size_t k = i;
            while (k) {
              ++shift;
              k = k >> 1;
            }
          }
          float t = zisc::cast<float>((u * u) >> shift);
          t = (1 < (i % 4)) ? 1.0f / t : t;
          t = ((i % 2) == 0) ? t : -t;
//          std::cout << "[" << index << "]: u = " << u
//                    << ", f = " << std::scientific << t << std::endl;
          const auto data = zisc::SingleFloat::fromFloat(t);
          inputs[index++] = zisc::HalfFloat{data}.bits();
        }
      }
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(buffer1->size());

    auto buffer3 = makeStorageBuffer<uint16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(buffer1->size());

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(built_in_func, testHalfUVectorData));
    kernel->run(*buffer1, *buffer2, *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The load and store funcs of uint16 are wrong:";
    {
      std::vector<float> fresults;
      fresults.resize(buffer1->size());
      buffer2->read(fresults.data(), fresults.size(), 0, 0);

      std::vector<uint16b> results;
      results.resize(buffer1->size());
      buffer3->read(results.data(), results.size(), 0, 0);

      std::size_t index = 0;
      for (std::size_t i = 0; i < resolution; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
          const std::size_t u = (n * (i / 4)) + j;
          std::size_t shift = 0;
          {
            std::size_t k = i;
            while (k) {
              ++shift;
              k = k >> 1;
            }
          }
          float expected = zisc::cast<float>((u * u) >> shift);
          {
            bool is_subnormal_half = false;
            expected = (1 < (i % 4)) ? 1.0f / expected : expected;
            expected = ((i % 2) == 0) ? expected : -expected;
            {
              auto data = zisc::SingleFloat::fromFloat(expected);
              auto hdata = zisc::HalfFloat{data};
              is_subnormal_half = hdata.isSubnormal();
              data = hdata;
              expected = data.toFloat();
            }
            const float result = fresults[index];
            ASSERT_FALSE(is_subnormal_half);
            EXPECT_FLOAT_EQ(expected, result) << error_message
                << " [" << index << "]: " << std::scientific << result << std::endl
                << "    subnormal: " << is_subnormal_half;
          }
          {
            expected = 3.0f * expected;
            {
              auto data = zisc::SingleFloat::fromFloat(expected);
              data = zisc::HalfFloat{data};
              expected = data.toFloat();
            }
            const zisc::HalfFloat data{results[index]};
            const zisc::SingleFloat t{data};
            const float result = t.toFloat();
            const std::bitset<16> bits{results[index]};
            EXPECT_FLOAT_EQ(expected, result) << error_message
                << " [" << index << "]: " << std::scientific << result << std::endl
                << "bits : " << bits;
            std::cout << "## Bit: " << bits << std::endl;
          }
          ++index;
          if (index == 24)
            break;
        }
        if( index == 24 )
          break;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
