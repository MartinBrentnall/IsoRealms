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
#include "WallSurface.h"

WallSurface::WallSurface(int x, int y, WallColumn::WallSection wallSegment, int facing, int symbol) {
  cX = x;
  cY = y;
  cZ = wallSegment.la;
  cLength = 1;
  cHeight = wallSegment.hb - wallSegment.lb;
  cFacing = facing;
  cBottomSlope = wallSegment.la - wallSegment.lb;
  cTopSlope = wallSegment.ha - wallSegment.hb;
  cSymbol = symbol;
  cCondition = wallSegment.getCondition();
}

bool WallSurface::attemptJoin(int x, int y, WallColumn::WallSection wall) {
  int mMin = (cLength * cBottomSlope) + cZ;
  int mMax = (cLength * cTopSlope) + cZ + cHeight;
  bool mHorizontal = cFacing == EAST || cFacing == WEST;

  if (mHorizontal ? x != cX : y != cY) {
    return false;
  }

   Condition* mColumnCondition = wall.getCondition();
   if (mColumnCondition != NULL && cCondition != NULL && !mColumnCondition->equals(*cCondition)) {
     return false;
   } else if ((cCondition == NULL) != (mColumnCondition == NULL)) {
     return false;
   } 

  if (wall.lb == mMin && wall.la == mMin + cBottomSlope && wall.hb == mMax && wall.ha == mMax + cTopSlope) {
    int mCurrent = mHorizontal ? cY : cX;
    int mRequired = mHorizontal ? y : x;

    if (mCurrent + cLength == mRequired) {
      cLength++;
      return true;
    }
  }
  return false;
}

void WallSurface::render(TextureSet& textures) {
  if (cCondition != NULL) {
    if (!(cCondition->isTrue())) {
      return;
    }
  }
  float mBlockRadius = BLOCK_RADIUS;
  double mFromX = cX + (cFacing == EAST ? mBlockRadius : -mBlockRadius);
  double mFromY = cY + (cFacing == NORTH ? mBlockRadius : -mBlockRadius); 
  double mFromZ = (cZ - 1) * BLOCK_HEIGHT;
  double mToX = (cFacing == WEST || cFacing == EAST) ? (mFromX) : cX - mBlockRadius + cLength;
  double mToY = (cFacing == SOUTH || cFacing == NORTH) ? (mFromY) : cY - mBlockRadius + cLength;
  double mHighStartSlopeZ = (cZ + cHeight - 1) * BLOCK_HEIGHT;
  double mHighEndSlopeZ =  ((cZ + cHeight - 1) + cTopSlope * cLength) * BLOCK_HEIGHT;
  double mHighStartSlopeTexture = cZ + cHeight;
  double mHighEndSlopeTexture = (cZ + cHeight) + cTopSlope * cLength;

  textures.setWallTexture(cSymbol, cFacing);
  double mEdgeWidth = BLOCK_HEIGHT * 0.5;
  glBegin(GL_QUADS);

  if (cSymbol == ICE) { // TODO: Make generic (in textures or something)
    if (cFacing == EAST || cFacing == SOUTH) {
      glTexCoord2f(cX,           cZ);                     glVertex3f(mFromX, mFromY, mFromZ);
      glTexCoord2f(cX + cLength, cZ);                     glVertex3f(mToX,   mToY,   mFromZ);
      glTexCoord2f(cX + cLength, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glTexCoord2f(cX,           mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
    } else {
      glTexCoord2f(cX,           mHighStartSlopeTexture); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
      glTexCoord2f(cX + cLength, mHighEndSlopeTexture);   glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glTexCoord2f(cX + cLength, cZ);                     glVertex3f(mToX,   mToY,   mFromZ);
      glTexCoord2f(cX,           cZ);                     glVertex3f(mFromX, mFromY, mFromZ);
    }
  } else {
    if (cFacing == EAST || cFacing == SOUTH) {
      if (mHighEndSlopeZ - mFromZ > 1.0 || mHighStartSlopeZ - mFromZ > 1.0) {
        glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
        glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
        glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
        glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
      }
      glTexCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
      glTexCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
      glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
      glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
      glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
      glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
      glTexCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glTexCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);

/*      if (mHighEndSlopeZ - mFromZ > 1.0 || mHighStartSlopeZ - mFromZ > 1.0) {
        glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
        glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
        glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
        glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
      }
      glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
      glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
      glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
      glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
      glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
      glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
      glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);*/
    } else {
      if (mHighEndSlopeZ - mFromZ > 1.0 || mHighStartSlopeZ - mFromZ > 1.0) {
        glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
        glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
        glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
        glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
      }
      glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
      glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
      glTexCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
      glTexCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
      glTexCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
      glTexCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
      glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);

/*      if (mHighEndSlopeZ - mFromZ > 1.0 || mHighStartSlopeZ - mFromZ > 1.0) {
        glColor3f(0.0f, 1.0f, 0.0f); glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);
        glColor3f(1.0f, 0.0f, 0.0f); glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
        glColor3f(1.0f, 0.0f, 0.0f); glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
        glColor3f(0.0f, 1.0f, 0.0f); glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
      }
      glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           0.25); glVertex3f(mFromX, mFromY, mFromZ + mEdgeWidth);
      glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 0.25); glVertex3f(mToX,   mToY,   mFromZ + mEdgeWidth);
      glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 0.0);  glVertex3f(mToX,   mToY,   mFromZ);
      glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           0.0);  glVertex3f(mFromX, mFromY, mFromZ);
      glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           1.00); glVertex3f(mFromX, mFromY, mHighStartSlopeZ);
      glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 1.00); glVertex3f(mToX,   mToY,   mHighEndSlopeZ);
      glColor3f(1.0f, 0.0f, 0.0f);       glTexCoord2f(cX + cLength, 0.75); glVertex3f(mToX,   mToY,   mHighEndSlopeZ - mEdgeWidth);
      glColor3f(0.0f, 1.0f, 0.0f);       glTexCoord2f(cX,           0.75); glVertex3f(mFromX, mFromY, mHighStartSlopeZ - mEdgeWidth);*/
    }
  }
  glEnd();
}

