/*
 * Copyright 2009 Martin Brentnall
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
#ifndef BLOCK_SUBTRACTOR_CACHE_H
#define BLOCK_SUBTRACTOR_CACHE_H

#include <map>
#include <stdlib.h>
#include <vector>

#include <IsoRealms/IProject.h>
#include <IsoRealms/IZone.h>

#include "../ISurfaceProvider.h"

#include "SurfaceDataCache.h"

class BlockSubtractorCache {
  private:
  int cZoneXOffset;
  int cZoneYOffset;
  std::vector<ISurfaceProvider*>* cCurrentZoneProviders;
  SurfaceDataCache* cCurrentSurfaceDataCache;
  IZone* cCurrentZone;
  IProject* cCurrentProject;
  std::map<IZone*, std::vector<ISurfaceProvider*>*> cOrderedSurfaceProvidersByZone;
  std::map<IZone*, SurfaceDataCache*> cSurfaceDataCaches;

  int getIndex(std::vector<ISurfaceProvider*>*, ISurfaceProvider*);

  public:
  BlockSubtractorCache();
  void setZone(IZone*);
  void setProject(IProject*);
  void add(ISurfaceProvider*);
  void remove(ISurfaceProvider*);
  void setDirty();
  void reinitialise();

  std::vector<TileColumn*>* getTileColumn(int, int);
  void putTileColumn(std::vector<TileColumn*>*, int, int);
  
  /**
   * Return all surface provides in this cache.
   * 
   * @returns  All surface providers in this cache.
   */
  std::vector<ISurfaceProvider*> getSurfaceProviders();

  /**
   * Return surface providers with relative priority to the specified provider.
   * 
   * @param bool  True to return providers with higher priority; false to
   *           return surfaces with lower priority.
   * @param ISurfaceProvider*  The relative provider.
   * @returns  The providers with higher / lower priority than the specified
   *           one.
   */
  std::vector<ISurfaceProvider*> getSurfaceProviders(bool, ISurfaceProvider*);

  void clear();
  
  ~BlockSubtractorCache();
};

#endif
