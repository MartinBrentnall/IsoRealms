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
#include "SpindizzyGERALD.h"

const float SpindizzyGERALD::CRAFT_ACCELERATION = 0.00003f;
const float SpindizzyGERALD::GRAVITY_STRENGTH = -0.0002f;
const unsigned int SpindizzyGERALD::INIT_REGISTER_BLOCKS = 0;
const unsigned int SpindizzyGERALD::INIT_PROCESS_BLOCKS = 1;
const unsigned int SpindizzyGERALD::INIT_REGISTER_SURFACES = 2;
const unsigned int SpindizzyGERALD::INIT_USE_SURFACES = 3;
const unsigned int SpindizzyGERALD::BOUNCE_CONTROL_TIME = 40;

SpindizzyGERALD::SpindizzyGERALD(ISpindizzyGERALDType* elementType, IProject* project, DOMNodeWrapper* node) : Element<ISpindizzyGERALDSet, ISpindizzyGERALDType>(elementType) {
  cLocation = new Vertex();
  readData(node);
  initInstance(project);
}

SpindizzyGERALD::SpindizzyGERALD(ISpindizzyGERALDType* elementType, BlockLocation* startLocation, IProject* project) : Element<ISpindizzyGERALDSet, ISpindizzyGERALDType>(elementType) {
  cStartLocation = BlockLocation(*startLocation);
  cLocation = new Vertex();
  cLocation->x = cStartLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->y = cStartLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->z = cStartLocation.z;
  cGERALDModel = elementType->createModel(cLocation);
  initInstance(project);
}

SpindizzyGERALD::SpindizzyGERALD(ISpindizzyGERALDType* elementType, IProject* project) : Element<ISpindizzyGERALDSet, ISpindizzyGERALDType>(elementType) {
  cLocation = new Vertex();
  initInstance(project);
}

void SpindizzyGERALD::initInstance(IProject* project) {
  cProject = project;
  cRespawning = false;
  cLockNorth = NULL;
  cLockSouth = NULL;
  cLockEast = NULL;
  cLockWest = NULL;
  if (cProject != NULL) {
    cPressedForward  = cProject->registerDigitalInput("Move North");
    cPressedRight    = cProject->registerDigitalInput("Move East");
    cPressedBackward = cProject->registerDigitalInput("Move South");
    cPressedLeft     = cProject->registerDigitalInput("Move West");
    cThrust          = cProject->registerDigitalInput("Thrust");
  }
  cMapBottom = -20.0f; // TODO: Do this for real!
  cZone = NULL;
}

void SpindizzyGERALD::collected(ICollectable* collectable) {
  // Nothing to do.  TODO: Something to do?
}

void SpindizzyGERALD::renderStatic() {
  // Nothing to do.
}

bool SpindizzyGERALD::initElement(unsigned int pass) {
  ISpindizzyGERALDSet* mGERALDSet = getElementSet();
  if (mGERALDSet->isEditing()) {
    return true;
  }
  ISpindizzyGERALDType* mGERALDType = getElementType();
  switch (pass) {
    case INIT_REGISTER_BLOCKS: {
      if (cProject != NULL) {
        cZone = cProject->getZone(*cLocation);
        mGERALDType->executeZoneEnteredScript(cZone);
      }
      return false;
    }

    case INIT_USE_SURFACES: {
      cCurrentSurface = mGERALDType->getSurfaceAt(*cLocation);
      cRespawnData = new RespawnData();
      cRespawnData->cSurface = cCurrentSurface;
      cRespawnSurfaceStack.push(cRespawnData);
      cPeakHeight = cMapBottom;
      return true;
    }

    default: {
      return false;
    }
  }
}

std::vector<IVisualElement*> SpindizzyGERALD::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyGERALD::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  return mDynamicElements;
}

std::vector<IDynamicElement*> SpindizzyGERALD::getDynamicElementsRuntime() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