bool WallSurface::isDynamic() {
  return cCondition != NULL;
}

bool WallSurface::occupies(int x, int y) {
  int mXEnd = cX + (cFacing == SOUTH || cFacing == NORTH ? cLength - 1 : 0);
  int mYEnd = cY + (cFacing == WEST  || cFacing == EAST  ? cLength - 1 : 0);
  return x >= cX && x <= mXEnd && y >= cY && y <= mYEnd;
}

ImpactPoint* WallSurface::getIntersectionGradient(Rect rect, Coordinate& start, Coordinate& end) {
  float mLowestGradient = 2.0f;
  float mLineWidth = end.x - start.x;
  float mLineHeight = end.y - start.y;
  float mLeft = rect.getLeft();
  float mTop = rect.getTop();
  float mRight = rect.getRight();
  float mBottom = rect.getBottom();
  float mImpactX = 0.0f;
  float mImpactY = 0.0f;
  bool mXKnown = false;
  bool mYKnown = false;

  if (cFacing != EAST && start.x < end.x) {
    float mTempGradient = (mLeft - start.x) / mLineWidth;
    if (mTempGradient > 0.0f && mTempGradient <= mLowestGradient) {
      float mLeftYLocation = start.y + mLineHeight * mTempGradient;
      if (mLeftYLocation >= mTop && mLeftYLocation <= mBottom) {
        mLowestGradient = mTempGradient;
        mImpactX = mLeft;
        mXKnown = true;
      }
    }
  }

  if (cFacing != WEST && start.x > end.x) {
    float mTempGradient = (mRight - start.x) / mLineWidth;
    if (mTempGradient > 0.0f && mTempGradient <= mLowestGradient) {
      float mRightYLocation = start.y + mLineHeight * mTempGradient;
      if (mRightYLocation >= mTop && mRightYLocation <= mBottom) {
        mLowestGradient = mTempGradient;
        mImpactX = mRight;
        mXKnown = true;
      }
    }
  }

  if (cFacing != NORTH && start.y < end.y) {
    float mTempGradient = (mTop - start.y) / mLineHeight;
    if (mTempGradient > 0.0f && mTempGradient <= mLowestGradient) {
      float mTopXLocation = start.x + mLineWidth * mTempGradient;
      if (mTopXLocation >= mLeft && mTopXLocation <= mRight) {
        mLowestGradient = mTempGradient;
        mImpactY = mTop;
        mXKnown = false;
        mYKnown = true;
      }
    }
  }

  if (cFacing != SOUTH && start.y > end.y) {
    float mTempGradient = (mBottom - start.y) / mLineHeight;
    if (mTempGradient > 0.0f && mTempGradient <= mLowestGradient) {
      float mBottomXLocation = start.x + mLineWidth * mTempGradient;
      if (mBottomXLocation >= mLeft && mBottomXLocation <= mRight) {
        mLowestGradient = mTempGradient;
        mImpactY = mBottom;
        mXKnown = false;
        mYKnown = true;
      }
    }
  }

  if (mLowestGradient <= 1.0f) {
    float mMovementZ = end.z - start.z;
    if (!mXKnown) {
      mImpactX = start.x + mLineWidth * mLowestGradient;
    }
    if (!mYKnown) {
      mImpactY = start.y + mLineHeight * mLowestGradient;
    }
    float mImpactZ = start.z + mMovementZ * mLowestGradient;
    Coordinate mLeaveEnter(mImpactX, mImpactY, mImpactZ);
    int mEventType = getCollisionEventType();
    ImpactPoint *mImpactPoint = new ImpactPoint(mEventType, mLowestGradient, mLeaveEnter);
    return mImpactPoint;
  }
  return NULL;
}

