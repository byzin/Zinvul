/*!
  \file kernel_arg_parser-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2020 Sho Ikeda
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
#include "buffer.hpp"
#include "kernel.hpp"
#include "cppcl/address_space_pointer.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \brief Global or Local type info
  */
template <cl::AddressSpaceType kAddressType, typename Type>
class AddressSpaceInfo<cl::AddressSpacePointer<kAddressType, Type>>
{
 public:
  using ElementType = std::remove_cv_t<Type>;

  static constexpr bool kIsConstant = (kAddressType == cl::AddressSpaceType::kConstant);
  static constexpr bool kIsGlobal = (kAddressType == cl::AddressSpaceType::kGlobal) ||
                                    kIsConstant;
  static constexpr bool kIsLocal = (kAddressType == cl::AddressSpaceType::kLocal);
  static constexpr bool kIsPod = false;

  static_assert(kIsGlobal || kIsLocal, "The address space is wrong.");

 private:
  static_assert(!std::is_pointer_v<ElementType>, "The element type is pointer.");
  static_assert(!std::is_reference_v<ElementType>, "The element type is reference.");
};

/*!
  */
inline
constexpr KernelArgParseResult::KernelArgParseResult() noexcept :
    is_global_{false},
    is_local_{false},
    is_pod_{false},
    is_constant_{false},
    index_{0}
{
}

/*!
  */
inline
constexpr KernelArgParseResult::KernelArgParseResult(const bool is_global,
                                                     const bool is_local,
                                                     const bool is_pod,
                                                     const bool is_constant)
    noexcept :
        is_global_{is_global},
        is_local_{is_local},
        is_pod_{is_pod},
        is_constant_{is_constant},
        index_{0}
{
}

/*!
  */
inline
constexpr bool KernelArgParseResult::isConstant() const noexcept
{
  return is_constant_;
}

/*!
  */
inline
constexpr bool KernelArgParseResult::isGlobal() const noexcept
{
  return is_global_;
}

/*!
  */
inline
constexpr bool KernelArgParseResult::isLocal() const noexcept
{
  return is_local_;
}

/*!
  */
inline
constexpr bool KernelArgParseResult::isPod() const noexcept
{
  return is_pod_;
}

/*!
  */
inline
constexpr std::size_t KernelArgParseResult::index() const noexcept
{
  return index_;
}

/*!
  */
inline
constexpr void KernelArgParseResult::setIndex(const std::size_t index) noexcept
{
  index_ = index;
}

/*!
  */
template <std::size_t kDimension, typename T, typename ...ArgumentTypes>
class KernelArgParser<kDimension, T, ArgumentTypes...>
{
  using Parent = KernelArgParser<kDimension, ArgumentTypes...>;
  using ArgInfo = KernelArgInfo<T>;

  template <typename ...Types> struct ExtendedKernel;

  template <typename ...Types>
  struct ExtendedKernel<Kernel<kDimension, Types...>>
  {
    using BufferArg = Buffer<ArgInfo::kDescriptor, typename ArgInfo::ElementType>;
    using KernelType = Kernel<kDimension, BufferArg, Types...>;
  };

 public:
  using KernelWithoutLocal = std::conditional_t<ArgInfo::kIsLocal,
      typename Parent::KernelWithoutLocal,
      typename ExtendedKernel<typename Parent::KernelWithoutLocal>::KernelType>;


