# file: CMakeLists.txt
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

cmake_minimum_required(VERSION 3.16)


#
set(project_description "Kernel baker.")
project(KernelBaker VERSION 0.0.1 DESCRIPTION "${project_description}" LANGUAGES CXX)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

set(dependencies_dir ${PROJECT_SOURCE_DIR}/../../dependencies)
set(zisc_path ${dependencies_dir}/zisc/source/zisc)

include(${zisc_path}/cmake/general.cmake)
include(${zisc_path}/cmake/platform.cmake)
getPlatformFlags(platform_definitions)
setVariablesOnCMake(${platform_definitions})

set(source_files ${PROJECT_SOURCE_DIR}/kernel_baker.cpp)
add_executable(${PROJECT_NAME} ${source_files})
source_group(${PROJECT_NAME} FILES ${source_files})

# Compiler flags
include(${zisc_path}/cmake/compiler.cmake)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17
                                      CXX_STANDARD_REQUIRED ON)
getCxxWarningFlags(cxx_compile_warning_flags)
target_compile_options(${PROJECT_NAME} PRIVATE ${cxx_compile_warning_flags})
target_link_libraries(${PROJECT_NAME} PRIVATE Zisc Zinvul libzstd_static)

#
setStaticAnalyzer(${PROJECT_NAME})
