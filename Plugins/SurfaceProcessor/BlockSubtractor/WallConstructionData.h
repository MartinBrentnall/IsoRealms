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
#ifndef WALL_CONSTRUCTION_DATA_H
#define WALL_CONSTRUCTION_DATA_H

#include "../ISurfaceProvider.h"
#include "../IWallSurface.h"

#include "WallColumn.h"

class WallConstructionData {
  private:
  int cLength;
  int cBottomHeightStart;
  int cBottomHeightSlope;
  int cTopHeightStart;
  int cTopHeightSlope;

  public:
  WallConstructionData(WallColumn*);
  bool unite(WallColumn*);

  /**
   * Instruct the surface provider to create us a new wall surface based on our
   * construction data.
   * 
   * @param ISurfaceProvider*  The provider for creating the wall.
   * @param int  The X tile end location of the wall.
   * @param int  The Y tile end location of the wall.
   * @param IWallSurface::FaceDirection  The face direction of the wall, away
   *           from the specified tile.
   * @returns  The newly constructed wall surface.
   */
  IWallSurface* constructSurface(ISurfaceProvider*, int, int, IWallSurface::FaceDirection);
};

#endif
