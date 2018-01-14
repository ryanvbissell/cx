# vim: set ft=make:
#
# Copyright (c) 2016-2017, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#

$(call tf-export-makeforge-as,cxlib,$(TF_TESTROOT)/../GNUmakefile.MF)

$(call tf-declare-target,SHELL)
    # here we intentionally use := to avoid test pollution by user
    override CPPFLAGS:=-I$(TF_TESTROOT) -I${TF_TESTROOT}/../inc
    override CPPFLAGS+=-DCX_OPSYS=$(HOSTOS)
    $(call tf-static-dependencies,static@cxlib)
    $(call tf-add-sources,C++,${TF_TESTDIR},shell.cpp)
    $(call tf-build-executable)

$(call tf-test-md5sum,shell,8e83cb9e0e7337e5edbf9b611f21f289)


