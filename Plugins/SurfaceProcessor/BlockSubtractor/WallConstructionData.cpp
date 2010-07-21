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
#include "WallConstructionData.h"

WallConstructionData::WallConstructionData(int x, int y, WallColumn* wallColumn, Condition* condition, IWallSurface::FaceDirection facing) {
  cX = x;
  cY = y;
  cBottomHeightStart = wallColumn->getBottomHeightStart();
  cTopHeightStart    = wallColumn->getTopHeightStart();
  cBottomHeightSlope = wallColumn->getBottomHeightEnd() - cBottomHeightStart;
  cTopHeightSlope    = wallColumn->getTopHeightEnd()    - cTopHeightStart;
  cLength = 1;
  cCondition = condition;
  cFacing = facing;
}

bool WallConstructionData::unite(WallColumn* wallColumn, Condition* condition) {
  if ((condition != NULL && cCondition != NULL && !(*condition == *cCondition)) || (cCondition == NULL) != (condition == NULL)) {
    return false;
  } 
  int mBottomHeightStart = wallColumn->getBottomHeightStart();
  int mTopHeightStart    = wallColumn->getTopHeightStart();
  int mBottomHeightSlope = wallColumn->getBottomHeightEnd() - mBottomHeightStart;
  int mTopHeightSlope    = wallColumn->getTopHeightEnd()    - mTopHeightStart;
  int mExpectedTopHeight = cTopHeightStart + cTopHeightSlope * cLength;
  int mExpectedBottomHeight = cBottomHeightStart + cBottomHeightSlope * cLength;
  if (mBottomHeightSlope == cBottomHeightSlope && mTopHeightSlope == cTopHeightSlope && mExpectedTopHeight == mTopHeightStart && mExpectedBottomHeight == mBottomHeightStart) {
    cLength++;
    return true;
  }
  return false;
}

int WallConstructionData::getWallBottom() {
  // TODO: Needs to work for slopes
  return cBottomHeightStart;
}

IWallSurface::FaceDirection WallConstructionData::getFaceDirection() {
  return cFacing;
}

int WallConstructionData::getX() {
  return cX;
//  return cFacing == IWallSurface::NORTH || IWallSurface::SOUTH ? cX - (cLength - 1) : cX;
}

int WallConstructionData::getY() {
  return cY;
//  return cFacing == IWallSurface::EAST || IWallSurface::WEST ? cY - (cLength - 1) : cY;
}

int WallConstructionData::getLength() {
  return cLength;
}

int WallConstructionData::getStartHeight() {
  return cBottomHeightStart;
}

int WallConstructionData::getEndHeight() {
  int mSlopeOffset = cTopHeightSlope < 0 ? cLength * cTopHeightSlope : 0;
  return (cTopHeightStart - cBottomHeightStart) + mSlopeOffset;
}

int WallConstructionData::getBottomSlope() {
  return cBottomHeightSlope;
}

int WallConstructionData::getTopSlope() {
  return cTopHeightSlope;
}

Condition* WallConstructionData::getCondition() {
  return cCondition;
}

