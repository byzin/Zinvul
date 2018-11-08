# file: option.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2018 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


# Validate options
function(validateOptions)
endfunction(validateOptions)


# Set command options
function(initCommandOptions)
  set(option_description "Suppress excessive warnings.")
  setBooleanOption(ZINVUL_SUPPRESS_EXCESSIVE_WARNING ON ${option_description})

  validateOptions()
endfunction(initCommandOptions)
