/*
 * Copyright 2023 Martin Brentnall
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

#include "Modules/Spindizzy/LiftType/LiftType.h"
#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string Lift::ATTRIBUTE_BOTTOM       = "bottom";
  const std::string Lift::ATTRIBUTE_BOTTOM_PAUSE = "bottomPause";
  const std::string Lift::ATTRIBUTE_DOWN_SPEED   = "downSpeed";
  const std::string Lift::ATTRIBUTE_TOP          = "top";
  const std::string Lift::ATTRIBUTE_TOP_PAUSE    = "topPause";
  const std::string Lift::ATTRIBUTE_TYPE         = "type";
  const std::string Lift::ATTRIBUTE_UP_SPEED     = "upSpeed";
  const std::string Lift::ATTRIBUTE_X            = "x";
  const std::string Lift::ATTRIBUTE_Y            = "y";
  const std::string Lift::ATTRIBUTE_Z            = "z";

  Lift::Lift(Zone& zone, LiftType* type, int x, int y, int z, int bottom, int top) :
            cDefZone(zone),
            cDefType(type),
            cDefModel(cDefType->createModel()),
            cDefX(x),
            cDefY(y),
            cDefZ(z),
            cDefTop(top),
            cDefBottom(bottom),
            cDefTopPause(1500),
            cDefBottomPause(1500),
            cDefSpeedUp(1000),
            cDefSpeedDown(1000),
            cSurface(*this) {
    reset();
  }

  Lift::Lift(Zone& zone, DOMNode& node) :
            cDefZone(zone),
            cDefType(nullptr),
            cDefModel(nullptr),
            cDefX(node.getIntegerAttribute(ATTRIBUTE_X) + cDefZone.getStartX()),
            cDefY(node.getIntegerAttribute(ATTRIBUTE_Y) + cDefZone.getStartY()),
            cDefZ(node.getIntegerAttribute(ATTRIBUTE_Z) + cDefZone.getStartZ()),
            cDefTop(node.getIntegerAttribute(ATTRIBUTE_TOP) + cDefZone.getStartZ()),
            cDefBottom(node.getIntegerAttribute(ATTRIBUTE_BOTTOM) + cDefZone.getStartZ()),
            cDefTopPause(node.getIntegerAttribute(ATTRIBUTE_TOP_PAUSE)),
            cDefBottomPause(node.getIntegerAttribute(ATTRIBUTE_BOTTOM_PAUSE)),
            cDefSpeedUp(node.getIntegerAttribute(ATTRIBUTE_UP_SPEED)),
            cDefSpeedDown(node.getIntegerAttribute(ATTRIBUTE_DOWN_SPEED)),
            cSurface(*this) {
    cDefZone.getWorld()->getSpindizzy()->getProject()->init([this, node](IAssets* assets) {
      cDefType = cDefZone.getWorld()->getSpindizzy()->getLiftType(node.getAttribute(ATTRIBUTE_TYPE));
      cDefModel = cDefType->createModel();
      reset();
    });
  }

  void Lift::initialise() {
    cDefZone.getWorld()->attachPhysicalSurface(&cSurface, true);
  }

  void Lift::reset() {
    cRuntimeState.cState = State::START_UP_DELAY;
    cRuntimeState.cDelay = cDefTop < cDefZ ? cDefTopPause : cDefBottomPause;
  }

  void Lift::save(DOMNodeWriter* node, int x, int y, int z) {
    node->addAttribute(ATTRIBUTE_X,            cDefX - x);
    node->addAttribute(ATTRIBUTE_Y,            cDefY - y);
    node->addAttribute(ATTRIBUTE_Z,            cDefZ - z);
    node->addAttribute(ATTRIBUTE_TYPE,         cDefZone.getWorld()->getSpindizzy()->getID(cDefType));
    node->addAttribute(ATTRIBUTE_TOP,          cDefTop    - z);
    node->addAttribute(ATTRIBUTE_BOTTOM,       cDefBottom - z);
    node->addAttribute(ATTRIBUTE_UP_SPEED,     cDefSpeedUp);
    node->addAttribute(ATTRIBUTE_DOWN_SPEED,   cDefSpeedDown);
    node->addAttribute(ATTRIBUTE_TOP_PAUSE,    cDefTopPause);
    node->addAttribute(ATTRIBUTE_BOTTOM_PAUSE, cDefBottomPause);
  }

  bool Lift::isType(const LiftType* const type) const {
    return cDefType == type;
  }

  void Lift::updateRuntime(unsigned int milliseconds) {
    cDefModel->update(milliseconds);
    if (cDefType->isActive() && !cDefZone.getWorld()->getSpindizzy()->isPaused()) {
      int mOldHeight = static_cast<int>(getHeight(cRuntimeState));
      State mOldState = cRuntimeState.cState;
      cRuntimeState = getZLocationAfter(milliseconds);
      int mNewHeight = static_cast<int>(getHeight(cRuntimeState));
      State mNewState = cRuntimeState.cState;

      // Lift movement when starting to move, or moving to the next integer height
      if ((mOldHeight != mNewHeight && (mOldHeight != cDefTop && (mNewHeight != cDefTop || mOldState == State::START_UP_MOVING_DOWN)))
        || (mOldState != mNewState && (mNewState == State::MOVING_DOWN || mNewState == State::MOVING_UP || (mNewState == State::START_UP_MOVING_UP && cDefZ != cDefTop)))) {
        cDefType->executeTickAction();
      }
    }
  }

  void Lift::updateEditing(unsigned int milliseconds) {
    cDefModel->update(milliseconds);
  }

  void Lift::renderRuntime() const {
    glPushMatrix();
    glTranslatef(cDefX, cDefY, getHeight(cRuntimeState) * 0.5f);
    cDefModel->render();
    glPopMatrix();
  }

  void Lift::renderEditing() const {
    glPushMatrix();
    glTranslatef(cDefX, cDefY, cDefZ * 0.5f);
    cDefModel->render();
    glPopMatrix();
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
  }































  void Lift::renderEditingArrow() const {
    float mLineRadius = 0.5f * 0.5;
    float mArrowOffset = 0.5f * 0.5;
    glBegin(GL_LINES);
    glVertex3f(cDefX, cDefY, cDefTop * 0.5f);
    glVertex3f(cDefX, cDefY, cDefBottom * 0.5f);

    // Top point
    glVertex3f(cDefX,               cDefY, cDefTop * 0.5f);
    glVertex3f(cDefX + mLineRadius, cDefY, cDefTop * 0.5f - mArrowOffset);
    glVertex3f(cDefX,               cDefY, cDefTop * 0.5f);
    glVertex3f(cDefX - mLineRadius, cDefY, cDefTop * 0.5f - mArrowOffset);

    // Bottom point
    glVertex3f(cDefX,               cDefY, cDefBottom * 0.5f);
    glVertex3f(cDefX + mLineRadius, cDefY, cDefBottom * 0.5f + mArrowOffset);
    glVertex3f(cDefX,               cDefY, cDefBottom * 0.5f);
    glVertex3f(cDefX - mLineRadius, cDefY, cDefBottom * 0.5f + mArrowOffset);
    glEnd();
  }

  Lift::LiftValues Lift::getZLocationAfter(unsigned int milliseconds) {
    LiftValues mState = cRuntimeState;
    if (cDefTop != cDefBottom || mState.cState == State::START_UP_DELAY || mState.cState == State::START_UP_MOVING_UP || mState.cState == State::START_UP_MOVING_DOWN) {      
      while (milliseconds != 0U) {
        if (mState.cDelay > milliseconds) {
          mState.cDelay -= milliseconds;
          milliseconds = 0U;
        } else {
          milliseconds -= mState.cDelay;
          switch (mState.cState) {
            case State::START_UP_DELAY: {
              if (cDefZ <= cDefTop) {
                mState.cState = State::START_UP_MOVING_UP; 
                mState.cDelay = (cDefTop - cDefZ) * cDefSpeedUp;
              } else {
                mState.cState = State::START_UP_MOVING_DOWN; 
                mState.cDelay = (cDefZ - cDefBottom) * cDefSpeedDown;
              }
              break;
            }
            case State::PAUSED_BOTTOM:  {mState.cState = State::MOVING_UP;          mState.cDelay = (cDefTop - cDefBottom) * cDefSpeedUp;     break;}
            case State::START_UP_MOVING_UP:
            case State::MOVING_UP:      {mState.cState = State::PAUSED_TOP;         mState.cDelay = cDefTopPause;                             break;}
            case State::PAUSED_TOP:     {mState.cState = State::MOVING_DOWN;        mState.cDelay = (cDefTop - cDefBottom) * cDefSpeedDown;   break;}
            case State::START_UP_MOVING_DOWN:
            case State::MOVING_DOWN:    {mState.cState = State::PAUSED_BOTTOM;      mState.cDelay = cDefBottomPause;                          break;}
          }
        }
      }
    }
    return mState;
  }
  
  double Lift::getHeight(LiftValues state) const {
    switch (state.cState) {
      case State::START_UP_DELAY:       {return cDefZ;}
      case State::START_UP_MOVING_UP:   {unsigned int mRange = cDefTop - cDefZ;    return mRange == 0 ? cDefTop :    cDefTop    - mRange * (state.cDelay / (static_cast<double>(mRange) * cDefSpeedUp));}
      case State::START_UP_MOVING_DOWN: {unsigned int mRange = cDefZ - cDefBottom; return mRange == 0 ? cDefBottom : cDefBottom + mRange * (state.cDelay / (static_cast<double>(mRange) * cDefSpeedDown));}
      case State::PAUSED_BOTTOM:        {return cDefBottom;}
      case State::MOVING_UP:            {unsigned int mRange = cDefTop - cDefBottom;     return mRange == 0 ? cDefTop : cDefTop    - mRange * (state.cDelay / (static_cast<double>(mRange) * cDefSpeedUp));}
      case State::PAUSED_TOP:           {return cDefTop;}
      case State::MOVING_DOWN:          {unsigned int mRange = cDefTop - cDefBottom;     return mRange == 0 ? cDefTop : cDefBottom + mRange * (state.cDelay / (static_cast<double>(mRange) * cDefSpeedUp));}
    }
    std::cout << "WARNING: Lift::getHeight(): Invaled value in state.cState" << std::endl;
    return cDefZ;
  }
  
  Zone* Lift::Surface::getZone() {
    return &cParent.cDefZone;
  }

  bool Lift::Surface::isSolid() {
    return true;
  }

  std::unique_ptr<Lift::LiftSurfaceEvent> Lift::getEvent(double startTime, double endTime) {
    if (cDefType->isActive() && !cDefZone.getWorld()->getSpindizzy()->isPaused()) {
      double mMillisecondsProcessed = 0.0;
      LiftValues mState = cRuntimeState;
      if (cDefTop != cDefBottom || cDefTopPause != 0 || cDefBottomPause != 0) {      
        while (mMillisecondsProcessed < endTime) {
          if (mState.cDelay <= (endTime - mMillisecondsProcessed)) {
            mMillisecondsProcessed += mState.cDelay;
            switch (mState.cState) {
              case State::START_UP_DELAY: {mState.cState = State::START_UP_MOVING_UP; mState.cDelay = (cDefTop - cDefZ) * cDefSpeedUp; break;}
              case State::PAUSED_BOTTOM:  {mState.cState = State::MOVING_UP;          mState.cDelay = (cDefTop - cDefBottom) * cDefSpeedUp;     break;}
              case State::START_UP_MOVING_UP:
              case State::MOVING_UP:      {mState.cState = State::PAUSED_TOP;         mState.cDelay = cDefTopPause;                       break;}
              case State::PAUSED_TOP:     {mState.cState = State::MOVING_DOWN;        mState.cDelay = (cDefTop - cDefBottom) * cDefSpeedDown;   break;}
              case State::START_UP_MOVING_DOWN:
              case State::MOVING_DOWN:    {mState.cState = State::PAUSED_BOTTOM;      mState.cDelay = cDefBottomPause;                    break;}
            }
            if (mMillisecondsProcessed > startTime) {
              return std::make_unique<LiftSurfaceEvent>(this, mState, mMillisecondsProcessed - startTime);
            }
          } else {
            mMillisecondsProcessed = endTime;
          }
        }
      }
    }
    return nullptr;
  }

  void Lift::Surface::adjustPosition(LiteralVertex& location, double milliseconds) {
    LiftValues mValues = cParent.getZLocationAfter(milliseconds);
    location.z = cParent.getHeight(mValues);
  }

  std::unique_ptr<LiteralVertex> Lift::getBoundaryCrossingPoint(LiteralVertex& start, LiteralVertex& end, float* mLowestGradient, float infinity) {
    *mLowestGradient = 2.0f;
    float mXMovement = end.x - start.x;
    float mYMovement = end.y - start.y;
    float mSouth = cDefY - 0.5f;
    float mWest  = cDefX - 0.5f;
    float mNorth = cDefY + 0.5f;
    float mEast  = cDefX + 0.5f;
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
      return std::make_unique<LiteralVertex>(mImpactX, mImpactY, mImpactZ);
    }

    // Line doesn't cross boundary
    return nullptr;
  }

  Lift::Surface::Surface(Lift& parent) :
            cParent(parent) {
  }

  bool Lift::Surface::contains(LiteralVertex& location, float stepHeight) {
    float mSouthEdge = cParent.cDefY - 0.5f;
    float mWestEdge  = cParent.cDefX - 0.5f;
    float mNorthEdge = cParent.cDefY + 0.5f;
    float mEastEdge  = cParent.cDefX + 0.5f;
    if (location.y >= mSouthEdge  && location.y < mNorthEdge && location.x >= mWestEdge && location.x < mEastEdge) {
      float mEnterHeight = getHeightAt(location.x, location.y);
      return location.z < mEnterHeight && location.z >= mEnterHeight - stepHeight;
    }
    return false;
  }

  std::unique_ptr<CollisionData> Lift::getCollision(Surface* surface, LiteralVertex& start, LiteralVertex& end, LiteralVertex& actualStart, LiteralVertex& actualEnd, float height, bool down) {
    if (((start.z > height) != (end.z > height)) && (start.z > height) == down) {
      float mGradient = CollisionUtils::getCrossingPoint(start.z, end.z, height);
      float mXImpact = start.x + (end.x - start.x) * mGradient;
      float mYImpact = start.y + (end.y - start.y) * mGradient;
      float mZImpact = actualStart.z + (actualEnd.z - actualStart.z) * mGradient;
      LiteralVertex mImpactLocation(mXImpact, mYImpact, mZImpact);
      int mX = round(mXImpact);
      int mY = round(mYImpact);
      if (mX == cDefX && mY == cDefY) {
        return std::make_unique<CollisionData>(surface, CollisionData::Type::SURFACE_MOUNT, mImpactLocation, 0.0f, 0.0f, mGradient);
      }
    } 
    return nullptr;
  }
  
  std::unique_ptr<CollisionData> Lift::Surface::getCollision(LiteralVertex& start, LiteralVertex& end, float stepHeight, double startTime, double endTime) {

    // Previous event may have corrected object position into the lift.
    if (contains(start, stepHeight)) {
      LiteralVertex mPosition(start);
      mPosition.z = getHeightAt(start.x, start.y);
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, mPosition, 0.0f, 0.0f, 0.0f);
    }    
    
    // Normalise start and end positions
    float mStartHeight = cParent.getHeight(cParent.getZLocationAfter(startTime));
    float mEndHeight   = cParent.getHeight(cParent.getZLocationAfter(endTime));
    LiteralVertex mNormalStart(start);
    LiteralVertex mNormalEnd(end);
    mNormalStart.z = start.z - mStartHeight;
    mNormalEnd.z   = end.z   - mEndHeight;

    float mGradient;
    std::unique_ptr<LiteralVertex> mEnterPoint = cParent.getBoundaryCrossingPoint(mNormalStart, mNormalEnd, &mGradient, -INFINITY);
    if (mEnterPoint != nullptr && mEnterPoint->getZ() <= 0.0 && mEnterPoint->getZ() >= 0.0 - stepHeight) {
      mEnterPoint->z = start.z + (end.z - start.z) * mGradient;
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOUNT, *mEnterPoint.get(), 0.0f, 0.0f, mGradient);
    }

    std::unique_ptr<CollisionData> mCollision = cParent.getCollision(this, mNormalStart, mNormalEnd, start, end, 0.0f, true);
    if (mCollision == nullptr) {
      mCollision = cParent.getCollision(this, mNormalStart, mNormalEnd, start, end, 0.0f - stepHeight, false);
    }
    return mCollision;;
  }

  std::unique_ptr<CollisionData> Lift::Surface::getRollingEvent(LiteralVertex& start, LiteralVertex& end, double startTime, double endTime) {
    if (start.x < cParent.cDefX - 0.5f || start.x >= cParent.cDefX + 0.5f || start.y < cParent.cDefY - 0.5f || start.y >= cParent.cDefY + 0.5f) {
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_LEAVE, LiteralVertex(start), 0.0f, 0.0f, 0.0f);
    }
    
    float mGradient;
    std::unique_ptr<LiteralVertex> mLeavePoint = cParent.getBoundaryCrossingPoint(start, end, &mGradient, INFINITY);
    double mDuration = endTime - startTime;
    if (mLeavePoint != nullptr) {
      double mEventTime = mDuration * mGradient;
      if (mEventTime < cParent.cRuntimeState.cDelay) {
        float mStartHeight = cParent.getHeight(cParent.getZLocationAfter(startTime));
        float mEndHeight   = cParent.getHeight(cParent.getZLocationAfter(endTime));
        mLeavePoint->z = mStartHeight + (mEndHeight - mStartHeight) * mGradient;
        return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_LEAVE, *mLeavePoint.get(), 0.0f, 0.0f, mGradient);
      }
    }
    
    std::unique_ptr<LiftSurfaceEvent> mEvent = cParent.getEvent(startTime, endTime);
    if (mEvent != nullptr) {
      mGradient = mEvent->cTime / mDuration;
      double mX = (end.x - start.x) * mGradient + start.x;
      double mY = (end.y - start.y) * mGradient + start.y;
      double mZ = cParent.cRuntimeState.cState == State::MOVING_DOWN || cParent.cRuntimeState.cState == State::PAUSED_BOTTOM ? cParent.cDefBottom : cParent.cDefTop;
//       std::cout << "FROM: " << startTime << " TO " << endTime << std::endl;
//       std::cout << "SETTING GRADIENT: " << mGradient << " (" << mEvent->cTime << " / " << mDuration << ")" << std::endl;
      return std::make_unique<CollisionData>(this, CollisionData::Type::SURFACE_MOVEMENT, LiteralVertex(mX, mY, mZ), 0.0f, 0.0f, mGradient);
    }
    return nullptr;
  }

  float Lift::Surface::getHeightAt(float x, float y) const {
    return cParent.getHeight(cParent.cRuntimeState);
  }

  float Lift::Surface::getXAcceleration(float, float) {
    return 0.0f;
  }

  float Lift::Surface::getYAcceleration(float, float) {
    return 0.0f;
  }

  void Lift::Surface::notifyContact() {
    // Nothing to do
  }

  void Lift::Surface::notifyImpact() {
    // Nothing to do
  }

  float Lift::Surface::getSurfaceFriction() {
    return 0.001f;
  }

  float Lift::Surface::getSurfaceGrip() {
    return 1.0f;
  }

  float Lift::Surface::getSurfaceBounce() {
    return 0.0f;
  }

  Lift::Surface::Respawn Lift::Surface::isRespawnAllowed() {
    return Respawn::NO;
  }

  bool Lift::Surface::isRespawnPossible() {
    return false;
  }

  void Lift::Surface::getRestingLocation(LiteralVertex&) {
    // Nothing to do
  }

  void Lift::Surface::saveCache(std::ostream& cache, bool physical) {
    // Nothing to do
  }

  int Lift::Surface::getXStart() const {
    return cParent.cDefX;
  }

  int Lift::Surface::getXEnd() const {
    return cParent.cDefX;
  }

  int Lift::Surface::getYStart() const {
    return cParent.cDefY;
  }

  int Lift::Surface::getYEnd() const {
    return cParent.cDefY;
  }

  IWorldObject* Lift::Surface::getOwner() {
    return &cParent;
  }

  void Lift::Surface::render() const {
    // Not supported.
  }
  
  void Lift::Surface::renderOutline() const {
    // Not supported.
  }
  
  void Lift::Surface::renderHighlight() const {
    // Not supported.
  }
  
  int Lift::Surface::getSurfaceCellHeight(int x, int y) const {
    return cParent.cDefZ;
  }
  
  int Lift::Surface::getSurfaceCellElevation(int x, int y) const {
    return 0;
  }
  
  bool Lift::Surface::alligned(int x, int y) const {
    return cParent.cDefX == x && cParent.cDefY == y;
  }
  
  std::vector<std::unique_ptr<IVisualElement>> Lift::Surface::getStaticVisuals() {
    return std::vector<std::unique_ptr<IVisualElement>> {
    };
  }

  Lift::LiftSurfaceEvent::LiftSurfaceEvent(Lift* parent, LiftValues state, double time) {
    cParent = parent;
    cState  = state;
    cTime   = time;
  }

  bool Lift::contains(const LiteralVertex& location) const {
    return location.x >= cDefX - 0.5f && location.x <= cDefX + 0.5f && location.y >= cDefY - 0.5f && location.y <= cDefY + 0.5f && location.z >= std::min(cDefZ, cDefBottom) && location.z <= std::max(cDefZ, cDefTop);
  }

  void Lift::renderSelectionHighlight() const {
    glColor4f(0.5f, 0.0f, 1.0f, 0.5f);
    Utils::renderVolumeCuboid(cDefX - 0.5f, cDefX + 0.5f, cDefY - 0.5f, cDefY + 0.5f, std::min(cDefZ, cDefBottom) * 0.5f, std::max(cDefZ, cDefTop) * 0.5f + 0.02f);
  }

  void Lift::remove() {
    cDefZone.remove(this);
  }

  std::vector<std::unique_ptr<IProperty>> Lift::getProperties(IPropertyAppearance* appearance) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>(appearance, "Pause", [this]() {return cDefTopPause > 0;}, [this](bool value) {
      cDefTopPause = value ? 1500 : 0;
      cDefBottomPause = value ? 1500 : 0;
      std::cout << "SETTING LIFT PAUSE: " << cDefTopPause << std::endl;
    }));
    return mProperties;
  }

  std::string Lift::getTypeName() const {
    return "Lift";
  }

  Zone& Lift::getObjectZone() {
    return cDefZone;
  }
}
