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
#ifndef PLANE_SURFACE_H
#define PLANE_SURFACE_H

#include "Event.h"
#include "Coordinate.h"
#include "TextureSet.h"
#include "Point.h"
#include "ImpactPoint.h"

class PlaneSurface {
  public:

  /**************************************************************************\
   * Runtime functions                                                      *
  \**************************************************************************/
  /**
   * Return the stepping of the slope along the X axis, at the specified
   * coordinate.  A negative value indicates sloping towards the east; a
   * positive value indicates sloping towards the west.  A value of zero
   * indicates no sloping along the X axis.
   *
   * @param Coordinate&  Location to test.
   * @returns            Slope along the X axis.
   */
  virtual int getXSlopeAt(Coordinate& location) = 0;

  /**
   * Return the stepping of the slope along the Y axis, at the specified
   * coordinate.  A negative value indicates sloping towards the north; a
   * positive value indicates sloping towards the south.  A value of zero
   * indicates no sloping along the Y axis.
   *
   * @param Coordinate&  Location to test.
   * @returns            Slope along the Y axis.
   */
  virtual int getYSlopeAt(Coordinate& location) = 0;

  /**
   * Return the exact height of the surface at the specified coordinate.
   * 
   * @param Coordinate&  Location to test.
   * @returns            The height of the surface at the specified coordinate.
   */
  virtual float getHeightAt(Coordinate& point) = 0;

  /**
   * Determines whether this surface is currently present according to the
   * current global switch states.
   *
   * @returns  true if this surface is currently present, otherwise false.
   */
  virtual bool isVisible() = 0;

  // TODO: docs
  virtual int getNorth() = 0;
  virtual int getSouth() = 0;
  virtual int getEast() = 0;
  virtual int getWest() = 0;

  /**************************************************************************\
   * General functions                                                      *
  \**************************************************************************/
  /**
   * Render the plane surface using the specified texture set.
   *
   * @param TextureSet&  Texture set used for rendering this surface.
   */
  virtual void render(TextureSet& textures) = 0;

  /**
   * Determines whether the specified tile location is vertically in-line with
   * this surface.  Height data is disregarded for this test.
   *
   * @param int   Absolute X location of the tile.
   * @param int   Absolute Y location of the tile.
   * @returns     true if the tile location aligns vertically with a tile of
   *              this surface, otherwise false.
   */
  virtual bool within(int x, int y) = 0;

  /**
   * Determine whether this surface is a top-surface or bottom-surface.
   *
   * @returns  TODO: Either TOP_SURFACE or BOTTOM_SURFACE.
   */
  virtual bool isCeiling() = 0;

  /**
   * Determine whether the surface is directly below the specified point.  That
   * is, the surface must also be aligned vertically for this test to pass.
   *
   * @returns  true if the surface is directly below the specified point,
   *           otherwise false.
   */
  virtual bool isVerticallyAligned(Coordinate& location) = 0;

  /**
   * Determine whether the surface is directly below the specified point.  That
   * is, the surface must also be aligned vertically for this test to pass.
   *
   * @returns  true if the surface is directly below the specified point,
   *           otherwise false.
   */
  virtual bool isBelow(Point& point) = 0;

  /**
   * Determine whether the presence of this surface is dependent on a condition.
   *
   * @returns  true if this surface depends on a condition, otherwise false.
   */
  virtual bool isDynamic() = 0;

  /**
   * Determines the first location at which the specified line collides with
   * this surface.
   *
   * @param Coordinate  Start point of movement (the line).
   * @param Coordinate  end point of movement (the line).
   * @param float       ticks from start point to end point.
   * @returns           TODO
   */
  virtual ImpactPoint* getCollisionEvent(Coordinate&, Coordinate&, float) = 0;

  /**
   * Returns the first event for a craft rolling along this surface between
   * the two points.
   *
   * @param Coordinate&  Start point of rolling movement.
   * @param Coordinate&  End point of rolling movement.
   * @param float        ticks from start point to end point.
   * @returns            TODO
   */
  virtual ImpactPoint* getRollingEvent(Coordinate&, Coordinate&, float) = 0;

  /**************************************************************************\
   * Gameplay functionality and events                                      *
  \**************************************************************************/
  /**
   * Determine whether respawn is allowed at the specified location of the
   * player craft.  This method is only called when the player craft is
   * rolling on the surface.  The respawn point will be the center of the
   * block closest to the specified location
   *
   * @param Coordinate&  Location of the player craft.
   * @returns  true if the player can respawn here, otherwise false.
   */
  virtual bool allowRespawn(Coordinate&) = 0;

  /**
   * Get the resistance of the surface at the specified coordinate.  A return
   * of zero will cause an instant stop; a return value of one causes the
   * craft to continue at its current speed.
   */
  virtual float getResistance(Coordinate&) = 0;

  /**
   * The returned Event shall be executed on entry to this surface.  If the
   * returned Event is NULL, there is no effect.
   *
   * @returns  Pointer to an event to execute on entrance to this surface.
   */
  virtual Event* getEntryEvent() = 0;

  /**
   * The returned Event shall be executed on exit of this surface.  If the
   * returned Event is NULL, there is no effect.
   *
   * @returns  Pointer to an event to execute on exit of this surface.
   */
  virtual Event* getExitEvent() = 0;

  /**
   * The returned Event shall be executed on impact of this surface; that is,
   * when the craft lands on the surface rather than rolling onto it.  If the
   * returned Event is NULL, there is no effect.
   *
   * @returns  Pointer to an event to execute on impact of this surface.
   */
  virtual Event* getImpactEvent() = 0;
};

#endif
