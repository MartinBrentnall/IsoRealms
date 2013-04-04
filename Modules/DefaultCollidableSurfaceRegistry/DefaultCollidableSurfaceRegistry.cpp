#include "DefaultCollidableSurfaceRegistry.h"

void DefaultCollidableSurfaceRegistry::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cProject = resources->getProject();
}

void DefaultCollidableSurfaceRegistry::registerRollableSurface(IRollableSurface* rollableSurface, bool intercepting) {
  SurfaceCache* mSurfaceCache = cZoneSurfaceCaches[cEditingZone];
  if (mSurfaceCache == NULL) {
    mSurfaceCache = new SurfaceCache();
    cZoneSurfaceCaches[cEditingZone] = mSurfaceCache;
  }
  mSurfaceCache->addRollableSurface(rollableSurface, intercepting);
}

void DefaultCollidableSurfaceRegistry::registerWallSurface(ICollidableWallSurface* wallSurface) {
  SurfaceCache* mSurfaceCache = cZoneSurfaceCaches[cEditingZone];
  if (mSurfaceCache == NULL) {
    mSurfaceCache = new SurfaceCache();
    cZoneSurfaceCaches[cEditingZone] = mSurfaceCache;
  }
  mSurfaceCache->addWallSurface(wallSurface);
}

ICollisionData* DefaultCollidableSurfaceRegistry::getNextEvent(Vertex& start, Vertex& end, IRollableSurface* currentSurface) {
  std::map<IZone*, SurfaceCache*>::iterator i = cZoneSurfaceCaches.find(cRuntimeZone);
  ICollisionData* mFirstEvent = NULL;
  if (i != cZoneSurfaceCaches.end()) {
    ICollisionData* mEvent = i->second->getNextEvent(start, end, currentSurface);
    if (mEvent != NULL) {
      mFirstEvent = mEvent;
    }
  }
  std::vector<IZone*> mAdjacentZones = cAdjacentZones[i->first];
  for (unsigned int i = 0; i < mAdjacentZones.size(); i++) {
    std::map<IZone*, SurfaceCache*>::iterator j = cZoneSurfaceCaches.find(mAdjacentZones[i]);
    ICollisionData* mEvent = j->second->getNextEvent(start, end, currentSurface);
    if (mEvent != NULL) {
      float mGradient = mEvent->getGradient();
      if (mFirstEvent == NULL || mGradient < mFirstEvent->getGradient()) {
        mFirstEvent = mEvent;
      }
    }
  }
  if (mFirstEvent != NULL) {
    return mFirstEvent;
  }
  if (currentSurface == NULL) {
    std::vector<ZoneEvent*> mZoneEvents = cProject->getZoneEvents(start, end);
    for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
      if (mZoneEvents[i]->getType() == ZoneEvent::ENTERED) {
        IZone* mEnteredZone = mZoneEvents[i]->getZone();
        std::map<IZone*, SurfaceCache*>::iterator j = cZoneSurfaceCaches.find(mEnteredZone);
        if (j != cZoneSurfaceCaches.end()) {
          ICollisionData* mEvent = j->second->getNextEvent(start, end, NULL);
          if (mEvent != NULL) {
            return mEvent;
          }
        }
      }
    }
  }
  return NULL;
}

IRollableSurface* DefaultCollidableSurfaceRegistry::getSurfaceAt(Vertex& location) {
  std::map<IZone*, SurfaceCache*>::iterator i = cZoneSurfaceCaches.find(cRuntimeZone);
  if (i != cZoneSurfaceCaches.end()) {
    return i->second->getSurfaceAt(location);
  }
  return NULL;
}

void DefaultCollidableSurfaceRegistry::initPlugin(IZone* zone, unsigned int pass) {
  cEditingZone = zone;
  cAdjacentZones[cEditingZone] = cProject->getAdjacentZones(zone);
}

void DefaultCollidableSurfaceRegistry::zoneContextChanged(IMap* map, IZone* zone) {
  cEditingZone = zone;
// TODO: This should change at runtime  cRuntimeZone = zone;
}

void DefaultCollidableSurfaceRegistry::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  // TODO
}

extern "C" IPlugin* create() {
  return new SingleResourceTypeModule<DefaultCollidableSurfaceRegistry>("SurfaceRegistry");
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
