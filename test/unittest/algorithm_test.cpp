/*!
  \file algorithm_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <bitset>
#include <iomanip>
#include <limits>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/algorithm.hpp"
// Test
#include "test.hpp"

TEST(AlgorithmTest, AbsInt8Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<uint8b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(5);

    auto result2_buff = makeStorageBuffer<cl::uchar2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(3);
    auto result3_buff = makeStorageBuffer<cl::uchar3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(3);
    auto result4_buff = makeStorageBuffer<cl::uchar4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(3);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testAbsInt8));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'abs' func is wrong.";
    constexpr auto maxu = zisc::cast<uint8b>(std::numeric_limits<int8b>::max());
    constexpr auto minu = zisc::cast<uint8b>(std::numeric_limits<int8b>::min());
    {
      std::vector<uint8b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(maxu, results[i++]) << error_message; 
      ASSERT_EQ(minu, results[i++]) << error_message; 
    }
    {
      std::vector<cl::uchar2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i++].y) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i++].y) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::uchar3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i].y) << error_message; 
      ASSERT_EQ(minu, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::uchar4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i].y) << error_message; 
      ASSERT_EQ(minu, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, AbsInt16Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<uint16b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(5);

    auto result2_buff = makeStorageBuffer<cl::ushort2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(3);
    auto result3_buff = makeStorageBuffer<cl::ushort3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(3);
    auto result4_buff = makeStorageBuffer<cl::ushort4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(3);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testAbsInt16));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'abs' func is wrong.";
    constexpr auto maxu = zisc::cast<uint16b>(std::numeric_limits<int16b>::max());
    constexpr auto minu = zisc::cast<uint16b>(std::numeric_limits<int16b>::min());
    {
      std::vector<uint16b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(maxu, results[i++]) << error_message; 
      ASSERT_EQ(minu, results[i++]) << error_message; 
    }
    {
      std::vector<cl::ushort2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i++].y) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i++].y) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::ushort3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i].y) << error_message; 
      ASSERT_EQ(minu, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::ushort4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i].y) << error_message; 
      ASSERT_EQ(minu, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, AbsInt32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(5);

    auto result2_buff = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(3);
    auto result3_buff = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(3);
    auto result4_buff = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(3);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testAbsInt32));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'abs' func is wrong.";
    constexpr auto maxu = zisc::cast<uint32b>(std::numeric_limits<int32b>::max());
    constexpr auto minu = zisc::cast<uint32b>(std::numeric_limits<int32b>::min());
    {
      std::vector<uint32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(maxu, results[i++]) << error_message; 
      ASSERT_EQ(minu, results[i++]) << error_message; 
    }
    {
      std::vector<cl::uint2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i++].y) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i++].y) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::uint3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i].y) << error_message; 
      ASSERT_EQ(minu, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::uint4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(minu, results[i].y) << error_message; 
      ASSERT_EQ(minu, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, AbsUInt32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(3);
    auto result3_buff = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(3);
    auto result4_buff = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(3);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testAbsUInt32));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'abs' func is wrong.";
    constexpr auto maxu = std::numeric_limits<uint32b>::max();
    {
      std::vector<uint32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(maxu, results[i++]) << error_message; 
      ASSERT_EQ(0u, results[i++]) << error_message; 
    }
    {
      std::vector<cl::uint2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i++].y) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i++].y) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::uint3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::uint4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
      ASSERT_EQ(maxu, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(0u, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, AbsFloatTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(9);

    auto result2_buff = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(3);
    auto result3_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(3);
    auto result4_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(3);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testAbsFloat));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'abs' func is wrong.";
    constexpr auto maxf = std::numeric_limits<float>::max();
    constexpr auto minf = std::numeric_limits<float>::min();
    constexpr auto inff = std::numeric_limits<float>::infinity();
    constexpr auto ef = std::numeric_limits<float>::epsilon();
    {
      std::vector<float> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(maxf, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(maxf, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(minf, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(minf, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(inff, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(inff, results[i++]) << error_message; 
    }
    {
      std::vector<cl::float2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(maxf, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(maxf, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::float3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(ef, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(maxf, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(minf, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(maxf, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::float4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(ef, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(ef, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(inff, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(inff, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(maxf, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(minf, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(maxf, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, ClampInt8Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int8b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(12);

    auto result2_buff = makeStorageBuffer<cl::char2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(4);
    auto result3_buff = makeStorageBuffer<cl::char3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(4);
    auto result4_buff = makeStorageBuffer<cl::char4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(4);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testClampInt8));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clamp' func is wrong.";
    {
      std::vector<int8b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i++]) << error_message; 
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(0, results[i++]) << error_message; 
      ASSERT_EQ(-10, results[i++]) << error_message; 
      ASSERT_EQ(10, results[i++]) << error_message; 
    }
    {
      std::vector<cl::char2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i++].y) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i++].y) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i++].y) << error_message; 
      ASSERT_EQ(10, results[i].x) << error_message; 
      ASSERT_EQ(-10, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::char3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i].y) << error_message; 
      ASSERT_EQ(-1, results[i++].z) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i].y) << error_message; 
      ASSERT_EQ(20, results[i++].z) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i].y) << error_message; 
      ASSERT_EQ(-20, results[i++].z) << error_message; 
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(10, results[i].y) << error_message; 
      ASSERT_EQ(-10, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::char4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i].y) << error_message; 
      ASSERT_EQ(-1, results[i].z) << error_message; 
      ASSERT_EQ(-1, results[i++].w) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i].y) << error_message; 
      ASSERT_EQ(20, results[i].z) << error_message; 
      ASSERT_EQ(20, results[i++].w) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i].y) << error_message; 
      ASSERT_EQ(-20, results[i].z) << error_message; 
      ASSERT_EQ(-20, results[i++].w) << error_message; 
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(10, results[i].y) << error_message; 
      ASSERT_EQ(-10, results[i].z) << error_message; 
      ASSERT_EQ(std::numeric_limits<int8b>::min(), results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, ClampInt16Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int16b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(12);

    auto result2_buff = makeStorageBuffer<cl::short2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(4);
    auto result3_buff = makeStorageBuffer<cl::short3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(4);
    auto result4_buff = makeStorageBuffer<cl::short4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(4);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testClampInt16));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clamp' func is wrong.";
    {
      std::vector<int16b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i++]) << error_message; 
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(0, results[i++]) << error_message; 
      ASSERT_EQ(-10, results[i++]) << error_message; 
      ASSERT_EQ(10, results[i++]) << error_message; 
    }
    {
      std::vector<cl::short2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i++].y) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i++].y) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i++].y) << error_message; 
      ASSERT_EQ(10, results[i].x) << error_message; 
      ASSERT_EQ(-10, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::short3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i].y) << error_message; 
      ASSERT_EQ(-1, results[i++].z) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i].y) << error_message; 
      ASSERT_EQ(20, results[i++].z) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i].y) << error_message; 
      ASSERT_EQ(-20, results[i++].z) << error_message; 
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(10, results[i].y) << error_message; 
      ASSERT_EQ(-10, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::short4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i].y) << error_message; 
      ASSERT_EQ(-1, results[i].z) << error_message; 
      ASSERT_EQ(-1, results[i++].w) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i].y) << error_message; 
      ASSERT_EQ(20, results[i].z) << error_message; 
      ASSERT_EQ(20, results[i++].w) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i].y) << error_message; 
      ASSERT_EQ(-20, results[i].z) << error_message; 
      ASSERT_EQ(-20, results[i++].w) << error_message; 
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(10, results[i].y) << error_message; 
      ASSERT_EQ(-10, results[i].z) << error_message; 
      ASSERT_EQ(std::numeric_limits<int16b>::min(), results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, ClampInt32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(12);

    auto result2_buff = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(4);
    auto result3_buff = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(4);
    auto result4_buff = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(4);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testClampInt32));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clamp' func is wrong.";
    {
      std::vector<int32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i++]) << error_message; 
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(-20, results[i++]) << error_message; 
      ASSERT_EQ(0, results[i++]) << error_message; 
      ASSERT_EQ(-10, results[i++]) << error_message; 
      ASSERT_EQ(10, results[i++]) << error_message; 
    }
    {
      std::vector<cl::int2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i++].y) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i++].y) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i++].y) << error_message; 
      ASSERT_EQ(10, results[i].x) << error_message; 
      ASSERT_EQ(-10, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::int3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i].y) << error_message; 
      ASSERT_EQ(-1, results[i++].z) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i].y) << error_message; 
      ASSERT_EQ(20, results[i++].z) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i].y) << error_message; 
      ASSERT_EQ(-20, results[i++].z) << error_message; 
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(10, results[i].y) << error_message; 
      ASSERT_EQ(-10, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::int4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i].y) << error_message; 
      ASSERT_EQ(-1, results[i].z) << error_message; 
      ASSERT_EQ(-1, results[i++].w) << error_message; 
      ASSERT_EQ(20, results[i].x) << error_message; 
      ASSERT_EQ(20, results[i].y) << error_message; 
      ASSERT_EQ(20, results[i].z) << error_message; 
      ASSERT_EQ(20, results[i++].w) << error_message; 
      ASSERT_EQ(-20, results[i].x) << error_message; 
      ASSERT_EQ(-20, results[i].y) << error_message; 
      ASSERT_EQ(-20, results[i].z) << error_message; 
      ASSERT_EQ(-20, results[i++].w) << error_message; 
      ASSERT_EQ(0, results[i].x) << error_message; 
      ASSERT_EQ(10, results[i].y) << error_message; 
      ASSERT_EQ(-10, results[i].z) << error_message; 
      ASSERT_EQ(std::numeric_limits<int32b>::min(), results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, ClampUInt32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(6);

    auto result2_buff = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(3);
    auto result3_buff = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(3);
    auto result4_buff = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(3);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testClampUInt32));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clamp' func is wrong.";
    {
      std::vector<uint32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(20u, results[i++]) << error_message; 
      ASSERT_EQ(20u, results[i++]) << error_message; 
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(10u, results[i++]) << error_message; 
    }
    {
      std::vector<cl::uint2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i++].y) << error_message; 
      ASSERT_EQ(20u, results[i].x) << error_message; 
      ASSERT_EQ(20u, results[i++].y) << error_message; 
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(10u, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::uint3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(10u, results[i].y) << error_message; 
      ASSERT_EQ(10u, results[i++].z) << error_message; 
      ASSERT_EQ(20u, results[i].x) << error_message; 
      ASSERT_EQ(20u, results[i].y) << error_message; 
      ASSERT_EQ(20u, results[i++].z) << error_message; 
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(10u, results[i].y) << error_message; 
      ASSERT_EQ(std::numeric_limits<uint32b>::max(), results[i++].z) << error_message; 
    }
    {
      std::vector<cl::uint4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(1u, results[i].z) << error_message; 
      ASSERT_EQ(1u, results[i++].w) << error_message; 
      ASSERT_EQ(20u, results[i].x) << error_message; 
      ASSERT_EQ(20u, results[i].y) << error_message; 
      ASSERT_EQ(20u, results[i].z) << error_message; 
      ASSERT_EQ(20u, results[i++].w) << error_message; 
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(10u, results[i].y) << error_message; 
      ASSERT_EQ(std::numeric_limits<uint32b>::max(), results[i].z) << error_message; 
      ASSERT_EQ(100u, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, ClampFloatTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(12);

    auto result2_buff = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(4);
    auto result3_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(4);
    auto result4_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(5);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testClampFloat));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'clamp' func is wrong.";
    {
      std::vector<float> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-10.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(10.0f, results[i++]) << error_message; 
    }
    {
      std::vector<cl::float2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(10.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-10.0f, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::float3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(10.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-10.0f, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::float4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(std::numeric_limits<float>::min(), results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(20.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(-20.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(10.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-10.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(std::numeric_limits<float>::min(), results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(10.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-10.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(std::numeric_limits<float>::min(), results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MaxInt8Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int8b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::char2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::char3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::char4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMaxInt8));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    constexpr auto maxi = std::numeric_limits<int8b>::max();
    {
      std::vector<int8b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(maxi, results[i++]) << error_message; 
      ASSERT_EQ(maxi, results[i++]) << error_message; 
    }
    {
      std::vector<cl::char2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i++].y) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(maxi, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::char3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(1, results[i++].z) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(maxi, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::char4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(1, results[i].z) << error_message; 
      ASSERT_EQ(0, results[i++].w) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(maxi, results[i].z) << error_message; 
      ASSERT_EQ(maxi, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MaxInt16Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int16b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::short2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::short3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::short4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMaxInt16));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    constexpr auto maxi = std::numeric_limits<int16b>::max();
    {
      std::vector<int16b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(maxi, results[i++]) << error_message; 
      ASSERT_EQ(maxi, results[i++]) << error_message; 
    }
    {
      std::vector<cl::short2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i++].y) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(maxi, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::short3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(1, results[i++].z) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(maxi, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::short4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(1, results[i].z) << error_message; 
      ASSERT_EQ(0, results[i++].w) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(maxi, results[i].z) << error_message; 
      ASSERT_EQ(maxi, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MaxInt32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMaxInt32));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    constexpr auto maxi = std::numeric_limits<int32b>::max();
    {
      std::vector<int32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(maxi, results[i++]) << error_message; 
      ASSERT_EQ(maxi, results[i++]) << error_message; 
    }
    {
      std::vector<cl::int2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(1, results[i++].y) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(maxi, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::int3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(1, results[i++].z) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(maxi, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::int4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(1, results[i].z) << error_message; 
      ASSERT_EQ(0, results[i++].w) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(0, results[i].y) << error_message; 
      ASSERT_EQ(maxi, results[i].z) << error_message; 
      ASSERT_EQ(maxi, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MaxUInt32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMaxUInt32));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    constexpr auto maxi = std::numeric_limits<uint32b>::max();
    {
      std::vector<uint32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(maxi, results[i++]) << error_message; 
      ASSERT_EQ(maxi, results[i++]) << error_message; 
    }
    {
      std::vector<cl::uint2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i++].y) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(maxi, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::uint3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(1u, results[i].y) << error_message; 
      ASSERT_EQ(1u, results[i++].z) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(maxi, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::uint4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(1u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(1u, results[i].z) << error_message; 
      ASSERT_EQ(1u, results[i++].w) << error_message; 
      ASSERT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(maxi, results[i].z) << error_message; 
      ASSERT_EQ(maxi, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MaxFloatTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMaxFloat));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    constexpr auto maxi = std::numeric_limits<float>::max();
    {
      std::vector<float> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i++]) << error_message; 
    }
    {
      std::vector<cl::float2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::float3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::float4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(maxi, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MinInt8Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int8b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::char2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::char3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::char4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMinInt8));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'min' func is wrong.";
    constexpr auto mini = std::numeric_limits<int8b>::min();
    {
      std::vector<int8b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(mini, results[i++]) << error_message; 
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(mini, results[i++]) << error_message; 
    }
    {
      std::vector<cl::char2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i++].y) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::char3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i++].z) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::char4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i].z) << error_message; 
      ASSERT_EQ(0, results[i++].w) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i].z) << error_message; 
      ASSERT_EQ(mini, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MinInt16Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int16b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::short2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::short3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::short4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMinInt16));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'min' func is wrong.";
    constexpr auto mini = std::numeric_limits<int16b>::min();
    {
      std::vector<int16b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(mini, results[i++]) << error_message; 
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(mini, results[i++]) << error_message; 
    }
    {
      std::vector<cl::short2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i++].y) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::short3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i++].z) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::short4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i].z) << error_message; 
      ASSERT_EQ(0, results[i++].w) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i].z) << error_message; 
      ASSERT_EQ(mini, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MinInt32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMinInt32));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'min' func is wrong.";
    constexpr auto mini = std::numeric_limits<int32b>::min();
    {
      std::vector<int32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i++]) << error_message; 
      ASSERT_EQ(mini, results[i++]) << error_message; 
      ASSERT_EQ(1, results[i++]) << error_message; 
      ASSERT_EQ(mini, results[i++]) << error_message; 
    }
    {
      std::vector<cl::int2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i++].y) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::int3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i++].z) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::int4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(-1, results[i].x) << error_message; 
      ASSERT_EQ(-1, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i].z) << error_message; 
      ASSERT_EQ(0, results[i++].w) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i].y) << error_message; 
      ASSERT_EQ(0, results[i].z) << error_message; 
      ASSERT_EQ(mini, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MinUInt32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMinUInt32));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'max' func is wrong.";
    constexpr auto mini = std::numeric_limits<uint32b>::min();
    {
      std::vector<uint32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(0u, results[i++]) << error_message; 
      ASSERT_EQ(1u, results[i++]) << error_message; 
      ASSERT_EQ(mini, results[i++]) << error_message; 
    }
    {
      std::vector<cl::uint2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i++].y) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::uint3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::uint4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_EQ(0u, results[i].x) << error_message; 
      ASSERT_EQ(0u, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(1u, results[i++].w) << error_message; 
      ASSERT_EQ(mini, results[i].x) << error_message; 
      ASSERT_EQ(mini, results[i].y) << error_message; 
      ASSERT_EQ(0u, results[i].z) << error_message; 
      ASSERT_EQ(mini, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MinFloatTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(4);

    auto result2_buff = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(2);
    auto result3_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(2);
    auto result4_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMinFloat));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'min' func is wrong.";
    constexpr auto mini = std::numeric_limits<float>::lowest();
    {
      std::vector<float> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i++]) << error_message; 
    }
    {
      std::vector<cl::float2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::float3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::float4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(mini, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MixScalarTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(8);

    auto result2_buff = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(4);
    auto result3_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(4);
    auto result4_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(4);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMixScalar));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'mix' func is wrong.";
    {
      std::vector<float> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(0.4f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-0.4f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++]) << error_message; 
    }
    {
      std::vector<cl::float2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(0.4f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-0.4f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::float3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-100.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(0.4f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-0.4f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(40.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(100.0f, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::float4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-100.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(100.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(0.4f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-0.4f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(40.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(-40.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(100.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(-100.0f, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, MixTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(2);

    auto result2_buff = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(1);
    auto result3_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(1);
    auto result4_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(1);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testMix));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'mix' func is wrong.";
    {
      std::vector<float> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message; 
    }
    {
      std::vector<cl::float2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-0.4f, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::float3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(-0.4f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(100.0f, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::float4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(40.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(-100.0f, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(AlgorithmTest, SignTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto result1_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(7);

    auto result2_buff = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(4);
    auto result3_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(3);
    auto result4_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(2);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testSign));
    kernel->run(*result1_buff, *result2_buff, *result3_buff, *result4_buff, {1}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'sign' func is wrong.";
    {
      std::vector<float> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++]) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message; 
    }
    {
      std::vector<cl::float2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].y) << error_message; 
    }
    {
      std::vector<cl::float3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].z) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].z) << error_message; 
    }
    {
      std::vector<cl::float4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i++].w) << error_message; 
      ASSERT_FLOAT_EQ(-1.0f, results[i].x) << error_message; 
      ASSERT_FLOAT_EQ(1.0f, results[i].y) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i].z) << error_message; 
      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message; 
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

//TEST(AlgorithmTest, Popcount8Test)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b bit_size = 8 * sizeof(uint8b);
//    constexpr uint32b resolution = 2 * bit_size + bit_size / 2; 
//
//    std::vector<uint8b> inputs;
//    inputs.resize(resolution);
//    for (uint32b i = 0; i < inputs.size(); ++i) {
//      if (i < bit_size) {
//        uint8b input = 0b1;
//        for (uint32b j = 0; j < i; ++j)
//          input = (input << 1) | input;
//        inputs[i] = input;
//      }
//      else if (i < 2 * bit_size) {
//        uint8b input = 0b1u << (bit_size - 1);
//        for (uint32b j = 0; j < (i - bit_size); ++j)
//          input = (input >> 1) | input;
//        inputs[i] = input;
//      }
//      else {
//        uint8b input = 0b1u;
//        for (uint32b j = 0; j < (i - (2 * bit_size)); ++j)
//          input = (input << 2) | input;
//        inputs[i] = input;
//      }
//    }
//
//    auto input_buff = makeStorageBuffer<uint8b>(device.get(), BufferUsage::kDeviceOnly);
//    input_buff->setSize(resolution);
//    input_buff->write(inputs.data(), inputs.size(), 0, 0);
//    auto result1_buff = makeStorageBuffer<uint8b>(device.get(), BufferUsage::kDeviceOnly);
//    result1_buff->setSize(resolution);
//
//    auto result2_buff = makeStorageBuffer<cl::uchar2>(device.get(), BufferUsage::kDeviceOnly);
//    result2_buff->setSize(resolution);
//    auto result3_buff = makeStorageBuffer<cl::uchar3>(device.get(), BufferUsage::kDeviceOnly);
//    result3_buff->setSize(resolution);
//    auto result4_buff = makeStorageBuffer<cl::uchar4>(device.get(), BufferUsage::kDeviceOnly);
//    result4_buff->setSize(resolution);
//    auto resolution_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
//    resolution_buff->setSize(1);
//    resolution_buff->write(&resolution, 1, 0, 0);
//
//    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testPopcount8));
//    kernel->run(*input_buff, *result1_buff, *result2_buff, *result3_buff, *result4_buff, *resolution_buff, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'popcount' func is wrong.";
//    {
//      std::vector<uint8b> results;
//      results.resize(result1_buff->size());
//      result1_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint8b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        EXPECT_EQ(expected, result) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::uchar2> results;
//      results.resize(result2_buff->size());
//      result2_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint8b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 2; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::uchar3> results;
//      results.resize(result3_buff->size());
//      result3_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint8b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 3; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::uchar4> results;
//      results.resize(result4_buff->size());
//      result4_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint8b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 4; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}

//TEST(AlgorithmTest, Popcount16Test)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b bit_size = 8 * sizeof(uint16b);
//    constexpr uint32b resolution = 2 * bit_size + bit_size / 2; 
//
//    std::vector<uint16b> inputs;
//    inputs.resize(resolution);
//    for (uint32b i = 0; i < inputs.size(); ++i) {
//      if (i < bit_size) {
//        uint16b input = 0b1;
//        for (uint32b j = 0; j < i; ++j)
//          input = (input << 1) | input;
//        inputs[i] = input;
//      }
//      else if (i < 2 * bit_size) {
//        uint16b input = 0b1u << (bit_size - 1);
//        for (uint32b j = 0; j < (i - bit_size); ++j)
//          input = (input >> 1) | input;
//        inputs[i] = input;
//      }
//      else {
//        uint16b input = 0b1u;
//        for (uint32b j = 0; j < (i - (2 * bit_size)); ++j)
//          input = (input << 2) | input;
//        inputs[i] = input;
//      }
//    }
//
//    auto input_buff = makeStorageBuffer<uint16b>(device.get(), BufferUsage::kDeviceOnly);
//    input_buff->setSize(resolution);
//    input_buff->write(inputs.data(), inputs.size(), 0, 0);
//    auto result1_buff = makeStorageBuffer<uint16b>(device.get(), BufferUsage::kDeviceOnly);
//    result1_buff->setSize(resolution);
//
//    auto result2_buff = makeStorageBuffer<cl::ushort2>(device.get(), BufferUsage::kDeviceOnly);
//    result2_buff->setSize(resolution);
//    auto result3_buff = makeStorageBuffer<cl::ushort3>(device.get(), BufferUsage::kDeviceOnly);
//    result3_buff->setSize(resolution);
//    auto result4_buff = makeStorageBuffer<cl::ushort4>(device.get(), BufferUsage::kDeviceOnly);
//    result4_buff->setSize(resolution);
//    auto resolution_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
//    resolution_buff->setSize(1);
//    resolution_buff->write(&resolution, 1, 0, 0);
//
//    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testPopcount16));
//    kernel->run(*input_buff, *result1_buff, *result2_buff, *result3_buff, *result4_buff, *resolution_buff, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'popcount' func is wrong.";
//    {
//      std::vector<uint16b> results;
//      results.resize(result1_buff->size());
//      result1_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint16b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        EXPECT_EQ(expected, result) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::ushort2> results;
//      results.resize(result2_buff->size());
//      result2_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint16b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 2; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::ushort3> results;
//      results.resize(result3_buff->size());
//      result3_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint16b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 3; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::ushort4> results;
//      results.resize(result4_buff->size());
//      result4_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint16b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 4; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}

TEST(AlgorithmTest, Popcount32Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b bit_size = 8 * sizeof(uint32b);
    constexpr uint32b resolution = 2 * bit_size + bit_size / 2; 

    std::vector<uint32b> inputs;
    inputs.resize(resolution);
    for (uint32b i = 0; i < inputs.size(); ++i) {
      if (i < bit_size) {
        uint32b input = 0b1;
        for (uint32b j = 0; j < i; ++j)
          input = (input << 1) | input;
        inputs[i] = input;
      }
      else if (i < 2 * bit_size) {
        uint32b input = 0b1u << (bit_size - 1);
        for (uint32b j = 0; j < (i - bit_size); ++j)
          input = (input >> 1) | input;
        inputs[i] = input;
      }
      else {
        uint32b input = 0b1u;
        for (uint32b j = 0; j < (i - (2 * bit_size)); ++j)
          input = (input << 2) | input;
        inputs[i] = input;
      }
    }

    auto input_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(resolution);
    input_buff->write(inputs.data(), inputs.size(), 0, 0);
    auto result1_buff = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    result1_buff->setSize(resolution);

    auto result2_buff = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceOnly);
    result2_buff->setSize(resolution);
    auto result3_buff = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceOnly);
    result3_buff->setSize(resolution);
    auto result4_buff = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceOnly);
    result4_buff->setSize(resolution);
    auto resolution_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    resolution_buff->setSize(1);
    resolution_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testPopcount32));
    kernel->run(*input_buff, *result1_buff, *result2_buff, *result3_buff, *result4_buff, *resolution_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'popcount' func is wrong.";
    {
      std::vector<uint32b> results;
      results.resize(result1_buff->size());
      result1_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto input = inputs[i];
        const auto expected = zisc::cast<uint32b>(std::bitset<bit_size>{input}.count());
        const auto result = results[i];
        EXPECT_EQ(expected, result) << error_message << ": " << std::hex << input;
      }
    }
    {
      std::vector<cl::uint2> results;
      results.resize(result2_buff->size());
      result2_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto input = inputs[i];
        const auto expected = zisc::cast<uint32b>(std::bitset<bit_size>{input}.count());
        const auto result = results[i];
        for (uint32b j = 0; j < 2; ++j)
          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
      }
    }
    {
      std::vector<cl::uint3> results;
      results.resize(result3_buff->size());
      result3_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto input = inputs[i];
        const auto expected = zisc::cast<uint32b>(std::bitset<bit_size>{input}.count());
        const auto result = results[i];
        for (uint32b j = 0; j < 3; ++j)
          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
      }
    }
    {
      std::vector<cl::uint4> results;
      results.resize(result4_buff->size());
      result4_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto input = inputs[i];
        const auto expected = zisc::cast<uint32b>(std::bitset<bit_size>{input}.count());
        const auto result = results[i];
        for (uint32b j = 0; j < 4; ++j)
          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

//#if !defined(Z_MAC)
//
//TEST(AlgorithmTest, Popcount64Test)
//{
//  using namespace zinvul;
//
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b bit_size = 8 * sizeof(uint64b);
//    constexpr uint32b resolution = 2 * bit_size + bit_size / 2; 
//
//    std::vector<uint64b> inputs;
//    inputs.resize(resolution);
//    for (uint32b i = 0; i < inputs.size(); ++i) {
//      if (i < bit_size) {
//        uint64b input = 0b1;
//        for (uint32b j = 0; j < i; ++j)
//          input = (input << 1) | input;
//        inputs[i] = input;
//      }
//      else if (i < 2 * bit_size) {
//        uint64b input = zisc::cast<uint64b>(0b1u) << (bit_size - 1);
//        for (uint32b j = 0; j < (i - bit_size); ++j)
//          input = (input >> 1) | input;
//        inputs[i] = input;
//      }
//      else {
//        uint64b input = 0b1u;
//        for (uint32b j = 0; j < (i - (2 * bit_size)); ++j)
//          input = (input << 2) | input;
//        inputs[i] = input;
//      }
//    }
//
//    auto input_buff = makeStorageBuffer<uint64b>(device.get(), BufferUsage::kDeviceOnly);
//    input_buff->setSize(resolution);
//    input_buff->write(inputs.data(), inputs.size(), 0, 0);
//    auto result1_buff = makeStorageBuffer<uint64b>(device.get(), BufferUsage::kDeviceOnly);
//    result1_buff->setSize(resolution);
//
//    auto result2_buff = makeStorageBuffer<cl::ulong2>(device.get(), BufferUsage::kDeviceOnly);
//    result2_buff->setSize(resolution);
//    auto result3_buff = makeStorageBuffer<cl::ulong3>(device.get(), BufferUsage::kDeviceOnly);
//    result3_buff->setSize(resolution);
//    auto result4_buff = makeStorageBuffer<cl::ulong4>(device.get(), BufferUsage::kDeviceOnly);
//    result4_buff->setSize(resolution);
//    auto resolution_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
//    resolution_buff->setSize(1);
//    resolution_buff->write(&resolution, 1, 0, 0);
//
//    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(algorithm, testPopcount64));
//    kernel->run(*input_buff, *result1_buff, *result2_buff, *result3_buff, *result4_buff, *resolution_buff, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'popcount' func is wrong.";
//    {
//      std::vector<uint64b> results;
//      results.resize(result1_buff->size());
//      result1_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint64b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        EXPECT_EQ(expected, result) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::ulong2> results;
//      results.resize(result2_buff->size());
//      result2_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint64b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 2; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::ulong3> results;
//      results.resize(result3_buff->size());
//      result3_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint64b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 3; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//    {
//      std::vector<cl::ulong4> results;
//      results.resize(result4_buff->size());
//      result4_buff->read(results.data(), results.size(), 0, 0);
//
//      for (uint32b i = 0; i < inputs.size(); ++i) {
//        const auto input = inputs[i];
//        const auto expected = zisc::cast<uint64b>(std::bitset<bit_size>{input}.count());
//        const auto result = results[i];
//        for (uint32b j = 0; j < 4; ++j)
//          ASSERT_EQ(expected, result[j]) << error_message << ": " << std::hex << input;
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//#endif // Z_MAC
