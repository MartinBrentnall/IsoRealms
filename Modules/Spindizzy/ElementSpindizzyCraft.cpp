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
#include "ElementSpindizzyCraft.h"

const float ElementSpindizzyCraft::CRAFT_ACCELERATION = 0.00003f;
const float ElementSpindizzyCraft::GRAVITY_STRENGTH = -0.0002f;
const unsigned int ElementSpindizzyCraft::INIT_REGISTER_BLOCKS = 0;
const unsigned int ElementSpindizzyCraft::INIT_PROCESS_BLOCKS = 1;
const unsigned int ElementSpindizzyCraft::INIT_REGISTER_SURFACES = 2;
const unsigned int ElementSpindizzyCraft::INIT_USE_SURFACES = 3;
const unsigned int ElementSpindizzyCraft::BOUNCE_CONTROL_TIME = 40;

const float ElementSpindizzyCraft::STEP_REACH_NORMAL = 0.5f;
const float ElementSpindizzyCraft::STEP_REACH_BOUNCE = 0.8f;

ElementSpindizzyCraft::ElementSpindizzyCraft(ISpindizzyGERALDType* elementType, IResourceAccessor* resources, DOMNodeWrapper* node) {
  cCraftType = elementType;
  cLocation = new Vertex();
  cAppeared = false;
  readData(node);
  cStepReach = STEP_REACH_NORMAL;
}

ElementSpindizzyCraft::ElementSpindizzyCraft(ISpindizzyGERALDType* elementType, IResourceAccessor* resources, BlockLocation* startLocation) {
  cCraftType = elementType;
  cStartLocation = BlockLocation(*startLocation);
  cLocation = new Vertex();
  cLocation->x = cStartLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->y = cStartLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->z = cStartLocation.z;
  cGERALDModel = elementType->createModel(cLocation);
  cAppeared = false;
  cStepReach = STEP_REACH_NORMAL;
}

ElementSpindizzyCraft::ElementSpindizzyCraft(ISpindizzyGERALDType* elementType) {
  cCraftType = elementType;
  cLocation = new Vertex();
  cAppeared = false;
  cStepReach = STEP_REACH_NORMAL;
}

void ElementSpindizzyCraft::setModelType(I3DModelType* oldModelType, I3DModelType* newModelType) {
  oldModelType->destroyModel(cGERALDModel);
  cGERALDModel = newModelType->createModel(cLocation);
}

void ElementSpindizzyCraft::setElementContainer(IElementContainer* container) {
  cContainer = container;
}

IElementContainer* ElementSpindizzyCraft::getElementContainer() {
  return cContainer;
}

void ElementSpindizzyCraft::entered(IBoundary* boundary) {
  // Nothing to do.  TODO: Something to do?
}

void ElementSpindizzyCraft::setArguments() {
  // TODO: Something to do?
}

void ElementSpindizzyCraft::unsetArguments() {
  // TODO: Something to do?
}

std::vector<IVisualElement*> ElementSpindizzyCraft::getStaticVisuals() {
  return std::vector<IVisualElement*>();
}

std::string ElementSpindizzyCraft::getTypeName() {
  return "Spindizzy Craft"; // TODO
}
  
std::vector<IObjectProperty*> ElementSpindizzyCraft::getProperties(IComponentContainer* windowWorkspace) {
  return std::vector<IObjectProperty*>();
}

IElementType* ElementSpindizzyCraft::getElementType() {
  return cCraftType;
}

