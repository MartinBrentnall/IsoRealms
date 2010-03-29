#ifndef I_SPINDIZZY_TILE_SURFACE_H
#define I_SPINDIZZY_TILE_SURFACE_H

#include "../../Plugins/CollidableSurfaceRegistry/IRollableSurface.h"
#include "../../Plugins/SurfaceProcessor/ITileSurface.h"

class ISpindizzyTileSurface:public ITileSurface,
                            public IRollableSurface {
  public:
  virtual void render() = 0;
};

#endif
