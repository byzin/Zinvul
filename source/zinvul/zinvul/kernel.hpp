/*!
  \file kernel.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_KERNEL_HPP
#define ZINVUL_KERNEL_HPP

#include <cstddef>
#include <type_traits>

namespace zinvul {

// Forward declaration
template <typename> class Buffer;

/*!
  */
template <typename ...ArgumentTypes>
class Kernel
{
 protected:
  template <typename Type>
  using AddBufferRef =
      std::add_lvalue_reference_t<Buffer<std::remove_pointer_t<Type>>>;

 public:
  //! Execute a kernel
  virtual void run(AddBufferRef<ArgumentTypes>...) noexcept = 0;

  //! Return the number of a kernel arguments
  static constexpr std::size_t numOfArguments() noexcept;

 private:
};

} // namespace zinvul

#endif // ZINVUL_KERNEL_HPP
