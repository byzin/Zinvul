/*!
  \file geometric_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <limits>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/geometric.hpp"
// Test
#include "test.hpp"

TEST(GeometricTest, Cross3Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;

    auto parameter_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    parameter_buff->setSize(2 * resolution);

    auto result_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(2 * resolution);

    auto res_buff= makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(geometric, testCross3));
    kernel->run(*parameter_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'cross' func is wrong.";
    {
      std::vector<cl::float3> parameters;
      parameters.resize(parameter_buff->size());
      parameter_buff->read(parameters.data(), parameters.size(), 0, 0);
      std::vector<cl::float3> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < resolution; ++i) {
        const auto& a = parameters[2 * i + 0];
        ASSERT_NE(0.0f, a.x);
        const auto& b = parameters[2 * i + 1];
        ASSERT_NE(0.0f, b.x);
        {
          const auto expected = cl::cross(a, b);
          const auto& result = results[2 * i + 0];
          for (uint32b j = 0; j < 3; ++j) {
            ASSERT_FLOAT_EQ(expected[j], result[j]) << j << " th: " << error_message
                << " [" << i << "] " << std::scientific
                << "a=(" << a.x << ", " << a.y << ", " << a.z << "), "
                << "b=(" << b.x << ", " << b.y << ", " << b.z << ").";
          }
        }
        {
          const auto expected = cl::cross(b, a);
          const auto& result = results[2 * i + 1];
          for (uint32b j = 0; j < 3; ++j) {
            ASSERT_FLOAT_EQ(expected[j], result[j]) << j << " th: " << error_message
                << " [" << i << "] " << std::scientific
                << "a=(" << a.x << ", " << a.y << ", " << a.z << "), "
                << "b=(" << b.x << ", " << b.y << ", " << b.z << ").";
          }
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(GeometricTest, Cross4Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;

    auto parameter_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    parameter_buff->setSize(2 * resolution);

    auto result_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(2 * resolution);

    auto res_buff= makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(geometric, testCross4));
    kernel->run(*parameter_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'cross' func is wrong.";
    {
      std::vector<cl::float4> parameters;
      parameters.resize(parameter_buff->size());
      parameter_buff->read(parameters.data(), parameters.size(), 0, 0);
      std::vector<cl::float4> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < resolution; ++i) {
        const auto& a = parameters[2 * i + 0];
        ASSERT_NE(0.0f, a.x);
        const auto& b = parameters[2 * i + 1];
        ASSERT_NE(0.0f, b.x);
        {
          const auto expected = cl::cross(a, b);
          const auto& result = results[2 * i + 0];
          for (uint32b j = 0; j < 4; ++j) {
            ASSERT_FLOAT_EQ(expected[j], result[j]) << j << " th: " << error_message
              << " [" << i << "] " << std::scientific
              << "a=(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << "), "
              << "b=(" << b.x << ", " << b.y << ", " << b.z << ", " << b.w << ").";
          }
        }
        {
          const auto expected = cl::cross(b, a);
          const auto& result = results[2 * i + 1];
          for (uint32b j = 0; j < 4; ++j) {
            ASSERT_FLOAT_EQ(expected[j], result[j]) << j << " th: " << error_message
              << " [" << i << "] " << std::scientific
              << "a=(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << "), "
              << "b=(" << b.x << ", " << b.y << ", " << b.z << ", " << b.w << ").";
          }
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(GeometricTest, Dot3Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;

    auto parameter_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    parameter_buff->setSize(2 * resolution);

    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(2 * resolution);

    auto res_buff= makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(geometric, testDot3));
    kernel->run(*parameter_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'dot' func is wrong.";
    {
      std::vector<cl::float3> parameters;
      parameters.resize(parameter_buff->size());
      parameter_buff->read(parameters.data(), parameters.size(), 0, 0);
      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < resolution; ++i) {
        auto a = parameters[2 * i + 0];
        ASSERT_NE(0.0f, a.x);
        auto b = parameters[2 * i + 1];
        ASSERT_NE(0.0f, b.x);
        const auto expected = std::fma(a.x, b.x, std::fma(a.y, b.y, a.z * b.z));
        {
          const auto result = results[2 * i + 0];
          ASSERT_FLOAT_EQ(expected, result) << error_message
                << " [" << i << "] " << std::scientific
                << "a=(" << a.x << ", " << a.y << ", " << a.z << "), "
                << "b=(" << b.x << ", " << b.y << ", " << b.z << ").";
        }
        {
          const auto result = results[2 * i + 1];
          ASSERT_FLOAT_EQ(expected, result) << error_message
              << " [" << i << "] " << std::scientific
              << "a=(" << a.x << ", " << a.y << ", " << a.z << "), "
              << "b=(" << b.x << ", " << b.y << ", " << b.z << ").";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(GeometricTest, Dot4Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;

    auto parameter_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    parameter_buff->setSize(2 * resolution);

    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(2 * resolution);

    auto res_buff= makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(geometric, testDot4));
    kernel->run(*parameter_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'dot' func is wrong.";
    {
      std::vector<cl::float4> parameters;
      parameters.resize(parameter_buff->size());
      parameter_buff->read(parameters.data(), parameters.size(), 0, 0);
      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < resolution; ++i) {
        auto a = parameters[2 * i + 0];
        ASSERT_NE(0.0f, a.x);
        auto b = parameters[2 * i + 1];
        ASSERT_NE(0.0f, b.x);
        {
          const auto expected = std::fma(a.x, b.x, std::fma(a.y, b.y, std::fma(a.z, b.z, a.w * b.w)));
          const auto result = results[2 * i + 0];
          ASSERT_FLOAT_EQ(expected, result) << error_message
            << " [" << i << "] " << std::scientific
            << "a=(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << "), "
            << "b=(" << b.x << ", " << b.y << ", " << b.z << ", " << b.w << ").";
        }
        {
          a.w = 0.0f;
          b.w = 0.0f;
          const auto expected = std::fma(a.x, b.x, std::fma(a.y, b.y, a.z * b.z));
          const auto result = results[2 * i + 1];
          ASSERT_FLOAT_EQ(expected, result) << error_message
            << " [" << i << "] " << std::scientific
            << "a=(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << "), "
            << "b=(" << b.x << ", " << b.y << ", " << b.z << ", " << b.w << ").";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(GeometricTest, Distance3Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;

    auto parameter_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    parameter_buff->setSize(2 * resolution);

    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(2 * resolution);

    auto res_buff= makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(geometric, testDistance3));
    kernel->run(*parameter_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'distance' func is wrong.";
    {
      std::vector<cl::float3> parameters;
      parameters.resize(parameter_buff->size());
      parameter_buff->read(parameters.data(), parameters.size(), 0, 0);
      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < resolution; ++i) {
        const auto& a = parameters[2 * i + 0];
        ASSERT_NE(0.0f, a.x);
        const auto& b = parameters[2 * i + 1];
        ASSERT_NE(0.0f, b.x);
        const auto t = a - b;
        {
          const auto expected = std::sqrt(cl::dot(t, t));
          const auto result = results[2 * i + 0];
          ASSERT_FLOAT_EQ(expected, result) << error_message
                << " [" << i << "] " << std::scientific
                << "a=(" << a.x << ", " << a.y << ", " << a.z << "), "
                << "b=(" << b.x << ", " << b.y << ", " << b.z << ").";
        }
        {
          const auto expected = std::sqrt(cl::dot(t, t));
          const auto result = results[2 * i + 1];
          ASSERT_FLOAT_EQ(expected, result) << error_message
              << " [" << i << "] " << std::scientific
              << "a=(" << a.x << ", " << a.y << ", " << a.z << "), "
              << "b=(" << b.x << ", " << b.y << ", " << b.z << ").";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(GeometricTest, Distance4Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;

    auto parameter_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    parameter_buff->setSize(2 * resolution);

    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(2 * resolution);

    auto res_buff= makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(geometric, testDistance4));
    kernel->run(*parameter_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'distance' func is wrong.";
    {
      std::vector<cl::float4> parameters;
      parameters.resize(parameter_buff->size());
      parameter_buff->read(parameters.data(), parameters.size(), 0, 0);
      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < resolution; ++i) {
        const auto& a = parameters[2 * i + 0];
        ASSERT_NE(0.0f, a.x);
        const auto& b = parameters[2 * i + 1];
        ASSERT_NE(0.0f, b.x);
        const auto t = a - b;
        {
          const auto expected = std::sqrt(cl::dot(t, t));
          const auto result = results[2 * i + 0];
          ASSERT_FLOAT_EQ(expected, result) << error_message
            << " [" << i << "] " << std::scientific
            << "a=(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << "), "
            << "b=(" << b.x << ", " << b.y << ", " << b.z << ", " << b.w << ").";
        }
        {
          const auto expected = std::sqrt(cl::dot(t, t));
          const auto result = results[2 * i + 1];
          ASSERT_FLOAT_EQ(expected, result) << error_message
            << " [" << i << "] " << std::scientific
            << "a=(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << "), "
            << "b=(" << b.x << ", " << b.y << ", " << b.z << ", " << b.w << ").";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(GeometricTest, Normalize3Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;

    auto parameter_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    parameter_buff->setSize(2 * resolution);

    auto result_buff = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(2 * resolution);

    auto res_buff= makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(geometric, testNormalize3));
    kernel->run(*parameter_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'normalize' func is wrong.";
    {
      std::vector<cl::float3> parameters;
      parameters.resize(parameter_buff->size());
      parameter_buff->read(parameters.data(), parameters.size(), 0, 0);
      std::vector<cl::float3> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < resolution; ++i) {
        const auto& a = parameters[2 * i + 0];
        ASSERT_NE(0.0f, a.x);
        const auto& b = parameters[2 * i + 1];
        ASSERT_NE(0.0f, b.x);
        {
          const auto expected = a * (1.0f / std::sqrt(cl::dot(a, a)));
          const auto& result = results[2 * i + 0];
          for (uint32b j = 0; j < 3; ++j) {
            ASSERT_FLOAT_EQ(expected[j], result[j]) << j << " th: " << error_message
                << " [" << i << "] " << std::scientific
                << "a=(" << a.x << ", " << a.y << ", " << a.z << ").";
          }
        }
        {
          const auto expected = b * (1.0f / std::sqrt(cl::dot(b, b)));
          const auto& result = results[2 * i + 1];
          for (uint32b j = 0; j < 3; ++j) {
            ASSERT_FLOAT_EQ(expected[j], result[j]) << j << " th: " << error_message
              << " [" << i << "] " << std::scientific
              << "b=(" << b.x << ", " << b.y << ", " << b.z << ").";
          }
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(GeometricTest, Normalize4Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;

    auto parameter_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    parameter_buff->setSize(2 * resolution);

    auto result_buff = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(2 * resolution);

    auto res_buff= makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(geometric, testNormalize4));
    kernel->run(*parameter_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The 'normalize' func is wrong.";
    {
      std::vector<cl::float4> parameters;
      parameters.resize(parameter_buff->size());
      parameter_buff->read(parameters.data(), parameters.size(), 0, 0);
      std::vector<cl::float4> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < resolution; ++i) {
        const auto& a = parameters[2 * i + 0];
        ASSERT_NE(0.0f, a.x);
        auto b = parameters[2 * i + 1];
        ASSERT_NE(0.0f, b.x);
        {
          const auto expected = a * (1.0f / std::sqrt(cl::dot(a, a)));
          const auto& result = results[2 * i + 0];
          for (uint32b j = 0; j < 3; ++j) {
            ASSERT_FLOAT_EQ(expected[j], result[j]) << j << " th: " << error_message
                << " [" << i << "] " << std::scientific
                << "a=(" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << ").";
          }
        }
        {
          b.w = 0.0f;
          const auto expected = b * (1.0f / std::sqrt(cl::dot(b, b)));
          const auto& result = results[2 * i + 1];
          for (uint32b j = 0; j < 3; ++j) {
            ASSERT_FLOAT_EQ(expected[j], result[j]) << j << " th: " << error_message
              << " [" << i << "] " << std::scientific
              << "b=(" << b.x << ", " << b.y << ", " << b.z << ", " << b.w << ").";
          }
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

