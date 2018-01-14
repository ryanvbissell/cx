# vim: set ft=make:
#
# Copyright (c) 2016, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#


$(call tf-export-makeforge-as,cxlib,$(TF_TESTROOT)/../GNUmakefile.MF)


$(call tf-declare-target,PATH)
    # here we intentionally use := to avoid test pollution by user
    override CPPFLAGS:=-I$(TF_TESTROOT) -I${TF_TESTROOT}/../inc
    override CPPFLAGS+=-DCX_OPSYS=$(HOSTOS)
    $(call tf-static-dependencies,static@cxlib)
    $(call tf-add-sources,C++,${TF_TESTDIR},path.cpp)
    $(call tf-add-sources,C++,${TF_TESTDIR},testpaths.cpp)
    $(call tf-build-executable)

$(call tf-test-md5sum,path,f84d498e6388ff56ad30c0dfa5dbf36e)



$(call tf-declare-target,PATHLIST)
    # here we intentionally use := to avoid test pollution by user
    override CPPFLAGS:=-I$(TF_TESTROOT) -I${TF_TESTROOT}/../inc
    override CPPFLAGS+=-DCX_OPSYS=$(HOSTOS)
    $(call tf-static-dependencies,static@cxlib)
    $(call tf-add-sources,C++,${TF_TESTDIR},testpaths.cpp)
    $(call tf-add-sources,C++,${TF_TESTDIR},pathlist.cpp)
    $(call tf-build-executable)

$(call tf-test-md5sum,pathlist,1341ab191ae4ca16fa12097d8d341b27)

