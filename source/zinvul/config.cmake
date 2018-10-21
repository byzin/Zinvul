# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__zinvul_root__ ${CMAKE_CURRENT_LIST_DIR})
set(__num_of_groups__ 0)


function(initZinvulOption)
  set(option_description "Bake vulkan kernels into a binary.")
  setBooleanOption(ZINVUL_BAKE_KERNELS OFF ${option_description})

  set(option_description "Enable vulkan backend.")
  setBooleanOption(ZINVUL_ENABLE_VULKAN_BACKEND ON ${option_description})
endfunction(initZinvulOption)


function(getZinvulOption zinvul_compile_flags zinvul_linker_flags zinvul_definitions)
  set(definitions "")

  if(ZINVUL_ENABLE_VULKAN_BACKEND)
    list(APPEND definitions ZINVUL_ENABLE_VULKAN_BACKEND)
  endif()


  # Output variables
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
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


function(listCppKernels cpp_kernel_list_code)
  set(kernel_key "(__kernel|kernel)")
  set(function_name "[a-zA-Z0-9_]+")
  set(arguments "[a-zA-Z0-9_*, ]*")
  set(kernel_pattern "${kernel_key} +void +${function_name} *\\(${arguments}\\)")

  set(kernel_list_code "")
  foreach(cl_file IN LISTS ARGN)
    file(STRINGS ${cl_file} cl_strings)
    string(REGEX MATCHALL ${kernel_pattern} kernel_list "${cl_strings}")
    foreach(cl_kernel IN LISTS kernel_list)
      string(APPEND kernel_list_code "${cl_kernel};\n\n")
    endforeach(cl_kernel)
  endforeach(cl_file)


  # Output variables
  set(${cpp_kernel_list_code} ${kernel_list_code} PARENT_SCOPE)
endfunction(listCppKernels)


macro(getCppAddressQualifiersCodeImpl address_qualifier)
  string(APPEND define_address_qualifiers_code "#ifdef ${address_qualifier}\n")
  string(APPEND define_address_qualifiers_code "static_assert(false, \"The macro '${address_qualifier}' is already defined.\");\n")
  string(APPEND define_address_qualifiers_code "#endif // ${address_qualifier}\n")
  string(APPEND define_address_qualifiers_code "#define ${address_qualifier}\n")
  string(APPEND undefine_address_qualifiers_code "#undef ${address_qualifier}\n")
endmacro(getCppAddressQualifiersCodeImpl)


function(getCppAddressQualifiersCode define_cpp_address_qualifiers_code undefine_cpp_address_qualifiers_code)

  set(define_address_qualifiers "")
  set(undefine_address_qualifiers "")
  getCppAddressQualifiersCodeImpl(__kernel)
  getCppAddressQualifiersCodeImpl(kernel)
  getCppAddressQualifiersCodeImpl(__global)
  getCppAddressQualifiersCodeImpl(global)
  getCppAddressQualifiersCodeImpl(__local)
  getCppAddressQualifiersCodeImpl(local)
  getCppAddressQualifiersCodeImpl(__constant)
  getCppAddressQualifiersCodeImpl(constant)
  getCppAddressQualifiersCodeImpl(__private)
  #  getCppAddressQualifiersCodeImpl(private)


  # Output
  set(${define_cpp_address_qualifiers_code} ${define_address_qualifiers_code} PARENT_SCOPE)
  set(${undefine_cpp_address_qualifiers_code} ${undefine_address_qualifiers_code} PARENT_SCOPE)
endfunction(getCppAddressQualifiersCode)


function(makeKernelGroup kernel_group_name zinvul_source_files zinvul_definitions)
  set(options "")
  set(one_value_args "")
  set(multi_value_args SOURCE_FILES INCLUDE_DIRS)
  cmake_parse_arguments(PARSE_ARGV 3 ZINVUL "${options}" "${one_value_args}" "${multi_value_args}")

  if(NOT ZINVUL_SOURCE_FILES)
    message(FATAL_ERROR "The kernel group '${kernel_group_name}' has no source.")
  endif()

  set(group_number ${__num_of_groups__})
  math(EXPR __num_of_groups__ "${__num_of_groups__} + 1")

  set(kernel_include_lines "")
  foreach(cl_file IN LISTS ZINVUL_SOURCE_FILES)
    string(APPEND kernel_include_lines "#include \"${cl_file}\"\n")
  endforeach(cl_file)

  # C++ backend
  listCppKernels(cpp_kernel_list_code ${ZINVUL_SOURCE_FILES})
  getCppAddressQualifiersCode(define_cpp_address_qualifiers_code undefine_cpp_address_qualifiers_code)
  set(definitions "")
  set(kernel_hpp_file ${PROJECT_BINARY_DIR}/include/zinvul/${kernel_group_name}.hpp)
  configure_file(${PROJECT_SOURCE_DIR}/source/template/kernel_group.hpp.in
                 ${kernel_hpp_file}
                 @ONLY)
  set(kernel_cpp_file ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.cpp)
  configure_file(${PROJECT_SOURCE_DIR}/source/template/kernel_group.cpp.in
                 ${kernel_cpp_file}
                 @ONLY)
  # Vulkan backend
  if(ZINVUL_ENABLE_VULKAN_BACKEND)
    find_program(clspv "clspv")
    if(clspv-NOTFOUND)
      message(FATAL_ERROR "'clspv' not found in path.") 
    endif()
    set(cl_file_path ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.cl)
    configure_file(${PROJECT_SOURCE_DIR}/source/template/kernel_group.cl.in
                   ${cl_file_path}
                   @ONLY)
    set(clspv_options "")
    if(Z_DEBUG_MODE)
      list(APPEND clspv_options -O0 -DZ_DEBUG)
    elseif(Z_RELEASE_MODE)
      list(APPEND clspv_options -O3 -DZ_RELEASE)
    endif()
    set(spv_file_path ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.spv)
    add_custom_command(OUTPUT ${spv_file_path}
      COMMAND ${clspv} ${clspv_options} -o ${spv_file_path} ${cl_file_path}
      DEPENDS ${ZINVUL_SOURCE_FILES}
      COMMENT "Building CL object ${cl_file_path}")
  endif()
  add_custom_target(${kernel_group_name} DEPENDS ${spv_file_path})

  # Output variables
  set(${zinvul_source_files} ${kernel_hpp_file} ${kernel_cpp_file} PARENT_SCOPE)
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
endfunction(makeKernelGroup)
