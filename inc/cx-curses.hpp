// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_CURSES_HPP
#define CX_CURSES_HPP

#include "cx-exceptions.hpp"

#include <panel.h>  // part of ncurses distribution

namespace CX {
namespace Curses {

  #define CX_CURSES_EXCEPTIONS                                        \
    X(NONE,   "Something ineffable happened.")                        \
    X(NEWWIN, "Curses 'newwin()' returned NULL.")                     \
    X(NEWPANEL, "Curses 'new_panel()' returned NULL.")                \
    X(USERPTR, "Curses 'set_panel_userptr()' returned ERR.")          \
    X(MOVE, "Unable to move window/panel.")                           \
    X(RESIZE, "Unable to resize window/panel.")

  #define X(v,str)  v,
  enum class Error: U32
  {
    CX_CURSES_EXCEPTIONS
  };
  extern std::map<Error, const char*> ErrorNameMap;
  extern std::map<Error, const char*> ErrorStringMap;
  #undef X

  CX_DECLARE_EXCEPTION_CLASS(Exception, Error, CX::Exception);

} // namespace 'Curses'
} // namespace 'CX'

#endif // CX_CURSES_HPP
