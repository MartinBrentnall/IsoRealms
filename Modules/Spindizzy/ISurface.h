/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <optional>
#include <vector>

#include "IsoRealms.h"

namespace IsoRealms::Spindizzy {
  class CollisionData;
  class IWorldObject;
  class Zone;

  class ISurface {
    public:
    enum class Direction {
      UP,
      DOWN
    };
    
    enum class Respawn {
      YES,
      CONDITIONAL,
      NO
    };

    /**
    * Determine whether an event occurs assuming that the specified vertices
    * represent non-rolling movement over the surface (i.e. the surface has not
    * yet been mounted by the entity represented by the movement).  The only
    * possible event type is a CollisionData::Type::SURFACE_MOUNT event.
    * 
    * @param Vertex&  Starting point of movement along the surface.
    * @param Vertex&  Ending point of movement along the surface.
    * @returns  An event if one is generated, otherwise nullptr.
    */
    virtual std::unique_ptr<CollisionData> getCollision(LiteralVertex&, LiteralVertex&, float, double, double) = 0;
    
    /**
    * Determine whether an event occurs assuming that the specified vertices
    * represent a rolling movement over the surface (height is ignored).  The
    * only possible event type is a CollisionData::SURFACE_LEFT event.  The
    * location of the event will be such that contains() returns false for this
    * location.
    * 
    * @param Vertex&  Starting point of movement along the surface.
    * @param Vertex&  Ending point of movement along the surface.
    * @returns  An event if one is generated, otherwise nullptr.
    */
    virtual std::unique_ptr<CollisionData> getRollingEvent(LiteralVertex&, LiteralVertex&, double, double) = 0;
    
    /**
    * Return the height (Z) of this surface at the specified X,Y location, such
    * that a call to contains() using all three values in a Vertex will return
    * true.
    * 
    * @param float  X location.
    * @param float  Y location.
    * @returns  Height (z) of this surface at X,Y location.
    */
    virtual float getHeightAt(float x, float y) const = 0;

    /**
    * Determine whether the specified Vertex is on this surface.
    * 
    * @param Vertex&  The vertex to test.
    * @returns true if the vertex is on this surface, otherwise false.
    */
    virtual bool contains(LiteralVertex&, float) = 0;
    
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
    * to decide what represents contact, although a SURFACE_MOUNT event is
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
    * Determine whether respawning is allowed on this surface.  There are three
    * possibilities: respawn is possible (YES), respawn is not possible (NO), or
    * respawn is conditional (CONDITIONAL).
    * 
    * @returns Respawn allowance for this surface.
    */
    virtual Respawn isRespawnAllowed() = 0;
    
    /**
    * Determine whether respawn is possible at this moment.  This is only useful
    * when isRespawnAllowed() returns CONDITIONAL.
    * 
    * @returns true if respawn is possible now, otherwise false.
    */
    virtual bool isRespawnPossible() = 0;
    
    /**
    * Determine the resting point from the specified location.  The resting point
    * is the location where an entity standing at the specified location would
    * naturally roll to without intervention.  The resulting location will be
    * such that contains() returns true.
    */
    virtual void getRestingLocation(LiteralVertex&) = 0;
    
    virtual void saveCache(std::ostream& cache, bool physical) = 0;

    virtual int getXStart() const = 0;
    virtual int getXEnd() const = 0;
    virtual int getYStart() const = 0;
    virtual int getYEnd() const = 0;
    
    virtual IWorldObject* getOwner() = 0;
    virtual Zone& getZone() = 0;
    
    virtual bool isSolid() = 0;
    
    virtual void adjustPosition(LiteralVertex& location, double milliseconds) = 0;

    virtual void render() const = 0;
    virtual void renderOutline() const = 0;
    virtual void renderHighlight() const = 0;


    /**
    * Get the base height of the specified cell in this surface.
    *
    * @param int  X location of the cell.
    * @param int  Y location of the cell.
    * @returns  Base height of the cell.
    */
    virtual int getSurfaceCellHeight(int x, int y) const = 0;

    /**
    * Get the elevation of the specified cell in this surface.  The elevation
    * is the difference between the base height of the cell surface and the
    * highest point of the cell surface.  Note that the value may be negative!
    *
    * @param int  X location of the cell.
    * @param int  Y location of the cell.
    * @returns  Elevation of the cell.
    */
    virtual int getSurfaceCellElevation(int x, int y) const = 0;

    virtual bool alligned(int x, int y) const = 0;

    virtual std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals() = 0;

    virtual ~ISurface() {}
  };
}
