/*!
  \file vector_data_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <bitset>
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
#include "zinvul/kernel_set/vector_data.hpp"
// Test
#include "test.hpp"

TEST(VectorDataTest, Int8bVectorDataTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector_data, testInt8bVectorData));
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

TEST(VectorDataTest, Int16bVectorDataTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector_data, testInt16bVectorData));
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

TEST(VectorDataTest, Int32bVectorDataTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector_data, testInt32bVectorData));
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

TEST(VectorDataTest, UInt32bVectorDataTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector_data, testUInt32bVectorData));
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

TEST(VectorDataTest, FloatVectorDataTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector_data, testFloatVectorData));
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

TEST(VectorDataTest, HalfVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
  std::cout << "## Warning: only CPU test will be done." << std::endl;
  const std::size_t number = 0; {
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector_data, testHalfVectorData));
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
  }
}

TEST(VectorDataTest, HalfUVectorDataTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr std::size_t n = 10;
    constexpr std::size_t n_special = 8;
    constexpr uint32b resolution = 65545 / n;

    auto buffer1 = makeStorageBuffer<cl::half>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(n * resolution + n_special);
    {
      std::vector<cl::half> inputs;
      inputs.resize(buffer1->size());
      std::size_t index = 0;
      for (std::size_t i = 0; i < resolution; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
          const std::size_t u = (n * (i / 4)) + j;
          float t = zisc::cast<float>(u);
          t = (1 < (i % 4)) ? 1.0f / t : t;
          t = ((i % 2) == 0) ? t : -t;
//          std::cout << "[" << index << "]: u = " << u
//                    << ", f = " << std::scientific << t << std::endl;
          const auto data = zisc::SingleFloat::fromFloat(t);
          inputs[index++] = zisc::HalfFloat{data};
        }
      }
      inputs[index++] = std::numeric_limits<zisc::HalfFloat>::max();
      inputs[index++] = std::numeric_limits<zisc::HalfFloat>::lowest();
      inputs[index++] = std::numeric_limits<zisc::HalfFloat>::min();
      inputs[index++] = -std::numeric_limits<zisc::HalfFloat>::min();
      inputs[index++] = std::numeric_limits<zisc::HalfFloat>::epsilon();
      inputs[index++] = std::numeric_limits<zisc::HalfFloat>::infinity();
      inputs[index++] = -std::numeric_limits<zisc::HalfFloat>::infinity();
      inputs[index++] = std::numeric_limits<zisc::HalfFloat>::quiet_NaN();
      buffer1->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto buffer2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(buffer1->size());

    auto buffer3 = makeStorageBuffer<uint16b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(buffer1->size());

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(vector_data, testHalfUVectorData));
    kernel->run(*(buffer1->treatAs<uint16b>()), *buffer2, *buffer3, *res_buff, {resolution}, 0);
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
          float expected = zisc::cast<float>(u);
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
            ASSERT_FALSE(is_subnormal_half) << "Subnormal: "
                << " [" << index << "]: " << std::scientific << result;
            ASSERT_FLOAT_EQ(expected, result) << error_message
                << " [" << index << "]: " << std::scientific << result;
          }
          {
            expected = 3.0f * expected;
            std::bitset<16> expectedbits;
            {
              auto data = zisc::SingleFloat::fromFloat(expected);
              zisc::HalfFloat hdata{data};
              expectedbits = hdata.bits();
              data = zisc::HalfFloat{data};
              expected = data.toFloat();
            }
            float result = 0.0f;
            std::bitset<16> bits;
            {
              const zisc::HalfFloat hdata{results[index]};
              bits = hdata.bits();
              const zisc::SingleFloat data{hdata};
              result = data.toFloat();
            }
            ASSERT_FLOAT_EQ(expected, result) << error_message
                << " [" << index << "]: " << std::scientific << result << std::endl
                << "  expected    : " << std::scientific << expected
                << " (" << expectedbits << ")" << std::endl
                << "  result      : " << std::scientific << result
                << " (" <<  bits << ")";
          }
          ++index;
        }
      }
      {
        const auto expected = std::numeric_limits<zisc::HalfFloat>::max();
        const zisc::HalfFloat result{results[index++]};
        const std::bitset<16> expectedbits{expected.bits()};
        const std::bitset<16> bits{result.bits()};
        ASSERT_EQ(expected, result) << error_message
            << "expected bits: " << expectedbits << std::endl
            << "resultbits   : " << bits;
      }
      {
        const auto expected = std::numeric_limits<zisc::HalfFloat>::lowest();
        const zisc::HalfFloat result{results[index++]};
        const std::bitset<16> expectedbits{expected.bits()};
        const std::bitset<16> bits{result.bits()};
        ASSERT_EQ(expected, result) << error_message
            << "expected bits: " << expectedbits << std::endl
            << "resultbits   : " << bits;
      }
      {
        const auto expected = std::numeric_limits<zisc::HalfFloat>::min();
        const zisc::HalfFloat result{results[index++]};
        const std::bitset<16> expectedbits{expected.bits()};
        const std::bitset<16> bits{result.bits()};
        ASSERT_EQ(expected, result) << error_message
            << "expected bits: " << expectedbits << std::endl
            << "resultbits   : " << bits;
      }
      {
        const auto expected = -std::numeric_limits<zisc::HalfFloat>::min();
        const zisc::HalfFloat result{results[index++]};
        const std::bitset<16> expectedbits{expected.bits()};
        const std::bitset<16> bits{result.bits()};
        ASSERT_EQ(expected, result) << error_message
            << "expected bits: " << expectedbits << std::endl
            << "resultbits   : " << bits;
      }
      {
        const auto expected = std::numeric_limits<zisc::HalfFloat>::epsilon();
        const zisc::HalfFloat result{results[index++]};
        const std::bitset<16> expectedbits{expected.bits()};
        const std::bitset<16> bits{result.bits()};
        ASSERT_EQ(expected, result) << error_message
            << "expected bits: " << expectedbits << std::endl
            << "resultbits   : " << bits;
      }
      {
        const zisc::HalfFloat result{results[index++]};
        const std::bitset<16> bits{result.bits()};
        ASSERT_TRUE(result.isInf()) << error_message
            << "resultbits   : " << bits;
      }
      {
        const zisc::HalfFloat result{results[index++]};
        const std::bitset<16> bits{result.bits()};
        ASSERT_TRUE(result.isInf()) << error_message
            << "resultbits   : " << bits;
      }
      {
        const zisc::HalfFloat result{results[index++]};
        const std::bitset<16> bits{result.bits()};
        ASSERT_TRUE(result.isNan()) << error_message
            << "resultbits   : " << bits;
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
