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
#ifndef WALL_CONSTRUCTION_DATA_H
#define WALL_CONSTRUCTION_DATA_H

#include <IsoRealms/Resources/GeometryProcessor/IWallSurface.h>
#include <IsoRealms/Resources/GeometryProcessor/IWallSurfaceTemplate.h>

#include "WallColumn.h"

class WallConstructionData:public IWallSurfaceTemplate {
  private:
  int cX;
  int cY;
  int cLength;
  int cBottomHeightStart;
  int cBottomHeightSlope;
  int cTopHeightStart;
  int cTopHeightSlope;
  Condition* cCondition;
  IWallSurface::FaceDirection cFacing;

  public:
  WallConstructionData(int, int, WallColumn*, Condition*, IWallSurface::FaceDirection);
  bool unite(WallColumn*, Condition*);

  int getWallBottom();

  /***********************************\
   * Implements IWallSurfaceTemplate *
  \***********************************/
  int getX();
  int getY();
  IWallSurface::FaceDirection getFaceDirection();
  int getLength();
  int getStartHeight();
  int getEndHeight();
  int getBottomSlope();
  int getTopSlope();
  Condition* getCondition();
  
  ~WallConstructionData();
};

#endif
