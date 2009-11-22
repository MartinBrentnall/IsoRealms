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
#ifndef WALL_SURFACE_H
#define WALL_SURFACE_H

#include "ColourScheme.h"
#include "Colour.h"
#include "WallColumn.h"
#include "Condition.h"
#include "TextureSet.h"
#include "ImpactPoint.h"
#include "Rect.h"

/**
 * Objects of this class represent quad wall surfaces.
 */
class WallSurface:public SpindizzyConstants {
  private:

  /** Starting X location of the surface. */
  int cX;

  /** Starting Y location of the surface. */
  int cY;

  /** Starting Z location (bottom) of the surface. */
  int cZ;

  /** Length of the surface. */
  int cLength;

  /** Height of the surface. */
  int cHeight;

  /** The face direction of the surface. */
  int cFacing;

  /** Sloping at the bottom of this surface, from the starting location. */
  int cBottomSlope;

  /** Sloping at the top of this surface, from the starting location. */
  int cTopSlope;

  /** The type of surface. */
  int cSymbol;

  /** The condition on which this surface is present. */
  Condition* cCondition;

  /**
   * Internal function used to determine collision with the wall in two
   * dimensions (top-down).
   */
  ImpactPoint* getIntersectionGradient(Rect, Coordinate&, Coordinate&);

  /**
   * Get the height of the wall at the specified coordinate.  Used
   * internally for the collision detection mechanism.
   */
  float getHeightAt(Coordinate&);

  /**
   * Return the event type for collisions with this wall.
   *
   * @returns  The event type for collisions with this wall.
   */
  int getCollisionEventType();

  public:
  /**
   * Construct a new wall surface at the specified location, 
   *
   * @param int  
   * @param int  
   * @param WallColumn::WallSection  
   * @param int  
   * @param Condition*  
   * @param int  
   */
  WallSurface(int, int, WallColumn::WallSection, int, int symbol);

  /**
   * Attempts to join the specified wall tile at the specifed location to
   * this one.  If successful, this wall is modified and true is returned,
   * otherwise false is returned and this function has no effect.
   *
   * @param int  
   * @param int  
   * @param WallColumn::WallSection  
   * @returns
   */
  bool attemptJoin(int, int, WallColumn::WallSection wall);

  /**
   * Render the plane surface using the specified texture set.
   *
   * @param TextureSet&  Texture set used for rendering this surface.
   */
  void render(TextureSet& textures);

  /**
   * Determine whether the presence of this surface is dependent on a condition.
   *
   * @returns  true if this surface depends on a condition, otherwise false.
   */
  bool isDynamic();

  /**
   * Determines whether this surface occupies the specified tile location.
   *
   * @param int  X tile location.
   * @param int  Y tile location.
   */
  bool occupies(int, int);

  /**
   * Determine TODO
   */
  ImpactPoint* getCollisionEvent(Coordinate&, Coordinate&);

  ImpactPoint* getSlideEvent(Coordinate&, Coordinate&);

  /**
   * Facing
   */
  int getFacing();

  void debug();
};

#endif
