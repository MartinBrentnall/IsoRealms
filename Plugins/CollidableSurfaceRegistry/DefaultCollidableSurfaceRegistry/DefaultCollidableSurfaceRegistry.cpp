#include "DefaultCollidableSurfaceRegistry.h"

DefaultCollidableSurfaceRegistry::DefaultCollidableSurfaceRegistry() {
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cZoneContextSocket.push_back(new PlugSocket("ZoneContext"));
}

void DefaultCollidableSurfaceRegistry::registerRollableSurface(IRollableSurface* rollableSurface) {
  SurfaceCache* mSurfaceCache = cZoneSurfaceCaches[cZone];
  if (mSurfaceCache == NULL) {
    mSurfaceCache = new SurfaceCache();
    cZoneSurfaceCaches[cZone];
  }
  mSurfaceCache->addRollableSurface(rollableSurface);
}

void DefaultCollidableSurfaceRegistry::registerWallSurface(ICollidableWallSurface*) {
  // TODO: Implement this
}

void DefaultCollidableSurfaceRegistry::notifyZoneAction(IZone* zone) {
  cZone = zone;
}

void DefaultCollidableSurfaceRegistry::initPlugin(IZone* zone) {
  cZone = zone;
}

void DefaultCollidableSurfaceRegistry::zoneContextChanged(IZone* zone) {
  cZone = zone;
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