void SpindizzyGERALD::checkCurrentZoneEvents(Vertex& start, Vertex& end) {
  std::vector<ZoneEvent*> mZoneEvents = cZone->getZoneEvents(start, end);
  for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
    switch (mZoneEvents[i]->getType()) {
      case ZoneEvent::EXITED: {
        ISpindizzyGERALDType* mGERALDType = getElementType();
        mGERALDType->executeZoneExitedScript(cZone);
        cZone = NULL;
        break;
      }

      default: {
        // TODO: Throw wobbly
        std::cout << "Zone entered event should never happen for the current zone" << std::endl;
        exit(1);
      }
    }
  }
}

Vertex* SpindizzyGERALD::getLocation() {
  return cLocation;
}

void SpindizzyGERALD::checkMapZoneEvents(IZone* previousZone, Vertex& start, Vertex& end) {
  std::vector<ZoneEvent*> mZoneEvents = cProject->getZoneEvents(start, end);
  for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
    if (mZoneEvents[i]->getZone() != previousZone) {
      switch (mZoneEvents[i]->getType()) {
        case ZoneEvent::ENTERED: {
          if (cZone != NULL) {
            // TODO: Throw wobbly
            std::cout << "Multiple zone entered event should occur in a row" << std::endl;
            exit(1);
          }
          ISpindizzyGERALDType* mGERALDType = getElementType();
          cZone = mZoneEvents[i]->getZone();
          mGERALDType->executeZoneEnteredScript(cZone);
          break;
        }

        case ZoneEvent::EXITED: {
          if (cZone == NULL) {
            // TODO: Throw wobbly
            std::cout << "On event " << i << ": Zone exited event should never happen when not inside a zone" << std::endl;
            exit(1);
          }
          ISpindizzyGERALDType* mGERALDType = getElementType();
          mGERALDType->executeZoneExitedScript(cZone);
          cZone = NULL;
          break;
        }
      }
    }
  }
}

bool SpindizzyGERALD::isMovingNorth() {
  ISpindizzyGERALDType* mGERALDType = getElementType();
  ICamera* mCamera = mGERALDType->getCamera();
  if (mCamera != NULL) {
    float mCameraAngle = mCamera->getAngle();
    return mCameraAngle >= 40.0f && mCameraAngle <= 130.0f   ? *cPressedLeft
         : mCameraAngle >= -50.0f && mCameraAngle <= 40.0f   ? *cPressedForward
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? *cPressedRight
         :                                                     *cPressedBackward;
  }
  return false;
}

bool SpindizzyGERALD::isMovingEast() {
  ISpindizzyGERALDType* mGERALDType = getElementType();
  ICamera* mCamera = mGERALDType->getCamera();
  if (mCamera != NULL) {
    float mCameraAngle = mCamera->getAngle();
    return mCameraAngle >= 40.0f && mCameraAngle <= 130.0f   ? *cPressedForward
         : mCameraAngle >= -50.0f && mCameraAngle <= 40.0f   ? *cPressedRight
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? *cPressedBackward
         :                                                     *cPressedLeft;
  }
  return false;
}

bool SpindizzyGERALD::isMovingSouth() {
  ISpindizzyGERALDType* mGERALDType = getElementType();
  ICamera* mCamera = mGERALDType->getCamera();
  if (mCamera != NULL) {
    float mCameraAngle = mCamera->getAngle();
    return mCameraAngle >= 40.0f && mCameraAngle <= 130.0f   ? *cPressedRight
         : mCameraAngle >= -50.0f && mCameraAngle <= 40.0f   ? *cPressedBackward
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? *cPressedLeft
         :                                                     *cPressedForward;
  }
  return false;
}

bool SpindizzyGERALD::isMovingWest() {
  ISpindizzyGERALDType* mGERALDType = getElementType();
  ICamera* mCamera = mGERALDType->getCamera();
  if (mCamera != NULL) {
    float mCameraAngle = mCamera->getAngle();
    return mCameraAngle >= 40.0f && mCameraAngle <= 130.0f   ? *cPressedBackward
         : mCameraAngle >= -50.0f && mCameraAngle <= 40.0f   ? *cPressedLeft
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? *cPressedForward
         :                                                     *cPressedRight;
  }
  return false;
}

