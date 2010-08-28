#include "DefaultCollidableSurfaceRegistry.h"

DefaultCollidableSurfaceRegistry::DefaultCollidableSurfaceRegistry() {
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cZoneContextSocket.push_back(new PlugSocket("ZoneContext"));
}

void DefaultCollidableSurfaceRegistry::registerRollableSurface(IRollableSurface* rollableSurface, bool intercepting) {
  SurfaceCache* mSurfaceCache = cZoneSurfaceCaches[cEditingZone];
  if (mSurfaceCache == NULL) {
    mSurfaceCache = new SurfaceCache();
    cZoneSurfaceCaches[cEditingZone] = mSurfaceCache;
  }
  mSurfaceCache->addRollableSurface(rollableSurface, intercepting);
}

void DefaultCollidableSurfaceRegistry::registerWallSurface(ICollidableWallSurface*) {
  // TODO: Implement this
}

ICollisionData* DefaultCollidableSurfaceRegistry::getNextEvent(Vertex& start, Vertex& end, IRollableSurface* currentSurface) {
  std::map<IZone*, SurfaceCache*>::iterator i = cZoneSurfaceCaches.find(cRuntimeZone);
  if (i != cZoneSurfaceCaches.end()) {
    ICollisionData* mEvent = i->second->getNextEvent(start, end, currentSurface);
    if (mEvent != NULL) {
      return mEvent;
    }
  }
  if (currentSurface == NULL) {
    std::vector<ZoneEvent*> mZoneEvents = cMap->getZoneEvents(start, end);
    for (unsigned int i = 0; i < mZoneEvents.size(); i++) {
      if (mZoneEvents[i]->getType() == ZoneEvent::ENTERED) {
        IZone* mEnteredZone = mZoneEvents[i]->getZone();
        std::map<IZone*, SurfaceCache*>::iterator j = cZoneSurfaceCaches.find(mEnteredZone);
        if (j != cZoneSurfaceCaches.end()) {
          ICollisionData* mEvent = j->second->getNextEvent(start, end, false);
          if (mEvent != NULL) {
            return mEvent;
          }
        }
      }
    }
  }
  return NULL;
}

void DefaultCollidableSurfaceRegistry::setRuntimeContext(IMap* map) {
  cMap = map;
}

IRollableSurface* DefaultCollidableSurfaceRegistry::getSurfaceAt(Vertex& location) {
  std::map<IZone*, SurfaceCache*>::iterator i = cZoneSurfaceCaches.find(cRuntimeZone);
  if (i != cZoneSurfaceCaches.end()) {
    return i->second->getSurfaceAt(location);
  }
  return NULL;
}

void DefaultCollidableSurfaceRegistry::notifyZoneAction(IZone* zone) {
  cEditingZone = zone;
}

void DefaultCollidableSurfaceRegistry::initPlugin(IZone* zone, unsigned int pass) {
  cEditingZone = zone;
}

void DefaultCollidableSurfaceRegistry::zoneContextChanged(IZone* zone) {
  cRuntimeZone = zone;
}

std::vector<PlugSocket*> DefaultCollidableSurfaceRegistry::getPlugSockets() {
  return cZoneContextSocket;
}

void DefaultCollidableSurfaceRegistry::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(plugin, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
      cEditingZone = cZoneContext->getZoneContext();
      cRuntimeZone = cEditingZone;
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* DefaultCollidableSurfaceRegistry::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "ZoneContext") {return cZoneContext;}
  // TODO: Throw
  return NULL;
}

extern "C" IPlugin* create() {
  return new DefaultCollidableSurfaceRegistry();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
