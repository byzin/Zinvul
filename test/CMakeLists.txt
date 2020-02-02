# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.16)


set(__zisc_dir__ ${CMAKE_SOURCE_DIR}/source/dependencies/zisc/source/zisc)


function(getTestCompileFlags test_compile_flags test_linker_flags test_definitions)
  set(compile_flags "")
  set(linker_flags "")
  set(definitions "")

  # Output variable
  set(${test_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${test_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${test_definitions} ${definitions} PARENT_SCOPE)
endfunction(getTestCompileFlags)


function(getTestWarningFlags test_warning_flags)
  set(warning_flags "")

  if(ZINVUL_SUPPRESS_EXCESSIVE_WARNING)
    if(Z_CLANG)
      list(APPEND warning_flags #-Wno-covered-switch-default
                                #-Wno-exit-time-destructors
                                #-Wno-extra-semi-stmt
                                -Wno-float-equal
                                #-Wno-global-constructors
                                #-Wno-old-style-cast
                                #-Wno-padded
                                -Wno-sign-conversion
                                #-Wno-undefined-reinterpret-cast
                                #-Wno-weak-vtables
                                #-Wno-zero-as-null-pointer-constant
                                )
    elseif(Z_GCC)
      list(APPEND warning_flags #-Wno-restrict
                                -Wno-sign-conversion
                                #-Wno-strict-overflow
                                #-Wno-unused-but-set-variable
                                #-Wno-redundant-move
                                )
    elseif(Z_MSVC)
      list(APPEND warning_flags /wd4996
                                )
    endif()
  endif()

  # Output variables
  set(${test_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getTestWarningFlags)


#function(buildUnitTest)
#  # Load GoogleTest
#  include(${zisc_path}/cmake/googletest.cmake)
#  set(gtest_project_root ${__test_root__}/googletest)
#  checkSubmodule(${gtest_project_root})
#  buildGoogleTest(${gtest_project_root} gtest_include_dir gtest_libraries)
#
#  # Test kernels
#  # Data
#  set(data_dir ${__test_root__}/kernels/data)
#  file(GLOB_RECURSE data_cl_files ${data_dir}/*.cl)
#  makeKernelSet(data data_source_files data_definitions
#    SOURCE_FILES ${data_cl_files} INCLUDE_DIRS ${data_dir} DEFINITIONS ${data_definitions})
#  # Vector
#  set(vector_dir ${__test_root__}/kernels/vector)
#  file(GLOB_RECURSE vector_cl_files ${vector_dir}/*.cl)
#  makeKernelSet(vector vector_source_files vector_definitions
#    SOURCE_FILES ${vector_cl_files} INCLUDE_DIRS ${vector_dir} DEFINITIONS ${vector_definitions})
#  # WorkItem
#  set(work_item_dir ${__test_root__}/kernels/work_item)
#  file(GLOB_RECURSE work_item_cl_files ${work_item_dir}/*.cl)
#  makeKernelSet(work_item work_item_files work_item_definitions
#      SOURCE_FILES ${work_item_cl_files} INCLUDE_DIRS ${work_item_dir})
#  # Atomic
#  set(atomic_dir ${__test_root__}/kernels/atomic)
#  file(GLOB_RECURSE atomic_cl_files ${atomic_dir}/*.cl)
#  makeKernelSet(atomic atomic_files atomic_definitions
#      SOURCE_FILES ${atomic_cl_files} INCLUDE_DIRS ${atomic_dir})
#  # VectorData
#  set(vector_data_dir ${__test_root__}/kernels/vector_data)
#  file(GLOB_RECURSE vector_data_cl_files ${vector_data_dir}/*.cl)
#  makeKernelSet(vector_data vector_data_files vector_data_definitions
#      SOURCE_FILES ${vector_data_cl_files} INCLUDE_DIRS ${vector_data_dir})
#  # Relational functions
#  set(relational_dir ${__test_root__}/kernels/relational)
#  file(GLOB_RECURSE relational_cl_files ${relational_dir}/*.cl)
#  makeKernelSet(relational relational_source_files relational_definitions
#      SOURCE_FILES ${relational_cl_files} INCLUDE_DIRS ${relational_dir})
#  # Algorithm functions
#  set(algorithm_dir ${__test_root__}/kernels/algorithm)
#  file(GLOB_RECURSE algorithm_cl_files ${algorithm_dir}/*.cl)
#  makeKernelSet(algorithm algorithm_source_files algorithm_definitions
#      SOURCE_FILES ${algorithm_cl_files} INCLUDE_DIRS ${algorithm_dir})
#  # Geometric functions
#  set(geometric_dir ${__test_root__}/kernels/geometric)
#  file(GLOB_RECURSE geometric_cl_files ${geometric_dir}/*.cl)
#  makeKernelSet(geometric geometric_source_files geometric_definitions
#      SOURCE_FILES ${geometric_cl_files} INCLUDE_DIRS ${geometric_dir})
#  # Math
#  set(math_dir ${__test_root__}/kernels/math)
#  file(GLOB_RECURSE math_cl_files ${math_dir}/*.cl)
#  makeKernelSet(math math_source_files math_definitions
#      SOURCE_FILES ${math_cl_files} INCLUDE_DIRS ${math_dir})
#  # Matrix
#  set(matrix_dir ${__test_root__}/kernels/matrix)
#  file(GLOB_RECURSE matrix_cl_files ${matrix_dir}/*.cl)
#  makeKernelSet(matrix matrix_source_files matrix_definitions
#      SOURCE_FILES ${matrix_cl_files} INCLUDE_DIRS ${matrix_dir})
#  # Random
#  set(rng_dir ${__test_root__}/kernels/rng)
#  file(GLOB_RECURSE rng_cl_files ${rng_dir}/*.cl)
#  makeKernelSet(rng rng_source_files rng_definitions
#      SOURCE_FILES ${rng_cl_files} INCLUDE_DIRS ${rng_dir})
#  # Experiment
#  set(experiment_dir ${__test_root__}/kernels/experiment)
#  file(GLOB_RECURSE experiment_cl_files ${experiment_dir}/*.cl)
#  makeKernelSet(experiment experiment_source_files experiment_definitions
#      SOURCE_FILES ${experiment_cl_files} INCLUDE_DIRS ${experiment_dir})
#
#  #
#  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/mathlog)
#
#  # Build unit tests
#  file(GLOB unittest_source_files ${__test_root__}/unittest/*.hpp
#                                  ${__test_root__}/unittest/*.cpp)
#  add_executable(UnitTest ${unittest_source_files}
#                          ${zisc_header_files}
#                          ${zinvul_source_files}
#                          ${data_source_files}
#                          ${vector_source_files}
#                          ${work_item_source_files}
#                          ${atomic_files}
#                          ${vector_data_files}
#                          ${relational_source_files}
#                          ${geometric_source_files}
#                          ${algorithm_source_files}
#                          ${math_source_files}
#                          ${matrix_source_files}
#                          ${rng_source_files}
#                          ${experiment_source_files})
#  source_group(UnitTest FILES ${unittest_source_files})
#  # Set unittest properties
#  set_target_properties(UnitTest PROPERTIES CXX_STANDARD 17
#                                            CXX_EXTENSIONS ON
#                                            CXX_STANDARD_REQUIRED ON)
#  getCxxWarningFlags(cxx_warning_flags)
#  getTestWarningFlags(test_warning_flags)
#  target_compile_options(UnitTest PRIVATE ${cxx_compiler_flags}
#                                          ${zisc_compile_flags}
#                                          ${zinvul_compile_flags}
#                                          ${cxx_warning_flags}
#                                          ${test_warning_flags})
#  target_include_directories(UnitTest PRIVATE ${PROJECT_SOURCE_DIR}/source
#                                              ${PROJECT_BINARY_DIR}/include
#                                              ${zisc_include_dirs}
#                                              ${zinvul_include_dirs})
#  target_include_directories(UnitTest SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS}
#                                                     ${vma_include_dir}
#                                                     ${gtest_include_dir})
#  target_link_libraries(UnitTest PRIVATE ${CMAKE_THREAD_LIBS_INIT}
#                                         ${Vulkan_LIBRARIES}
#                                         ${cxx_linker_flags}
#                                         ${zisc_linker_flags}
#                                         ${zinvul_linker_flags}
#                                         cpu_features
#                                         ${gtest_libraries})
#  target_compile_definitions(UnitTest PRIVATE ${cxx_definitions}
#                                              ${zisc_definitions}
#                                              ${zinvul_definitions}
#                                              ${environment_definitions}
#                                              ${data_definitions}
#                                              ${vector_definitions}
#                                              ${work_item_definitions}
#                                              ${atomic_definitions}
#                                              ${vector_data_definitions}
#                                              ${relational_definitions}
#                                              ${geometric_definitions}
#                                              ${algorithm_definitions}
#                                              ${math_definitions}
#                                              ${matrix_definitions}
#                                              ${rng_definitions}
#                                              ${experiment_definitions})
#  add_dependencies(UnitTest data
#                            vector
#                            work_item
#                            atomic
#                            vector_data
#                            relational
#                            geometric
#                            algorithm
#                            math
#                            matrix
#                            rng
#                            experiment)
#  setStaticAnalyzer(UnitTest)
#endfunction(buildUnitTest)


#
set(project_description "Unit tests of Zinvul")
project(UnitTest VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES CXX)


# Initialize platform info
include(${__zisc_dir__}/cmake/general.cmake)
include(${__zisc_dir__}/cmake/platform.cmake)
getPlatformFlags(platform_definitions)
setVariablesOnCMake(${platform_definitions})

# Check dependencies
checkTarget(Zinvul)
checkTarget(gtest_main)

# Build unit tests
#file(GLOB unittest_source_files ${__test_root__}/unittest/*[.hpp|.cpp])
set(unittest_source_files ${PROJECT_SOURCE_DIR}/unittest/unittest.cpp
                          ${PROJECT_SOURCE_DIR}/unittest/zinvul_test.cpp)
source_group(${PROJECT_NAME} FILES ${unittest_source_files})
add_executable(${PROJECT_NAME} ${unittest_source_files})

#
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PROJECT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})

include(${__zisc_dir__}/cmake/compiler.cmake)
getCxxWarningFlags(cxx_compile_warning_flags)
getTestCompileFlags(test_compile_flags test_linker_flags test_definitions)
getTestWarningFlags(test_warning_flags)
target_compile_options(${PROJECT_NAME} PRIVATE ${test_compile_flags}
                                               ${cxx_compile_warning_flags}
                                               ${test_warning_flags})
target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main
                                              Zinvul
                                              ${test_linker_flags})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${test_definitions})

#
setStaticAnalyzer(${PROJECT_NAME})

# Save the dependency list
savePrerequisites(${PROJECT_NAME} ${PROJECT_BINARY_DIR} "" "" "")
