#ifndef DUMMY_COLLIDABLE_SURFACE_REGISTRY_H
#define DUMMY_COLLIDABLE_SURFACE_REGISTRY_H

#include "../../Global/DummyPlugin.h"

#include "ICollidableSurfaceRegistry.h"

class DummyCollidableSurfaceRegistry:public ICollidableSurfaceRegistry,
                                     public DummyPlugin {
  public:

  /*****************************************\
   * Implements ICollidableSurfaceRegistry *
  \*****************************************/
  void registerRollableSurface(IRollableSurface*);
  void registerWallSurface(ICollidableWallSurface*);
};

#endif
