/*!
  \file math_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/math.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/math.hpp"
// Test
#include "test.hpp"

TEST(MathTest, ConstantValueTest)
{
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    auto pi_buffer = zinvul::makeBuffer<float>(device.get(),
                                               zinvul::BufferUsage::kDeviceToHost);
    pi_buffer->setSize(1);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testConstantValues, 1);
    kernel->run(*pi_buffer, {1}, 0);
    device->waitForCompletion();

    {
      float pi = 0.0f;
      pi_buffer->read(&pi);
      EXPECT_EQ(zisc::kPi<float>, pi)
          << "The pi values aren't same on host and device: "
          << std::setprecision(std::numeric_limits<float>::max_digits10)
          << zisc::kPi<float>;
    }
  }
}
