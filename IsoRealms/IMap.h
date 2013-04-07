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
#include "Resources/Vertex/Vertex.h"
#include "IPluginRegistry.h"
#include "IZone.h"
#include "Zone.h"
#include "ZoneEvent.h"

class IMap:public IResource {
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

  virtual std::vector<IZone*> getAdjacentZones(IZone*) = 0;

  virtual void addZone(Zone*) = 0;
  
  virtual void removeZone(Zone*) = 0;

  /**
   * Test whether the specified block area overlaps with a zone in this map.
   *
   * @param BlockArea&  The block area to test.
   * @returns  True if it overlaps, otherwise false.
   */
  virtual bool overlaps(BlockArea&) = 0;

  /*************\
   * Lua API's *
  \*************/
  virtual void setZoneHandler(IZoneHandler*) = 0;
  virtual float getEast() = 0;
  virtual float getWest() = 0;
  virtual float getNorth() = 0;
  virtual float getSouth() = 0;
  virtual float getTop() = 0;
  virtual float getBottom() = 0;
  virtual float getAspectRatio() = 0;
  virtual int getZoneCount() = 0;  
};

#endif
