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

namespace IsoRealms::Equilibria {
  class ISurface;
  class Wall;
  class Zone;

  /**
   * Interface to be implemented by physical objects, containing functions for
   * determining the internal state of the object as well as notifying the
   * object of collision events.
   */
  class IPhysicalObject {
    public:
    
    /**
     * Retrieve the thrust of this object on the X axis.  This value is used to
     * calculate physical movement of the object.
     *
     * @return Thrust of this object on the X axis.
     */
    virtual float getXThrust() const = 0;
    
    /**
     * Retrieve the thrust of this object on the Y axis.  This value is used to
     * calculate physical movement of the object.
     *
     * @return Thrust of this object on the Y axis.
     */
    virtual float getYThrust() const = 0;
    
    /**
     * Retrieve the step reach of this object.  This value is used to determine
     * when the object is able to mount a surface that is higher than itself.
     *
     * @return Step reach of this object.
     */
    virtual float getStepReach() const = 0;

    /**
     * Retrieve the height of this object.
     *
     * @return Height of this object.
     */
    virtual float getHeight() const = 0;
    
    /**
     * Retrieve the radius of this object.
     *
     * @return Radius of this object.
     */
    virtual float getRadius() const = 0;

    /**
     * Retrieve the momentum below which this object will begin to "hug" a
     * wall.
     *
     * @return Hug momentum of this object.
     */
    virtual float getHugMomentum() const = 0;

    /**
     * Retrieve the bounce factor of this object.  This value is used to
     * calculate a new Z-momentum when this object bounces from a surface.
     *
     * @return Bounce factor of this object.
     */
    virtual float getBounceFactor() const = 0;
    
    /**
     * Retrieve the zone in which this object resides.  This value is passed
     * to the world for determining collision events.  nullptr indicates that
     * the object resides in the world as a whole and does not reside in any
     * specific zone (e.g. the Player object).
     *
     * @return Zone in which this object resides.
     */
    virtual Zone* getHome() const = 0;
    
    virtual bool allowTraversal(ISurface* surface) const = 0;
    virtual bool isHuggable(Wall* wall) const = 0;
    
    /**
     * Determine whether this object triggers actions via contact with surfaces
     * and walls.
     *
     * @return true if this object triggers action via contact, otherwise false
     *         so that actions won't be triggered by this object.
     */
    virtual bool triggersContacts() const = 0;

    /**
     * Called to notify the object that the physical state has changed (the
     * location and momentum).
     */
    virtual void physicalStateChanged() = 0;
    
    /**
     * Called to notify the object of impact with the current surface.
     */
    virtual void impactSurface() = 0;
    
    /**
     * Called to notify the object that it has mounted the current surface.
     */
    virtual void mountSurface() = 0;
    
    /**
     * Called to notify the object that it has bounced from the current
     * surface.
     */
    virtual void bounceSurface() = 0;
    
    /**
     * Called to notify the object that it's about to leave the current
     * surface.
     */
    virtual void leaveSurface(ISurface* surface) = 0;
    
    /**
     * Called to notify the object that it has bounced off a wall.
     */
    virtual void bounceWall(Wall* wall) = 0;

    /**
     * Retrieve surface specific to this object (e.g. used for effects like
     * levitation).
     */
    virtual ISurface* getObjectSurface() = 0;

    virtual void objectApex() = 0;
  };
}
