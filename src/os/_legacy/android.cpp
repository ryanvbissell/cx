// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include <sstream>

#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/elf.h> // for NT_PRSTATUS and friends

#include "os/android.hpp"
#include "cx-exceptions.hpp"

using CX::OS::PID;

using std::string;

#include "gnu_linux.cpp"

CX_CONSTRUCTOR(CX::OS::Path__android::Path__android,
                                            :CX::OS::Path__gnu_linux())
CX_ENDMETHOD


CX_DESTRUCTOR(CX::OS::Path__android::~Path__android)
CX_ENDMETHOD


// ====================================================================


CX_CONSTRUCTOR(CX::OS::File__android::File__android,
                                            :CX::OS::File__gnu_linux())
CX_ENDMETHOD



CX_DESTRUCTOR(CX::OS::File__android::~File__android)
CX_ENDMETHOD


// ====================================================================


CX_CONSTRUCTOR(CX::OS::Process__android::Process__android,
                                          :CX::OS::Process_gnu_linux())
CX_ENDMETHOD



CX_DESTRUCTOR(CX::OS::Process__android::~Process__android)
CX_ENDMETHOD




