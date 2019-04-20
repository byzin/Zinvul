# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2019 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

set(__zinvul_root__ ${CMAKE_CURRENT_LIST_DIR})
set(__zinvul_num_of_groups__ -1 CACHE INTERNAL "")

function(initZinvulOption)
  set(option_description "Enable validation layers.")
  setBooleanOption(ZINVUL_ENABLE_VALIDATION_LAYERS ON ${option_description})

  set(option_description "Bake vulkan kernels into a binary.")
  setBooleanOption(ZINVUL_BAKE_KERNELS OFF ${option_description})

  set(option_description "Enable vulkan backend.")
  setBooleanOption(ZINVUL_ENABLE_VULKAN_BACKEND ON ${option_description})

  set(option_description "Use built-in math funcs instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN OFF ${option_description})

  set(option_description "Use built-in `select{ instead of the Zinvul func.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_SELECT OFF ${option_description})

  set(option_description "Use built-in 'sign' instead of the Zinvul func.")
  if(Z_MAC)
    setBooleanOption(ZINVUL_MATH_BUILTIN_SIGN ON ${option_description})
  else()
    setBooleanOption(ZINVUL_MATH_BUILTIN_SIGN OFF ${option_description})
  endif()

  set(option_description "Use built-in nearest integer operations instead of the Zinvul funcs.")
  if(Z_MAC)
    setBooleanOption(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER ON ${option_description})
  else()
    setBooleanOption(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER OFF ${option_description})
  endif()

  set(option_description "Use built-in 'abs' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_ABS OFF ${option_description})

  set(option_description "Use built-in 'min' and 'max' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_MINMAX OFF ${option_description})

  set(option_description "Use build-in 'mix' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_MIX OFF ${option_description})

  set(option_description "Use built-in 'clamp' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_CLAMP OFF ${option_description})

  set(option_description "Use built-in 'mod' instead of the Zinvul func.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_MOD OFF ${option_description})

  set(option_description "Use built-in 'fract' instead of the Zinvul func.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_FRACT OFF ${option_description})

  set(option_description "Use built-in 'degrees' and 'radians' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_RADIAN OFF ${option_description})

  set(option_description "Use built-in 'frexp' and 'ldexp' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_FRLDEXP OFF ${option_description})

  set(option_description "Use built-in 'clz' instead of the Zinvul func.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_CLZ OFF ${option_description})

  set(option_description "Use built-in 'popcount' instead of the Zinvul func.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_POPCOUNT OFF ${option_description})

  set(option_description "Use built-in 'exp' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_EXP OFF ${option_description})

  set(option_description "Use built-in 'log' and 'log2' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_LOG OFF ${option_description})

  set(option_description "Use built-in 'pow' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_POW OFF ${option_description})

  set(option_description "Use built-in 'sqrt' and 'rsqrt' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_SQRT OFF ${option_description})

  set(option_description "Use built-in 'cbrt' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_CBRT OFF ${option_description})

  set(option_description "Use built-in 'sin', 'cos' and 'tan' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC OFF ${option_description})

  set(option_description "Use built-in 'asin', 'acos' and 'atan' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC OFF ${option_description})

  set(option_description "Use built-in geometric functions instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_GEOMETRY OFF ${option_description})
endfunction(initZinvulOption)


function(getZinvulKernelOption zinvul_compile_flags zinvul_definitions)
  set(compile_flags "")
  set(definitions "")

  if(ZINVUL_ENABLE_VALIDATION_LAYERS)
    list(APPEND definitions ZINVUL_ENABLE_VALIDATION_LAYERS)
  endif()

  # Relational
  if(ZINVUL_MATH_BUILTIN_SELECT)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_SELECT)
  endif()
  # Math
  if(ZINVUL_MATH_BUILTIN)
    list(APPEND definitions ZINVUL_MATH_BUILTIN)
  endif()
  if(ZINVUL_MATH_BUILTIN_SIGN)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_SIGN)
  endif()
  if(ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_NEAREST_INTEGER)
  endif()
  if(ZINVUL_MATH_BUILTIN_ABS)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_ABS)
  endif()
  if(ZINVUL_MATH_BUILTIN_MINMAX)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_MINMAX)
  endif()
  if(ZINVUL_MATH_BUILTIN_MIX)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_MIX)
  endif()
  if(ZINVUL_MATH_BUILTIN_CLAMP)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_CLAMP)
  endif()
  if(ZINVUL_MATH_BUILTIN_MOD)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_MOD)
  endif()
  if(ZINVUL_MATH_BUILTIN_FRACT)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_FRACT)
  endif()
  if(ZINVUL_MATH_BUILTIN_RADIAN)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_RADIAN)
  endif()
  if(ZINVUL_MATH_BUILTIN_FRLDEXP)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_FRLDEXP)
  endif()
  if(ZINVUL_MATH_BUILTIN_CLZ)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_CLZ)
  endif()
  if(ZINVUL_MATH_BUILTIN_POPCOUNT)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_POPCOUNT)
  endif()
  if(ZINVUL_MATH_BUILTIN_EXP)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_EXP)
  endif()
  if(ZINVUL_MATH_BUILTIN_LOG)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_LOG)
  endif()
  if(ZINVUL_MATH_BUILTIN_POW)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_POW)
  endif()
  if(ZINVUL_MATH_BUILTIN_SQRT)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_SQRT)
  endif()
  if(ZINVUL_MATH_BUILTIN_CBRT)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_CBRT)
  endif()
  if(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_TRIGONOMETRIC)
  endif()
  if(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC)
  endif()
  if(ZINVUL_MATH_BUILTIN_GEOMETRY)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_GEOMETRY)
  endif()

  # Output variables
  set(${zinvul_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
endfunction(getZinvulKernelOption)


function(getZinvulOption zinvul_compile_flags zinvul_linker_flags zinvul_definitions)
  set(compile_flags "")
  set(definitions "")
  getZinvulKernelOption(compile_flags definitions)

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
  list(APPEND definitions ZINVUL_CPU)


  # Output variables
  set(${zinvul_source_files} ${source_files} PARENT_SCOPE)
  set(${zinvul_include_dirs} ${__zinvul_root__}
                             ${PROJECT_BINARY_DIR}/include PARENT_SCOPE)
  set(${zinvul_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zinvul_linker_flags} ${linker_flags} PARENT_SCOPE)
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
endfunction(loadZinvul)


macro(getCppClQualifiersCodeImpl qualifier definition)
  string(APPEND define_qualifiers_code "#ifdef ${qualifier}\n")
  string(APPEND define_qualifiers_code "static_assert(false, \"The macro '${qualifier}' is already defined.\");\n")
  string(APPEND define_qualifiers_code "#endif // ${qualifier}\n")
  string(APPEND define_qualifiers_code "#define ${qualifier} ${definition}\n")
  string(APPEND undefine_qualifiers_code "#undef ${qualifier}\n")
endmacro(getCppClQualifiersCodeImpl)


function(getCppClQualifiersCode define_cppcl_qualifiers_code undefine_cppcl_qualifiers_code)

  set(define_qualifiers "")
  set(undefine_qualifiers "")
  getCppClQualifiersCodeImpl(__kernel "")
  getCppClQualifiersCodeImpl(kernel "")

  # Output
  set(${define_cppcl_qualifiers_code} ${define_qualifiers_code} PARENT_SCOPE)
  set(${undefine_cppcl_qualifiers_code} ${undefine_qualifiers_code} PARENT_SCOPE)
endfunction(getCppClQualifiersCode)


function(makeKernelGroup kernel_group_name zinvul_source_files zinvul_definitions)
  # Parse arguments
  set(options "")
  set(one_value_args "")
  set(multi_value_args SOURCE_FILES INCLUDE_DIRS DEFINITIONS)
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
  getCppClQualifiersCode(define_cppcl_qualifiers_code undefine_cppcl_qualifiers_code)
  set(definitions "" ${ZINVUL_DEFINITIONS})
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
    # Set clspv options
    set(clspv_options -D=ZINVUL_VULKAN)
    getZinvulKernelOption(clspv_compile_flags clspv_definitions)
    foreach(compile_flag IN LISTS clspv_compile_flags)
      list(APPEND clspv_options ${compile_flag})
    endforeach(compile_flag)
    foreach(include_dir IN LISTS ZINVUL_INCLUDE_DIRS)
      list(APPEND clspv_options -I=${include_dir})
    endforeach(include_dir)
    foreach(definition IN LISTS definitions clspv_definitions)
      list(APPEND clspv_options -D=${definition})
    endforeach(definition)
    if(Z_DEBUG_MODE)
      list(APPEND clspv_options -D=Z_DEBUG_MODE)
    elseif(Z_RELEASE_MODE)
      list(APPEND clspv_options -D=Z_RELEASE_MODE)
    endif()
    if(Z_WINDOWS)
      list(APPEND clspv_options -D=Z_WINDOWS)
    elseif(Z_LINUX)
      list(APPEND clspv_options -D=Z_LINUX)
    elseif(Z_MAC)
      list(APPEND clspv_options -D=Z_MAC)
    endif()
    list(APPEND clspv_options -O=3
                              -c++
                              -cl-no-signed-zeros
                              -cost-kind=throughput
                              -cl-denorms-are-zero
                              -cl-finite-math-only
                              -expensive-combines
                              -f16bit_storage
                              -inline-entry-points
                              -instcombine-code-sinking
                              -int8
                              )
    set(clspv_commands COMMAND ${clspv} ${clspv_options}
                               -I=${__zinvul_root__}
                               -o=${spv_file_path} ${cl_file_path})
    # Descriptor map
    set(descriptor_map_path ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.csv)
    list(APPEND clspv_commands -descriptormap=${descriptor_map_path})
    # SPIRV-dis
    find_program(spirv_dis "spirv-dis")
    if(spirv_dis)
      set(dis_file_path ${PROJECT_BINARY_DIR}/zinvul/${kernel_group_name}.txt)
      set(spirv_dis_command COMMAND ${spirv_dis} ${spv_file_path} -o ${dis_file_path})
    else()
      message(WARNING "The `spirv-dis` command not found.")
    endif()
    # Bake spir-v kernels
    if(ZINVUL_BAKE_KERNELS)
      list(APPEND clspv_commands COMMAND
          ${Python3_EXECUTABLE}
          ${__zinvul_root__}/python/bake_spirv_command.py
          ${kernel_group_name}
          ${spv_file_path}
          ${baked_spv_file_path})
#      list(APPEND kernel_source_files ${baked_spv_file_path})
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
