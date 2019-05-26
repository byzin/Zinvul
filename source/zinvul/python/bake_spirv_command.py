# file: bake_spirv_command.py

# Import system plugins
import argparse

def main():
  parser = argparse.ArgumentParser(description='Generate a baked SPIR-V file.')
  parser.add_argument('kernel_set_name',
      metavar='KernelSetName',
      help='A set name of kernels in a baked SPIR-V.')
  parser.add_argument('spirv_file_path',
      metavar='SpirVFilePath',
      help='A file path to a SPIR-V file.')
  parser.add_argument('baked_spirv_file_path',
      metavar='BakedSpirVFilePath',
      help='A file path to a baked SPIR-V file.')

  args = parser.parse_args()

  # Header
  baked_spirv_code = "/*!\n"
  baked_spirv_code += "  \\file baked_" + args.kernel_set_name + "_spirv.hpp\n"
  baked_spirv_code += "  \\author Sho Ikeda\n"
  baked_spirv_code += "  Copyright (c) 2015-2019 Sho Ikeda\n"
  baked_spirv_code += "  This software is released under the MIT License.\n"
  baked_spirv_code += "  http://opensource.org/licenses/mit-license.php\n"
  baked_spirv_code += "  */\n"
  baked_spirv_code += "\n"
  baked_spirv_code += "#ifndef ZINVUL_BAKED_" + args.kernel_set_name + "_SPIRV_HPP\n"
  baked_spirv_code += "#define ZINVUL_BAKED_" + args.kernel_set_name + "_SPIRV_HPP\n"
  baked_spirv_code += "\n"

  # Baked SPIR-V code
  with open(args.spirv_file_path, 'rb') as spirv:
    position = 0
    code_list = []
    bytes_code_format = "to_4byte(0x{:02x}u,0x{:02x}u,0x{:02x}u,0x{:02x}u),\n"
    for spirv_code in spirv.read():
      code_list.append(spirv_code)
      if len(code_list) == 4:
        baked_spirv_code += bytes_code_format.format(code_list[0],
                                                     code_list[1],
                                                     code_list[2],
                                                     code_list[3])
        code_list.clear()

  # Footer
  baked_spirv_code += "\n"
  baked_spirv_code += "#endif // ZINVUL_BAKED_" + args.kernel_set_name + "_SPIRV_HPP\n"

  with open(args.baked_spirv_file_path, 'w') as baked_spirv:
    baked_spirv.write(baked_spirv_code)

if __name__ == '__main__':
  main()
