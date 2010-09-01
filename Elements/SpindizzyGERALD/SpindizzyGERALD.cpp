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

SpindizzyGERALD::SpindizzyGERALD(ISpindizzyGERALDFactory* elementFactory, BlockLocation* startLocation, ISimpleModelFactory* geraldModelFactory, ICollectables* collectables, ICollidableSurfaceRegistry* collidableSurfaceRegistry, ILocationAwareness* locationAwareness, IZoneContext* zoneContext, ICamera* camera, float fallLimit, Script* fallLimitScript) : Element<ISpindizzyGERALDSet, ISpindizzyGERALDFactory>(elementFactory) {
  cStartLocation = BlockLocation(*startLocation);
  cLocation.x = cStartLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation.y = cStartLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation.z = cStartLocation.z;
  if (locationAwareness != NULL) {
    locationAwareness->setLocation(&cLocation);
  }
  cGERALDModel = geraldModelFactory->createModel(&cLocation);
  cCamera = camera;
  cCollectables = collectables;
  cCollidableSurfaceRegistry = collidableSurfaceRegistry;
  cZoneContext = zoneContext;
  cZone = NULL;
  cMap = NULL;
  cNorthKey = SDLK_UP;
  cSouthKey = SDLK_DOWN;
  cEastKey = SDLK_RIGHT;
  cWestKey = SDLK_LEFT;
  cFallScript = NULL;
  cFallLimitScript = fallLimitScript;
  cFallLimit = fallLimit;
}

void SpindizzyGERALD::setModel(ISimpleModelFactory* geraldModelFactory) {
  cGERALDModel = geraldModelFactory->createModel(&cLocation);
}

void SpindizzyGERALD::setCamera(ICamera* camera) {
  cCamera = camera;
}

void SpindizzyGERALD::setCollectables(ICollectables* collectables) {
  cCollectables = collectables;
}

void SpindizzyGERALD::collected(ICollectable* collectable) {
  // Nothing to do.  TODO: Something to do?
}

void SpindizzyGERALD::setZoneContext(IZoneContext* zoneContext) {
  cZoneContext = zoneContext;
}

void SpindizzyGERALD::renderStatic() {
  // Nothing to do.
}