int mShitCount = 0;

void SpindizzyGERALD::getNewLocation(float ticks, Vertex* location, Vertex* momentum) {
  bool mMovingNorth = isMovingNorth();
  bool mMovingEast  = isMovingEast();
  bool mMovingSouth = isMovingSouth();
  bool mMovingWest  = isMovingWest();
  if (cCurrentSurface != NULL) {
    float mXSlopeMomentum = cCurrentSurface->getXAcceleration(cLocation->x, cLocation->y) * CRAFT_ACCELERATION;
    float mYSlopeMomentum = cCurrentSurface->getYAcceleration(cLocation->x, cLocation->y) * CRAFT_ACCELERATION;
    if (cLockEast != NULL && mMovingEast) {
      mXSlopeMomentum = max(mXSlopeMomentum, 0.0f);
    }
    if (cLockWest != NULL && mMovingWest) {
      mXSlopeMomentum = min(mXSlopeMomentum, 0.0f);
    }
    if (cLockNorth != NULL && mMovingNorth) {
      mYSlopeMomentum = max(mYSlopeMomentum, 0.0f);
    }
    if (cLockSouth != NULL && mMovingSouth) {
      mYSlopeMomentum = min(mYSlopeMomentum, 0.0f);
    }
    float mSurfaceFriction = 1.0f - cCurrentSurface->getSurfaceFriction();
    float mSurfaceGrip = cCurrentSurface->getSurfaceGrip();
    for (unsigned int i = 0; i < ticks; i++) {
      float mAcceleration = (*cThrust ? CRAFT_ACCELERATION * 2.0f : CRAFT_ACCELERATION);
      if (mMovingWest && cLockWest == NULL && !mMovingEast) {
        momentum->x -= mSurfaceGrip * ((!*cThrust && mAcceleration > 0.0f && mAcceleration < mXSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, -mXSlopeMomentum)
                     : mAcceleration);
      }
      if (mMovingEast && cLockEast == NULL && !mMovingWest) {
        momentum->x += mSurfaceGrip * ((!*cThrust && mAcceleration > 0.0f && mAcceleration < -mXSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, mXSlopeMomentum)
                     : mAcceleration);
      }
      if (mMovingSouth && cLockSouth == NULL && !mMovingNorth) {
        momentum->y -= mSurfaceGrip * ((!*cThrust && mAcceleration > 0.0f && mAcceleration < mYSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, -mYSlopeMomentum)
                     : mAcceleration);
      }
      if (mMovingNorth && cLockNorth == NULL && !mMovingSouth) {
        momentum->y += mSurfaceGrip * ((!*cThrust && mAcceleration > 0.0f && mAcceleration < -mYSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, mYSlopeMomentum)
                     : mAcceleration);
      }
      momentum->x += mXSlopeMomentum;
      momentum->y += mYSlopeMomentum;
      momentum->x *= mSurfaceFriction;
      momentum->y *= mSurfaceFriction;
      location->x += momentum->x;
      location->y += momentum->y;
      if (momentum->x > 0.0f) {
        cLockWest = NULL;
      } else if (momentum->x < 0.0f) {
        cLockEast = NULL;
      }
      if (momentum->y > 0.0f) {
        cLockSouth = NULL;
      } else if (momentum->y < 0.0f) {
        cLockNorth = NULL;
      }
    }
    location->z = cCurrentSurface->getHeightAt(location->x, location->y);
  } else {
    for (unsigned int i = 0; i < ticks; i++) {
      momentum->z += GRAVITY_STRENGTH;
      location->x += momentum->x;
      location->y += momentum->y;
      location->z += momentum->z;
    }
  }
}

bool SpindizzyGERALD::isValidEvent(ICollisionData* event) {
  if (event->getType() == ICollisionData::WALL_IMPACT) {
    return true;
  }
  if (event->getType() == ICollisionData::WALL_CLIP) {
    return true;
  }
  if (event->getType() == ICollisionData::SURFACE_LEAVE) {
    return true;
  }
  if (event->getType() == ICollisionData::SURFACE_MOUNT) {
    IRollableSurface* mEventSurface = event->getSurface();
    return mEventSurface != cCurrentSurface;
  }
  return false;
}

