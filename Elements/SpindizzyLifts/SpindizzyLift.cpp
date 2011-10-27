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

SpindizzyLift::SpindizzyLift(ISpindizzyLiftFactory* elementFactory, BlockLocation* location, ISimpleModelFactory* liftModelFactory, SpindizzyLiftProperties* properties, int bottom, int top) : Element<ISpindizzyLiftSet, ISpindizzyLiftFactory>(elementFactory) {
  cTopDelay    = properties->getTopDelay();
  cBottomDelay = properties->getBottomDelay();
  cUpSpeed     = properties->getUpSpeed();
  cDownSpeed   = properties->getDownSpeed();
  cLocation    = BlockLocation(*location);
  cBottom      = bottom;
  cTop         = top;
  cLiftValues.cLocation.x = cLocation.x;
  cLiftValues.cLocation.y = cLocation.y;
  cLiftValues.cLocation.z = cLocation.z;
  cLiftModel   = liftModelFactory->createModel(&cLiftValues.cLocation);
  reset();
}

void SpindizzyLift::setModel(ISimpleModelFactory* oldFactory, ISimpleModelFactory* newFactory) {
  oldFactory->destroyModel(cLiftModel);
  cLiftModel = newFactory->createModel(&cLiftValues.cLocation);
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
    if (cTop == cBottom && cBottom == mLift.cLocation.z) {
      return mLift;
    }

    while (milliseconds > 0) {
      switch (mLift.cState) {
        case SpindizzyLift::MOVING_UP: {
          double mToMove = milliseconds * 1.0f / cUpSpeed;
          mLift.cLocation.z += mToMove;
          if (mLift.cLocation.z > cTop) {
            mLift.cState = SpindizzyLift::PAUSED_TOP;
            mLift.cDelay = cTopDelay;
            milliseconds = 0; // TODO: Recover remaining ticks for pause
            mLift.cLocation.z = cTop;
          } else if ((int) mLift.cLocation.z != (int) cLiftValues.cLocation.z) {
            executeLiftMovedScript();
            milliseconds = 0;
          } else {
            milliseconds = 0;
          }
          break;
        }
  
        case SpindizzyLift::MOVING_DOWN: {
          double mToMove = milliseconds * 1.0f / cDownSpeed;
          mLift.cLocation.z -= mToMove;
          if (mLift.cLocation.z < cBottom) {
            mLift.cState = SpindizzyLift::PAUSED_BOTTOM;
            mLift.cDelay = cBottomDelay;
            milliseconds = 0; // TODO: Recover remaining ticks for pause
            mLift.cLocation.z = cBottom;
          } else if ((int) mLift.cLocation.z != (int) cLiftValues.cLocation.z) {
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
            mLift.cState = mLift.cLocation.z <= cBottom ? SpindizzyLift::PAUSED_BOTTOM : SpindizzyLift::MOVING_DOWN;
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
            mLift.cState = mLift.cLocation.z >= cTop ? SpindizzyLift::PAUSED_TOP : SpindizzyLift::MOVING_UP;
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
  cLiftValues.cLocation.z = cLocation.z;
  if (cTop < cLocation.z) {
    cLiftValues.cState = SpindizzyLift::PAUSED_TOP;
    cLiftValues.cDelay = cTopDelay;
  } else {
    cLiftValues.cState = SpindizzyLift::PAUSED_BOTTOM;
    cLiftValues.cDelay = cBottomDelay;
  }
}

void SpindizzyLift::render() {
  cLiftModel->render();
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

bool SpindizzyLift::initElement(unsigned int pass, bool editing) {
  switch (pass) {
    case 1: {
      ISpindizzyLiftSet* mLiftSet = getElementSet();
      if (!editing) {
        mLiftSet->registerInterceptingSurface(this);
      }
      return true;
    }
  }
  return false;
}

Vertex* SpindizzyLift::getBoundaryCrossingPoint(Vertex& start, Vertex& end, float* mLowestGradient, float infinity) {
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
      mImpactX = nextafterf(mWest, -infinity);
      mXKnown = true;
    }
  }

  mTempGradient = (mEast - start.x) / mXMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mEastYLocation = start.y + mYMovement * mTempGradient;
    if (mEastYLocation >= mSouth && mEastYLocation <= mNorth) {
      *mLowestGradient = mTempGradient;
      mImpactX = nextafterf(mEast, infinity);
      mXKnown = true;
    }
  }

  mTempGradient = (mNorth - start.y) / mYMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mNorthXLocation = start.x + mXMovement * mTempGradient;
    if (mNorthXLocation >= mWest && mNorthXLocation <= mEast) {
      *mLowestGradient = mTempGradient;
      mImpactY = nextafterf(mNorth, infinity);
      mXKnown = false;
      mYKnown = true;
    }
  }

  mTempGradient = (mSouth - start.y) / mYMovement;
  if (mTempGradient > 0.0f && mTempGradient <= *mLowestGradient) {
    float mSouthXLocation = start.x + mXMovement * mTempGradient;
    if (mSouthXLocation >= mWest && mSouthXLocation <= mEast) {
      *mLowestGradient = mTempGradient;
      mImpactY = nextafterf(mSouth, -infinity);
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
    std::cout << std::fixed << std::setprecision(56) << "Impact Y: " << mImpactY << std::endl;
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
  if (location.y >= mSouthEdge  && location.y < mNorthEdge && location.x >= mWestEdge && location.x < mEastEdge) {
    float mEnterHeight = getHeightAt(location.x, location.y);
    return location.z <= mEnterHeight && location.z >= mEnterHeight - 0.5f;
  }
  return false;
}

ICollisionData* SpindizzyLift::getCollision(Vertex& start, Vertex& end) {
  if (contains(start)) {
    Vertex* mEnterPoint = new Vertex(start);
    return new LiftSurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, 0.0f);;
  }
  
  float mGradient;
  Vertex* mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient, -INFINITY);
  if (mEnterPoint != NULL) {
    float mEnterHeight = getHeightAt(mEnterPoint->x, mEnterPoint->y);
    if (mEnterPoint->z <= mEnterHeight && mEnterPoint->z >= mEnterHeight - 0.5f) {
      return new LiftSurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, mGradient);
    }
  }

  float mLiftHeight = getHeightAt(start.x, start.y);
  if ((start.z > mLiftHeight) != (end.z > mLiftHeight) && start.z > mLiftHeight) {
    float mGradient = Collision::getCrossingPoint(start.z, end.z, mLiftHeight);
    float mXImpact = start.x + (end.x - start.x) * mGradient;
    float mYImpact = start.y + (end.y - start.y) * mGradient;
    float mZImpact = start.z + (end.z - start.z) * mGradient;
    Vertex* mImpactLocation = new Vertex(mXImpact, mYImpact, mZImpact);
    int mX = round(mXImpact);
    int mY = round(mYImpact);
    if (mX == cLocation.x && mY == cLocation.y) {
      return new LiftSurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mImpactLocation, mGradient);
    }
  }
  return NULL;
}

ICollisionData* SpindizzyLift::getRollingEvent(Vertex& start, Vertex& end) {
  float mGradient;
  Vertex* mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient, INFINITY);
  if (mLeavePoint != NULL) {
    return new LiftSurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, mLeavePoint, mGradient);
  }
  return NULL;
}

float SpindizzyLift::getHeightAt(float, float) {
  return cLiftValues.cLocation.z;
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

void SpindizzyLift::getRestingLocation(Vertex&) {
  // Nothing to do
}
