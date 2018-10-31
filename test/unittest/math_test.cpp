/*!
  \file math_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <iomanip>
#include <limits>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/math.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/math.hpp"

TEST(MathTest, ConstantValueTest)
{
  zinvul::DeviceOptions options;
  auto device = zinvul::makeDevice(options);
  {
    auto pi_buffer = zinvul::makeBuffer<float>(device.get(), zinvul::kDeviceToHost);
    pi_buffer->setSize(1);

    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testConstantValues);
    kernel->run(*pi_buffer, 1);

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
