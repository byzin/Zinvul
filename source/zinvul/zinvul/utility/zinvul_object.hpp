/*!
  \file zinvul_object.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ZINVUL_OBJECT_HPP
#define ZINVUL_ZINVUL_OBJECT_HPP

// Standard C++ library
#include <memory>
// Zisc
#include "zisc/non_copyable.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "id_data.hpp"
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \brief No brief description

  No detailed description.
  */
class ZinvulObject : private zisc::NonCopyable<ZinvulObject>
{
 public:
  // Type aliases
  using SharedPtr = std::shared_ptr<ZinvulObject>;
  using WeakPtr = std::weak_ptr<ZinvulObject>;


  //! Initialize the object
  ZinvulObject(IdData&& id) noexcept;

  //! Destroy the object
  virtual ~ZinvulObject() noexcept;


  //! Destroy the object
  void destroyObject() noexcept;

  //! Check if the object has own weak pointer
  bool hasOwn() const noexcept;

  //! Check if the object has shared parent pointer
  bool hasParent() const noexcept;

  //! Return the underlying ID data
  IdData& id() noexcept;

  //! Return the underlying ID data
  const IdData& id() const noexcept;

  //! Initialize the object
  void initObject(SharedPtr&& parent, WeakPtr&& own) noexcept;

  //! Check if the zinvul object is in debug mode
  virtual bool isDebugMode() const noexcept;

  //! Issue an ID of a child object
  virtual IdData issueId() noexcept;

  //! Return the underlying memory resource
  virtual zisc::pmr::memory_resource* memoryResource() noexcept;

  //! Return the underlying memory resource
  virtual const zisc::pmr::memory_resource* memoryResource() const noexcept;

  //! Return the sub-platform type
  virtual SubPlatformType type() const noexcept;

 protected:
  //! Return the own weak pointer
  const WeakPtr& getOwn() const noexcept;

  //! Return the parent pointer
  ZinvulObject* getParent() noexcept;

  //! Return the parent pointer
  const ZinvulObject* getParent() const noexcept;

 private:
  SharedPtr parent_;
  WeakPtr own_;
  IdData id_;
};

} // namespace zinvul

#include "zinvul_object-inl.hpp"

#endif // ZINVUL_ZINVUL_OBJECT_HPP