ICollisionData* SpindizzyGERALD::pollSlideEvent(Vertex& startLocation, Vertex& endLocation) {
  ICollisionData* mEvent = NULL;
  ICollidableWallSurface* mWallLocks[4] = {cLockNorth, cLockSouth, cLockEast, cLockWest};
  for (unsigned int i = 0; i < 4; i++) {
    if (mWallLocks[i] != NULL) {
      ICollisionData* mSurfaceLeftEvent = mWallLocks[i]->getSlidingEvent(startLocation, endLocation);
      if (mSurfaceLeftEvent != NULL) {
        if (mEvent == NULL || mSurfaceLeftEvent->getGradient() < mEvent->getGradient()) {
          mEvent = mSurfaceLeftEvent;
        }
      }
    }
  }
  return mEvent;
}

ICollisionData* SpindizzyGERALD::pollCollisionEvent(Vertex& startLocation, Vertex& endLocation) {
  ICollisionData* mEvent = NULL;
  if (cCurrentSurface != NULL) {
    ICollisionData* mSurfaceLeftEvent = cCurrentSurface->getRollingEvent(startLocation, endLocation);
    if (mSurfaceLeftEvent != NULL) {
      mEvent = mSurfaceLeftEvent;
    }
  }
  
  ICollisionData* mSlideEvent = pollSlideEvent(startLocation, endLocation);
  if (mSlideEvent != NULL && (mEvent == NULL || mSlideEvent->getGradient() < mEvent->getGradient())) {
    mEvent = mSlideEvent;
  }
  
  ISpindizzyGERALDType* mGERALDType = getElementType();
  ICollisionData* mOtherEvent = mGERALDType->getNextEvent(startLocation, endLocation, cCurrentSurface);
  if (mOtherEvent != NULL) {
    bool mValidEvent = isValidEvent(mOtherEvent);
    if (mValidEvent && (mEvent == NULL || mOtherEvent->getGradient() < mEvent->getGradient())) {
      mEvent = mOtherEvent;
    }
  }
  return mEvent;
}

void SpindizzyGERALD::updateRespawnData() {
  IRollableSurface::RespawnPossibility mRespawnPossility = cCurrentSurface->getRespawnPossibility();
  switch (mRespawnPossility) {
    case IRollableSurface::YES: {
      while (cRespawnSurfaceStack.size() > 1) {
        delete cRespawnSurfaceStack.top();
        cRespawnSurfaceStack.pop();
      }
      cRespawnData = cRespawnSurfaceStack.top();
      cRespawnData->cSurface = cCurrentSurface;
      break;
    }
    
    case IRollableSurface::CONDITIONAL: {
      cRespawnData = new RespawnData();
      cRespawnData->cSurface = cCurrentSurface;
      cRespawnSurfaceStack.push(cRespawnData);
      break;
    }
    
    case IRollableSurface::NO: {
      // Nothing to do
      break;
    }
  }
}

void SpindizzyGERALD::discoverZone(ICollidableWallSurface* wallSurface) {
  Vertex* mLocation = wallSurface->getLocation();
  IZone* mWallZone = cProject->getZone(*mLocation);
  delete mLocation; // TODO: Should this be done here?
  if (cZone != mWallZone) {
    ISpindizzyGERALDType* mGERALDType = getElementType();
    if (cZone != NULL) {
      mGERALDType->executeZoneExitedScript(cZone);
    }
    mGERALDType->executeZoneEnteredScript(mWallZone);
    mGERALDType->executeZoneExitedScript(mWallZone);
    if (cZone != NULL) {
      mGERALDType->executeZoneEnteredScript(cZone);
    }
  }
}

