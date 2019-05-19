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
#include <cstddef>
#include <iostream>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/built_in_func.hpp"
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

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(9);
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    buffer2->setSize(resolution);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    buffer3->setSize(resolution);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto resolution_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    resolution_buff->setSize(1);
    resolution_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testWorkItem1D, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, *resolution_buff, {resolution}, 0);
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

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(9);
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    buffer2->setSize(res);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    buffer3->setSize(res);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto resolution_buff = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTDst);
    resolution_buff->setSize(1);
    resolution_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testWorkItem2D, 2);
    kernel->run(*buffer1, *buffer2, *buffer3, *resolution_buff, {resolution.x, resolution.y}, 0);
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

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    buffer1->setSize(9);
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    buffer2->setSize(res);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    buffer3->setSize(res);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto resolution_buff = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTDst);
    resolution_buff->setSize(1);
    resolution_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testWorkItem3D, 3);
    kernel->run(*buffer1, *buffer2, *buffer3, *resolution_buff, {resolution.x, resolution.y, resolution.z}, 0);
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

    auto table_buf = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    table_buf->setSize(resolution);
    auto results_buf = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
    results_buf->setSize(resolution);
    auto resolution_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    resolution_buff->setSize(1);
    resolution_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testLocalMemoryFence, 1);
    kernel->run(*table_buf, *results_buf, *resolution_buff, {resolution}, 0);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddGlobalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddLocalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddGlobalNegative, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddLocalNegative, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddGlobalUint, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddLocalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubGlobalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubLocalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubGlobalNegative, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubLocalNegative, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubGlobalUint, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubLocalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicXchgGlobalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicXchgGlobalNegative, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicXchgGlobalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicIncGlobal, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicIncLocal, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicIncGlobalUint, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicIncLocalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicDecGlobal, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicDecLocal, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicDecGlobalUint, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicDecLocalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgGlobalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgLocalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgGlobalNegative, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgLocalNegative, 1);
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
      const int32b expected = -zisc::cast<int32b>(resolution);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgGlobalUint, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgLocalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinGlobalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinLocalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinGlobalNegative, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinLocalNegative, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinGlobalUint, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinLocalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxGlobalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxLocalPositive, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::min();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxGlobalNegative, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::min();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxLocalNegative, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxGlobalUint, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxLocalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAndGlobal, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAndGlobalUint, 1);
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

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicOrGlobal, 1);
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicOrGlobalUint, 1);
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

    auto result_buff = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrcDst);
    result_buff->setSize(1);
    {
      const float init = 0.0f;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution, 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicFloatIncGlobal, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
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

//TEST(BuiltInFuncTest, RelationalFunctionTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 24;
//    constexpr std::size_t n_vectors = 11;
//    auto scalar_results =
//        makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    scalar_results->setSize(n_scalars);
//    auto vector_results =
//        makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    vector_results->setSize(n_vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testRelational, 1);
//    kernel->run(*scalar_results, *vector_results, {1}, 0);
//    device->waitForCompletion();
//
//    // Scalar results
//    {
//      std::array<int32b, n_scalars> result;
//      scalar_results->read(result.data(), result.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_TRUE(result[index++]) << "The isequal func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isequal func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isnotequal func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The isnotequal func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The isgreater func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isgreater func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isgreater func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The isgreaterequal func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The isgreaterequal func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isgreaterequal func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isless func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isless func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The isless func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The islessequal func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The islessequal func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The islessequal func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isinf func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The isinf func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The isnan func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The isnan func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The signbit func is wrong.";
//      ASSERT_FALSE(result[index++]) << "The signbit func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The signbit func is wrong.";
//      ASSERT_TRUE(result[index++]) << "The signbit func is wrong.";
//    }
//    // Vector results
//    {
//      std::array<cl::int4, n_vectors> result;
//      vector_results->read(result.data(), result.size(), 0, 0);
//      std::size_t index = 0;
//      {
//        const cl::int4 expected{kVecTrue, kVecTrue, kVecTrue, kVecTrue};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isequal func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecFalse, kVecTrue, kVecFalse, kVecFalse};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isequal func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecFalse, kVecFalse, kVecFalse, kVecFalse};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isnotequal func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecTrue, kVecFalse, kVecTrue, kVecTrue};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isnotequal func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecFalse, kVecFalse, kVecTrue, kVecTrue};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isgreater func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecFalse, kVecTrue, kVecTrue, kVecTrue};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isgreaterequal func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecTrue, kVecFalse, kVecFalse, kVecFalse};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isless func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecTrue, kVecTrue, kVecFalse, kVecFalse};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The islessequal func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecFalse, kVecTrue, kVecTrue, kVecFalse};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isinf func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecFalse, kVecFalse, kVecFalse, kVecTrue};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The isnan func is wrong.";
//      }
//      {
//        const cl::int4 expected{kVecFalse, kVecFalse, kVecTrue, kVecTrue};
//        const auto r = result[index++];
//        for (std::size_t i = 0; i < 4; ++i) 
//          ASSERT_EQ(expected[i], r[i]) << "The signbit func is wrong.";
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, SelectTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 4;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testSelect, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'select' func is wrong.";
//    // Scalar results
//    {
//      std::array<int32b, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//      ASSERT_EQ(-5, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::int2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index++].y) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::int3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index].y) << error_message;
//      ASSERT_EQ(-9, results[index++].z) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::int4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index].y) << error_message;
//      ASSERT_EQ(-9, results[index].z) << error_message;
//      ASSERT_EQ(-15, results[index++].w) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index].z) << error_message;
//      ASSERT_EQ(15, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, zSelectImplTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 4;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zSelectImpl' func is wrong.";
//    // Scalar results
//    {
//      std::array<int32b, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//      ASSERT_EQ(-5, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::int2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index++].y) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::int3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index].y) << error_message;
//      ASSERT_EQ(-9, results[index++].z) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::int4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index].y) << error_message;
//      ASSERT_EQ(-9, results[index].z) << error_message;
//      ASSERT_EQ(-15, results[index++].w) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index].z) << error_message;
//      ASSERT_EQ(15, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, zSelectTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 4;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelect, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zSelect' func is wrong.";
//    // Scalar results
//    {
//      std::array<int32b, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//      ASSERT_EQ(-5, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::int2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index++].y) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::int3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index].y) << error_message;
//      ASSERT_EQ(-9, results[index++].z) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::int4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5, results[index].x) << error_message;
//      ASSERT_EQ(-2, results[index].y) << error_message;
//      ASSERT_EQ(-9, results[index].z) << error_message;
//      ASSERT_EQ(-15, results[index++].w) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index].z) << error_message;
//      ASSERT_EQ(15, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, SelectUTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 2;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testSelectU, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'select' func is wrong.";
//    // Scalar results
//    {
//      std::array<uint32b, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::uint2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index++].y) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::uint3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index++].z) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index].y) << error_message;
//      ASSERT_EQ(18, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::uint4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index].z) << error_message;
//      ASSERT_EQ(15, results[index++].w) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index].y) << error_message;
//      ASSERT_EQ(18, results[index].z) << error_message;
//      ASSERT_EQ(30, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, zSelectUImplTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 2;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectUImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zSelectUImpl' func is wrong.";
//    // Scalar results
//    {
//      std::array<uint32b, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::uint2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index++].y) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::uint3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index++].z) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index].y) << error_message;
//      ASSERT_EQ(18, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::uint4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index].z) << error_message;
//      ASSERT_EQ(15, results[index++].w) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index].y) << error_message;
//      ASSERT_EQ(18, results[index].z) << error_message;
//      ASSERT_EQ(30, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, zSelectUTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 2;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectU, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zSelectU' func is wrong.";
//    // Scalar results
//    {
//      std::array<uint32b, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index++]) << error_message;
//      ASSERT_EQ(5, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::uint2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index++].y) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::uint3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index++].z) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index].y) << error_message;
//      ASSERT_EQ(18, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::uint4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(10, results[index].x) << error_message;
//      ASSERT_EQ(2, results[index].y) << error_message;
//      ASSERT_EQ(9, results[index].z) << error_message;
//      ASSERT_EQ(15, results[index++].w) << error_message;
//      ASSERT_EQ(5, results[index].x) << error_message;
//      ASSERT_EQ(4, results[index].y) << error_message;
//      ASSERT_EQ(18, results[index].z) << error_message;
//      ASSERT_EQ(30, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, SelectFTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 4;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testSelectF, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'select' func is wrong.";
//    // Scalar results
//    {
//      std::array<float, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index++]) << error_message;
//      ASSERT_EQ(5.0f, results[index++]) << error_message;
//      ASSERT_EQ(-5.0f, results[index++]) << error_message;
//      ASSERT_EQ(5.0f, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::float2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index++].y) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::float3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index].y) << error_message;
//      ASSERT_EQ(-9.0f, results[index++].z) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index].y) << error_message;
//      ASSERT_EQ(9.0f, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::float4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index].y) << error_message;
//      ASSERT_EQ(-9.0f, results[index].z) << error_message;
//      ASSERT_EQ(-15.0f, results[index++].w) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index].y) << error_message;
//      ASSERT_EQ(9.0f, results[index].z) << error_message;
//      ASSERT_EQ(15.0f, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, zSelectFImplTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 4;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectFImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zSelectFImpl' func is wrong.";
//    // Scalar results
//    {
//      std::array<float, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index++]) << error_message;
//      ASSERT_EQ(5.0f, results[index++]) << error_message;
//      ASSERT_EQ(-5.0f, results[index++]) << error_message;
//      ASSERT_EQ(5.0f, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::float2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index++].y) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::float3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index].y) << error_message;
//      ASSERT_EQ(-9.0f, results[index++].z) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index].y) << error_message;
//      ASSERT_EQ(9.0f, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::float4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index].y) << error_message;
//      ASSERT_EQ(-9.0f, results[index].z) << error_message;
//      ASSERT_EQ(-15.0f, results[index++].w) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index].y) << error_message;
//      ASSERT_EQ(9.0f, results[index].z) << error_message;
//      ASSERT_EQ(15.0f, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(BuiltInFuncTest, zSelectFTest)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr std::size_t n_scalars = 4;
//    constexpr std::size_t n_2vectors = 2;
//    constexpr std::size_t n_3vectors = 2;
//    constexpr std::size_t n_4vectors = 2;
//    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n_scalars);
//    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n_2vectors);
//    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n_3vectors);
//    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n_4vectors);
//
//    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectF, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zSelectF' func is wrong.";
//    // Scalar results
//    {
//      std::array<float, n_scalars> results;
//      results1->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index++]) << error_message;
//      ASSERT_EQ(5.0f, results[index++]) << error_message;
//      ASSERT_EQ(-5.0f, results[index++]) << error_message;
//      ASSERT_EQ(5.0f, results[index++]) << error_message;
//    }
//    // Vector2 results
//    {
//      std::array<cl::float2, n_2vectors> results;
//      results2->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index++].y) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index++].y) << error_message;
//    }
//    // Vector3 results
//    {
//      std::array<cl::float3, n_3vectors> results;
//      results3->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index].y) << error_message;
//      ASSERT_EQ(-9.0f, results[index++].z) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index].y) << error_message;
//      ASSERT_EQ(9.0f, results[index++].z) << error_message;
//    }
//    // Vector4 results
//    {
//      std::array<cl::float4, n_4vectors> results;
//      results4->read(results.data(), results.size(), 0, 0);
//      std::size_t index = 0;
//      ASSERT_EQ(-5.0f, results[index].x) << error_message;
//      ASSERT_EQ(-2.0f, results[index].y) << error_message;
//      ASSERT_EQ(-9.0f, results[index].z) << error_message;
//      ASSERT_EQ(-15.0f, results[index++].w) << error_message;
//      ASSERT_EQ(5.0f, results[index].x) << error_message;
//      ASSERT_EQ(2.0f, results[index].y) << error_message;
//      ASSERT_EQ(9.0f, results[index].z) << error_message;
//      ASSERT_EQ(15.0f, results[index++].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}

