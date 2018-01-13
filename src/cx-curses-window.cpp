// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "curses"

#include "cx-curses-window.hpp"


U32 CX::Curses::Window::_ncnum = 0;  // count of ncurses windows


CX_CONSTRUCTOR(CX::Curses::Window::Window,  :_focused(false),
                                                   int rows, int cols,
                                                   int y,    int x)
  if (!_ncnum++)
    ::initscr();

  if (!(_ncwin = newwin(rows, cols, y, x)))
  {
    CX_THROW( CX::Curses::Exception,
              CX::Curses::Error::NEWWIN,
              "Failed to initialize _ncwin." );
  }

   keypad(_ncwin, TRUE);
   meta(_ncwin, TRUE);
   intrflush(_ncwin, TRUE);

CX_ENDMETHOD



CX_CONSTRUCTOR(CX::Curses::Window::Window, :Window(0,0,0,0))
CX_ENDMETHOD



CX_DESTRUCTOR(CX::Curses::Window::~Window)

  delwin(_ncwin);

  if (!(--_ncnum))
    endwin();

CX_ENDMETHOD



CX_CONSTMETHOD(CX::Curses::Window::operator WINDOW *)

  CX_RETURN(_ncwin);

CX_ENDMETHOD



CX_CONSTMETHOD(int CX::Curses::Window::Y)

  CX_RETURN(getY());

CX_ENDMETHOD



CX_CONSTMETHOD(int CX::Curses::Window::X)

  CX_RETURN(getX());

CX_ENDMETHOD



CX_CONSTMETHOD(int CX::Curses::Window::Rows)

  CX_RETURN(getRows());

CX_ENDMETHOD



CX_CONSTMETHOD(int CX::Curses::Window::Cols)

  CX_RETURN(getCols());

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::Move, int y, int x)

  move(y,x);

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::Resize, int rows, int cols)

  resize(rows, cols);

CX_ENDMETHOD



CX_CONSTMETHOD(bool CX::Curses::Window::HasFocus)

  // does this window believe it has focus?
  CX_RETURN(_focused);

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::SetFocus, bool focused)

  // this is just a drawing hint to the window
  _focused = focused;

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::Draw)

  draw();

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::Event, int e)

  event(e);

CX_ENDMETHOD



CX_CONSTMETHOD(int CX::Curses::Window::getY)

  CX_RETURN(getbegy(_ncwin));

CX_ENDMETHOD



CX_CONSTMETHOD(int CX::Curses::Window::getX)

  CX_RETURN(getbegx(_ncwin));

CX_ENDMETHOD



CX_CONSTMETHOD(int CX::Curses::Window::getRows)

  CX_RETURN(getmaxy(_ncwin));

CX_ENDMETHOD


CX_CONSTMETHOD(int CX::Curses::Window::getCols)

  CX_RETURN(getmaxx(_ncwin));

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::move, int y, int x)

  if (OK != mvwin(_ncwin, y, x))
  {
    // note: this can happen if the move causes
    // part of the window to be off-screen.
    CX_THROW( CX::Curses::Exception,
              CX::Curses::Error::MOVE,
              "Failed to move _nvwin to y=%d, x=%d", y, x );
  }

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::resize, int rows, int cols)

  if (OK != wresize(_ncwin, rows, cols))
  {
    CX_THROW( CX::Curses::Exception,
              CX::Curses::Error::RESIZE,
              "Failed to resize _ncwin to %d x %d", rows, cols );
  }

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::draw)
CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::event, int e)

  switch (e)
  {
    case KEY_MOUSE:
      mouseEvent();
      break;

    case KEY_RESIZE:
      resizeEvent();
      break;

    default:
      // anything not handled above is assumed to be keyboard-related
      keyEvent(e);
  }

CX_ENDMETHOD





CX_METHOD(void CX::Curses::Window::keyEvent, int key)
  // intentionally empty
CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::mouseEvent)
  // intentionally empty
CX_ENDMETHOD



CX_METHOD(void CX::Curses::Window::resizeEvent)
  // intentionally empty
CX_ENDMETHOD



