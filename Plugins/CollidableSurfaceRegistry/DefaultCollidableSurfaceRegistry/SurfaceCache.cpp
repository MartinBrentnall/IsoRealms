#include "SurfaceCache.h"

void SurfaceCache::addRollableSurface(IRollableSurface* surface) {
  cRollableSurfaces.push_back(surface);
}

