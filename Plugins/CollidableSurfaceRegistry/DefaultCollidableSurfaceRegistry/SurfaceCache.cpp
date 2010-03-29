#include "SurfaceCache.h"

void SurfaceCache::addRollableSurface(IRollableSurface* surface) {
  cRollableSurfaces.push_back(surface);
}

ICollisionData* SurfaceCache::getNextEvent(Vertex& start, Vertex& end) {
  return NULL;
}

IRollableSurface* SurfaceCache::getSurfaceAt(Vertex& location) {
  for (unsigned int i = 0; i < cRollableSurfaces.size(); i++) {
    if (cRollableSurfaces[i]->contains(location)) {
      return cRollableSurfaces[i];
    }
  }
  return NULL;
}



