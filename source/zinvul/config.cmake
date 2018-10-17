# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__zinvul_root__ ${CMAKE_CURRENT_LIST_DIR})


function(initZinvulOption)
  set(option_description "Bake vulkan kernels into a binary.")
  setBooleanOption(ZINVUL_BAKE_KERNELS OFF ${option_description})
endfunction(initZinvulOption)


function(getZinvulOption zinvul_compile_flags zinvul_linker_flags zinvul_definitions)
endfunction(getZinvulOption)


function(loadZinvul zinvul_header_files zinvul_include_dirs zinvul_compile_flags zinvul_linker_flags zinvul_definitions)
  set(zinvul_version_major 0)
  set(zinvul_version_minor 0)
  set(zinvul_version_patch 1)
  set(zinvul_version ${zinvul_version_major}.${zinvul_version_minor}.${zinvul_version_patch})
  message(STATUS "Zinvul version: ${zinvul_version}")

  initZinvulOption()
  getZinvulOption(compile_flags linker_flags definitions)

  # Make configuration header file
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
  set(zinvul_config_path ${PROJECT_BINARY_DIR}/include/zinvul)
  configure_file(${__zinvul_root__}/zinvul/zinvul_config.hpp.in
                 ${zinvul_config_path}/zinvul_config.hpp)

  # Set source code
  file(GLOB hpp_files ${__zinvul_root__}/zinvul/*.hpp)
  list(APPEND hpp_files ${zinvul_config_path}/zinvul_config.hpp)
  source_group(Zinvul FILES ${hpp_files})


  # Output variables
  set(${zinvul_header_files} ${hpp_files} PARENT_SCOPE)
  set(${zinvul_include_dirs} ${__zinvul_root__}
                             ${PROJECT_BINARY_DIR}/include PARENT_SCOPE)
  set(${zinvul_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zinvul_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
endfunction(loadZinvul)


function(makeKernelGroup kernel_group_name source_files include_dirs)
endfunction(makeKernelGroup)
