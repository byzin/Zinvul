/*!
  \file rng_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/correlated_multi_jittered_engine.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/rng.hpp"
// Test
#include "test.hpp"

TEST(RngTest, Cmj64Test)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b root_n = 8;
    const uint32b seed = 123'456'789u;
    const uint32b num_of_samples = 8192;
    auto result_1d = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    result_1d->setSize(num_of_samples);
    auto result_2d = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
    result_2d->setSize(num_of_samples);
    auto seed_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    seed_buffer->setSize(1);
    seed_buffer->write(&seed, seed_buffer->size(), 0, 0);
    auto iteration_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    iteration_buffer->setSize(1);
    iteration_buffer->write(&num_of_samples, iteration_buffer->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), rng, testCmj64, 1);
    kernel->run(*result_1d, *result_2d, *seed_buffer, *iteration_buffer, {num_of_samples}, 0);
    device->waitForCompletion();

    constexpr uint32b n = root_n * root_n;
    using CmjEngine = zisc::CorrelatedMultiJitteredEngine<root_n>;
    {
      std::vector<float> results;
      results.resize(num_of_samples);
      result_1d->read(results.data(), num_of_samples, 0, 0);
      for (uint32b sample = 0; sample < num_of_samples; ++sample) {
        const uint32b s = sample / n;
        const uint32b p = seed + sample % n;
        const auto expected = CmjEngine::template generate1D<float>(s, p);
        ASSERT_FLOAT_EQ(expected, results[sample])
            << "[" << sample << "] zCmjGenerate1D(" << s << "," << p << ") is wrong.";
      }
    }
    {
      std::vector<cl::float2> results;
      results.resize(num_of_samples);
      result_2d->read(results.data(), num_of_samples, 0, 0);
      for (uint32b sample = 0; sample < num_of_samples; ++sample) {
        const uint32b s = sample / n;
        const uint32b p = seed + sample % n;
        const auto expected = CmjEngine::template generate2D<float>(s, p);
        for (std::size_t i = 0; i < 2; ++i) {
          EXPECT_FLOAT_EQ(expected[i], results[sample][i])
              << "[" << sample << "] zCmjGenerate2D(" << s << "," << p << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

#if !defined(Z_MAC)

TEST(RngTest, Cmj64DTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b root_n = 8;
    const uint32b seed = 123'456'789u;
    const uint32b num_of_samples = 8192;
    auto result_1d = makeBuffer<double>(device.get(), BufferUsage::kDeviceTSrc);
    result_1d->setSize(num_of_samples);
    auto result_2d = makeBuffer<cl::double2>(device.get(), BufferUsage::kDeviceTSrc);
    result_2d->setSize(num_of_samples);
    auto seed_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    seed_buffer->setSize(1);
    seed_buffer->write(&seed, seed_buffer->size(), 0, 0);
    auto iteration_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    iteration_buffer->setSize(1);
    iteration_buffer->write(&num_of_samples, iteration_buffer->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), rng, testCmj64D, 1);
    kernel->run(*result_1d, *result_2d, *seed_buffer, *iteration_buffer, {num_of_samples}, 0);
    device->waitForCompletion();

    constexpr uint32b n = root_n * root_n;
    using CmjEngine = zisc::CorrelatedMultiJitteredEngine<root_n>;
    {
      std::vector<double> results;
      results.resize(num_of_samples);
      result_1d->read(results.data(), num_of_samples, 0, 0);
      for (uint32b sample = 0; sample < num_of_samples; ++sample) {
        const uint32b s = sample / n;
        const uint32b p = seed + sample % n;
        const auto expected = CmjEngine::template generate1D<double>(s, p);
        ASSERT_DOUBLE_EQ(expected, results[sample])
            << "[" << sample << "] zCmjGenerate1D(" << s << "," << p << ") is wrong.";
      }
    }
    {
      std::vector<cl::double2> results;
      results.resize(num_of_samples);
      result_2d->read(results.data(), num_of_samples, 0, 0);
      for (uint32b sample = 0; sample < num_of_samples; ++sample) {
        const uint32b s = sample / n;
        const uint32b p = seed + sample % n;
        const auto expected = CmjEngine::template generate2D<double>(s, p);
        for (std::size_t i = 0; i < 2; ++i) {
          EXPECT_DOUBLE_EQ(expected[i], results[sample][i])
              << "[" << sample << "] zCmjGenerate2D(" << s << "," << p << ") is wrong.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

#endif // Z_MAC

TEST(RngTest, Cmj64ImageTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution_x = 1920;
    constexpr uint32b resolution_y = 1080;
    constexpr uint32b resolution = resolution_x * resolution_y;
    auto red_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    red_buffer->setSize(resolution);
    auto green_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    green_buffer->setSize(resolution);
    auto blue_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
    blue_buffer->setSize(resolution);
    auto sample_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    sample_buffer->setSize(1);
    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
    resolution_buffer->setSize(1);
    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);

    auto kernel = makeZinvulKernel(device.get(), rng, testCmj64Image, 1);

    uint32b sample = 0;
    // Red
    sample_buffer->write(&sample, 1, 0, 0);
    kernel->run(*red_buffer, *sample_buffer, *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();
    // Green
    sample = 1;
    sample_buffer->write(&sample, 1, 0, 0);
    kernel->run(*green_buffer, *sample_buffer, *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();
    // Green
    sample = 2;
    sample_buffer->write(&sample, 1, 0, 0);
    kernel->run(*blue_buffer, *sample_buffer, *resolution_buffer, {resolution}, 0);
    device->waitForCompletion();

    {
      auto image_name = "cmj64_image_device" + std::to_string(number) + ".ppm";
      std::ofstream image{image_name, std::ios_base::binary};
      image << "P6" << std::endl;
      image << "#" << std::endl;
      image << resolution_x << " " << resolution_y << std::endl;
      image << 255 << std::endl;

      std::vector<float> red_results;
      red_results.resize(resolution);
      red_buffer->read(red_results.data(), red_results.size(), 0, 0);

      std::vector<float> green_results;
      green_results.resize(resolution);
      green_buffer->read(green_results.data(), green_results.size(), 0, 0);

      std::vector<float> blue_results;
      blue_results.resize(resolution);
      blue_buffer->read(blue_results.data(), blue_results.size(), 0, 0);

      const std::vector<float>* color_results[3] = {&red_results,
                                                    &green_results,
                                                    &blue_results};
      for (uint32b i = 0; i < resolution; ++i) {
        for (uint32b color = 0; color < 3; ++color) {
          const float v = 256.0f * (*color_results[color])[i];
          const uint32b c = zisc::cast<uint32b>(v);
          if (256 <= c)
            std::cerr << "256 is happened: " << c << std::endl;
          image << zisc::cast<uint8b>(c);
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
