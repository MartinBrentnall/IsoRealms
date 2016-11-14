/*
 * Copyright 2009,2010 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef RESOURCE_SURFACE_REGISTRY_H
#define RESOURCE_SURFACE_REGISTRY_H

#include <map>
#include <vector>

#include <IsoRealms/IProject.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Struct/SpatialContainer2D.h>

#include "IRollableSurface.h"
#include "ICollidableSurfaceElement.h"
#include "ICollidableWallSurface.h"

class ResourceSurfaceRegistry {
  private:
    
  class ElementSurfaces {
    public:
    std::vector<IRollableSurface*> cFloorSurfaces;
    std::vector<ICollidableWallSurface*> cWallSurfaces;
  };
    
  SDL_mutex* cAccessMutex;
  SpatialContainer2D<IRollableSurface> cRollableSurfaces;
  SpatialContainer2D<IRollableSurface> cInterceptingSurfaces;
  SpatialContainer2D<ICollidableWallSurface> cWallSurfaces;
  std::map<ICollidableSurfaceElement*, ElementSurfaces*> cElementSurfaces;

  public:
  ResourceSurfaceRegistry();

  /*******************************\
   * Implements ISurfaceRegistry *
  \*******************************/
  void registerRollableSurface(ICollidableSurfaceElement*, IRollableSurface*, bool);
  void registerWallSurface(ICollidableSurfaceElement*, ICollidableWallSurface*);
  void unregisterSurfaces(ICollidableSurfaceElement*);
  void unregisterRollableSurface(IRollableSurface*);
  void unregisterWallSurface(ICollidableWallSurface*);
  ICollisionData* getNextEvent(Vertex&, Vertex&, IRollableSurface*, float);
  IRollableSurface* getSurfaceAt(Vertex&, float);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void save(DOMNodeWriter*, IResourceLocator*);
  
  virtual ~ResourceSurfaceRegistry() {}
};

#endif
