/*!
  \file math_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/compensated_summation.hpp"
#include "zisc/floating_point.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "zinvul/zinvul.hpp"
#include "zinvul/kernel_set/math.hpp"
// Test
#include "test.hpp"

namespace {

template <typename Float>
void printFloat(const char* message, const Float x)
{
  using FloatData = zisc::FloatingPointFromBytes<sizeof(Float)>;
  std::cout << std::setfill(' ') << std::setw(24) << message << ": "
            << std::scientific
            << std::setfill(' ') << std::setw(26)
            << std::setprecision(std::numeric_limits<Float>::max_digits10)
            << x;
  if (sizeof(FloatData) == 4)
    std::cout << "f";
  const auto data = FloatData::fromFloat(x);
  std::cout << ", hex: " << std::hex
            << std::setfill(' ') << std::setw(18) << data.bits();
  std::cout << std::endl;
}

template <zinvul::uint32b kAllowableUlps = 2,
          zinvul::uint32b kCautionedUlps = 8192,
          typename Float> 
void testFloat(const char* func_name,
    const Float x,
    const Float* y,
    const Float expected,
    const Float result,
    zinvul::uint32b& num_of_trials,
    zinvul::uint32b& num_of_failures,
    zinvul::uint32b& num_of_inf_failures,
    zinvul::uint32b& num_of_nan_failures,
    zisc::CompensatedSummation<Float>& sum_ulps,
    Float& max_ulps,
    Float& max_diff,
    const bool print_failure_values = true)
{
  using namespace zinvul;
  using namespace zisc;

  char result_str[256];
  auto get_result_str = [func_name, x, y, expected, result, &result_str]()
  {
    char expression[64];
    if (y != nullptr) {
      std::sprintf(expression, "%s(%g, %g)", func_name, cast<double>(x),
                                                        cast<double>(*y));
    }
    else {
      std::sprintf(expression, "%s(%g)", func_name, cast<double>(x));
    }
    std::string_view expression_str{expression};

    std::sprintf(result_str,
                 "- %s = %g, hex = %x\n  %*s = %g, hex = %x",
                 expression_str.data(),
                 cast<double>(result),
                 *treatAs<const int*>(&result),
                 cast<int>(expression_str.size()),
                 "expected",
                 cast<double>(expected),
                 *treatAs<const int*>(&expected));
  };

  ++num_of_trials;
  if (std::isfinite(result)) {
    if (!constant::Math::isAlmostEqual<kAllowableUlps>(expected, result)) {
      ++num_of_failures;
      const auto diff = std::abs(expected - result);
      const auto ulps = diff / constant::Math::getUlps<1>(expected + result);
      if (std::isfinite(ulps)) {
        sum_ulps.add(ulps);
        max_ulps = std::max(max_ulps, ulps);
      }
      if (!std::isnan(diff)) {
        max_diff = std::max(max_diff, diff);
      }
//      if (print_failure_values &&
//          (std::numeric_limits<Float>::min() <= diff) &&
//          !constant::Math::isAlmostEqual<kCautionedUlps>(expected, result)) {
//        get_result_str();
//        std::cout << result_str << std::endl;
//      }
    }
  }
  else if (std::isinf(result) && !std::isinf(expected)) {
    ++num_of_inf_failures;
    if (print_failure_values) {
      get_result_str();
      std::cout << result_str << std::endl;
    }
  }
  else if (std::isnan(result) && !std::isnan(expected)) {
    ++num_of_nan_failures;
    if (print_failure_values) {
      get_result_str();
      std::cout << result_str << std::endl;
    }
  }
}

} // namespace 

TEST(MathTest, ConstantValueTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b ni = 7;
    constexpr uint32b nf = 23;

    auto buffer1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(ni);
    auto buffer2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(nf);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testConstantValue));
    kernel->run(*buffer1, *buffer2, {1}, 0);
    device->waitForCompletion();

    {
      std::vector<int32b> results;
      results.resize(ni);
      buffer1->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      EXPECT_EQ(6, results[i++]) << "FLT_DIG is wrong.";
      EXPECT_EQ(24, results[i++]) << "FLT_MANT_DIG is wrong.";
      EXPECT_EQ(+38, results[i++]) << "FLT_MAX_10_EXP is wrong.";
      EXPECT_EQ(+128, results[i++]) << "FLT_MAX_EXP is wrong.";
      EXPECT_EQ(-37, results[i++]) << "FLT_MIN_10_EXP is wrong.";
      EXPECT_EQ(-125, results[i++]) << "FLT_MIN_EXP is wrong.";
      EXPECT_EQ(2, results[i++]) << "FLT_RADIX is wrong.";
    }
    {
      std::vector<float> results;
      results.resize(nf);
      buffer2->read(results.data(), results.size(), 0, 0);
      uint32b i = 0;
      {
        const float expected = std::numeric_limits<float>::max();
        ::printFloat("expected max", expected);
        const float result = results[i++];
        ::printFloat("zinvul max", result);
        EXPECT_FLOAT_EQ(expected, result) << "FLT_MAX doesn't match.";
      }
      {
        const float expected = std::numeric_limits<float>::min();
        ::printFloat("expected min", expected);
        const float result = results[i++];
        ::printFloat("zinvul min", result);
        EXPECT_FLOAT_EQ(expected, result) << "FLT_MIN doesn't match.";
      }
      {
        const float expected = std::numeric_limits<float>::epsilon();
        ::printFloat("expected epsilon", expected);
        const float result = results[i++];
        ::printFloat("zinvul epsilon", result);
        EXPECT_FLOAT_EQ(expected, result) << "FLT_EPSILON doesn't match.";
      }
      {
        const float expected = std::exp(1.0f);
        ::printFloat("expected e", expected);
        const float result = results[i++];
        ::printFloat("zinvul e", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_E_F doesn't match.";
      }
      {
        const float e = std::exp(1.0f);
        const float expected = std::log2(e);
        ::printFloat("expected log2(e)", expected);
        const float result = results[i++];
        ::printFloat("zinvul log2(e)", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_LOG2E_F doesn't match.";
      }
      {
        const float e = std::exp(1.0f);
        const float expected = std::log10(e);
        ::printFloat("expected log10(e)", expected);
        const float result = results[i++];
        ::printFloat("zinvul log10(e)", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_LOG10E_F doesn't match.";
      }
      {
        const float expected = std::log(2.0f);
        ::printFloat("expected log(2)", expected);
        const float result = results[i++];
        ::printFloat("zinvul log(2)", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_LN2_F doesn't match.";
      }
      {
        const float expected = std::log(10.0f);
        ::printFloat("expected log(10)", expected);
        const float result = results[i++];
        ::printFloat("zinvul log(10)", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_LN10_F doesn't match.";
      }
      {
        const float expected = 4.0f * std::atan(1.0f);
        ::printFloat("expected pi", expected);
        const float result = results[i++];
        ::printFloat("zinvul pi", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_PI_F doesn't match.";
      }
      {
        const float expected = 2.0f * std::atan(1.0f);
        ::printFloat("expected pi/2", expected);
        const float result = results[i++];
        ::printFloat("zinvul pi/2", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_PI_2_F doesn't match.";
      }
      {
        const float expected = std::atan(1.0f);
        ::printFloat("expected pi/4", expected);
        const float result = results[i++];
        ::printFloat("zinvul pi/4", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_PI_4_F doesn't match.";
      }
      {
        const float expected = 0.25f / std::atan(1.0f);
        ::printFloat("expected 1/pi", expected);
        const float result = results[i++];
        ::printFloat("zinvul 1/pi", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_1_PI_F doesn't match.";
      }
      {
        const float expected = 0.5f / std::atan(1.0f);
        ::printFloat("expected 2/pi", expected);
        const float result = results[i++];
        ::printFloat("zinvul 2/pi", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_2_PI_F doesn't match.";
      }
      {
        const float pi = 4.0f * std::atan(1.0f);
        const float expected = 2.0f / std::sqrt(pi);
        ::printFloat("expected 2/sqrt(pi)", expected);
        const float result = results[i++];
        ::printFloat("zinvul 2/sqrt(pi)", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_2_SQRTPI_F doesn't match.";
      }
      {
        const float expected = std::sqrt(2.0f);
        ::printFloat("expected sqrt(2)", expected);
        const float result = results[i++];
        ::printFloat("zinvul sqrt(2)", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_SQRT2_F doesn't match.";
      }
      {
        const float expected = 1.0f / std::sqrt(2.0f);
        ::printFloat("expected 1/sqrt(2)", expected);
        const float result = results[i++];
        ::printFloat("zinvul 1/sqrt(2)", result);
        EXPECT_FLOAT_EQ(expected, result) << "M_SQRT1_2_F doesn't match.";
      }

      {
        const float expected = 4.0f * std::atan(1.0f);
        ::printFloat("expected pi", expected);
        const float result = results[i++];
        ::printFloat("zinvul zpi", result);
        EXPECT_FLOAT_EQ(expected, result) << "pi doesn't match.";
      }
      {
        const float pi = 4.0f * std::atan(1.0f);
        const float expected = std::sqrt(pi);
        ::printFloat("expected sqrt(pi)", expected);
        const float result = results[i++];
        ::printFloat("zinvul zsqrt(pi)", result);
        EXPECT_FLOAT_EQ(expected, result) << "sqrt(pi) doesn't match.";
      }
      {
        const float expected = std::sqrt(2.0f);
        ::printFloat("expected sqrt(2)", expected);
        const float result = results[i++];
        ::printFloat("zinvul zsqrt(2)", result);
        EXPECT_FLOAT_EQ(expected, result) << "sqrt(2) doesn't match.";
      }
      {
        const float expected = std::sqrt(3.0f);
        ::printFloat("expected sqrt(3)", expected);
        const float result = results[i++];
        ::printFloat("zinvul zsqrt(3)", result);
        EXPECT_FLOAT_EQ(expected, result) << "sqrt(3) doesn't match.";
      }
      {
        const float pi = 4.0f * std::atan(1.0f);
        const float expected = std::cbrt(pi);
        ::printFloat("expected cbrt(pi)", expected);
        const float result = results[i++];
        ::printFloat("zinvul zcbrt(pi)", result);
        EXPECT_FLOAT_EQ(expected, result) << "cbrt(pi) doesn't match.";
      }
      {
        const float expected = std::cbrt(2.0f);
        ::printFloat("expected cbrt(2)", expected);
        const float result = results[i++];
        ::printFloat("zinvul zcbrt(2)", result);
        EXPECT_FLOAT_EQ(expected, result) << "cbrt(2) doesn't match.";
      }
      {
        const float expected = std::cbrt(3.0f);
        ::printFloat("expected cbrt(3)", expected);
        const float result = results[i++];
        ::printFloat("zinvul zcbrt(3)", result);
        EXPECT_FLOAT_EQ(expected, result) << "cbrt(3) doesn't match.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

#if !defined(Z_MAC)

TEST(MathTest, ConstantValue64Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b ni = 7;
    constexpr uint32b nf = 23;

    auto buffer1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    buffer1->setSize(ni);
    auto buffer2 = makeStorageBuffer<double>(device.get(), BufferUsage::kDeviceOnly);
    buffer2->setSize(nf);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testConstantValue64));
    kernel->run(*buffer1, *buffer2, {1}, 0);
    device->waitForCompletion();

    {
      std::vector<int32b> results;
      results.resize(ni);
      buffer1->read(results.data(), results.size(), 0, 0);

      uint32b i = 0;
      EXPECT_EQ(15, results[i++]) << "DBL_DIG is wrong.";
      EXPECT_EQ(53, results[i++]) << "DBL_MANT_DIG is wrong.";
      EXPECT_EQ(+308, results[i++]) << "DBL_MAX_10_EXP is wrong.";
      EXPECT_EQ(+1024, results[i++]) << "DBL_MAX_EXP is wrong.";
      EXPECT_EQ(-307, results[i++]) << "DBL_MIN_10_EXP is wrong.";
      EXPECT_EQ(-1021, results[i++]) << "DBL_MIN_EXP is wrong.";
    }
    {
      std::vector<double> results;
      results.resize(nf);
      buffer2->read(results.data(), results.size(), 0, 0);
      uint32b i = 0;
      {
        const double expected = std::numeric_limits<double>::max();
        ::printFloat("expected max", expected);
        const double result = results[i++];
        ::printFloat("zinvul max", result);
        EXPECT_DOUBLE_EQ(expected, result) << "DBL_MAX doesn't match.";
      }
      {
        const double expected = std::numeric_limits<double>::min();
        ::printFloat("expected min", expected);
        const double result = results[i++];
        ::printFloat("zinvul min", result);
        EXPECT_DOUBLE_EQ(expected, result) << "DBL_MIN doesn't match.";
      }
      {
        const double expected = std::numeric_limits<double>::epsilon();
        ::printFloat("expected epsilon", expected);
        const double result = results[i++];
        ::printFloat("zinvul epsilon", result);
        EXPECT_DOUBLE_EQ(expected, result) << "DBL_EPSILON doesn't match.";
      }
      {
        const double expected = std::exp(1.0);
        ::printFloat("expected e", expected);
        const double result = results[i++];
        ::printFloat("zinvul e", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_E doesn't match.";
      }
      {
        const double e = std::exp(1.0);
        const double expected = std::log2(e);
        ::printFloat("expected log2(e)", expected);
        const double result = results[i++];
        ::printFloat("zinvul log2(e)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_LOG2E doesn't match.";
      }
      {
        const double e = std::exp(1.0);
        const double expected = std::log10(e);
        ::printFloat("expected log10(e)", expected);
        const double result = results[i++];
        ::printFloat("zinvul log10(e)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_LOG10E doesn't match.";
      }
      {
        const double expected = std::log(2.0);
        ::printFloat("expected log(2)", expected);
        const double result = results[i++];
        ::printFloat("zinvul log(2)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_LN2 doesn't match.";
      }
      {
        const double expected = std::log(10.0);
        ::printFloat("expected log(10)", expected);
        const double result = results[i++];
        ::printFloat("zinvul log(10)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_LN10 doesn't match.";
      }
      {
        const double expected = 4.0 * std::atan(1.0);
        ::printFloat("expected pi", expected);
        const double result = results[i++];
        ::printFloat("zinvul pi", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_PI doesn't match.";
      }
      {
        const double expected = 2.0 * std::atan(1.0);
        ::printFloat("expected pi/2", expected);
        const double result = results[i++];
        ::printFloat("zinvul pi/2", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_PI_2 doesn't match.";
      }
      {
        const double expected = std::atan(1.0);
        ::printFloat("expected pi/4", expected);
        const double result = results[i++];
        ::printFloat("zinvul pi/4", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_PI_4 doesn't match.";
      }
      {
        const double expected = 0.25 / std::atan(1.0);
        ::printFloat("expected 1/pi", expected);
        const double result = results[i++];
        ::printFloat("zinvul 1/pi", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_1_PI doesn't match.";
      }
      {
        const double expected = 0.5 / std::atan(1.0);
        ::printFloat("expected 2/pi", expected);
        const double result = results[i++];
        ::printFloat("zinvul 2/pi", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_2_PI doesn't match.";
      }
      {
        const double pi = 4.0 * std::atan(1.0);
        const double expected = 2.0 / std::sqrt(pi);
        ::printFloat("expected 2/sqrt(pi)", expected);
        const double result = results[i++];
        ::printFloat("zinvul 2/sqrt(pi)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_2_SQRTPI doesn't match.";
      }
      {
        const double expected = std::sqrt(2.0);
        ::printFloat("expected sqrt(2)", expected);
        const double result = results[i++];
        ::printFloat("zinvul sqrt(2)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_SQRT2 doesn't match.";
      }
      {
        const double expected = 1.0 / std::sqrt(2.0);
        ::printFloat("expected 1/sqrt(2)", expected);
        const double result = results[i++];
        ::printFloat("zinvul 1/sqrt(2)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "M_SQRT1_2 doesn't match.";
      }

      {
        const double expected = 4.0 * std::atan(1.0);
        ::printFloat("expected pi", expected);
        const double result = results[i++];
        ::printFloat("zinvul zpi", result);
        EXPECT_DOUBLE_EQ(expected, result) << "pi doesn't match.";
      }
      {
        const double pi = 4.0 * std::atan(1.0);
        const double expected = std::sqrt(pi);
        ::printFloat("expected sqrt(pi)", expected);
        const double result = results[i++];
        ::printFloat("zinvul zsqrt(pi)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "sqrt(pi) doesn't match.";
      }
      {
        const double expected = std::sqrt(2.0);
        ::printFloat("expected sqrt(2)", expected);
        const double result = results[i++];
        ::printFloat("zinvul zsqrt(2)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "sqrt(2) doesn't match.";
      }
      {
        const double expected = std::sqrt(3.0);
        ::printFloat("expected sqrt(3)", expected);
        const double result = results[i++];
        ::printFloat("zinvul zsqrt(3)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "sqrt(3) doesn't match.";
      }
      {
        const double pi = 4.0 * std::atan(1.0);
        const double expected = std::cbrt(pi);
        ::printFloat("expected cbrt(pi)", expected);
        const double result = results[i++];
        ::printFloat("zinvul zcbrt(pi)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "cbrt(pi) doesn't match.";
      }
      {
        const double expected = std::cbrt(2.0);
        ::printFloat("expected cbrt(2)", expected);
        const double result = results[i++];
        ::printFloat("zinvul zcbrt(2)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "cbrt(2) doesn't match.";
      }
      {
        const double expected = std::cbrt(3.0);
        ::printFloat("expected cbrt(3)", expected);
        const double result = results[i++];
        ::printFloat("zinvul zcbrt(3)", result);
        EXPECT_DOUBLE_EQ(expected, result) << "cbrt(3) doesn't match.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

#endif // !defined(Z_MAC)

TEST(MathTest, MakeNormalTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 1024;

    auto pinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    pinput_buff->setSize(resolution);
    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(resolution);
    auto yinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    yinput_buff->setSize(resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testMakeNormal));
    kernel->run(*pinput_buff, *input_buff, *yinput_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> presults;
      presults.resize(pinput_buff->size());
      pinput_buff->read(presults.data(), presults.size(), 0, 0);

      for (uint32b i = 0; i < presults.size(); ++i) {
        const auto expected = cl::math::test::makeNormalP<float>(i, resolution);
        const auto result = presults[i];
        ASSERT_FLOAT_EQ(expected, result)
          << "'makeNormalP(" << i << ", " << resolution << ")' failed.";
      }
    }
    {
      std::vector<float> results;
      results.resize(input_buff->size());
      input_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < results.size(); ++i) {
        const auto expected = cl::math::test::makeNormal<float>(i, resolution);
        const auto result = results[i];
        ASSERT_FLOAT_EQ(expected, result) << std::scientific
          << "'makeNormal(" << i << ", " << resolution << ")' failed.";
      }
    }
    {
      std::vector<float> results;
      results.resize(yinput_buff->size());
      yinput_buff->read(results.data(), results.size(), 0, 0);

      for (uint32b i = 0; i < results.size(); ++i) {
        const auto expected = cl::math::test::makeNormalY<float>(i);
        const auto result = results[i];
        ASSERT_FLOAT_EQ(expected, result) << std::scientific
          << "'makeNormalY(" << i << ", " << resolution << ")' failed.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CeilTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCeil));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::ceil(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("ceil", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'ceil' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;


    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FloorTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFloor));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::floor(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("floor", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }

      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'floor' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, TruncTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testTrunc));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::trunc(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("trunc", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'trunc' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RoundTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRound));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::round(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("round", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'round' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RoundBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRoundBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::round(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::round", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::round' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RoundZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRoundZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::round(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::round", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::round' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RintTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRint));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
//        const auto expected = std::rint(x);
        const auto expected = std::round(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("rint", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'rint' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RintBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRintBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
//        const auto expected = std::rint(x);
        const auto expected = std::round(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::rint", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::rint' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RintZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRintZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
//        const auto expected = std::rint(x);
        const auto expected = std::round(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::rint", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::rint' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ExpTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testExp));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::exp(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("exp", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'exp' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ExpBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testExpBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::exp(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::exp", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::exp' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, ExpZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testExpZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::exp(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::exp", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::exp' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, LogTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testLog));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::log(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("log", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'log' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, LogBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testLogBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::log(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::log", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::log' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, LogZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testLogZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::log(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::log", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::log' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, Log2Test)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testLog2));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::log2(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("log2", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'log2' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, Log2BuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testLog2Builtin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::log2(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::log2", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::log2' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, Log2ZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testLog2Zinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::log2(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::log2", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::log2' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, PowTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto xinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    xinput_buff->setSize(n * resolution);
    auto yinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    yinput_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testPow));
    kernel->run(*xinput_buff, *yinput_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> xinputs;
      xinputs.resize(xinput_buff->size());
      xinput_buff->read(xinputs.data(), xinputs.size(), 0, 0);

      std::vector<float> yinputs;
      yinputs.resize(yinput_buff->size());
      yinput_buff->read(yinputs.data(), yinputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < xinputs.size(); ++i) {
        const auto x = xinputs[i];
        const auto y = yinputs[i];
        const auto expected = std::pow(x, y);
        const auto result = results[i];
        ::testFloat("pow", x, &y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'pow' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, PowBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto xinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    xinput_buff->setSize(n * resolution);
    auto yinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    yinput_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testPowBuiltin));
    kernel->run(*xinput_buff, *yinput_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> xinputs;
      xinputs.resize(xinput_buff->size());
      xinput_buff->read(xinputs.data(), xinputs.size(), 0, 0);

      std::vector<float> yinputs;
      yinputs.resize(yinput_buff->size());
      yinput_buff->read(yinputs.data(), yinputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < xinputs.size(); ++i) {
        const auto x = xinputs[i];
        const auto y = yinputs[i];
        const auto expected = std::pow(x, y);
        const auto result = results[i];
        ::testFloat("builtin::pow", x, &y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::pow' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, PowZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto xinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    xinput_buff->setSize(n * resolution);
    auto yinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    yinput_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testPowZinvul));
    kernel->run(*xinput_buff, *yinput_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> xinputs;
      xinputs.resize(xinput_buff->size());
      xinput_buff->read(xinputs.data(), xinputs.size(), 0, 0);

      std::vector<float> yinputs;
      yinputs.resize(yinput_buff->size());
      yinput_buff->read(yinputs.data(), yinputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < xinputs.size(); ++i) {
        const auto x = xinputs[i];
        const auto y = yinputs[i];
        const auto expected = std::pow(x, y);
        const auto result = results[i];
        ::testFloat("zinvul::pow", x, &y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::pow' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RsqrtTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRsqrt));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = zisc::Algorithm::invert(std::sqrt(x));
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("rsqrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'rsqrt' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RsqrtBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRsqrtBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = zisc::Algorithm::invert(std::sqrt(x));
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::rsqrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::rsqrt' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, RsqrtZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testRsqrtZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = zisc::Algorithm::invert(std::sqrt(x));
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::rsqrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::rsqrt' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SqrtTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSqrt));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sqrt(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("sqrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'sqrt' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SqrtBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSqrtBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sqrt(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::sqrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::sqrt' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SqrtZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSqrtZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sqrt(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::sqrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::sqrt' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("sin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'sin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SinBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSinBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::sin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::sin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SinZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSinZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::sin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::sin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CosTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCos));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::cos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("cos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'cos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CosBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCosBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::cos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::cos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::cos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CosZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCosZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::cos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::cos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::cos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, TanTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testTan));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::tan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("tan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'tan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, TanBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testTanBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::tan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::tan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::tan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, TanZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testTanZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::tan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::tan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::tan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SinPiTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSinPi));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("sin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'sin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SinPiBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSinPiBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::sin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::sin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, SinPiZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testSinPiZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::sin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::sin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::sin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CosPiTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCosPi));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::cos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("cos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'cos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CosPiBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCosPiBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::cos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::cos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::cos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CosPiZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCosPiZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::cos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::cos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::cos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, TanPiTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testTanPi));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::tan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("tan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'tan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, TanPiBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testTanPiBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::tan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::tan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::tan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, TanPiZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testTanPiZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::tan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::tan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::tan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AsinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAsin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::asin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("asin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'asin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AsinBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAsinBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::asin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::asin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::asin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AsinZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAsinZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::asin(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::asin", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::asin' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AcosTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAcos));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::acos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("acos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'acos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AcosBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAcosBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::acos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::acos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::acos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AcosZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAcosZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::acos(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::acos", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::acos' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AtanTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAtan));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::atan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("atan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'atan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AtanBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAtanBuiltin));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::atan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::atan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::atan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, AtanZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testAtanZinvul));
    kernel->run(*input_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        const auto expected = std::atan(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::atan", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::atan' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FractTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto fresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    fresult_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFract));
    kernel->run(*input_buff, *result_buff, *fresult_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      std::vector<float> fresults;
      fresults.resize(fresult_buff->size());
      fresult_buff->read(fresults.data(), fresults.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        float fexpected = 0.0f;
        const auto expected = cl::fract(x, &fexpected);
        const auto result = results[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(expected, result)
              << "[" << i << "]: fract(" << x << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "[" << i << "]: fract(" << x << ") failed.";
        }
        const auto fresult = fresults[i];
        ASSERT_FLOAT_EQ(fexpected, fresult)
              << "[" << i << "]: fract(" << x << ") failed.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FractBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto fresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    fresult_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFractBuiltin));
    kernel->run(*input_buff, *result_buff, *fresult_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      std::vector<float> fresults;
      fresults.resize(fresult_buff->size());
      fresult_buff->read(fresults.data(), fresults.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        float fexpected = 0.0f;
        const auto expected = cl::fract(x, &fexpected);
        const auto result = results[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(expected, result)
              << "[" << i << "]: fract(" << x << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "[" << i << "]: fract(" << x << ") failed.";
        }
        const auto fresult = fresults[i];
        ASSERT_FLOAT_EQ(fexpected, fresult)
              << "[" << i << "]: fract(" << x << ") failed.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FractZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto fresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    fresult_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFractZinvul));
    kernel->run(*input_buff, *result_buff, *fresult_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      std::vector<float> fresults;
      fresults.resize(fresult_buff->size());
      fresult_buff->read(fresults.data(), fresults.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        float fexpected = 0.0f;
        const auto expected = cl::fract(x, &fexpected);
        const auto result = results[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(expected, result)
              << "[" << i << "]: fract(" << x << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(result))
              << "[" << i << "]: fract(" << x << ") failed.";
        }
        const auto fresult = fresults[i];
        ASSERT_FLOAT_EQ(fexpected, fresult)
              << "[" << i << "]: fract(" << x << ") failed.";
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FmodTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto xinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    xinput_buff->setSize(n * resolution);
    auto yinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    yinput_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFmod));
    kernel->run(*xinput_buff, *yinput_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> xinputs;
      xinputs.resize(xinput_buff->size());
      xinput_buff->read(xinputs.data(), xinputs.size(), 0, 0);

      std::vector<float> yinputs;
      yinputs.resize(yinput_buff->size());
      yinput_buff->read(yinputs.data(), yinputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < xinputs.size(); ++i) {
        const auto x = xinputs[i];
        const auto y = yinputs[i];
        const auto expected = std::fmod(x, y);
        const auto result = results[i];
        ::testFloat("fmod", x, &y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'fmod' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FmodBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto xinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    xinput_buff->setSize(n * resolution);
    auto yinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    yinput_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFmodBuiltin));
    kernel->run(*xinput_buff, *yinput_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> xinputs;
      xinputs.resize(xinput_buff->size());
      xinput_buff->read(xinputs.data(), xinputs.size(), 0, 0);

      std::vector<float> yinputs;
      yinputs.resize(yinput_buff->size());
      yinput_buff->read(yinputs.data(), yinputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < xinputs.size(); ++i) {
        const auto x = xinputs[i];
        const auto y = yinputs[i];
        const auto expected = std::fmod(x, y);
        const auto result = results[i];
        ::testFloat("builtin::fmod", x, &y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'builtin::fmod' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FmodZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto xinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    xinput_buff->setSize(n * resolution);
    auto yinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    yinput_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFmodZinvul));
    kernel->run(*xinput_buff, *yinput_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> xinputs;
      xinputs.resize(xinput_buff->size());
      xinput_buff->read(xinputs.data(), xinputs.size(), 0, 0);

      std::vector<float> yinputs;
      yinputs.resize(yinput_buff->size());
      yinput_buff->read(yinputs.data(), yinputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < xinputs.size(); ++i) {
        const auto x = xinputs[i];
        const auto y = yinputs[i];
        const auto expected = std::fmod(x, y);
        const auto result = results[i];
        ::testFloat("zinvul::fmod", x, &y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::fmod' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FrLdexpTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto fresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    fresult_buff->setSize(n * resolution);
    auto eresult_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    eresult_buff->setSize(n * resolution);
    auto lresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    lresult_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFrLdexp));
    kernel->run(*input_buff, *fresult_buff, *eresult_buff, *lresult_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> fresults;
      fresults.resize(fresult_buff->size());
      fresult_buff->read(fresults.data(), fresults.size(), 0, 0);

      std::vector<int32b> eresults;
      eresults.resize(eresult_buff->size());
      eresult_buff->read(eresults.data(), eresults.size(), 0, 0);

      std::vector<float> lresults;
      lresults.resize(lresult_buff->size());
      lresult_buff->read(lresults.data(), lresults.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        int eexpected = 0;
        const auto fexpected = std::frexp(x, &eexpected);
        const auto fresult = fresults[i];
        const auto eresult = eresults[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(fexpected, fresult)
              << "[" << i << "]: frexp(" << x << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(fresult))
              << "[" << i << "]: frexp(" << x << ") failed.";
        }
        ASSERT_EQ(eexpected, eresult)
            << "[" << i << "]: frexp(" << x << ") failed.";
        const auto lexpected = std::ldexp(fexpected, eexpected);
        const auto lresult = lresults[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(lexpected, lresult)
              << "[" << i << "]: ldexp(" << fexpected << ", " << eexpected << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(lresult))
              << "[" << i << "]: ldexp(" << fexpected << ", " << eexpected << ") failed.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FrLdexpBuiltinTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto fresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    fresult_buff->setSize(n * resolution);
    auto eresult_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    eresult_buff->setSize(n * resolution);
    auto lresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    lresult_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFrLdexpBuiltin));
    kernel->run(*input_buff, *fresult_buff, *eresult_buff, *lresult_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> fresults;
      fresults.resize(fresult_buff->size());
      fresult_buff->read(fresults.data(), fresults.size(), 0, 0);

      std::vector<int32b> eresults;
      eresults.resize(eresult_buff->size());
      eresult_buff->read(eresults.data(), eresults.size(), 0, 0);

      std::vector<float> lresults;
      lresults.resize(lresult_buff->size());
      lresult_buff->read(lresults.data(), lresults.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        int eexpected = 0;
        const auto fexpected = std::frexp(x, &eexpected);
        const auto fresult = fresults[i];
        const auto eresult = eresults[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(fexpected, fresult)
              << "[" << i << "]: frexp(" << x << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(fresult))
              << "[" << i << "]: frexp(" << x << ") failed.";
        }
        ASSERT_EQ(eexpected, eresult)
            << "[" << i << "]: frexp(" << x << ") failed.";
        const auto lexpected = std::ldexp(fexpected, eexpected);
        const auto lresult = lresults[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(lexpected, lresult)
              << "[" << i << "]: ldexp(" << fexpected << ", " << eexpected << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(lresult))
              << "[" << i << "]: ldexp(" << fexpected << ", " << eexpected << ") failed.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, FrLdexpZinvulTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto input_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    input_buff->setSize(n * resolution);
    auto fresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    fresult_buff->setSize(n * resolution);
    auto eresult_buff = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceOnly);
    eresult_buff->setSize(n * resolution);
    auto lresult_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    lresult_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testFrLdexpZinvul));
    kernel->run(*input_buff, *fresult_buff, *eresult_buff, *lresult_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> inputs;
      inputs.resize(input_buff->size());
      input_buff->read(inputs.data(), inputs.size(), 0, 0);

      std::vector<float> fresults;
      fresults.resize(fresult_buff->size());
      fresult_buff->read(fresults.data(), fresults.size(), 0, 0);

      std::vector<int32b> eresults;
      eresults.resize(eresult_buff->size());
      eresult_buff->read(eresults.data(), eresults.size(), 0, 0);

      std::vector<float> lresults;
      lresults.resize(lresult_buff->size());
      lresult_buff->read(lresults.data(), lresults.size(), 0, 0);

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        int eexpected = 0;
        const auto fexpected = std::frexp(x, &eexpected);
        const auto fresult = fresults[i];
        const auto eresult = eresults[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(fexpected, fresult)
              << "[" << i << "]: frexp(" << x << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(fresult))
              << "[" << i << "]: frexp(" << x << ") failed.";
        }
        ASSERT_EQ(eexpected, eresult)
            << "[" << i << "]: frexp(" << x << ") failed.";
        const auto lexpected = std::ldexp(fexpected, eexpected);
        const auto lresult = lresults[i];
        if (i < (inputs.size() - 1)) {
          ASSERT_FLOAT_EQ(lexpected, lresult)
              << "[" << i << "]: ldexp(" << fexpected << ", " << eexpected << ") failed.";
        }
        else {
          ASSERT_TRUE(std::isnan(lresult))
              << "[" << i << "]: ldexp(" << fexpected << ", " << eexpected << ") failed.";
        }
      }
    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

TEST(MathTest, CopysignTest)
{
  using namespace zinvul;

  auto options = makeTestOptions();
  auto device_list = makeTestDeviceList(options);
  for (uint32b number = 0; number < device_list.size(); ++number) {
    auto& device = device_list[number];
    std::cout << getTestDeviceInfo(*device);

    constexpr uint32b resolution = 2'097'152u;
    constexpr uint32b n = 10;

    auto xinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    xinput_buff->setSize(n * resolution);
    auto yinput_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    yinput_buff->setSize(n * resolution);
    auto result_buff = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceOnly);
    result_buff->setSize(n * resolution);
    auto res_buff = makeUniformBuffer<uint32b>(device.get(), BufferUsage::kDeviceOnly);
    res_buff->setSize(1);
    res_buff->write(&resolution, 1, 0, 0);

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCopysign));
    kernel->run(*xinput_buff, *yinput_buff, *result_buff, *res_buff, {resolution}, 0);
    device->waitForCompletion();

    {
      std::vector<float> xinputs;
      xinputs.resize(xinput_buff->size());
      xinput_buff->read(xinputs.data(), xinputs.size(), 0, 0);

      std::vector<float> yinputs;
      yinputs.resize(yinput_buff->size());
      yinput_buff->read(yinputs.data(), yinputs.size(), 0, 0);

      std::vector<float> results;
      results.resize(result_buff->size());
      result_buff->read(results.data(), results.size(), 0, 0);

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < xinputs.size(); ++i) {
        const auto x = xinputs[i];
        const auto y = yinputs[i];
        const auto expected = std::copysign(x, y);
        const auto result = results[i];
        ::testFloat("copysign", x, &y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      ASSERT_FALSE(num_of_failures) << std::scientific
          << "'copysign' func failed." << std::endl
          << "  Num of failures: " << num_of_failures << std::endl
          << "  Avg ulps: " << (sum_ulps.get() / zisc::cast<float>(num_of_failures))
          << std::endl
          << "  Max ulps: " << max_ulps << std::endl
          << "  Max diff: " << max_diff << std::endl
          << "  Num of inf failures: " << num_of_inf_failures << std::endl
          << "  Num of NaN failures: " << num_of_nan_failures << std::endl;

    }

    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
  }
}

//TEST(MathTest, ConstantValueTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n_int = 16;
//    constexpr uint32b n_float = 44;
//    auto int_values = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    int_values->setSize(n_int);
//    auto max_values = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    max_values->setSize(8);
//    auto umax_values = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    umax_values->setSize(4);
//    auto float_values = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    float_values->setSize(n_float);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testConstantValues, 1);
//    kernel->run(*int_values, *max_values, *umax_values, *float_values, {1}, 0);
//    device->waitForCompletion();
//
//    {
//      std::array<int32b, n_int> results;
//      int_values->read(results.data(), n_int, 0, 0);
//      uint32b index = 0;
//      EXPECT_EQ(FLT_DIG, results[index++])
//          << "The constant 'FLT_DIG' is wrong.";
//      EXPECT_EQ(FLT_MANT_DIG, results[index++])
//          << "The constant 'FLT_MANT_DIG' is wrong.";
//      EXPECT_EQ(FLT_MAX_10_EXP, results[index++])
//          << "The constant 'FLT_MAX_10_EXP' is wrong.";
//      EXPECT_EQ(FLT_MAX_EXP, results[index++])
//          << "The constant 'FLT_MAX_EXP' is wrong.";
//      EXPECT_EQ(FLT_MIN_10_EXP, results[index++])
//          << "The constant 'FLT_MIN_10_EXP' is wrong.";
//      EXPECT_EQ(FLT_MIN_EXP, results[index++])
//          << "The constant 'FLT_MIN_EXP' is wrong.";
//      EXPECT_EQ(FLT_RADIX, results[index++])
//          << "The constant 'FLT_RADIX' is wrong.";
//      EXPECT_EQ(CHAR_BIT, results[index++])
//          << "The constant 'CHAR_BIT' is wrong.";
//      EXPECT_EQ(SCHAR_MAX, results[index++])
//          << "The constant 'SCHAR_MAX' is wrong.";
//      EXPECT_EQ(SCHAR_MIN, results[index++])
//          << "The constant 'SCHAR_MIN' is wrong.";
//      EXPECT_EQ(CHAR_MAX, results[index++])
//          << "The constant 'CHAR_MAX' is wrong.";
//      EXPECT_EQ(CHAR_MIN, results[index++])
//          << "The constant 'CHAR_MIN' is wrong.";
//      EXPECT_EQ(SHRT_MAX, results[index++])
//          << "The constant 'SHRT_MAX' is wrong.";
//      EXPECT_EQ(SHRT_MIN, results[index++])
//          << "The constant 'SHRT_MIN' is wrong.";
//      EXPECT_EQ(INT_MAX, results[index++])
//          << "The constant 'INT_MAX' is wrong.";
//      EXPECT_EQ(INT_MIN, results[index++])
//          << "The constant 'INT_MIN' is wrong.";
//    }
//
//    {
//      std::array<int32b, 8> results;
//      max_values->read(results.data(), results.size(), 0, 0);
//      {
//        constexpr int32b expected = std::numeric_limits<int8b>::max();
//        const int32b result = results[0];
//        ASSERT_EQ(expected, result) << "The constant 'zCharMax' is wrong.";
//      }
//      {
//        constexpr int32b expected = std::numeric_limits<int8b>::min();
//        const int32b result = results[1];
//        ASSERT_EQ(expected, result) << "The constant 'zCharMin' is wrong.";
//      }
//      {
//        constexpr int32b expected = std::numeric_limits<int16b>::max();
//        const int32b result = results[2];
//        ASSERT_EQ(expected, result) << "The constant 'zShortMax' is wrong.";
//      }
//      {
//        constexpr int32b expected = std::numeric_limits<int16b>::min();
//        const int32b result = results[3];
//        ASSERT_EQ(expected, result) << "The constant 'zShortMin' is wrong.";
//      }
//      {
//        constexpr int32b expected = std::numeric_limits<int32b>::max();
//        const int32b result = results[4];
//        ASSERT_EQ(expected, result) << "The constant 'zIntMax' is wrong.";
//      }
//      {
//        constexpr int32b expected = std::numeric_limits<int32b>::min();
//        const int32b result = results[5];
//        ASSERT_EQ(expected, result) << "The constant 'zIntMin' is wrong.";
//      }
////      {
////        constexpr int64b expected = std::numeric_limits<int64b>::max();
////        const int64b result = results[6];
////        ASSERT_EQ(expected, result) << "The constant 'zLongMax' is wrong.";
////      }
////      {
////        constexpr int64b expected = std::numeric_limits<int64b>::min();
////        const int64b result = results[7];
////        ASSERT_EQ(expected, result) << "The constant 'zLongMin' is wrong.";
////      }
//    }
//
//    {
//      std::array<uint32b, 4> results;
//      umax_values->read(results.data(), results.size(), 0, 0);
//      {
//        constexpr uint32b expected = std::numeric_limits<uint8b>::max();
//        const uint32b result = results[0];
//        ASSERT_EQ(expected, result) << "The constant 'zUCharMax' is wrong.";
//      }
//      {
//        constexpr uint32b expected = std::numeric_limits<uint16b>::max();
//        const uint32b result = results[1];
//        ASSERT_EQ(expected, result) << "The constant 'zUShortMax' is wrong.";
//      }
//      {
//        constexpr uint32b expected = std::numeric_limits<uint32b>::max();
//        const uint32b result = results[2];
//        ASSERT_EQ(expected, result) << "The constant 'zUIntMax' is wrong.";
//      }
////      {
////        constexpr uint64b expected = std::numeric_limits<uint64b>::max();
////        const uint64b result = results[3];
////        ASSERT_EQ(expected, result) << "The constant 'zULongMax' is wrong.";
////      }
//    }
//
//    {
//      std::array<float, n_float> results;
//      float_values->read(results.data(), n_float, 0, 0);
//      uint32b index = 0;
//
//      const float e = std::exp(1.0f);
//      const float log2e = std::log2(e);
//      const float log10e = std::log10(e);
//      const float ln2 = std::log(2.0f);
//      const float ln10 = std::log(10.0f);
//      const float pi = 4.0f * std::atan(1.0f);
//
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), results[index++])
//          << "The constant 'MAXFLOAT' is wrong.";
//      EXPECT_TRUE(std::isinf(results[index++])) 
//          << "The constant 'INFINITY' is wrong.";
//      EXPECT_TRUE(std::isnan(results[index++])) 
//          << "The constant 'NAN' is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), results[index++])
//          << "The constant 'FLT_MAX' is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::min(), results[index++])
//          << "The constant 'FLT_MIN' is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::epsilon(), results[index++])
//          << "The constant 'FLT_EPSILON' is wrong.";
//      EXPECT_FLOAT_EQ(e, results[index++])
//          << "The constant 'M_E_F' is wrong.";
//      EXPECT_FLOAT_EQ(log2e, results[index++])
//          << "The constant 'M_LOG2E_F' is wrong.";
//      EXPECT_FLOAT_EQ(log10e, results[index++])
//          << "The constant 'M_LOG10E_F' is wrong.";
//      EXPECT_FLOAT_EQ(ln2, results[index++])
//          << "The constant 'M_LN2_F' is wrong.";
//      EXPECT_FLOAT_EQ(ln10, results[index++])
//          << "The constant 'M_LN10_F' is wrong.";
//      EXPECT_FLOAT_EQ(pi, results[index++])
//          << "The constant 'M_PI_F' is wrong.";
//      EXPECT_FLOAT_EQ(pi / 2.0f, results[index++])
//          << "The constant 'M_PI_2_F' is wrong.";
//      EXPECT_FLOAT_EQ(pi / 4.0f, results[index++])
//          << "The constant 'M_PI_4_F' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / pi, results[index++])
//          << "The constant 'M_1_PI_F' is wrong.";
//      EXPECT_FLOAT_EQ(2.0f / pi, results[index++])
//          << "The constant 'M_2_PI_F' is wrong.";
//      EXPECT_FLOAT_EQ(2.0f / std::sqrt(pi), results[index++])
//          << "The constant 'M_2_SQRTPI_F' is wrong.";
//      EXPECT_FLOAT_EQ(std::sqrt(2.0f), results[index++])
//          << "The constant 'M_SQRT2_F' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / std::sqrt(2.0f), results[index++])
//          << "The constant 'M_SQRT1_2_F' is wrong.";
//
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), results[index++])
//          << "The constant 'zFloatMax' is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::min(), results[index++])
//          << "The constant 'zFloatMin' is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::epsilon(), results[index++])
//          << "The constant 'zFloatEpsilon' is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::min() - std::numeric_limits<float>::denorm_min(), results[index++])
//          << "The constant 'zFloatDenormMax' is wrong.";
//      EXPECT_FLOAT_EQ(std::numeric_limits<float>::denorm_min(), results[index++])
//          << "The constant 'zFloatDenormMin' is wrong.";
//      EXPECT_FLOAT_EQ(e, results[index++])
//          << "The constant 'zEF' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / e, results[index++])
//          << "The constant 'zInvEF' is wrong.";
//      EXPECT_FLOAT_EQ(log2e, results[index++])
//          << "The constant 'zLog2EF' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / log2e, results[index++])
//          << "The constant 'zInvLog2EF' is wrong.";
//      EXPECT_FLOAT_EQ(log10e, results[index++])
//          << "The constant 'zLog10EF' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / log10e, results[index++])
//          << "The constant 'zInvLog10EF' is wrong.";
//      EXPECT_FLOAT_EQ(ln2, results[index++])
//          << "The constant 'zLn2F' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / ln2, results[index++])
//          << "The constant 'zInvLn2F' is wrong.";
//      EXPECT_FLOAT_EQ(ln10, results[index++])
//          << "The constant 'zLn10F' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / ln10, results[index++])
//          << "The constant 'zInvLn10F' is wrong.";
//      EXPECT_FLOAT_EQ(pi, results[index++])
//          << "The constant 'zPiF' is wrong.";
//      EXPECT_FLOAT_EQ(pi / 2.0f, results[index++])
//          << "The constant 'zPi2F' is wrong.";
//      EXPECT_FLOAT_EQ(pi / 4.0f, results[index++])
//          << "The constant 'zPi4F' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / pi, results[index++])
//          << "The constant 'zInvPi' is wrong.";
//      EXPECT_FLOAT_EQ(2.0f / pi, results[index++])
//          << "The constant 'z2InvPi' is wrong.";
//      EXPECT_FLOAT_EQ(2.0f / std::sqrt(pi), results[index++])
//          << "The constant 'z2InvSqrtPiF' is wrong.";
//      EXPECT_FLOAT_EQ(std::sqrt(2.0f), results[index++])
//          << "The constant 'zSqrt2F' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / std::sqrt(2.0f), results[index++])
//          << "The constant 'zInvSqrt2F' is wrong.";
//      EXPECT_FLOAT_EQ(std::sqrt(3.0f), results[index++])
//          << "The constant 'zSqrt3F' is wrong.";
//      EXPECT_FLOAT_EQ(1.0f / std::sqrt(3.0f), results[index++])
//          << "The constant 'zInvSqrt3F' is wrong.";
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, CeilTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(36);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(16);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(16);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testCeil, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'ceil' func is wrong.";
//    {
//      std::array<float, 36> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::ceil(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zCeilImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(36);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(16);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(16);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzCeilImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zCeilImpl' func is wrong.";
//    {
//      std::array<float, 36> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::ceil(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zCeilTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(36);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(16);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(16);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzCeil, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zCeil' func is wrong.";
//    {
//      std::array<float, 36> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::ceil(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::ceil(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, FloorTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testFloor, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'floor' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::floor(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zFloorImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzFloorImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zFloorImpl' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::floor(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zFloorTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzFloor, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zFloor' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::floor(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::floor(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, TruncTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testTrunc, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'trunc' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::trunc(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zTruncImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzTruncImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zTruncImpl' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::trunc(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zTruncTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzTrunc, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zTrunc' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::trunc(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::trunc(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, RoundTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testRound, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'round' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::round(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zRoundImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzRoundImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zRoundImpl' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::round(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zRoundTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzRound, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zRound' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 1; ++i) {
//        const float x = result[2 * i];
//        const float expected = std::round(x);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = std::round(x[j]);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, AbsTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(2);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testAbs, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'abs' func is wrong.";
//    {
//      std::array<uint32b, 2> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0]) << error_message;
//      ASSERT_EQ(5, result[1]) << error_message;
//    }
//    {
//      std::array<cl::uint2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//    }
//    {
//      std::array<cl::uint3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//      ASSERT_EQ(9, result[0].z) << error_message;
//    }
//    {
//      std::array<cl::uint4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//      ASSERT_EQ(9, result[0].z) << error_message;
//      ASSERT_EQ(15, result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zAbsImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(2);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzAbsImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zAbsImpl' func is wrong.";
//    {
//      std::array<uint32b, 2> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0]) << error_message;
//      ASSERT_EQ(5, result[1]) << error_message;
//    }
//    {
//      std::array<cl::uint2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//    }
//    {
//      std::array<cl::uint3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//      ASSERT_EQ(9, result[0].z) << error_message;
//    }
//    {
//      std::array<cl::uint4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//      ASSERT_EQ(9, result[0].z) << error_message;
//      ASSERT_EQ(15, result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zAbsTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(2);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzAbs, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zAbs' func is wrong.";
//    {
//      std::array<uint32b, 2> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0]) << error_message;
//      ASSERT_EQ(5, result[1]) << error_message;
//    }
//    {
//      std::array<cl::uint2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//    }
//    {
//      std::array<cl::uint3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//      ASSERT_EQ(9, result[0].z) << error_message;
//    }
//    {
//      std::array<cl::uint4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5, result[0].x) << error_message;
//      ASSERT_EQ(2, result[0].y) << error_message;
//      ASSERT_EQ(9, result[0].z) << error_message;
//      ASSERT_EQ(15, result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, AbsFTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(2);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testAbsF, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'fabs' func is wrong.";
//    {
//      std::array<float, 2> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0]) << error_message;
//      ASSERT_EQ(5.0f, result[1]) << error_message;
//    }
//    {
//      std::array<cl::float2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//    }
//    {
//      std::array<cl::float3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//      ASSERT_EQ(9.0f, result[0].z) << error_message;
//    }
//    {
//      std::array<cl::float4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//      ASSERT_EQ(9.0f, result[0].z) << error_message;
//      ASSERT_EQ(15.0f, result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zAbsFImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(2);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzAbsFImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zAbsFImpl' func is wrong.";
//    {
//      std::array<float, 2> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0]) << error_message;
//      ASSERT_EQ(5.0f, result[1]) << error_message;
//    }
//    {
//      std::array<cl::float2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//    }
//    {
//      std::array<cl::float3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//      ASSERT_EQ(9.0f, result[0].z) << error_message;
//    }
//    {
//      std::array<cl::float4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//      ASSERT_EQ(9.0f, result[0].z) << error_message;
//      ASSERT_EQ(15.0f, result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zAbsFTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(2);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzAbsF, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zAbsF' func is wrong.";
//    {
//      std::array<float, 2> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0]) << error_message;
//      ASSERT_EQ(5.0f, result[1]) << error_message;
//    }
//    {
//      std::array<cl::float2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//    }
//    {
//      std::array<cl::float3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//      ASSERT_EQ(9.0f, result[0].z) << error_message;
//    }
//    {
//      std::array<cl::float4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_EQ(5.0f, result[0].x) << error_message;
//      ASSERT_EQ(2.0f, result[0].y) << error_message;
//      ASSERT_EQ(9.0f, result[0].z) << error_message;
//      ASSERT_EQ(15.0f, result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zIsAlmostSameTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(6);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzIsAlmostSame, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zIsAlmostSame' func is wrong.";
//    {
//      std::array<int32b, 6> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_TRUE(result[0]) << error_message;
//      ASSERT_TRUE(result[1]) << error_message;
//      ASSERT_FALSE(result[2]) << error_message;
//      ASSERT_FALSE(result[3]) << error_message;
//      ASSERT_TRUE(result[4]) << error_message;
//      ASSERT_FALSE(result[5]) << error_message;
//    }
//    {
//      std::array<cl::int2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_TRUE(result[0].x) << error_message;
//      ASSERT_TRUE(result[0].y) << error_message;
//    }
//    {
//      std::array<cl::int3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_TRUE(result[0].x) << error_message;
//      ASSERT_TRUE(result[0].y) << error_message;
//      ASSERT_TRUE(result[0].z) << error_message;
//    }
//    {
//      std::array<cl::int4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_TRUE(result[0].x) << error_message;
//      ASSERT_TRUE(result[0].y) << error_message;
//      ASSERT_TRUE(result[0].z) << error_message;
//      ASSERT_TRUE(result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zIsOddTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(5);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzIsOdd, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zIsOdd' func is wrong.";
//    {
//      std::array<int32b, 5> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_FALSE(result[0]) << error_message;
//      ASSERT_TRUE(result[1]) << error_message;
//      ASSERT_FALSE(result[2]) << error_message;
//      ASSERT_TRUE(result[3]) << error_message;
//      ASSERT_FALSE(result[4]) << error_message;
//    }
//    {
//      std::array<cl::int2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_TRUE(result[0].x) << error_message;
//      ASSERT_FALSE(result[0].y) << error_message;
//    }
//    {
//      std::array<cl::int3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_FALSE(result[0].x) << error_message;
//      ASSERT_TRUE(result[0].y) << error_message;
//      ASSERT_FALSE(result[0].z) << error_message;
//    }
//    {
//      std::array<cl::int4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_TRUE(result[0].x) << error_message;
//      ASSERT_FALSE(result[0].y) << error_message;
//      ASSERT_TRUE(result[0].z) << error_message;
//      ASSERT_FALSE(result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zIsOddUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(5);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(1);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(1);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(1);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzIsOddU, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zIsOddU' func is wrong.";
//    {
//      std::array<int32b, 5> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_FALSE(result[0]) << error_message;
//      ASSERT_TRUE(result[1]) << error_message;
//      ASSERT_FALSE(result[2]) << error_message;
//    }
//    {
//      std::array<cl::int2, 1> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FALSE(result[0].x) << error_message;
//      ASSERT_TRUE(result[0].y) << error_message;
//    }
//    {
//      std::array<cl::int3, 1> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_FALSE(result[0].x) << error_message;
//      ASSERT_TRUE(result[0].y) << error_message;
//      ASSERT_FALSE(result[0].z) << error_message;
//    }
//    {
//      std::array<cl::int4, 1> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_FALSE(result[0].x) << error_message;
//      ASSERT_TRUE(result[0].y) << error_message;
//      ASSERT_FALSE(result[0].z) << error_message;
//      ASSERT_TRUE(result[0].w) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, MaxTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 30;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMax, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'max' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const int32b lhs = result[3 * i];
//        const int32b rhs = result[3 * i + 1];
//        const int32b expected = std::max(lhs, rhs);
//        const int32b m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMaxImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 30;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMaxImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMaxImpl' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const int32b lhs = result[3 * i];
//        const int32b rhs = result[3 * i + 1];
//        const int32b expected = std::max(lhs, rhs);
//        const int32b m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMaxTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 30;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMax, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMax' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const int32b lhs = result[3 * i];
//        const int32b rhs = result[3 * i + 1];
//        const int32b expected = std::max(lhs, rhs);
//        const int32b m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = std::max(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, MaxUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 18;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMaxU, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'max' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::max(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMaxUImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 18;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMaxUImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMaxUImpl' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::max(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMaxUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 18;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMaxU, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMaxU' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::max(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, MinTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 30;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMin, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'min' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const int32b lhs = result[3 * i];
//        const int32b rhs = result[3 * i + 1];
//        const int32b expected = std::min(lhs, rhs);
//        const int32b m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMinImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 30;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMinImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMinImpl' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const int32b lhs = result[3 * i];
//        const int32b rhs = result[3 * i + 1];
//        const int32b expected = std::min(lhs, rhs);
//        const int32b m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMinTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 30;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMin, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMin' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const int32b lhs = result[3 * i];
//        const int32b rhs = result[3 * i + 1];
//        const int32b expected = std::min(lhs, rhs);
//        const int32b m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = std::min(lhs[j], rhs[j]);
//          const int32b m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, MinUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 18;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMinU, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'min' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::min(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMinUImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 18;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMinUImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMinUImpl' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::min(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMinUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 18;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMinU, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMinU' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::min(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, MaxFTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 48;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMaxF, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'max' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::max(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMaxFImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 48;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMaxFImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMaxFImpl' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::max(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMaxFTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 48;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMaxF, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMaxF' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::max(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::max(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, MinFTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 48;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMinF, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'min' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::min(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMinFImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 48;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMinFImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMinFImpl' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::min(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMinFTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b n = 48;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMinF, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMinF' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        const auto expected = std::min(lhs, rhs);
//        const auto m = result[3 * i + 2];
//        ASSERT_EQ(expected, m) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 3> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 3> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 3> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 3; ++i) {
//        const auto lhs = result[3 * i];
//        const auto rhs = result[3 * i + 1];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::min(lhs[j], rhs[j]);
//          const auto m = result[3 * i + 2][j];
//          ASSERT_EQ(expected, m) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, MixTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto mix_result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result1->setSize(3);
//    auto mix_result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result2->setSize(6);
//    auto mix_result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result3->setSize(6);
//    auto mix_result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result4->setSize(6);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMix, 1);
//    kernel->run(*mix_result1, *mix_result2, *mix_result3, *mix_result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'mix' func is wrong.";
//    {
//      std::array<float, 3> result;
//      mix_result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2]) << error_message;
//    }
//    {
//      std::array<cl::float2, 6> result;
//      mix_result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//    }
//    {
//      std::array<cl::float3, 6> result;
//      mix_result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
//    }
//    {
//      std::array<cl::float4, 6> result;
//      mix_result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
//      ASSERT_FLOAT_EQ(-4.0f, result[0][3]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][3]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
//      ASSERT_FLOAT_EQ(4.0f, result[2][3]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
//      ASSERT_FLOAT_EQ(-4.0f, result[3][3]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][3]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
//      ASSERT_FLOAT_EQ(4.0f, result[5][3]) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMixImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto mix_result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result1->setSize(3);
//    auto mix_result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result2->setSize(6);
//    auto mix_result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result3->setSize(6);
//    auto mix_result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result4->setSize(6);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMixImpl, 1);
//    kernel->run(*mix_result1, *mix_result2, *mix_result3, *mix_result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMixImpl' func is wrong.";
//    {
//      std::array<float, 3> result;
//      mix_result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2]) << error_message;
//    }
//    {
//      std::array<cl::float2, 6> result;
//      mix_result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//    }
//    {
//      std::array<cl::float3, 6> result;
//      mix_result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
//    }
//    {
//      std::array<cl::float4, 6> result;
//      mix_result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
//      ASSERT_FLOAT_EQ(-4.0f, result[0][3]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][3]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
//      ASSERT_FLOAT_EQ(4.0f, result[2][3]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
//      ASSERT_FLOAT_EQ(-4.0f, result[3][3]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][3]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
//      ASSERT_FLOAT_EQ(4.0f, result[5][3]) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zMixTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto mix_result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result1->setSize(3);
//    auto mix_result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result2->setSize(6);
//    auto mix_result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result3->setSize(6);
//    auto mix_result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    mix_result4->setSize(6);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMix, 1);
//    kernel->run(*mix_result1, *mix_result2, *mix_result3, *mix_result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMix' func is wrong.";
//    {
//      std::array<float, 3> result;
//      mix_result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2]) << error_message;
//    }
//    {
//      std::array<cl::float2, 6> result;
//      mix_result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//    }
//    {
//      std::array<cl::float3, 6> result;
//      mix_result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
//    }
//    {
//      std::array<cl::float4, 6> result;
//      mix_result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(-1.0f, result[0][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[0][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[0][2]) << error_message;
//      ASSERT_FLOAT_EQ(-4.0f, result[0][3]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[1][3]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[2][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[2][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[2][2]) << error_message;
//      ASSERT_FLOAT_EQ(4.0f, result[2][3]) << error_message;
//      ASSERT_FLOAT_EQ(-1.0f, result[3][0]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[3][1]) << error_message;
//      ASSERT_FLOAT_EQ(3.0f, result[3][2]) << error_message;
//      ASSERT_FLOAT_EQ(-4.0f, result[3][3]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][0]) << error_message;
//      ASSERT_FLOAT_EQ(2.0f, result[4][1]) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, result[4][2]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[4][3]) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, result[5][0]) << error_message;
//      ASSERT_FLOAT_EQ(-2.0f, result[5][1]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, result[5][2]) << error_message;
//      ASSERT_FLOAT_EQ(4.0f, result[5][3]) << error_message;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, ClampTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 15 * 4;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(8);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(8);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testClamp, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'clamp' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 8> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 8> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 8> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClampImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 15 * 4;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(8);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(8);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClampImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClampImpl' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 8> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 8> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 8> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClampTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 15 * 4;
//    auto results1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(8);
//    auto results3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(8);
//    auto results4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClamp, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClamp' func is wrong.";
//    {
//      std::array<int32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 8> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 8> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 8> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, ClampUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 12 * 4;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(8);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(8);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testClampU, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'clamp' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 8> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 8> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 8> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClampUImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 12 * 4;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(8);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(8);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClampUImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClampUImpl' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 8> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 8> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 8> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClampUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 12 * 4;
//    auto results1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(8);
//    auto results3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(8);
//    auto results4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClampU, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClampU' func is wrong.";
//    {
//      std::array<uint32b, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 8> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 8> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 8> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, ClampFTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 21 * 4;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(16);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(16);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testClampF, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'clamp' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 16> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClampFImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 21 * 4;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(16);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(16);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClampFImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClampFImpl' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 16> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClampFTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    const uint32b n = 21 * 4;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(16);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(16);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClampF, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClampF' func is wrong.";
//    {
//      std::array<float, n> result;
//      results1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        const auto expected = std::clamp(x, minval, maxval);
//        const auto c = result[4 * i + 3];
//        ASSERT_EQ(expected, c) << error_message;
//      }
//    }
//    {
//      std::array<cl::float2, 16> result;
//      results2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 2; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      results3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 3; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      results4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 4; ++i) {
//        const auto x = result[4 * i];
//        const auto minval = result[4 * i + 1];
//        const auto maxval = result[4 * i + 2];
//        for (uint32b j = 0; j < 4; ++j) {
//          const auto expected = std::clamp(x[j], minval[j], maxval[j]);
//          const auto c = result[4 * i + 3][j];
//          ASSERT_EQ(expected, c) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, ClzTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testClz, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'clz' func is wrong.";
//    {
//      std::array<int32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const int32b expected = cl::clz(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClzImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClzImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClzImpl' func is wrong.";
//    {
//      std::array<int32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const int32b expected = cl::clz(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClzTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClz, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClz' func is wrong.";
//    {
//      std::array<int32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const int32b expected = cl::clz(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, ClzUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testClzU, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'clz' func is wrong.";
//    {
//      std::array<uint32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const uint32b expected = cl::clz(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClzUImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClzUImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClzUImpl' func is wrong.";
//    {
//      std::array<uint32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const uint32b expected = cl::clz(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zClzUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzClzU, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zClzU' func is wrong.";
//    {
//      std::array<uint32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const uint32b expected = cl::clz(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const uint32b expected = cl::clz(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, PopcountTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testPopcount, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'popcount' func is wrong.";
//    {
//      std::array<int32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const int32b expected = cl::popcount(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zPopcountImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzPopcountImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zPopcountImpl' func is wrong.";
//    {
//      std::array<int32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const int32b expected = cl::popcount(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zPopcountTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzPopcount, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zPopcount' func is wrong.";
//    {
//      std::array<int32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const int32b expected = cl::popcount(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::int2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::int4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        int32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr int32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const int32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, PopcountUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testPopcountU, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'popcount' func is wrong.";
//    {
//      std::array<uint32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const uint32b expected = cl::popcount(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zPopcountUImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzPopcountUImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zPopcountUImpl' func is wrong.";
//    {
//      std::array<uint32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const uint32b expected = cl::popcount(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zPopcountUTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(32);
//    auto result2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(32);
//    auto result3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(32);
//    auto result4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(32);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzPopcountU, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zPopcountU' func is wrong.";
//    {
//      std::array<uint32b, 32> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        const uint32b expected = cl::popcount(x);
//        ASSERT_EQ(expected, result[i]) << error_message;
//      }
//    }
//    {
//      std::array<cl::uint2, 32> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 2; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint3, 32> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 3; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::uint4, 32> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size(); ++i) {
//        uint32b x = 0;
//        for (uint32b j = 0; j < i; ++j)
//          x = (x << 1) | 0b1;
//        constexpr uint32b k[] = {1, 2, 3, 5};
//        for (uint32b j = 0; j < 4; ++j) {
//          const uint32b expected = cl::popcount(x / k[j]);
//          ASSERT_EQ(expected, result[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//TEST(MathTest, FractTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result1i = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1i->setSize(18);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result2i = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2i->setSize(8);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result3i = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3i->setSize(8);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//    auto result4i = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4i->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testFract, 1);
//    kernel->run(*result1, *result1i, *result2, *result2i,
//                *result3, *result3i, *result4, *result4i, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'fract' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      std::array<float, 18> resulti;
//      result1i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 3; ++i) {
//        const float x = result[2 * i];
//        float expectedi = 0.0f;
//        const float expected = cl::fract(x, &expectedi);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//        ASSERT_FLOAT_EQ(expectedi, resulti[i]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float2, 8> resulti;
//      result2i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float3, 8> resulti;
//      result3i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float4, 8> resulti;
//      result4i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zFractImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result1i = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1i->setSize(18);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result2i = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2i->setSize(8);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result3i = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3i->setSize(8);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//    auto result4i = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4i->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzFractImpl, 1);
//    kernel->run(*result1, *result1i, *result2, *result2i,
//                *result3, *result3i, *result4, *result4i, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zFractImpl' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      std::array<float, 18> resulti;
//      result1i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 3; ++i) {
//        const float x = result[2 * i];
//        float expectedi = 0.0f;
//        const float expected = cl::fract(x, &expectedi);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//        ASSERT_FLOAT_EQ(expectedi, resulti[i]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float2, 8> resulti;
//      result2i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float3, 8> resulti;
//      result3i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float4, 8> resulti;
//      result4i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zFractTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result1i = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1i->setSize(18);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result2i = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2i->setSize(8);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result3i = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3i->setSize(8);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//    auto result4i = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4i->setSize(8);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzFract, 1);
//    kernel->run(*result1, *result1i, *result2, *result2i,
//                *result3, *result3i, *result4, *result4i, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zFract' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      std::array<float, 18> resulti;
//      result1i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 3; ++i) {
//        const float x = result[2 * i];
//        float expectedi = 0.0f;
//        const float expected = cl::fract(x, &expectedi);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//        ASSERT_FLOAT_EQ(expectedi, resulti[i]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float2, 8> resulti;
//      result2i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float3, 8> resulti;
//      result3i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      std::array<cl::float4, 8> resulti;
//      result4i->read(resulti.data(), resulti.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          float expectedi = 0.0f;
//          const float expected = cl::fract(x[j], &expectedi);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//          ASSERT_FLOAT_EQ(expectedi, resulti[i][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, ModTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testMod, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'fmod' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 3; ++i) {
//        const float x = result[2 * i];
//        const float y = 1.0f;
//        const float expected = std::fmod(x, y);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zModImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzModImpl, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zModImpl' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 3; ++i) {
//        const float x = result[2 * i];
//        const float y = 1.0f;
//        const float expected = std::fmod(x, y);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zModTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    result1->setSize(36);
//    auto result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    result2->setSize(16);
//    auto result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    result3->setSize(16);
//    auto result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    result4->setSize(16);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzMod, 1);
//    kernel->run(*result1, *result2, *result3, *result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zMod' func is wrong.";
//    {
//      std::array<float, 36> result;
//      result1->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < (result.size() / 2) - 3; ++i) {
//        const float x = result[2 * i];
//        const float y = 1.0f;
//        const float expected = std::fmod(x, y);
//        ASSERT_FLOAT_EQ(expected, result[2 * i + 1]) << error_message;
//      }
//      ASSERT_TRUE(std::isnan(result[result.size() - 5])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 3])) << error_message;
//      ASSERT_TRUE(std::isnan(result[result.size() - 1])) << error_message;
//    }
//    {
//      std::array<cl::float2, 16> result;
//      result2->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 2; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float3, 16> result;
//      result3->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 3; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//    {
//      std::array<cl::float4, 16> result;
//      result4->read(result.data(), result.size(), 0, 0);
//      for (uint32b i = 0; i < result.size() / 2; ++i) {
//        const auto x = result[2 * i];
//        for (uint32b j = 0; j < 4; ++j) {
//          const float y = zisc::cast<float>(j + 1);
//          const float expected = std::fmod(x[j], y);
//          ASSERT_FLOAT_EQ(expected, result[2 * i + 1][j]) << error_message;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, RadianTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto radian_result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result1->setSize(2);
//    auto radian_result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result2->setSize(2);
//    auto radian_result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result3->setSize(2);
//    auto radian_result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result4->setSize(2);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testRadian, 1);
//    kernel->run(*radian_result1, *radian_result2, *radian_result3, *radian_result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message1 = "The 'degrees' func is wrong.";
//    const char* error_message2 = "The 'radians' func is wrong.";
//    constexpr float pi = zisc::kPi<float>;
//    {
//      std::array<float, 2> result;
//      radian_result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1]) << error_message2;
//    }
//    {
//      std::array<cl::float2, 2> result;
//      radian_result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//    }
//    {
//      std::array<cl::float3, 2> result;
//      radian_result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
//    }
//    {
//      std::array<cl::float4, 2> result;
//      radian_result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) <<error_message1; 
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
//      ASSERT_FLOAT_EQ(0.0f, result[0][3]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
//      ASSERT_FLOAT_EQ(0.0f * pi, result[1][3]) << error_message2;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zRadianImplTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto radian_result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result1->setSize(2);
//    auto radian_result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result2->setSize(2);
//    auto radian_result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result3->setSize(2);
//    auto radian_result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result4->setSize(2);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzRadianImpl, 1);
//    kernel->run(*radian_result1, *radian_result2, *radian_result3, *radian_result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message1 = "The 'zDegreesImpl' func is wrong.";
//    const char* error_message2 = "The 'zRadiansImpl' func is wrong.";
//    constexpr float pi = zisc::kPi<float>;
//    {
//      std::array<float, 2> result;
//      radian_result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1]) << error_message2;
//    }
//    {
//      std::array<cl::float2, 2> result;
//      radian_result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//    }
//    {
//      std::array<cl::float3, 2> result;
//      radian_result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
//    }
//    {
//      std::array<cl::float4, 2> result;
//      radian_result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) <<error_message1; 
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
//      ASSERT_FLOAT_EQ(0.0f, result[0][3]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
//      ASSERT_FLOAT_EQ(0.0f * pi, result[1][3]) << error_message2;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zRadianTest)
//{
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    auto radian_result1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result1->setSize(2);
//    auto radian_result2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result2->setSize(2);
//    auto radian_result3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result3->setSize(2);
//    auto radian_result4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    radian_result4->setSize(2);
//
//    auto kernel = makeZinvulKernel(device.get(), math, testzRadian, 1);
//    kernel->run(*radian_result1, *radian_result2, *radian_result3, *radian_result4, {1}, 0);
//    device->waitForCompletion();
//
//    const char* error_message1 = "The 'zDegrees' func is wrong.";
//    const char* error_message2 = "The 'zRadians' func is wrong.";
//    constexpr float pi = zisc::kPi<float>;
//    {
//      std::array<float, 2> result;
//      radian_result1->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1]) << error_message2;
//    }
//    {
//      std::array<cl::float2, 2> result;
//      radian_result2->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//    }
//    {
//      std::array<cl::float3, 2> result;
//      radian_result3->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) << error_message1;
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
//    }
//    {
//      std::array<cl::float4, 2> result;
//      radian_result4->read(result.data(), result.size(), 0, 0);
//      ASSERT_FLOAT_EQ(180.0f, result[0][0]) <<error_message1; 
//      ASSERT_FLOAT_EQ(90.0f, result[0][1]) << error_message1;
//      ASSERT_FLOAT_EQ(45.0f, result[0][2]) << error_message1;
//      ASSERT_FLOAT_EQ(0.0f, result[0][3]) << error_message1;
//      ASSERT_FLOAT_EQ(pi, result[1][0]) << error_message2;
//      ASSERT_FLOAT_EQ(0.5f * pi, result[1][1]) << error_message2;
//      ASSERT_FLOAT_EQ(0.25f * pi, result[1][2]) << error_message2;
//      ASSERT_FLOAT_EQ(0.0f * pi, result[1][3]) << error_message2;
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, FrLdexpTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(3 * (resolution + 2));
//    auto results_exp1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp1->setSize(resolution + 2);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3 * resolution);
//    auto results_exp2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp2->setSize(resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3 * resolution);
//    auto results_exp3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp3->setSize(resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3 * resolution);
//    auto results_exp4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp4->setSize(resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testFrLdexp, 1);
//    kernel->run(*results1, *results_exp1,
//                *results2, *results_exp2,
//                *results3, *results_exp3,
//                *results4, *results_exp4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message1 = "The 'frexp' func is wrong.";
//    const char* error_message2 = "The 'ldexp' func is wrong.";
//    // Scalar
//    {
//      std::vector<float> results;
//      results.resize(3 * (resolution + 2));
//      std::vector<int32b> results_exp;
//      results_exp.resize(resolution + 2);
//      results1->read(results.data(), results.size(), 0, 0);
//      results_exp1->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const float n = results[3 * i];
//        int e = 0;
//        float expected = std::frexp(n, &e);
//        float result = results[3 * i + 1];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message1;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message1;
//        if (i < resolution)
//          ASSERT_EQ(e, results_exp[i]) << error_message1;
//        const float m = expected;
//        expected = std::ldexp(m, e);
//        result = results[3 * i + 2];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message2;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message2;
//      }
//    }
//
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int2> results_exp;
//      results_exp.resize(resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      results_exp2->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float2 n = results[3 * i];
//        int e = 0;
//        cl::float2 result1 = results[3 * i + 1];
//        cl::float2 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int3> results_exp;
//      results_exp.resize(resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      results_exp3->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float3 n = results[3 * i];
//        int e = 0;
//        cl::float3 result1 = results[3 * i + 1];
//        cl::float3 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//  }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int4> results_exp;
//      results_exp.resize(resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      results_exp4->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float4 n = results[3 * i];
//        int e = 0;
//        cl::float4 result1 = results[3 * i + 1];
//        cl::float4 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zFrLdexpImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(3 * (resolution + 2));
//    auto results_exp1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp1->setSize(resolution + 2);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3 * resolution);
//    auto results_exp2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp2->setSize(resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3 * resolution);
//    auto results_exp3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp3->setSize(resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3 * resolution);
//    auto results_exp4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp4->setSize(resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzFrLdexpImpl, 1);
//    kernel->run(*results1, *results_exp1,
//                *results2, *results_exp2,
//                *results3, *results_exp3,
//                *results4, *results_exp4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message1 = "The 'zFrexpImpl' func is wrong.";
//    const char* error_message2 = "The 'zLdexpImpl' func is wrong.";
//    // Scalar
//    {
//      std::vector<float> results;
//      results.resize(3 * (resolution + 2));
//      std::vector<int32b> results_exp;
//      results_exp.resize(resolution + 2);
//      results1->read(results.data(), results.size(), 0, 0);
//      results_exp1->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const float n = results[3 * i];
//        int e = 0;
//        float expected = std::frexp(n, &e);
//        float result = results[3 * i + 1];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message1;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message1;
//        if (i < resolution)
//          ASSERT_EQ(e, results_exp[i]) << error_message1;
//        const float m = expected;
//        expected = std::ldexp(m, e);
//        result = results[3 * i + 2];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message2;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message2;
//      }
//    }
//
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int2> results_exp;
//      results_exp.resize(resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      results_exp2->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float2 n = results[3 * i];
//        int e = 0;
//        cl::float2 result1 = results[3 * i + 1];
//        cl::float2 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int3> results_exp;
//      results_exp.resize(resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      results_exp3->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float3 n = results[3 * i];
//        int e = 0;
//        cl::float3 result1 = results[3 * i + 1];
//        cl::float3 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//  }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int4> results_exp;
//      results_exp.resize(resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      results_exp4->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float4 n = results[3 * i];
//        int e = 0;
//        cl::float4 result1 = results[3 * i + 1];
//        cl::float4 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zFrLdexpTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(3 * (resolution + 2));
//    auto results_exp1 = makeStorageBuffer<int32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp1->setSize(resolution + 2);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3 * resolution);
//    auto results_exp2 = makeStorageBuffer<cl::int2>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp2->setSize(resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3 * resolution);
//    auto results_exp3 = makeStorageBuffer<cl::int3>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp3->setSize(resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3 * resolution);
//    auto results_exp4 = makeStorageBuffer<cl::int4>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp4->setSize(resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzFrLdexp, 1);
//    kernel->run(*results1, *results_exp1,
//                *results2, *results_exp2,
//                *results3, *results_exp3,
//                *results4, *results_exp4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message1 = "The 'zFrexp' func is wrong.";
//    const char* error_message2 = "The 'zLdexp' func is wrong.";
//    // Scalar
//    {
//      std::vector<float> results;
//      results.resize(3 * (resolution + 2));
//      std::vector<int32b> results_exp;
//      results_exp.resize(resolution + 2);
//      results1->read(results.data(), results.size(), 0, 0);
//      results_exp1->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const float n = results[3 * i];
//        int e = 0;
//        float expected = std::frexp(n, &e);
//        float result = results[3 * i + 1];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message1;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message1;
//        if (i < resolution)
//          ASSERT_EQ(e, results_exp[i]) << error_message1 << std::endl
//              << std::scientific << n;
//        const float m = expected;
//        expected = std::ldexp(m, e);
//        result = results[3 * i + 2];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message2;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message2;
//      }
//    }
//
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int2> results_exp;
//      results_exp.resize(resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      results_exp2->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float2 n = results[3 * i];
//        int e = 0;
//        cl::float2 result1 = results[3 * i + 1];
//        cl::float2 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int3> results_exp;
//      results_exp.resize(resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      results_exp3->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float3 n = results[3 * i];
//        int e = 0;
//        cl::float3 result1 = results[3 * i + 1];
//        cl::float3 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//  }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(3 * resolution);
//      std::vector<cl::int4> results_exp;
//      results_exp.resize(resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      results_exp4->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float4 n = results[3 * i];
//        int e = 0;
//        cl::float4 result1 = results[3 * i + 1];
//        cl::float4 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          ASSERT_EQ(e, results_exp[i][j]) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zFrLdexpBiasedImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(3 * (resolution + 2));
//    auto results_exp1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp1->setSize(resolution + 2);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3 * resolution);
//    auto results_exp2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp2->setSize(resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3 * resolution);
//    auto results_exp3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp3->setSize(resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3 * resolution);
//    auto results_exp4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp4->setSize(resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzFrLdexpBiasedImpl, 1);
//    kernel->run(*results1, *results_exp1,
//                *results2, *results_exp2,
//                *results3, *results_exp3,
//                *results4, *results_exp4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message1 = "The 'zFrexpBiasedImpl' func is wrong.";
//    const char* error_message2 = "The 'zLdexpBiasedImpl' func is wrong.";
//    // Scalar
//    {
//      std::vector<float> results;
//      results.resize(3 * (resolution + 2));
//      std::vector<uint32b> results_exp;
//      results_exp.resize(resolution + 2);
//      results1->read(results.data(), results.size(), 0, 0);
//      results_exp1->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const float n = results[3 * i];
//        int e = 0;
//        float expected = std::frexp(n, &e);
//        float result = results[3 * i + 1];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message1;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message1;
//        if (i < resolution) {
//          const int expt = (n == 0.0f) ? cast<int>(results_exp[i]) : cast<int>(results_exp[i]) - 127;
//          ASSERT_EQ(e, expt) << error_message1;
//        }
//        const float m = expected;
//        expected = std::ldexp(m, e);
//        result = results[3 * i + 2];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message2;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message2;
//      }
//    }
//
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(3 * resolution);
//      std::vector<cl::uint2> results_exp;
//      results_exp.resize(resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      results_exp2->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float2 n = results[3 * i];
//        int e = 0;
//        cl::float2 result1 = results[3 * i + 1];
//        cl::float2 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          const int expt = (n[j] == 0.0f) ? cast<int>(results_exp[i][j]) : cast<int>(results_exp[i][j]) - 127;
//          ASSERT_EQ(e, expt) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(3 * resolution);
//      std::vector<cl::uint3> results_exp;
//      results_exp.resize(resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      results_exp3->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float3 n = results[3 * i];
//        int e = 0;
//        cl::float3 result1 = results[3 * i + 1];
//        cl::float3 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          const int expt = (n[j] == 0.0f) ? cast<int>(results_exp[i][j]) : cast<int>(results_exp[i][j]) - 127;
//          ASSERT_EQ(e, expt) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//  }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(3 * resolution);
//      std::vector<cl::uint4> results_exp;
//      results_exp.resize(resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      results_exp4->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float4 n = results[3 * i];
//        int e = 0;
//        cl::float4 result1 = results[3 * i + 1];
//        cl::float4 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          const int expt = (n[j] == 0.0f) ? cast<int>(results_exp[i][j]) : cast<int>(results_exp[i][j]) - 127;
//          ASSERT_EQ(e, expt) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zFrLdexpBiasedTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(3 * (resolution + 2));
//    auto results_exp1 = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp1->setSize(resolution + 2);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3 * resolution);
//    auto results_exp2 = makeStorageBuffer<cl::uint2>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp2->setSize(resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3 * resolution);
//    auto results_exp3 = makeStorageBuffer<cl::uint3>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp3->setSize(resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3 * resolution);
//    auto results_exp4 = makeStorageBuffer<cl::uint4>(device.get(), BufferUsage::kDeviceTSrc);
//    results_exp4->setSize(resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzFrLdexpBiased, 1);
//    kernel->run(*results1, *results_exp1,
//                *results2, *results_exp2,
//                *results3, *results_exp3,
//                *results4, *results_exp4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message1 = "The 'zFrexpBiased' func is wrong.";
//    const char* error_message2 = "The 'zLdexpBiased' func is wrong.";
//    // Scalar
//    {
//      std::vector<float> results;
//      results.resize(3 * (resolution + 2));
//      std::vector<uint32b> results_exp;
//      results_exp.resize(resolution + 2);
//      results1->read(results.data(), results.size(), 0, 0);
//      results_exp1->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const float n = results[3 * i];
//        int e = 0;
//        float expected = std::frexp(n, &e);
//        float result = results[3 * i + 1];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message1;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message1;
//        if (i < resolution) {
//          const int expt = (n == 0.0f) ? cast<int>(results_exp[i]) : cast<int>(results_exp[i]) - 127;
//          ASSERT_EQ(e, expt) << error_message1;
//        }
//        const float m = expected;
//        expected = std::ldexp(m, e);
//        result = results[3 * i + 2];
//        if (i == results_exp.size() - 1)
//          ASSERT_TRUE(std::isnan(result)) << error_message2;
//        else
//          ASSERT_FLOAT_EQ(expected, result) << error_message2;
//      }
//    }
//
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(3 * resolution);
//      std::vector<cl::uint2> results_exp;
//      results_exp.resize(resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      results_exp2->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float2 n = results[3 * i];
//        int e = 0;
//        cl::float2 result1 = results[3 * i + 1];
//        cl::float2 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          const int expt = (n[j] == 0.0f) ? cast<int>(results_exp[i][j]) : cast<int>(results_exp[i][j]) - 127;
//          ASSERT_EQ(e, expt) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(3 * resolution);
//      std::vector<cl::uint3> results_exp;
//      results_exp.resize(resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      results_exp3->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float3 n = results[3 * i];
//        int e = 0;
//        cl::float3 result1 = results[3 * i + 1];
//        cl::float3 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          const int expt = (n[j] == 0.0f) ? cast<int>(results_exp[i][j]) : cast<int>(results_exp[i][j]) - 127;
//          ASSERT_EQ(e, expt) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//  }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(3 * resolution);
//      std::vector<cl::uint4> results_exp;
//      results_exp.resize(resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      results_exp4->read(results_exp.data(), results_exp.size(), 0, 0);
//      for (uint32b i = 0; i < results_exp.size(); ++i) {
//        const cl::float4 n = results[3 * i];
//        int e = 0;
//        cl::float4 result1 = results[3 * i + 1];
//        cl::float4 result2 = results[3 * i + 2];
//        for (uint32b j = 0; j < result1.size(); ++j) {
//          float expected = std::frexp(n[j], &e);
//          ASSERT_FLOAT_EQ(expected, result1[j]) << error_message1;
//          const int expt = (n[j] == 0.0f) ? cast<int>(results_exp[i][j]) : cast<int>(results_exp[i][j]) - 127;
//          ASSERT_EQ(e, expt) << error_message1;
//          const float m = expected;
//          expected = std::ldexp(m, e);
//          ASSERT_FLOAT_EQ(expected, result2[j]) << error_message2;
//        }
//      }
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//
//namespace  {
//
//template <uint32b kAllowableUlps = 2, uint32b kCautionedUlps = 5> 
//void testFloat(const char* func_name,
//    const float x,
//    const float expected,
//    const float result,
//    uint32b& num_of_trials,
//    uint32b& num_of_failures,
//    zisc::CompensatedSummation<float>& sum_ulps,
//    float& max_ulps,
//    const bool print_inf_value = true,
//    const bool print_nan_value = true)
//{
//  char name[256] = "";
//  std::sprintf(name, func_name, zisc::cast<double>(x), zisc::cast<double>(result));
//  if (std::isfinite(result)) {
//    ++num_of_trials;
//    if (!zisc::constant::isAlmostEqual<kAllowableUlps>(expected, result)) {
//      ++num_of_failures;
//      const float ulps = calcUlps(expected, result);
//      sum_ulps.add(ulps);
//      max_ulps = std::max(max_ulps, ulps);
//      if (!isfinite(ulps) || (zisc::cast<float>(kCautionedUlps) <= ulps)) {
//        std::cout << "  - " << name << " ulps: " << std::scientific << ulps << std::endl;
//      }
//    }
//  }
//  else if (std::isinf(result)) {
//    if (print_inf_value)
//      std::cout << "  - " << name;
//    if (!std::isinf(expected)) {
//      ++num_of_trials;
//      ++num_of_failures;
//      sum_ulps.add(4.0f);
//      if (print_inf_value)
//        std::cout << " is unexpected.";
//    }
//    if (print_inf_value)
//      std::cout << std::endl;
//  }
//  else if (print_nan_value && std::isnan(result)) {
//    if (print_inf_value)
//    std::cout << "  - " << name;
//    if (!std::isnan(expected)) {
//      ++num_of_trials;
//      ++num_of_failures;
//      sum_ulps.add(4.0f);
//      if (print_inf_value)
//        std::cout << " is unexpected.";
//    }
//    if (print_inf_value)
//      std::cout << std::endl;
//  }
//}
//
//} // namespace 
//
//TEST(MathTest, MakeNormalTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(4 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testMakeNormal, 1);
//    kernel->run(*results1, *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'makeNormal' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(4 * resolution);
//      results1->read(results.data(), results.size(), 0, 0);
//      zinvul::math::KernelGroup k;
//      for (uint32b i = 0; i < resolution; ++i) {
////        {
////          const float x = cast<float>(2 * i) / cast<float>(resolution) - 1.0f;
////          const float expected = x;
////          const float result = results[4 * i];
////          char func_name[256] = "x1(%g)=%g";
////          ::testFloat(func_name, x, expected, result,
////                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
////        }
//        {
//          const float x = results[4 * i];
//          const float expected = k.makeNormal(x);
//          const float result = results[4 * i + 1];
//          char func_name[256] = "makeNormal(%g)=%g";
//          ::testFloat(func_name, x, expected, result,
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//        {
//          const float x = cast<float>(i + 1) / cast<float>(resolution);
//          const float expected = x;
//          const float result = results[4 * i + 2];
//          char func_name[256] = "x2(%g)=%g";
//          ::testFloat(func_name, x, expected, result,
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//        {
//          const float x = results[4 * i + 2];
//          const float expected = k.makeNormal(x);
//          const float result = results[4 * i + 3];
//          char func_name[256] = "makeNormal(%g)=%g";
//          ::testFloat(func_name, x, expected, result,
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, ExpTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testExp, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'exp' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::exp(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "exp(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "exp(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "exp(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "exp(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zExpImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzExpImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zExpImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::exp(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zExpImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "zExpImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "zExpImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "zExpImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zExpTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzExp, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zExp' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::exp(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zExp(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "zExp(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "zExp(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::exp(z[j]);
//          char func_name[256] = "zExp(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, LogTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testLog, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'log' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::log(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "log(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "log(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "log(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "log(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zLogImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLogImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zLogImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::log(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zLogImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "zLogImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "zLogImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "zLogImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zLogTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLog, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zLog' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::log(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zLog(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "zLog(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "zLog(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log(z[j]);
//          char func_name[256] = "zLog(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, Log2Test)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testLog2, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'log2' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::log2(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "log2(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "log2(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "log2(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "log2(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zLog2ImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLog2Impl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zLo2Impl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::log2(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zLog2Impl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "zLog2Impl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "zLog2Impl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "zLog2Impl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zLog2Test)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLog2, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zLo2' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::log2(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zLog2(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "zLog2(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "zLog2(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::log2(z[j]);
//          char func_name[256] = "zLog2(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, PowTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(9 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testPow, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(9 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(0.0f, z);
//        const float result = results[9 * i + 1];
//        char func_name[256] = "pow(0, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(0,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(1.0f, z);
//        const float result = results[9 * i + 2];
//        char func_name[256] = "pow(1, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(1,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      //! \todo pow(-1, exp) should be checked.
////      for (uint32b i = 0; i < resolution + 3; ++i) {
////        const float z = results[9 * i];
////        const float expected = std::pow(-1.0f, z);
////        const float result = results[9 * i + 3];
////        char func_name[256] = "pow(-1, %g)=%g";
////        ::testFloat(func_name, z, expected, result,
////                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
////      }
////      EXPECT_FALSE(num_of_failures)
////          << "The 'pow(-1,x)' func is wrong." << std::endl
////          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
////          << "maxulps=" << std::scientific << max_ulps << ", "
////          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(2.0f, z);
//        const float result = results[9 * i + 4];
//        char func_name[256] = "pow(2, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(0.5f, z);
//        const float result = results[9 * i + 5];
//        char func_name[256] = "pow(0.5, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(0.5,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 6];
//        char func_name[256] = "pow(inf, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(inf,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
////      for (uint32b i = 0; i < resolution + 3; ++i) {
////        const float z = results[9 * i];
////        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
////        const float result = results[9 * i + 7];
////        char func_name[256] = "pow(-inf, %g)=%g";
////        ::testFloat(func_name, z, expected, result,
////                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
////      }
////      EXPECT_FALSE(num_of_failures)
////          << "The 'pow(-inf,x)' func is wrong." << std::endl
////          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
////          << "maxulps=" << std::scientific << max_ulps << ", "
////          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
//        const float result = results[9 * i + 8];
//        char func_name[256] = "pow(NaN, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, true, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(NaN,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "pow(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "pow(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "pow(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'pow(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zPowImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(9 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzPowImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(9 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(0.0f, z);
//        const float result = results[9 * i + 1];
//        char func_name[256] = "zPowImpl(0, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(0,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(1.0f, z);
//        const float result = results[9 * i + 2];
//        char func_name[256] = "zPowImpl(1, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(1,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      //! \todo zPowImpl(-1, exp) should be checked.
////      for (uint32b i = 0; i < resolution + 3; ++i) {
////        const float z = results[9 * i];
////        const float expected = std::pow(-1.0f, z);
////        const float result = results[9 * i + 3];
////        char func_name[256] = "zPowImpl(-1, %g)=%g";
////        ::testFloat(func_name, z, expected, result,
////                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
////      }
////      EXPECT_FALSE(num_of_failures)
////          << "The 'zPowImpl(-1,x)' func is wrong." << std::endl
////          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
////          << "maxulps=" << std::scientific << max_ulps << ", "
////          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(2.0f, z);
//        const float result = results[9 * i + 4];
//        char func_name[256] = "zPowImpl(2, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(0.5f, z);
//        const float result = results[9 * i + 5];
//        char func_name[256] = "zPowImpl(0.5, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(0.5,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 6];
//        char func_name[256] = "zPowImpl(inf, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(inf,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
////      for (uint32b i = 0; i < resolution + 3; ++i) {
////        const float z = results[9 * i];
////        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
////        const float result = results[9 * i + 7];
////        char func_name[256] = "zPowImpl(-inf, %g)=%g";
////        ::testFloat(func_name, z, expected, result,
////                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
////      }
////      EXPECT_FALSE(num_of_failures)
////          << "The 'zPowImpl(-inf,x)' func is wrong." << std::endl
////          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
////          << "maxulps=" << std::scientific << max_ulps << ", "
////          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
//        const float result = results[9 * i + 8];
//        char func_name[256] = "zPowImpl(NaN, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, true, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(NaN,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPowImpl(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPowImpl(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPowImpl(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPowImpl(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zPowTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(9 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzPow, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(9 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(0.0f, z);
//        const float result = results[9 * i + 1];
//        char func_name[256] = "zPow(0, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(0,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(1.0f, z);
//        const float result = results[9 * i + 2];
//        char func_name[256] = "zPow(1, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(1,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      //! \todo zPow(-1, exp) should be checked.
////      for (uint32b i = 0; i < resolution + 3; ++i) {
////        const float z = results[9 * i];
////        const float expected = std::pow(-1.0f, z);
////        const float result = results[9 * i + 3];
////        char func_name[256] = "zPow(-1, %g)=%g";
////        ::testFloat(func_name, z, expected, result,
////                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
////      }
////      EXPECT_FALSE(num_of_failures)
////          << "The 'zPow(-1,x)' func is wrong." << std::endl
////          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
////          << "maxulps=" << std::scientific << max_ulps << ", "
////          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(2.0f, z);
//        const float result = results[9 * i + 4];
//        char func_name[256] = "zPow(2, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(0.5f, z);
//        const float result = results[9 * i + 5];
//        char func_name[256] = "zPow(0.5, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(0.5,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 6];
//        char func_name[256] = "zPow(inf, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(inf,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
////      for (uint32b i = 0; i < resolution + 3; ++i) {
////        const float z = results[9 * i];
////        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
////        const float result = results[9 * i + 7];
////        char func_name[256] = "zPow(-inf, %g)=%g";
////        ::testFloat(func_name, z, expected, result,
////                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
////      }
////      EXPECT_FALSE(num_of_failures)
////          << "The 'zPow(-inf,x)' func is wrong." << std::endl
////          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
////          << "maxulps=" << std::scientific << max_ulps << ", "
////          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
//        const float result = results[9 * i + 8];
//        char func_name[256] = "zPow(NaN, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, true, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(NaN,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPow(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPow(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPow(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPow(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zPownTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 32u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(9 * resolution);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzPown, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(9 * resolution);
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(0.0f, z);
//        const float result = results[9 * i + 1];
//        char func_name[256] = "zPown(0, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(0,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(1.0f, z);
//        const float result = results[9 * i + 2];
//        char func_name[256] = "zPown(1, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(1,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-1.0f, z);
//        const float result = results[9 * i + 3];
//        char func_name[256] = "zPown(-1, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(-1,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(2.0f, z);
//        const float result = results[9 * i + 4];
//        char func_name[256] = "zPown(2, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(0.5f, z);
//        const float result = results[9 * i + 5];
//        char func_name[256] = "zPown(0.5, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(0.5,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 6];
//        char func_name[256] = "zPown(inf, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false, true);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(inf,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      //! \todo zPow(-inf, exp) should be checked.
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(-std::numeric_limits<float>::infinity(), z);
//        const float result = results[9 * i + 7];
//        char func_name[256] = "zPown(-inf, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, false, true);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(-inf,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//
//      num_of_trials = 0;
//      num_of_failures = 0;
//      sum_ulps.set(0.0f);
//      max_ulps = 0.0f;
//
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[9 * i];
//        const float expected = std::pow(std::numeric_limits<float>::quiet_NaN(), z);
//        const float result = results[9 * i + 8];
//        char func_name[256] = "zPown(NaN, %g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps, true, false);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(NaN,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPown(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPown(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::pow(2.0f, z[j]);
//          char func_name[256] = "zPown(2, %g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << "The 'zPown(2,x)' func is wrong." << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, RsqrtTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * resolution);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testRsqrt, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'rsqrt' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * resolution);
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[2 * i];
//        const float expected = 1.0f / std::sqrt(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "rsqrt(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "rsqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "rsqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "rsqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zRsqrtImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * resolution);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzRsqrtImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zRsqrtImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * resolution);
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[2 * i];
//        const float expected = 1.0f / std::sqrt(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zRsqrtImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "zRsqrtImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "zRsqrtImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "zRsqrtImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zRsqrtTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * resolution);
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzRsqrt, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zRsqrt' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * resolution);
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const float z = results[2 * i];
//        const float expected = 1.0f / std::sqrt(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zRsqrt(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "zRsqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "zRsqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = 1.0f / std::sqrt(z[j]);
//          char func_name[256] = "zRsqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, SqrtTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testSqrt, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'sqrt' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::sqrt(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "sqrt(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "sqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "sqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "sqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zSqrtImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSqrtImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zSqrtImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::sqrt(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zSqrtImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "zSqrtImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "zSqrtImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "zSqrtImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zSqrtTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSqrt, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zSqrt' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::sqrt(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zSqrt(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "zSqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "zSqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sqrt(z[j]);
//          char func_name[256] = "zSqrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zCbrtTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzCbrt, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zCbrt' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::cbrt(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zCbrt(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cbrt(z[j]);
//          char func_name[256] = "zCbrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cbrt(z[j]);
//          char func_name[256] = "zCbrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cbrt(z[j]);
//          char func_name[256] = "zCbrt(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, SinTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testSin, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'sin' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::sin(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "sin(%g)=%g";
//        ::testFloat<2, 100000>(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "sin(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "sin(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "sin(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zSinImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSinImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zSinImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::sin(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zSinImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "zSinImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "zSinImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "zSinImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zSinTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSinImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zSin' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::sin(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zSin(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "zSin(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "zSin(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::sin(z[j]);
//          char func_name[256] = "zSin(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, CosTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testCos, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'cos' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::cos(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "cos(%g)=%g";
//        ::testFloat<2, 100000>(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "cos(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "cos(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "cos(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zCosImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzCosImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zCosImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::cos(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zCosImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "zCosImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "zCosImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "zCosImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zCosTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzCos, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zCos' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::cos(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zCos(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "zCos(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "zCos(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::cos(z[j]);
//          char func_name[256] = "zCos(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zSincosImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(3 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSincosImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    const char* error_message = "The 'zSincosImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(3 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[3 * i];
//        {
//          const float expected = std::sin(z);
//          const float result = results[3 * i + 1];
//          char func_name[256] = "zSincosImpl(%g)=%g";
//          ::testFloat(func_name, z, expected, result,
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//        {
//          const float expected = std::cos(z);
//          const float result = results[3 * i + 2];
//          char func_name[256] = "zSincosImpl(%g)=%g";
//          ::testFloat(func_name, z, expected, result,
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(3 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[3 * i];
//        {
//          const auto& result = results[3 * i + 1];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::sin(z[j]);
//            char func_name[256] = "zSincosImpl(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//        {
//          const auto& result = results[3 * i + 2];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::cos(z[j]);
//            char func_name[256] = "zSincosImpl(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(3 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[3 * i];
//        {
//          const auto& result = results[3 * i + 1];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::sin(z[j]);
//            char func_name[256] = "zSincosImpl(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//        {
//          const auto& result = results[3 * i + 2];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::cos(z[j]);
//            char func_name[256] = "zSincosImpl(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(3 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[3 * i];
//        {
//          const auto& result = results[3 * i + 1];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::sin(z[j]);
//            char func_name[256] = "zSincosImpl(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//        {
//          const auto& result = results[3 * i + 2];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::cos(z[j]);
//            char func_name[256] = "zSincosImpl(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//  }
//}
//
//TEST(MathTest, zSincosTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(3 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(3 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(3 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(3 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzSincos, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zSincos' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(3 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[3 * i];
//        {
//          const float expected = std::sin(z);
//          const float result = results[3 * i + 1];
//          char func_name[256] = "zSincos(%g)=%g";
//          ::testFloat(func_name, z, expected, result,
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//        {
//          const float expected = std::cos(z);
//          const float result = results[3 * i + 2];
//          char func_name[256] = "zSincos(%g)=%g";
//          ::testFloat(func_name, z, expected, result,
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(3 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[3 * i];
//        {
//          const auto& result = results[3 * i + 1];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::sin(z[j]);
//            char func_name[256] = "zSincos(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//        {
//          const auto& result = results[3 * i + 2];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::cos(z[j]);
//            char func_name[256] = "zSincos(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(3 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[3 * i];
//        {
//          const auto& result = results[3 * i + 1];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::sin(z[j]);
//            char func_name[256] = "zSincos(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//        {
//          const auto& result = results[3 * i + 2];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::cos(z[j]);
//            char func_name[256] = "zSincos(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(3 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[3 * i];
//        {
//          const auto& result = results[3 * i + 1];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::sin(z[j]);
//            char func_name[256] = "zSincos(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//        {
//          const auto& result = results[3 * i + 2];
//          for (uint32b j = 0; j < result.size(); ++j) {
//            float expected = std::cos(z[j]);
//            char func_name[256] = "zSincos(%g)=%g";
//            ::testFloat(func_name, z[j], expected, result[j],
//                        num_of_trials, num_of_failures, sum_ulps, max_ulps);
//          }
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, TanTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testTan, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'tan' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::tan(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "tan(%g)=%g";
//        ::testFloat<2, 100000>(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "tan(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "tan(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "tan(%g)=%g";
//          ::testFloat<2, 100000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zTanImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzTanImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zTanImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::tan(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zTanImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "zTanImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "zTanImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "zTanImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zTanTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzTan, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zTan' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::tan(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zTan(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "zTan(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "zTan(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::tan(z[j]);
//          char func_name[256] = "zTan(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, AsinTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAsin, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'asin' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::asin(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "asin(%g)=%g";
//        ::testFloat<2, 10000>(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "asin(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "asin(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "asin(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zAsinImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAsinImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zAsinImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::asin(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zAsinImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "zAsinImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "zAsinImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "zAsinImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zAsinTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAsin, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zAsin' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::asin(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zAsin(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "zAsin(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "zAsin(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::asin(z[j]);
//          char func_name[256] = "zAsin(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, AcosTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAcos, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'acos' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::acos(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "acos(%g)=%g";
//        ::testFloat<2, 10000>(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "acos(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "acos(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "acos(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zAcosImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAcosImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zAcosImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::acos(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zAcosImpl(%g)=%g";
//        ::testFloat<2, 10000>(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "zAcosImpl(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "zAcosImpl(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "zAcosImpl(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zAcosTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 2));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAcos, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zAcos' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 2));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 2; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::acos(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zAcos(%g)=%g";
//        ::testFloat<2, 10000>(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "zAcos(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "zAcos(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::acos(z[j]);
//          char func_name[256] = "zAcos(%g)=%g";
//          ::testFloat<2, 10000>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, AtanTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testAtan, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'atan' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::atan(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "atan(%g)=%g";
//        ::testFloat<2, 100>(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "atan(%g)=%g";
//          ::testFloat<2, 100>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "atan(%g)=%g";
//          ::testFloat<2, 100>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "atan(%g)=%g";
//          ::testFloat<2, 100>(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zAtanImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAtanImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zAtanImpl' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::atan(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zAtanImpl(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "zAtanImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "zAtanImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "zAtanImpl(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zAtanTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1000000u;
//
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(2 * (resolution + 3));
//    auto results2 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(2 * resolution);
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(2 * resolution);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(2 * resolution);
//    auto resolution_buffer = makeStorageBuffer<uint32b>(device.get(), BufferUsage::kDeviceTDst);
//    resolution_buffer->setSize(1);
//    resolution_buffer->write(&resolution, resolution_buffer->size(), 0, 0);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzAtan, 1);
//    kernel->run(*results1, *results2, *results3, *results4,
//                *resolution_buffer, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zAtan' func is wrong.";
//    // Scalar
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<float> results;
//      results.resize(2 * (resolution + 3));
//      results1->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution + 3; ++i) {
//        const float z = results[2 * i];
//        const float expected = std::atan(z);
//        const float result = results[2 * i + 1];
//        char func_name[256] = "zAtan(%g)=%g";
//        ::testFloat(func_name, z, expected, result,
//                    num_of_trials, num_of_failures, sum_ulps, max_ulps);
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector2
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float2> results;
//      results.resize(2 * resolution);
//      results2->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "zAtan(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector3
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float3> results;
//      results.resize(2 * resolution);
//      results3->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "zAtan(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    // Vector4
//    {
//      uint32b num_of_trials = 0;
//      uint32b num_of_failures = 0;
//      zisc::CompensatedSummation<float> sum_ulps{0.0f};
//      float max_ulps = 0.0f;
//
//      std::vector<cl::float4> results;
//      results.resize(2 * resolution);
//      results4->read(results.data(), results.size(), 0, 0);
//      for (uint32b i = 0; i < resolution; ++i) {
//        const auto& z = results[2 * i];
//        const auto& result = results[2 * i + 1];
//        for (uint32b j = 0; j < result.size(); ++j) {
//          float expected = std::atan(z[j]);
//          char func_name[256] = "zAtan(%g)=%g";
//          ::testFloat(func_name, z[j], expected, result[j],
//                      num_of_trials, num_of_failures, sum_ulps, max_ulps);
//        }
//      }
//      EXPECT_FALSE(num_of_failures)
//          << error_message << std::endl
//          << "Failures: " << num_of_failures << "/" << num_of_trials << ", "
//          << "maxulps=" << std::scientific << max_ulps << ", "
//          << "averageulps=" << std::scientific << (sum_ulps.get() / zisc::cast<float>(num_of_failures));
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, DotTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testDot, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'dot' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(12.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(24.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(12.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(24.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(11.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zDotImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzDotImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zDotImpl' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(12.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(24.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(12.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(24.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(11.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zDotTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzDot, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zDot' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(12.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(24.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(12.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(24.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(11.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, CrossTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n3 = 4;
//    const uint32b n4 = 4;
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//#if defined(Z_MAC)
//    FAIL();
//#else // Z_MAC
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testCross, 1);
//    kernel->run(*results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//#endif // Z_MAC
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'cross' func is wrong.";
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(n3);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i++].z) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zCrossImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n3 = 4;
//    const uint32b n4 = 4;
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzCrossImpl, 1);
//    kernel->run(*results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zCrossImpl' func is wrong.";
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(n3);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i++].z) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zCrossTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n3 = 4;
//    const uint32b n4 = 4;
//    auto results3 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzCross, 1);
//    kernel->run(*results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zCross' func is wrong.";
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(n3);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i++].z) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i++].z) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(0.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(1.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(-3.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//      ASSERT_FLOAT_EQ(-14.0f, results[i].x) << error_message;
//      ASSERT_FLOAT_EQ(-6.0f, results[i].y) << error_message;
//      ASSERT_FLOAT_EQ(10.0f, results[i].z) << error_message;
//      ASSERT_FLOAT_EQ(0.0f, results[i++].w) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, LengthTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testLength, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'length' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(2.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zLengthImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLengthImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zLengthImpl' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(2.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zLengthTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzLength, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zLength' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(2.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, DistanceTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testDistance, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'distance' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(2.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zDistanceImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzDistanceImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zDistanceImpl' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(2.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zDistanceTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 3;
//    const uint32b n3 = 3;
//    const uint32b n3f4 = 3;
//    const uint32b n4 = 2;
//    auto results1 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<float>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzDistance, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zDistance' func is wrong.";
//    // Vector2
//    {
//      std::vector<float> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(2.0f, results[i++]) << error_message;
//    }
//
//    // Vector3
//    {
//      std::vector<float> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector3f4
//    {
//      std::vector<float> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//      ASSERT_FLOAT_EQ(5.0f, results[i++]) << error_message;
//    }
//
//    // Vector4
//    {
//      std::vector<float> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      ASSERT_FLOAT_EQ(6.0f, results[i++]) << error_message;
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, NormalizeTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 4;
//    const uint32b n3 = 4;
//    const uint32b n3f4 = 4;
//    const uint32b n4 = 4;
//    auto results1 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testNormalize, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'normalize' func is wrong.";
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector3f4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zNormalizeImplTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 4;
//    const uint32b n3 = 4;
//    const uint32b n3f4 = 4;
//    const uint32b n4 = 4;
//    auto results1 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzNormalizeImpl, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zNormalizeImpl' func is wrong.";
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector3f4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        auto p = results[i++];
//        p.w = 0.0f;
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    std::cout << std::endl;
//  }
//}
//
//TEST(MathTest, zNormalizeTest)
//{
//  using zisc::cast;
//  using namespace zinvul;
//  auto options = makeTestOptions();
//  auto device_list = makeTestDeviceList(options);
//  for (uint32b number = 0; number < device_list.size(); ++number) {
//    auto& device = device_list[number];
//    std::cout << getTestDeviceInfo(*device);
//
//    constexpr uint32b resolution = 1;
//
//    const uint32b n2 = 4;
//    const uint32b n3 = 4;
//    const uint32b n3f4 = 4;
//    const uint32b n4 = 4;
//    auto results1 = makeStorageBuffer<cl::float2>(device.get(), BufferUsage::kDeviceTSrc);
//    results1->setSize(n2);
//    auto results2 = makeStorageBuffer<cl::float3>(device.get(), BufferUsage::kDeviceTSrc);
//    results2->setSize(n3);
//    auto results3 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results3->setSize(n3f4);
//    auto results4 = makeStorageBuffer<cl::float4>(device.get(), BufferUsage::kDeviceTSrc);
//    results4->setSize(n4);
//
//    auto kernel = zinvul::makeZinvulKernel(device.get(), math, testzNormalize, 1);
//    kernel->run(*results1, *results2, *results3, *results4, {resolution}, 0);
//    device->waitForCompletion();
//
//    std::cout << getTestDeviceUsedMemory(*device) << std::endl;
//
//    const char* error_message = "The 'zNormalize' func is wrong.";
//    // Vector2
//    {
//      std::vector<cl::float2> results;
//      results.resize(n2);
//      results1->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 2; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector3
//    {
//      std::vector<cl::float3> results;
//      results.resize(n3);
//      results2->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 3; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector3f4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n3f4);
//      results3->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        auto p = results[i++];
//        p.w = 0.0f;
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    // Vector4
//    {
//      std::vector<cl::float4> results;
//      results.resize(n4);
//      results4->read(results.data(), results.size(), 0, 0);
//
//      uint32b i = 0;
//      {
//        const auto& p = results[i++];
//        const auto& n = results[i++];
//        const auto pl = cl::length(p);
//        const auto pn = cl::length(n);
//        ASSERT_FLOAT_EQ(1.0f, pn) << error_message;
//        for (uint32b j = 0; j < 4; ++j) {
//          const float expected = p[j];
//          const float result = pl * n[j];
//          ASSERT_FLOAT_EQ(expected, result) << error_message;
//        }
//      }
//    }
//
//    std::cout << std::endl;
//  }
//}

