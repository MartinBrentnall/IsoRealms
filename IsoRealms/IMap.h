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
#ifndef I_MAP_H
#define I_MAP_H

#include <vector>

#include "BlockLocation.h"
#include "Vertex.h"
#include "IPluginRegistry.h"
#include "IZone.h"
#include "ZoneEvent.h"

class IMap {
  public:

  virtual IZone* getZone(BlockLocation&) = 0;

  /**
   * Return the zone at the specified vertex location.
   * 
   * @param Vertex&  The location.
   * @return  The zone at the location.
   */
  virtual IZone* getZone(Vertex&) = 0;

  /**
   * Return a list of zone events triggered by a point moving from the first
   * specified location to the second.
   *
   * @param Vertex&  The starting location.
   * @param Vertex&  The ending location.
   * @return  A list of ZoneEvents in order of occurrence.
   */
  virtual std::vector<ZoneEvent*> getZoneEvents(Vertex&, Vertex&) = 0;

  virtual void zoneChanged(IZone*) = 0;

  virtual IPluginRegistry* getPluginRegistry() = 0;
};

#endif
