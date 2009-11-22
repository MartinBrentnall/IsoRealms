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
#include "Lift.h"

SwitchLogic* Lift::cSwitchLogic = NULL;
TextureSet* Lift::cTextures = NULL;
SoundSet* Lift::cSoundSet = NULL;

void Lift::init() {
  Configuration* mConfiguration = Configuration::getInstance();
  cSwitchLogic = mConfiguration->getSwitchLogic();
  cTextures = mConfiguration->getTextureSet();
  cSounds = mConfiguration->getSoundSet();
}

Lift::Lift(int type) {
  cType = type;
  cHighZPoint = 0;
  cLowZPoint = 0;
  cDownDelay = 0;
  cUpDelay = 0;
  resetToOrigin();
}

void Lift::playClick() {
  long int mLiftClickSound = (random() % LIFT_CLICKS_COUNT) + LIFT_CLICKS_START;
  cSoundSet->play(mLiftClickSound);
}

float Lift::getZLocationAfter(float ticks, int *state, float *delayUntilMove, bool sound) {
  float mNewZPoint = cCurrentZPoint;
  if (cSwitchLogic->isActive(cType)) {

    // Prevent an infinite loop occurring when pause intervals are both zero and the range of the lift is zero
    if (cHighZPoint == cLowZPoint && cLowZPoint == mNewZPoint) {
      return mNewZPoint;
    }

    while (ticks > 0) {
      switch (*state) {
        case MOVING_UP: {
          float mToMove = ticks * LIFT_SPEED;
          mNewZPoint += mToMove;
          if (mNewZPoint > cHighZPoint) {
            *state = PAUSE_TOP;
            *delayUntilMove = cUpDelay;
            ticks = 0; // TODO: Recover remaining ticks for pause
            mNewZPoint = cHighZPoint;
          } else if ((int) mNewZPoint != (int) cPreviousZPoint) {
            if (sound) {
              playClick();
            }
            ticks = 0;
          } else {
            ticks = 0;
          }
          break;
        }
  
        case MOVING_DOWN: {
          float mToMove = ticks * LIFT_SPEED;
          mNewZPoint -= mToMove;
          if (mNewZPoint < cLowZPoint) {
            *state = PAUSE_BOTTOM;
            *delayUntilMove = cDownDelay;
            ticks = 0; // TODO: Recover remaining ticks for pause
            mNewZPoint = cLowZPoint;
          } else if ((int) mNewZPoint != (int) cPreviousZPoint) {
            if (sound) {
              playClick();
            }
            ticks = 0;
          } else {
            ticks = 0;
          }
          break;
        }
  
        case PAUSE_TOP:
          *delayUntilMove -= ticks;
          if (*delayUntilMove < 0) {
            ticks = *delayUntilMove > 0 ? *delayUntilMove : -(*delayUntilMove);
            int mPauseBottom = PAUSE_BOTTOM;
            *state = mNewZPoint <= cLowZPoint ? mPauseBottom : MOVING_DOWN;
            if (*state == PAUSE_BOTTOM) {
              *delayUntilMove = cDownDelay;
            }
          } else {
            ticks = 0;
          }
          break;
  
        case PAUSE_BOTTOM:
          *delayUntilMove -= ticks;
          if (*delayUntilMove < 0) {
            ticks = *delayUntilMove > 0 ? *delayUntilMove : -(*delayUntilMove);
            int mPauseTop = PAUSE_TOP;
            *state = mNewZPoint >= cHighZPoint ? mPauseTop : MOVING_UP;
            if (*state == MOVING_UP) {
              if (sound) {
                playClick();
              }
            } else {
              *delayUntilMove = cUpDelay;
            }
          } else {
            ticks = 0;
          }
          break;
      }
    }
  }
  return mNewZPoint;
}

void Lift::updateLocation(float ticks) {
  cCurrentZPoint = getZLocationAfter(ticks, &cState, &cDelayUntilMove, true);
  cPreviousZPoint = cCurrentZPoint;
}

void Lift::setLocation(Point& location) {
  cLocation.x = location.x;
  cLocation.y = location.y;
  cLocation.z = location.z;
  cCurrentZPoint = location.z;
  cHighZPoint = location.z;
  cLowZPoint = location.z;
}

