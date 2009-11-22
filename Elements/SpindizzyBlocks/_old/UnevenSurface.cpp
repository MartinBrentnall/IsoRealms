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
#include "UnevenSurface.h"

UnevenSurface::UnevenSurface(int x, int y, int z, int northCorner, int eastCorner, int southCorner, int westCorner, bool split, Condition* condition) {
  cX = x;
  cY = y;
  cZ = z;
  cNorthCorner = northCorner;
  cEastCorner = eastCorner;
  cSouthCorner = southCorner;
  cWestCorner = westCorner;
  cSplit = split;
  cCondition = condition;
}

int UnevenSurface::getSlopeStartFor(int corner, int edgeA, int edgeB) {
  return corner < edgeA && corner < edgeB ? corner : max(corner, edgeA, edgeB) - (abs(corner - edgeA) + abs(corner - edgeB));
}

/**
 * The idea here is that we calculate where the bottom of the slope WOULD be
 * if triangle at the co-ordinate was expanded to the entire block
 */
int UnevenSurface::getSlopeStartAt(Coordinate& location) {
  if (cSplit) {
    if (location.x + location.y > cX + cY) {
      return getSlopeStartFor(cNorthCorner, cEastCorner, cWestCorner);
    } else {
      return getSlopeStartFor(cSouthCorner, cEastCorner, cWestCorner);
    }
  } else {
    if (location.x - cX > location.y - cY) {
      return getSlopeStartFor(cEastCorner, cNorthCorner, cSouthCorner);
    } else {
      return getSlopeStartFor(cWestCorner, cNorthCorner, cSouthCorner);
    }
  }
  return 0;
}

int UnevenSurface::getXSlopeAt(Coordinate& location) {
  if (cSplit) {
    if (location.x + location.y > cX + cY) {
      return cNorthCorner - cWestCorner;
    } else {
      return cEastCorner - cSouthCorner;
    }
  } else {
    if (location.x - cX > location.y - cY) {
      return cEastCorner - cSouthCorner;
    } else {
      return cNorthCorner - cWestCorner;
    }
  }
  return 0;
}

int UnevenSurface::getYSlopeAt(Coordinate& location) {
  if (cSplit) {
    if (location.x + location.y > cX + cY) {
      return cNorthCorner - cEastCorner;
    } else {
      return cWestCorner - cSouthCorner;
    }
  } else {
    if (location.x - cX > location.y - cY) {
      return cNorthCorner - cEastCorner;
    } else {
      return cWestCorner - cSouthCorner;
    }
  }
}

int UnevenSurface::getNorth() {
  return cY;
}

int UnevenSurface::getSouth() {
  return cY;
}

int UnevenSurface::getEast() {
  return cX;
}

int UnevenSurface::getWest() {
  return cX;
}

bool UnevenSurface::within(int x, int y) {
  return x == cX && y == cY;
}

bool UnevenSurface::isCeiling() {
  return false;
}

float UnevenSurface::getHeightAt(Coordinate& point) {
  float xSlope = getXSlopeAt(point);
  float ySlope = getYSlopeAt(point);
  int mSlopeStart = getSlopeStartAt(point);
  return (xSlope * ((xSlope > 0 ? point.x - cX : -(cX + 1 - point.x)) + BLOCK_RADIUS)) +
         (ySlope * ((ySlope > 0 ? point.y - cY : -(cY + 1 - point.y)) + BLOCK_RADIUS)) + cZ + mSlopeStart;
}

bool UnevenSurface::isVerticallyAligned(Coordinate& location) {
  return false; // TODO
}

bool UnevenSurface::isBelow(Point& point) {
  return point.x == cX && point.y == cY && point.z >= cZ;
}

void UnevenSurface::render(TextureSet& textures) {
  if (cCondition != NULL) {
    if (!cCondition->isTrue()) {
      return;
    }
  }

  textures.setTexture(PLAIN_SPLIT);
  if (cSplit == HORIZONTAL) {
    renderAsHorizontalSplit();
  } else { // cSplit == VERTICAL
    renderAsVerticalSplit();
  }
}

