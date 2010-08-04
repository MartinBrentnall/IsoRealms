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
#include "SpindizzyLift.h"

SpindizzyLift::SpindizzyLift(ISpindizzyLiftFactory* elementFactory, BlockLocation* location, ISpindizzyTexture* texture, SpindizzyLiftProperties* properties, int bottom, int top) : Element<ISpindizzyLiftSet, ISpindizzyLiftFactory>(elementFactory) {
  cTopDelay    = properties->getTopDelay();
  cBottomDelay = properties->getBottomDelay();
  cUpSpeed     = properties->getUpSpeed();
  cDownSpeed   = properties->getDownSpeed();
  cTexture     = texture;
  cLocation    = BlockLocation(*location);
  cBottom      = bottom;
  cTop         = top;
  reset();
}

void SpindizzyLift::setTexture(ISpindizzyTexture* texture) {
  cTexture = texture;
}

void SpindizzyLift::renderStatic() {
  // Nothing to do.
}

void SpindizzyLift::renderEditingArrow() {
  float mLineRadius = IsoRealmsConstants::BLOCK_RADIUS * 0.5;
  float mArrowOffset = IsoRealmsConstants::BLOCK_HEIGHT * 0.5;
  glBegin(GL_LINES);
  glVertex3f(cLocation.x, cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x, cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT);

  // Top point
  glVertex3f(cLocation.x,               cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x + mLineRadius, cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT - mArrowOffset);
  glVertex3f(cLocation.x,               cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x - mLineRadius, cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT - mArrowOffset);

  // Bottom point
  glVertex3f(cLocation.x,               cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x + mLineRadius, cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT + mArrowOffset);
  glVertex3f(cLocation.x,               cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x - mLineRadius, cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT + mArrowOffset);
  glEnd();
}

void SpindizzyLift::renderStaticEditing() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(1.0, 1.0, 0.0);
  glLineWidth(3.0);
  glLineStipple(1, 255);
  glEnable(GL_LINE_STIPPLE);
  renderEditingArrow();
  glColor3f(1.0, 0.0, 0.0);
  glDisable(GL_LINE_STIPPLE);
  glLineWidth(5.0);
  renderEditingArrow();
  glColor3f(1.0, 1.0, 1.0);
  glLineWidth(1.0);
  glPopMatrix();
}

std::vector<IVisualElement*> SpindizzyLift::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyLift::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SpindizzyLift::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

void SpindizzyLift::executeLiftMovedScript() {
  ISpindizzyLiftSet* mLiftSet = getElementSet();
  mLiftSet->executeLiftMovedScript();
}

SpindizzyLift::LiftValues SpindizzyLift::getZLocationAfter(int milliseconds) {
  LiftValues mLift = cLiftValues;
  
  ISpindizzyLiftFactory* mLiftFactory = getElementFactory();
  if (mLiftFactory->isActive()) {

    // Prevent an infinite loop occurring when pause intervals are both zero and the range of the lift is zero
    if (cTop == cBottom && cBottom == mLift.cZ) {
      return mLift;
    }

    while (milliseconds > 0) {
      switch (mLift.cState) {
        case SpindizzyLift::MOVING_UP: {
          float mToMove = milliseconds * 1.0f / cUpSpeed;
          mLift.cZ += mToMove;
          if (mLift.cZ > cTop) {
            mLift.cState = SpindizzyLift::PAUSED_TOP;
            mLift.cDelay = cTopDelay;
            milliseconds = 0; // TODO: Recover remaining ticks for pause
            mLift.cZ = cTop;
          } else if ((int) mLift.cZ != (int) cLiftValues.cZ) {
            executeLiftMovedScript();
            milliseconds = 0;
          } else {
            milliseconds = 0;
          }
          break;
        }
  
        case SpindizzyLift::MOVING_DOWN: {
          float mToMove = milliseconds * 1.0f / cDownSpeed;
          mLift.cZ -= mToMove;
          if (mLift.cZ < cBottom) {
            mLift.cState = SpindizzyLift::PAUSED_BOTTOM;
            mLift.cDelay = cBottomDelay;
            milliseconds = 0; // TODO: Recover remaining ticks for pause
            mLift.cZ = cBottom;
          } else if ((int) mLift.cZ != (int) cLiftValues.cZ) {
            executeLiftMovedScript();
            milliseconds = 0;
          } else {
            milliseconds = 0;
          }
          break;
        }
  
        case SpindizzyLift::PAUSED_TOP:
          mLift.cDelay -= milliseconds;
          if (mLift.cDelay < 0) {
            milliseconds = mLift.cDelay > 0 ? mLift.cDelay : -mLift.cDelay;
            mLift.cState = mLift.cZ <= cBottom ? SpindizzyLift::PAUSED_BOTTOM : SpindizzyLift::MOVING_DOWN;
            if (mLift.cState == SpindizzyLift::PAUSED_BOTTOM) {
              mLift.cDelay = cBottomDelay;
            }
          } else {
            milliseconds = 0;
          }
          break;
  
        case SpindizzyLift::PAUSED_BOTTOM:
          mLift.cDelay -= milliseconds;
          if (mLift.cDelay < 0) {
            milliseconds = mLift.cDelay > 0 ? mLift.cDelay : -mLift.cDelay;
            mLift.cState = mLift.cZ >= cTop ? SpindizzyLift::PAUSED_TOP : SpindizzyLift::MOVING_UP;
            if (mLift.cState == SpindizzyLift::MOVING_UP) {
              executeLiftMovedScript();
            } else {
              mLift.cDelay = cTopDelay;
            }
          } else {
            milliseconds = 0;
          }
          break;
      }
    }
  }
  return mLift;
}

