# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.16)


set(__zisc_dir__ ${CMAKE_SOURCE_DIR}/source/dependencies/zisc/source/zisc)


set(project_description "Zinvul documents")
project(Document VERSION 0.0.1)
find_package(Doxygen)
if(DOXYGEN_FOUND)
  # Check dependencies
  include(${__zisc_dir__}/cmake/general.cmake)
  checkTarget(Zinvul)

  include(${__zisc_dir__}/cmake/document.cmake)

  # Doxygen custom properties
  # Project
  set(DOXYGEN_PROJECT_NAME Zinvul)
  set(DOXYGEN_PROJECT_NUMBER 0.0.1)
  set(DOXYGEN_PROJECT_BRIEF "Zinvul.")

  # HTML
  set(html_theme_path ${PROJECT_SOURCE_DIR}/doxygen_dark_theme)
  if(EXISTS ${html_theme_path})
    set(DOXYGEN_HTML_HEADER ${html_theme_path}/html_header.html)
    set(DOXYGEN_HTML_FOOTER ${html_theme_path}/html_footer.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${html_theme_path}/custom_dark_theme.css)
  endif()

  # Example
  #  set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR}/example)
  #  set(DOXYGEN_EXAMPLE_RECURSIVE YES)

  # Compile flags
  get_target_property(zinvul_definitions Zinvul COMPILE_DEFINITIONS)
  if(zinvul_definitions)
    set(DOXYGEN_PREDEFINED ${zinvul_definitions})
  endif()

  get_target_property(zinvul_source_files Zinvul SOURCES)
  addDoxygenDoc(document ${PROJECT_BINARY_DIR}
    SOURCE_FILES_OR_DIRS ${CMAKE_SOURCE_DIR}/README.md
                         ${CMAKE_SOURCE_DIR}/LICENSE.txt
                         ${__zisc_dir__}/document.dox
                         ${zinvul_source_files})
endif()
