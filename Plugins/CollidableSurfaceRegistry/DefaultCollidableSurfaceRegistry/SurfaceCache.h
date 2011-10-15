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
#ifndef SURFACE_CACHE_H
#define SURFACE_CACHE_H

#include <vector>

#include "../IRollableSurface.h"
#include "../ICollidableWallSurface.h"

class SurfaceCache {
  private:
  std::vector<IRollableSurface*> cRollableSurfaces;
  std::vector<IRollableSurface*> cInterceptingSurfaces;
  std::vector<ICollidableWallSurface*> cNorthWallSurfaces;
  std::vector<ICollidableWallSurface*> cSouthWallSurfaces;
  std::vector<ICollidableWallSurface*> cWestWallSurfaces;
  std::vector<ICollidableWallSurface*> cEastWallSurfaces;

  public:
  void addRollableSurface(IRollableSurface*, bool);
  void addWallSurface(ICollidableWallSurface*);
  ICollisionData* getNextEvent(Vertex&, Vertex&, IRollableSurface*);
  IRollableSurface* getSurfaceAt(Vertex&);
};

#endif
