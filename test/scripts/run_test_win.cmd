@echo off

rem author: Sho Ikeda
rem Copyright (c) 2015-2020 Sho Ikeda
rem This software is released under the MIT License.
rem http://opensource.org/licenses/mit-license.php

Release\UnitTest --gtest_filter=-*\.FrLd* 2>&1 | tee test_result.txt
