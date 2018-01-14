// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include "cx-os.hpp"

// this instantiates the proper OS objects
#define CX_OS_DECLARE_STATIC2(type, os)         \
        static CX::OS::type##_##os g##type;     \
        CX::OS::type##Base *type = &g##type;

#define CX_OS_DECLARE_STATIC(type, os)  CX_OS_DECLARE_STATIC2(type, os)

//CX_OS_DECLARE_STATIC(Shell, CX_OPSYS)
//CX_OS_DECLARE_STATIC(Process, CX_OPSYS)


// the one and only instantiation of CX::OS::Shell
//CX::OS::Shell g_shell;


