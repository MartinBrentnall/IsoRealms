#include "DummyCollidableSurfaceRegistry.h"

void DummyCollidableSurfaceRegistry::registerRollableSurface(IRollableSurface*) {
  // Nothing to do.
}

void DummyCollidableSurfaceRegistry::registerWallSurface(ICollidableWallSurface*) {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new DummyCollidableSurfaceRegistry();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
