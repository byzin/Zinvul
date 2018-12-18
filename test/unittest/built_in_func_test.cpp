/*!
  \file built_in_func_test.cpp
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
      EXPECT_EQ(resolution, -result) << "The 'atomic_min' is wrong.";
    }
    // atomic_max
    {
      int32b result;
      max_result->read(&result, 1, 0, 0);
      EXPECT_EQ(resolution, result) << "The 'atomic_max' is wrong.";
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

    constexpr std::size_t n_scalars = 26;
    constexpr std::size_t n_vectors = 11;
    constexpr std::size_t n_vectors2 = 1;
    auto scalar_results =
        makeBuffer<int32b>(device.get(), BufferUsage::kDeviceSrc);
    scalar_results->setSize(n_scalars);
    auto vector_results =
        makeBuffer<cl::int4>(device.get(), BufferUsage::kDeviceSrc);
    vector_results->setSize(n_vectors);
    auto vector_results2 =
        makeBuffer<cl::float4>(device.get(), BufferUsage::kDeviceSrc);
    vector_results2->setSize(n_vectors2);

    auto kernel = makeZinvulKernel(device.get(), built_in_func, testRelational, 1);
    kernel->run(*scalar_results, *vector_results, *vector_results2, {1}, 0);
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
      ASSERT_EQ(2, result[index++]) << "The select func is wrong.";
      ASSERT_EQ(-2, result[index++]) << "The select func is wrong.";
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
    // Vector result2
    {
      std::array<cl::float4, n_vectors2> result;
      vector_results2->read(result.data(), result.size(), 0, 0);
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
