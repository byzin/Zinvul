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
#include "zisc/matrix.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/matrix.hpp"
// Test
#include "test.hpp"

//TEST(MatrixTest, zInitMatrix2x2Test)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix2x2 = zinvul::math::KernelGroup::ZMatrix2x2;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(1);
//    {
//      const auto matrix = k.zMakeMat2x2(0.0f, 1.0f,
//                                        2.0f, 3.0f);
//      input_buf->write(&matrix, 1, 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(6);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix2x2Init, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix2x2, 6> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      {
//        const auto& matrix = results[0];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[1];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[2];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[3];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[4];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[5];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix2x2 initialization failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zInitMatrix3x3Test)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix3x3 = zinvul::math::KernelGroup::ZMatrix3x3;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(1);
//    {
//      const auto matrix = k.zMakeMat3x3(0.0f, 1.0f, 2.0f,
//                                        3.0f, 4.0f, 5.0f,
//                                        6.0f, 7.0f, 8.0f);
//      input_buf->write(&matrix, 1, 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(6);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix3x3Init, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix3x3, 6> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      {
//        const auto& matrix = results[0];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[1];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[2];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[3];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[4];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[5];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix3x3 initialization failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zInitMatrix4x4Test)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix4x4 = zinvul::math::KernelGroup::ZMatrix4x4;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(1);
//    {
//      const auto matrix = k.zMakeMat4x4(0.0f, 1.0f, 2.0f, 3.0f,
//                                        4.0f, 5.0f, 6.0f, 7.0f,
//                                        8.0f, 9.0f, 10.0f, 11.0f,
//                                        12.0f, 13.0f, 14.0f, 15.0f);
//      input_buf->write(&matrix, 1, 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(6);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix4x4Init, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix4x4, 6> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      {
//        const auto& matrix = results[0];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[1];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[2];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[3];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[4];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
//        }
//      }
//      {
//        const auto& matrix = results[5];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = zisc::cast<float>(i);
//          const float result = matrix.m_[i];
//          ASSERT_EQ(expected, result) << "Matrix4x4 initialization failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix2x2AdditionTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix2x2 = zinvul::math::KernelGroup::ZMatrix2x2;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat2x2(0.0f, 1.0f,
//                                        2.0f, 3.0f);
//      std::array<ZMatrix2x2, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(11);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix2x2Addition, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix2x2, 11> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix2x2 addition failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix3x3AdditionTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix3x3 = zinvul::math::KernelGroup::ZMatrix3x3;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat3x3(0.0f, 1.0f, 2.0f,
//                                        3.0f, 4.0f, 5.0f,
//                                        6.0f, 7.0f, 8.0f);
//      std::array<ZMatrix3x3, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(11);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix3x3Addition, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix3x3, 11> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix3x3 addition failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix4x4AdditionTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix4x4 = zinvul::math::KernelGroup::ZMatrix4x4;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat4x4(0.0f, 1.0f, 2.0f, 3.0f,
//                                        4.0f, 5.0f, 6.0f, 7.0f,
//                                        8.0f, 9.0f, 10.0f, 11.0f,
//                                        12.0f, 13.0f, 14.0f, 15.0f);
//      std::array<ZMatrix4x4, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(11);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix4x4Addition, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix4x4, 11> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = zisc::cast<float>(i * 2);
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix4x4 addition failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix2x2SubtractionTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix2x2 = zinvul::math::KernelGroup::ZMatrix2x2;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat2x2(0.0f, 1.0f,
//                                        2.0f, 3.0f);
//      std::array<ZMatrix2x2, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(11);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix2x2Subtraction, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix2x2, 11> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = 0.0f;
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix2x2 subtraction failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix3x3SubtractionTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix3x3 = zinvul::math::KernelGroup::ZMatrix3x3;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat3x3(0.0f, 1.0f, 2.0f,
//                                        3.0f, 4.0f, 5.0f,
//                                        6.0f, 7.0f, 8.0f);
//      std::array<ZMatrix3x3, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(11);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix3x3Subtraction, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix3x3, 11> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = 0.0f;
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix3x3 subtraction failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix4x4SubtractionTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix4x4 = zinvul::math::KernelGroup::ZMatrix4x4;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat4x4(0.0f, 1.0f, 2.0f, 3.0f,
//                                        4.0f, 5.0f, 6.0f, 7.0f,
//                                        8.0f, 9.0f, 10.0f, 11.0f,
//                                        12.0f, 13.0f, 14.0f, 15.0f);
//      std::array<ZMatrix4x4, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(11);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix4x4Subtraction, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      std::array<ZMatrix4x4, 11> results;
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = 0.0f;
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix4x4 subtraction failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix2x2MultiplicationTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix2x2 = zinvul::math::KernelGroup::ZMatrix2x2;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat2x2(0.0f, 1.0f,
//                                        2.0f, 3.0f);
//      std::array<ZMatrix2x2, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(21);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix2x2Multiplication, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      using ziscMatrix2x2 = zisc::Matrix<float, 2, 2>;
//      ziscMatrix2x2 expected_matrix{{0.0f, 1.0f,
//                                     2.0f, 3.0f}};
//      expected_matrix = expected_matrix * expected_matrix;
//
//      std::vector<ZMatrix2x2> results;
//      results.resize(output_buf->size());
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = expected_matrix.data()[i];
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix2x2 multiplication failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix2x2ScalarMultiplicationTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix2x2 = zinvul::math::KernelGroup::ZMatrix2x2;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(1);
//    {
//      const auto matrix = k.zMakeMat2x2(0.0f, 1.0f,
//                                        2.0f, 3.0f);
//      input_buf->write(&matrix, 1, 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix2x2>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(9);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix2x2ScalarMultiplication, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      using ziscMatrix2x2 = zisc::Matrix<float, 2, 2>;
//      ziscMatrix2x2 expected_matrix{{0.0f, 1.0f,
//                                     2.0f, 3.0f}};
//      expected_matrix = 3.0f * expected_matrix;
//
//      std::vector<ZMatrix2x2> results;
//      results.resize(output_buf->size());
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 4; ++i) {
//          const float expected = expected_matrix.data()[i];
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix2x2 scalar multiplication failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix3x3MultiplicationTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix3x3 = zinvul::math::KernelGroup::ZMatrix3x3;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat3x3(0.0f, 1.0f, 2.0f,
//                                        3.0f, 4.0f, 5.0f,
//                                        6.0f, 7.0f, 8.0f);
//      std::array<ZMatrix3x3, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(21);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix3x3Multiplication, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      using ziscMatrix3x3 = zisc::Matrix<float, 3, 3>;
//      ziscMatrix3x3 expected_matrix{{0.0f, 1.0f, 2.0f,
//                                     3.0f, 4.0f, 5.0f,
//                                     6.0f, 7.0f, 8.0f}};
//      expected_matrix = expected_matrix * expected_matrix;
//
//      std::vector<ZMatrix3x3> results;
//      results.resize(output_buf->size());
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = expected_matrix.data()[i];
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix3x3 multiplication failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix3x3ScalarMultiplicationTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix3x3 = zinvul::math::KernelGroup::ZMatrix3x3;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(1);
//    {
//      const auto matrix = k.zMakeMat3x3(0.0f, 1.0f, 2.0f,
//                                        3.0f, 4.0f, 5.0f,
//                                        6.0f, 7.0f, 8.0f);
//      input_buf->write(&matrix, 1, 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix3x3>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(9);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix3x3ScalarMultiplication, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      using ziscMatrix3x3 = zisc::Matrix<float, 3, 3>;
//      ziscMatrix3x3 expected_matrix{{0.0f, 1.0f, 2.0f,
//                                     3.0f, 4.0f, 5.0f,
//                                     6.0f, 7.0f, 8.0f}};
//      expected_matrix = 3.0f * expected_matrix;
//
//      std::vector<ZMatrix3x3> results;
//      results.resize(output_buf->size());
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 9; ++i) {
//          const float expected = expected_matrix.data()[i];
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix3x3 scalar multiplication failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix4x4MultiplicationTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix4x4 = zinvul::math::KernelGroup::ZMatrix4x4;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(2);
//    {
//      const auto matrix = k.zMakeMat4x4(0.0f, 1.0f, 2.0f, 3.0f,
//                                        4.0f, 5.0f, 6.0f, 7.0f,
//                                        8.0f, 9.0f, 10.0f, 11.0f,
//                                        12.0f, 13.0f, 14.0f, 15.0f);
//      std::array<ZMatrix4x4, 2> mat_list{{matrix, matrix}};
//      input_buf->write(mat_list.data(), mat_list.size(), 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(21);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix4x4Multiplication, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      using ziscMatrix4x4 = zisc::Matrix<float, 4, 4>;
//      ziscMatrix4x4 expected_matrix{{0.0f, 1.0f, 2.0f, 3.0f,
//                                     4.0f, 5.0f, 6.0f, 7.0f,
//                                     8.0f, 9.0f, 10.0f, 11.0f,
//                                     12.0f, 13.0f, 14.0f, 15.0f}};
//      expected_matrix = expected_matrix * expected_matrix;
//
//      std::vector<ZMatrix4x4> results;
//      results.resize(output_buf->size());
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = expected_matrix.data()[i];
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix4x4 multiplication failed.";
//        }
//      }
//    }
//  }
//}
//
//TEST(MatrixTest, zMatrix4x4ScalarMultiplicationTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    using ZMatrix4x4 = zinvul::math::KernelGroup::ZMatrix4x4;
//    zinvul::math::KernelGroup k;
//
//    auto input_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTDst);
//    input_buf->setSize(1);
//    {
//      const auto matrix = k.zMakeMat4x4(0.0f, 1.0f, 2.0f, 3.0f,
//                                        4.0f, 5.0f, 6.0f, 7.0f,
//                                        8.0f, 9.0f, 10.0f, 11.0f,
//                                        12.0f, 13.0f, 14.0f, 15.0f);
//      input_buf->write(&matrix, 1, 0, 0);
//    }
//    auto output_buf = makeStorageBuffer<ZMatrix4x4>(device.get(), BufferUsage::kDeviceTSrc);
//    output_buf->setSize(9);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMatrix4x4ScalarMultiplication, 1);
//    kernel->run(*input_buf, *output_buf, {1}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    {
//      using ziscMatrix4x4 = zisc::Matrix<float, 4, 4>;
//      ziscMatrix4x4 expected_matrix{{0.0f, 1.0f, 2.0f, 3.0f,
//                                     4.0f, 5.0f, 6.0f, 7.0f,
//                                     8.0f, 9.0f, 10.0f, 11.0f,
//                                     12.0f, 13.0f, 14.0f, 15.0f}};
//      expected_matrix = 3.0f * expected_matrix;
//
//      std::vector<ZMatrix4x4> results;
//      results.resize(output_buf->size());
//      output_buf->read(results.data(), results.size(), 0, 0);
//      for (std::size_t id = 0; id < results.size(); ++id) {
//        const auto& matrix = results[id];
//        for (std::size_t i = 0; i < 16; ++i) {
//          const float expected = expected_matrix.data()[i];
//          const float result = matrix.m_[i];
//          ASSERT_FLOAT_EQ(expected, result) << "[" << id << "]: Matrix4x4 scalar multiplication failed.";
//        }
//      }
//    }
//  }
//}
