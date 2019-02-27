# file: bake_spirv_command.py

# Import system plugins
import argparse

def main():
  parser = argparse.ArgumentParser(description='Generate a baked SPIR-V file.')
  parser.add_argument('kernel_group_name',
      metavar='KernelGroupName',
      help='A group name of kernels in a baked SPIR-V.')
  parser.add_argument('spirv_file_path',
      metavar='SpirVFilePath',
      help='A file path to a SPIR-V file.')
  parser.add_argument('baked_spirv_file_path',
      metavar='BakedSpirVFilePath',
      help='A file path to a baked SPIR-V file.')

  args = parser.parse_args()

  # Header
  baked_spirv_code = "/*!\n"
  baked_spirv_code += "  \\file baked_" + args.kernel_group_name + "_spirv.hpp\n"
  baked_spirv_code += "  \\author Sho Ikeda\n"
  baked_spirv_code += "  Copyright (c) 2015-2019 Sho Ikeda\n"
  baked_spirv_code += "  This software is released under the MIT License.\n"
  baked_spirv_code += "  http://opensource.org/licenses/mit-license.php\n"
  baked_spirv_code += "  */\n"
  baked_spirv_code += "\n"
  baked_spirv_code += "#ifndef ZINVUL_BAKED_" + args.kernel_group_name.upper() + "_SPIRV_HPP\n"
  baked_spirv_code += "#define ZINVUL_BAKED_" + args.kernel_group_name.upper() + "_SPIRV_HPP\n"
  baked_spirv_code += "\n"

  # Baked SPIR-V code
  baked_spirv_code += "zisc::pmr::vector<uint32b> spirv_code{mem_resource};\n"
  baked_spirv_code += "spirv_code = {\n"
  with open(args.spirv_file_path, 'rb') as spirv:
    byte_position = 0
    for spirv_code in spirv.read():
      baked_spirv_code += "static_cast<uint32b>(" + str(spirv_code) + "u)"
      baked_spirv_code += "<<" + str(8 * byte_position) + "u"
      byte_position = byte_position + 1
      if byte_position == 4:
        baked_spirv_code += ","
        byte_position = 0
      else:
        baked_spirv_code += "|"
  baked_spirv_code += "};\n"
  baked_spirv_code += "return spirv_code;\n"

  # Footer
  baked_spirv_code += "\n"
  baked_spirv_code += "#endif // ZINVUL_BAKED_" + args.kernel_group_name + "_SPIRV_HPP\n"

  with open(args.baked_spirv_file_path, 'w') as baked_spirv:
    baked_spirv.write(baked_spirv_code)

if __name__ == '__main__':
  main()
