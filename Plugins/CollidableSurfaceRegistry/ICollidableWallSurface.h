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
#ifndef I_COLLIDABLE_WALL_SURFACE_H
#define I_COLLIDABLE_WALL_SURFACE_H

#include "ICollisionData.h"

class ICollidableWallSurface {
  public:
  enum WallFaceDirection {
    FACE_NORTH,
    FACE_SOUTH,
    FACE_EAST,
    FACE_WEST
  };
  
  virtual WallFaceDirection getWallFaceDirection() = 0;

  /**
   * Determine whether an impact event occurs.
   * 
   * @param Vertex&  Starting point of movement along the surface.
   * @param Vertex&  Ending point of movement along the surface.
   * @returns  An event if one is generated, otherwise NULL.
   */
  virtual ICollisionData* getCollision(Vertex&, Vertex&) = 0;  
};

#endif
