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
  # Work item
  set(work_item_dir ${__test_root__}/kernels/work_item)
  file(GLOB_RECURSE work_item_cl_files ${work_item_dir}/*.cl)
  makeKernelGroup(work_item work_item_source_files work_item_definitions 
      SOURCE_FILES ${work_item_cl_files} INCLUDE_DIRS ${work_item_dir})
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
                          ${zinvul_header_files}
                          ${work_item_source_files}
                          ${math_source_files})
  source_group(UnitTest FILES ${unittest_source_files})
  # Set unittest properties
  set_target_properties(UnitTest PROPERTIES CXX_STANDARD 17
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
                                              ${work_item_definitions}
                                              ${math_definitions})
  add_dependencies(UnitTest work_item math)
  setStaticAnalyzer(UnitTest)
endfunction(buildUnitTest)