void SpindizzyLift::update(int milliseconds) {
  ISpindizzyLiftFactory* mLiftFactory = getElementFactory();
  if (mLiftFactory->isActive()) {
    cLiftValues = getZLocationAfter(milliseconds);
  }
}

void SpindizzyLift::reset() {
  cLiftValues.cZ = cLocation.z;
  if (cTop < cLocation.z) {
    cLiftValues.cState = SpindizzyLift::PAUSED_TOP;
    cLiftValues.cDelay = cTopDelay;
  } else {
    cLiftValues.cState = SpindizzyLift::PAUSED_BOTTOM;
    cLiftValues.cDelay = cBottomDelay;
  }
}

void SpindizzyLift::render() {
  glPushMatrix();
  glTranslatef(cLocation.x, cLocation.y, cLiftValues.cZ * IsoRealmsConstants::BLOCK_HEIGHT + (IsoRealmsConstants::BLOCK_HEIGHT * 0.05));
  cTexture->set();
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
  float mScaleFactor = 1.45;
  cTexture->texCoord2f(1.0, 1.0); glVertex3f(    IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0 - IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0.0);
  cTexture->texCoord2f(1.0, 0.0); glVertex3f(    IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor,     IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0.0);
  cTexture->texCoord2f(0.0, 0.0); glVertex3f(0 - IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor,     IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0.0);
  cTexture->texCoord2f(0.0, 1.0); glVertex3f(0 - IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0 - IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0.0);
  glEnd();
  glEnable(GL_CULL_FACE);
  glDisable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopMatrix();
}

void SpindizzyLift::save(DOMNodeWriter* node, BlockLocation& relative) {
  DOMNodeWriter* mLocation = node->addBranch("Location");
  cLocation.saveRelative(mLocation, relative);
  DOMNodeWriter* mLiftRange = node->addBranch("LiftMovement");
  mLiftRange->addAttribute("top", cTop - relative.z);
  mLiftRange->addAttribute("bottom", cBottom - relative.z);
  DOMNodeWriter* mLiftProperties = node->addBranch("LiftProperties");
  mLiftProperties->addAttribute("upSpeed", cUpSpeed);
  mLiftProperties->addAttribute("downSpeed", cDownSpeed);
  mLiftProperties->addAttribute("topDelay", cTopDelay);
  mLiftProperties->addAttribute("bottomDelay", cBottomDelay);
}

bool SpindizzyLift::initElement(unsigned int pass) {
  switch (pass) {
    case 1: {
      ISpindizzyLiftSet* mLiftSet = getElementSet();
      // TODO: This should only happen in runtime
      mLiftSet->registerInterceptingSurface(this);
      return true;
    }
  }
  return false;
}

