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
#ifndef DEFAULT_COLLIDABLE_SURFACE_REGISTRY_H
#define DEFAULT_COLLIDABLE_SURFACE_REGISTRY_H

#include <map>
#include <vector>

#include <IsoRealms/IProject.h>
#include <IsoRealms/IZone.h>
#include <IsoRealms/IZoneContextListener.h>
#include <IsoRealms/Resources/SurfaceRegistry/ICollidableSurfaceRegistry.h>
#include <IsoRealms/SingleResourceTypeModule.h>

#include "SurfaceCache.h"

class DefaultCollidableSurfaceRegistry:public ICollidableSurfaceRegistry,
                                       public IZoneContextListener {
  private:
  std::map<IZone*, SurfaceCache*> cZoneSurfaceCaches;
  std::map<IZone*, std::vector<IZone*> > cAdjacentZones;
  IZone* cEditingZone;
  IZone* cRuntimeZone;
  IMap* cMap;

  public:
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  /*****************************************\
   * Implements ICollidableSurfaceRegistry *
  \*****************************************/
  void registerRollableSurface(IRollableSurface*, bool);
  void registerWallSurface(ICollidableWallSurface*);
  ICollisionData* getNextEvent(Vertex&, Vertex&, IRollableSurface*);
  IRollableSurface* getSurfaceAt(Vertex&);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void zoneContextChanged(IMap*, IZone*);
  void initPlugin(IZone*, unsigned int);
  
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif
