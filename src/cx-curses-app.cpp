// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "curses"

#include "cx-curses-app.hpp"

using CX::Curses::Panel;


CX_CONSTRUCTOR(CX::Curses::App::App, :Panel(0,0,0,0))
CX_ENDMETHOD



CX_DESTRUCTOR(CX::Curses::App::~App)
CX_ENDMETHOD



CX_METHOD(int CX::Curses::App::operator())

  // TODO: what about argv[0] ?
  CX_RETURN(operator()(0, nullptr));

CX_ENDMETHOD



CX_METHOD(int CX::Curses::App::operator(), int argc, char const** argv)

  handleArgs(argc, argv);

  noecho();
  cbreak();

  while (1)
  {
    int event = wgetch(*getKeyFocus());
    Event(event);
  }

  CX_RETURN(0);

CX_ENDMETHOD


CX_METHOD(void CX::Curses::App::handleArgs, int argc, char const** argv)
CX_ENDMETHOD


CX_METHOD(void CX::Curses::App::keyEvent, int key)

  if (_tempfocus)
    _tempfocus->Event(key);
  else
    _taborder.at(_keyfocus)->Event(key);

CX_ENDMETHOD



CX_METHOD(void CX::Curses::App::setTabOrder, U32 numpanels, ...)

  va_list panels;
  va_start(panels, numpanels);

  _taborder.clear();
  while (numpanels--)
    _taborder.push_back(va_arg(panels, Panel*));

  _keyfocus = 0;

CX_ENDMETHOD



CX_METHOD(void CX::Curses::App::setTempFocus, Panel* temp)

  _tempfocus = temp;

CX_ENDMETHOD



CX_METHOD(void CX::Curses::App::clearTempFocus)

  setTempFocus(nullptr);

CX_ENDMETHOD



CX_METHOD(Panel *CX::Curses::App::getKeyFocus)

  if (!_taborder.size())
    if (!_tempfocus)
      CX_RETURN(this);
    else
      CX_RETURN(_tempfocus);

  CX_RETURN(_taborder.at(_keyfocus));

CX_ENDMETHOD

