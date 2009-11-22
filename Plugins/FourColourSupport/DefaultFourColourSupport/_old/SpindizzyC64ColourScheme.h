/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef COLOUR_SCHEME_H
#define COLOUR_SCHEME_H

#include <iostream>

#include "Colour.h"

using namespace std;

/**
 * TODO: C64 specific?
 * A ColourScheme stores a set of four colours used for generating a texture
 * set.  Each colour has a primary purpose, but is also used in small amounts
 * in other objects (switches, arrows, trampolines, ice blocks, etc.).  In
 * addition, some colours are mixed or modified to produce new colours to be
 * used in the colour scheme.
 */
class ColourScheme {
  private:
  /** The floor colour is used primarily for floors. */
  Colour cFloor;

  /** The wall colour is used primarily for walls. */
  Colour cWall;

  /** The grid colour is used primarily for the grid. */
  Colour cGrid;

  /** The background colour is used primarily for the background. */
  Colour cBackground;

  public:
  /** Constant indicating the floor colour. */
  static const int FLOOR = 0;

  /** Constant indicating the wall colour. */
  static const int WALL = 1;

  /** Constant indicating the grid colour. */
  static const int GRID = 2;

  /** Constant indicating the background colour. */
  static const int BACKGROUND = 3;

  /**
   * Constructs a ColourScheme using a sensible set of default colours.
   */
  ColourScheme(); // TODO: Remove when not needed anymore.

  /**
   * Constructs a ColourScheme using the specified colours.
   *
   * @param Colour  Floor colour.
   * @param Colour  Wall colour.
   * @param Colour  Grid colour.
   * @param Colour  Background colour.
   */
  ColourScheme(Colour floor, Colour wall, Colour grid, Colour background);

  /**
   * Access the specified colour (using one of the constants).
   *
   * @param int  The colour to access.
   */
  Colour& operator[](int type);

  /**************************************************************************\
   * Marshalling                                                            *
  \**************************************************************************/
  /**
   * Write this colour scheme to the specified file.
   *
   * @param FILE*  The file to write the colour scheme to.
   */
  void write(FILE *);
};

#endif
