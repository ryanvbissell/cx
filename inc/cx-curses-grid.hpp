// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_CURSES_GRID_HPP
#define CX_CURSES_GRID_HPP

#include "cx-curses-panel.hpp"
#include "impl/cx-curses-grid-impl.hpp"

namespace CX {
namespace Curses {

  class Grid: public virtual Panel
  {
  friend Tile;  // TODO, get rid of this?
  public:
    Grid();
    Grid(int rows, int cols, int y, int x);
    virtual ~Grid();

    void Define(Panel* first, char const* format, ...);

  protected:
    virtual void draw() override;

  private:
    TileList _tiles;
    Tile *_createTile(Panel *p, int rows, int cols, int y, int x);
  };

} // namespace 'Curses'
} // namespace 'CX'

#endif // CX_CURSES_GRID_HPP
