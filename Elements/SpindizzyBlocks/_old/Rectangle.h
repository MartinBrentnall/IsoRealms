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
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream> // NOTE: For debug.
#include <math.h>

#include "SpindizzyConstants.h"
#include "PlaneSurface.h"
#include "TextureSet.h"
#include "ColourScheme.h"
#include "Coordinate.h"
#include "Colour.h"
#include "Point.h"
#include "Condition.h"
#include "Event.h"

using namespace std; // TODO: Localise scope.
// TODO: Rename all fields
class Rectangle:public SpindizzyConstants, PlaneSurface {
  private:
  int top;
  int bottom;
  int left;
  int right;

  /** Starting height of this surface. */
  int height;

  /** Type of this surface. */
  int symbol;

  /** true if this is a bottom-surface. TODO: TOP_SURFACE or BOTTOM_SURFACE */
  bool ceiling;

  /** Sloping along the X axis of this surface. */
  int xSlope;

  /** Sloping along the Y axis of the surface. */
  int ySlope;

  /** The condition on which this surface is present. */
  Condition* cCondition;

  /**
   * Using a line between the two specified Coordinates, returns the point at
   * which the line crosses the X,Y boundary of this surface.  If the line does
   * not cross the boundary, NULL is returned.
   *
   * @param  Coordinate&  Start point.
   * @param  Coordinate&  End point.
   * @returns             The Coordinate at which the line crosses the boundary
   *                      of this surface, or NULL if it doesn't cross.
   */
  Coordinate* getBoundaryCrossingPoint(Coordinate&, Coordinate&, float*);

  public:

  /**************************************************************************\
   * Constructors                                                           *
  \**************************************************************************/
  Rectangle(); // TODO: Temporary for Block.cpp

  /**
   * Construct a surface rectangle of the specified values.
   *
   * TODO: Fill in; re-order parameters.
   * @param  int              
   * @param  int              
   * @param  int              
   * @param  int              
   * @param  int         Height of this surface at its lowest point.
   * @param  int         Surface type.
   * @param  bool        Either TOP_SURFACE or BOTTOM_SURFACE.
   * @param  int         Sloping of this surface along the X axis.
   * @param  int         Sloping of this surface along the Y axis.
   * @param  Condition*  The condition on which this surface should be present.
   */
  Rectangle(int, int, int, int, int, int, bool, int, int, Condition*);

  /**
   * Determines whether this surface represents a switch.
   *
   * @returns  true if this surface represents a switch, otherwise false.
   */ 
  bool isSwitch();
  void debug();

  float getHeightAt(float x, float y);
  
  /**************************************************************************\
   * Implemented methods of PlaneSurface.h                                  *
  \**************************************************************************/
  int getXSlopeAt(Coordinate& location);
  int getYSlopeAt(Coordinate& location);
  int getNorth();
  int getSouth();
  int getEast();
  int getWest();
  float getHeightAt(Coordinate& point);
  bool isVisible();
  void render(TextureSet& textures);
  bool within(int, int);
  bool isCeiling();
  bool isVerticallyAligned(Coordinate&);
  bool isBelow(Point& point);
  bool isDynamic();
  ImpactPoint* getRollingEvent(Coordinate&, Coordinate&, float);
  ImpactPoint* getCollisionEvent(Coordinate&, Coordinate&, float);
};

#endif
