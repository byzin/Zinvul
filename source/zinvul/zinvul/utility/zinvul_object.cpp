/*!
  \file zinvul_object.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "zinvul_object.hpp"
// Standard C++ library
#include <memory>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "id_data.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] id No description.
  */
ZinvulObject::ZinvulObject(IdData&& id) noexcept : id_{std::move(id)}
{
}

/*!
  \details No detailed description
  */
ZinvulObject::~ZinvulObject() noexcept
{
  destroyObject();
}

/*!
  \details No detailed description
  */
void ZinvulObject::destroyObject() noexcept
{
  own_.reset();
  parent_.reset();
}

/*!
  \details No detailed description

  \param [in] parent No description.
  \param [in] own No description.
  */
void ZinvulObject::initObject(SharedPtr&& parent, WeakPtr&& own) noexcept
{
  parent_ = std::move(parent);
  own_ = std::move(own);
}

/*!
  \details No detailed description

  \return No description
  */
bool ZinvulObject::isDebugMode() const noexcept
{
  ZISC_ASSERT(hasParent(), "Parent is null.");
  const auto p = getParent();
  const bool mode = p->isDebugMode();
  return mode;
}

/*!
  \details No detailed description

  \return No description
  */
IdData ZinvulObject::issueId() noexcept
{
  ZISC_ASSERT(hasParent(), "Parent is null.");
  auto p = getParent();
  IdData child_id = p->issueId();
  return child_id;
}

/*!
  \details No detailed description

  \return No description
  */
zisc::pmr::memory_resource* ZinvulObject::memoryResource() noexcept
{
  ZISC_ASSERT(hasParent(), "Parent is null.");
  auto p = getParent();
  zisc::pmr::memory_resource* mem_resource = p->memoryResource();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
const zisc::pmr::memory_resource* ZinvulObject::memoryResource() const noexcept
{
  ZISC_ASSERT(hasParent(), "Parent is null.");
  const auto p = getParent();
  const zisc::pmr::memory_resource* mem_resource = p->memoryResource();
  return mem_resource;
}

/*!
  \details No detailed description

  \return No description
  */
SubPlatformType ZinvulObject::type() const noexcept
{
  ZISC_ASSERT(hasParent(), "Parent is null.");
  const auto p = getParent();
  return p->type();
}

} // namespace zinvul
