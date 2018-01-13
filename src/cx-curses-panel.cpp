// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "curses"

#include "cx-curses-panel.hpp"



CX_CONSTRUCTOR(CX::Curses::Panel::Panel, :Window(rows, cols, y, x),
                                                   int rows, int cols,
                                                   int y,    int x)

  if (!(_ncpanel = new_panel(*this)))
  {
    CX_THROW( CX::Curses::Exception,
              CX::Curses::Error::NEWPANEL,
              "Failed to initialize _ncpanel."  );
  }

  if (OK != set_panel_userptr(_ncpanel, this))
  {
    CX_THROW( CX::Curses::Exception,
              CX::Curses::Error::USERPTR,
              "Failed to associate 'this' pointer with _ncpanel" );
  }

CX_ENDMETHOD



CX_CONSTRUCTOR(CX::Curses::Panel::Panel, :Panel(0,0,0,0))
CX_ENDMETHOD



CX_DESTRUCTOR(CX::Curses::Panel::~Panel)

  del_panel(_ncpanel);

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Panel::move, int y, int x)

  // presumably, this changes the underlying WINDOW as well
  if (OK != move_panel(_ncpanel, y, x))
  {
    CX_THROW( CX::Curses::Exception,
              CX::Curses::Error::MOVE,
              "Failed to move _ncpanel to y=%d, x=%d", y, x );
  }

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Panel::resize, int rows, int cols)

  // if this doesn't work, see
  // http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/panels.html

  Window::resize(rows, cols);

  if (OK != replace_panel(_ncpanel, *this))
  {
    CX_THROW( CX::Curses::Exception,
              CX::Curses::Error::RESIZE,
              "Call to replace_panel() failed." );
  }

CX_ENDMETHOD



