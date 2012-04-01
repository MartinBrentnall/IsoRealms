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
#include "Colour.h"
#include "ColourProxy.h"
#include "DefaultZoneRenderer.h"
#include "DOMNodeWrapper.h"
#include "DOMNodeWriter.h"
#include "ElementSetRegistry.h"
#include "IElementHandler.h"
#include "IMap.h"
#include "InputCommands.h"
#include "IPluginRegistryListener.h"
#include "IProject.h"
#include "ITexture.h"
#include "PluginRegistry.h"
#include "Registry.h"
#include "TextureProxy.h"
#include "Zone.h"

/**
 * This class holds data for an IsoRealms map.
 */
class Map:public IMap,
          public IZoneChangeListener,
          public IElementContainer {
  private:
  
  /**
   * The project to which this map belongs
   */
  IProject* cProject;
    
  /**
   * These elements are not confined to one specific zone.
   */
  std::vector<IElement*> cElements;
  std::vector<IElementHandler*> cElementHandlers;
  std::vector<IElement*> cDirtyElements;
  GLuint cDisplayList;
  GLuint cEditingDisplayList;

  /**
   * The zones in the map.
   */
  std::vector<Zone*> cZones;
  
  /**
   * Used for updating and rendering the map.  These are not used for saving.
   */
  std::vector<IZoneRenderer*> cZoneRenderers;

  /**
   * A list of zones to be reinitialized at the next update.  This list is
   * cleared when the zones have been reinitialized.
   */
  std::vector<IZone*> cDirtyZones;

  int getZoneIndex(IZone*);
  int getElementIndex(IElement*);

  bool containsElement(IElement*);

  DOMNodeWrapper* getConfigurationNode(DOMNodeWrapper*);

  public:
  Map();
  Map(DOMNodeWrapper*, bool, IProject*);

  void addZone(Zone*);
  
  void removeZone(Zone*);

  /**
   * Test whether the specified block area overlaps with a zone in this map.
   *
   * @param BlockArea&  The block area to test.
   * @returns  True if it overlaps, otherwise false.
   */
  bool overlaps(BlockArea&);

  void initMap(unsigned int pass, bool);

  /**
   * Initialize the map.  This function will only initialise dirty zones and
   * elements of the map.  It is called immediately when a map is loaded by the
   * editor or runtime, and when a change takes place on an element that may
   * require some recalculations.
   */
  void initMap(bool);

  /**
   * Initialise the map for runtime.  TODO
   */
  void initRuntime();

  /**
   * Update the map.
   */
  void update(int);
  void updateRuntime(int);

  /**
   * Render the map.
   */
  void render();

  void renderEditing();

  /**
   * Pass input to interactive elements and plugins.
   */
  void input(SDL_Event&);

  void executePreLoopCommands(int);

  void executePostLoopCommands(int);
  
  void executePreLoopRenderers();
  
  void executePostLoopRenderers();  

  /**
   *
   */
  void save(DOMNodeWriter*);

  void pushElement(IElement*);

  /**
   * Remove the specified element from anywhere in the map.  This function
   * will be pretty slow, especially on large maps.
   * 
   * @param IElement*  The element to remove.
   * @returns The zone from which the element was removed, or NULL if the
   *          element wasn't found.
   */
  Zone* removeElement(IElement*);

  Zone* getElementContainer(IElement*);
  
  /*******************\
   * Implements IMap *
  \*******************/
  Zone* getZone(BlockLocation&);
  IZone* getZone(Vertex&);
  std::vector<ZoneEvent*> getZoneEvents(Vertex&, Vertex&);
  std::vector<IZone*> getAdjacentZones(IZone*);

  /**********************************\
   * Implements IZoneChangeListener *
  \**********************************/
  void zoneChanged(IZone*);

  /********************************\
   * Implements IElementContainer *
  \********************************/
  void elementDirty(IElement*);
  void addElementHandler(IElementHandler*);
  void setHandlerActive(IElementHandler*, bool);
  
  ~Map();
};

#endif
