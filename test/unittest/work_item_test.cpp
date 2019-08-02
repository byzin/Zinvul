/*!
  \file work_item_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
#include "zinvul/kernel_set/work_item.hpp"
// Test
#include "test.hpp"

TEST(WorkItemTest, WorkItem1DTest)
{
  using namespace zinvul;

  constexpr uint resolution = 1000;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(9);
    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(resolution);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(resolution);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(work_item, testWorkItem1D));

    kernel->run(*buffer1, *buffer2, *buffer3, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    const char* error_message = "The work-item functions are wrong.";
    {
      std::cout << "  resolutions: " <<  resolution << std::endl;

      std::vector<uint32b> results;
      results.resize(9, 0u);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::cout << "  group size[0]: " << results[0] << std::endl;
      std::cout << "  group size[1]: " << results[1] << std::endl;
      std::cout << "  group size[2]: " << results[2] << std::endl;
      std::cout << "  local size[0]: " << results[3] << std::endl;
      std::cout << "  local size[1]: " << results[4] << std::endl;
      std::cout << "  local size[2]: " << results[5] << std::endl;
      std::cout << "  global size[0]: " << results[6] << std::endl;
      std::cout << "  global size[1]: " << results[7] << std::endl;
      std::cout << "  global size[2]: " << results[8] << std::endl;

      const std::size_t num_of_works = results[6] * results[7] * results[8];
      std::cout << "  num of works: " << num_of_works << std::endl;
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]) << error_message;
    }

    {
      uint32b c = 0;
      buffer3->read(&c, 1, 0, 0);
      std::cout << "  out of range: " << c << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(WorkItemTest, WorkItem2DTest)
{
  using namespace zinvul;

  const cl::uint2 resolution{40, 25};
  const std::size_t res = resolution.x * resolution.y;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(9);
    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(res);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(res);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<cl::uint2>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<2>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(work_item, testWorkItem2D));
    kernel->run(*buffer1, *buffer2, *buffer3, *res_buff, {resolution.x, resolution.y}, 0);
    device->waitForCompletion();

    const char* error_message = "The work-item functions are wrong.";
    {
      std::cout << "  resolutions: " <<  res << std::endl;

      std::vector<uint32b> results;
      results.resize(9, 0u);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::cout << "  group size[0]: " << results[0] << std::endl;
      std::cout << "  group size[1]: " << results[1] << std::endl;
      std::cout << "  group size[2]: " << results[2] << std::endl;
      std::cout << "  local size[0]: " << results[3] << std::endl;
      std::cout << "  local size[1]: " << results[4] << std::endl;
      std::cout << "  local size[2]: " << results[5] << std::endl;
      std::cout << "  global size[0]: " << results[6] << std::endl;
      std::cout << "  global size[1]: " << results[7] << std::endl;
      std::cout << "  global size[2]: " << results[8] << std::endl;

      const std::size_t num_of_works = results[6] * results[7] * results[8];
      std::cout << "  num of works: " << num_of_works << std::endl;
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]) << error_message;
    }

    {
      uint32b c = 0;
      buffer3->read(&c, 1, 0, 0);
      std::cout << "  out of range: " << c << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(WorkItemTest, WorkItem3DTest)
{
  using namespace zinvul;

  const cl::uint3 resolution{10, 10, 10};
  const std::size_t res = resolution.x * resolution.y * resolution.z;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (std::size_t number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    auto buffer1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(9);
    auto buffer2 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(res);
    {
      std::vector<uint32b> work_item_table;
      work_item_table.resize(buffer2->size(), 0u);
      buffer2->write(work_item_table.data(), work_item_table.size(), 0, 0);
    }
    auto buffer3 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer3->setSize(res);
    {
      const uint32b c = 0;
      buffer3->write(&c, 1, 0, 0);
    }
    auto res_buff = makeUniformBuffer<cl::uint3>(device.get(), BufferUsage::kHostToDevice);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<3>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(work_item, testWorkItem3D));
    kernel->run(*buffer1, *buffer2, *buffer3, *res_buff, {resolution.x, resolution.y, resolution.z}, 0);
    device->waitForCompletion();

    const char* error_message = "The work-item functions are wrong.";
    {
      std::cout << "  resolutions: " <<  res << std::endl;

      std::vector<uint32b> results;
      results.resize(9, 0u);
      buffer1->read(results.data(), results.size(), 0, 0);
      std::cout << "  group size[0]: " << results[0] << std::endl;
      std::cout << "  group size[1]: " << results[1] << std::endl;
      std::cout << "  group size[2]: " << results[2] << std::endl;
      std::cout << "  local size[0]: " << results[3] << std::endl;
      std::cout << "  local size[1]: " << results[4] << std::endl;
      std::cout << "  local size[2]: " << results[5] << std::endl;
      std::cout << "  global size[0]: " << results[6] << std::endl;
      std::cout << "  global size[1]: " << results[7] << std::endl;
      std::cout << "  global size[2]: " << results[8] << std::endl;

      const std::size_t num_of_works = results[6] * results[7] * results[8];
      std::cout << "  num of works: " << num_of_works << std::endl;
    }

    {
      std::vector<uint32b> results;
      results.resize(buffer2->size(), 0u);
      buffer2->read(results.data(), results.size(), 0, 0);
      for (std::size_t i = 0; i < results.size(); ++i)
        ASSERT_TRUE(results[i]) << error_message;
    }

    {
      uint32b c = 0;
      buffer3->read(&c, 1, 0, 0);
      std::cout << "  out of range: " << c << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}
