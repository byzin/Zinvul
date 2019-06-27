/*!
  \file utility.cl
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_UTILITY_CL
#define ZINVUL_UTILITY_CL

// Zinvul
#include "types.cl"
#include "type_traits.cl"

namespace zinvul {

//! Write the message on standard output
template <typename ...Types>
int32b print(ConstConstantPtr<char> format, const Types... args) noexcept;

//! Assert the message on standard error output and raise an error
template <typename ...Types>
void assertIfFalse(const int32b condition,
                   ConstConstantPtr<char> format,
                   const Types... args) noexcept;

// Work-Item functions

//! Return the number of global work-items
uint32b getGlobalSize(const uint32b dimension) noexcept;

//! Return the number of global work-items of x dimension
uint32b getGlobalSizeX() noexcept;

//! Return the number of global work-items of y dimension
uint32b getGlobalSizeY() noexcept;

//! Return the number of global work-items of z dimension
uint32b getGlobalSizeZ() noexcept;

//! Return the global work-item ID
uint32b getGlobalId(const uint32b dimension) noexcept;

//! Return the global work-item ID of x dimension
uint32b getGlobalIdX() noexcept;

//! Return the global work-item ID of y dimension
uint32b getGlobalIdY() noexcept;

//! Return the global work-item ID of z dimension
uint32b getGlobalIdZ() noexcept;

//! Return the number of local work-items
uint32b getLocalSize(const uint32b dimension) noexcept;

//! Return the number of local work-items of x dimension
uint32b getLocalSizeX() noexcept;

//! Return the number of local work-items of y dimension
uint32b getLocalSizeY() noexcept;

//! Return the number of local work-items of z dimension
uint32b getLocalSizeZ() noexcept;

//! Return the local work-item ID
uint32b getLocalId(const uint32b dimension) noexcept;

//! Return the local work-item ID of x dimension
uint32b getLocalIdX() noexcept;

//! Return the local work-item ID of y dimension
uint32b getLocalIdY() noexcept;

//! Return the local work-item ID of z dimension
uint32b getLocalIdZ() noexcept;

//! Return the number of work-groups
uint32b getNumGroups(const uint32b dimension) noexcept;

//! Return the number of work-groups of x dimension
uint32b getNumGroupsX() noexcept;

//! Return the number of work-groups of y dimension
uint32b getNumGroupsY() noexcept;

//! Return the number of work-groups of z dimension
uint32b getNumGroupsZ() noexcept;

//! Return the work-group ID
uint32b getGroupId(const uint32b dimension) noexcept;

//! Return the work-group ID of x dimension
uint32b getGroupIdX() noexcept;

//! Return the work-group ID of y dimension
uint32b getGroupIdY() noexcept;

//! Return the work-group ID of z dimension
uint32b getGroupIdZ() noexcept;

//! Convert type from T to Type
template <typename Type, typename T>
Type cast(T value) noexcept;

//! Treat T* as Type*
template <typename Type, typename T>
Type treatAs(T object) noexcept;

//!
template <typename Type>
Type&& forward(RemoveReferenceType<Type>& t) noexcept;

template <typename Type>
Type&& forward(RemoveReferenceType<Type>&& t) noexcept;

} // namespace zinvul

#include "utility-inl.cl"

#endif /* ZINVUL_UTILITY_CL */
