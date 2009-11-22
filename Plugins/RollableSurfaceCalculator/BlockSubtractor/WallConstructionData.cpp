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

WallConstructionData::WallConstructionData(WallColumn* wallColumn) {
  cBottomHeightStart = wallColumn->getBottomHeightStart();
  cTopHeightStart    = wallColumn->getTopHeightStart();
  cBottomHeightSlope = wallColumn->getBottomHeightEnd() - cBottomHeightStart;
  cTopHeightSlope    = wallColumn->getTopHeightEnd()    - cTopHeightStart;
  cLength = 1;
}

bool WallConstructionData::unite(WallColumn* wallColumn) {
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

IWallSurface* WallConstructionData::constructSurface(IRollableSurfaceProvider* provider, int x, int y, IWallSurface::FaceDirection facing) {
  switch (facing) {
    case IWallSurface::NORTH: 
    case IWallSurface::SOUTH: {
      x -= (cLength - 1);
      break;
    }

    case IWallSurface::EAST: 
    case IWallSurface::WEST: {
      y -= (cLength - 1);
      break;
    }
  }
  int mSlopeOffset = cTopHeightSlope < 0 ? cLength * cTopHeightSlope : 0;
  return provider->createSubSurface(x, y, facing, cLength, cBottomHeightStart, (cTopHeightStart - cBottomHeightStart) + mSlopeOffset, cBottomHeightSlope, cTopHeightSlope);
}
