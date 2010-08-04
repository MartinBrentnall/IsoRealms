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
#ifndef I_ROLLABLE_SURFACE_H
#define I_ROLLABLE_SURFACE_H

#include <IsoRealms/Vertex.h>

#include "ICollisionData.h"

class IRollableSurface {
  public:
  enum RespawnPossibility {
    YES,
    CONDITIONAL,
    NO
  };
    
  virtual ICollisionData* getCollision(Vertex&, Vertex&) = 0;
  virtual ICollisionData* getRollingEvent(Vertex&, Vertex&) = 0;
  virtual float getHeightAt(float, float) = 0;
  virtual bool contains(Vertex&) = 0;
  virtual float getXAcceleration(float, float) = 0;
  virtual float getYAcceleration(float, float) = 0;
  virtual void notifyContact() = 0;
  virtual void notifyImpact() = 0;
  
  /**
   * Value between 0.0f (no friction) and 1.0f (instant halt).
   */
  virtual float getSurfaceFriction() = 0;

    /**
   * Value between 0.0f (no control) and 1.0f (full control).
   */
  virtual float getSurfaceGrip() = 0;
  
  virtual float getSurfaceBounce() = 0;
  
  virtual RespawnPossibility getRespawnPossibility() = 0;
  
  virtual bool isRespawnPossibleNow() = 0;
};

#endif
