/*!
  \file work_item_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstddef>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/work_item.hpp"

TEST(WorkItemTest, CpuTest)
{
  const zinvul::uint resolution = 1920 * 1080;

  zinvul::DeviceOptions options;
  auto device = zinvul::makeDevice(options);
  {
    auto buffer1 = zinvul::makeBuffer<zinvul::uint32b>(device.get(),
                                                       zinvul::kDeviceToHost);
    buffer1->setSize(resolution);
    auto buffer2 = zinvul::makeBuffer<zinvul::uint32b>(device.get(),
                                                       zinvul::kHostToDevice);
    buffer2->setSize(1);
    buffer2->write(&resolution);

    auto kernel = zinvul::makeZinvulKernel(device.get(), work_item, test1dWorkSize);
    kernel->run(*buffer1, *buffer2, resolution);

    std::vector<zinvul::uint32b> result;
    result.resize(resolution);
    buffer1->read(result.data());

    for (std::size_t i = 0; i < result.size(); ++i) {
      EXPECT_EQ(i, result[i]) << "The 'getGlobalIdX()' func doesn't work.";
    }
  }
}
