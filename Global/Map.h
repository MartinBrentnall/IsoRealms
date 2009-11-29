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
#ifndef MAP_H
#define MAP_H

#include <vector>

#include "BlockArea.h"
#include "DOMNodeWrapper.h"
#include "DOMNodeWriter.h"
#include "ElementSetRegistry.h"
#include "PluginRegistry.h"
#include "Zone.h"

/**
 * This class holds data for an IsoRealms map.
 */
class Map:public IZoneChangeListener {
  private:
  PluginRegistry cPluginRegistry;
  ElementSetRegistry cElementSetRegistry;

  /**
   * The zones in the map.
   */
  std::vector<Zone*> cZones;

  /**
   * A list of zones to be reinitialized at the next update.  This list is
   * cleared when the zones have been reinitialized.
   */
  std::vector<Zone*> cDirtyZones;

  int getZoneIndex(Zone*);

  public:
  Map();
  Map(DOMNodeWrapper*, IPluginRegistryListener*, IElementRegistryListener*);

  void addZone(Zone*);

  /**
   * Return the zone at the specified block location.
   *
   * @param BlockLocation&  The location.
   * @returns  The zone at the location.
   */
  Zone* getZone(BlockLocation&);

  /**
   * Test whether the specified block area overlaps with a zone in this map.
   *
   * @param BlockArea&  The block area to test.
   * @returns  True if it overlaps, otherwise false.
   */
  bool overlaps(BlockArea&);

  /**
   * Initialize the map.  This function will only initialise dirty zones and
   * element sof the map.  It is called immediately when a map is loaded by the
   * editor or runtime, and when a change takes place on an element that may
   * require some recalculations.
   */
  void initMap();

  /**
   * Update the map.
   */
  void update(int);

  /**
   * Render the map.
   */
  void render();

  /**
   *
   */
  void save();

  ElementSetRegistry* getElementSetRegistry();

  PluginRegistry* getPluginRegistry();

  /**
   * This function is called when an editing action is about to be performed on
   * a zone.
   * 
   * @param Zone*  The zone on which an editing action is about to be
   *           performed.
   */
  void notifyZoneAction(Zone*);

  /**********************************\
   * Implements IZoneChangeListener *
  \**********************************/
  void zoneChanged(Zone*);
};

#endif
