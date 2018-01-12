# vim: set ft=make:
#
# Copyright (c) 2016-2017, Ryan V. Bissell
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
# See the enclosed "LICENSE" file for exact license terms.
#

define hackery_quick_test =
    $(call tf-declare-target,$1)
        $(eval override CPPFLAGS+=-I${TF_TESTROOT} -I${TF_TESTROOT}/../inc)
        $(call tf-add-sources,C++,$(TF_TESTDIR),$2.cpp)
        $(call tf-build-executable)

    $(call tf-test-exitstatus,$2)
endef


$(eval $(call hackery_quick_test,BITS,bits))
$(eval $(call hackery_quick_test,MATH,math))