Vertex* SpindizzyLift::getBoundaryCrossingPoint(Vertex& start, Vertex& end, float* mLowestGradient) {
  *mLowestGradient = 2.0f;
  float mXMovement = end.x - start.x;
  float mYMovement = end.y - start.y;
  float mSouth = cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
  float mWest  = cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
  float mNorth = cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  float mEast  = cLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  float mImpactX;
  float mImpactY;
  bool mXKnown = false;
  bool mYKnown = false;

  float mTempGradient = (mWest - start.x) / mXMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mWestYLocation = start.y + mYMovement * mTempGradient;
    if (mWestYLocation >= mSouth && mWestYLocation <= mNorth) {
      *mLowestGradient = mTempGradient;
      mImpactX = mWest;
      mXKnown = true;
    }
  }

  mTempGradient = (mEast - start.x) / mXMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mEastYLocation = start.y + mYMovement * mTempGradient;
    if (mEastYLocation >= mSouth && mEastYLocation <= mNorth) {
      *mLowestGradient = mTempGradient;
      mImpactX = mEast;
      mXKnown = true;
    }
  }

  mTempGradient = (mNorth - start.y) / mYMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mNorthXLocation = start.x + mXMovement * mTempGradient;
    if (mNorthXLocation >= mWest && mNorthXLocation <= mEast) {
      *mLowestGradient = mTempGradient;
      mImpactY = mNorth;
      mXKnown = false;
      mYKnown = true;
    }
  }

  mTempGradient = (mSouth - start.y) / mYMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mSouthXLocation = start.x + mXMovement * mTempGradient;
    if (mSouthXLocation >= mWest && mSouthXLocation <= mEast) {
      *mLowestGradient = mTempGradient;
      mImpactY = mSouth;
      mXKnown = false;
      mYKnown = true;
    }
  }

  float mMovementZ = end.z - start.z;
  if (*mLowestGradient <= 1.0f) {
    if (!mXKnown) {
      mImpactX = start.x + mXMovement * *mLowestGradient;
    }
    if (!mYKnown) {
      mImpactY = start.y + mYMovement * *mLowestGradient;
    }
    float mImpactZ = start.z + mMovementZ * *mLowestGradient;
    return new Vertex(mImpactX, mImpactY, mImpactZ);
  }

  // Line doesn't cross boundary
  return NULL;
}

bool SpindizzyLift::contains(Vertex& location) {
  float mSouthEdge = cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
  float mWestEdge  = cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
  float mNorthEdge = cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  float mEastEdge  = cLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  if (location.y > mSouthEdge  && location.y <= mNorthEdge && location.x > mWestEdge && location.x <= mEastEdge) {
    float mEnterHeight = getHeightAt(location.x, location.y);
    return location.z <= mEnterHeight + 0.01f && location.z >= mEnterHeight - 0.5f;
  }
  return false;
}

ICollisionData* SpindizzyLift::getCollision(Vertex& start, Vertex& end) {
  if (!contains(start)) {
    float mGradient;
    Vertex* mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient);
    if (mEnterPoint != NULL) {
      float mEnterHeight = getHeightAt(mEnterPoint->x, mEnterPoint->y);
      // TODO: The "0.01f" is a bit nasty magic number
      if (mEnterPoint->z <= mEnterHeight + 0.01f && mEnterPoint->z >= mEnterHeight - 0.5f) {
        return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, mGradient);
      }
    }
  }

  float mStartHeight = getHeightAt(start.x, start.y);
  float mEndHeight = getHeightAt(end.x, end.y);
  if ((start.z > mStartHeight) != (end.z > mEndHeight) && start.z > mStartHeight) {
    float mEndHeightModified = mEndHeight - (start.z - end.z);
    float mGradient = (start.z - mStartHeight) / (mEndHeightModified - mStartHeight);
    float mXImpact = start.x + (end.x - start.x) * mGradient;
    float mYImpact = start.y + (end.y - start.y) * mGradient;
    float mZImpact = start.z + (end.z - start.z) * mGradient;
    Vertex* mImpactLocation = new Vertex(mXImpact, mYImpact, mZImpact);
    int mX = round(mXImpact);
    int mY = round(mYImpact);
    if (mX == cLocation.x && mY == cLocation.y) {
      SurfaceCollisionEvent *mEvent = new SurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mImpactLocation, mGradient);
      return mEvent;
    }
  }
  return NULL;
}

ICollisionData* SpindizzyLift::getRollingEvent(Vertex& start, Vertex& end) {
  float mGradient;
  Vertex* mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient);
  if (mLeavePoint != NULL) {
    return new SurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, mLeavePoint, mGradient);
//    mImpactPoint->setRelocationPoint(*mLeavePoint);
  }
  return NULL;
}

float SpindizzyLift::getHeightAt(float, float) {
  return cLiftValues.cZ;
}

float SpindizzyLift::getXAcceleration(float, float) {
  return 0.0f;
}

float SpindizzyLift::getYAcceleration(float, float) {
  return 0.0f;
}

void SpindizzyLift::notifyContact() {
  // Nothing to do
}

void SpindizzyLift::notifyImpact() {
  // Nothing to do
}

float SpindizzyLift::getSurfaceFriction() {
  return 0.001f;
}

float SpindizzyLift::getSurfaceGrip() {
  return 1.0f;
}

float SpindizzyLift::getSurfaceBounce() {
  return 0.0f;
}

IRollableSurface::RespawnPossibility SpindizzyLift::getRespawnPossibility() {
  return IRollableSurface::NO;
}

bool SpindizzyLift::isRespawnPossibleNow() {
  return false;
}
