/*!
  \file kernel_arg_parser.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_ARG_PARSER_HPP
#define ZINVUL_KERNEL_ARG_PARSER_HPP

#include "kernel_arg_parser-inl.hpp"
// Standard C++ library
#include <array>
#include <type_traits>
// Zinvul
#include "kernel.hpp"

namespace zinvul {

/*!
  */
template <typename ...Arguments>
class KernelArgParser
{
//  using ArgParseResult =
//      typename cppinner::KernelArgParserImpl<Arguments...>::ArgParseResult;
//
 public:
//  using ArgumentData = typename ArgParseResult::ArgumentData;
//
//  static constexpr std::size_t kNumOfGlobal = ArgParseResult::kNumOfGlobal;
//  static constexpr std::size_t kNumOfLocal = ArgParseResult::kNumOfLocal;
//  static constexpr std::size_t kNumOfArgs = ArgParseResult::kNumOfArgs;
//
//  static constexpr auto globalList() noexcept
//  {
//    return ArgParseResult::globalList();
//  }
//
//  static constexpr auto localList() noexcept
//  {
//    return ArgParseResult::localList();
//  }
//
//  static constexpr auto argumentDataList() noexcept
//  {
//    return ArgParseResult::argumentDataList();
//  }

  static_assert(std::is_pointer_v<Arguments> || ...,
                "The kernel has any pointer argument.");
};

} // namespace zinvul


#endif // ZINVUL_KERNEL_ARG_PARSER_HPP
