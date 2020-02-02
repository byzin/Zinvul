# file: project.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 


function(attachZinvulOption target)
  checkTarget(Zisc)
  checkTarget(${target})
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  get_target_property(zisc_compile_flags Zisc INTERFACE_COMPILE_OPTIONS)
  if(zisc_compile_flags)
    target_compile_options(${target} PRIVATE ${zisc_compile_flags})
  endif()
  get_target_property(zisc_libraries Zisc INTERFACE_LINK_LIBRARIES)
  if(zisc_libraries)
    target_link_libraries(${target} PRIVATE ${zisc_libraries})
  endif()
  get_target_property(zisc_linker_flags Zisc INTERFACE_LINK_OPTIONS)
  if(zisc_linker_flags)
    target_link_options(${target} PRIVATE ${zisc_linker_flags})
  endif()
endfunction(attachZinvulOption)
