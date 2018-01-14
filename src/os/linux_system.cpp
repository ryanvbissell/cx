// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include "cx-os.hpp"
#include "cx-exceptions.hpp"

#include <sys/utsname.h>

using std::string;

namespace CX {
namespace OS {


CX_CONSTRUCTOR(System_Linux::System_Linux, :SystemBase<System_Linux>())

CX_ENDMETHOD



CX_CONSTMETHOD(string System_Linux::_getHardwarePlatform)

  utsname uts;

  if (uname(&uts))
    CX_THROW( Exception,
              Error::ERRNO,
              "Unable to obtain this host's hardware platform." );

  CX_RETURN(string(uts.machine));

CX_ENDMETHOD



} // namespace OS
} // namespace CX