bool SpindizzyGERALD::initElement(unsigned int pass) {
  switch (pass) {
    case INIT_REGISTER_BLOCKS: {
      if (cZoneContext != NULL && cMap != NULL) {
        cZone = cMap->getZone(cLocation);
        cZoneContext->setZoneContext(cZone);
      }
      return false;
    }

    case INIT_USE_SURFACES: {
      cCurrentSurface = cCollidableSurfaceRegistry->getSurfaceAt(cLocation);
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

void SpindizzyGERALD::setRuntimeContext(IMap* map) {
  cMap = map;
  cMapBottom = -20.0f; // TODO: Do this for real!
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

std::vector<IInteractiveElement*> SpindizzyGERALD::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  mInteractiveElements.push_back(this);
  return mInteractiveElements;
}

void SpindizzyGERALD::checkCurrentZoneEvents(Vertex& start, Vertex& end) {
  std::vector<ZoneEvent*> mZoneEvents = cZone->getZoneEvents(start, end);
  for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
    switch (mZoneEvents[i]->getType()) {
      case ZoneEvent::EXITED: {
        cZone = NULL;
        cZoneContext->setZoneContext(cZone);
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

void SpindizzyGERALD::checkMapZoneEvents(IZone* previousZone, Vertex& start, Vertex& end) {
  std::vector<ZoneEvent*> mZoneEvents = cMap->getZoneEvents(start, end);
  for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
    if (mZoneEvents[i]->getZone() != previousZone) {
      switch (mZoneEvents[i]->getType()) {
        case ZoneEvent::ENTERED: {
          if (cZone != NULL) {
            // TODO: Throw wobbly
            std::cout << "Multiple zone entered event should occur in a row" << std::endl;
            exit(1);
          }
          cZone = mZoneEvents[i]->getZone();
          cZoneContext->setZoneContext(cZone);
          break;
        }

        case ZoneEvent::EXITED: {
          if (cZone == NULL) {
            // TODO: Throw wobbly
            std::cout << "On event " << i << ": Zone exited event should never happen when not inside a zone" << std::endl;
            exit(1);
          }
          cZone = NULL;
          cZoneContext->setZoneContext(cZone);
          break;
        }
      }
    }
  }
}

SDLKey SpindizzyGERALD::rotateKey(const SDLKey& key) {
  if (cCamera != NULL) {
    float mCameraAngle = cCamera->getAngle();
    if (mCameraAngle >= 40.0f && mCameraAngle <= 130.0f) {
      if (key == cNorthKey) {return cWestKey;}
      if (key == cWestKey)  {return cSouthKey;}
      if (key == cSouthKey) {return cEastKey;}
      if (key == cEastKey)  {return cNorthKey;}
    } else if (mCameraAngle >= -50.0f && mCameraAngle <= 40.0f) {
      //  Normal orientation
    } else if (mCameraAngle >= -140.0f && mCameraAngle <= -50.0f) {
      if (key == cNorthKey) {return cEastKey;}
      if (key == cWestKey)  {return cNorthKey;}
      if (key == cSouthKey) {return cWestKey;}
      if (key == cEastKey)  {return cSouthKey;}
    } else {
      if (key == cNorthKey) {return cSouthKey;}
      if (key == cWestKey)  {return cEastKey;}
      if (key == cSouthKey) {return cNorthKey;}
      if (key == cEastKey)  {return cWestKey;}
    } 
  }
  return key;  
}

void SpindizzyGERALD::getNewLocation(float ticks, Vertex* location, Vertex* momentum) {
  bool mFast = KeyStates::isKeyDown(SDLK_TAB);
  bool mGoingWest = KeyStates::isKeyDown(rotateKey(cWestKey));
  bool mGoingEast = KeyStates::isKeyDown(rotateKey(cEastKey));
  bool mGoingSouth = KeyStates::isKeyDown(rotateKey(cSouthKey));
  bool mGoingNorth = KeyStates::isKeyDown(rotateKey(cNorthKey));
  if (cCurrentSurface != NULL) {
    float mXSlopeMomentum = cCurrentSurface->getXAcceleration(cLocation.x, cLocation.y) * CRAFT_ACCELERATION;
    float mYSlopeMomentum = cCurrentSurface->getYAcceleration(cLocation.x, cLocation.y) * CRAFT_ACCELERATION;
    float mSurfaceFriction = 1.0f - cCurrentSurface->getSurfaceFriction();
    float mSurfaceGrip = cCurrentSurface->getSurfaceGrip();
    for (unsigned int i = 0; i < ticks; i++) {
      float mAcceleration = (mFast ? CRAFT_ACCELERATION * 2.0f : CRAFT_ACCELERATION);
      if (mGoingWest && !mGoingEast) {
        momentum->x -= mSurfaceGrip * ((!mFast && mAcceleration > 0.0f && mAcceleration < mXSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, -mXSlopeMomentum)
                     : mAcceleration);
      }
      if (mGoingEast && !mGoingWest) {
        momentum->x += mSurfaceGrip * ((!mFast && mAcceleration > 0.0f && mAcceleration < -mXSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, mXSlopeMomentum)
                     : mAcceleration);
      }
      if (mGoingSouth && !mGoingNorth) {
        momentum->y -= mSurfaceGrip * ((!mFast && mAcceleration > 0.0f && mAcceleration < mYSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, -mYSlopeMomentum)
                     : mAcceleration);
      }
      if (mGoingNorth && !mGoingSouth) {
        momentum->y += mSurfaceGrip * ((!mFast && mAcceleration > 0.0f && mAcceleration < -mYSlopeMomentum)
                     ? std::max(mAcceleration * 2.0f, mYSlopeMomentum)
                     : mAcceleration);
      }
      momentum->x += mXSlopeMomentum;
      momentum->y += mYSlopeMomentum;
      momentum->x *= mSurfaceFriction;
      momentum->y *= mSurfaceFriction;
      location->x += momentum->x;
      location->y += momentum->y;
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

ICollisionData* SpindizzyGERALD::pollCollisionEvent(Vertex& startLocation, Vertex& endLocation) {
  ICollisionData* mEvent = NULL;
  if (cCurrentSurface != NULL) {
    ICollisionData* mSurfaceLeftEvent = cCurrentSurface->getRollingEvent(startLocation, endLocation);
    if (mSurfaceLeftEvent != NULL) {
      mEvent = mSurfaceLeftEvent;
    }
  }
  
  ICollisionData* mOtherEvent = cCollidableSurfaceRegistry->getNextEvent(startLocation, endLocation, cCurrentSurface);
  if (mOtherEvent != NULL) {
    IRollableSurface* mEventSurface = mOtherEvent->getSurface();
    if (mEventSurface != cCurrentSurface && (mEvent == NULL || mOtherEvent->getGradient() < mEvent->getGradient())) {
      return mOtherEvent;
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

int mCycle = 0;

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
          if (cPeakHeight - mEventLocation->z > cFallLimit) {
            std::cout << "Peak height " << cPeakHeight << std::endl;
            if (cFallLimitScript != NULL) {
              cFallLimitScript->execute();
            }
            destroy();
            return false;
          } else {
            cCurrentSurface = mMountedSurface;
            cCurrentSurface->notifyContact();
            updateRespawnData();
            std::cout << " + + + We entd: " << cCurrentSurface << std::endl;
            cPeakHeight = cMapBottom;
          }
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
      break;
    }
    
    case ICollisionData::WALL_CLIP: {
      // TODO: Implement this
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
  cDestroyLocation = cLocation;
  cRespawnAnimation = 0.0f;
  cRespawning = true;
  cZone = NULL;
  cZoneContext->setZoneContext(cZone);
}

void SpindizzyGERALD::respawn() {
  SpindizzyGERALD::RespawnData* mRespawnData = getRespawnData();
  cLocation.x = mRespawnData->cX;
  cLocation.y = mRespawnData->cY;
  cCurrentSurface = mRespawnData->cSurface;
  cLocation.z = cCurrentSurface->getHeightAt(cLocation.x, cLocation.y);
  cZone = cMap->getZone(cLocation);
  cZoneContext->setZoneContext(cZone);
  cMomentum.x = 0.0f;
  cMomentum.y = 0.0f;
  cMomentum.z = 0.0f;
  cRespawning = false;
  cPeakHeight = cMapBottom;
}

void SpindizzyGERALD::checkFall() {
  if (cLocation.z < cMapBottom) {
    if (cFallScript != NULL) {
      cFallScript->execute();
    }
    destroy();
  }
}

void SpindizzyGERALD::updateRespawnLocation() {
  if (cCurrentSurface == cRespawnData->cSurface) {
    cRespawnData->cX = round(cLocation.x);
    cRespawnData->cY = round(cLocation.y);
  }
}

void SpindizzyGERALD::updateLocation(Vertex& location) {
  IZone* mPreviousZone = cZone;
  cCollectables->collect(this, cLocation, location);
  if (cZone != NULL) {
    checkCurrentZoneEvents(cLocation, location);
  }
  if (cZone == NULL) {
    checkMapZoneEvents(mPreviousZone, cLocation, location);
  }
  cLocation = location;
  if (cCurrentSurface == NULL && cLocation.z > cPeakHeight) {
    cPeakHeight = cLocation.z;
  }    
  updateRespawnLocation();
}

void SpindizzyGERALD::updateAlive(int ticks) {
  cFastEvents = 0;
  cCycleBounce = false;
  float mTicks = static_cast<float>(ticks);
  Vertex mProposedLocation = cLocation;
  Vertex mProposedMomentum = cMomentum;
  getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
  ICollisionData* mNextEvent = pollCollisionEvent(cLocation, mProposedLocation);
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
        mProposedLocation = cLocation;
        mProposedMomentum = cMomentum;
        getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
      }

      mNextEvent = pollCollisionEvent(cLocation, mProposedLocation);
      if (cFastEvents > 8 && mNextEvent != NULL && mNextEvent->getType() == ICollisionData::SURFACE_LEAVE) {
        mProposedLocation = cLocation;
        mProposedMomentum = cMomentum;
        getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
        cCurrentSurface = mNextEvent->getSurface();
        if (cFastEvents < 16) {
          cCurrentSurface->getRestingLocation(mProposedLocation);
        } else {
          mProposedLocation = cLocation;
        }
        mProposedLocationSet = true;
        mNextEvent = pollCollisionEvent(cLocation, mProposedLocation);
      }
    }
  }
  if (!mProposedLocationSet) {
    mProposedLocation = cLocation;
    getNewLocation(mTicks, &mProposedLocation, &cMomentum);
  } else {
    cMomentum = mProposedMomentum;
  }
  updateLocation(mProposedLocation);
  checkFall();
}

void SpindizzyGERALD::updateDead(int ticks) {
  cRespawnAnimation += ticks / 500.0f;
  if (cRespawnAnimation >= 1.0f) {
    respawn();
  } else {
    SpindizzyGERALD::RespawnData* mRespawnData = getRespawnData();
    cLocation.x = sine(cDestroyLocation.x, mRespawnData->cX, cRespawnAnimation);
    cLocation.y = sine(cDestroyLocation.y, mRespawnData->cY, cRespawnAnimation);
    cLocation.z = sine(cDestroyLocation.z, mRespawnData->cSurface->getHeightAt(mRespawnData->cX, mRespawnData->cY), cRespawnAnimation);
  }
}

void SpindizzyGERALD::update(int ticks) {
  if (cRespawning) {
    updateDead(ticks);
  } else {
    ISpindizzyGERALDSet* mGERALDSet = getElementSet();
    if (!mGERALDSet->isLocked()) {
      updateAlive(ticks);
    }
  }
}

void SpindizzyGERALD::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cCurrentSurface != NULL && cCurrentSurface->getSurfaceFriction() > 0.0f) {
        cMomentum.x = 0.0f;
        cMomentum.y = 0.0f;
        cMomentum.z = 0.0f;
      }
      break;
    }

    default: {
      // Nothing to do
    }
  }
}

bool SpindizzyGERALD::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      keyDown(event.key.keysym.sym);
      break;
    }
  }
  return false;
}

void SpindizzyGERALD::render() {
  if (!cRespawning) {
    glPushMatrix();
    cGERALDModel->render();
    glPopMatrix();
  }
}

void SpindizzyGERALD::save(DOMNodeWriter* node, BlockLocation& relative) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  cStartLocation.saveRelative(mLocationNode, relative);  
}