  static constexpr std::size_t kNumOfArgs = Parent::kNumOfArgs + 1;
  static constexpr std::size_t kNumOfGlobalArgs = ArgInfo::kIsGlobal
      ? Parent::kNumOfGlobalArgs + 1
      : Parent::kNumOfGlobalArgs;
  static constexpr std::size_t kNumOfLocalArgs = ArgInfo::kIsLocal
      ? Parent::kNumOfLocalArgs + 1
      : Parent::kNumOfLocalArgs;
  static constexpr std::size_t kNumOfStorageBuffer =
      (ArgInfo::kIsGlobal && !(ArgInfo::kIsConstant || ArgInfo::kIsPod))
          ? Parent::kNumOfStorageBuffer + 1
          : Parent::kNumOfStorageBuffer;
  static constexpr std::size_t kNumOfUniformBuffer =
      (ArgInfo::kIsGlobal && (ArgInfo::kIsConstant || ArgInfo::kIsPod))
          ? Parent::kNumOfUniformBuffer + 1
          : Parent::kNumOfUniformBuffer;


  //! Return the info of arguments
  static constexpr std::array<KernelArgParseResult, kNumOfArgs>
      getArgInfoList() noexcept
  {
    std::array<KernelArgParseResult, kNumOfArgs> result_list;
    if constexpr (0u < Parent::kNumOfArgs) {
      auto parent_list = Parent::getArgInfoList();
      for (std::size_t i = 0; i < Parent::kNumOfArgs; ++i) {
        result_list[i + 1] = parent_list[i];
        result_list[i + 1].setIndex(i + 1);
      }
    }

    result_list[0] = KernelArgParseResult{ArgInfo::kIsGlobal,
                                          ArgInfo::kIsLocal,
                                          ArgInfo::kIsPod,
                                          ArgInfo::kIsConstant};
    result_list[0].setIndex(0);

    return result_list;
  }

  //! Return the info of global arguments
  static constexpr std::array<KernelArgParseResult, kNumOfGlobalArgs>
      getGlobalArgInfoList() noexcept
  {
    std::array<KernelArgParseResult, kNumOfGlobalArgs> result_list;
    if constexpr (0u < Parent::kNumOfGlobalArgs) {
      const std::size_t offset = ArgInfo::kIsGlobal ? 1u : 0u;
      auto parent_list = Parent::getGlobalArgInfoList();
      for (std::size_t i = 0; i < Parent::kNumOfGlobalArgs; ++i) {
        result_list[i + offset] = parent_list[i];
        result_list[i + offset].setIndex(result_list[i + offset].index() + 1);
      }
    }

    if constexpr (ArgInfo::kIsGlobal) {
      result_list[0] = KernelArgParseResult{ArgInfo::kIsGlobal,
                                            ArgInfo::kIsLocal,
                                            ArgInfo::kIsPod,
                                            ArgInfo::kIsConstant};
      result_list[0].setIndex(0);
    }

    return result_list;
  }

  //! Return the info of local arguments
  static constexpr std::array<KernelArgParseResult, kNumOfLocalArgs>
      getLocalArgInfoList() noexcept
  {
    std::array<KernelArgParseResult, kNumOfLocalArgs> result_list;
    if constexpr (0u < Parent::kNumOfLocalArgs) {
      const std::size_t offset = ArgInfo::kIsLocal ? 1u : 0u;
      auto parent_list = Parent::getLocalArgInfoList();
      for (std::size_t i = 0; i < Parent::kNumOfLocalArgs; ++i) {
        result_list[i + offset] = parent_list[i];
        result_list[i + offset].setIndex(result_list[i + offset].index() + 1);
      }
    }

    if constexpr (ArgInfo::kIsLocal) {
      result_list[0] = KernelArgParseResult{ArgInfo::kIsGlobal,
                                            ArgInfo::kIsLocal,
                                            ArgInfo::kIsPod,
                                            ArgInfo::kIsConstant};
      result_list[0].setIndex(0);
    }

    return result_list;
  }

  //! Check if there are const local arguments in the kernel arguments
  static constexpr bool hasConstLocal() noexcept
  {
    const bool is_const_local = std::is_const_v<T> && ArgInfo::kIsLocal;
    return is_const_local || Parent::hasConstLocal();
  }
};

} // namespace zinvul

#endif // ZINVUL_KERNEL_ARG_PARSER_INL_HPP
