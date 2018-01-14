// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include "cx-os.hpp"
#include "cx-exceptions.hpp"


#define X(v,str) { CX::OS::Error::v, "CX::OS::Error::"#v },
std::map<CX::OS::Error, char const*> CX::OS::ErrorNameMap =
{
  CX_OS_EXCEPTIONS
};
#undef X


#define X(v,str) { CX::OS::Error::v, str },
std::map<CX::OS::Error, char const*> CX::OS::ErrorStringMap =
{
  CX_OS_EXCEPTIONS
};
#undef X


