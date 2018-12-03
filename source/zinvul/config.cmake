# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__zinvul_root__ ${CMAKE_CURRENT_LIST_DIR})
set(__zinvul_num_of_groups__ -1 CACHE INTERNAL "")

function(initZinvulOption)
  set(option_description "Bake vulkan kernels into a binary.")
  setBooleanOption(ZINVUL_BAKE_KERNELS OFF ${option_description})

  set(option_description "Enable vulkan backend.")
  setBooleanOption(ZINVUL_ENABLE_VULKAN_BACKEND ON ${option_description})
endfunction(initZinvulOption)


function(getZinvulOption zinvul_compile_flags zinvul_linker_flags zinvul_definitions)
  set(definitions "")

  if(ZINVUL_BAKE_KERNELS)
    list(APPEND definitions ZINVUL_BAKE_KERNELS)
  endif()
  if(ZINVUL_ENABLE_VULKAN_BACKEND)
    list(APPEND definitions ZINVUL_ENABLE_VULKAN_BACKEND)
  endif()


  # Output variables
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
endfunction(getZinvulOption)


function(loadZinvul zinvul_source_files zinvul_include_dirs zinvul_compile_flags zinvul_linker_flags zinvul_definitions)
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
                 ${zinvul_config_path}/zinvul_config.hpp
                 @ONLY)

  # Set source code
  file(GLOB_RECURSE source_files ${__zinvul_root__}/zinvul/*.hpp
                              ${__zinvul_root__}/zinvul/*.cl)
  list(APPEND source_files ${zinvul_config_path}/zinvul_config.hpp)
  source_group(Zinvul FILES ${source_files})

  # Vulkan
  list(APPEND definitions VULKAN_HPP_TYPESAFE_CONVERSION=0
                          VULKAN_HPP_NO_SMART_HANDLE
                          VULKAN_HPP_NO_EXCEPTIONS)
  if(Z_DEBUG_MODE)
    list(APPEND definitions VMA_DEBUG_INITIALIZE_ALLOCATIONS=1
                            VMA_DEBUG_MARGIN=128
                            VMA_DEBUG_DETECT_CORRUPTION=1)
  endif()
  # OpenCL
  list(APPEND definitions ZINVUL_CPP_CL)


  # Output variables
  set(${zinvul_source_files} ${source_files} PARENT_SCOPE)
  set(${zinvul_include_dirs} ${__zinvul_root__}
                             ${PROJECT_BINARY_DIR}/include PARENT_SCOPE)
  set(${zinvul_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zinvul_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
endfunction(loadZinvul)


macro(getCppAddressQualifiersCodeImpl address_qualifier)
  string(APPEND define_address_qualifiers_code "#ifdef ${address_qualifier}\n")
  string(APPEND define_address_qualifiers_code "static_assert(false, \"The macro '${address_qualifier}' is already defined.\");\n")
  string(APPEND define_address_qualifiers_code "#endif // ${address_qualifier}\n")
  if("${address_qualifier}" MATCHES "constant")
    set(cpp_address_qualifier "static constexpr")
  else()
    set(cpp_address_qualifier "")
  endif()
  string(APPEND define_address_qualifiers_code "#define ${address_qualifier} ${cpp_address_qualifier}\n")
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
  # Parse arguments
  set(options "")
  set(one_value_args "")
  set(multi_value_args SOURCE_FILES INCLUDE_DIRS)
  cmake_parse_arguments(PARSE_ARGV 3 ZINVUL "${options}" "${one_value_args}" "${multi_value_args}")
  if(NOT ZINVUL_SOURCE_FILES)
    message(FATAL_ERROR "The kernel group '${kernel_group_name}' has no source.")
  endif()

  # Set parameters
  # Get the group number
  math(EXPR group_number "${__zinvul_num_of_groups__} + 1")

  set(kernel_include_lines "")
  foreach(cl_file IN LISTS ZINVUL_SOURCE_FILES)
    string(APPEND kernel_include_lines "#include \"${cl_file}\"\n")
  endforeach(cl_file)

  set(kernel_source_files ${ZINVUL_SOURCE_FILES})
  set(spv_file_path ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.spv)
  set(baked_spv_file_path ${PROJECT_BINARY_DIR}/zinvul/baked_${kernel_group_name}_spirv.hpp)

  # C++ backend
  getCppAddressQualifiersCode(define_cpp_address_qualifiers_code undefine_cpp_address_qualifiers_code)
  set(definitions "")
  set(kernel_hpp_file_path ${PROJECT_BINARY_DIR}/include/zinvul/${kernel_group_name}.hpp)
  configure_file(${__zinvul_root__}/template/kernel_group.hpp.in
                 ${kernel_hpp_file_path}
                 @ONLY)
  list(APPEND kernel_source_files ${kernel_hpp_file_path})
  # Vulkan backend
  if(ZINVUL_ENABLE_VULKAN_BACKEND)
    find_program(clspv "clspv")
    if(clspv-NOTFOUND)
      message(FATAL_ERROR "'clspv' not found in path.") 
    endif()
    set(cl_file_path ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.cl)
    configure_file(${__zinvul_root__}/template/kernel_group.cl.in
                   ${cl_file_path}
                   @ONLY)
    list(APPEND kernel_source_files ${cl_file_path})
    set(clspv_options "")
    if(Z_DEBUG_MODE)
      list(APPEND clspv_options -O0 -DZ_DEBUG)
    elseif(Z_RELEASE_MODE)
      message(WARNING "The clspv optimization is disabled.")
      #      list(APPEND clspv_options -O3 -DZ_RELEASE)
      list(APPEND clspv_options -O0 -DZ_RELEASE)
    endif()
    list(APPEND clspv_options -f16bit_storage)
    set(clspv_commands COMMAND ${clspv} ${clspv_options}
                               -I ${__zinvul_root__}
                               -o ${spv_file_path} ${cl_file_path})
    if(Z_DEBUG_MODE)
      set(descriptor_map_path ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.csv)
      list(APPEND clspv_commands -descriptormap=${descriptor_map_path})
      # SPIRV-dis
      find_program(spirv_dis "spirv-dis")
      if(NOT spirv_dis-NOTFOUND)
        set(dis_file_path ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.txt)
        set(spirv_dis_command COMMAND ${spirv_dis} ${spv_file_path} -o ${dis_file_path})
      endif()
    endif()
    if(ZINVUL_BAKE_KERNELS)
      list(APPEND clspv_commands COMMAND
          ${CMAKE_COMMAND}
          -DKERNEL_GROUP_NAME=${kernel_group_name}
          -DBAKED_SPIRV_TEMPLATE_PATH=${__zinvul_root__}/template/baked_spirv.hpp.in
          -DSPIRV_PATH=${spv_file_path}
          -DBAKED_SPIRV_PATH=${baked_spv_file_path}
          -DFILE_TYPE=SpirV
          -P ${__zinvul_root__}/cmake/bake_command.cmake)
      list(APPEND kernel_source_files ${baked_spv_file_path})
    endif()
    add_custom_command(OUTPUT ${spv_file_path}
      ${clspv_commands}
      ${spirv_dis_command}
      DEPENDS ${ZINVUL_SOURCE_FILES}
      COMMENT "Building CL object ${cl_file_path}")
    add_custom_target(${kernel_group_name} DEPENDS ${spv_file_path})
  else()
    add_custom_target(${kernel_group_name})
  endif()

  source_group(${kernel_group_name} FILES ${kernel_source_files})

  # Output variables
  set(${zinvul_source_files} ${kernel_source_files} PARENT_SCOPE)
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
  set(__zinvul_num_of_groups__ ${group_number} CACHE INTERNAL "")
endfunction(makeKernelGroup)
