/*!
  \file id_data.hpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2020 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZINVUL_ID_DATA_HPP
#define ZINVUL_ID_DATA_HPP

// Standard C++ library
#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
// Zisc
#include "zisc/non_copyable.hpp"
// Zinvul
#include "zinvul/zinvul_config.hpp"

namespace zinvul {

/*!
  \brief No brief description

  No detailed description.
  */
class IdData : private zisc::NonCopyable<IdData>
{
 public:
  //! Create an ID data
  IdData(const uint32b id) noexcept;

  //! Move a data
  IdData(IdData&& other) noexcept;


  //! Move a data
  IdData& operator=(IdData&& other) noexcept;


  //! Return the array size of the data
  static constexpr std::size_t dataSize() noexcept;

  //! Return the file name where this data is created
  std::string_view fileName() const noexcept;

  //! Return the data by the given index
  void* getData(const std::size_t index) noexcept;

  //! Return the data by the given index
  const void* getData(const std::size_t index) const noexcept;

  //! Chekc if this ID data has a data
  bool hasData(const std::size_t index) const noexcept;

  //! Check if this data has a file name and a line number where it is created
  bool hasFileInfo() const noexcept;

  //! Check if this data has a name
  bool hasName() const noexcept;

  //! Return the ID
  uint32b id() const noexcept;

  //! Return the invalid ID number
  static constexpr uint32b invalidId() noexcept;

  //! Return the line number where this data is created
  uint32b lineNumber() const noexcept;

  //! Return the name of this data
  std::string_view name() const noexcept;

  //! Set a data by the given index
  void setData(void* data, const std::size_t index) noexcept;

  //! Set a file name and a line number where this data is created
  void setFileInfo(std::string_view file_name,
                   const uint32b line_number) noexcept;

  //! Set a name
  void setName(std::string_view data_name) noexcept;

 private:
  static constexpr std::size_t kDataSize = 4;
  static constexpr std::size_t kMaxNameLength = 128;


  std::array<void*, kDataSize> data_list_;
  uint32b id_;
  uint32b line_number_;
  char name_[kMaxNameLength] = "";
  char file_name_[kMaxNameLength] = "";
};

} // namespace zinvul

#include "id_data-inl.hpp"

#endif // ZINVUL_ID_DATA_HPP
