/*!
  \file work_item_test.cpp
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
#include "zinvul/work_item.hpp"
// Test
#include "test.hpp"

TEST(WorkItemTest, WorkSize1dTest)
{
  using namespace zinvul;

  constexpr uint resolution = 500;
  constexpr uint n_func = 8;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceToHost);
    buffer1->setSize(n_func * resolution);
    auto buffer2 = makeBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    buffer2->setSize(1);
    buffer2->write(&resolution);

    auto kernel = makeZinvulKernel(device.get(), work_item, test1dWorkSize, 1);
    kernel->run(*buffer1, *buffer2, {resolution}, 0);
    device->waitForCompletion();

    std::vector<uint32b> result;
    result.resize(n_func * resolution);
    buffer1->read(result.data());

    for (std::size_t index = 0; index < resolution; ++index) {
      ASSERT_EQ(index, result[index * n_func])
          << "The 'getGlobalIdX()' func doesn't work.";
      ASSERT_EQ(resolution, result[index * n_func + 7])
          << "The resolution is wrong.";
    }
  }
}

TEST(WorkItemTest, WorkSize2dTest)
{
  using namespace zinvul;

  const cl::uint2 resolution{500, 500};
  constexpr uint n_func = 16;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    auto buffer1 = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceToHost);
    buffer1->setSize(n_func * resolution.x * resolution.y);
    auto buffer2 = makeBuffer<cl::uint2>(device.get(), BufferUsage::kHostToDevice);
    buffer2->setSize(1);
    buffer2->write(&resolution);

    auto kernel = makeZinvulKernel(device.get(), work_item, test2dWorkSize, 2);
    kernel->run(*buffer1, *buffer2, {resolution.x, resolution.y}, 0);
    device->waitForCompletion();

    std::vector<uint32b> result;
    result.resize(n_func * resolution.x * resolution.y);
    buffer1->read(result.data());

    for (std::size_t y = 0; y < resolution.y; ++y) {
      for (std::size_t x = 0; x < resolution.x; ++x) {
        const std::size_t index = x + y * resolution.x;
        ASSERT_EQ(x, result[index * n_func + 0])
            << "The 'getGlobalIdX()' func doesn't work.";
        ASSERT_EQ(y, result[index * n_func + 1])
            << "The 'getGlobalIdY()' func doesn't work.";
        ASSERT_EQ(resolution.x, result[index * n_func + 14])
            << "The resolution.x is wrong.";
        ASSERT_EQ(resolution.y, result[index * n_func + 15])
            << "The resolution.y is wrong.";
      }
    }
  }
}
