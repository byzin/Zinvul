# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2019 Sho Ikeda
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
# Zisc
include(${zisc_path}/source/zisc/config.cmake)
loadZisc(zisc_header_files
         zisc_include_dirs
         zisc_compile_flags
         zisc_linker_flags
         zisc_definitions)
# Zinvul
include(${PROJECT_SOURCE_DIR}/source/zinvul/config.cmake)
loadZinvul(zinvul_source_files
           zinvul_include_dirs
           zinvul_compile_flags
           zinvul_linker_flags
           zinvul_definitions)
# Thread
find_package(Threads REQUIRED)
# Vulkan
if(ZINVUL_ENABLE_VULKAN_BACKEND)
  find_package(Vulkan REQUIRED)
  if(ZINVUL_BAKE_KERNELS)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
  endif()
  set(vma_include_dir ${PROJECT_SOURCE_DIR}/source/dependencies/VulkanMemoryAllocator/src)
endif()

# Unit tests
include(${PROJECT_SOURCE_DIR}/test/config.cmake)
buildUnitTest()
