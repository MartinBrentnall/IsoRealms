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

SpindizzyGERALD::SpindizzyGERALD(IElementFactory* elementFactory, BlockLocation* startLocation, ISimpleModelFactory* geraldModelFactory, ICollectables* collectables, ICollidableSurfaceRegistry* collidableSurfaceRegistry, ILocationAwareness* locationAwareness, IZoneContext* zoneContext, ICamera* camera) : Element<>(elementFactory) {
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
  if (!cEventQueue.empty()) {
    ICollisionData* mEvent = cEventQueue.front();
    cEventQueue.pop();
    std::cout << "Returning queued event!" << std::endl;
    return mEvent;
  }
  
  if (cCurrentSurface != NULL) {
    ICollisionData* mEvent = cCurrentSurface->getRollingEvent(startLocation, endLocation);
    if (mEvent != NULL) {
      cEventQueue.push(mEvent);
    }
  }
  
  std::vector<ICollisionData*> mEvents = cCollidableSurfaceRegistry->getNextEvent(startLocation, endLocation, cCurrentSurface != NULL && cEventQueue.empty());
  for (unsigned int i = 0; i < mEvents.size(); i++) {
    if (cEventQueue.empty() || cEventQueue.front()->getGradient() == mEvents[i]->getGradient()) {
      IRollableSurface* mEventSurface = mEvents[i]->getSurface();
      if (mEventSurface != cCurrentSurface) {
        cEventQueue.push(mEvents[i]);
      }
    }
  }
  if (!cEventQueue.empty()) {
    ICollisionData* mEvent = cEventQueue.front();
    cEventQueue.pop();
    return mEvent;
  }
  return NULL;
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

void SpindizzyGERALD::processEvent(ICollisionData& event, bool simultaneousEvent) {
  Vertex* mEventLocation = event.getEventLocation();
  switch (event.getType()) {
    case ICollisionData::SURFACE_LEAVE: {
      std::cout << " - - - We left: " << event.getSurface() << std::endl;
      Vertex* mEventLocation = event.getEventLocation();
      cMomentum.z = cMomentum.x * -cCurrentSurface->getXAcceleration(mEventLocation->x, mEventLocation->y) +
                    cMomentum.y * -cCurrentSurface->getYAcceleration(mEventLocation->x, mEventLocation->y); 
      cJumpedFromRamp = cMomentum.z > 0.0f;
      cSurfaceLeaveVerticalMomentum = cMomentum.z;
      cCurrentSurface = NULL;
      break;
    }
    
    case ICollisionData::SURFACE_MOUNT: {
      IRollableSurface* mMountedSurface = event.getSurface();
      if (mMountedSurface != cCurrentSurface) {
        if (cMomentum.z == cSurfaceLeaveVerticalMomentum) { // TODO: This condition could pass in rare cases when GERALD bounces
          float mMountedSurfaceIncline = cMomentum.x * -mMountedSurface->getXAcceleration(mEventLocation->x, mEventLocation->y) +
                                         cMomentum.y * -mMountedSurface->getYAcceleration(mEventLocation->x, mEventLocation->y); 
          if (cMomentum.z > mMountedSurfaceIncline) {
            // Ignore this event because GERALD went over a bump, so he'll go into the air
            std::cout << "IGNORED SURFACE MOUNT EVENT!" << std::endl;
            return;
          }
        }
        
        cMomentum.z = -cMomentum.z * mMountedSurface->getSurfaceBounce();
        if (cMomentum.z == 0.0f || cPeakHeight <= mMountedSurface->getHeightAt(mEventLocation->x, mEventLocation->y)) {
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
          float mFallHeight = cPeakHeight - mEventLocation->z;
          std::cout << "Fall height is " << mFallHeight << "...!" << std::endl;
          float mIdealProjection = sqrt(mFallHeight * 2 * -GRAVITY_STRENGTH);
          std::cout << "Ideal projection is " << mIdealProjection << "...!" << std::endl;
          std::cout << "Jumped from ramp? " << cJumpedFromRamp << "...!" << std::endl;
          cMomentum.z = mIdealProjection;
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
}

SpindizzyGERALD::RespawnData* SpindizzyGERALD::getRespawnData() {
  SpindizzyGERALD::RespawnData* mRespawnData = cRespawnSurfaceStack.top();
  while (!mRespawnData->cSurface->isRespawnPossibleNow()) {
    cRespawnSurfaceStack.pop();
    mRespawnData = cRespawnSurfaceStack.top();
  }
  return mRespawnData;
}

void SpindizzyGERALD::checkFall() {
  if (cLocation.z < cMapBottom) {
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
    if (cFallScript != NULL) {
      cFallScript->execute();
    }
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

void SpindizzyGERALD::update(int ticks) {
  float mTicks = static_cast<float>(ticks);
  Vertex mProposedLocation = cLocation;
  Vertex mProposedMomentum = cMomentum;
  getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
  ICollisionData* mNextEvent = pollCollisionEvent(cLocation, mProposedLocation);
  while (mNextEvent != NULL) {
    bool mSimultaneousEvent = !cEventQueue.empty();
    processEvent(*mNextEvent, mSimultaneousEvent);
    if (!mSimultaneousEvent) {
      std::cout << "Processing last event of set!" << std::endl;
      mTicks -= mTicks * mNextEvent->getGradient();
      mProposedLocation = cLocation;
      mProposedMomentum = cMomentum;
      getNewLocation(mTicks, &mProposedLocation, &mProposedMomentum);
    }
    mNextEvent = pollCollisionEvent(cLocation, mProposedLocation);
  }
  mProposedLocation = cLocation;
  mProposedMomentum = cMomentum;
  getNewLocation(mTicks, &mProposedLocation, &cMomentum);
  updateLocation(mProposedLocation);
  checkFall();
}

void SpindizzyGERALD::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      if (cCurrentSurface != NULL && cCurrentSurface->getSurfaceGrip() > 0.5f) {
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
  glPushMatrix();
  cGERALDModel->render();
  glPopMatrix();
}

void SpindizzyGERALD::save(DOMNodeWriter* node, BlockLocation& relative) {
  DOMNodeWriter* mLocationNode = node->addBranch("Location");
  cStartLocation.saveRelative(mLocationNode, relative);  
}

