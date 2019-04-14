/*!
  \file test.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TEST_TEST_HPP
#define ZINVUL_TEST_TEST_HPP

// Standard C++ library
#include <cstddef>
#include <string>
#include <vector>
// Zisc
#include "zisc/unique_memory_pointer.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/cppcl/vector.hpp"

#define ASSERT_FLOAT_NEAR(val1, val2, abs_error) \
  const double val1d = static_cast<double>(val1); \
  const double val2d = static_cast<double>(val2); \
  const double abs_errord = static_cast<double>(abs_error); \
  ASSERT_NEAR(val1d, val2d, abs_errord)

#define EXPECT_FLOAT_NEAR(val1, val2, abs_error) \
  const double val1d = static_cast<double>(val1); \
  const double val2d = static_cast<double>(val2); \
  const double abs_errord = static_cast<double>(abs_error); \
  EXPECT_NEAR(val1d, val2d, abs_errord)

std::vector<zinvul::UniqueDevice> makeTestDeviceList(zinvul::DeviceOptions& options);

zinvul::DeviceOptions makeTestOptions();

std::string getTestDeviceInfo(const zinvul::Device& device);

std::string getTestDeviceUsedMemory(const zinvul::Device& device);

float calcUlps(const float lhs, const float rhs);

#endif // ZINVUL_TEST_TEST_HPP
