#include "DummyCollidableSurfaceRegistry.h"

void DummyCollidableSurfaceRegistry::registerRollableSurface(IRollableSurface*, bool) {
  // Nothing to do.
}

void DummyCollidableSurfaceRegistry::registerWallSurface(ICollidableWallSurface*) {
  // Nothing to do.
}

std::vector<ICollisionData*> DummyCollidableSurfaceRegistry::getNextEvent(Vertex&, Vertex&, bool) {
  std::vector<ICollisionData*> mNoEvents;
  return mNoEvents;
}

IRollableSurface* DummyCollidableSurfaceRegistry::getSurfaceAt(Vertex&) {
  return NULL;
}

extern "C" IPlugin* create() {
  return new DummyCollidableSurfaceRegistry();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
