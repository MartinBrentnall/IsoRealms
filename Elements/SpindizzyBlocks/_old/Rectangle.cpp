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
#include "Rectangle.h"

// TODO: Rename class to better reflect purpose, e.g: EvenSurface
Rectangle::Rectangle(int l, int r, int t, int b, int z, int s, bool c, int x, int y, Condition* condition) {
  top = t;
  bottom = b;
  left = l;
  right = r;
  height = z;
  symbol = s;
  ceiling = c;
  xSlope = x;
  ySlope = y;
  cCondition = condition;
}

int Rectangle::getXSlopeAt(Coordinate& location) {
  return xSlope;
}

int Rectangle::getYSlopeAt(Coordinate& location) {
  return ySlope;
}

int Rectangle::getNorth() {
  return top;
}

int Rectangle::getSouth() {
  return bottom;
}

int Rectangle::getEast() {
  return right;
}

int Rectangle::getWest() {
  cout << "Height is nooooooooooooooow: " << getHeightAt(3, 5) << endl;
  return left;
}

float Rectangle::getHeightAt(Coordinate& point) {
  return getHeightAt(point.x, point.y);
}

float Rectangle::getHeightAt(float x, float y) {
  return xSlope * ((xSlope > 0 ? x - left : -(right + 1 - x)) + BLOCK_RADIUS) +
         ySlope * ((ySlope > 0 ? y - top : -(bottom + 1 - y)) + BLOCK_RADIUS) + height;
}

bool Rectangle::isVerticallyAligned(Coordinate& location) {
  float mX = location.x + BLOCK_RADIUS;
  float mY = location.y + BLOCK_RADIUS;
  return mX >= left && mX <= right + 1 && mY >= top && mY <= bottom + 1;
}

// TODO: This looks as though it doesn't take slopes into account
bool Rectangle::isBelow(Point& point) {
  return point.x >= left && point.x <= right && point.y >= top && point.y <= bottom && point.z >= height;
}

bool Rectangle::within(int x, int y) {
  return x >= left && x <= right && y >= top && y <= bottom;
}

