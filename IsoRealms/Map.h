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
#include "Persistence/DOMNodeWrapper.h"
#include "Persistence/DOMNodeWriter.h"
#include "IMap.h"
#include "Input/InputCommands.h"
#include "IProject.h"
#include "Resources/ElementType/IElementHandler.h"
#include "Resources/IResources.h"
#include "PluginRegistry.h"
#include "Zone.h"

/**
 * This class holds data for an IsoRealms map.
 */
class Map:public IMap,
          public IZoneChangeListener,
	  public IElementContainer,
	  public IElement {
  private:
  bool cEditing;
  ICamera* cCamera;

  /**
   * The project to which this map belongs
   */
  IProject* cProject;
    
  /**
   * These elements are not confined to one specific zone.
   */
  ElementHandler cElementHandler;

  /**
   * The zones in the map.
   */
  std::vector<Zone*> cZones;
  
  /**
   * Used for updating and rendering the map.  These are not used for saving.
   */
  std::vector<IZoneHandler*> cZoneHandlers;
  std::vector<IZoneHandler*> cZoneHandlersPersisted;

  /**
   * A list of zones to be reinitialized at the next update.  This list is
   * cleared when the zones have been reinitialized.
   */
  std::vector<IZone*> cDirtyZones;

  int getZoneIndex(IZone*);

  bool containsElement(IElement*);

  DOMNodeWrapper* getConfigurationNode(DOMNodeWrapper*);

  public:
  Map(IProject*);
  Map(bool, IProject*, IResourceAccessor*);
  Map(DOMNodeWrapper*, bool, IProject*, IResourceAccessor*);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
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

  void pushElement(IElement*);

  void addElement(IElement*);
  
  /**
   * TODO: This is probably wrong now.
   * Remove the specified element from anywhere in the map.  This function
   * will be pretty slow, especially on large maps.
   * 
   * @param IElement*  The element to remove.
   * @returns The zone from which the element was removed, or NULL if the
   *          element wasn't found.
   */  
  void removeElement(IElement*);

  Zone* getElementContainer(IElement*);

  void setZoneHandler(IZoneHandler*);
  
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

  float getEast();
  float getWest();
  float getNorth();
  float getSouth();
  float getTop();
  float getBottom();
  float getAspectRatio();
  int getZoneCount();

  /***********************\
   * Implements IElement *
  \***********************/
  IPlugin* getElementSet();
  IElementType* getElementType();
  bool initElement(unsigned int);
  void renderStatic();
  void renderRuntime();
  void renderEditing();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void input(SDL_Event&);
  bool isVisualRuntime();
  bool isVisualEditing();
  bool isDynamicRuntime();
  bool isDynamicEditing();
  bool isInteractive();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
  void initRuntime();
  void staticChanged();
  
  ~Map();
};

#endif
