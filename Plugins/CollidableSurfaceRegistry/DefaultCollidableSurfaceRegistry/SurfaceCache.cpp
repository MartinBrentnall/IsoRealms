#include "SurfaceCache.h"

void SurfaceCache::addRollableSurface(IRollableSurface* surface, bool intercepting) {
  if (intercepting) {
    std::cout << "Registering intercepting surface...!" << std::endl;
    cInterceptingSurfaces.push_back(surface);
  } else {
    cRollableSurfaces.push_back(surface);
  }
}

ICollisionData* SurfaceCache::getNextEvent(Vertex& start, Vertex& end, IRollableSurface* currentSurface) {
  std::vector<ICollisionData*> mEvents;
  float mEventGradient = -1.0f;
  ICollisionData* mStaticEvent = NULL;
  if (currentSurface == NULL) {
    for (unsigned int i = 0; i < cRollableSurfaces.size(); i++) {
      ICollisionData* mEvent = cRollableSurfaces[i]->getCollision(start, end);
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
  }

  for (unsigned int i = 0; i < cInterceptingSurfaces.size(); i++) {
    ICollisionData* mEvent = cInterceptingSurfaces[i]->getCollision(start, end);
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
}

IRollableSurface* SurfaceCache::getSurfaceAt(Vertex& location) {
  for (unsigned int i = 0; i < cRollableSurfaces.size(); i++) {
    if (cRollableSurfaces[i]->contains(location)) {
      return cRollableSurfaces[i];
    }
  }
  return NULL;
}