void Lift::render(float alpha) {
  cTextures->setTexture(cType);

  if (alpha < 1.0) {
    glEnable(GL_BLEND);
  }
  float mScaleFactor = 1.45;

  glColor4f(1.0, 1.0, 1.0, alpha);
  glPushMatrix();
  glTranslatef(cLocation.x, cLocation.y, cLocation.z * BLOCK_HEIGHT + (BLOCK_HEIGHT * 0.05));
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
  if (cTextures->isBackwards(cType)) {
    glTexCoord2f(0.0, 1.0); glVertex3f(    BLOCK_RADIUS * mScaleFactor, 0 - BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(    BLOCK_RADIUS * mScaleFactor,     BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0 - BLOCK_RADIUS * mScaleFactor,     BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0 - BLOCK_RADIUS * mScaleFactor, 0 - BLOCK_RADIUS * mScaleFactor, 0.0);
  } else {
    glTexCoord2f(1.0, 0.0); glVertex3f(    BLOCK_RADIUS * mScaleFactor, 0 - BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(    BLOCK_RADIUS * mScaleFactor,     BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0 - BLOCK_RADIUS * mScaleFactor,     BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0 - BLOCK_RADIUS * mScaleFactor, 0 - BLOCK_RADIUS * mScaleFactor, 0.0);
  }
  glEnd();
  glEnable(GL_CULL_FACE);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);

  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(1.0, 1.0, 0.0);
  glLineWidth(3.0);
  glLineStipple(1, 255);
  glEnable(GL_LINE_STIPPLE);
  renderArrowLines();
  glColor3f(1.0, 0.0, 0.0);
  glDisable(GL_LINE_STIPPLE);
  glLineWidth(5.0);
  renderArrowLines();
  glColor3f(1.0, 1.0, 1.0);
  glLineWidth(1.0);
  glPopMatrix();
}

void Lift::renderAsDynamic(float alpha) {
  cTextures->setTexture(cType);

  if (alpha < 1.0) {
    glEnable(GL_BLEND);
  }
  float mScaleFactor = 1.45;

  glColor4f(1.0, 1.0, 1.0, alpha);
  glPushMatrix();
  glTranslatef(cLocation.x, cLocation.y, cCurrentZPoint * BLOCK_HEIGHT + (BLOCK_HEIGHT * 0.05));
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
  if (cTextures->isBackwards(cType)) {
    glTexCoord2f(0.0, 1.0); glVertex3f(    BLOCK_RADIUS * mScaleFactor, 0 - BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(    BLOCK_RADIUS * mScaleFactor,     BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0 - BLOCK_RADIUS * mScaleFactor,     BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0 - BLOCK_RADIUS * mScaleFactor, 0 - BLOCK_RADIUS * mScaleFactor, 0.0);
  } else {
    glTexCoord2f(1.0, 0.0); glVertex3f(    BLOCK_RADIUS * mScaleFactor, 0 - BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(    BLOCK_RADIUS * mScaleFactor,     BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0 - BLOCK_RADIUS * mScaleFactor,     BLOCK_RADIUS * mScaleFactor, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0 - BLOCK_RADIUS * mScaleFactor, 0 - BLOCK_RADIUS * mScaleFactor, 0.0);
  }
  glEnd();
  glColor4f(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_CULL_FACE);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glPopMatrix();
}

void Lift::setHighPoint(int height) {
  cHighZPoint = height;
}

void Lift::setLowPoint(int height) {
  cLowZPoint = height;
}

void Lift::setUpDelay(int milliseconds) {
  cUpDelay = milliseconds;
}

void Lift::setDownDelay(int milliseconds) {
  cDownDelay = milliseconds;
}

void Lift::resetToOrigin() {
  cCurrentZPoint = cLocation.z;
  int mPauseTop = PAUSE_TOP;
  cState = cHighZPoint < cLocation.z ? mPauseTop : PAUSE_BOTTOM;
  cDelayUntilMove = cHighZPoint < cLocation.z ? cUpDelay : cDownDelay;
}

void Lift::write(FILE *w) {
  fprintf(w, "<LIFT TYPE %d LOCATION %d %d %d TOP %d %d BOTTOM %d %d>\n", cType, cLocation.x, cLocation.y, cLocation.z, cHighZPoint, cUpDelay, cLowZPoint, cDownDelay);
}

// Implemented for PlaneSurface interface
int Lift::getXSlopeAt(Coordinate& location) {
  return 0;
}

int Lift::getYSlopeAt(Coordinate& location) {
  return 0;
}

int Lift::getNorth() {
  return cLocation.y;
}

int Lift::getSouth() {
  return cLocation.y;
}

int Lift::getEast() {
  return cLocation.x;
}

int Lift::getWest() {
  return cLocation.x;
}

void Lift::render(TextureSet& textures) {
  render(1.0);
}

bool Lift::within(int x, int y) {
  return cLocation.x == x && cLocation.y == y;
}

bool Lift::isCeiling() {
  return false;
}

float Lift::getHeightAt(Coordinate& point) {
  return cCurrentZPoint;
}

bool Lift::isVerticallyAligned(Coordinate& location) {
  return false; // TODO: Fill in
}

bool Lift::isBelow(Point& point) {
  return point.x == cLocation.x && point.y == cLocation.y && point.z >= cCurrentZPoint;
}

bool Lift::isDynamic() {
  return false;  
}

bool Lift::isVisible() {
  return true;
}

Coordinate* Lift::getBoundaryCrossingPoint(Coordinate& start, Coordinate& end, float ticks, float *mLowestGradient) {
  *mLowestGradient = 2.0f;
  float mLineWidth = end.x - start.x;
  float mLineHeight = end.y - start.y;
  float mLeft = cLocation.x - BLOCK_RADIUS;
  float mTop = cLocation.y - BLOCK_RADIUS;
  float mRight = cLocation.x + BLOCK_RADIUS;
  float mBottom = cLocation.y + BLOCK_RADIUS;
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

    // We must find the lift height at the point of impact
    float mTicksUsed = *mLowestGradient * ticks;
    return new Coordinate(mImpactX, mImpactY, mImpactZ);
  }

  // Line doesn't cross boundary
  return NULL;
}

ImpactPoint* Lift::getRollingEvent(Coordinate& start, Coordinate& end, float ticks) {

  // Test if we left the lift
  if (isVerticallyAligned(start)) {
    float mGradient;
    Coordinate* mLeavePoint = getBoundaryCrossingPoint(start, end, ticks, &mGradient);
    if (mLeavePoint != NULL) {
      ImpactPoint *mImpactPoint = new ImpactPoint(EVENT_LEAVE_FLOOR, mGradient, *mLeavePoint);
      mImpactPoint->setRelocationPoint(*mLeavePoint);
      return mImpactPoint;
    }
  }

  // No event
  return NULL;
}

ImpactPoint* Lift::getCollisionEvent(Coordinate& start, Coordinate& end, float ticks) {
  float mLowestGradient = 2.0f;
  float mLineWidth = end.x - start.x;
  float mLineHeight = end.y - start.y;
  float mLeft = cLocation.x - BLOCK_RADIUS;
  float mTop = cLocation.y - BLOCK_RADIUS;
  float mRight = cLocation.x + BLOCK_RADIUS;
  float mBottom = cLocation.y + BLOCK_RADIUS;
  float mImpactX = 0.0f;
  float mImpactY = 0.0f;
  bool mXKnown = false;
  bool mYKnown = false;

  // The first test is whether we step onto the lift from outside it.
  if (!isVerticallyAligned(start)) {
    float mTempGradient = (mLeft - start.x) / mLineWidth;
    if (mTempGradient >= 0.0f && mTempGradient <= mLowestGradient) {
      float mLeftYLocation = start.y + mLineHeight * mTempGradient;
      if (mLeftYLocation >= mTop && mLeftYLocation <= mBottom) {
        mLowestGradient = mTempGradient;
        mImpactX = mLeft;
        mXKnown = true;
      }
    }
  
    mTempGradient = (mRight - start.x) / mLineWidth;
    if (mTempGradient >= 0.0f && mTempGradient <= mLowestGradient) {
      float mRightYLocation = start.y + mLineHeight * mTempGradient;
      if (mRightYLocation >= mTop && mRightYLocation <= mBottom) {
        mLowestGradient = mTempGradient;
        mImpactX = mRight;
        mXKnown = true;
      }
    }
  
    mTempGradient = (mTop - start.y) / mLineHeight;
    if (mTempGradient >= 0.0f && mTempGradient <= mLowestGradient) {
      float mTopXLocation = start.x + mLineWidth * mTempGradient;
      if (mTopXLocation >= mLeft && mTopXLocation <= mRight) {
        mLowestGradient = mTempGradient;
        mImpactY = mTop;
        mXKnown = false;
        mYKnown = true;
      }
    }
  
    mTempGradient = (mBottom - start.y) / mLineHeight;
    if (mTempGradient >= 0.0f && mTempGradient <= mLowestGradient) {
      float mBottomXLocation = start.x + mLineWidth * mTempGradient;
      if (mBottomXLocation >= mLeft && mBottomXLocation <= mRight) {
        mLowestGradient = mTempGradient;
        mImpactY = mBottom;
        mXKnown = false;
        mYKnown = true;
      }
    }
  
    float mMovementZ = end.z - start.z;
    if (mLowestGradient <= 1.0f) {
      if (!mXKnown) {
        mImpactX = start.x + mLineWidth * mLowestGradient;
      }
      if (!mYKnown) {
        mImpactY = start.y + mLineHeight * mLowestGradient;
      }
      float mImpactZ = start.z + mMovementZ * mLowestGradient;
  
      // We must find the lift height at the point of impact
      float mTicksUsed = mLowestGradient * ticks;
      int mSimulatedState = cState;
      float mSimulatedDelayUntilMove = cDelayUntilMove;
      float mZLocationAtImpact = getZLocationAfter(mTicksUsed, &mSimulatedState, &mSimulatedDelayUntilMove, false);
      if (mImpactZ >= mZLocationAtImpact - STEP_HEIGHT && mImpactZ <= mZLocationAtImpact) {
        int mTemp = EVENT_LEAVE_FLOOR;
        int mEventType = (!isVerticallyAligned(start))? EVENT_HIT_FLOOR : mTemp;
        Coordinate mLeaveEnter(mImpactX, mImpactY, mImpactZ);
        ImpactPoint *mImpactPoint = new ImpactPoint(mEventType, mLowestGradient, mLeaveEnter);
        return mImpactPoint;
      }
    }
  }

  // The second test is whether we land on the lift from above, or "climb" on from underneath
  // For this test, we use the lift as the frame of reference.  For this, we may need to split
  // the work between lift start / stops and direction changes over the course of the frame.
  float mTicksToUse = ticks;
  int mSimulatedState = cState;
  float mSimulatedZPoint = cCurrentZPoint;
  float mSimulatedDelay = cDelayUntilMove;
  cout << endl << "----->" << endl;
  while (mTicksToUse > 0.0f) {
    float mStartGradient = -(mTicksToUse / ticks) + 1.0f;
    switch (mSimulatedState) {
      case PAUSE_TOP: {
        cout << "  Pause top: ";
        mTicksToUse -= mSimulatedDelay;
        mSimulatedState = MOVING_DOWN;
        break;
      }
  
      case PAUSE_BOTTOM: {
        cout << "  Pause bot: ";
        mTicksToUse -= mSimulatedDelay;
        mSimulatedState = MOVING_UP;
        break;
      }
  
      case MOVING_UP: {
        cout << "  Moving up: ";
        float mToMove = mTicksToUse * LIFT_SPEED;
        float mToGo = (cHighZPoint - mSimulatedZPoint);
        float mUsed = (mToGo / mToMove);
        mTicksToUse -= mTicksToUse * mUsed;
        mSimulatedZPoint = cHighZPoint;
        mSimulatedState = PAUSE_TOP;
        mSimulatedDelay = cUpDelay;
        break;
      }

      case MOVING_DOWN: {
        cout << "  Moving Dn: ";
        float mToMove = mTicksToUse = LIFT_SPEED;
        float mToGo = (mSimulatedZPoint - cLowZPoint);
        float mUsed = (mToGo / mToMove);
        mTicksToUse -= mTicksToUse * mUsed;
        mSimulatedZPoint = cLowZPoint;
        mSimulatedState = PAUSE_BOTTOM;
        mSimulatedDelay = cDownDelay;
        break;
      }
    }

    if (mTicksToUse < 0.0f) {
      mTicksToUse = 0.0f;
    }

    float mEndGradient = -(mTicksToUse / ticks) + 1.0f;
    if (mEndGradient > mStartGradient) {
      cout << "  Gradient " << mStartGradient << " , " << mEndGradient << "  -(" << mTicksToUse << " / " << ticks << ") + 1      " << cCurrentZPoint << endl;
    }

/*    float mStartHeight = getHeightAt(start);
    float mEndHeight = getHeightAt(end);*/
/*    if (start.z > mStartHeight != end.z > mEndHeight && start.z > mStartHeight) {
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
        ImpactPoint *mImpactPoint = new ImpactPoint(mGradient, *mImpactLocation);
        mImpactPoint->setRelocationPoint(*mImpactLocation);
        return mImpactPoint;
      }
    }*/
  }

  return NULL;
}

