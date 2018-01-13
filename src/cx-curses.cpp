// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "curses"

#include "cx-curses.hpp"
#include "cx-exceptions.hpp"

#define X(v,str) { CX::Curses::Error::v, "CX::Curses::Error::"#v },
std::map<CX::Curses::Error, const char*> CX::Curses::ErrorNameMap =
{
  CX_CURSES_EXCEPTIONS
};
#undef X

#define X(v,str) { CX::Curses::Error::v, str },
std::map<CX::Curses::Error, const char*> CX::Curses::ErrorStringMap =
{
  CX_CURSES_EXCEPTIONS
};
#undef X