void Rectangle::render(TextureSet& textures) {
  if (cCondition != NULL) {
    if (!(cCondition->isTrue())) {
      return;
    }
  }
  double xs = left - BLOCK_RADIUS;
  double ys = top - BLOCK_RADIUS;
  double xe = right + BLOCK_RADIUS;
  double ye = bottom + BLOCK_RADIUS;
  
  double xsys = height * BLOCK_HEIGHT;
  double xsye = height * BLOCK_HEIGHT;
  double xeye = height * BLOCK_HEIGHT;
  double xeys = height * BLOCK_HEIGHT;

  if (xSlope < 0) {
    xsys += (abs(xSlope) * BLOCK_HEIGHT * (xe - xs));
    xeys += (abs(xSlope) * BLOCK_HEIGHT * (xe - xs));
  } else {
    xsye += (abs(xSlope) * BLOCK_HEIGHT * (xe - xs));
    xeye += (abs(xSlope) * BLOCK_HEIGHT * (xe - xs));
  }

  if (ySlope < 0) {
    xsys += (abs(ySlope) * BLOCK_HEIGHT * (ye - ys));
    xsye += (abs(ySlope) * BLOCK_HEIGHT * (ye - ys));
  } else {
    xeys += (abs(ySlope) * BLOCK_HEIGHT * (ye - ys));
    xeye += (abs(ySlope) * BLOCK_HEIGHT * (ye - ys));
  }
   
  textures.setTexture(symbol);
//  glBindTexture(GL_TEXTURE_2D, 0);

  if (symbol == WATER) {
    glDisable(GL_CULL_FACE);
  }
  glBegin(GL_QUADS);
  if (ceiling) {
    glTexCoord2f(left,      top);    glVertex3f(xs, ye, xeys);
    glTexCoord2f(right + 1, top);    glVertex3f(xe, ye, xeye);
    glTexCoord2f(right + 1, bottom + 1); glVertex3f(xe, ys, xsye);
    glTexCoord2f(left,      bottom + 1); glVertex3f(xs, ys, xsys);
  } else {
    
    if (symbol == ARROW_SOUTH || textures.isBackwards(symbol)) {
      glTexCoord2f(left,      bottom + 1); glVertex3f(xe, ys, xsye);
      glTexCoord2f(left,      top);        glVertex3f(xe, ye, xeye);
      glTexCoord2f(right + 1, top);        glVertex3f(xs, ye, xeys);
      glTexCoord2f(right + 1, bottom + 1); glVertex3f(xs, ys, xsys);
    } else if (symbol == ARROW_WEST) {
      glTexCoord2f(bottom + 1, right + 1); glVertex3f(xe, ys, xsye);
      glTexCoord2f(top,        right + 1); glVertex3f(xe, ye, xeye);
      glTexCoord2f(top,        left);      glVertex3f(xs, ye, xeys);
      glTexCoord2f(bottom + 1, left);      glVertex3f(xs, ys, xsys);
    } else if (symbol == ARROW_EAST) {
      glTexCoord2f(top,        right + 1); glVertex3f(xe, ys, xsye);
      glTexCoord2f(bottom + 1, right + 1); glVertex3f(xe, ye, xeye);
      glTexCoord2f(bottom + 1, left);      glVertex3f(xs, ye, xeys);
      glTexCoord2f(top,        left);      glVertex3f(xs, ys, xsys);
    } else { // All symbols drawn north by default
      glTexCoord2f(right + 1, top);        glVertex3f(xe, ys, xsye);
      glTexCoord2f(right + 1, bottom + 1); glVertex3f(xe, ye, xeye);
      glTexCoord2f(left,      bottom + 1); glVertex3f(xs, ye, xeys);
      glTexCoord2f(left,      top);        glVertex3f(xs, ys, xsys);
/*      glColor3f(0.0, 1.0, 0.0); glTexCoord2f(right + 1, top);        glVertex3f(xe, ys, xsye);
      glColor3f(1.0, 1.0, 0.0); glTexCoord2f(right + 1, bottom + 1); glVertex3f(xe, ye, xeye);
      glColor3f(1.0, 0.0, 0.0); glTexCoord2f(left,      bottom + 1); glVertex3f(xs, ye, xeys);
      glColor3f(0.0, 0.0, 1.0); glTexCoord2f(left,      top);        glVertex3f(xs, ys, xsys);*/
    }
  }
  glEnd();
  if (symbol == WATER) {
    glEnable(GL_CULL_FACE);
  }
}

// Gameplay specific
bool Rectangle::isSwitch() {
  return symbol >= 0 && symbol <= 11; // TODO: SWITCH RANGE CONSTANTS
}

bool Rectangle::isCeiling() {
  return ceiling;
}

bool Rectangle::isDynamic() {
  return cCondition != NULL;
}

bool Rectangle::isVisible() {
  return cCondition->isTrue();
}

