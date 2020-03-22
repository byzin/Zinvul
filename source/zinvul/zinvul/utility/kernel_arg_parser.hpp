/*!
  \file kernel_arg_parser.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
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
//#include "kernel.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \brief POD type info

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class AddressSpaceInfo
{
 public:
  using ElementType = std::remove_cv_t<Type>;


  static constexpr bool kIsGlobal = true;
  static constexpr bool kIsLocal = false;
  static constexpr bool kIsConstant = false;
  static constexpr bool kIsPod = true;

 private:
  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");
};

/*!
  \brief Type information of a kernel arg

  No detailed description.

  \tparam Type No description.
  */
template <typename Type>
class KernelArgInfo
{
  using ASpaceInfo = AddressSpaceInfo<std::remove_cv_t<Type>>;

 public:
  using ElementType = typename ASpaceInfo::ElementType;


  static constexpr bool kIsGlobal = ASpaceInfo::kIsGlobal;
  static constexpr bool kIsLocal = ASpaceInfo::kIsLocal;
  static constexpr bool kIsConstant = ASpaceInfo::kIsConstant;
  static constexpr bool kIsPod = ASpaceInfo::kIsPod;

 private:
  static_assert(!std::is_pointer_v<Type>, "The Type is pointer.");
  static_assert(!std::is_reference_v<Type>, "The Type is reference.");
  static_assert(!(!kIsPod && std::is_const_v<Type>),
                "The address space pointer is const-qualified.");
};

/*!
  \brief No brief description

  No detailed description.
  */
class KernelArgParseResult
{
 public:
  //! Initialize a parse result
  constexpr KernelArgParseResult() noexcept;

  //! Initialize a parse result
  constexpr KernelArgParseResult(const bool is_global,
                                 const bool is_local,
                                 const bool is_pod,
                                 const bool is_constant) noexcept;


  //! Check if the argument is global qualified
  constexpr bool isGlobal() const noexcept;

  //! Check if the argument is local qualified
  constexpr bool isLocal() const noexcept;

  //! Check if the argument is constant qualified
  constexpr bool isConstant() const noexcept;

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
  bool is_constant_;
  std::size_t index_;
};

/*!
  \brief Type information of kernel arguments

  No detailed description.

  \tparam kDimension No description.
  \tparam ArgumentTypes No description.
  */
template <std::size_t kDimension, typename ...ArgumentTypes>
class KernelArgParser
{
 public:
  template <std::size_t kSize>
  using ResultList = std::array<KernelArgParseResult, kSize>;
  using KernelWithoutLocal = Kernel<kDimension>;



  static constexpr std::size_t kNumOfArgs = 0; //!< The number of arguments
  static constexpr std::size_t kNumOfGlobalArgs = 0; //!< The number of globals
  static constexpr std::size_t kNumOfLocalArgs = 0; //!< The number of locals
  static constexpr std::size_t kNumOfStorageBuffer = 0; //!< The number of storage
  static constexpr std::size_t kNumOfUniformBuffer = 0; //!< The number of uniform


  //! Return the info of arguments
  static constexpr ResultList<kNumOfArgs> getArgInfoList() noexcept
  {
    ResultList<kNumOfArgs> result_list;
    return result_list;
  }

  //! Return the info of global arguments
  static constexpr ResultList<kNumOfGlobalArgs> getGlobalArgInfoList() noexcept
  {
    ResultList<kNumOfGlobalArgs> result_list;
    return result_list;
  }

  //! Return the info of local arguments
  static constexpr ResultList<kNumOfLocalArgs> getLocalArgInfoList() noexcept
  {
    ResultList<kNumOfLocalArgs> result_list;
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
