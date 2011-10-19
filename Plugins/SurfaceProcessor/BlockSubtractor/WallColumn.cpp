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
#include "WallColumn.h"

WallColumn::WallColumn(int bottomStart, int bottomEnd, int topStart, int topEnd) {
  cBottomHeightStart = bottomStart;
  cBottomHeightEnd = bottomEnd;
  cTopHeightStart = topStart;
  cTopHeightEnd = topEnd;
}

WallColumn::WallColumn(const WallColumn& wallColumn) {
  cBottomHeightStart = wallColumn.cBottomHeightStart;
  cBottomHeightEnd = wallColumn.cBottomHeightEnd;
  cTopHeightStart = wallColumn.cTopHeightStart;
  cTopHeightEnd = wallColumn.cTopHeightEnd;
}

int WallColumn::getBottomHeightStart() {
  return cBottomHeightStart;
}

int WallColumn::getBottomHeightEnd() {
  return cBottomHeightEnd;
}

int WallColumn::getTopHeightStart() {
  return cTopHeightStart;
}

int WallColumn::getTopHeightEnd() {
  return cTopHeightEnd;
}

bool WallColumn::unite(WallColumn* wallColumn) {
  if (isOverlapping(wallColumn)) {
    cBottomHeightStart = min(cBottomHeightStart, wallColumn->cBottomHeightStart);
    cBottomHeightEnd   = min(cBottomHeightEnd,   wallColumn->cBottomHeightEnd);
    cTopHeightStart    = max(cTopHeightStart,    wallColumn->cTopHeightStart);
    cTopHeightEnd      = max(cTopHeightEnd,      wallColumn->cTopHeightEnd);
    return true;
  }
  return false;
}

bool WallColumn::isInside(WallColumn* wallColumn) {
  return (cBottomHeightStart > wallColumn->cBottomHeightStart && cTopHeightStart < wallColumn->cTopHeightStart)
      || (cBottomHeightEnd   > wallColumn->cBottomHeightEnd   && cTopHeightEnd   < wallColumn->cTopHeightEnd);
}

bool WallColumn::isCovering(WallColumn* wallColumn) {
  return wallColumn->cBottomHeightStart >= cBottomHeightStart
      && wallColumn->cTopHeightStart    <= cTopHeightStart
      && wallColumn->cBottomHeightEnd   >= cBottomHeightEnd
      && wallColumn->cTopHeightEnd      <= cTopHeightEnd;
}

bool WallColumn::isHigher(WallColumn* wallColumn) {
  return (cBottomHeightStart > wallColumn->cBottomHeightStart && cBottomHeightEnd > wallColumn->cBottomHeightEnd)
      || (cTopHeightStart    > wallColumn->cTopHeightStart    && cTopHeightEnd    > wallColumn->cTopHeightEnd);
}

bool WallColumn::isOverlapping(WallColumn* wallColumn) {
  return ((cTopHeightStart   >= wallColumn->cBottomHeightStart && cTopHeightStart    <= wallColumn->cTopHeightStart)
      || (cBottomHeightStart >= wallColumn->cBottomHeightStart && cBottomHeightStart <= wallColumn->cTopHeightStart))
      && ((cTopHeightEnd     >= wallColumn->cBottomHeightEnd   && cTopHeightEnd      <= wallColumn->cTopHeightEnd)
      || (cBottomHeightEnd   >= wallColumn->cBottomHeightEnd   && cBottomHeightEnd   <= wallColumn->cTopHeightEnd));
}

WallColumn::Relation WallColumn::getRelationWith(WallColumn* wallColumn) {
  return isInside(wallColumn)   ? COVERING
       : isCovering(wallColumn) ? INSIDE
       : isHigher(wallColumn)   ? HIGHER
       : /* Must be */            LOWER;
}

bool WallColumn::isAddition() {
  return cTopHeightStart > cBottomHeightStart && cTopHeightEnd > cBottomHeightEnd;
}

bool WallColumn::raiseBottom(WallColumn* wallColumn) {
  if (!isAddition()) {
    return true;
  }
  cTopHeightStart = min(cTopHeightStart, wallColumn->cBottomHeightStart);
  cTopHeightEnd   = min(cTopHeightEnd,   wallColumn->cBottomHeightEnd);
  return isAddition();
}

bool WallColumn::lowerTop(WallColumn* wallColumn) {
  if (!isAddition()) {
    return true;
  }
  cBottomHeightStart = max(cBottomHeightStart, wallColumn->cTopHeightStart);
  cBottomHeightEnd   = max(cBottomHeightEnd,   wallColumn->cTopHeightEnd);
  return isAddition();
}

void WallColumn::convertToAddition() {
  if (cBottomHeightStart > cTopHeightStart) {
    int temp = cBottomHeightStart;
    cBottomHeightStart = cTopHeightStart;
    cTopHeightStart = temp;
    temp = cBottomHeightEnd;
    cBottomHeightEnd = cTopHeightEnd;
    cTopHeightEnd = temp;
  }
}

void WallColumn::debug() {
  for (int i = -2; i < 8; i++) {
    if (i > cBottomHeightStart && i <= cTopHeightStart) {
      std::cout << "[O]";
    } else {
      std::cout << "   ";
    }
  }
  std::cout << std::endl;
}

WallColumn::~WallColumn() {
}


