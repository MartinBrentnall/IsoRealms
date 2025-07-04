/*
 * Copyright 2023 Martin Brentnall
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
#include "WallTemplate.h"

#include "IsoRealms/Project.h"

namespace IsoRealms::Spindizzy {
  WallTemplate::WallTemplate(int x, int y, WallSegment* segment, std::optional<Condition>& condition, Wall::Direction facing) :
            cX(x),
            cY(y),
            cLength(1),
            cFacing(facing),
            cBottomHeightStart(segment->getBottomStart()),
            cBottomHeightSlope(segment->getBottomEnd() - cBottomHeightStart),
            cTopHeightStart(segment->getTopStart()),
            cTopHeightSlope(segment->getTopEnd() - cTopHeightStart),
            cCondition(condition) {
  }

  bool WallTemplate::unite(WallSegment* segment, std::optional<Condition>& condition) {
    if ((condition.has_value() && cCondition.has_value() && !(condition == cCondition)) || (!cCondition.has_value()) != (!condition.has_value())) {
      return false;
    } 
    int mBottomHeightStart = segment->getBottomStart();
    int mTopHeightStart    = segment->getTopStart();
    int mBottomHeightSlope = segment->getBottomEnd() - mBottomHeightStart;
    int mTopHeightSlope    = segment->getTopEnd()    - mTopHeightStart;
    int mExpectedTopHeight = cTopHeightStart + cTopHeightSlope * cLength;
    int mExpectedBottomHeight = cBottomHeightStart + cBottomHeightSlope * cLength;
    if (mBottomHeightSlope == cBottomHeightSlope && mTopHeightSlope == cTopHeightSlope && mExpectedTopHeight == mTopHeightStart && mExpectedBottomHeight == mBottomHeightStart) {
      cLength++;
      return true;
    }
    return false;
  }

  Wall::Direction WallTemplate::getFaceDirection() {
    return cFacing;
  }

  int WallTemplate::getX() {
    return cX;
  //  return cFacing == Wall::Direction::NORTH || Wall::Direction::SOUTH ? cX - (cLength - 1) : cX;
  }

  int WallTemplate::getY() {
    return cY;
  //  return cFacing == Wall::Direction::EAST || Wall::Direction::WEST ? cY - (cLength - 1) : cY;
  }

  int WallTemplate::getLength() {
    return cLength;
  }

  int WallTemplate::getStartHeight() {
    return cBottomHeightStart;
  }

  int WallTemplate::getEndHeight() {
    int mSlopeOffset = cTopHeightSlope < 0 ? cLength * cTopHeightSlope : 0;
    return (cTopHeightStart - cBottomHeightStart) + mSlopeOffset;
  }

  int WallTemplate::getBottomSlope() {
    return cBottomHeightSlope;
  }

  int WallTemplate::getTopSlope() {
    return cTopHeightSlope;
  }

  std::optional<Condition>& WallTemplate::getCondition() {
    return cCondition;
  }
}
