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

set(zisc_path ${PROJECT_SOURCE_DIR}/source/dependencies/zisc/source/zisc)
include(${zisc_path}/cmake/general.cmake)
message(STATUS "System information:")
printSystemInfo("  + ")
## Project options
include(${PROJECT_SOURCE_DIR}/cmake/option.cmake)
initProjectOptions()
restrictBuildDirectory(${PROJECT_SOURCE_DIR}/build)

# cpu_features
set(cpu_features_path ${PROJECT_SOURCE_DIR}/source/dependencies/cpu_features)
add_subdirectory(${cpu_features_path} ${PROJECT_BINARY_DIR}/cpu_features)

# Build Zisc
add_subdirectory(${zisc_path} ${PROJECT_BINARY_DIR}/Zisc)

# Build unit test
include(${PROJECT_SOURCE_DIR}/cmake/googletest.cmake)
addZinvulGoogleTest()

# Build documents
# add_subdirectory(${PROJECT_SOURCE_DIR}/document ${PROJECT_BINARY_DIR}/Document)
