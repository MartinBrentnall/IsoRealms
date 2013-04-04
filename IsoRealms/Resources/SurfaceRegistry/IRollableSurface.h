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

#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "ICollisionData.h"

class IRollableSurface {
  public:
  enum RespawnPossibility {
    YES,
    CONDITIONAL,
    NO
  };

  /**
   * Determine whether an event occurs assuming that the specified vertices
   * represent non-rolling movement over the surface (i.e. the surface has not
   * yet been mounted by the entity represented by the movement).  The only
   * possible event type is a ICollisionData::SURFACE_MOUNTED event.
   * 
   * @param Vertex&  Starting point of movement along the surface.
   * @param Vertex&  Ending point of movement along the surface.
   * @returns  An event if one is generated, otherwise NULL.
   */
  virtual ICollisionData* getCollision(Vertex&, Vertex&) = 0;
  
  /**
   * Determine whether an event occurs assuming that the specified vertices
   * represent a rolling movement over the surface (height is ignored).  The
   * only possible event type is a ICollisionData::SURFACE_LEFT event.  The
   * location of the event will be such that contains() returns false for this
   * location.
   * 
   * @param Vertex&  Starting point of movement along the surface.
   * @param Vertex&  Ending point of movement along the surface.
   * @returns  An event if one is generated, otherwise NULL.
   */
  virtual ICollisionData* getRollingEvent(Vertex&, Vertex&) = 0;
  
  /**
   * Return the height (Z) of this surface at the specified X,Y location, such
   * that a call to contains() using all three values in a Vertex will return
   * true.
   * 
   * @param float  X location.
   * @param float  Y location.
   * @returns  Height (z) of this surface at X,Y location.
   */
  virtual float getHeightAt(float, float) = 0;

  /**
   * Determine whether the specified Vertex is on this surface.
   * 
   * @param Vertex&  The vertex to test.
   * @returns true if the vertex is on this surface, otherwise false.
   */
  virtual bool contains(Vertex&) = 0;
  
  /**
   * Determine the sloping of this surface along the X axis at the specified
   * X,Y location.
   * 
   * @param float  X location.
   * @param float  Y location.
   * @returns  X axis slope of this surface at X,Y location.
   */
  virtual float getXAcceleration(float, float) = 0;

  /**
   * Determine the sloping of this surface along the Y axis at the specified
   * X,Y location.
   * 
   * @param float  X location.
   * @param float  Y location.
   * @returns  Y axis slope of this surface at X,Y location.
   */
  virtual float getYAcceleration(float, float) = 0;

  /**
   * Notify this surface that contact was made by an entity.  This may be useful
   * if the surface represents a switch or pressure pad.  It is up to the entity
   * to decide what represents contact, although a SURFACE_MOUNTED event is
   * probably a good way to determine this.
   */
  virtual void notifyContact() = 0;
  
  /**
   * Notify this surface of an impact.  It is the responsibility of the entity
   * to decide what represents an impact.  For example, impact may be when
   * falling to the surface from above at a high speed.
   */
  virtual void notifyImpact() = 0;
  
  /**
   * Returns the friction of this surface.  The friction determines how much
   * momentum an entity should lose when moving along this surface.  The value
   * will lie between 0.0f (no friction) and 1.0f (instant halt).
   * 
   * @returns The friction of this surface.
   */
  virtual float getSurfaceFriction() = 0;

  /**
   * Returns the grip of this surface.  The grip determines how much control an
   * entity should have when moving along this surface.  The value will lie
   * between 0.0f (no control) and 1.0f (full control).
   * 
   * @returns The grip of this surface.
   */
  virtual float getSurfaceGrip() = 0;
  
  /**
   * Returns the bounciness of this surface.  The bounciness determines how much
   * of the momentum lost due to a fall onto this surface should be transferred
   * back into the entity.  A value of 0.0f should give back no momentum, whilst
   * a value of 1.0f should give back 100% of the momentum.
   * 
   * @return The bounciness of the surface.
   */
  virtual float getSurfaceBounce() = 0;
  
  /**
   * Determine whether respawning is possible on this surface.  There are three
   * possibilities: respawn is possible (YES), respawn is not possible (NO), or
   * possibility depends on some external factor (CONDITIONAL).
   * 
   * @returns Respawn possibility for this surface.
   */
  virtual RespawnPossibility getRespawnPossibility() = 0;
  
  /**
   * Determine whether respawn is possible at this moment.  This is only useful
   * when getRespawnPossibility() returns CONDITIONAL.
   * 
   * @returns true if respawn is possible now, otherwise false.
   */
  virtual bool isRespawnPossibleNow() = 0;
  
  /**
   * Determine the resting point from the specified location.  The resting point
   * is the location where an entity standing at the specified location would
   * naturally roll to without intervention.  The resulting location will be
   * such that contains() returns true.
   */
  virtual void getRestingLocation(Vertex&) = 0;
};

#endif
