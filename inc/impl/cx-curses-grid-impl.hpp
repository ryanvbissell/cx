// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_CURSES_GRID_IMPL_HPP
#define CX_CURSES_GRID_IMPL_HPP

#include "cx-curses-grid.hpp"

#include <vector>

namespace CX {
namespace Curses {

  class Grid;  // TODO: make this go away

  class Tile final
  {
  friend class Grid;
  public:
    Tile(Panel *p, int rows, int cols, int y, int x);
    ~Tile();

    void SetMinRows(U32 r);
    void SetMinCols(U32 c);

    // TODO: these could take percentage & min arguments.
    Tile* SplitUp(Grid *grid, Panel *u);
    Tile* SplitDown(Grid *grid, Panel *d);
    Tile* SplitLeft(Grid *grid, Panel *l);
    Tile* SplitRight(Grid *grid, Panel *r);


  private:
    Panel *self;
    int _y, _x;
    int _rows, _cols;
    U32 _minRows;
    U32 _minCols;

    // these are indices+1 into Grid::_tiles
    // (so, 0 means no neighbor, 1 means _tiles[0], etc.)
    U32 _me; // this Tile's index
    U32 _up, _down;
    U32 _left, _right;

    void setNeighbor(U32 *which, Tile *that, char const *name);
    void setUp(Tile *up);
    void setDown(Tile *down);
    void setLeft(Tile *left);
    void setRight(Tile *right);
  };

  typedef std::vector<Tile *> TileList;

} // namespace 'Curses'
} // namespace 'CX'

#endif // CX_CURSES_GRID_IMPL_HPP

