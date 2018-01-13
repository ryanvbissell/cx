// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_CURSES_WINDOW_HPP
#define CX_CURSES_WINDOW_HPP

#include "cx-curses.hpp"

namespace CX {
namespace Curses {

  class Window
  {
  public:
    Window();
    Window(int rows, int cols, int y, int x);
    virtual ~Window();

    operator WINDOW *() const;

    int Y() const;
    int X() const;
    int Rows() const;
    int Cols() const;
    void Move(int y, int x);
    void Resize(int rows, int cols);

    bool HasFocus() const;
    void SetFocus(bool focused);

    void Draw();
    void Event(int e);

  protected:
    virtual int getY() const;
    virtual int getX() const;
    virtual int getRows() const;
    virtual int getCols() const;
    virtual void move(int y, int x);
    virtual void resize(int rows, int cols);
    virtual void draw();
    virtual void event(int e);
    virtual void keyEvent(int key);
    virtual void mouseEvent();
    virtual void resizeEvent();

  private:
    static U32 _ncnum;
    WINDOW *_ncwin;
    bool _focused;
  };

} // namespace 'Curses'
} // namespace 'CX'

#endif // CX_CURSES_WINDOW_HPP
