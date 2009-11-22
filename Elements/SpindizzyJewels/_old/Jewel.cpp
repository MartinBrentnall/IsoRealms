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
#include "Jewel.h"

int Jewel::cFrameDisplayList = 0;
int Jewel::cPanelDisplayList = 0;


Jewel::Jewel(Point location) {
  setLocation(location);
  long int mRandomNumber = random();
  cColourChannel = mRandomNumber % 3 == 0 ? &cCurrentColour.cRed 
                : (mRandomNumber % 3 == 1 ? &cCurrentColour.cGreen
                                          : &cCurrentColour.cBlue);
  cColourUp = mRandomNumber % 2 == 1 ? true : false;
  cCollected = false;
  cCurrentColour.cRed = cColourChannel == &cCurrentColour.cRed ? (mRandomNumber % 10000) / 10000.0 : 0.0;
  cCurrentColour.cGreen = cColourChannel == &cCurrentColour.cGreen ? (mRandomNumber % 10000) / 10000.0 : 0.0;
  cCurrentColour.cBlue = cColourChannel == &cCurrentColour.cBlue ? (mRandomNumber % 10000) / 10000.0 : 0.0;
}

void Jewel::initDisplayLists() {
  float mRadius = BLOCK_RADIUS;
  float mLineWidth = 0.05;
  Coordinate mBottom(0.0, 0.0, 0.0);
  Coordinate mTop(0.0, 0.0,  BLOCK_HEIGHT * 1.4);
  Coordinate mNorth(0.0, mRadius, BLOCK_HEIGHT * 0.7);
  Coordinate mEast(mRadius, 0.0, BLOCK_HEIGHT * 0.7);
  Coordinate mSouth(0.0, -mRadius, BLOCK_HEIGHT * 0.7);
  Coordinate mWest(-mRadius, 0.0, BLOCK_HEIGHT * 0.7);

  cPanelDisplayList = glGenLists(1);
  glNewList(cPanelDisplayList, GL_COMPILE);
  glBegin(GL_TRIANGLES);
  renderInnerTriangle(mBottom, mNorth, mEast, mLineWidth);
  renderInnerTriangle(mBottom, mEast, mSouth, mLineWidth);
  renderInnerTriangle(mBottom, mSouth, mWest, mLineWidth);
  renderInnerTriangle(mBottom, mWest, mNorth, mLineWidth);
  renderInnerTriangle(mTop, mNorth, mWest, mLineWidth);
  renderInnerTriangle(mTop, mWest, mSouth, mLineWidth);
  renderInnerTriangle(mTop, mSouth, mEast, mLineWidth);
  renderInnerTriangle(mTop, mEast, mNorth, mLineWidth);
  glEnd();
  glEndList();

  cFrameDisplayList = glGenLists(1);
  glNewList(cFrameDisplayList, GL_COMPILE);
  glBegin(GL_TRIANGLES);
  renderOuterTriangle(mBottom, mNorth, mEast, mLineWidth);
  renderOuterTriangle(mBottom, mEast, mSouth, mLineWidth);
  renderOuterTriangle(mBottom, mSouth, mWest, mLineWidth);
  renderOuterTriangle(mBottom, mWest, mNorth, mLineWidth);
  renderOuterTriangle(mTop, mNorth, mWest, mLineWidth);
  renderOuterTriangle(mTop, mWest, mSouth, mLineWidth);
  renderOuterTriangle(mTop, mSouth, mEast, mLineWidth);
  renderOuterTriangle(mTop, mEast, mNorth, mLineWidth);
  glEnd();
  glEndList();
}

void Jewel::setLocation(Point location) {
  cLocation.x = location.x;
  cLocation.y = location.y;
  cLocation.z = location.z;
}

bool Jewel::isCollected(Coordinate& start, Coordinate& end) {
  if (!cCollected) {
    float mLowestGradient = 2.0f;
    float mLineWidth = end.x - start.x;
    float mLineHeight = end.y - start.y;
    float mLeft = cLocation.x - BLOCK_RADIUS;
    float mTop = cLocation.y - BLOCK_RADIUS;
    float mRight = cLocation.x + BLOCK_RADIUS;
    float mBottom = cLocation.y + BLOCK_RADIUS;
  
    // The first test is whether we step onto the lift from outside it.
    float mTempGradient = (mLeft - start.x) / mLineWidth;
    if (mTempGradient >= 0.0f && mTempGradient <= mLowestGradient) {
      float mLeftYLocation = start.y + mLineHeight * mTempGradient;
      if (mLeftYLocation >= mTop && mLeftYLocation <= mBottom) {
        mLowestGradient = mTempGradient;
      }
    }
  
    mTempGradient = (mRight - start.x) / mLineWidth;
    if (mTempGradient >= 0.0f && mTempGradient <= mLowestGradient) {
      float mRightYLocation = start.y + mLineHeight * mTempGradient;
      if (mRightYLocation >= mTop && mRightYLocation <= mBottom) {
        mLowestGradient = mTempGradient;
      }
    }
  
    mTempGradient = (mTop - start.y) / mLineHeight;
    if (mTempGradient >= 0.0f && mTempGradient <= mLowestGradient) {
      float mTopXLocation = start.x + mLineWidth * mTempGradient;
      if (mTopXLocation >= mLeft && mTopXLocation <= mRight) {
        mLowestGradient = mTempGradient;
      }
    }
  
    mTempGradient = (mBottom - start.y) / mLineHeight;
    if (mTempGradient >= 0.0f && mTempGradient <= mLowestGradient) {
      float mBottomXLocation = start.x + mLineWidth * mTempGradient;
      if (mBottomXLocation >= mLeft && mBottomXLocation <= mRight) {
        mLowestGradient = mTempGradient;
      }
    }
  
    float mMovementZ = end.z - start.z;
    if (mLowestGradient <= 1.0f) {
      float mImpactZ = start.z + mMovementZ * mLowestGradient;
      if (mImpactZ >= cLocation.z - STEP_HEIGHT && mImpactZ <= cLocation.z + 1.0f) {
        cCollected = true;
        return true;
      }
    }
  }
  return false;
}

bool Jewel::isCollected() {
  return cCollected;
}

void Jewel::render(int ticks, float alpha) {
  if (cCollected) {
    return;
  }
}

void Jewel::write(FILE *w) {
  fprintf(w, "<JEWEL LOCATION %d %d %d>\n", cLocation.x, cLocation.y, cLocation.z);
}


