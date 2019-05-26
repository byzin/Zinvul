/*!
  \file kernel_arg_parser-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_ARG_PARSER_INL_HPP
#define ZINVUL_KERNEL_ARG_PARSER_INL_HPP

#include "kernel_arg_parser.hpp"
// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Zisc
#include "zisc/zisc_config.hpp"
// Zinvul
#include "kernel.hpp"
#include "cppcl/address_space_pointer.hpp"

namespace zinvul {

namespace cppinner {

template <typename ...Arguments>
class KernelArgParserImpl
{
 public:
//  class ArgParseResult
//  {
//   public:
//    struct ArgumentData
//    {
//      uint8b is_global_ = zisc::kFalse;
//      uint8b is_local_ = zisc::kFalse;
//      uint8b is_const_ = zisc::kFalse;
//      uint8b is_pod_ = zisc::kFalse;
//    };
//
//    static constexpr std::size_t kNumOfGlobal = 0;
//    static constexpr std::size_t kNumOfLocal = 0;
//    static constexpr std::size_t kNumOfArgs = 0;
//
//    static constexpr auto globalList() noexcept
//    {
//      std::array<std::size_t, kNumOfGlobal> global_list;
//      return global_list;
//    }
//
//    static constexpr auto localList() noexcept
//    {
//      std::array<std::size_t, kNumOfLocal> local_list;
//      return local_list;
//    }
//
//    static constexpr auto argumentDataList() noexcept
//    {
//      std::array<ArgumentData, kNumOfArgs> argument_data_list;
//      return argument_data_list;
//    }
//  };
};

template <typename T, typename ...Arguments>
class KernelArgParserImpl<T, Arguments...>
{
  // POD type
//  template <typename Type>
//  struct AddressSpaceParser
//  {
//    static constexpr uint8b kIsGlobal = 1;
//    static constexpr uint8b kIsLocal = 0;
//    static constexpr uint8b kIsPod = zisc::kTrue;
//  };
//
//  // Global or Local types
//  template <typename Type, cl::AddressSpaceType kAddressType>
//  struct AddressSpaceParser<cl::AddressSpacePointer<Type, kAddressType>>
//  {
//    static constexpr uint8b kIsGlobal =
//        (kAddressType == cl::AddressSpaceType::kGlobal) ? 1 : 0;
//    static constexpr uint8b kIsLocal =
//        (kAddressType == cl::AddressSpaceType::kLocal) ? 1 : 0;
//    static constexpr uint8b kIsPod = zisc::kFalse;
//    static_assert(kIsGlobal || kIsLocal, "The address space is wrong.");
//  };
//
//  using Type = std::remove_volatile_t<T>;
//  using ASpaceResult = AddressSpaceParser<std::remove_cv_t<Type>>;
//  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
//
//  static constexpr uint8b kIsGlobal = ASpaceResult::kIsGlobal;
//  static constexpr uint8b kIsLocal = ASpaceResult::kIsLocal;
//  static constexpr uint8b kIsPod = ASpaceResult::kIsPod;

//  template <typename ...Types> struct ExtendedKernel;
//
//  template <std::size_t kDimension, typename ...Types>
//  struct ExtendedKernel<Kernel<kDimension, Types...>>
//  {
//    using KernelType = Kernel<kDimension, BufferType, Types...>;
//  };

 public:
//  class ArgParseResult
//  {
//    using Parent = typename KernelArgParserImpl<Arguments...>::ArgParseResult;
//
//   public:
//    using ArgumentData = typename Parent::ArgumentData;
//
//    static constexpr std::size_t kNumOfGlobal = Parent::kNumOfGlobal + kIsGlobal;
//    static constexpr std::size_t kNumOfLocal = Parent::kNumOfLocal + kIsLocal;
//    static constexpr std::size_t kNumOfArgs = Parent::kNumOfArgs + 1;
//
//    static constexpr auto globalList() noexcept
//    {
//      std::array<std::size_t, kNumOfGlobal> global_list;
//      if (kIsGlobal)
//        global_list[0] = 0;
//      const auto parent_list = Parent::globalList();
//      for (std::size_t i = 0; i < parent_list.size(); ++i)
//        global_list[i + kIsGlobal] = parent_list[i] + 1;
//      return global_list;
//    }
//
//    static constexpr auto localList() noexcept
//    {
//      std::array<std::size_t, kNumOfLocal> local_list;
//      if (kIsLocal)
//        local_list[0] = 0;
//      const auto parent_list = Parent::localList();
//      for (std::size_t i = 0; i < parent_list.size(); ++i)
//        local_list[i + kIsLocal] = parent_list[i] + 1;
//      return local_list;
//    }
//
//    static constexpr auto argumentDataList() noexcept
//    {
//      std::array<ArgumentData, kNumOfArgs> argument_data_list;
//      const auto parent_list = Parent::argumentDataList();
//      argument_data_list[0].is_global_ = kIsGlobal;
//      argument_data_list[0].is_local_ = kIsLocal;
//      argument_data_list[0].is_const_ = std::is_const_v<Type> ? zisc::kTrue :
//                                                                zisc::kFalse;
//      argument_data_list[0].is_pod_ = kIsPod;
//      for (std::size_t i = 0; i < parent_list.size(); ++i)
//        argument_data_list[i + 1] = parent_list[i];
//      return argument_data_list;
//    }
//  };
};

} // namespace cppinner

} // namespace zinvul

#endif // ZINVUL_KERNEL_ARG_PARSER_INL_HPP
