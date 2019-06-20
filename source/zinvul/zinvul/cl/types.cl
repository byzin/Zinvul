/*!
  \file types.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_TYPES_CL
#define ZINVUL_TYPES_CL

#include "types-inl.cl"

namespace zinvul {

// Type aliases
// Integer types
typedef inner::int8b int8b;
typedef inner::int16b int16b;
typedef inner::int32b int32b;
typedef inner::int64b int64b;
// Unsigned integer types
typedef inner::uint8b uint8b;
typedef inner::uint16b uint16b;
typedef inner::uint32b uint32b;
typedef inner::uint64b uint64b;
// Address space types
// Global
template <typename Type>
using Global = inner::Global<Type>;
template <typename Type>
using GlobalPtr = inner::GlobalPtr<Type>;
template <typename Type>
using ConstGlobalPtr = inner::ConstGlobalPtr<Type>;
// Local
template <typename Type>
using Local = inner::Local<Type>;
template <typename Type>
using LocalPtr = inner::LocalPtr<Type>;
template <typename Type>
using ConstLocalPtr = inner::ConstLocalPtr<Type>;
// Constant
template <typename Type>
using Constant = inner::Constant<Type>;
template <typename Type>
using ConstantPtr = inner::ConstantPtr<Type>;
template <typename Type>
using ConstConstantPtr = inner::ConstConstantPtr<Type>;
// Private
template <typename Type>
using Private = inner::Private<Type>;
template <typename Type>
using PrivatePtr = inner::PrivatePtr<Type>;
template <typename Type>
using ConstPrivatePtr = inner::ConstPrivatePtr<Type>;
// Generic
template <typename Type>
using Generic = inner::Generic<Type>;
template <typename Type>
using GenericPtr = inner::GenericPtr<Type>;
template <typename Type>
using ConstGenericPtr = inner::ConstGenericPtr<Type>;

} // namespace zinvul

#endif /* ZINVUL_TYPES_CL */
