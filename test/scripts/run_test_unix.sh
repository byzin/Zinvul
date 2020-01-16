#! /bin/sh
# author: Sho Ikeda
# Copyright (c) 2015-2020 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
# 

./UnitTest --gtest_filter=-*\.FrLd* 2>&1 | tee test_result.txt