bool SpindizzyGERALD::processEvent(ICollisionData& event) {
  Vertex* mEventLocation = event.getEventLocation();
  switch (event.getType()) {
    case ICollisionData::SURFACE_LEAVE: {
      std::cout << " - - - We left: " << event.getSurface() << std::endl;
      cMomentum.z = cMomentum.x * -event.getXSlope() + cMomentum.y * -event.getYSlope(); 
      cJumpedFromRamp = cMomentum.z > 0.0f;
      cSurfaceLeaveVerticalMomentum = cMomentum.z;
      cCurrentSurface = NULL;
      break;
    }
    
    case ICollisionData::SURFACE_MOUNT: {
      IRollableSurface* mMountedSurface = event.getSurface();
      if (mMountedSurface != cCurrentSurface) {
        cMomentum.z = -cMomentum.z * mMountedSurface->getSurfaceBounce();
        if (cCycleBounce || cMomentum.z == 0.0f || cPeakHeight <= mMountedSurface->getHeightAt(mEventLocation->x, mEventLocation->y)) {
          ISpindizzyGERALDType* mGERALDType = getElementType();
          mGERALDType->executeFallImpactScript(cPeakHeight - mEventLocation->z, this);
          if (cRespawning) {
            return false;      
          }
          cCurrentSurface = mMountedSurface;
          cCurrentSurface->notifyContact();
          updateRespawnData();
          std::cout << " + + + We entd: " << cCurrentSurface << std::endl;
          cPeakHeight = cMapBottom;
        } else {
          Vertex mDummyLocation = *mEventLocation;
          cCurrentSurface = mMountedSurface;
          getNewLocation(BOUNCE_CONTROL_TIME, &mDummyLocation, &cMomentum);
          cCurrentSurface->notifyImpact();
          cCurrentSurface = NULL;
          std::cout << "============================" << std::endl;
          std::cout << "Bounced to " << cMomentum.z << "...!" << std::endl;
          std::cout << "Peak height is " << cPeakHeight << "...!" << std::endl;
          std::cout << "Current height is " << mEventLocation->z << "...!" << std::endl;
          float mFallHeight = (cPeakHeight - mEventLocation->z) + (cJumpedFromRamp ? 0.0f : 0.335f);
          std::cout << "Fall height is " << mFallHeight << "...!" << std::endl;
          float mIdealProjection = sqrt(mFallHeight * 2 * -GRAVITY_STRENGTH);
          std::cout << "Ideal projection is " << mIdealProjection << "...!" << std::endl;
          std::cout << "Jumped from ramp? " << cJumpedFromRamp << "...!" << std::endl;
          cMomentum.z = mIdealProjection;
          cCycleBounce = true;
        }
      }
      break;
    }
    
    case ICollisionData::WALL_IMPACT: {
      
      // If GERALD hits a wall whilst running up a slope, he might leave the slope
      if (cCurrentSurface != NULL) {
        cMomentum.z = cMomentum.x * -cCurrentSurface->getXAcceleration(mEventLocation->x, mEventLocation->y) + cMomentum.y * -cCurrentSurface->getYAcceleration(mEventLocation->x, mEventLocation->y); 
        if (cMomentum.z > 0.0f) {
          cJumpedFromRamp = true;
          cSurfaceLeaveVerticalMomentum = cMomentum.z;
          cCurrentSurface = NULL;
        }
      }
      
      // Handle wall bounce
      ICollidableWallSurface* mWallSurface = event.getWallSurface();
      ICollidableWallSurface::WallFaceDirection mFaceDirection = mWallSurface->getWallFaceDirection();
      float mSurfaceBounce = mWallSurface->getSurfaceBounce();
      if (mFaceDirection == ICollidableWallSurface::FACE_NORTH || mFaceDirection == ICollidableWallSurface::FACE_SOUTH) {
        cMomentum.y = -cMomentum.y * mSurfaceBounce;
        if (fabs(cMomentum.y) < CRAFT_ACCELERATION) {
          (mFaceDirection == ICollidableWallSurface::FACE_NORTH ? cLockSouth : cLockNorth) = mWallSurface;
          cMomentum.y = 0.0f;
        }
      } else {
        cMomentum.x = -cMomentum.x * mSurfaceBounce;
        if (fabs(cMomentum.x) < CRAFT_ACCELERATION) {
          (mFaceDirection == ICollidableWallSurface::FACE_EAST ? cLockWest : cLockEast) = mWallSurface;
          cMomentum.x = 0.0f;
        }
      }
      discoverZone(mWallSurface);
      break;
    }
    
    case ICollisionData::WALL_CLIP: {
      ICollidableWallSurface* mWallSurface = event.getWallSurface();
      discoverZone(mWallSurface);
      break;
    }
    
    case ICollisionData::WALL_LEAVE: {
      ICollidableWallSurface* mWallSurface = event.getWallSurface();
      ICollidableWallSurface::WallFaceDirection mFaceDirection = mWallSurface->getWallFaceDirection();
      switch (mFaceDirection) {
        case ICollidableWallSurface::FACE_NORTH: cLockSouth = NULL; break;
        case ICollidableWallSurface::FACE_EAST:  cLockWest  = NULL; break;
        case ICollidableWallSurface::FACE_SOUTH: cLockNorth = NULL; break;
        case ICollidableWallSurface::FACE_WEST:  cLockEast  = NULL; break;
      }
      break;
    }
  }
  updateLocation(*mEventLocation);
  return true;
}

