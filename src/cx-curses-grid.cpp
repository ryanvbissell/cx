// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "curses"

#include "cx-curses-grid.hpp"

#include <cstdarg>

using CX::Curses::Tile;



CX_CONSTRUCTOR5(CX::Curses::Tile::Tile, : self(p), _y(y), _x(x),
                                          _rows(rows), _cols(cols),
                                                Panel *p,
                                                int rows, int cols,
                                                int y, int x)
  _me = 0;
  _minRows = 0;
  _minCols = 0;
  _up = _down = _left = _right = 0;

CX_ENDMETHOD



CX_DESTRUCTOR(CX::Curses::Tile::~Tile)
CX_ENDMETHOD



CX_METHOD(void CX::Curses::Tile::SetMinRows, U32 r)

  _minRows = r;
  _rows = CX_MAX(_rows, _minRows);

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Tile::SetMinCols, U32 c)

  _minCols = c;
  _cols = CX_MAX(_cols, _minCols);

CX_ENDMETHOD



// TODO: I think maybe this would be better in Grid
CX_METHOD(Tile * CX::Curses::Tile::SplitUp, Grid* grid, Panel *p)

  int uy, ux, ur, uc;

  // I am splitting, and I will become the bottom.
  ux = _x;
  uc = _cols;
  ur = (_rows -1) / 2; // 50% split, less 1 hdiv
  _rows = _rows - ur - 1;
  uy = _y;
  _y = uy + ur + 1;

  Tile *up = grid->_createTile(p, ur, uc, uy, ux);
  setUp(up);
  up->setDown(this);

  // update the underlying panels to match
  // (must resize before move, to avoid ERR from move_panel)
  self->Resize(_rows, _cols);
  self->Move(_y, _x);
  up->self->Resize(ur, uc);
  up->self->Move(uy, ux);

  CX_RETURN(up);

CX_ENDMETHOD



// TODO: I think maybe this would be better in Grid
CX_METHOD(Tile * CX::Curses::Tile::SplitDown, Grid *grid, Panel *p)

  int dy, dx, dr, dc;

  // I am splitting, and I will become the top.
  dx = _x;
  dc = _cols;
  dr = (_rows -1) / 2; // 50% split, less 1 hdiv
  _rows = _rows - dr - 1;
  dy = _y + _rows + 1;

  Tile *down = grid->_createTile(p, dr, dc, dy, dx);
  setDown(down);
  down->setUp(this);

  // update the underlying panels to match
  // (must resize before move, to avoid ERR from move_panel)
  self->Resize(_rows, _cols);
  self->Move(_y, _x);
  down->self->Resize(dr, dc);
  down->self->Move(dy, dx);

  CX_RETURN(down);

CX_ENDMETHOD



// TODO: I think maybe this would be better in Grid
CX_METHOD(Tile * CX::Curses::Tile::SplitLeft, Grid *grid, Panel *p)

  int ly, lx, lr, lc;

  // I am splitting, and I will become the right.
  ly = _y;
  lr = _rows;
  lc = (_cols-1) / 2; // 50% split, less 1 vdiv
  _cols = _cols - lc - 1;
  lx = _x + _cols + 1;

  Tile *left = grid->_createTile(p, lr, lc, ly, lx);
  setLeft(left);
  left->setRight(this);

  // update the underlying panels to match
  // (must resize before move, to avoid ERR from move_panel)
  self->Resize(_rows, _cols);
  self->Move(_y, _x);
  left->self->Resize(lr, lc);
  left->self->Move(ly, lx);

  CX_RETURN(left);

CX_ENDMETHOD


// TODO: I think maybe this would be better in Grid
CX_METHOD(Tile * CX::Curses::Tile::SplitRight, Grid *grid, Panel *p)

  int ry, rx, rr, rc;

  // I am splitting, and I will become the left.
  ry = _y;
  rr = _rows;
  rc = (_cols-1) / 2; // 50% split, less 1 vdiv
  _cols = _cols - rc - 1;
  rx = _x + _cols + 1;

  Tile *right = grid->_createTile(p, rr, rc, ry, rx);
  setRight(right);
  right->setLeft(this);

  // update the underlying panels to match
  // (must resize before move, to avoid ERR from move_panel)
  self->Resize(_rows, _cols);
  self->Move(_y, _x);
  right->self->Resize(rr, rc);
  right->self->Move(ry, rx);

  CX_RETURN(right);

CX_ENDMETHOD



CX_FUNCTION(void CX::Curses::Tile::setNeighbor, U32 *which,
                                                Tile *that,
                                                char const *name)

  if (*which)
    CX_THROW( CX::Curses::Exception,
              CX::Curses::Error::NONE,
              "Attempted to call Set%s() twice on a tile.", name );

  *which = that->_me;

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Tile::setUp, Tile *up)

  setNeighbor(&_up, up, "Up");

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Tile::setDown, Tile *down)

  setNeighbor(&_down, down, "Down");

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Tile::setLeft, Tile *left)

  setNeighbor(&_left, left, "Left");

CX_ENDMETHOD



CX_METHOD(void CX::Curses::Tile::setRight, Tile *right)

  setNeighbor(&_right, right, "Right");

