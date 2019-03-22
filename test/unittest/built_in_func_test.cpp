/*!
  \file built_in_func_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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

  constexpr uint resolution = 1024;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(9);
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    buffer2->setSize(resolution);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    buffer3->setSize(1);
    buffer3->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testWorkItem1D, 1);
    kernel->run(*buffer1, *buffer2, *buffer3, {resolution}, 0);
    device->waitForCompletion();

    {
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
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]);
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, WorkItem2DTest)
{
  using namespace zinvul;

  const cl::uint2 resolution{32, 32};

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(9);
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    buffer2->setSize(resolution.x * resolution.y);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceDst);
    buffer3->setSize(1);
    {
      buffer3->write(&resolution, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testWorkItem2D, 2);
    kernel->run(*buffer1, *buffer2, *buffer3, {resolution.x, resolution.y}, 0);
    device->waitForCompletion();

    {
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
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]);
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, WorkItem3DTest)
{
  using namespace zinvul;

  const cl::uint3 resolution{16, 8, 8};

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(9);
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    buffer2->setSize(resolution.x * resolution.y * resolution.z);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceDst);
    buffer3->setSize(1);
    {
      buffer3->write(&resolution, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testWorkItem3D, 3);
    kernel->run(*buffer1, *buffer2, *buffer3, {resolution.x, resolution.y, resolution.z}, 0);
    device->waitForCompletion();

    {
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
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]);
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, LocalDataShareTest)
{
  using namespace zinvul;

  constexpr uint resolution = 1024;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(resolution);
    auto resolution_buf = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    resolution_buf->setSize(1);
    resolution_buf->write(&resolution, 1, 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testLocalDataShare, 1);
    kernel->run(*buffer1, *resolution_buf, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<uint32b> results;
      results.resize(buffer1->size(), 0u);
      buffer1->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_EQ(64, results[i]);
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicFuncTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;
//  constexpr uint num_of_funcs = 11;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto add_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    add_table->setSize(resolution);
    auto add_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    add_result->setSize(1);
    auto sub_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    sub_table->setSize(resolution);
    auto sub_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    sub_result->setSize(1);
    auto xchg_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    xchg_table->setSize(resolution);
    auto xchg_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    xchg_result->setSize(1);
    auto inc_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    inc_table->setSize(resolution);
    auto inc_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    inc_result->setSize(1);
    auto dec_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    dec_table->setSize(resolution);
    auto dec_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    dec_result->setSize(1);
    auto cmpxchg_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    cmpxchg_table->setSize(resolution);
    auto cmpxchg_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    cmpxchg_result->setSize(1);
    auto min_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    min_table->setSize(resolution);
    auto min_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    min_result->setSize(1);
    auto max_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    max_table->setSize(resolution);
    auto max_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    max_result->setSize(1);
    auto and_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    and_table->setSize(resolution);
    auto and_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    and_result->setSize(1);
    auto or_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    or_table->setSize(resolution);
    auto or_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    or_result->setSize(1);
    auto xor_table = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    xor_table->setSize(resolution);
    auto xor_result = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    xor_result->setSize(1);
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto init_kernel = makeZinvulKernel(device.get(), built_in_func, initAtomicTest, 1);
    init_kernel->run(*add_table, *add_result, *sub_table, *sub_result,
        *xchg_table, *xchg_result, *inc_table, *inc_result,
        *dec_table, *dec_result, *cmpxchg_table, *cmpxchg_result,
        *min_table, *min_result, *max_table, *max_result,
        *and_table, *and_result, *or_table, *or_result,
        *xor_table, *xor_result, *res_buff, {1}, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomic, 1);
    kernel->run(*add_table, *add_result, *sub_table, *sub_result,
        *xchg_table, *xchg_result, *inc_table, *inc_result,
        *dec_table, *dec_result, *cmpxchg_table, *cmpxchg_result,
        *min_table, *min_result, *max_table, *max_result,
        *and_table, *and_result, *or_table, *or_result,
        *xor_table, *xor_result, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_add
    {
      std::array<int32b, resolution> table;
      add_table->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_NE(0, table[i]) << "The 'atomic_add' is wrong.";
      int32b result;
      add_result->read(&result, 1, 0, 0);
      EXPECT_EQ(resolution, result) << "The 'atomic_add' is wrong.";
    }
    // atomic_sub
    {
      std::array<int32b, resolution> table;
      sub_table->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_NE(0, table[i]) << "The 'atomic_sub' is wrong.";
      int32b result;
      sub_result->read(&result, 1, 0, 0);
      EXPECT_EQ(resolution, -result) << "The 'atomic_sub' is wrong.";
    }
    // atomic_inc
    {
      std::array<int32b, resolution> table;
      inc_table->read(table.data(), table.size(), 0,0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_NE(0, table[i]) << "The 'atomic_inc' is wrong.";
      int32b result;
      inc_result->read(&result, 1, 0, 0);
      EXPECT_EQ(resolution, result) << "The 'atomic_inc' is wrong.";
    }
    // atomic_dec
    {
      std::array<int32b, resolution> table;
      dec_table->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_NE(0, table[i]) << "The 'atomic_dec' is wrong.";
      int32b result;
      dec_result->read(&result, 1, 0, 0);
      EXPECT_EQ(resolution, -result) << "The 'atomic_dec' is wrong.";
    }
    // atomic_min
    {
      int32b result;
      min_result->read(&result, 1, 0, 0);
//      constexpr int32b expected = -zisc::cast<int32b>(resolution >> 1) + 1;
//      EXPECT_EQ(expected, result) << "The 'atomic_min' is wrong.";
    }
    // atomic_max
    {
      int32b result;
      max_result->read(&result, 1, 0, 0);
//      constexpr int32b expected = zisc::cast<int32b>(resolution >> 1);
//      EXPECT_EQ(expected, result) << "The 'atomic_max' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicAddPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddPositive, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_add
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_add' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'atomic_add' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicAddNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddNegative, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_add
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_add' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'atomic_add' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicAddUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAddUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_add
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_add' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << "The 'atomic_add' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicAddPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicAddPositive, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_add
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicAdd' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'zAtomicAdd' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicAddNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicAddNegative, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_add
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicAdd' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'zAtomicAdd' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicAddUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicAddUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_add
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicAddU' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << "The 'zAtomicAddU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicSubPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubPositive, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_sub
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_sub' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'atomic_sub' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicSubNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubNegative, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_sub
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_sub' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'atomic_sub' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicSubUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicSubUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_sub
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_sub' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << "The 'atomic_sub' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicSubPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicSubPositive, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_sub
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicSub' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'zAtomicSub' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicSubNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicSubNegative, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_sub
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicSub' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'zAtomicSub' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicSubUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicSubUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_sub
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicSubU' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << "The 'zAtomicSubU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicXchgPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicXchgPositive, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_xchg
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
      EXPECT_EQ(last_one, result) << "The 'atomic_xchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicXchgNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicXchgNegative, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_xchg
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
      EXPECT_EQ(last_one, result) << "The 'atomic_xchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicXchgUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicXchgUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_xchg
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
      EXPECT_EQ(last_one, result) << "The 'atomic_xchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicXchgPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicXchgPositive, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_xchg
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
      EXPECT_EQ(last_one, result) << "The 'zAtomicXchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicXchgNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicXchgNegative, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_xchg
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
      EXPECT_EQ(last_one, result) << "The 'zAtomicXchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicXchgUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicXchgUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_xchg
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
      EXPECT_EQ(last_one, result) << "The 'zAtomicXchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicIncTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicInc, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_inc
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_inc' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'atomic_inc' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicIncUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicIncUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_inc
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_inc' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << "The 'atomic_inc' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicDecTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicDec, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_inc
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_dec' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'atomic_dec' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicDecUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicDecUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_inc
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_dec' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << "The 'atomic_dec' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicIncTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicInc, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_inc
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicInc' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'zAtomicInc' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicIncUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicIncUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_inc
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicIncU' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << "The 'zAtomicIncU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicDecTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicDec, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_inc
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicDec' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'zAtomicDec' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicDecUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = resolution;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicDecUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_inc
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicDecU' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << "The 'zAtomicDecU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicCmpxchgPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgPositive, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_cmpxchg
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_cmpxchg' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'atomic_cmpxchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicCmpxchgNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgNegative, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_cmpxchg
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_cmpxchg' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'atomic_cmpxchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicCmpxchgUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicCmpxchgUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_cmpxchg
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'atomic_cmpxchg' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << "The 'atomic_cmpxchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicCmpxchgPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicCmpxchgPositive, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_cmpxchg
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicCmpxchg' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'zAtomicCmpxchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicCmpxchgNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicCmpxchgNegative, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_cmpxchg
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicCmpxchg' is wrong.";

      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution);
      EXPECT_EQ(expected, result) << "The 'zAtomicCmpxchg' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicCmpxchgUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto table_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    table_buff->setSize(resolution);
    {
      std::vector<int32b> init;
      init.resize(resolution);
      std::fill(init.begin(), init.end(), 0);
      table_buff->write(init.data(), init.size(), 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicCmpxchgUint, 1);
    kernel->run(*result_buff, *table_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_cmpxchg
    {
      std::vector<int32b> table;
      table.resize(resolution);
      table_buff->read(table.data(), table.size(), 0, 0);
      for (std::size_t i = 0; i < table.size(); ++i)
        ASSERT_TRUE(table[i]) << "The 'zAtomicCmpxchgU' is wrong.";

      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution;
      EXPECT_EQ(expected, result) << "The 'zAtomicCmpxchgU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicMinPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinPositive, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_min
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = 0;
      EXPECT_EQ(expected, result) << "The 'atomic_min' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicMinNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinNegative, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_min
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution - 1);
      EXPECT_EQ(expected, result) << "The 'atomic_min' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicMinUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMinUint, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_min
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = 0;
      EXPECT_EQ(expected, result) << "The 'atomic_min' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicMaxPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxPositive, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_max
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution - 1);
      EXPECT_EQ(expected, result) << "The 'atomic_max' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicMaxNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::min();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxNegative, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_max
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = 0;
      EXPECT_EQ(expected, result) << "The 'atomic_max' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicMaxUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicMaxUint, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_max
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution - 1;
      EXPECT_EQ(expected, result) << "The 'atomic_max' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicMinPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicMinPositive, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_min
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = 0;
      EXPECT_EQ(expected, result) << "The 'zAtomicMin' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicMinNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicMinNegative, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_min
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -zisc::cast<int32b>(resolution - 1);
      EXPECT_EQ(expected, result) << "The 'zAtomicMin' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicMinUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicMinUint, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_min
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = 0;
      EXPECT_EQ(expected, result) << "The 'zAtomicMinU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicMaxPositiveTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicMaxPositive, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_max
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = zisc::cast<int32b>(resolution - 1);
      EXPECT_EQ(expected, result) << "The 'zAtomicMax' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicMaxNegativeTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = std::numeric_limits<int32b>::min();
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicMaxNegative, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_max
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = 0;
      EXPECT_EQ(expected, result) << "The 'zAtomicMax' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicMaxUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 10000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }
    auto res_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicMaxUint, 1);
    kernel->run(*result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_max
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = resolution - 1;
      EXPECT_EQ(expected, result) << "The 'zAtomicMaxU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicAndTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAnd, 1);
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_and
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << "The 'atomic_and' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicAndUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicAndUint, 1);
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_and
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << "The 'atomic_and' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicAndTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = -1;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicAnd, 1);
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_and
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << "The 'zAtomicAnd' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicAndUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = std::numeric_limits<uint32b>::max();
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicAndUint, 1);
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_and
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      EXPECT_FALSE(result) << "The 'zAtomicAndU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, AtomicOrTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicOr, 1);
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_or
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -1;
      EXPECT_EQ(expected, result) << "The 'atomic_or' is wrong.";
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

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testAtomicOrUint, 1);
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_or
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = std::numeric_limits<uint32b>::max();
      EXPECT_EQ(expected, result) << "The 'atomic_or' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicOrTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const int32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicOr, 1);
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_or
    {
      int32b result;
      result_buff->read(&result, 1, 0, 0);
      const int32b expected = -1;
      EXPECT_EQ(expected, result) << "The 'zAtomicOr' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zAtomicOrUintTest)
{
  using namespace zinvul;

  constexpr uint resolution = 32;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result_buff = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrcDst);
    result_buff->setSize(1);
    {
      const uint32b init = 0;
      result_buff->write(&init, 1, 0, 0);
    }

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzAtomicOrUint, 1);
    kernel->run(*result_buff, {resolution}, 0);
    device->waitForCompletion();

    // atomic_or
    {
      uint32b result;
      result_buff->read(&result, 1, 0, 0);
      const uint32b expected = std::numeric_limits<uint32b>::max();
      EXPECT_EQ(expected, result) << "The 'zAtomicOrU' is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, RelationalFunctionTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 24;
    constexpr std::size_t n_vectors = 11;
    auto scalar_results =
        makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    scalar_results->setSize(n_scalars);
    auto vector_results =
        makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    vector_results->setSize(n_vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testRelational, 1);
    kernel->run(*scalar_results, *vector_results, {1}, 0);
    device->waitForCompletion();

    // Scalar results
    {
      std::array<int32b, n_scalars> result;
      scalar_results->read(result.data(), result.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_TRUE(result[index++]) << "The isequal func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isequal func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isnotequal func is wrong.";
      ASSERT_TRUE(result[index++]) << "The isnotequal func is wrong.";
      ASSERT_TRUE(result[index++]) << "The isgreater func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isgreater func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isgreater func is wrong.";
      ASSERT_TRUE(result[index++]) << "The isgreaterequal func is wrong.";
      ASSERT_TRUE(result[index++]) << "The isgreaterequal func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isgreaterequal func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isless func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isless func is wrong.";
      ASSERT_TRUE(result[index++]) << "The isless func is wrong.";
      ASSERT_FALSE(result[index++]) << "The islessequal func is wrong.";
      ASSERT_TRUE(result[index++]) << "The islessequal func is wrong.";
      ASSERT_TRUE(result[index++]) << "The islessequal func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isinf func is wrong.";
      ASSERT_TRUE(result[index++]) << "The isinf func is wrong.";
      ASSERT_FALSE(result[index++]) << "The isnan func is wrong.";
      ASSERT_TRUE(result[index++]) << "The isnan func is wrong.";
      ASSERT_FALSE(result[index++]) << "The signbit func is wrong.";
      ASSERT_FALSE(result[index++]) << "The signbit func is wrong.";
      ASSERT_TRUE(result[index++]) << "The signbit func is wrong.";
      ASSERT_TRUE(result[index++]) << "The signbit func is wrong.";
    }
    // Vector results
    {
      std::array<cl::int4, n_vectors> result;
      vector_results->read(result.data(), result.size(), 0, 0);
      std::size_t index = 0;
      {
        const cl::int4 expected{kVecTrue, kVecTrue, kVecTrue, kVecTrue};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isequal func is wrong.";
      }
      {
        const cl::int4 expected{kVecFalse, kVecTrue, kVecFalse, kVecFalse};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isequal func is wrong.";
      }
      {
        const cl::int4 expected{kVecFalse, kVecFalse, kVecFalse, kVecFalse};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isnotequal func is wrong.";
      }
      {
        const cl::int4 expected{kVecTrue, kVecFalse, kVecTrue, kVecTrue};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isnotequal func is wrong.";
      }
      {
        const cl::int4 expected{kVecFalse, kVecFalse, kVecTrue, kVecTrue};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isgreater func is wrong.";
      }
      {
        const cl::int4 expected{kVecFalse, kVecTrue, kVecTrue, kVecTrue};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isgreaterequal func is wrong.";
      }
      {
        const cl::int4 expected{kVecTrue, kVecFalse, kVecFalse, kVecFalse};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isless func is wrong.";
      }
      {
        const cl::int4 expected{kVecTrue, kVecTrue, kVecFalse, kVecFalse};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The islessequal func is wrong.";
      }
      {
        const cl::int4 expected{kVecFalse, kVecTrue, kVecTrue, kVecFalse};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isinf func is wrong.";
      }
      {
        const cl::int4 expected{kVecFalse, kVecFalse, kVecFalse, kVecTrue};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The isnan func is wrong.";
      }
      {
        const cl::int4 expected{kVecFalse, kVecFalse, kVecTrue, kVecTrue};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i) 
          ASSERT_EQ(expected[i], r[i]) << "The signbit func is wrong.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, SelectTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 4;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testSelect, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'select' func is wrong.";
    // Scalar results
    {
      std::array<int32b, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
      ASSERT_EQ(-5, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::int2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index++].y) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::int3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index].y) << error_message;
      ASSERT_EQ(-9, results[index++].z) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::int4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index].y) << error_message;
      ASSERT_EQ(-9, results[index].z) << error_message;
      ASSERT_EQ(-15, results[index++].w) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index].z) << error_message;
      ASSERT_EQ(15, results[index++].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zSelectImplTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 4;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zSelectImpl' func is wrong.";
    // Scalar results
    {
      std::array<int32b, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
      ASSERT_EQ(-5, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::int2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index++].y) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::int3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index].y) << error_message;
      ASSERT_EQ(-9, results[index++].z) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::int4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index].y) << error_message;
      ASSERT_EQ(-9, results[index].z) << error_message;
      ASSERT_EQ(-15, results[index++].w) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index].z) << error_message;
      ASSERT_EQ(15, results[index++].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zSelectTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 4;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::int2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::int3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelect, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zSelect' func is wrong.";
    // Scalar results
    {
      std::array<int32b, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
      ASSERT_EQ(-5, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::int2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index++].y) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::int3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index].y) << error_message;
      ASSERT_EQ(-9, results[index++].z) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::int4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5, results[index].x) << error_message;
      ASSERT_EQ(-2, results[index].y) << error_message;
      ASSERT_EQ(-9, results[index].z) << error_message;
      ASSERT_EQ(-15, results[index++].w) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index].z) << error_message;
      ASSERT_EQ(15, results[index++].w) << error_message;
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

    constexpr std::size_t n_scalars = 2;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testSelectU, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'select' func is wrong.";
    // Scalar results
    {
      std::array<uint32b, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::uint2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index++].y) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::uint3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index++].z) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index].y) << error_message;
      ASSERT_EQ(18, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::uint4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index].z) << error_message;
      ASSERT_EQ(15, results[index++].w) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index].y) << error_message;
      ASSERT_EQ(18, results[index].z) << error_message;
      ASSERT_EQ(30, results[index++].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zSelectUImplTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 2;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectUImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zSelectUImpl' func is wrong.";
    // Scalar results
    {
      std::array<uint32b, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::uint2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index++].y) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::uint3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index++].z) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index].y) << error_message;
      ASSERT_EQ(18, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::uint4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index].z) << error_message;
      ASSERT_EQ(15, results[index++].w) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index].y) << error_message;
      ASSERT_EQ(18, results[index].z) << error_message;
      ASSERT_EQ(30, results[index++].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zSelectUTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 2;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectU, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zSelectU' func is wrong.";
    // Scalar results
    {
      std::array<uint32b, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index++]) << error_message;
      ASSERT_EQ(5, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::uint2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index++].y) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::uint3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index++].z) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index].y) << error_message;
      ASSERT_EQ(18, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::uint4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(10, results[index].x) << error_message;
      ASSERT_EQ(2, results[index].y) << error_message;
      ASSERT_EQ(9, results[index].z) << error_message;
      ASSERT_EQ(15, results[index++].w) << error_message;
      ASSERT_EQ(5, results[index].x) << error_message;
      ASSERT_EQ(4, results[index].y) << error_message;
      ASSERT_EQ(18, results[index].z) << error_message;
      ASSERT_EQ(30, results[index++].w) << error_message;
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

    constexpr std::size_t n_scalars = 4;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testSelectF, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'select' func is wrong.";
    // Scalar results
    {
      std::array<float, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index++]) << error_message;
      ASSERT_EQ(5.0f, results[index++]) << error_message;
      ASSERT_EQ(-5.0f, results[index++]) << error_message;
      ASSERT_EQ(5.0f, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::float2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index++].y) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::float3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index].y) << error_message;
      ASSERT_EQ(-9.0f, results[index++].z) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index].y) << error_message;
      ASSERT_EQ(9.0f, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::float4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index].y) << error_message;
      ASSERT_EQ(-9.0f, results[index].z) << error_message;
      ASSERT_EQ(-15.0f, results[index++].w) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index].y) << error_message;
      ASSERT_EQ(9.0f, results[index].z) << error_message;
      ASSERT_EQ(15.0f, results[index++].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zSelectFImplTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 4;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectFImpl, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zSelectFImpl' func is wrong.";
    // Scalar results
    {
      std::array<float, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index++]) << error_message;
      ASSERT_EQ(5.0f, results[index++]) << error_message;
      ASSERT_EQ(-5.0f, results[index++]) << error_message;
      ASSERT_EQ(5.0f, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::float2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index++].y) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::float3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index].y) << error_message;
      ASSERT_EQ(-9.0f, results[index++].z) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index].y) << error_message;
      ASSERT_EQ(9.0f, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::float4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index].y) << error_message;
      ASSERT_EQ(-9.0f, results[index].z) << error_message;
      ASSERT_EQ(-15.0f, results[index++].w) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index].y) << error_message;
      ASSERT_EQ(9.0f, results[index].z) << error_message;
      ASSERT_EQ(15.0f, results[index++].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, zSelectFTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 4;
    constexpr std::size_t n_2vectors = 2;
    constexpr std::size_t n_3vectors = 2;
    constexpr std::size_t n_4vectors = 2;
    auto results1 = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
    results1->setSize(n_scalars);
    auto results2 = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
    results2->setSize(n_2vectors);
    auto results3 = makeBuffer<cl::float3>(device.get(), BufferUsage::kDeviceSrc);
    results3->setSize(n_3vectors);
    auto results4 = makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    results4->setSize(n_4vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testzSelectF, 1);
    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'zSelectF' func is wrong.";
    // Scalar results
    {
      std::array<float, n_scalars> results;
      results1->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index++]) << error_message;
      ASSERT_EQ(5.0f, results[index++]) << error_message;
      ASSERT_EQ(-5.0f, results[index++]) << error_message;
      ASSERT_EQ(5.0f, results[index++]) << error_message;
    }
    // Vector2 results
    {
      std::array<cl::float2, n_2vectors> results;
      results2->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index++].y) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index++].y) << error_message;
    }
    // Vector3 results
    {
      std::array<cl::float3, n_3vectors> results;
      results3->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index].y) << error_message;
      ASSERT_EQ(-9.0f, results[index++].z) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index].y) << error_message;
      ASSERT_EQ(9.0f, results[index++].z) << error_message;
    }
    // Vector4 results
    {
      std::array<cl::float4, n_4vectors> results;
      results4->read(results.data(), results.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(-5.0f, results[index].x) << error_message;
      ASSERT_EQ(-2.0f, results[index].y) << error_message;
      ASSERT_EQ(-9.0f, results[index].z) << error_message;
      ASSERT_EQ(-15.0f, results[index++].w) << error_message;
      ASSERT_EQ(5.0f, results[index].x) << error_message;
      ASSERT_EQ(2.0f, results[index].y) << error_message;
      ASSERT_EQ(9.0f, results[index].z) << error_message;
      ASSERT_EQ(15.0f, results[index++].w) << error_message;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

