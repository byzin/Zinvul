/*!
  \file test.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
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

std::vector<zinvul::UniqueDevice> makeTestDeviceList(zinvul::DeviceOptions& options);

zinvul::DeviceOptions makeTestOptions();

std::string getTestDeviceInfo(const zinvul::Device& device);

std::string getTestDeviceUsedMemory(const zinvul::Device& device);

#endif // ZINVUL_TEST_TEST_HPP