Coordinate* Rectangle::getBoundaryCrossingPoint(Coordinate& start, Coordinate& end, float *mLowestGradient) {
  *mLowestGradient = 2.0f;
  float mLineWidth = end.x - start.x;
  float mLineHeight = end.y - start.y;
  float mLeft = left - BLOCK_RADIUS;
  float mTop = top - BLOCK_RADIUS;
  float mRight = right + BLOCK_RADIUS;
  float mBottom = bottom + BLOCK_RADIUS;
  float mImpactX = 0.0f;
  float mImpactY = 0.0f;
  bool mXKnown = false;
  bool mYKnown = false;

  float mTempGradient = (mLeft - start.x) / mLineWidth;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mLeftYLocation = start.y + mLineHeight * mTempGradient;
    if (mLeftYLocation >= mTop && mLeftYLocation <= mBottom) {
      *mLowestGradient = mTempGradient;
      mImpactX = mLeft;
      mXKnown = true;
    }
  }

  mTempGradient = (mRight - start.x) / mLineWidth;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mRightYLocation = start.y + mLineHeight * mTempGradient;
    if (mRightYLocation >= mTop && mRightYLocation <= mBottom) {
      *mLowestGradient = mTempGradient;
      mImpactX = mRight;
      mXKnown = true;
    }
  }

  mTempGradient = (mTop - start.y) / mLineHeight;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mTopXLocation = start.x + mLineWidth * mTempGradient;
    if (mTopXLocation >= mLeft && mTopXLocation <= mRight) {
      *mLowestGradient = mTempGradient;
      mImpactY = mTop;
      mXKnown = false;
      mYKnown = true;
    }
  }

  mTempGradient = (mBottom - start.y) / mLineHeight;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
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

ImpactPoint* Rectangle::getRollingEvent(Coordinate& start, Coordinate& end, float ticks) {

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

ImpactPoint* Rectangle::getCollisionEvent(Coordinate& start, Coordinate& end, float ticks) {

  // First test if we hit the floor from the edge
  if (!isVerticallyAligned(start)) {
    float mGradient;
    Coordinate* mBoundaryEntryPoint = getBoundaryCrossingPoint(start, end, &mGradient);
    if (mBoundaryEntryPoint != NULL) {
      float mGroundZ = getHeightAt(*mBoundaryEntryPoint);
      if (mBoundaryEntryPoint->z <= mGroundZ + 0.01f && mBoundaryEntryPoint->z >= mGroundZ - STEP_HEIGHT) {
        ImpactPoint *mImpactPoint = new ImpactPoint(EVENT_HIT_FLOOR, mGradient, *mBoundaryEntryPoint);
        mImpactPoint->setRelocationPoint(*mBoundaryEntryPoint);
        return mImpactPoint;
      }
    }
  } else {
    // TODO: This code makes no consideration for the HEIGHT of a block!!  It definitely should do!
/*    if (start.x == left - BLOCK_RADIUS || start.x == right + BLOCK_RADIUS || start.y == top - BLOCK_RADIUS || start.y == bottom + BLOCK_RADIUS) {
      if (end.x > left - BLOCK_RADIUS && end.x < right + BLOCK_RADIUS && end.y > top - BLOCK_RADIUS && end.y < bottom + BLOCK_RADIUS) {
        ImpactPoint *mImpactPoint = new ImpactPoint(EVENT_HIT_FLOOR, 0.0f, start);
        mImpactPoint->setRelocationPoint(start);
        return mImpactPoint;
      }
    }*/
  }

  // Second, test if we hit the floor from above (fallen and landed on it).
  float mStartHeight = getHeightAt(start);
  float mEndHeight = getHeightAt(end);
  if (start.z > mStartHeight != end.z > mEndHeight && start.z > mStartHeight) {
    float mGroundSlope = mEndHeight - mStartHeight;
    float mPlayerSlope = end.z - start.z;
    Coordinate *mImpactLocation;
    float mEndHeightModified = mEndHeight - (start.z - end.z);
    float mGradient = (start.z - mStartHeight) / (mEndHeightModified - mStartHeight);
    float mXImpact = start.x + (end.x - start.x) * mGradient;
    float mYImpact = start.y + (end.y - start.y) * mGradient;
    float mZImpact = start.z + (end.z - start.z) * mGradient;
    mImpactLocation = new Coordinate(mXImpact, mYImpact, mZImpact);
    if (isVerticallyAligned(*mImpactLocation)) {
      ImpactPoint *mImpactPoint = new ImpactPoint(EVENT_HIT_FLOOR, mGradient, *mImpactLocation);
      mImpactPoint->setRelocationPoint(*mImpactLocation);
      return mImpactPoint;
    }
  }

  // No event
  return NULL;
}