float WallSurface::getHeightAt(Coordinate& location) {
  float mLocationAlong;
  float mWallStart;
  if (cFacing == WEST || cFacing == EAST) {
    mLocationAlong = location.y;
    mWallStart = cY - BLOCK_RADIUS;
  } else {
    mLocationAlong = location.x;
    mWallStart = cX - BLOCK_RADIUS;
  }
  float mSlopeHeight = (mLocationAlong - mWallStart) * cTopSlope;
  return (cZ + cHeight + mSlopeHeight) - 1.0f;
}

ImpactPoint* WallSurface::getSlideEvent(Coordinate& start, Coordinate& end) {
  float mPlayerStart;
  float mPlayerEnd;
  float mWallStart;
  float mWallEnd;

  float mStartHeight = getHeightAt(start);
  float mEndHeight = getHeightAt(end);

  // Test to see if we've gone above or below the wall
  if (start.z > mStartHeight - STEP_HEIGHT != end.z > mEndHeight - STEP_HEIGHT) {
    // TODO: Needs to return correct gradient and intermediatary point
    ImpactPoint* mSlideEvent = new ImpactPoint(EVENT_LEAVE_WALL, 1.0f, end);
    mSlideEvent->setRelocationPoint(end);
    return mSlideEvent;
  }

  // Test to see if we're no longer in line with the wall
  if (cFacing == WEST || cFacing == EAST) {
    mPlayerStart = start.y;
    mPlayerEnd = end.y;
    mWallStart = cY - BLOCK_RADIUS;
  } else {
    mPlayerStart = start.x;
    mPlayerEnd = end.x;
    mWallStart = cX - BLOCK_RADIUS;
  }
  mWallEnd = mWallStart + cLength;
  float mMoveLength = mPlayerEnd - mPlayerStart;
  if (mPlayerStart > mPlayerEnd) {
    float mGradient = (mWallStart - mPlayerStart) / mMoveLength;
    if (mGradient >= 0.0f && mGradient <= 1.0f) {
      float mSlideImpact = mPlayerStart + mMoveLength * mGradient;
      float mMovementZ = end.z - start.z;
      float mImpactZ = start.z + mMovementZ * mGradient;
      Coordinate* mCoordinate = cFacing == WEST || cFacing == EAST ? new Coordinate(start.x, mSlideImpact, mImpactZ)
                                                                   : new Coordinate(mSlideImpact, start.y, mImpactZ);
      ImpactPoint* mSlideEvent = new ImpactPoint(EVENT_LEAVE_WALL, mGradient, *mCoordinate);
      mSlideEvent->setRelocationPoint(*mCoordinate);
      return mSlideEvent;
    }
  } else if (mPlayerStart < mPlayerEnd) {
    float mGradient = (mWallEnd - mPlayerStart) / mMoveLength;
    if (mGradient >= 0.0f && mGradient <= 1.0f) {
      float mSlideImpact = mPlayerStart + mMoveLength * mGradient;
      float mMovementZ = end.z - start.z;
      float mImpactZ = start.z + mMovementZ * mGradient;
      Coordinate* mCoordinate = cFacing == WEST || cFacing == EAST ? new Coordinate(start.x, mSlideImpact, mImpactZ)
                                                                   : new Coordinate(mSlideImpact, start.y, mImpactZ);
      ImpactPoint* mSlideEvent = new ImpactPoint(EVENT_LEAVE_WALL, mGradient, *mCoordinate);
      mSlideEvent->setRelocationPoint(*mCoordinate);
      return mSlideEvent;
    }
  } else {
    cout << endl;
  }

//  cout << mWallStart << " - " << mWallEnd << " ................... " << mPlayerStart << " - " << mPlayerEnd << endl;
  return NULL;
}

