/*!
  \file rng_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <chrono>
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
#include "zisc/stopwatch.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/zinvul_config.hpp"
#include "zinvul/kernel_set/rng.hpp"
// Test
#include "test.hpp"

TEST(RngTest, Cmj64Test)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b root_n = 8;
    const uint32b seed = 123'456'789u;
    const uint32b num_of_samples = 8192;
    auto result_1d = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_1d->setSize(num_of_samples);
    auto result_2d = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceOnly);
    result_2d->setSize(num_of_samples);
    auto seed_buffer = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    seed_buffer->setSize(1);
    seed_buffer->write(&seed, seed_buffer->size(), 0, 0);
    auto iteration_buffer = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    iteration_buffer->setSize(1);
    iteration_buffer->write(&num_of_samples, iteration_buffer->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(rng, testCmj64));
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
        for (uint32b i = 0; i < 2; ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b root_n = 8;
    const uint32b seed = 123'456'789u;
    const uint32b num_of_samples = 8192;
    auto result_1d = makeStorageBuffer<double>(device.get(), BufferUsage::kDeviceOnly);
    result_1d->setSize(num_of_samples);
    auto result_2d = makeStorageBuffer<cl::double2>(device.get(), BufferUsage::kDeviceOnly);
    result_2d->setSize(num_of_samples);
    auto seed_buffer = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    seed_buffer->setSize(1);
    seed_buffer->write(&seed, seed_buffer->size(), 0, 0);
    auto iteration_buffer = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    iteration_buffer->setSize(1);
    iteration_buffer->write(&num_of_samples, iteration_buffer->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(rng, testCmj64D));
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
        for (uint32b i = 0; i < 2; ++i) {
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
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution_x = 1920;
    constexpr uint32b resolution_y = 1080;
    constexpr uint32b resolution = resolution_x * resolution_y;
    auto red_buffer = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    red_buffer->setSize(resolution);
    auto green_buffer = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    green_buffer->setSize(resolution);
    auto blue_buffer = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    blue_buffer->setSize(resolution);
    auto sample_buffer = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    sample_buffer->setSize(1);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, res_buff->size(), 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(rng, testCmj64Image));

    uint32b sample = 0;
    // Red
    sample_buffer->write(&sample, 1, 0, 0);
    kernel->run(*red_buffer, *sample_buffer, *res_buff, {resolution}, 0);
    device->waitForCompletion();
    // Green
    sample = 1;
    sample_buffer->write(&sample, 1, 0, 0);
    kernel->run(*green_buffer, *sample_buffer, *res_buff, {resolution}, 0);
    device->waitForCompletion();
    // Green
    sample = 2;
    sample_buffer->write(&sample, 1, 0, 0);
    kernel->run(*blue_buffer, *sample_buffer, *res_buff, {resolution}, 0);
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

TEST(RngTest, Cmj256PerformanceTest)
{
  using namespace zinvul;
  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution_x = 2560;
    constexpr uint32b resolution_y = 1440;
    constexpr uint32b resolution = resolution_x * resolution_y;

    constexpr uint32b iterations = 64;
    constexpr uint32b block = 64;

    auto color_buffer = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    color_buffer->setSize(resolution);

    auto color_comp_buffer = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceOnly);
    color_comp_buffer->setSize(resolution);
    {
      std::vector<cl::float4> inputs;
      inputs.resize(resolution, cl::float4{});
      color_buffer->write(inputs.data(), inputs.size(), 0, 0);
      color_comp_buffer->write(inputs.data(), inputs.size(), 0, 0);
    }

    auto sample_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    sample_buff->setSize(1);
    {
      const uint32b sample_count = 0;
      sample_buff->write(&sample_count, 1, 0, 0);
    }

    auto block_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    block_buff->setSize(1);
    block_buff->write(&block, 1, 0, 0);

    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel1 = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(rng, testCmj256Performance));
    auto kernel2 = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(rng, testCmj256PerformanceNormalization));

    // Test
    zisc::Stopwatch stopwatch;
    stopwatch.start();

    std::cout << "  [" << std::flush;
    for (uint32b i = 0; i < iterations; ++i) {
      std::cout << "-" << std::flush;
      kernel1->run(*color_buffer, *color_comp_buffer, *sample_buff, *block_buff, *res_buff, {resolution}, 0);
      device->waitForCompletion();

      auto sample = sample_buff->mapMemory();
      sample[0] += block;
    }
    std::cout << "]" << std::endl;

    kernel2->run(*color_buffer, *color_comp_buffer, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      using std::chrono::duration_cast;
      const auto elapsed_time = stopwatch.elapsedTime();
      stopwatch.stop();
      const auto m = duration_cast<std::chrono::milliseconds>(elapsed_time);
      std::cout << "  Elapsed time: " << m.count() << " ms" << std::endl;
    }

    {
      auto image_name = "cmj256_performance_device" + std::to_string(number) +
                        ".ppm";
      std::ofstream image{image_name, std::ios_base::binary};
      image << "P6" << std::endl;
      image << "#" << std::endl;
      image << resolution_x << " " << resolution_y << std::endl;
      image << 255 << std::endl;

      std::vector<cl::float4> colors;
      colors.resize(resolution);
      color_buffer->read(colors.data(), colors.size(), 0, 0);

      for (uint32b i = 0; i < colors.size(); ++i) {
        const cl::float4 color = 256.0f * colors[i];
        for (uint32b c = 0; c < 3; ++c) {
          const uint32b v = zisc::cast<uint32b>(color[c]);
          if (256 <= v)
            std::cerr << "256 is happened: " << c << std::endl;
          image << zisc::cast<uint8b>(v);
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
