# vim: set ft=make:
#
# Copyright (c) 2016-2017, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#

$(call tf-declare-target,OFF)
    override CPPFLAGS:=-I$(TF_TESTROOT) -I$(TF_TESTROOT)/../inc
    $(call tf-add-sources,C++,$(TF_TESTROOT)/../src,cx-exceptions.cpp)
    $(call tf-add-sources,C++,$(TF_TESTROOT)/../src,cx-tracedebug.cpp)
    $(call tf-add-sources,C++,$(TF_TESTDIR),tracing.cpp)
    $(call tf-build-executable)

$(call tf-test-md5sum,off,96f14a8b92b362149993d0fd69e16fa8)


$(call tf-declare-target,ON)
    override CPPFLAGS+=-I$(TF_TESTROOT) -I$(TF_TESTROOT)/../inc
    override CPPFLAGS+=-DCX_OPT_DEBUGOUT=1 -DCX_OPT_TRACING=1
    $(call tf-add-sources,C++,$(TF_TESTROOT)/../src,cx-exceptions.cpp)
    $(call tf-add-sources,C++,$(TF_TESTROOT)/../src,cx-tracedebug.cpp)
    $(call tf-add-sources,C++,$(TF_TESTDIR),tracing.cpp)
    $(call tf-build-executable)


$(call tf-test-md5sum,on,29662be206d5d44a1b4b2f8099174b48)

override TF_ENVVARS:= CX_TRACE='fib'
$(call tf-test-md5sum,fib,15ffd9d8abe86aa9aa4b4bd1533e5da7)


