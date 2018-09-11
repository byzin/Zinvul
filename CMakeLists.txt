# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.10)

function(checkSubmodule submodule_path)
  if(NOT EXISTS ${submodule_path})
    get_filename_component(submodule_name "${submodule_path}" NAME)
    message(FATAL_ERROR "Submodule '${submodule_name}' not found, please init submodules.")
  endif()
endfunction(checkSubmodule)

# Set project information
project(Zinvul LANGUAGES C CXX)
include(${PROJECT_SOURCE_DIR}/cmake/project.cmake)
setZinvulProperties()
message(STATUS "Zinvul version: ${PROJECT_VERSION}")

# Configure build types
set(CMAKE_CONFIGURATION_TYPES "Debug" "RelWithDebInfo" "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(zisc_path ${CMAKE_SOURCE_DIR}/source/dependencies/zisc)
checkSubmodule(${zisc_path})

# Build configuration
include(${zisc_path}/cmake/general.cmake)
restrictBuildDirectory(${PROJECT_SOURCE_DIR}/build)
detectEnvironment(environment_definitions)
# Command option
include(${PROJECT_SOURCE_DIR}/cmake/option.cmake)
initCommandOptions()
#
include(${zisc_path}/cmake/compiler.cmake)
initCompilerOption()
getCxxCompilerOption(cxx_compiler_flags cxx_linker_flags cxx_definitions)

# Common libraries
# Thread
find_package(Threads REQUIRED)

# Unit tests
include(${PROJECT_SOURCE_DIR}/test/config.cmake)
buildUnitTest()
