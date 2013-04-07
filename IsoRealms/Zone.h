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
#ifndef ZONE_H
#define ZONE_H

#include <cmath>
#include <GL/glew.h>
#include <iomanip>
#include <set>

#include "BlockArea.h"
#include "BlockLocation.h"
#include "Collision.h"
#include "CollisionVertex.h"
#include "Persistence/DOMNodeWrapper.h"
#include "Persistence/DOMNodeWriter.h"
#include "IProject.h"
#include "IsoRealmsConstants.h"
#include "IZone.h"
#include "IZoneChangeListener.h"
#include "Resources/ElementType/ElementHandler.h"
#include "Resources/ElementType/IElement.h"
#include "Resources/ElementType/IElementHandler.h"
#include "Resources/IResources.h"

/**
 * When performing collision and contains tests and edge cases show up, zones
 * have the following bias:
 * 
 *  - On south/north faces, south zone is preferred over north.
 *  - On west/east faces, West zone is preferred over east.
 *  - On bottom/top faces, bottom zone preferred over top.
 */
class Zone:public BlockArea,
           public IsoRealmsConstants,
           public IZone {
  private:
  ElementHandler cElementHandler;
  
  std::vector<IZoneChangeListener*> cChangeListeners;


  void renderBounds();

  void zoneChanged();

  bool containsElement(IElement*);

  public:
  Zone(BlockLocation&, BlockLocation&);
  Zone(DOMNodeWrapper*, IProject*, IResourceAccessor*, IMap*);

  /**
   * TODO: Write more about this.
   */
  bool initZone(unsigned int, bool);

  /**
   * Return the location inside this zone closest to the specified location.
   */
  void restrainLocation(BlockLocation*);
 
  void input(SDL_Event&);

  void save(DOMNodeWriter*, IResourceLocator*);

  void pushElement(IElement*);
  void setDirty(IElement*);

  IElement* popElement();

  /**
   * Attempt to remove the specified element from this zone.  If the specified
   * element doesn't exist in this zone, no change occurs.
   * 
   * @param IElement*  The element to remove.
   * @returns true if one or more instances of the specified element were
   *          removed, otherwise false.
   */
  void removeElement(IElement*);

  /**
   * Add a listener to be notified when elements are added or removed from the
   * zone.
   * 
   * @param IZoneChangeListener*  The listener to remove.
   */
  void addChangeListener(IZoneChangeListener*);

  /**
   * Remove a listener to be notified when elements are added or removed from
   * the zone.
   * 
   * @param IZoneChangeListener*  The listener to remove.
   * @throws IllegalArgumentException  If the listener is not registered with
   *         this zone or is NULL.
   */
  void removeChangeListener(IZoneChangeListener*);

  bool contains(BlockLocation&);

  bool contains(IElement*);
  
  void addElement(IElement*);
  
  void staticChanged();
  
  /********************\
   * Implements IZone *
  \********************/
  bool contains(Vertex&);
  std::vector<ZoneEvent*> getZoneEvents(Vertex&, Vertex&);
  BlockArea* getZoneArea();
  void updateEditing(unsigned int);
  void updateRuntime(unsigned int);
  void renderStatic();
  void renderEditing();
  void renderRuntime();
  
  int getZoneEast(); 
  int getZoneWest();
  int getZoneNorth();
  int getZoneSouth();
  int getZoneTop();
  int getZoneBottom();
};

#endif
