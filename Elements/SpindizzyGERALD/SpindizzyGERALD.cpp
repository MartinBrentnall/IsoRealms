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

const float SpindizzyGERALD::CRAFT_ACCELERATION = 0.0005f;
const float SpindizzyGERALD::GRAVITY_STRENGTH = -0.1f;
const unsigned int SpindizzyGERALD::INIT_REGISTER_BLOCKS = 0;
const unsigned int SpindizzyGERALD::INIT_PROCESS_BLOCKS = 1;
const unsigned int SpindizzyGERALD::INIT_REGISTER_SURFACES = 2;
const unsigned int SpindizzyGERALD::INIT_USE_SURFACES = 3;

SpindizzyGERALD::SpindizzyGERALD(IElementFactory* elementFactory, BlockLocation* startLocation, ISimpleModelFactory* geraldModelFactory, ICollectables* collectables, ICollidableSurfaceRegistry* collidableSurfaceRegistry, ILocationAwareness* locationAwareness, IZoneContext* zoneContext, ICamera* camera) : Element<>(elementFactory) {
  cStartLocation = BlockLocation(*startLocation);
  cLocation.x = cStartLocation.x + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation.y = cStartLocation.y + IsoRealmsConstants::BLOCK_RADIUS;
  cLocation.z = cStartLocation.z;
  cRespawnX = round(cLocation.x);
  cRespawnY = round(cLocation.y);
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
  std::cout << "Init GERALD " << pass << std::endl;
  switch (pass) {
    case INIT_REGISTER_BLOCKS: {
      std::cout << "   Register blocks" << std::endl;
      if (cZoneContext != NULL && cMap != NULL) {
        std::cout << "   Getting GERALD start zone..." << std::endl;
        cZone = cMap->getZone(cLocation);
        std::cout << "   Setting zone in zone context..." << std::endl;
        cZoneContext->setZoneContext(cZone);
      }
      std::cout << "   Done!" << std::endl;
      return false;
    }

    case INIT_USE_SURFACES: {
      std::cout << "   Getting current surface..." << std::endl;
      cCurrentSurface = cCollidableSurfaceRegistry->getSurfaceAt(cLocation);
      std::cout << "   Setting respawn surface..." << std::endl;
      cRespawnSurface = cCurrentSurface;
      std::cout << "   Done!" << std::endl;
      return true;
    }

    default: {
      std::cout << "   Done!" << std::endl;
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

void SpindizzyGERALD::updateZoneContext(Vertex& start, Vertex& end) {
  IZone* mPreviousZone = cZone;
  cCollectables->collect(this, start, end);
  if (cZone != NULL) {
    checkCurrentZoneEvents(start, end);
  }
  if (cZone == NULL) {
    checkMapZoneEvents(mPreviousZone, start, end);
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

void SpindizzyGERALD::update(int ticks) {
  float mAcceleration = (KeyStates::isKeyDown(SDLK_TAB) ? CRAFT_ACCELERATION * 2.0f : CRAFT_ACCELERATION) * ticks;
/*  if (cCurrentSurface != NULL) {*/
    if (KeyStates::isKeyDown(rotateKey(cWestKey))) {
      cMomentum.x -= mAcceleration;
    }
    if (KeyStates::isKeyDown(rotateKey(cEastKey))) {
      cMomentum.x += mAcceleration;
    }
    if (KeyStates::isKeyDown(rotateKey(cSouthKey))) {
      cMomentum.y -= mAcceleration;
    }
    if (KeyStates::isKeyDown(rotateKey(cNorthKey))) {
      cMomentum.y += mAcceleration;
    }
    float mResistance = pow(0.999f, ticks);
    cMomentum.x *= mResistance;
    cMomentum.y *= mResistance;
/*  } else {
    cMomentum.z += GRAVITY_STRENGTH;
  }*/
  Vertex mNewLocation(cLocation.x + cMomentum.x, cLocation.y + cMomentum.y, cLocation.z + cMomentum.z);
  if (mNewLocation.z < cMapBottom) {
    cLocation.x = cRespawnX;
    cLocation.y = cRespawnY;
    cLocation.z = cRespawnSurface->getHeightAt(cLocation.x, cLocation.y);
    cCurrentSurface = cRespawnSurface;
    cZone = cMap->getZone(cLocation);
    cZoneContext->setZoneContext(cZone);
    cMomentum.x = 0.0f;
    cMomentum.y = 0.0f;
    cMomentum.z = 0.0f;
    for (unsigned int i = 0; i < cFallenCommands.size(); i++) {
      cFallenCommands[i]->execute();
    }
  } else {
    if (cCurrentSurface != NULL) {
      ICollisionData* mCollision = cCurrentSurface->getRollingEvent(cLocation, mNewLocation);
      if (mCollision != NULL) {
        std::cout << " - - - We left: " << mCollision->getSurface() << std::endl;
        cCurrentSurface = NULL;
        // TODO: Test from this point on
      }
    }
    if (cCurrentSurface == NULL) {
      ICollisionData* mCollision = cCollidableSurfaceRegistry->getNextEvent(cLocation, mNewLocation);
      if (mCollision != NULL) {
        cCurrentSurface = mCollision->getSurface();
        // TODO: Respawn surface should only be for normal surfaces!
        cRespawnSurface = cCurrentSurface;
        std::cout << " + + + We entd: " << cCurrentSurface << std::endl;
      }
    }
    if (cCurrentSurface != NULL) {
      cRespawnX = round(cLocation.x);
      cRespawnY = round(cLocation.y);
    }

    if (cZoneContext != NULL) {
      updateZoneContext(cLocation, mNewLocation);
    }
    cLocation.x = mNewLocation.x;
    cLocation.y = mNewLocation.y;
    if (cCurrentSurface == NULL) {
      cLocation.z = mNewLocation.z;
    } else {
      cLocation.z = cCurrentSurface->getHeightAt(cLocation.x, cLocation.y);
    }
  }
}

void SpindizzyGERALD::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_SPACE: {
      // TODO: Only works when on surface
      cMomentum.x = 0.0f;
      cMomentum.y = 0.0f;
      cMomentum.z = 0.0f;
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