// 0   0
// 0   1
// 1   1
// 1   0
void UnevenSurface::renderAsVerticalSplit() {
  double nH = cNorthCorner * BLOCK_HEIGHT;
  double sH = cSouthCorner * BLOCK_HEIGHT;
  double eH = cEastCorner * BLOCK_HEIGHT;
  double wH = cWestCorner * BLOCK_HEIGHT;
  double z = cZ * BLOCK_HEIGHT;

  float mNegBlockRadius = 0 - BLOCK_RADIUS;

  glBegin(GL_TRIANGLES);
  glTexCoord2f(1.0, 1.0); glVertex3f(mNegBlockRadius + cX, BLOCK_RADIUS    + cY, wH + z);
  glTexCoord2f(0.0, 1.0); glVertex3f(mNegBlockRadius + cX, mNegBlockRadius + cY, sH + z);
  glTexCoord2f(1.0, 0.0); glVertex3f(BLOCK_RADIUS    + cX, BLOCK_RADIUS    + cY, nH + z);

  glTexCoord2f(0.0, 1.0); glVertex3f(mNegBlockRadius + cX, mNegBlockRadius + cY, sH + z);
  glTexCoord2f(0.0, 0.0); glVertex3f(BLOCK_RADIUS    + cX, mNegBlockRadius + cY, eH + z);
  glTexCoord2f(1.0, 0.0); glVertex3f(BLOCK_RADIUS    + cX, BLOCK_RADIUS    + cY, nH + z);
  glEnd();
}

void UnevenSurface::renderAsHorizontalSplit() {
  double nH = cNorthCorner * BLOCK_HEIGHT;
  double sH = cSouthCorner * BLOCK_HEIGHT;
  double eH = cEastCorner * BLOCK_HEIGHT;
  double wH = cWestCorner * BLOCK_HEIGHT;
  double z = cZ * BLOCK_HEIGHT;

  float mNegBlockRadius = 0 - BLOCK_RADIUS;

  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0, 1.0); glVertex3f(mNegBlockRadius + cX, BLOCK_RADIUS    + cY, wH + z);
  glTexCoord2f(1.0, 0.0); glVertex3f(BLOCK_RADIUS    + cX, mNegBlockRadius + cY, eH + z);
  glTexCoord2f(1.0, 1.0); glVertex3f(BLOCK_RADIUS    + cX, BLOCK_RADIUS    + cY, nH + z);

  glTexCoord2f(0.0, 1.0); glVertex3f(mNegBlockRadius + cX, BLOCK_RADIUS    + cY, wH + z);
  glTexCoord2f(0.0, 0.0); glVertex3f(mNegBlockRadius + cX, mNegBlockRadius + cY, sH + z);
  glTexCoord2f(1.0, 0.0); glVertex3f(BLOCK_RADIUS    + cX, mNegBlockRadius + cY, eH + z);
  glEnd();
}

bool UnevenSurface::isDynamic() {
  return cCondition != NULL;
}

bool UnevenSurface::isVisible() {
  return cCondition->isTrue();
}