SpindizzyGERALD::RespawnData* SpindizzyGERALD::getRespawnData() {
  SpindizzyGERALD::RespawnData* mRespawnData = cRespawnSurfaceStack.top();
  while (!mRespawnData->cSurface->isRespawnPossibleNow()) {
    cRespawnSurfaceStack.pop();
    mRespawnData = cRespawnSurfaceStack.top();
  }
  return mRespawnData;
}

void SpindizzyGERALD::destroy() {
  cDestroyLocation = *cLocation;
  cRespawnAnimation = 0.0f;
  cRespawning = true;
  cZone = NULL;
}

void SpindizzyGERALD::respawn() {
  SpindizzyGERALD::RespawnData* mRespawnData = getRespawnData();
  cLocation->x = mRespawnData->cX;
  cLocation->y = mRespawnData->cY;
  cCurrentSurface = mRespawnData->cSurface;
  cLocation->z = cCurrentSurface->getHeightAt(cLocation->x, cLocation->y);
  cZone = cProject->getZone(*cLocation);
  ISpindizzyGERALDType* mGERALDType = getElementType();
  mGERALDType->executeZoneEnteredScript(cZone);
  mGERALDType->executeRespawnScript();
  cMomentum.x = 0.0f;
  cMomentum.y = 0.0f;
  cMomentum.z = 0.0f;
  cRespawning = false;
  cPeakHeight = cMapBottom;
}

void SpindizzyGERALD::checkFall() {
  if (cLocation->z < cMapBottom) {
    destroy();
  }
}

void SpindizzyGERALD::updateRespawnLocation() {
  if (cCurrentSurface == cRespawnData->cSurface) {
    cRespawnData->cX = round(cLocation->x);
    cRespawnData->cY = round(cLocation->y);
  }
}

void SpindizzyGERALD::updateLocation(Vertex& location) {
  IZone* mPreviousZone = cZone;
  ISpindizzyGERALDType* mGERALDType = getElementType();
  mGERALDType->collect(this, *cLocation, location);
  if (cZone != NULL) {
    checkCurrentZoneEvents(*cLocation, location);
  }
  if (cZone == NULL) {
    checkMapZoneEvents(mPreviousZone, *cLocation, location);
  }
  cLocation->x = location.x;
  cLocation->y = location.y;
  cLocation->z = location.z;
  if (cCurrentSurface == NULL && cLocation->z > cPeakHeight) {
    cPeakHeight = cLocation->z;
  }    
  updateRespawnLocation();
}