int WallSurface::getCollisionEventType() {
  switch (cFacing) {
    case NORTH: return EVENT_HIT_NORTH_WALL;
    case SOUTH: return EVENT_HIT_SOUTH_WALL;
    case EAST: return EVENT_HIT_EAST_WALL;
    case WEST: return EVENT_HIT_WEST_WALL;
  }
}

ImpactPoint* WallSurface::getCollisionEvent(Coordinate& start, Coordinate& end) {
  float mLeft;
  float mRight;
  float mTop;
  float mBottom;

  if (cFacing == WEST) {
    mRight = cX - BLOCK_RADIUS;
    mLeft = mRight - CRAFT_RADIUS;
    mTop = cY - BLOCK_RADIUS;
    mBottom = mTop + cLength;
  } else if (cFacing == EAST) {
    mLeft = cX + BLOCK_RADIUS;
    mRight = mLeft + CRAFT_RADIUS;
    mTop = cY - BLOCK_RADIUS;
    mBottom = mTop + cLength;
  } else if (cFacing == SOUTH) {
    mLeft = cX - BLOCK_RADIUS;
    mRight = mLeft + cLength;
    mBottom = cY - BLOCK_RADIUS;
    mTop = mBottom - CRAFT_RADIUS;
  } else { // North
    mLeft = cX - BLOCK_RADIUS;
    mRight = mLeft + cLength;
    mTop = cY + BLOCK_RADIUS;
    mBottom = mTop + CRAFT_RADIUS;
  } 
  Rect mRect(mTop, mLeft, mBottom, mRight);

  float mStartHeight = getHeightAt(start) - STEP_HEIGHT;
  float mEndHeight = getHeightAt(end) - STEP_HEIGHT;

  // This section tests whether we hit the "top" of a wall (e.g. falling off a platform or running alongside the wall)
  if (start.z > mStartHeight != end.z > mEndHeight) {
    float mGroundSlope = mEndHeight - mStartHeight;
    float mPlayerSlope = end.z - start.z;
    Coordinate *mImpactLocation;
    float mEndHeightModified = mEndHeight - (start.z - end.z);
    float mGradient = (start.z - mStartHeight) / (mEndHeightModified - mStartHeight);
    float mXImpact = start.x + (end.x - start.x) * mGradient;
    float mYImpact = start.y + (end.y - start.y) * mGradient;
    float mZImpact = start.z + (end.z - start.z) * mGradient;
    mImpactLocation = new Coordinate(mXImpact, mYImpact, mZImpact);
    if (mRect.contains(*mImpactLocation)) {
      int mEventType = getCollisionEventType();
      ImpactPoint *mImpactPoint = new ImpactPoint(mEventType, mGradient, *mImpactLocation);
      Coordinate *mRelocationPoint = new Coordinate(*mImpactLocation);
      switch (cFacing) {
        case WEST: mRelocationPoint->x = mLeft - 0.00001f; break; // TODO: These 0.00001f values seem "hackish"
        case EAST: mRelocationPoint->x = mRight + 0.00001f; break;
        case SOUTH: mRelocationPoint->y = mTop - 0.00001f; break;
        case NORTH: mRelocationPoint->y = mBottom + 0.00001f; break;
      }
      mImpactPoint->setRelocationPoint(*mRelocationPoint);
      return mImpactPoint;
    }
  }
  
  // This section tests whether we hit the wall directly
  ImpactPoint *mImpactPoint = getIntersectionGradient(mRect, start, end);
  if (mImpactPoint != NULL) {
    Coordinate mLocation = mImpactPoint->getLocation();
    float mWallHeightAtImpact = getHeightAt(mLocation);
    if (mLocation.z >= (cZ - 1) - CRAFT_HEIGHT && mLocation.z < mWallHeightAtImpact - STEP_HEIGHT) {
      Coordinate *mRelocationPoint = new Coordinate(mLocation);
      switch (cFacing) {
        case WEST: mRelocationPoint->x = mLeft - 0.00001f; break; // TODO: These 0.00001f values seem "hackish"
        case EAST: mRelocationPoint->x = mRight + 0.00001f; break;
        case SOUTH: mRelocationPoint->y = mTop - 0.00001f; break;
        case NORTH: mRelocationPoint->y = mBottom + 0.00001f; break;
      }
      mImpactPoint->setRelocationPoint(*mRelocationPoint);
      return mImpactPoint;
    }
  }
  return NULL;
}

int WallSurface::getFacing() {
  return cFacing;
}

void WallSurface::debug() {
  cout << "X:" << cX << "  Y:" << cY << "  Z:" << cZ << "  L:" << cLength << "  D: " << cFacing << endl;
}
