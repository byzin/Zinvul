/*!
  \file zinvul_object-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_OBJECT_INL_HPP
#define ZINVUL_ZINVUL_OBJECT_INL_HPP

#include "zinvul_object.hpp"
// Standard C++ library
#include <memory>
#include <utility>
// Zisc
#include "zisc/std_memory_resource.hpp"
#include "zisc/utility.hpp"
// Zinvul
#include "id_data.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \return No description
  */
inline
bool ZinvulObject::hasOwn() const noexcept
{
  const auto& o = getOwn();
  const bool result = !o.expired();
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool ZinvulObject::hasParent() const noexcept
{
  const bool result = getParent() != nullptr;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
IdData& ZinvulObject::id() noexcept
{
  return id_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const IdData& ZinvulObject::id() const noexcept
{
  return id_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
auto ZinvulObject::getOwn() const noexcept -> const WeakPtr&
{
  return own_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
ZinvulObject* ZinvulObject::getParent() noexcept
{
  auto p = parent_.get();
  return p;
}

/*!
  \details No detailed description

  \return No description
  */
inline
const ZinvulObject* ZinvulObject::getParent() const noexcept
{
  const auto p = parent_.get();
  return p;
}

} // namespace zinvul

#endif // ZINVUL_ZINVUL_OBJECT_INL_HPP
