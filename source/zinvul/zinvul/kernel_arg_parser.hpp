/*!
  \file kernel_arg_parser.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_ARG_PARSER_HPP
#define ZINVUL_KERNEL_ARG_PARSER_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <type_traits>
// Zinvul
#include "kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \brief POD type info
  */
template <typename Type>
class AddressSpaceInfo
{
 public:
  using ElementType = std::remove_cv_t<Type>;

  static constexpr bool kIsGlobal = true;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsPod = true;
  static constexpr bool kIsConst = std::is_const_v<Type>;

 private:
  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");
};

/*!
  \brief Type information of a kernel arg
  */
template <typename Type>
class KernelArgInfo
{
  using ASpaceInfo = AddressSpaceInfo<std::remove_cv_t<Type>>;

 public:
  using ElementType = typename ASpaceInfo::ElementType;

  static constexpr bool kIsGlobal = ASpaceInfo::kIsGlobal;
  static constexpr bool kIsLocal = ASpaceInfo::kIsLocal;
  static constexpr bool kIsPod = ASpaceInfo::kIsPod;
  static constexpr bool kIsConst = std::is_const_v<Type> || ASpaceInfo::kIsConst;
  static constexpr DescriptorType kDescriptor = (kIsPod || kIsConst)
      ? DescriptorType::kUniform
      : DescriptorType::kStorage;

 private:
  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");
  static_assert(!(!kIsPod && std::is_const_v<Type>),
                "The address space pointer is const-qualified.");
};

class KernelArgParseResult
{
 public:
  //! Initialize a parse result
  constexpr KernelArgParseResult() noexcept;

  //! Initialize a parse result
  constexpr KernelArgParseResult(const bool is_global,
                                 const bool is_local,
                                 const bool is_pod,
                                 const bool is_const) noexcept;


  //! Check if the argument is const qualified
  constexpr bool isConst() const noexcept;

  //! Check if the argument is global
  constexpr bool isGlobal() const noexcept;

  //! Check if the argument is local
  constexpr bool isLocal() const noexcept;

  //! Check if the argument is pod
  constexpr bool isPod() const noexcept;

  //! Return the position of the argument
  constexpr std::size_t index() const noexcept;

  //! Set an index of the argument
  constexpr void setIndex(const std::size_t index) noexcept;

 private:
  bool is_global_;
  bool is_local_;
  bool is_pod_;
  bool is_const_;
  std::size_t index_;
};

/*!
  \brief Type information of kernel arguments
  */
template <std::size_t kDimension, typename ...ArgumentTypes>
class KernelArgParser
{
 public:
  using KernelWithoutLocal = Kernel<kDimension>;


  static constexpr std::size_t kNumOfArgs = 0; //!< The number of arguments
  static constexpr std::size_t kNumOfGlobalArgs = 0; //!< The number of globals
  static constexpr std::size_t kNumOfLocalArgs = 0; //!< The number of locals


  //! Return the info of arguments
  static constexpr std::array<KernelArgParseResult, kNumOfArgs>
      getArgInfoList() noexcept
  {
    std::array<KernelArgParseResult, kNumOfArgs> result_list;
    return result_list;
  }

  //! Return the info of global arguments
  static constexpr std::array<KernelArgParseResult, kNumOfGlobalArgs>
      getGlobalArgInfoList() noexcept
  {
    std::array<KernelArgParseResult, kNumOfGlobalArgs> result_list;
    return result_list;
  }

  //! Return the info of local arguments
  static constexpr std::array<KernelArgParseResult, kNumOfLocalArgs>
      getLocalArgInfoList() noexcept
  {
    std::array<KernelArgParseResult, kNumOfLocalArgs> result_list;
    return result_list;
  }

  //! Check if there are any const local arguments in the kernel arguments 
  static constexpr bool hasConstLocal() noexcept
  {
    return false;
  }
};

} // namespace zinvul

#include "kernel_arg_parser-inl.hpp"

#endif // ZINVUL_KERNEL_ARG_PARSER_HPP
