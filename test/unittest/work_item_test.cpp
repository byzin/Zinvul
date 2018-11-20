/*!
  \file work_item_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
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

TEST(WorkItemTest, CpuTest)
{
  const zinvul::uint resolution = 1920 * 1080;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    auto buffer1 = zinvul::makeBuffer<zinvul::uint32b>(
        device.get(),
        zinvul::BufferUsage::kDeviceToHost);
    buffer1->setSize(resolution);
    auto buffer2 = zinvul::makeBuffer<zinvul::uint32b>(
        device.get(),
        zinvul::BufferUsage::kHostToDevice);
    buffer2->setSize(1);
    buffer2->write(&resolution);

    auto kernel = zinvul::makeZinvulKernel(device.get(), work_item, test1dWorkSize, 1);
    kernel->run(*buffer1, *buffer2, {resolution}, 0);
    device->waitForCompletion();

    std::vector<zinvul::uint32b> result;
    result.resize(resolution);
    buffer1->read(result.data());

    for (std::size_t i = 0; i < result.size(); ++i) {
      ASSERT_EQ(i, result[i]) << "The 'getGlobalIdX()' func doesn't work.";
    }
  }
}
