# file: bake_command.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

function(bakeSpirvKernels)
  file(READ ${SPIRV_PATH} spirv_hex_code HEX)
  string(LENGTH "${spirv_hex_code}" spirv_hex_code_size)

  set(spirv_code_array "")
  math(EXPR n "(${spirv_hex_code_size} / 8) - 1")
  foreach(index RANGE ${n})
    set(code "")
    foreach(position RANGE 3)
      math(EXPR higher_bits_index "8 * ${index} + 2 * ${position}")
      math(EXPR lower_bits_index "8 * ${index} + 2 * ${position} + 1")
      string(SUBSTRING "${spirv_hex_code}" ${higher_bits_index} 1 higher_bits)
      string(SUBSTRING "${spirv_hex_code}" ${lower_bits_index} 1 lower_bits)
      math(EXPR higher_shift "8 * ${position} + 4")
      math(EXPR lower_shift "8 * ${position}")
      string(APPEND code
          "static_cast<uint32b>(0x${lower_bits}u)<<${lower_shift}|")
      string(APPEND code
          "static_cast<uint32b>(0x${higher_bits}u)<<${higher_shift}")
      if(NOT ${position} EQUAL 3)
        string(APPEND code "|")
      endif()
    endforeach(position)
    string(APPEND spirv_code_array ${code})
    if(NOT ${index} EQUAL ${n})
      string(APPEND spirv_code_array ",")
    endif()
  endforeach(index)

  configure_file(${BAKED_SPIRV_TEMPLATE_PATH}
                 ${BAKED_SPIRV_PATH}
                 @ONLY)
endfunction(bakeSpirvKernels)

if(FILE_TYPE MATCHES "SpirV")
  bakeSpirvKernels()
endif()