void SpindizzyGERALD::updateAlive(unsigned int ticks) {
  cFastEvents = 0;
  cCycleBounce = false;
  float mTicks = static_cast<float>(ticks);
  Vertex mProposedLocation = *cLocation;
  Vertex mProposedMomentum = cMomentum;
  getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
  ICollisionData* mNextEvent = pollCollisionEvent(*cLocation, mProposedLocation);
  bool mProposedLocationSet = false;
  while (mNextEvent != NULL) {
    if (!processEvent(*mNextEvent)) {
      mTicks = 0.0f;
      mNextEvent = NULL;
    } else {
      float mOldTicks = mTicks;
      mTicks -= mTicks * mNextEvent->getGradient();
      if (mOldTicks - mTicks < 0.1f) {
        cFastEvents++;
      } else {
        cFastEvents = 0;
      }
      if (mTicks > 0.0f) {
        mProposedLocation = *cLocation;
        mProposedMomentum = cMomentum;
        getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
      }

      mNextEvent = pollCollisionEvent(*cLocation, mProposedLocation);
      if (cFastEvents > 8 && mNextEvent != NULL && mNextEvent->getType() == ICollisionData::SURFACE_LEAVE) {
        mProposedLocation = *cLocation;
        mProposedMomentum = cMomentum;
        getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
        cCurrentSurface = mNextEvent->getSurface();
        if (cFastEvents < 16) {
          cCurrentSurface->getRestingLocation(mProposedLocation);
        } else {
          mProposedLocation = *cLocation;
        }
        mProposedLocationSet = true;
        mNextEvent = pollCollisionEvent(*cLocation, mProposedLocation);
      }
    }
  }
  if (!mProposedLocationSet) {
    mProposedLocation = *cLocation;
    getNewLocation(mTicks, &mProposedLocation, &cMomentum);
  } else {
    cMomentum = mProposedMomentum;
  }
  updateLocation(mProposedLocation);
  checkFall();
}

void SpindizzyGERALD::updateDead(unsigned int ticks) {
  cRespawnAnimation += ticks / 500.0f;
  if (cRespawnAnimation >= 1.0f) {
    respawn();
  } else {
    SpindizzyGERALD::RespawnData* mRespawnData = getRespawnData();
    cLocation->x = sine(cDestroyLocation.x, mRespawnData->cX, cRespawnAnimation);
    cLocation->y = sine(cDestroyLocation.y, mRespawnData->cY, cRespawnAnimation);
    cLocation->z = sine(cDestroyLocation.z, mRespawnData->cSurface->getHeightAt(mRespawnData->cX, mRespawnData->cY), cRespawnAnimation);
  }
}

void SpindizzyGERALD::update(unsigned int ticks) {
  if (cRespawning) {
    updateDead(ticks);
  } else {
    ISpindizzyGERALDSet* mGERALDSet = getElementSet();
    if (!mGERALDSet->isLocked()) {
      updateAlive(ticks);
    }
  }
}

void SpindizzyGERALD::stop() {
  std::cout << "Brake called!" << std::endl;
  if (cCurrentSurface != NULL && cCurrentSurface->getSurfaceFriction() > 0.0f) {
    cMomentum.x = 0.0f;
    cMomentum.y = 0.0f;
    cMomentum.z = 0.0f;
  }
}

void SpindizzyGERALD::render() {
  if (!cRespawning) {
    glPushMatrix();
    cGERALDModel->render();
    glPopMatrix();
  }
}

void SpindizzyGERALD::saveInstance(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& relative) {
  cStartLocation.saveRelative(node, relative);  
}

void SpindizzyGERALD::setDirty() {
  // Nothing to do
}

void SpindizzyGERALD::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& relative) {
  ISpindizzyGERALDType* mGERALDType = getElementType();
  std::string mInstanceName = mGERALDType->getInstanceName(this);
  if (mInstanceName != "") {
    node->addAttribute("instance", mInstanceName);
  } else {
    saveInstance(node, resourceLocator, relative);
  }
}

void SpindizzyGERALD::readData(DOMNodeWrapper* node) {
  cStartLocation.set(node);
  cLocation->x = cStartLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->y = cStartLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->z = cStartLocation.z;
}

void SpindizzyGERALD::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  ISpindizzyGERALDType* mGERALDType = getElementType();
  readData(node);
  cGERALDModel = mGERALDType->createModel(cLocation);
}
