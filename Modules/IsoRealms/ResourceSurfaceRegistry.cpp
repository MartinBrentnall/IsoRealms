/*
 * Copyright 2015 Martin Brentnall
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
#include "ResourceSurfaceRegistry.h"

ResourceSurfaceRegistry::ResourceSurfaceRegistry(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

void ResourceSurfaceRegistry::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cAccessMutex = SDL_CreateMutex();
}

void ResourceSurfaceRegistry::registerRollableSurface(IRollableSurface* rollableSurface, bool intercepting) {
  while (SDL_mutexP(cAccessMutex) == -1);
  if (intercepting) {
    cInterceptingSurfaces.add(rollableSurface);
  } else {
    cRollableSurfaces.add(rollableSurface);
  }
  SDL_mutexV(cAccessMutex);
}

void ResourceSurfaceRegistry::registerWallSurface(ICollidableWallSurface* wallSurface) {
  while (SDL_mutexP(cAccessMutex) == -1);
  cWallSurfaces.add(wallSurface);
  SDL_mutexV(cAccessMutex);
}

ICollisionData* ResourceSurfaceRegistry::getNextEvent(Vertex& start, Vertex& end, IRollableSurface* currentSurface) {
  int mSouth = std::floor(std::min(start.getY(), end.getY())) - 1;
  int mNorth = std::ceil(std::max(start.getY(), end.getY())) + 1;
  int mWest  = std::floor(std::min(start.getX(), end.getX())) - 1;
  int mEast  = std::ceil(std::max(start.getX(), end.getX())) + 1;
  
  while (SDL_mutexP(cAccessMutex) == -1);
  std::vector<IRollableSurface*> mRollableSurfaces = cRollableSurfaces.getElements(mSouth, mNorth, mWest, mEast);
  std::vector<ICollidableWallSurface*> mWallSurfaces = cWallSurfaces.getElements(mSouth, mNorth, mWest, mEast);
  std::vector<IRollableSurface*> mInterceptingSurfaces = cInterceptingSurfaces.getElements(mSouth, mNorth, mWest, mEast);
  SDL_mutexV(cAccessMutex);
  
  std::vector<ICollisionData*> mEvents;
  float mEventGradient = -1.0f;
  ICollisionData* mStaticEvent = NULL;
  if (currentSurface == NULL) {
    for (unsigned int i = 0; i < mRollableSurfaces.size(); i++) {
      ICollisionData* mEvent = mRollableSurfaces[i]->getCollision(start, end);
      if (mEvent != NULL) {
        float mGradient = mEvent->getGradient();
        if (mEventGradient == -1.0f || mGradient < mEventGradient) {
          mEventGradient = mEvent->getGradient();
          mStaticEvent = mEvent;
        }
      }
    }
  }
  
  for (unsigned int i = 0; i < mWallSurfaces.size(); i++) {
    ICollisionData* mEvent = mWallSurfaces[i]->getCollision(start, end);
    if (mEvent != NULL) {
      float mGradient = mEvent->getGradient();
      if (mEventGradient == -1.0f || mGradient < mEventGradient) {
        mEventGradient = mEvent->getGradient();
        mStaticEvent = mEvent;
      }
    }
  }

  if (mStaticEvent != NULL) {
    mEvents.push_back(mStaticEvent);
  }

  for (unsigned int i = 0; i < mInterceptingSurfaces.size(); i++) {
    ICollisionData* mEvent = mInterceptingSurfaces[i]->getCollision(start, end);
    if (mEvent != NULL) {
      float mGradient = mEvent->getGradient();
      if (mEventGradient == -1.0f || mGradient == mEventGradient) {
        Vertex* mLocation = mEvent->getEventLocation();
        bool mAdded = false;
        for (unsigned int j = 0; j < mEvents.size(); j++) {
          Vertex* mExistingEventLocation = mEvents[j]->getEventLocation();
          if (mExistingEventLocation->z > mLocation->z) {
            mEvents.insert(mEvents.begin() + j, mEvent);
            mAdded = true;
            break;
          }
        }
        if (!mAdded) {
          mEvents.push_back(mEvent);
        }
        mEventGradient = mGradient;
      }
    }
  }
  return mEvents.empty() ? NULL : mEvents[0];
//   ICollisionData* mFirstEvent = NULL;
//   if (i != cZoneSurfaceCaches.end()) {
//     ICollisionData* mEvent = i->second->getNextEvent(start, end, currentSurface);
//     if (mEvent != NULL) {
//       mFirstEvent = mEvent;
//     }
//   }
//   std::vector<IZone*> mAdjacentZones = cAdjacentZones[i->first];
//   for (unsigned int i = 0; i < mAdjacentZones.size(); i++) {
//     std::map<IZone*, SurfaceCache*>::iterator j = cZoneSurfaceCaches.find(mAdjacentZones[i]);
//     ICollisionData* mEvent = j->second->getNextEvent(start, end, currentSurface);
//     if (mEvent != NULL) {
//       float mGradient = mEvent->getGradient();
//       if (mFirstEvent == NULL || mGradient < mFirstEvent->getGradient()) {
//         mFirstEvent = mEvent;
//       }
//     }
//   }
//   if (mFirstEvent != NULL) {
//     return mFirstEvent;
//   }
//   if (currentSurface == NULL) {
//     std::vector<ZoneEvent*> mZoneEvents = cMap->getZoneEvents(start, end);
//     for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
//       if (mZoneEvents[i]->getType() == ZoneEvent::ENTERED) {
//         IZone* mEnteredZone = mZoneEvents[i]->getZone();
//         std::map<IZone*, SurfaceCache*>::iterator j = cZoneSurfaceCaches.find(mEnteredZone);
//         if (j != cZoneSurfaceCaches.end()) {
//           ICollisionData* mEvent = j->second->getNextEvent(start, end, NULL);
//           if (mEvent != NULL) {
//             return mEvent;
//           }
//         }
//       }
//     }
//   }
//   return NULL;
}

IRollableSurface* ResourceSurfaceRegistry::getSurfaceAt(Vertex& location) {
  while (SDL_mutexP(cAccessMutex) == -1);
  std::vector<IRollableSurface*> mRollableSurfaces = cRollableSurfaces.getElements(location.x, location.y);
  SDL_mutexV(cAccessMutex);
  for (unsigned int i = 0; i < mRollableSurfaces.size(); i++) {
    if (mRollableSurfaces[i]->contains(location)) {
      return mRollableSurfaces[i];
    }
  }
  return NULL;
}

void ResourceSurfaceRegistry::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  // TODO
}
