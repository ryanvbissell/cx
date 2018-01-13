# vim: set ft=make:
#
# Copyright (c) 2017, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#

$(call tf-declare-target,ENDIAN)
    override CPPFLAGS:=-I$(TF_TESTROOT) -I$(TF_TESTROOT)/../inc
    $(call tf-add-sources,C++,$(TF_TESTROOT)/../src,cx-endian.cpp)
    $(call tf-add-sources,C++,$(TF_TESTDIR),endian.cpp)
    $(call tf-build-executable)

$(call tf-test-exitstatus,endian)


