# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#

cmake_minimum_required(VERSION 3.16)


project(ZinvulProject VERSION 0.0.1 LANGUAGES CXX)

## Cmake configurations
set(CMAKE_CONFIGURATION_TYPES "Debug" "RelWithDebInfo" "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(InstallRequiredSystemLibraries)

set(zisc_path ${PROJECT_SOURCE_DIR}/source/dependencies/zisc/source/zisc)
include(${zisc_path}/cmake/general.cmake)
message(STATUS "System information:")
printSystemInfo("  + ")
## Project options
include(${PROJECT_SOURCE_DIR}/cmake/project.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/option.cmake)
initProjectOptions()
restrictBuildDirectory(${PROJECT_SOURCE_DIR}/build)

# Build Zinvul
set(zinvul_path ${PROJECT_SOURCE_DIR}/source/zinvul)
add_subdirectory(${zinvul_path} ${PROJECT_BINARY_DIR}/Zinvul)

# Build examples
add_subdirectory(${PROJECT_SOURCE_DIR}/example ${PROJECT_BINARY_DIR}/Examples)

# Build unit test
if(ZINVUL_BUILD_TESTS)
  # GoogleTest
  set(google_test_path ${PROJECT_SOURCE_DIR}/test/googletest)
  addGoogleTest(${google_test_path} ${PROJECT_BINARY_DIR}/GoogleTest)
  attachZinvulOption(gtest)
  attachZinvulOption(gtest_main)
  add_subdirectory(${PROJECT_SOURCE_DIR}/test ${PROJECT_BINARY_DIR}/UnitTest)
endif()

# Build documents
add_subdirectory(${PROJECT_SOURCE_DIR}/document ${PROJECT_BINARY_DIR}/Document)
