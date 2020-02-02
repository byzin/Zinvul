/*!
  \file id_data-inl.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ID_DATA_INL_HPP
#define ZINVUL_ID_DATA_INL_HPP

#include "id_data.hpp"
// Standard C++ library
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <string_view>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/std_memory_resource.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \details No detailed description

  \param [in] id No description.
  */
inline
IdData::IdData(const uint32b id) noexcept :
    data_list_{{nullptr, nullptr, nullptr, nullptr}},
    id_{id},
    line_number_{invalidId()}
{
}

/*!
  \details No detailed description

  \param [in] other No description.
  */
inline
IdData::IdData(IdData&& other) noexcept :
    data_list_{{nullptr, nullptr, nullptr, nullptr}},
    id_{invalidId()},
    line_number_{invalidId()}
{
  std::swap(data_list_, other.data_list_);
  std::swap(line_number_, other.line_number_);
  std::swap(id_, other.id_);
  std::strcpy(name_, other.name_);
  std::strcpy(file_name_, other.file_name_);
  // Clear the names of the other
  std::strcpy(other.name_, "");
  std::strcpy(other.file_name_, "");
}

/*!
  \details No detailed description

  \param [in] other No description.
  \return No description
  */
inline
IdData& IdData::operator=(IdData&& other) noexcept
{
  std::swap(data_list_, other.data_list_);
  std::swap(line_number_, other.line_number_);
  std::swap(id_, other.id_);
  std::strcpy(name_, other.name_);
  std::strcpy(file_name_, other.file_name_);
  // Clear the names of the other
  std::strcpy(other.name_, "");
  std::strcpy(other.file_name_, "");
  return *this;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr std::size_t IdData::dataSize() noexcept
{
  return kDataSize;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view IdData::fileName() const noexcept
{
  std::string_view n{file_name_};
  return n;
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
void* IdData::getData(const std::size_t index) noexcept
{
  ZISC_ASSERT(index < data_list_.size(), "The index is out of range.");
  return data_list_[index];
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
const void* IdData::getData(const std::size_t index) const noexcept
{
  ZISC_ASSERT(index < data_list_.size(), "The index is out of range.");
  return data_list_[index];
}

/*!
  \details No detailed description

  \param [in] index No description.
  \return No description
  */
inline
bool IdData::hasData(const std::size_t index) const noexcept
{
  const void* d = getData(index);
  const bool result = d != nullptr;
  return result;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool IdData::hasFileInfo() const noexcept
{
  const bool has_info = !fileName().empty() || (lineNumber() == invalidId());
  return has_info;
}

/*!
  \details No detailed description

  \return No description
  */
inline
bool IdData::hasName() const noexcept
{
  const bool has_info = !name().empty();
  return has_info;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b IdData::id() const noexcept
{
  return id_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
constexpr uint32b IdData::invalidId() noexcept
{
  const uint32b value = std::numeric_limits<uint32b>::max();
  return value;
}

/*!
  \details No detailed description

  \return No description
  */
inline
uint32b IdData::lineNumber() const noexcept
{
  return line_number_;
}

/*!
  \details No detailed description

  \return No description
  */
inline
std::string_view IdData::name() const noexcept
{
  std::string_view n{name_};
  return n;
}

/*!
  \details No detailed description

  \param [in] data No description.
  \param [in] index No description.
*/
inline
void IdData::setData(void* data, const std::size_t index) noexcept
{
  ZISC_ASSERT(index < data_list_.size(), "The index is out of range.");
  data_list_[index] = data;
}

/*!
  \details No detailed description

  \param [in] file_name No description.
  \param [in] line_number No description.
  */
inline
void IdData::setFileInfo(std::string_view file_name,
                         const uint32b line_number) noexcept
{
  std::strcpy(file_name_, file_name.data());
  line_number_ = line_number;
}

/*!
  \details No detailed description

  \param [in] data_name No description.
  */
inline
void IdData::setName(std::string_view data_name) noexcept
{
  std::strcpy(name_, data_name.data());
}

} // namespace zinvul

#endif // ZINVUL_ID_DATA_INL_HPP
