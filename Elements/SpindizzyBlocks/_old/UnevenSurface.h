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
#ifndef UNEVEN_SURFACE_H
#define UNEVEN_SURFACE_H

#include <math.h>

#include "MiscFunctions.h"
#include "PlaneSurface.h"
#include "Condition.h"
#include "SpindizzyConstants.h"
#include "TextureSet.h"

class UnevenSurface:public SpindizzyConstants, PlaneSurface {
  private:
  int cX;
  int cY;
  int cZ;
  int cNorthCorner;
  int cEastCorner;
  int cSouthCorner;
  int cWestCorner;
  Condition* cCondition;
  bool cSplit;

  void renderAsVerticalSplit();
  void renderAsHorizontalSplit();
  int getSlopeStartAt(Coordinate& location);
  int getSlopeStartFor(int corner, int edgeA, int edgeB);

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
  UnevenSurface(int, int, int, int, int, int, int, bool, Condition*);

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
  bool within(int x, int y);
  bool isCeiling();
  bool isVerticallyAligned(Coordinate&);
  bool isBelow(Point& point);
  bool isDynamic();
  ImpactPoint* getRollingEvent(Coordinate&, Coordinate&, float);
  ImpactPoint* getCollisionEvent(Coordinate&, Coordinate&, float);
};

#endif
