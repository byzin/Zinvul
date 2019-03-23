/*!
  \file matrix_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/compensated_summation.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/math.hpp"
// Test
#include "test.hpp"

TEST(MathTest, zInitMatrix2x2Test)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    using ZMatrix2x2 = zinvul::math::KernelGroup::ZMatrix2x2;
    zinvul::math::KernelGroup k;

    auto input_buf = makeBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTDst);
    input_buf->setSize(1);
    {
      const auto matrix = k.zMakeMat2x2(0.0f, 1.0f,
                                        2.0f, 3.0f);
      input_buf->write(&matrix, 1, 0, 0);
    }
    auto output_buf = makeBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTSrc);
    output_buf->setSize(6);

    auto kernel = makeZinvulKernel(device.get(), math, testMatrix2x2Init, 1);
    kernel->run(*input_buf, *output_buf, {1}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    {
      std::array<ZMatrix2x2, 6> results;
      output_buf->read(results.data(), results.size(), 0, 0);
      {
        const auto& matrix = results[0];
        for (std::size_t i = 0; i < 4; ++i) {
          const float expected = zisc::cast<float>(i * 2);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
        }
      }
      {
        const auto& matrix = results[1];
        for (std::size_t i = 0; i < 4; ++i) {
          const float expected = zisc::cast<float>(i * 2);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
        }
      }
      {
        const auto& matrix = results[2];
        for (std::size_t i = 0; i < 4; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
        }
      }
      {
        const auto& matrix = results[3];
        for (std::size_t i = 0; i < 4; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
        }
      }
      {
        const auto& matrix = results[4];
        for (std::size_t i = 0; i < 4; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
        }
      }
      {
        const auto& matrix = results[5];
        for (std::size_t i = 0; i < 4; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
        }
      }
    }
  }
}

TEST(MathTest, zInitMatrix3x3Test)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    using ZMatrix3x3 = zinvul::math::KernelGroup::ZMatrix3x3;
    zinvul::math::KernelGroup k;

    auto input_buf = makeBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTDst);
    input_buf->setSize(1);
    {
      const auto matrix = k.zMakeMat3x3(0.0f, 1.0f, 2.0f,
                                        3.0f, 4.0f, 5.0f,
                                        6.0f, 7.0f, 8.0f);
      input_buf->write(&matrix, 1, 0, 0);
    }
    auto output_buf = makeBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTSrc);
    output_buf->setSize(6);

    auto kernel = makeZinvulKernel(device.get(), math, testMatrix3x3Init, 1);
    kernel->run(*input_buf, *output_buf, {1}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    {
      std::array<ZMatrix3x3, 6> results;
      output_buf->read(results.data(), results.size(), 0, 0);
      {
        const auto& matrix = results[0];
        for (std::size_t i = 0; i < 9; ++i) {
          const float expected = zisc::cast<float>(i * 2);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
        }
      }
      {
        const auto& matrix = results[1];
        for (std::size_t i = 0; i < 9; ++i) {
          const float expected = zisc::cast<float>(i * 2);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
        }
      }
      {
        const auto& matrix = results[2];
        for (std::size_t i = 0; i < 9; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
        }
      }
      {
        const auto& matrix = results[3];
        for (std::size_t i = 0; i < 9; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
        }
      }
      {
        const auto& matrix = results[4];
        for (std::size_t i = 0; i < 9; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
        }
      }
      {
        const auto& matrix = results[5];
        for (std::size_t i = 0; i < 9; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
        }
      }
    }
  }
}

TEST(MathTest, zInitMatrix4x4Test)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    using ZMatrix4x4 = zinvul::math::KernelGroup::ZMatrix4x4;
    zinvul::math::KernelGroup k;

    auto input_buf = makeBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTDst);
    input_buf->setSize(1);
    {
      const auto matrix = k.zMakeMat4x4(0.0f, 1.0f, 2.0f, 3.0f,
                                        4.0f, 5.0f, 6.0f, 7.0f,
                                        8.0f, 9.0f, 10.0f, 11.0f,
                                        12.0f, 13.0f, 14.0f, 15.0f);
      input_buf->write(&matrix, 1, 0, 0);
    }
    auto output_buf = makeBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTSrc);
    output_buf->setSize(6);

    auto kernel = makeZinvulKernel(device.get(), math, testMatrix4x4Init, 1);
    kernel->run(*input_buf, *output_buf, {1}, 0);
    device->waitForCompletion();

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;

    {
      std::array<ZMatrix4x4, 6> results;
      output_buf->read(results.data(), results.size(), 0, 0);
      {
        const auto& matrix = results[0];
        for (std::size_t i = 0; i < 16; ++i) {
          const float expected = zisc::cast<float>(i * 2);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
        }
      }
      {
        const auto& matrix = results[1];
        for (std::size_t i = 0; i < 16; ++i) {
          const float expected = zisc::cast<float>(i * 2);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
        }
      }
      {
        const auto& matrix = results[2];
        for (std::size_t i = 0; i < 16; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
        }
      }
      {
        const auto& matrix = results[3];
        for (std::size_t i = 0; i < 16; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
        }
      }
      {
        const auto& matrix = results[4];
        for (std::size_t i = 0; i < 16; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
        }
      }
      {
        const auto& matrix = results[5];
        for (std::size_t i = 0; i < 16; ++i) {
          const float expected = zisc::cast<float>(i);
          const float result = matrix.m_[i];
          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
        }
      }
    }
  }
}
