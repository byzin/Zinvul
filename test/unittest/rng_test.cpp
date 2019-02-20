///*!
//  \file rng_test.cpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015-2019 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//// Standard C++ library
//#include <cstddef>
//#include <fstream>
//#include <iomanip>
//#include <iostream>
//#include <string>
//#include <vector>
//// GoogleTest
//#include "gtest/gtest.h"
//// Zisc
//#include "zisc/correlated_multi_jittered_engine.hpp"
//// Zinvul
//#include "zinvul/zinvul.hpp"
//#include "zinvul/rng.hpp"
//// Test
//#include "test.hpp"
//
//TEST(RngTest, Cmj64Test)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b seed = 123456789u;
//    constexpr uint32b root_n = 8;
//    constexpr uint32b num_of_samples = 100;
//    auto result_1d = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
//    result_1d->setSize(num_of_samples);
//    auto result_2d = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
//    result_2d->setSize(num_of_samples);
//    auto seed_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
//    seed_buffer->setSize(1);
//    seed_buffer->write(&seed, seed_buffer->size(), 0, 0);
//    auto root_n_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
//    root_n_buffer->setSize(1);
//    root_n_buffer->write(&root_n, root_n_buffer->size(), 0, 0);
//    auto iteration_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
//    iteration_buffer->setSize(1);
//    iteration_buffer->write(&num_of_samples, iteration_buffer->size(), 0, 0);
//
//    auto kernel = makeZinvulKernel(device.get(), rng, testCmj, 1);
//    kernel->run(*result_1d, *result_2d, *seed_buffer, *root_n_buffer, *iteration_buffer, {1}, 0);
//    device->waitForCompletion();
//
//    constexpr uint32b n = root_n * root_n;
//    using CmjEngine = zisc::CorrelatedMultiJitteredEngine<root_n>;
//    {
//      std::vector<float> results;
//      results.resize(num_of_samples);
//      result_1d->read(results.data(), num_of_samples, 0, 0);
//      for (uint32b sample = 0; sample < num_of_samples; ++sample) {
//        const uint32b s = sample / n;
//        const uint32b p = seed + sample % n;
//        const auto expected = CmjEngine::template generate1D<float>(s, p);
//        EXPECT_FLOAT_EQ(expected, results[sample])
//            << "zCmjGenerate1D(" << s << "," << p << ") is wrong.";
//      }
//    }
//    {
//      std::vector<cl::float2> results;
//      results.resize(num_of_samples);
//      result_2d->read(results.data(), num_of_samples, 0, 0);
//      for (uint32b sample = 0; sample < num_of_samples; ++sample) {
//        const uint32b s = sample / n;
//        const uint32b p = seed + sample % n;
//        const auto expected = CmjEngine::template generate2D<float>(s, p);
//        for (std::size_t i = 0; i < 2; ++i) {
//          EXPECT_FLOAT_EQ(expected[i], results[sample][i])
//              << "zCmjGenerate2D(" << s << "," << p << ") is wrong.";
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
////TEST(RngTest, Cmj81Test)
////{
////  using namespace zinvul;
////  auto options = makeTestOptions();
////  auto device_list = makeTestDeviceList(options);
////  for (std::size_t number = 0; number < device_list.size(); ++number) {
////    auto& device = device_list[number];
////    std::cout << getTestDeviceInfo(*device);
////
////    constexpr uint32b seed = 123456789u;
////    constexpr uint32b root_n = 9;
////    constexpr uint32b num_of_samples = 100;
////    auto result_1d = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
////    result_1d->setSize(num_of_samples);
////    auto result_2d = makeBuffer<cl::float2>(device.get(), BufferUsage::kDeviceSrc);
////    result_2d->setSize(num_of_samples);
////    auto seed_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
////    seed_buffer->setSize(1);
////    seed_buffer->write(&seed, seed_buffer->size(), 0, 0);
////    auto root_n_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
////    root_n_buffer->setSize(1);
////    root_n_buffer->write(&root_n, root_n_buffer->size(), 0, 0);
////    auto iteration_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
////    iteration_buffer->setSize(1);
////    iteration_buffer->write(&num_of_samples, iteration_buffer->size(), 0, 0);
////
////    auto kernel = makeZinvulKernel(device.get(), rng, testCmj, 1);
////    kernel->run(*result_1d, *result_2d, *seed_buffer, *root_n_buffer, *iteration_buffer, {1}, 0);
////    device->waitForCompletion();
////
////    constexpr uint32b n = root_n * root_n;
////    using CmjEngine = zisc::CorrelatedMultiJitteredEngine<root_n>;
////    {
////      std::vector<float> results;
////      results.resize(num_of_samples);
////      result_1d->read(results.data(), num_of_samples, 0, 0);
////      for (uint32b sample = 0; sample < num_of_samples; ++sample) {
////        const uint32b s = sample / n;
////        const uint32b p = seed + sample % n;
////        const auto expected = CmjEngine::template generate1D<float>(s, p);
////        EXPECT_FLOAT_EQ(expected, results[sample])
////            << "zCmjGenerate1D(" << s << "," << p << ") is wrong.";
////      }
////    }
////    {
////      std::vector<cl::float2> results;
////      results.resize(num_of_samples);
////      result_2d->read(results.data(), num_of_samples, 0, 0);
////      for (uint32b sample = 0; sample < num_of_samples; ++sample) {
////        const uint32b s = sample / n;
////        const uint32b p = seed + sample % n;
////        const auto expected = CmjEngine::template generate2D<float>(s, p);
////        for (std::size_t i = 0; i < 2; ++i) {
////          EXPECT_FLOAT_EQ(expected[i], results[sample][i])
////              << "zCmjGenerate2D(" << s << "," << p << ") is wrong.";
////        }
////      }
////    }
////
////    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
////  }
////}
//
//TEST(RngTest, Cmj64ImageTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (std::size_t number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution_x = 1920;
//    constexpr uint32b resolution_y = 1080;
//    constexpr uint32b resolution = resolution_x * resolution_y;
//    constexpr uint32b root_n = 8;
//    auto image_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
//    image_buffer->setSize(3 * resolution);
//    auto iteration_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
//    iteration_buffer->setSize(1);
//    auto root_n_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
//    root_n_buffer->setSize(1);
//    root_n_buffer->write(&root_n, root_n_buffer->size(), 0, 0);
//    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = makeZinvulKernel(device.get(), rng, testCmjImage, 1);
//    for (uint32b sample = 0; sample < 3; ++sample) {
//      iteration_buffer->write(&sample, iteration_buffer->size(), 0, 0);
//      kernel->run(*image_buffer, *iteration_buffer, *root_n_buffer, *resolution_buffer, {resolution}, 0);
//    }
//    device->waitForCompletion();
//
//    {
//      auto image_name = "cmj64_image_device" + std::to_string(number) + ".ppm";
//      std::ofstream image{image_name, std::ios_base::binary};
//      image << "P6" << std::endl;
//      image << "#" << std::endl;
//      image << resolution_x << " " << resolution_y << std::endl;
//      image << 255 << std::endl;
//
//      std::vector<float> results;
//      results.resize(3 * resolution);
//      image_buffer->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        for (uint32b color = 0; color < 3; ++color) {
//          const uint32b idx = 3 * i + color;
//          const float v = 256.0f * results[idx];
//          const uint32b c = zisc::cast<uint32b>(v);
//          if (256 <= c)
//            std::cerr << "256 is happened: " << c << std::endl;
//          image << zisc::cast<uint8b>(c);
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
////TEST(RngTest, Cmj81ImageTest)
////{
////  using namespace zinvul;
////  auto options = makeTestOptions();
////  auto device_list = makeTestDeviceList(options);
////  for (std::size_t number = 0; number < device_list.size(); ++number) {
////    auto& device = device_list[number];
////    std::cout << getTestDeviceInfo(*device);
////
////    constexpr uint32b resolution_x = 1920;
////    constexpr uint32b resolution_y = 1080;
////    constexpr uint32b resolution = resolution_x * resolution_y;
////    constexpr uint32b root_n = 9;
////    auto image_buffer = makeBuffer<float>(device.get(), BufferUsage::kDeviceSrc);
////    image_buffer->setSize(3 * resolution);
////    auto iteration_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
////    iteration_buffer->setSize(1);
////    auto root_n_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
////    root_n_buffer->setSize(1);
////    root_n_buffer->write(&root_n, root_n_buffer->size(), 0, 0);
////    auto resolution_buffer = makeBuffer<uint32b>(device.get(), BufferUsage::kDeviceDst);
////    resolution_buffer->setSize(1);
////    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
////
////    auto kernel = makeZinvulKernel(device.get(), rng, testCmjImage, 1);
////    for (uint32b sample = 0; sample < 3; ++sample) {
////      iteration_buffer->write(&sample, iteration_buffer->size(), 0, 0);
////      kernel->run(*image_buffer, *iteration_buffer, *root_n_buffer, *resolution_buffer, {resolution}, 0);
////    }
////    device->waitForCompletion();
////
////    {
////      auto image_name = "cmj81_image_device" + std::to_string(number) + ".ppm";
////      std::ofstream image{image_name, std::ios_base::binary};
////      image << "P6" << std::endl;
////      image << "#" << std::endl;
////      image << resolution_x << " " << resolution_y << std::endl;
////      image << 255 << std::endl;
////
////      std::vector<float> results;
////      results.resize(3 * resolution);
////      image_buffer->read(results.data(), results.size(), 0, 0);
////      for (uint32b i = 0; i < resolution; ++i) {
////        for (uint32b color = 0; color < 3; ++color) {
////          const uint32b idx = 3 * i + color;
////          const float v = 256.0f * results[idx];
////          const uint32b c = zisc::cast<uint32b>(v);
////          if (256 <= c)
////            std::cerr << "256 is happened: " << c << std::endl;
////          image << zisc::cast<uint8b>(c);
////        }
////      }
////    }
////
////    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
////  }
////}
