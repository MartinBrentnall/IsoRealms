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
#include "ElementSpindizzyLift.h"

ElementSpindizzyLift::ElementSpindizzyLift(ISpindizzyLiftType* elementType, BlockLocation* location, I3DModelType* modelType, SpindizzyLiftProperties* properties, int bottom, int top, IElementContainer* container) {
  cLiftType    = elementType;
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
  cLiftModel   = modelType->createModel(&cLiftValues.cLocation);
  cContainer   = container;
  cPropertyUpSpeed     = new PropertyUpSpeed(    this);
  cPropertyDownSpeed   = new PropertyDownSpeed(  this);
  cPropertyTopDelay    = new PropertyTopDelay(   this);
  cPropertyBottomDelay = new PropertyBottomDelay(this);
  reset();
}

void ElementSpindizzyLift::setModelType(I3DModelType* oldModelType, I3DModelType* newModelType) {
  oldModelType->destroyModel(cLiftModel);
  cLiftModel = newModelType->createModel(&cLiftValues.cLocation);
}

void ElementSpindizzyLift::renderStatic() {
  // Nothing to do.
}

void ElementSpindizzyLift::renderEditingArrow() {
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

void ElementSpindizzyLift::renderEditing() {
  renderRuntime();
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

bool ElementSpindizzyLift::renderSelectionHighlight() {
  return false; // TODO: Something to do?
}

void ElementSpindizzyLift::executeLiftMovedScript() {
  ISpindizzyLiftSet* mLiftSet = cLiftType->getSpindizzyLiftInterface();
  mLiftSet->executeLiftMovedScript();
}

ElementSpindizzyLift::LiftValues ElementSpindizzyLift::getZLocationAfter(int milliseconds) {
  LiftValues mLift = cLiftValues;
  
  if (cLiftType->isActive()) {

    // Prevent an infinite loop occurring when pause intervals are both zero and the range of the lift is zero
    if (cTop == cBottom && cBottom == mLift.cLocation.z) {
      return mLift;
    }

    while (milliseconds > 0) {
      switch (mLift.cState) {
        case ElementSpindizzyLift::MOVING_UP: {
          double mToMove = milliseconds * 1.0f / cUpSpeed;
          mLift.cLocation.z += mToMove;
          if (mLift.cLocation.z > cTop) {
            mLift.cState = ElementSpindizzyLift::PAUSED_TOP;
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
  
        case ElementSpindizzyLift::MOVING_DOWN: {
          double mToMove = milliseconds * 1.0f / cDownSpeed;
          mLift.cLocation.z -= mToMove;
          if (mLift.cLocation.z < cBottom) {
            mLift.cState = ElementSpindizzyLift::PAUSED_BOTTOM;
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
  
        case ElementSpindizzyLift::PAUSED_TOP:
          mLift.cDelay -= milliseconds;
          if (mLift.cDelay < 0) {
            milliseconds = mLift.cDelay > 0 ? mLift.cDelay : -mLift.cDelay;
            mLift.cState = mLift.cLocation.z <= cBottom ? ElementSpindizzyLift::PAUSED_BOTTOM : ElementSpindizzyLift::MOVING_DOWN;
            if (mLift.cState == ElementSpindizzyLift::PAUSED_BOTTOM) {
              mLift.cDelay = cBottomDelay;
            }
          } else {
            milliseconds = 0;
          }
          break;
  
        case ElementSpindizzyLift::PAUSED_BOTTOM:
          mLift.cDelay -= milliseconds;
          if (mLift.cDelay < 0) {
            milliseconds = mLift.cDelay > 0 ? mLift.cDelay : -mLift.cDelay;
            mLift.cState = mLift.cLocation.z >= cTop ? ElementSpindizzyLift::PAUSED_TOP : ElementSpindizzyLift::MOVING_UP;
            if (mLift.cState == ElementSpindizzyLift::MOVING_UP) {
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

IElementContainer* ElementSpindizzyLift::getElementContainer() {
  return cContainer;
}

void ElementSpindizzyLift::updateRuntime(unsigned int milliseconds) {
  if (cLiftType->isActive()) {
    cLiftValues = getZLocationAfter(milliseconds);
  }
}

void ElementSpindizzyLift::updateEditing(unsigned int milliseconds) {
  // Do not move the lift
}

void ElementSpindizzyLift::reset() {
  cLiftValues.cLocation.z = cLocation.z;
  if (cTop < cLocation.z) {
    cLiftValues.cState = ElementSpindizzyLift::PAUSED_TOP;
    cLiftValues.cDelay = cTopDelay;
  } else {
    cLiftValues.cState = ElementSpindizzyLift::PAUSED_BOTTOM;
    cLiftValues.cDelay = cBottomDelay;
  }
}

void ElementSpindizzyLift::renderRuntime() {
  cLiftModel->render();
}

void ElementSpindizzyLift::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& relative) {
  std::string mElementTypePath = resourceLocator->getPath(cLiftType);
  node->addAttribute("type", mElementTypePath);
  cLocation.saveRelative(node, relative);
  node->addAttribute("top",         cTop    - relative.z);
  node->addAttribute("bottom",      cBottom - relative.z);
  node->addAttribute("upSpeed",     cUpSpeed);
  node->addAttribute("downSpeed",   cDownSpeed);
  node->addAttribute("topDelay",    cTopDelay);
  node->addAttribute("bottomDelay", cBottomDelay);
}

std::string ElementSpindizzyLift::getTypeName() {
  return "Spindizzy Lift"; // TODO
}
  
std::vector<IObjectProperty*> ElementSpindizzyLift::getProperties(IComponentContainer* windowWorkspace) {
  std::vector<IObjectProperty*> mProperties;
  mProperties.push_back(new PropertyInteger("Time to move one block up (ms)",   cPropertyUpSpeed));
  mProperties.push_back(new PropertyInteger("Time to move one block down (ms)", cPropertyDownSpeed));
  mProperties.push_back(new PropertyInteger("Delay at top (ms)",                cPropertyTopDelay));
  mProperties.push_back(new PropertyInteger("Delay at bottom (ms)",             cPropertyBottomDelay));
  return mProperties;
}

void ElementSpindizzyLift::destroyProperties(std::vector<IObjectProperty*> properties) {
  // TODO: Implement this
}

IElementType* ElementSpindizzyLift::getElementType() {
  return cLiftType;
}

bool ElementSpindizzyLift::initElement(IUniverse* universe, unsigned int pass) {
  switch (pass) {
    case 0: {
      ISpindizzyLiftSet* mLiftSet = cLiftType->getSpindizzyLiftInterface();
      mLiftSet->registerInterceptingSurface(this, this, universe);
      return true;
    }
  }
  return false;
}

void ElementSpindizzyLift::setDirty() {
  // Nothing to do
}

IElementBounds* ElementSpindizzyLift::getBounds() {
  return this;
}

void ElementSpindizzyLift::processCursorAppearance(ILayerEditingContext* editingContext, Vertex& location) {
  Element::processCursorAppearance(editingContext, location);
}

void ElementSpindizzyLift::processCursorMovement(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
  Element::processCursorMovement(editingContext, start, end);
}

PickedElement* ElementSpindizzyLift::pickElement(Vertex& start, Vertex& end) {
  return Element::pickElement(start, end);
}

Vertex* ElementSpindizzyLift::getBoundaryCrossingPoint(Vertex& start, Vertex& end, float* mLowestGradient, float infinity) {
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
  return nullptr;
}

bool ElementSpindizzyLift::contains(Vertex& location, float stepHeight) {
  float mSouthEdge = cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
  float mWestEdge  = cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
  float mNorthEdge = cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  float mEastEdge  = cLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  if (location.y >= mSouthEdge  && location.y < mNorthEdge && location.x >= mWestEdge && location.x < mEastEdge) {
    float mEnterHeight = getHeightAt(location.x, location.y);
    return location.z <= mEnterHeight && location.z >= mEnterHeight - stepHeight;
  }
  return false;
}

ICollisionData* ElementSpindizzyLift::getCollision(Vertex& start, Vertex& end, float stepHeight) {
  if (contains(start, stepHeight)) {
    Vertex* mEnterPoint = new Vertex(start);
    return new LiftSurfaceCollisionEvent(this, ICollisionData::SURFACE_MOUNT, mEnterPoint, 0.0f);;
  }
  
  float mGradient;
  Vertex* mEnterPoint = getBoundaryCrossingPoint(start, end, &mGradient, -INFINITY);
  if (mEnterPoint != nullptr) {
    float mEnterHeight = getHeightAt(mEnterPoint->x, mEnterPoint->y);
    if (mEnterPoint->z <= mEnterHeight && mEnterPoint->z >= mEnterHeight - stepHeight) {
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
  return nullptr;
}

ICollisionData* ElementSpindizzyLift::getRollingEvent(Vertex& start, Vertex& end) {
  float mGradient;
  Vertex* mLeavePoint = getBoundaryCrossingPoint(start, end, &mGradient, INFINITY);
  if (mLeavePoint != nullptr) {
    return new LiftSurfaceCollisionEvent(this, ICollisionData::SURFACE_LEAVE, mLeavePoint, mGradient);
  }
  return nullptr;
}

float ElementSpindizzyLift::getHeightAt(float, float) {
  return cLiftValues.cLocation.z;
}

float ElementSpindizzyLift::getXAcceleration(float, float) {
  return 0.0f;
}

float ElementSpindizzyLift::getYAcceleration(float, float) {
  return 0.0f;
}

void ElementSpindizzyLift::notifyContact() {
  // Nothing to do
}

void ElementSpindizzyLift::notifyImpact() {
  // Nothing to do
}

float ElementSpindizzyLift::getSurfaceFriction() {
  return 0.001f;
}

float ElementSpindizzyLift::getSurfaceGrip() {
  return 1.0f;
}

float ElementSpindizzyLift::getSurfaceBounce() {
  return 0.0f;
}

IRollableSurface::RespawnPossibility ElementSpindizzyLift::getRespawnPossibility() {
  return IRollableSurface::NO;
}

bool ElementSpindizzyLift::isRespawnPossibleNow() {
  return false;
}

void ElementSpindizzyLift::getRestingLocation(Vertex&) {
  // Nothing to do
}

BlockArea* ElementSpindizzyLift::getCoverage() {
  return new BlockArea(cLocation, cLocation);
}

float ElementSpindizzyLift::getWest() {
  return cLocation.x - IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyLift::getEast() {
  return cLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyLift::getSouth() {
  return cLocation.y - IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyLift::getNorth() {
  return cLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyLift::getBottom() {
  return std::min(cLocation.z, cBottom) * IsoRealmsConstants::BLOCK_HEIGHT;
}

float ElementSpindizzyLift::getTop() {
  return std::max(cLocation.z, cTop) * IsoRealmsConstants::BLOCK_HEIGHT;
}

ElementSpindizzyLift::PropertyUpSpeed::PropertyUpSpeed(ElementSpindizzyLift* parent) {
  cParent = parent;
}

void ElementSpindizzyLift::PropertyUpSpeed::setValue(int value) {
  cParent->cUpSpeed = std::max(1, value);;
}

int ElementSpindizzyLift::PropertyUpSpeed::getValue() {
  return cParent->cUpSpeed;
}

ElementSpindizzyLift::PropertyDownSpeed::PropertyDownSpeed(ElementSpindizzyLift* parent) {
  cParent = parent;
}

void ElementSpindizzyLift::PropertyDownSpeed::setValue(int value) {
  cParent->cDownSpeed = std::max(1, value);;
}

int ElementSpindizzyLift::PropertyDownSpeed::getValue() {
  return cParent->cDownSpeed;
}

ElementSpindizzyLift::PropertyTopDelay::PropertyTopDelay(ElementSpindizzyLift* parent) {
  cParent = parent;
}

void ElementSpindizzyLift::PropertyTopDelay::setValue(int value) {
  cParent->cTopDelay = std::max(0, value);
}

int ElementSpindizzyLift::PropertyTopDelay::getValue() {
  return cParent->cTopDelay;
}

ElementSpindizzyLift::PropertyBottomDelay::PropertyBottomDelay(ElementSpindizzyLift* parent) {
  cParent = parent;
}

void ElementSpindizzyLift::PropertyBottomDelay::setValue(int value) {
  cParent->cBottomDelay = std::max(0, value);;
}

int ElementSpindizzyLift::PropertyBottomDelay::getValue() {
  return cParent->cBottomDelay;
}