Coordinate* UnevenSurface::getBoundaryCrossingPoint(Coordinate& start, Coordinate& end, float *mLowestGradient) {
  *mLowestGradient = 2.0f;
  float mLineWidth = end.x - start.x;
  float mLineHeight = end.y - start.y;
  float mLeft = cX - BLOCK_RADIUS;
  float mTop = cY - BLOCK_RADIUS;
  float mRight = cX + BLOCK_RADIUS;
  float mBottom = cY + BLOCK_RADIUS;
  float mImpactX = 0.0f;
  float mImpactY = 0.0f;
  bool mXKnown = false;
  bool mYKnown = false;

  float mTempGradient = (mLeft - start.x) / mLineWidth;
  if (mTempGradient >= 0.0f && mTempGradient <= *mLowestGradient) {
    float mLeftYLocation = start.y + mLineHeight * mTempGradient;
    if (mLeftYLocation >= mTop && mLeftYLocation <= mBottom) {
      *mLowestGradient = mTempGradient;
      mImpactX = mLeft;
      mXKnown = true;
    }
  }

  mTempGradient = (mRight - start.x) / mLineWidth;
  if (mTempGradient >= 0.0f && mTempGradient <= *mLowestGradient) {
    float mRightYLocation = start.y + mLineHeight * mTempGradient;
    if (mRightYLocation >= mTop && mRightYLocation <= mBottom) {
      *mLowestGradient = mTempGradient;
      mImpactX = mRight;
      mXKnown = true;
    }
  }

  mTempGradient = (mTop - start.y) / mLineHeight;
  if (mTempGradient >= 0.0f && mTempGradient <= *mLowestGradient) {
    float mTopXLocation = start.x + mLineWidth * mTempGradient;
    if (mTopXLocation >= mLeft && mTopXLocation <= mRight) {
      *mLowestGradient = mTempGradient;
      mImpactY = mTop;
      mXKnown = false;
      mYKnown = true;
    }
  }

  mTempGradient = (mBottom - start.y) / mLineHeight;
  if (mTempGradient >= 0.0f && mTempGradient <= *mLowestGradient) {
    float mBottomXLocation = start.x + mLineWidth * mTempGradient;
    if (mBottomXLocation >= mLeft && mBottomXLocation <= mRight) {
      *mLowestGradient = mTempGradient;
      mImpactY = mBottom;
      mXKnown = false;
      mYKnown = true;
    }
  }

  float mMovementZ = end.z - start.z;
  if (*mLowestGradient <= 1.0f) {
    if (!mXKnown) {
      mImpactX = start.x + mLineWidth * *mLowestGradient;
    }
    if (!mYKnown) {
      mImpactY = start.y + mLineHeight * *mLowestGradient;
    }
    float mImpactZ = start.z + mMovementZ * *mLowestGradient;
    return new Coordinate(mImpactX, mImpactY, mImpactZ);
  }

  // Line doesn't cross boundary
  return NULL;
}

ImpactPoint* UnevenSurface::getRollingEvent(Coordinate& start, Coordinate& end, float ticks) {

  // Test if we left the floor
  if (isVerticallyAligned(start)) {
    float mGradient;
    Coordinate* mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient);
    if (mLeavePoint != NULL) {
      ImpactPoint *mImpactPoint = new ImpactPoint(EVENT_LEAVE_FLOOR, mGradient, *mLeavePoint);
      mImpactPoint->setRelocationPoint(*mLeavePoint);
      return mImpactPoint;
    }
  }

  // No event
  return NULL;
}

ImpactPoint* UnevenSurface::getCollisionEvent(Coordinate& start, Coordinate& end, float ticks) {

  // The first test is whether we step onto the lift from outside it.
  if (!isVerticallyAligned(start)) {
    float mGradient;
    Coordinate* mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient);
    if (mEnterPoint != NULL) {
      float mGroundZ = getHeightAt(*mEnterPoint);
      if (mEnterPoint->z <= mGroundZ + 0.01f && mEnterPoint->z >= mGroundZ - STEP_HEIGHT) {
        ImpactPoint *mImpactPoint = new ImpactPoint(EVENT_HIT_FLOOR, mGradient, *mEnterPoint);
        mImpactPoint->setRelocationPoint(*mEnterPoint);
        return mImpactPoint;
      }
    }
  }

  // Second, test if we hit the floor from above (fallen and landed on it).
  // TODO: Need to split the line at the point where it crosses the split and test the two separately
//   float mStartHeight = getHeightAt(start);
//   float mEndHeight = getHeightAt(end);
//   if (start.z > mStartHeight != end.z > mEndHeight && start.z > mStartHeight) {
//     float mGroundSlope = mEndHeight - mStartHeight;
//     float mPlayerSlope = end.z - start.z;
//     Coordinate *mImpactLocation;
//     float mEndHeightModified = mEndHeight - (start.z - end.z);
//     float mGradient = (start.z - mStartHeight) / (mEndHeightModified - mStartHeight);
//     float mXImpact = start.x + (end.x - start.x) * mGradient;
//     float mYImpact = start.y + (end.y - start.y) * mGradient;
//     float mZImpact = start.z + (end.z - start.z) * mGradient;
//     mImpactLocation = new Coordinate(mXImpact, mYImpact, mZImpact);
//     if (isVerticallyAligned(*mImpactLocation)) {
//       ImpactPoint *mImpactPoint = new ImpactPoint(EVENT_HIT_FLOOR, mGradient, *mImpactLocation);
//       mImpactPoint->setRelocationPoint(*mImpactLocation);
//       return mImpactPoint;
//     }
//   }

  return NULL; // TODO: Fill me in!
}

