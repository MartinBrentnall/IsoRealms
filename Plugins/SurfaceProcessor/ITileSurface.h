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
#ifndef I_TILE_SURFACE_H
#define I_TILE_SURFACE_H

#include "../../Global/BlockArea.h"

class ITileSurface {
  public:
  enum FaceDirection {
    UP,
    DOWN
  };

  /**
   * Get the base height of the specified cell in this surface.
   * 
   * @param int  X location of the cell.
   * @param int  Y location of the cell.
   * @returns  Base height of the cell.
   */
  virtual int getSurfaceCellHeight(int, int) = 0;

  /**
   * Get the elevation of the specified cell in this surface.  The elevation
   * is the difference between the base height of the cell surface and the
   * highest point of the cell surface.  Note that the value may be negative!
   * 
   * @param int  X location of the cell.
   * @param int  Y location of the cell.
   * @returns  Elevation of the cell.
   */
  virtual int getSurfaceCellElevation(int, int) = 0;

  /**
   * TODO: Could be removed if we used BlockArea instead.
   */
  virtual bool alligned(int x, int y) = 0;

  virtual BlockArea* getCoverage() = 0;
};

#endif
