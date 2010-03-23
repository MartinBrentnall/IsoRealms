#ifndef SURFACE_CACHE_H
#define SURFACE_CACHE_H

#include <vector>

#include "../IRollableSurface.h"
#include "../ICollidableWallSurface.h"

class SurfaceCache {
  private:
  std::vector<IRollableSurface*> cRollableSurfaces;
  std::vector<ICollidableWallSurface*> cWallSurfaces;

  public:
  void addRollableSurface(IRollableSurface*);
};

#endif
