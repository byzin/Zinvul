# file: config.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


set(__template_path__ ${CMAKE_CURRENT_LIST_DIR})

function(getZinvulKernelFlags zinvul_compile_flags zinvul_definitions)
  set(compile_flags "")
  set(definitions "")

  # Math
  if(ZINVUL_MATH_BUILTIN)
    list(APPEND definitions ZINVUL_MATH_BUILTIN)
  endif()
  if(ZINVUL_MATH_BUILTIN_FREXP)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_FREXP)
  endif()
  if(ZINVUL_MATH_BUILTIN_LDEXP)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_LDEXP)
  endif()
  if(ZINVUL_MATH_BUILTIN_ILOGB)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_ILOGB)
  endif()
  if(ZINVUL_MATH_BUILTIN_MODF)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_MODF)
  endif()
  if(ZINVUL_MATH_BUILTIN_ROUND)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_ROUND)
  endif()
  if(ZINVUL_MATH_BUILTIN_FMOD)
    list(APPEND definitions ZINVUL_MATH_BUILTIN_FMOD)
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

  # Output variables
  set(${zinvul_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
endfunction(getZinvulKernelFlags)


set(__zinvul_num_of_sets__ 0 CACHE INTERNAL "")

function(getKernelSetNumber number)
  set(kernel_set_number ${__zinvul_num_of_sets__})
  math(EXPR num_of_sets "${__zinvul_num_of_sets__} + 1")
  set(__zinvul_num_of_sets__ ${num_of_sets} CACHE INTERNAL "")

  # Output
  set(${number} ${kernel_set_number} PARENT_SCOPE)
endfunction(getKernelSetNumber)


function(addKernelSet kernel_set_name kernel_set_version)
  # Parse arguments
  set(options "")
  set(one_value_args "")
  set(multi_value_args SOURCE_FILES INCLUDE_DIRS DEFINITIONS)
  cmake_parse_arguments(PARSE_ARGV 3 ZINVUL "${options}" "${one_value_args}" "${multi_value_args}")

  # Check source files
  if(NOT ZINVUL_SOURCE_FILES)
    message(FATAL_ERROR "The kernel set '${kernel_set_name}' has no source.")
  endif()

  # Set kernel properties
  getKernelSetNumber(kernel_set_number)
  set(kernel_set_template_path ${__template_path__})
  set(kernel_set_base_dir ${CMAKE_CURRENT_LIST_DIR})
  set(kernel_set_source_files ${ZINVUL_SOURCE_FILES})
  set(kernel_set_include_dirs ${ZINVUL_INCLUDE_DIRS})
  set(kernel_set_definitions ${ZINVUL_DEFINITIONS})
endfunction(addKernelSet)
