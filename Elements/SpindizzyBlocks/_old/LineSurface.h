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
#ifndef LINE_SURFACE
#define LINE_SURFACE

#include "PlaneSurface.h"
#include "SpindizzyConstants.h"
#include "MiscFunctions.h"

/**
 * The LineSurface represents the infinitely thin line lying between two
 * adjacent surfaces.
 */
class LineSurface:public PlaneSurface, SpindizzyConstants {
  private:
  bool cHorizontal;
  int cLocation;
  int cLow;
  int cHigh;
  int cXSlope;
  int cYSlope;
  int cZ;

  public:

  /**
   * Construct a line surface between the two specified surfaces.
   */
  LineSurface(PlaneSurface*, PlaneSurface*);

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
  int getSymbol();
  bool isDynamic();
  ImpactPoint* getRollingEvent(Coordinate&, Coordinate&, float);
  ImpactPoint* getCollisionEvent(Coordinate&, Coordinate&, float);
};

#endif

