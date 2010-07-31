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
#ifndef I_COLLISION_DATA_H
#define I_COLLISION_DATA_H

#include <IsoRealms/Vertex.h>

class IRollableSurface;

class ICollisionData {
  public:
  enum CollisionType {
    SURFACE_LEAVE,
    SURFACE_MOUNT,
    WALL_IMPACT,
    WALL_CLIP
  };
  
  virtual IRollableSurface* getSurface() = 0;
  virtual CollisionType getType() = 0;
  virtual Vertex* getEventLocation() = 0;
  virtual float getGradient() = 0;
};

#endif
