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
#ifndef I_ZONE_H
#define I_ZONE_H

#include <vector>

#include "BlockArea.h"
#include "Resources/ElementType/IElementGroup.h"
#include "Resources/Vertex/Vertex.h"
#include "ZoneEvent.h"

class IZone:public IElementContainer {
  public:

  virtual bool contains(Vertex&) = 0;

  /**
   *
   */
  virtual std::vector<ZoneEvent*> getZoneEvents(Vertex&, Vertex&) = 0;

  virtual BlockArea* getZoneArea() = 0;
  
  /**
   * TODO: THESE SHOULD NOT BE PART OF THIS INTERFACE!
   */ 
  virtual bool initZone(unsigned int, bool) = 0;
  virtual void renderStatic() = 0;
  virtual void renderDynamic() = 0;
  virtual void update(unsigned int) = 0;
  virtual void updateRuntime(unsigned int) = 0;

  virtual int getZoneEast() = 0;
  virtual int getZoneWest() = 0;
  virtual int getZoneNorth() = 0;
  virtual int getZoneSouth() = 0;
  virtual int getZoneTop() = 0;
  virtual int getZoneBottom() = 0;  
};

#endif
