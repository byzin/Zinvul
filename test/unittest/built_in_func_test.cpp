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

TEST(BuiltInFuncTest, WorkSize1dTest)
{
  using namespace zinvul;

  constexpr uint resolution = 500;
  constexpr uint num_of_funcs = 8;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(num_of_funcs * resolution);
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
    buffer2->setSize(1);
    buffer2->write(&resolution, buffer2->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, test1dWorkSize, 1);
    kernel->run(*buffer1, *buffer2, {resolution}, 0);
    device->waitForCompletion();

    std::array<uint32b, num_of_funcs * resolution> result;
    buffer1->read(result.data(), result.size(), 0, 0);

    {
      const std::size_t expected = device->subgroupSize();
      const std::size_t subgroup_x = result[5];
      std::cout << "  Device LocalSize[x]: " << result[5] << std::endl;
      ASSERT_EQ(expected, subgroup_x)
          << "The subgroup sizes are different on between host and device.";
    }
    for (std::size_t index = 0; index < resolution; ++index) {
      ASSERT_EQ(index, result[index * num_of_funcs])
          << "[" << index << "]: "
          << "The 'getGlobalIdX()' func doesn't work.";
      ASSERT_EQ(resolution, result[index * num_of_funcs + 7])
          << "[" << index << "]: "
          << "The resolution is wrong.";
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, WorkSize2dTest)
{
  using namespace zinvul;

  const cl::uint2 resolution{500, 500};
  constexpr uint num_of_funcs = 16;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceSrc);
    buffer1->setSize(num_of_funcs * resolution.x * resolution.y);
    auto buffer2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceDst);
    buffer2->setSize(1);
    buffer2->write(&resolution, buffer2->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, test2dWorkSize, 2);
    kernel->run(*buffer1, *buffer2, {resolution.x, resolution.y}, 0);
    device->waitForCompletion();

    std::vector<uint32b> result;
    result.resize(num_of_funcs * resolution.x * resolution.y);
    buffer1->read(result.data(), result.size(), 0, 0);

    {
      const std::size_t expected = device->subgroupSize();
      const std::size_t subgroup_x = result[10];
      const std::size_t subgroup_y = result[11];
      std::cout << "  Device LocalSize[x]: " << subgroup_x << std::endl;
      std::cout << "  Device LocalSize[y]: " << subgroup_y << std::endl;
      ASSERT_EQ(expected, subgroup_x * subgroup_y)
          << "The subgroup sizes are different on between host and device.";
    }
    for (std::size_t y = 0; y < resolution.y; ++y) {
      for (std::size_t x = 0; x < resolution.x; ++x) {
        const std::size_t index = x + y * resolution.x;
        ASSERT_EQ(x, result[index * num_of_funcs + 0])
            << "[" << x << "," << y << "]: "
            << "The 'getGlobalIdX()' func doesn't work.";
        ASSERT_EQ(y, result[index * num_of_funcs + 1])
            << "[" << x << "," << y << "]: "
            << "The 'getGlobalIdY()' func doesn't work.";
        ASSERT_EQ(resolution.x, result[index * num_of_funcs + 14])
            << "[" << x << "," << y << "]: "
            << "The resolution.x is wrong.";
        ASSERT_EQ(resolution.y, result[index * num_of_funcs + 15])
            << "[" << x << "," << y << "]: "
            << "The resolution.y is wrong.";
      }
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

TEST(BuiltInFuncTest, ZatomicAddPositiveTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicAddPositive, 1);
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

TEST(BuiltInFuncTest, ZatomicAddNegativeTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicAddNegative, 1);
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

TEST(BuiltInFuncTest, ZatomicAddUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicAddUint, 1);
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

TEST(BuiltInFuncTest, ZatomicSubPositiveTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicSubPositive, 1);
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

TEST(BuiltInFuncTest, ZatomicSubNegativeTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicSubNegative, 1);
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

TEST(BuiltInFuncTest, ZatomicSubUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicSubUint, 1);
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

TEST(BuiltInFuncTest, ZatomicXchgPositiveTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicXchgPositive, 1);
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

TEST(BuiltInFuncTest, ZatomicXchgNegativeTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicXchgNegative, 1);
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

TEST(BuiltInFuncTest, ZatomicXchgUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicXchgUint, 1);
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

TEST(BuiltInFuncTest, ZatomicIncTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicInc, 1);
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

TEST(BuiltInFuncTest, ZatomicIncUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicIncUint, 1);
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

TEST(BuiltInFuncTest, ZatomicDecTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicDec, 1);
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

TEST(BuiltInFuncTest, ZatomicDecUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicDecUint, 1);
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

TEST(BuiltInFuncTest, ZatomicCmpxchgPositiveTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicCmpxchgPositive, 1);
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

TEST(BuiltInFuncTest, ZatomicCmpxchgNegativeTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicCmpxchgNegative, 1);
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

TEST(BuiltInFuncTest, ZatomicCmpxchgUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicCmpxchgUint, 1);
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

TEST(BuiltInFuncTest, ZatomicMinPositiveTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicMinPositive, 1);
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

TEST(BuiltInFuncTest, ZatomicMinNegativeTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicMinNegative, 1);
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

TEST(BuiltInFuncTest, ZatomicMinUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicMinUint, 1);
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

TEST(BuiltInFuncTest, ZatomicMaxPositiveTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicMaxPositive, 1);
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

TEST(BuiltInFuncTest, ZatomicMaxNegativeTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicMaxNegative, 1);
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

TEST(BuiltInFuncTest, ZatomicMaxUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicMaxUint, 1);
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

TEST(BuiltInFuncTest, ZatomicAndTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicAnd, 1);
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

TEST(BuiltInFuncTest, ZatomicAndUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicAndUint, 1);
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

TEST(BuiltInFuncTest, ZatomicOrTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicOr, 1);
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

TEST(BuiltInFuncTest, ZatomicOrUintTest)
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

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZatomicOrUint, 1);
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

    constexpr std::size_t n_scalars = 2;
    constexpr std::size_t n_vectors = 1;
    auto scalar_results =
        makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    scalar_results->setSize(n_scalars);
    auto vector_results =
        makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    vector_results->setSize(n_vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testSelect, 1);
    kernel->run(*scalar_results, *vector_results, {1}, 0);
    device->waitForCompletion();

    // Scalar results
    {
      std::array<int32b, n_scalars> result;
      scalar_results->read(result.data(), result.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(2, result[index++]) << "The select func is wrong.";
      ASSERT_EQ(-2, result[index++]) << "The select func is wrong.";
    }
    // Vector results
    {
      std::array<cl::float4, n_vectors> result;
      vector_results->read(result.data(), result.size(), 0, 0);
      std::size_t index = 0;
      {
        const cl::float4 expected{1.0f, -1.0f, 1.0f, -1.0f};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i)
          ASSERT_EQ(expected[i], r[i]) << "The select func is wrong.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(BuiltInFuncTest, ZSelectTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n_scalars = 2;
    constexpr std::size_t n_vectors = 1;
    auto scalar_results =
        makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    scalar_results->setSize(n_scalars);
    auto vector_results =
        makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    vector_results->setSize(n_vectors);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testZSelect, 1);
    kernel->run(*scalar_results, *vector_results, {1}, 0);
    device->waitForCompletion();

    // Scalar results
    {
      std::array<int32b, n_scalars> result;
      scalar_results->read(result.data(), result.size(), 0, 0);
      std::size_t index = 0;
      ASSERT_EQ(2, result[index++]) << "The select func is wrong.";
      ASSERT_EQ(-2, result[index++]) << "The select func is wrong.";
    }
    // Vector results
    {
      std::array<cl::float4, n_vectors> result;
      vector_results->read(result.data(), result.size(), 0, 0);
      std::size_t index = 0;
      {
        const cl::float4 expected{1.0f, -1.0f, 1.0f, -1.0f};
        const auto r = result[index++];
        for (std::size_t i = 0; i < 4; ++i)
          ASSERT_EQ(expected[i], r[i]) << "The select func is wrong.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