CX_ENDMETHOD



// ====================================================================


CX_CONSTRUCTOR(CX::Curses::Grid::Grid, :Panel(rows, cols, y, x),
                                                    int rows, int cols,
                                                    int y,    int x)
CX_ENDMETHOD



CX_CONSTRUCTOR(CX::Curses::Grid::Grid, :Grid(0,0,0,0))
CX_ENDMETHOD



CX_DESTRUCTOR(CX::Curses::Grid::~Grid)
CX_ENDMETHOD



CX_METHOD(void CX::Curses::Grid::Define,  Panel *first,
                                          char const* layout,
                                          ...)

  // "r0d0l2" <-- 1=right-of-0, 2=down-of-0, 3=left-of-2
  // 'first' is always 0
  // cannot refer to a panel before it has been added to _panels

  _tiles.clear();
  Tile *tile = _createTile(first, Rows(), Cols(), 0, 0);

  va_list rest;
  va_start(rest, layout);

  // TODO: catch block, free all tiles before re-throwing
  // TODO: error checking
  // TODO: ratios
  char const* l = layout;
  while (*l)
  {
    Panel *p = va_arg(rest, Panel*);
    char dir = *(l++);
    char num = *(l++) - '0';
    Tile *nth = _tiles.at(num);
    switch (dir)
    {
      case 'u':
        tile = nth->SplitUp(this, p);
        break;

      case 'd':
        tile = nth->SplitDown(this, p);
        break;

      case 'l':
        tile = nth->SplitLeft(this, p);
        break;

      case 'r':
        tile = nth->SplitRight(this, p);
        break;

      default:
        CX_ASSERT(true);
    }
  }

  va_end(rest);

CX_ENDMETHOD


struct Muntin
{
  int y, x;
  int length;
  bool vertical;

  Muntin(int Y, int X, int len, bool vert) : y(Y), x(X),
                                            length(len),
                                            vertical(vert) {};
};


CX_METHOD(void CX::Curses::Grid::draw)

  // "muntins" are the strips of wood or metal between
  // the individual panes of glass in a sash window
  std::vector<Muntin> muntins;

  // Discover all the internal muntins of this grid.
  // Earlier entries within _tiles are considered to
  // be older than later entries. We use this age
  // relationship to avoid instantiating duplicates.
  for (auto tile : _tiles)
  {
    // do I have an up-neighbor with a shorter edge?
    if (tile->_up)
    {
      Tile *up = _tiles.at(tile->_up-1);
      if ( (up->_cols < tile->_cols) || // tie goes to the youngest
          ((up->_cols == tile->_cols) && tile->_me < tile->_up) )
        muntins.push_back(Muntin( tile->_y-1,
                                  CX_MAX(0, tile->_x-1),
                                  CX_MIN(Cols(), tile->_cols+2),
                                  false ));
    }

    // do I have a down-neighbor with a shorter edge?
    if (tile->_down)
    {
      Tile *down = _tiles.at(tile->_down-1);
      if ( (down->_cols < tile->_cols) || // tie goes to the youngest
          ((down->_cols == tile->_cols) && tile->_me < tile->_down) )
        muntins.push_back(Muntin( tile->_y+tile->_rows,
                                  CX_MAX(0, tile->_x-1),
                                  CX_MIN(Cols(), tile->_cols+2),
                                  false ));
    }

    // do I have a left-neighbor with a shorter edge?
    if (tile->_left)
    {
      Tile *left = _tiles.at(tile->_left-1);
      if ( (left->_rows < tile->_rows) || // tie goes to the youngest
          ((left->_rows == tile->_rows) && tile->_me < tile->_left) )
        muntins.push_back(Muntin( CX_MAX(0, tile->_y-1),
                                  tile->_x-1,
                                  CX_MIN(Rows(), tile->_rows+2),
                                  true ));
    }

    // do I have a right-neighbor with a shorter edge?
    if (tile->_right)
    {
      Tile *right = _tiles.at(tile->_right-1);
      if ( (right->_rows < tile->_rows) || // tie goes to the youngest
          ((right->_rows == tile->_rows) && tile->_me < tile->_right) )
        muntins.push_back(Muntin( CX_MAX(0, tile->_y-1),
                                  tile->_x+tile->_cols,
                                  CX_MIN(Rows(), tile->_rows+2),
                                  true ));
    }
  }

  // the muntin count should be minimized, due to the rules above
  for (auto m : muntins)
  {
    if (m.vertical)
      mvwvline(*this, m.y, m.x, '|', m.length);
    else
      mvwhline(*this, m.y, m.x, '-', m.length);

    update_panels();
  }

  doupdate();

  // TODO: calculate colors
  // TODO: calculate intersections
  // TODO: use line-drawing characters

CX_ENDMETHOD



CX_METHOD(Tile *CX::Curses::Grid::_createTile,  Panel *p,
    int rows, int cols,
    int y,    int x)

  Tile *tile = new Tile(p, rows, cols, y, x);
  _tiles.push_back(tile);
  tile->_me = _tiles.size();

  CX_RETURN(tile);

CX_ENDMETHOD


