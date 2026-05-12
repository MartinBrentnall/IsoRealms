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
#include "WallSegment.h"

namespace IsoRealms::Equilibria {
  WallSegment::WallSegment(int bottomStart, int bottomEnd, int topStart, int topEnd) :
            cBottomStart(bottomStart),
            cBottomEnd(bottomEnd),
            cTopStart(topStart),
            cTopEnd(topEnd) {
  }

  int WallSegment::getBottomStart() {
    return cBottomStart;
  }

  int WallSegment::getBottomEnd() {
    return cBottomEnd;
  }

  int WallSegment::getTopStart() {
    return cTopStart;
  }

  int WallSegment::getTopEnd() {
    return cTopEnd;
  }

  bool WallSegment::unite(WallSegment* segment) {
    if (isOverlapping(segment)) {
      cBottomStart = std::min(cBottomStart, segment->cBottomStart);
      cBottomEnd   = std::min(cBottomEnd,   segment->cBottomEnd);
      cTopStart    = std::max(cTopStart,    segment->cTopStart);
      cTopEnd      = std::max(cTopEnd,      segment->cTopEnd);
      return true;
    }
    return false;
  }

  bool WallSegment::isInside(WallSegment* segment) {
    return (cBottomStart > segment->cBottomStart && cTopStart < segment->cTopStart)
        || (cBottomEnd   > segment->cBottomEnd   && cTopEnd   < segment->cTopEnd);
  }

  bool WallSegment::isCovering(WallSegment* segment) {
    return segment->cBottomStart >= cBottomStart
        && segment->cTopStart    <= cTopStart
        && segment->cBottomEnd   >= cBottomEnd
        && segment->cTopEnd      <= cTopEnd;
  }

  bool WallSegment::isHigher(WallSegment* segment) {
    return (cBottomStart > segment->cBottomStart && cBottomEnd > segment->cBottomEnd)
        || (cTopStart    > segment->cTopStart    && cTopEnd    > segment->cTopEnd);
  }

  bool WallSegment::isOverlapping(WallSegment* segment) {
    return ((cTopStart   >= segment->cBottomStart && cTopStart    <= segment->cTopStart)
        || (cBottomStart >= segment->cBottomStart && cBottomStart <= segment->cTopStart))
        && ((cTopEnd     >= segment->cBottomEnd   && cTopEnd      <= segment->cTopEnd)
        || (cBottomEnd   >= segment->cBottomEnd   && cBottomEnd   <= segment->cTopEnd));
  }

  WallSegment::Relation WallSegment::getRelationWith(WallSegment* segment) {
    return isInside(segment)   ? Relation::COVERING
         : isCovering(segment) ? Relation::INSIDE
         : isHigher(segment)   ? Relation::HIGHER
         : /* Must be */         Relation::LOWER;
  }

  bool WallSegment::isAddition() {
    return cTopStart >= cBottomStart && cTopEnd >= cBottomEnd && cTopStart + cTopEnd > cBottomStart + cBottomEnd;
  }

  bool WallSegment::raiseBottom(WallSegment* segment) {
    if (!isAddition()) {
      return true;
    }
    cTopStart = std::min(cTopStart, segment->cBottomStart);
    cTopEnd   = std::min(cTopEnd,   segment->cBottomEnd);
    return isAddition();
  }

  bool WallSegment::lowerTop(WallSegment* segment) {
    if (!isAddition()) {
      return true;
    }
    cBottomStart = std::max(cBottomStart, segment->cTopStart);
    cBottomEnd   = std::max(cBottomEnd,   segment->cTopEnd);
    return isAddition();
  }

  void WallSegment::convertToAddition() {
    if (cBottomStart > cTopStart) {
      int temp = cBottomStart;
      cBottomStart = cTopStart;
      cTopStart = temp;
      temp = cBottomEnd;
      cBottomEnd = cTopEnd;
      cTopEnd = temp;
    }
  }

  void WallSegment::debug() {
    for (int i = -2; i < 8; i++) {
      if (i > cBottomStart && i <= cTopStart) {
        std::cout << "[O]";
      } else {
        std::cout << "   ";
      }
    }
    std::cout << std::endl;
  }
}
