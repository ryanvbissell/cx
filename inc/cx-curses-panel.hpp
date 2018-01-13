// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_CURSES_PANEL_HPP
#define CX_CURSES_PANEL_HPP

#include "cx-curses-window.hpp"

namespace CX {
namespace Curses {

  class Panel: public virtual Window
  {
  public:
    Panel();
    Panel(int rows, int cols, int y, int x);
    virtual ~Panel();

  protected:
    virtual void move(int y, int x) override;
    virtual void resize(int y, int x) override;

  private:
    PANEL *_ncpanel;
  };

} // namespace 'Curses'
} // namespace 'CX'

#endif // CX_CURSES_PANEL_HPP
