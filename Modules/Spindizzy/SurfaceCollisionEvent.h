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
#ifndef SURFACE_COLLISION_EVENT_H
#define SURFACE_COLLISION_EVENT_H

#include <IsoRealms/Resources/SurfaceRegistry/ICollisionData.h>

class SurfaceCollisionEvent:public ICollisionData {
  private:
  ICollisionData::CollisionType cType;
  IRollableSurface* cSurface;
  ICollidableWallSurface* cWallSurface;
  Vertex* cLocation;
  float cXSlope;
  float cYSlope;
  float cGradient;
  
  public:
  SurfaceCollisionEvent(IRollableSurface*, ICollisionData::CollisionType, Vertex*, float, float, float);
  // TODO: Probably should be split to separate class for wall collisions
  SurfaceCollisionEvent(ICollidableWallSurface*, ICollisionData::CollisionType, Vertex*, float);
    
  IRollableSurface* getSurface();
  ICollidableWallSurface* getWallSurface();
  ICollisionData::CollisionType getType();
  Vertex* getEventLocation();
  float getXSlope();
  float getYSlope();
  float getGradient();
};

#endif
