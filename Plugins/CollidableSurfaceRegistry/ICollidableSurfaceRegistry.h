#ifndef I_COLLIDABLE_SURFACE_REGISTRY_H
#define I_COLLIDABLE_SURFACE_REGISTRY_H

#include "../../Global/IPlugin.h"

#include "IRollableSurface.h"
#include "ICollidableWallSurface.h"

class ICollidableSurfaceRegistry:public virtual IPlugin {
  public:
  virtual void registerRollableSurface(IRollableSurface*) = 0;
  virtual void registerWallSurface(ICollidableWallSurface*) = 0;
};

#endif
