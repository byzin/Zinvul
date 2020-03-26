# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.16)


function(initZinvulOptions)
  set(option_description "Bake Vulkan kernels into a binary.")
  setBooleanOption(ZINVUL_BAKE_KERNELS OFF ${option_description})

  set(option_description "Enable Vulkan sub-platform.")
  setBooleanOption(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM ON ${option_description})

  set(option_description "Load Vulkan library dynamically at runtime.")
  setBooleanOption(ZINVUL_DYNAMIC_VULKAN_LOADING OFF ${option_description})

  set(option_description "Enable Zinvul to build VMA implementation.")
  setBooleanOption(ZINVUL_ENABLE_VMA_IMPLEMENTATION ON ${option_description})

  set(option_description "Enable SPIR-V analysis.")
  setBooleanOption(ZINVUL_ENABLE_SPIRV_ANALYSIS ON ${option_description})

  set(option_description "Use built-in math funcs instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN OFF ${option_description})

  set(option_description "Use built-in 'frexp' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_FREXP OFF ${option_description})

  set(option_description "Use built-in 'ldexp' instead of the Zinvul funcs.")
  if(Z_MAC)
    setBooleanOption(ZINVUL_MATH_BUILTIN_LDEXP OFF ${option_description})
  else()
    setBooleanOption(ZINVUL_MATH_BUILTIN_LDEXP ON ${option_description})
  endif()

  set(option_description "Use built-in 'ilogb' instead of the Zinvul func.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_ILOGB OFF ${option_description})

  set(option_description "Use built-in 'modf' instead of the Zinvul func.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_MODF OFF ${option_description})

  set(option_description "Use built-in 'round' and 'rint' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_ROUND OFF ${option_description})

  set(option_description "Use built-in 'fmod' instead of the Zinvul func.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_FMOD ON ${option_description})

  set(option_description "Use built-in 'exp' and 'exp2' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_EXP ON ${option_description})

  set(option_description "Use built-in 'log' and 'log2' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_LOG ON ${option_description})

  set(option_description "Use built-in 'pow' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_POW ON ${option_description})

  set(option_description "Use built-in 'sqrt' and 'rsqrt' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_SQRT ON ${option_description})

  set(option_description "Use built-in 'cbrt' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_CBRT OFF ${option_description})

  set(option_description "Use built-in 'sin', 'cos' and 'tan' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_TRIGONOMETRIC ON ${option_description})

  set(option_description "Use built-in 'asin', 'acos' and 'atan' instead of the Zinvul funcs.")
  setBooleanOption(ZINVUL_MATH_BUILTIN_INV_TRIGONOMETRIC ON ${option_description})
endfunction(initZinvulOptions)


function(getZinvulFlags zinvul_compile_flags zinvul_linker_flags zinvul_definitions)
  include(${PROJECT_SOURCE_DIR}/cmake/kernel.cmake)
  getZinvulKernelFlags(compile_flags definitions)

  if(ZINVUL_BAKE_KERNELS)
    list(APPEND definitions ZINVUL_BAKE_KERNELS)
  endif()
  if(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
    list(APPEND definitions ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)

    # Dynamic vulkan loading
    if(ZINVUL_DYNAMIC_VULKAN_LOADING)
      list(APPEND definitions ZINVUL_DYNAMIC_VULKAN_LOADING)
    endif()
    # Vulkan Memory Allocator
    if(ZINVUL_ENABLE_VMA_IMPLEMENTATION)
      list(APPEND definitions ZINVUL_ENABLE_VMA_IMPLEMENTATION)
    endif()
  endif()

  # C++
  list(APPEND definitions ZINVUL_CPU)

  # Output variables
  set(${zinvul_compile_flags} ${compile_flags} PARENT_SCOPE)
  set(${zinvul_definitions} ${definitions} PARENT_SCOPE)
endfunction(getZinvulFlags)


function(getZinvulWarningFlags zinvul_warning_flags)
  set(warning_flags "")

  # Suppress warnings
  if(Z_CLANG)
    list(APPEND warning_flags -Wno-float-equal
                              -Wno-padded
                              -Wno-sign-conversion
                              )
  elseif(Z_GCC)
    list(APPEND warning_flags -Wno-sign-conversion
                              )
  endif()

  set(${zinvul_warning_flags} ${warning_flags} PARENT_SCOPE)
endfunction(getZinvulWarningFlags)


#
set(project_description "Zinvul.")
project(Zinvul VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES CXX)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

set(dependencies_dir ${PROJECT_SOURCE_DIR}/../dependencies)
set(zisc_path ${dependencies_dir}/zisc/source/zisc)
set(cpu_features_path ${dependencies_dir}/cpu_features)

include(${zisc_path}/cmake/general.cmake)
include(${zisc_path}/cmake/platform.cmake)
getPlatformFlags(platform_definitions)
setVariablesOnCMake(${platform_definitions})
initZinvulOptions()

# Check dependencies
add_subdirectory(${zisc_path} ${PROJECT_BINARY_DIR}/Zisc)
add_subdirectory(${cpu_features_path} ${PROJECT_BINARY_DIR}/CpuFeatures)

# Make zinvul configuration header file
set(zinvul_config_dir ${PROJECT_BINARY_DIR}/include/zinvul)
file(MAKE_DIRECTORY ${zinvul_config_dir})
configure_file(${PROJECT_SOURCE_DIR}/zinvul/zinvul_config-inl.hpp.in
               ${zinvul_config_dir}/zinvul_config-inl.hpp
               @ONLY)

# Create Zinvul library
file(GLOB_RECURSE zinvul_source_files ${PROJECT_SOURCE_DIR}/zinvul/*[.hpp|.cpp])
if(NOT ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
  foreach(source_file IN LISTS zinvul_source_files)
    if(source_file MATCHES "${PROJECT_SOURCE_DIR}/zinvul/vulkan/.*")
      list(REMOVE_ITEM zinvul_source_files ${source_file})
    endif()
  endforeach(source_file)
endif()
list(APPEND zinvul_source_files ${zinvul_config_dir}/zinvul_config-inl.hpp)
source_group(${PROJECT_NAME} FILES ${zinvul_source_files})
add_library(${PROJECT_NAME} STATIC ${zinvul_source_files})

# Compiler flags
include(${zisc_path}/cmake/compiler.cmake)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17
                                                 CXX_STANDARD_REQUIRED ON)

getCxxWarningFlags(cxx_compile_warning_flags)
getZinvulFlags(zinvul_compile_flags zinvul_linker_flags zinvul_definitions)
getZinvulWarningFlags(zinvul_warning_flags)
target_compile_options(${PROJECT_NAME} PUBLIC ${zinvul_compile_flags}
                                       PRIVATE ${cxx_compile_warning_flags}
                                               ${zinvul_warning_flags})
target_link_libraries(${PROJECT_NAME} PUBLIC Zisc cpu_features)
target_link_options(${PROJECT_NAME} PUBLIC ${zinvul_linker_flags})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${zinvul_definitions})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}
                                                  ${PROJECT_BINARY_DIR}/include)

if(ZINVUL_ENABLE_VULKAN_SUB_PLATFORM)
  # Find dependencies
  # Vulkan
  find_package(Vulkan REQUIRED)
  target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)
  # Vulkan Memory Allocator
  set(vma_path ${dependencies_dir}/VulkanMemoryAllocator)
  checkSubmodule(${vma_path})
  set(vma_include_dir ${PROJECT_BINARY_DIR}/VulkanMemoryAllocator)
  file(MAKE_DIRECTORY ${vma_include_dir})
  file(COPY ${vma_path}/src/vk_mem_alloc.h DESTINATION ${vma_include_dir})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC  ${vma_include_dir})
endif()

#
setStaticAnalyzer(${PROJECT_NAME})
