# vim: set ft=make:
#
# Copyright (c) 2016-2017, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#

$(call tf-declare-target,SCALARS)
    override CXXFLAGS:=-std=c++11
    override CPPFLAGS+=-I${TF_TESTROOT} -I${TF_TESTROOT}/../inc
    $(call tf-add-sources,C++,$(TF_TESTDIR),scalars.cpp)
    $(call tf-build-executable)

$(call tf-test-exitstatus,scalars)


