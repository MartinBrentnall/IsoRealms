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
#include <vector>

#include "../../../Global/Zone.h"

#include "../IRollableSurfaceProvider.h"

class BlockSubtractorCache {
  private:
  std::vector<IRollableSurfaceProvider*>* cCurrentZoneProviders;
  std::map<Zone*, std::vector<IRollableSurfaceProvider*>*> cOrderedSurfaceProvidersByZone;

  int getIndex(IRollableSurfaceProvider*);

  public:
  BlockSubtractorCache();
  void clear();
  void setZone(Zone*);
  void add(IRollableSurfaceProvider*);
  void remove(IRollableSurfaceProvider*);
  void setDirty();

  /**
   * Return all surface provides in this cache.
   * 
   * @returns  All surface providers in this cache.
   */
  std::vector<IRollableSurfaceProvider*> getSurfaceProviders();

  /**
   * Return surface providers with relative priority to the specified provider.
   * 
   * @param bool  True to return providers with higher priority; false to
   *           return surfaces with lower priority.
   * @param IRollableSurfaceProvider*  The relative provider.
   * @returns  The providers with higher / lower priority than the specified
   *           one.
   */
  std::vector<IRollableSurfaceProvider*> getSurfaceProviders(bool, IRollableSurfaceProvider*);

  ~BlockSubtractorCache();
};

#endif