bool ElementSpindizzyCraft::initElement(IUniverse* universe, unsigned int pass) {
  switch (pass) {
    case INIT_REGISTER_BLOCKS: {
//       if (cMap != nullptr) {
//         cZone = (*cMap)->getZone(*cLocation);
//         mGERALDType->executeZoneEnteredScript(cZone);
//       }
//       return false;
//     }
// 
//     case INIT_USE_SURFACES: {
      cCurrentSurface = cCraftType->getSurfaceAt(*cLocation, cStepReach);
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

// void ElementSpindizzyCraft::checkCurrentZoneEvents(Vertex& start, Vertex& end) {
//   std::vector<ZoneEvent*> mZoneEvents = cZone->getZoneEvents(start, end);
//   for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
//     switch (mZoneEvents[i]->getType()) {
//       case ZoneEvent::EXITED: {
//         cCraftType->executeZoneExitedScript(cZone);
//         cZone = nullptr;
//         break;
//       }
// 
//       default: {
//         // TODO: Throw wobbly
//         std::cout << "Zone entered event should never happen for the current zone" << std::endl;
//         exit(1);
//       }
//     }
//   }
// }

Vertex* ElementSpindizzyCraft::getLocation() {
  return cLocation;
}

// void ElementSpindizzyCraft::checkMapZoneEvents(IZone* previousZone, Vertex& start, Vertex& end) {
//   std::vector<ZoneEvent*> mZoneEvents = (*cMap)->getZoneEvents(start, end);
//   for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
//     if (mZoneEvents[i]->getZone() != previousZone) {
//       switch (mZoneEvents[i]->getType()) {
//         case ZoneEvent::ENTERED: {
//           if (cZone != nullptr) {
//             // TODO: Throw wobbly
//             std::cout << "Multiple zone entered event should occur in a row" << std::endl;
//             exit(1);
//           }
//           cZone = mZoneEvents[i]->getZone();
//           cCraftType->executeZoneEnteredScript(cZone);
//           break;
//         }
// 
//         case ZoneEvent::EXITED: {
//           if (cZone == nullptr) {
//             // TODO: Throw wobbly
//             std::cout << "On event " << i << ": Zone exited event should never happen when not inside a zone" << std::endl;
//             exit(1);
//           }
//           cCraftType->executeZoneExitedScript(cZone);
//           cZone = nullptr;
//           break;
//         }
//       }
//     }
//   }
// }

bool ElementSpindizzyCraft::isMovingNorth() {
  ICamera* mCamera = cCraftType->getCamera();
  if (mCamera != nullptr) {
    float mCameraAngle = mCamera->getAngle();
    return mCameraAngle >= 40.0f && mCameraAngle <= 130.0f   ? *cPressedLeft
         : mCameraAngle >= -50.0f && mCameraAngle <= 40.0f   ? *cPressedForward
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? *cPressedRight
         :                                                     *cPressedBackward;
  }
  return false;
}

bool ElementSpindizzyCraft::isMovingEast() {
  ICamera* mCamera = cCraftType->getCamera();
  if (mCamera != nullptr) {
    float mCameraAngle = mCamera->getAngle();
    return mCameraAngle >= 40.0f && mCameraAngle <= 130.0f   ? *cPressedForward
         : mCameraAngle >= -50.0f && mCameraAngle <= 40.0f   ? *cPressedRight
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? *cPressedBackward
         :                                                     *cPressedLeft;
  }
  return false;
}

bool ElementSpindizzyCraft::isMovingSouth() {
  ICamera* mCamera = cCraftType->getCamera();
  if (mCamera != nullptr) {
    float mCameraAngle = mCamera->getAngle();
    return mCameraAngle >= 40.0f && mCameraAngle <= 130.0f   ? *cPressedRight
         : mCameraAngle >= -50.0f && mCameraAngle <= 40.0f   ? *cPressedBackward
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? *cPressedLeft
         :                                                     *cPressedForward;
  }
  return false;
}

bool ElementSpindizzyCraft::isMovingWest() {
  ICamera* mCamera = cCraftType->getCamera();
  if (mCamera != nullptr) {
    float mCameraAngle = mCamera->getAngle();
    return mCameraAngle >= 40.0f && mCameraAngle <= 130.0f   ? *cPressedBackward
         : mCameraAngle >= -50.0f && mCameraAngle <= 40.0f   ? *cPressedLeft
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? *cPressedForward
         :                                                     *cPressedRight;
  }
  return false;
}

int mShitCount = 0;

void ElementSpindizzyCraft::getNewLocation(float ticks, Vertex* location, Vertex* momentum) {
  bool mMovingNorth = isMovingNorth();
  bool mMovingEast  = isMovingEast();
  bool mMovingSouth = isMovingSouth();
  bool mMovingWest  = isMovingWest();
  if (cCurrentSurface != nullptr) {
    float mXSlopeMomentum = cCurrentSurface->getXAcceleration(cLocation->x, cLocation->y) * CRAFT_ACCELERATION;
    float mYSlopeMomentum = cCurrentSurface->getYAcceleration(cLocation->x, cLocation->y) * CRAFT_ACCELERATION;
    if (cLockEast != nullptr && mMovingEast) {
      mXSlopeMomentum = max(mXSlopeMomentum, 0.0f);
    }
    if (cLockWest != nullptr && mMovingWest) {
      mXSlopeMomentum = min(mXSlopeMomentum, 0.0f);
    }
    if (cLockNorth != nullptr && mMovingNorth) {
      mYSlopeMomentum = max(mYSlopeMomentum, 0.0f);
    }
    if (cLockSouth != nullptr && mMovingSouth) {
      mYSlopeMomentum = min(mYSlopeMomentum, 0.0f);
    }
    float mSurfaceFriction = 1.0f - cCurrentSurface->getSurfaceFriction();
    float mSurfaceGrip = cCurrentSurface->getSurfaceGrip();
    for (unsigned int i = 0; i < ticks; i++) {
      float mAcceleration = (*cThrust ? CRAFT_ACCELERATION * 2.0f : CRAFT_ACCELERATION);
      if (mMovingWest && cLockWest == nullptr && !mMovingEast) {
        momentum->x -= mSurfaceGrip * ((!*cThrust && mAcceleration > 0.0f && mAcceleration < mXSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, -mXSlopeMomentum)
                     : mAcceleration);
      }
      if (mMovingEast && cLockEast == nullptr && !mMovingWest) {
        momentum->x += mSurfaceGrip * ((!*cThrust && mAcceleration > 0.0f && mAcceleration < -mXSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, mXSlopeMomentum)
                     : mAcceleration);
      }
      if (mMovingSouth && cLockSouth == nullptr && !mMovingNorth) {
        momentum->y -= mSurfaceGrip * ((!*cThrust && mAcceleration > 0.0f && mAcceleration < mYSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, -mYSlopeMomentum)
                     : mAcceleration);
      }
      if (mMovingNorth && cLockNorth == nullptr && !mMovingSouth) {
        momentum->y += mSurfaceGrip * ((!*cThrust && mAcceleration > 0.0f && mAcceleration < -mYSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, mYSlopeMomentum)
                     : mAcceleration);
      }
      
      // Disallow momentum above/below zero when craft is forced against wall by a slope.
      momentum->y = cLockNorth != nullptr ? std::min(momentum->y + mYSlopeMomentum, 0.0) 
                  : cLockSouth != nullptr ? std::max(momentum->y + mYSlopeMomentum, 0.0)
                  :                                  momentum->y + mYSlopeMomentum;
      momentum->x = cLockEast  != nullptr ? std::min(momentum->x + mXSlopeMomentum, 0.0) 
                  : cLockWest  != nullptr ? std::max(momentum->x + mXSlopeMomentum, 0.0)
                  :                                  momentum->x + mXSlopeMomentum;;
      momentum->x *= mSurfaceFriction;
      momentum->y *= mSurfaceFriction;
      location->x += momentum->x;
      location->y += momentum->y;
      if (momentum->x > 0.0f) {
        cLockWest = nullptr;
      } else if (momentum->x < 0.0f) {
        cLockEast = nullptr;
      }
      if (momentum->y > 0.0f) {
        cLockSouth = nullptr;
      } else if (momentum->y < 0.0f) {
        cLockNorth = nullptr;
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

bool ElementSpindizzyCraft::isValidEvent(ICollisionData* event) {
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

ICollisionData* ElementSpindizzyCraft::pollSlideEvent(Vertex& startLocation, Vertex& endLocation) {
  ICollisionData* mEvent = nullptr;
  ICollidableWallSurface* mWallLocks[4] = {cLockNorth, cLockSouth, cLockEast, cLockWest};
  for (unsigned int i = 0; i < 4; i++) {
    if (mWallLocks[i] != nullptr) {
      ICollisionData* mSurfaceLeftEvent = mWallLocks[i]->getSlidingEvent(startLocation, endLocation);
      if (mSurfaceLeftEvent != nullptr) {
        if (mEvent == nullptr || mSurfaceLeftEvent->getGradient() < mEvent->getGradient()) {
          mEvent = mSurfaceLeftEvent;
        }
      }
    }
  }
  return mEvent;
}

ICollisionData* ElementSpindizzyCraft::pollCollisionEvent(Vertex& startLocation, Vertex& endLocation) {
  ICollisionData* mEvent = nullptr;
  if (cCurrentSurface != nullptr) {
    ICollisionData* mSurfaceLeftEvent = cCurrentSurface->getRollingEvent(startLocation, endLocation);
    if (mSurfaceLeftEvent != nullptr) {
      mEvent = mSurfaceLeftEvent;
    }
  }
  
  ICollisionData* mSlideEvent = pollSlideEvent(startLocation, endLocation);
  if (mSlideEvent != nullptr && (mEvent == nullptr || mSlideEvent->getGradient() < mEvent->getGradient())) {
    mEvent = mSlideEvent;
  }
  
  ICollisionData* mOtherEvent = cCraftType->getNextEvent(startLocation, endLocation, cCurrentSurface, cStepReach);
  if (mOtherEvent != nullptr) {
    bool mValidEvent = isValidEvent(mOtherEvent);
    if (mValidEvent && (mEvent == nullptr || mOtherEvent->getGradient() < mEvent->getGradient())) {
      mEvent = mOtherEvent;
    }
  }
  return mEvent;
}

void ElementSpindizzyCraft::updateRespawnData() {
  IRollableSurface::RespawnPossibility mRespawnPossility = cCurrentSurface->getRespawnPossibility();
  switch (mRespawnPossility) {
    case IRollableSurface::YES: {
      while (cRespawnSurfaceStack.size() > 1) {
        delete cRespawnSurfaceStack.top();
        cRespawnSurfaceStack.pop();
      }
      if (cRespawnSurfaceStack.empty()) {
        std::cout << "ERROR: Craft not yet initialised" << std::endl;
        exit(1);
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

void ElementSpindizzyCraft::discoverZone(ICollidableWallSurface* wallSurface) {
//   Vertex* mLocation = wallSurface->getLocation();
//   IZone* mWallZone = (*cMap)->getZone(*mLocation);
//   delete mLocation; // TODO: Should this be done here?
//   if (cZone != mWallZone) {
//     if (cZone != nullptr) {
//       cCraftType->executeZoneExitedScript(cZone);
//     }
//     cCraftType->executeZoneEnteredScript(mWallZone);
//     cCraftType->executeZoneExitedScript(mWallZone);
//     if (cZone != nullptr) {
//       cCraftType->executeZoneEnteredScript(cZone);
//     }
//   }
}

bool ElementSpindizzyCraft::processEvent(ICollisionData& event) {
  Vertex* mEventLocation = event.getEventLocation();
  switch (event.getType()) {
    case ICollisionData::SURFACE_LEAVE: {
//      std::cout << " - - - We left: " << event.getSurface() << std::endl;
      cMomentum.z = cMomentum.x * -event.getXSlope() + cMomentum.y * -event.getYSlope(); 
      cJumpedFromRamp = cMomentum.z > 0.0f;
      cSurfaceLeaveVerticalMomentum = cMomentum.z;
      cCurrentSurface = nullptr;
      break;
    }
    
    case ICollisionData::SURFACE_MOUNT: {
      IRollableSurface* mMountedSurface = event.getSurface();
      if (mMountedSurface != cCurrentSurface) {
        cMomentum.z = -cMomentum.z * mMountedSurface->getSurfaceBounce();
        if (cCycleBounce || cMomentum.z == 0.0f || cPeakHeight <= mMountedSurface->getHeightAt(mEventLocation->x, mEventLocation->y)) {
          cStepReach = STEP_REACH_NORMAL;
          cCraftType->executeFallImpactScript(cPeakHeight - mEventLocation->z, this);
          if (cRespawning) {
            return false;      
          }
          cCurrentSurface = mMountedSurface;
          cCurrentSurface->notifyContact();
          updateRespawnData();
//          std::cout << " + + + We entd: " << cCurrentSurface << std::endl;
          cPeakHeight = cMapBottom;
        } else {
          cStepReach = STEP_REACH_BOUNCE;
          Vertex mDummyLocation = *mEventLocation;
          cCurrentSurface = mMountedSurface;
          getNewLocation(BOUNCE_CONTROL_TIME, &mDummyLocation, &cMomentum);
          cCurrentSurface->notifyImpact();
          cCurrentSurface = nullptr;
//          std::cout << "============================" << std::endl;
//          std::cout << "Bounced to " << cMomentum.z << "...!" << std::endl;
//          std::cout << "Peak height is " << cPeakHeight << "...!" << std::endl;
//          std::cout << "Current height is " << mEventLocation->z << "...!" << std::endl;
          float mFallHeight = (cPeakHeight - mEventLocation->z) + 0.335f;// (cJumpedFromRamp ? 0.0f : 0.335f);
//          std::cout << "Fall height is " << mFallHeight << "...!" << std::endl;
          float mIdealProjection = sqrt(mFallHeight * 2 * -GRAVITY_STRENGTH);
//          std::cout << "Ideal projection is " << mIdealProjection << "...!" << std::endl;
//          std::cout << "Jumped from ramp? " << cJumpedFromRamp << "...!" << std::endl;
          cMomentum.z = mIdealProjection;
          cCycleBounce = true;
        }
      }
      break;
    }
    
    case ICollisionData::WALL_IMPACT: {
      
      // If GERALD hits a wall whilst running up a slope, he might leave the slope
      if (cCurrentSurface != nullptr) {
        cMomentum.z = cMomentum.x * -cCurrentSurface->getXAcceleration(mEventLocation->x, mEventLocation->y) + cMomentum.y * -cCurrentSurface->getYAcceleration(mEventLocation->x, mEventLocation->y); 
        if (cMomentum.z > 0.0f) {
          cJumpedFromRamp = true;
          cSurfaceLeaveVerticalMomentum = cMomentum.z;
          cCurrentSurface = nullptr;
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
        case ICollidableWallSurface::FACE_NORTH: cLockSouth = nullptr; break;
        case ICollidableWallSurface::FACE_EAST:  cLockWest  = nullptr; break;
        case ICollidableWallSurface::FACE_SOUTH: cLockNorth = nullptr; break;
        case ICollidableWallSurface::FACE_WEST:  cLockEast  = nullptr; break;
      }
      break;
    }
  }
  updateLocation(*mEventLocation);
  return true;
}

ElementSpindizzyCraft::RespawnData* ElementSpindizzyCraft::getRespawnData() {
  RespawnData* mRespawnData = cRespawnSurfaceStack.top();
  while (!mRespawnData->cSurface->isRespawnPossibleNow()) {
    cRespawnSurfaceStack.pop();
    mRespawnData = cRespawnSurfaceStack.top();
  }
  return mRespawnData;
}

void ElementSpindizzyCraft::destroy() {
  cDestroyLocation = *cLocation;
  cCraftType->notifyDisappearance(this, *cLocation);
  cRespawnAnimation = 0.0f;
  cRespawning = true;
}

void ElementSpindizzyCraft::respawn() {
  RespawnData* mRespawnData = getRespawnData();
  cLocation->x = mRespawnData->cX;
  cLocation->y = mRespawnData->cY;
  cCurrentSurface = mRespawnData->cSurface;
  cLocation->z = cCurrentSurface->getHeightAt(cLocation->x, cLocation->y);
  cCraftType->executeRespawnScript();
  cCraftType->notifyAppearance(this, *cLocation);
  cMomentum.x = 0.0f;
  cMomentum.y = 0.0f;
  cMomentum.z = 0.0f;
  cRespawning = false;
  cPeakHeight = cMapBottom;
}

void ElementSpindizzyCraft::checkFall() {
  if (cLocation->z < cMapBottom) {
    destroy();
  }
}

void ElementSpindizzyCraft::updateRespawnLocation() {
  if (cCurrentSurface == cRespawnData->cSurface) {
    cRespawnData->cX = round(cLocation->x);
    cRespawnData->cY = round(cLocation->y);
  }
}

void ElementSpindizzyCraft::updateLocation(Vertex& location) {
//  IZone* mPreviousZone = cZone;
  cCraftType->notifyMovement(this, *cLocation, location);
//   if (cZone != nullptr) {
//     checkCurrentZoneEvents(*cLocation, location);
//   }
//   if (cZone == nullptr) {
//     checkMapZoneEvents(mPreviousZone, *cLocation, location);
//   }
  cLocation->x = location.x;
  cLocation->y = location.y;
  cLocation->z = location.z;
  if (cCurrentSurface == nullptr && cLocation->z > cPeakHeight) {
    cPeakHeight = cLocation->z;
  }    
  updateRespawnLocation();
}

void ElementSpindizzyCraft::updateAlive(unsigned int ticks) {
  cFastEvents = 0;
  cCycleBounce = false;
  float mTicks = static_cast<float>(ticks);
  Vertex mProposedLocation = *cLocation;
  Vertex mProposedMomentum = cMomentum;
  getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
  ICollisionData* mNextEvent = pollCollisionEvent(*cLocation, mProposedLocation);
  bool mProposedLocationSet = false;
  while (mNextEvent != nullptr) {
    if (!processEvent(*mNextEvent)) {
      mTicks = 0.0f;
      mNextEvent = nullptr;
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
      if (cFastEvents > 8 && mNextEvent != nullptr && mNextEvent->getType() == ICollisionData::SURFACE_LEAVE) {
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

void ElementSpindizzyCraft::updateDead(unsigned int ticks) {
  cRespawnAnimation += ticks / 500.0f;
  if (cRespawnAnimation >= 1.0f) {
    respawn();
  } else {
    RespawnData* mRespawnData = getRespawnData();
    cLocation->x = sine(cDestroyLocation.x, mRespawnData->cX, cRespawnAnimation);
    cLocation->y = sine(cDestroyLocation.y, mRespawnData->cY, cRespawnAnimation);
    cLocation->z = sine(cDestroyLocation.z, mRespawnData->cSurface->getHeightAt(mRespawnData->cX, mRespawnData->cY), cRespawnAnimation);
  }
}

void ElementSpindizzyCraft::updateRuntime(unsigned int ticks) {
  if (!cAppeared) {
    cCraftType->notifyAppearance(this, *cLocation);
    cAppeared = true;
  }
  if (cRespawning) {
    updateDead(ticks);
  } else {
    ISpindizzyGERALDSet* mGERALDSet = cCraftType->getSpindizzyCraftInterface();
    if (!mGERALDSet->isLocked()) {
      updateAlive(ticks);
    }
  }
}

void ElementSpindizzyCraft::updateEditing(unsigned int milliseconds) {
  // Nothing to do
}

void ElementSpindizzyCraft::stop() {
  if (cCurrentSurface != nullptr && cCurrentSurface->getSurfaceFriction() > 0.0f) {
    cMomentum.x = 0.0f;
    cMomentum.y = 0.0f;
    cMomentum.z = 0.0f;
  }
}

IUniverse* ElementSpindizzyCraft::getUniverse() {
  return cContainer->getUniverse();
}

void ElementSpindizzyCraft::renderRuntime() {
  if (!cRespawning) {
    glPushMatrix();
    cGERALDModel->render();
    glPopMatrix();
  }
}

bool ElementSpindizzyCraft::renderSelectionHighlight() {
  return false;
}

void ElementSpindizzyCraft::setDirty() {
  // Nothing to do
}

IElementBounds* ElementSpindizzyCraft::getBounds() {
  return this;
}

void ElementSpindizzyCraft::reset() {
  cLocation->x = cStartLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->y = cStartLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->z = cStartLocation.z;
  cMomentum.x = 0.0;
  cMomentum.y = 0.0;
  cMomentum.z = 0.0;
  cRespawning = false;
  cAppeared = false;
  cStepReach = STEP_REACH_NORMAL;
  while (!cRespawnSurfaceStack.empty()) {
    delete cRespawnSurfaceStack.top();
    cRespawnSurfaceStack.pop();
  }
}

float ElementSpindizzyCraft::getWest() {
  return cLocation->x - IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyCraft::getEast() {
  return cLocation->x + IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyCraft::getSouth() {
  return cLocation->y - IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyCraft::getNorth() {
  return cLocation->y + IsoRealmsConstants::BLOCK_RADIUS;
}

float ElementSpindizzyCraft::getBottom() {
  return cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT;
}

float ElementSpindizzyCraft::getTop() {
  return (cLocation->z + 2.0) * IsoRealmsConstants::BLOCK_HEIGHT;
}

void ElementSpindizzyCraft::saveInstance(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& relative) {
  cStartLocation.saveRelative(node, relative);
}

void ElementSpindizzyCraft::save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& relative) {
  std::string mInstanceName = cCraftType->getInstanceName(this);
  if (mInstanceName != "") {
    node->addAttribute("instance", mInstanceName);
  } else {
    saveInstance(node, resourceLocator, relative);
  }
}

void ElementSpindizzyCraft::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}

void ElementSpindizzyCraft::readData(DOMNodeWrapper* node) {
  cStartLocation.set(node);
  cLocation->x = cStartLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->y = cStartLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation->z = cStartLocation.z;
}

void ElementSpindizzyCraft::setModel(I3DModelType* modelType) {
  cGERALDModel = modelType->createModel(cLocation);
}

void ElementSpindizzyCraft::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  readData(node);
//  cGERALDModel = cCraftType->createModel(cLocation);
  cRespawning = false;
  cLockNorth = nullptr;
  cLockSouth = nullptr;
  cLockEast = nullptr;
  cLockWest = nullptr;
  cPressedForward  = resources->getDigitalInput("Move North");
  cPressedRight    = resources->getDigitalInput("Move East");
  cPressedBackward = resources->getDigitalInput("Move South");
  cPressedLeft     = resources->getDigitalInput("Move West");
  cThrust          = resources->getDigitalInput("Thrust");
  cMapBottom = -20.0f; // TODO: Do this for real!
}
