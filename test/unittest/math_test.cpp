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
      EXPECT_FALSE(num_of_failures) << std::scientific
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

      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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

TEST(MathTest, Exp2Test)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testExp2));
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
        const auto expected = std::exp2(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("exp2", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'exp2' func failed." << std::endl
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

TEST(MathTest, Exp2BuiltinTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testExp2Builtin));
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
        const auto expected = std::exp2(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::exp2", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'builtin::exp2' func failed." << std::endl
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

TEST(MathTest, Exp2ZinvulTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testExp2Zinvul));
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
        const auto expected = std::exp2(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::exp2", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::exp2' func failed." << std::endl
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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

TEST(MathTest, CbrtTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCbrt));
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
        const auto expected = std::cbrt(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("cbrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'cbrt' func failed." << std::endl
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

TEST(MathTest, CbrtBuiltinTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCbrtBuiltin));
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
        const auto expected = std::cbrt(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("builtin::cbrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'builtin::cbrt' func failed." << std::endl
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

TEST(MathTest, CbrtZinvulTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testCbrtZinvul));
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
        const auto expected = std::cbrt(x);
        const auto result = results[i];
        float* y = nullptr;
        ::testFloat("zinvul::cbrt", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::cbrt' func failed." << std::endl
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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

TEST(MathTest, ModfTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testModf));
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

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        float fexpected = 0.0f;
        const auto expected = std::modf(x, &fexpected);
        const auto result = results[i];
        const auto fresult = fresults[i];
        float* y = nullptr;
        ::testFloat("modf", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
        if (std::isnan(fexpected))
          ASSERT_TRUE(std::isnan(fresult)) << "'modf' func failed.";
        else
          ASSERT_FLOAT_EQ(fexpected, fresult) << "'modf' func failed.";
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'modf' func failed." << std::endl
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

TEST(MathTest, ModfBuiltinTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testModfBuiltin));
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

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        float fexpected = 0.0f;
        const auto expected = std::modf(x, &fexpected);
        const auto result = results[i];
        const auto fresult = fresults[i];
        float* y = nullptr;
        ::testFloat("builtin::modf", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
        if (std::isnan(fexpected))
          ASSERT_TRUE(std::isnan(fresult)) << "'builtin::modf' func failed.";
        else
          ASSERT_FLOAT_EQ(fexpected, fresult) << "'builtin::modf' func failed.";
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'builtin::modf' func failed." << std::endl
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

TEST(MathTest, ModfZinvulTest)
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

    auto kernel = makeKernel<1>(device.get(), ZINVUL_MAKE_KERNEL_ARGS(math, testModfZinvul));
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

      uint32b num_of_trials = 0;
      uint32b num_of_failures = 0;
      uint32b num_of_inf_failures = 0;
      uint32b num_of_nan_failures = 0;
      zisc::CompensatedSummation<float> sum_ulps;
      float max_ulps = 0.0f;
      float max_diff = 0.0f;

      for (uint32b i = 0; i < inputs.size(); ++i) {
        const auto x = inputs[i];
        float fexpected = 0.0f;
        const auto expected = std::modf(x, &fexpected);
        const auto result = results[i];
        const auto fresult = fresults[i];
        float* y = nullptr;
        ::testFloat("zinvul::modf", x, y, expected, result,
                    num_of_trials, num_of_failures,
                    num_of_inf_failures, num_of_nan_failures,
                    sum_ulps, max_ulps, max_diff);
        if (std::isnan(fexpected))
          ASSERT_TRUE(std::isnan(fresult)) << "'zinvul::modf' func failed.";
        else
          ASSERT_FLOAT_EQ(fexpected, fresult) << "'zinvul::modf' func failed.";
      }
      EXPECT_FALSE(num_of_failures) << std::scientific
          << "'zinvul::modf' func failed." << std::endl
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
      EXPECT_FALSE(num_of_failures) << std::scientific
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
