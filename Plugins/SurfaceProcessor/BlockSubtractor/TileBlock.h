/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef TILE_BLOCK_H
#define TILE_BLOCK_H

#include "../ISurfaceProvider.h"

class TileBlock {
  private:
  static int cInstanceCount;
  ISurfaceProvider* cTopSurfaceProvider;
  ISurfaceProvider* cBottomSurfaceProvider;
  int cTop;
  int cBottom;
  bool cTopExtended;
  bool cBottomExtended;

  bool isHigher(TileBlock&);
  bool isLower(TileBlock&);

  public:
  TileBlock(ISurfaceProvider*, int, int, bool, bool);
  TileBlock(const TileBlock&);

  bool isAddition();
  TileBlock* split(TileBlock&);
  bool merge(TileBlock&);
  bool subtractAsGhost(TileBlock&);
  
  ISurfaceProvider* getSurfaceProvider(ITileSurface::FaceDirection);
  
  void debug();
  
  ~TileBlock();
};

#endif
