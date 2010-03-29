#include "DefaultCollidableSurfaceRegistry.h"

DefaultCollidableSurfaceRegistry::DefaultCollidableSurfaceRegistry() {
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cZoneContextSocket.push_back(new PlugSocket("ZoneContext"));
}

void DefaultCollidableSurfaceRegistry::registerRollableSurface(IRollableSurface* rollableSurface) {
  SurfaceCache* mSurfaceCache = cZoneSurfaceCaches[cEditingZone];
  if (mSurfaceCache == NULL) {
    mSurfaceCache = new SurfaceCache();
    cZoneSurfaceCaches[cEditingZone] = mSurfaceCache;
  }
  mSurfaceCache->addRollableSurface(rollableSurface);
}

void DefaultCollidableSurfaceRegistry::registerWallSurface(ICollidableWallSurface*) {
  // TODO: Implement this
}

ICollisionData* DefaultCollidableSurfaceRegistry::getNextEvent(Vertex& start, Vertex& end) {
  std::map<IZone*, SurfaceCache*>::iterator i = cZoneSurfaceCaches.find(cRuntimeZone);
  if (i != cZoneSurfaceCaches.end()) {
    return i->second->getNextEvent(start, end);
  }
  return NULL;
}

IRollableSurface* DefaultCollidableSurfaceRegistry::getSurfaceAt(Vertex& location) {
  std::map<IZone*, SurfaceCache*>::iterator i = cZoneSurfaceCaches.find(cRuntimeZone);
  std::cout << "Finding surface in zone " << cRuntimeZone << "..." << std::endl; 
  if (i != cZoneSurfaceCaches.end()) {
    std::cout << "Checking surfaces..." << std::endl; 
    return i->second->getSurfaceAt(location);
  }
  return NULL;
}

void DefaultCollidableSurfaceRegistry::notifyZoneAction(IZone* zone) {
  cEditingZone = zone;
}

void DefaultCollidableSurfaceRegistry::initPlugin(IZone* zone) {
  cEditingZone = zone;
}

void DefaultCollidableSurfaceRegistry::zoneContextChanged(IZone* zone) {
  cRuntimeZone = zone;
}

std::string DefaultCollidableSurfaceRegistry::getName() {
  return "Default Collidable Surface Registry";
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
