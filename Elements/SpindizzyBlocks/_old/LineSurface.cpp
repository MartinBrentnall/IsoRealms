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
#include "LineSurface.h"

LineSurface::LineSurface(PlaneSurface* a, PlaneSurface* b) {
  cXSlope = 0;
  cYSlope = 0;
  cZ = 0;
  if (a->getSouth() + 1 == b->getNorth()) {
    cHorizontal = true;
    cLocation = a->getSouth();
    cLow = max(a->getWest(), b->getWest());
    cHigh = min(a->getEast(), b->getEast());
    Coordinate mCoordinate(cLow, cLocation, 0.0f);
    Coordinate mPointCoordinate(cLow - BLOCK_RADIUS, cLocation + BLOCK_RADIUS, 0.0f);
    int mBSlopePerp = a->getYSlopeAt(mCoordinate);
    int mASlopePerp = b->getYSlopeAt(mCoordinate);
    if (mASlopePerp < 0) {
      cYSlope = mASlopePerp;
      cXSlope = b->getXSlopeAt(mCoordinate);
      cZ = (int) b->getHeightAt(mPointCoordinate);
      cout << "Z1: " << cZ << endl;
    } else if (mBSlopePerp > 0) {
      cYSlope = mBSlopePerp;
      cXSlope = a->getXSlopeAt(mCoordinate);
      cZ = (int) a->getHeightAt(mPointCoordinate);
      cout << "Z2: " << cZ << endl;
    } else {
      cout << cLocation << "," << cLow << endl;
      cXSlope = a->getXSlopeAt(mCoordinate);
      cZ = (int) a->getHeightAt(mPointCoordinate);
      cout << "Z3: " << cZ << endl;
    }
  } else if (b->getSouth() + 1 == a->getNorth()) {
    cHorizontal = true;
    cLocation = b->getSouth();
    cLow = max(a->getWest(), b->getWest());
    cHigh = min(a->getEast(), b->getEast());
    Coordinate mCoordinate(cLow, cLocation, 0.0f);
    Coordinate mPointCoordinate(cLow + BLOCK_RADIUS, cLocation + BLOCK_RADIUS, 0.0f);
    int mASlopePerp = a->getYSlopeAt(mCoordinate);
    int mBSlopePerp = b->getYSlopeAt(mCoordinate);
    if (mASlopePerp < 0) {
      cYSlope = mASlopePerp;
      cXSlope = a->getXSlopeAt(mCoordinate);
      cZ = (int) a->getHeightAt(mPointCoordinate);
      cout << "Z4: " << cZ << endl;
    } else if (mBSlopePerp > 0) {
      cYSlope = mBSlopePerp;
      cXSlope = b->getXSlopeAt(mCoordinate);
      cZ = (int) b->getHeightAt(mPointCoordinate);
      cout << "Z5: " << cZ << endl;
    } else {
      cXSlope = a->getXSlopeAt(mCoordinate);
      cZ = (int) a->getHeightAt(mPointCoordinate);
      cout << "Z6: " << cZ << endl;
    }
  } else if (a->getEast() + 1 == b->getWest()) {
    cHorizontal = false;
    cLocation = a->getEast();
    cLow = max(a->getNorth(), b->getNorth());
    cHigh = min(a->getSouth(), b->getSouth());
    Coordinate mCoordinate(cLocation, cLow, 0.0f);
    int mBSlopePerp = a->getXSlopeAt(mCoordinate);
    int mASlopePerp = b->getXSlopeAt(mCoordinate);
    if (mASlopePerp < 0) {
      cXSlope = mASlopePerp;
      cYSlope = b->getYSlopeAt(mCoordinate);
    } else if (mBSlopePerp > 0) {
      cXSlope = mBSlopePerp;
      cYSlope = a->getYSlopeAt(mCoordinate);
    } else {
      cYSlope = a->getYSlopeAt(mCoordinate);
    }
  } else if (b->getEast() + 1 == a->getWest()) {
    cHorizontal = false;
    cLocation = b->getEast();
    cLow = max(a->getNorth(), b->getNorth());
    cHigh = min(a->getSouth(), b->getSouth());
    Coordinate mCoordinate(cLocation, cLow, 0.0f);
    int mASlopePerp = a->getXSlopeAt(mCoordinate);
    int mBSlopePerp = b->getXSlopeAt(mCoordinate);
    if (mASlopePerp < 0) {
      cXSlope = mASlopePerp;
      cYSlope = a->getYSlopeAt(mCoordinate);
    } else if (mBSlopePerp > 0) {
      cXSlope = mBSlopePerp;
      cYSlope = b->getYSlopeAt(mCoordinate);
    } else {
      cYSlope = a->getYSlopeAt(mCoordinate);
    }
  }
  cout << "On a line surface...  X: " << cXSlope << " Y: " << cYSlope << endl;
}

int LineSurface::getXSlopeAt(Coordinate& location) {
  return cXSlope;
}

int LineSurface::getYSlopeAt(Coordinate& location) {
  return cYSlope;
}

int LineSurface::getNorth() {
  cout << "Function should never be called" << endl;
  exit(1);
}

int LineSurface::getSouth() {
  cout << "Function should never be called" << endl;
  exit(1);
}

int LineSurface::getEast() {
  cout << "Function should never be called" << endl;
  exit(1);
}

int LineSurface::getWest() {
  cout << "Function should never be called" << endl;
  exit(1);
}

float LineSurface::getHeightAt(Coordinate& point) {
  return cHorizontal ? (point.x - cLow) * cXSlope + cZ + BLOCK_HEIGHT
                     : (point.y - cLow) * cYSlope + cZ + BLOCK_HEIGHT;
}

bool LineSurface::isVisible() {
  return true;
}

void LineSurface::render(TextureSet& textures) {
  // Nothing to do.
}

bool LineSurface::within(int, int) {
  return false;
}

bool LineSurface::isCeiling() {
  return false;
}

bool LineSurface::isVerticallyAligned(Coordinate& location) {
  cout << "Calling it!" << endl;
  if (cHorizontal) {
    return location.y == cLocation + BLOCK_RADIUS && location.y > cLow - BLOCK_RADIUS && location.y < cHigh + BLOCK_RADIUS;
  } else {
    return location.x == cLocation + BLOCK_RADIUS && location.x > cLow - BLOCK_RADIUS && location.x < cHigh + BLOCK_RADIUS;
  }
}

bool LineSurface::isBelow(Point& point) {
  return false;
}

int LineSurface::getSymbol() {
  return PLAIN; // TODO?
}

bool LineSurface::isDynamic() {
  return false;
}

ImpactPoint* LineSurface::getRollingEvent(Coordinate& start, Coordinate& end, float ticks) {
  float mLocation = cHorizontal ? end.y : end.x;
  if (mLocation != cLocation + BLOCK_RADIUS) {
    ImpactPoint* mImpactPoint = new ImpactPoint(EVENT_LEAVE_FLOOR, 0.0f, start);
    mImpactPoint->setRelocationPoint(start);
    return mImpactPoint;
  }

  // TODO: Roll from end of the line

  return NULL;
}

ImpactPoint* LineSurface::getCollisionEvent(Coordinate& start, Coordinate& end, float) {
  return NULL;
}
