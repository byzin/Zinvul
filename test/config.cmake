# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__test_root__ ${CMAKE_CURRENT_LIST_DIR})


function(getTestWarningOption test_warning_flags)
  set(warning_flags "")

  if(ZINVUL_SUPPRESS_EXCESSIVE_WARNING)
    if(Z_CLANG)
      list(APPEND warning_flags -Wno-covered-switch-default
                                -Wno-exit-time-destructors
                                -Wno-float-equal
                                -Wno-global-constructors
                                -Wno-old-style-cast
                                -Wno-padded
                                -Wno-weak-vtables
                                )
    elseif(Z_GCC)
      list(APPEND warning_flags -Wno-restrict
                                -Wno-sign-conversion
                                -Wno-strict-overflow
                                -Wno-unused-but-set-variable
                                )
    elseif(Z_MSVC)
      list(APPEND warning_flags /wd4996
                                )
    endif()
  endif()

  # Output variables
  set(${test_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getTestWarningOption)

function(buildUnitTest)
  # Load GoogleTest
  include(${zisc_path}/cmake/googletest.cmake)
  set(gtest_project_root ${__test_root__}/googletest)
  checkSubmodule(${gtest_project_root})
  buildGoogleTest(${gtest_project_root} gtest_include_dir gtest_libraries)

  # Test kernels
  # Data
  set(data_dir ${__test_root__}/kernels/data)
  file(GLOB_RECURSE data_cl_files ${data_dir}/*.cl)
  makeKernelGroup(data data_source_files data_definitions
      SOURCE_FILES ${data_cl_files} INCLUDE_DIRS ${data_dir})
  # Built-in functions
  set(built_in_func_dir ${__test_root__}/kernels/built_in_func)
  file(GLOB_RECURSE built_in_func_cl_files ${built_in_func_dir}/*.cl)
  makeKernelGroup(built_in_func built_in_func_source_files built_in_func_definitions
      SOURCE_FILES ${built_in_func_cl_files} INCLUDE_DIRS ${built_in_func_dir})
  # Math
  set(math_dir ${__test_root__}/kernels/math)
  file(GLOB_RECURSE math_cl_files ${math_dir}/*.cl)
  makeKernelGroup(math math_source_files math_definitions
      SOURCE_FILES ${math_cl_files} INCLUDE_DIRS ${math_dir})

  # Build unit tests
  file(GLOB unittest_source_files ${__test_root__}/unittest/*.hpp
                                  ${__test_root__}/unittest/*.cpp)
  add_executable(UnitTest ${unittest_source_files}
                          ${zisc_header_files}
                          ${zinvul_source_files}
                          ${data_source_files}
                          ${built_in_func_source_files}
                          ${math_source_files})
  source_group(UnitTest FILES ${unittest_source_files})
  # Set unittest properties
  set_target_properties(UnitTest PROPERTIES CXX_STANDARD 17
                                            CXX_EXTENSIONS ON
                                            CXX_STANDARD_REQUIRED ON)
  getCxxWarningOption(cxx_warning_flags)
  getTestWarningOption(test_warning_flags)
  target_compile_options(UnitTest PRIVATE ${cxx_compiler_flags}
                                          ${zisc_compile_flags}
                                          ${zinvul_compile_flags}
                                          ${cxx_warning_flags}
                                          ${test_warning_flags})
  target_include_directories(UnitTest PRIVATE ${PROJECT_SOURCE_DIR}/source
                                              ${PROJECT_BINARY_DIR}/include
                                              ${zisc_include_dirs}
                                              ${zinvul_include_dirs})
  target_include_directories(UnitTest SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS}
                                                     ${vma_include_dir}
                                                     ${gtest_include_dir})
  target_link_libraries(UnitTest PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                                         ${Vulkan_LIBRARIES}
                                         ${cxx_linker_flags}
                                         ${zisc_linker_flags}
                                         ${zinvul_linker_flags}
                                         ${gtest_libraries})
  target_compile_definitions(UnitTest PRIVATE ${cxx_definitions}
                                              ${zisc_definitions}
                                              ${zinvul_definitions}
                                              ${environment_definitions}
                                              ${data_definitions}
                                              ${built_in_func_definitions}
                                              ${math_definitions})
  add_dependencies(UnitTest data built_in_func math)
  setStaticAnalyzer(UnitTest)
endfunction(buildUnitTest)
