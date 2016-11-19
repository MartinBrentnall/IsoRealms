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
#ifndef LAYER_SPINDIZZY_MAP_H
#define LAYER_SPINDIZZY_MAP_H

#include <vector>

#include <IsoRealms/BlockArea.h>
#include <IsoRealms/Configuration.h>
#include <IsoRealms/Input/InputCommands.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/Layer/ILayer.h>
#include <IsoRealms/Resources/Layer/ILayerType.h>
#include <IsoRealms/Resources/ElementType/ElementHandler.h>
#include <IsoRealms/Resources/IResources.h>

#include "CameraLayerMapEditing.h"
#include "ISpindizzyMapType.h"
#include "ISpindizzyMapModule.h"
#include "LayerSpindizzyMapEditingContext.h"

/**
 * This class holds data for an IsoRealms map.
 */
class LayerSpindizzyMap:public ILayer,
                        public IElementContainer, 
                        public ILayerSpindizzyMap,
                        public IUniverse {
  private:
  ISpindizzyMapType* cMapType;
  ICamera* cCamera;
  LayerSpindizzyMapEditingContext* cEditingContext;
  Vertex* cLocationEditing;

  ElementHandler cElementHandler;

  bool containsElement(IElement*);

  DOMNodeWrapper* getConfigurationNode(DOMNodeWrapper*);

  public:
  LayerSpindizzyMap(ISpindizzyMapType*);
  
  void load(DOMNodeWrapper*, bool, IResourceAccessor*, bool);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*, bool);
  
  /**
   * Test whether the specified block area overlaps with a zone in this map.
   *
   * @param BlockArea&  The block area to test.
   * @returns  True if it overlaps, otherwise false.
   */
//   bool overlaps(BlockArea&);

  void initMap(unsigned int pass, bool);

  /**
   * Initialize the map.  This function will only initialise dirty zones and
   * elements of the map.  It is called immediately when a map is loaded by the
   * editor or runtime, and when a change takes place on an element that may
   * require some recalculations.
   */
  void initMap(bool);
  void initMapThread();

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
  void updateElement(IElement*);
  void addArgumentValue(IArgument*);
  void setArguments();
  void unsetArguments();

  BlockArea* getCoverage();
  void setDirty(IElement*);
  void restrictCursor(Vertex&);
  IUniverse* getUniverse();
  
  /****************************\
   * Implements ISpindizzyMap *
  \****************************/
  float getEast();
  float getWest();
  float getNorth();
  float getSouth();
  float getTop();
  float getBottom();
  float getAspectRatio();

  /*********************\
   * Implements ILayer *
  \*********************/
  void resourceSelected(IElementType*);  
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void renderRuntime();
  void renderEditing();
  bool input(SDL_Event&);
  bool inputEditor(SDL_Event&);
  void initRuntime();
  void initEditor();
  void save(DOMNodeWriter*, IResourceLocator*);
  void staticChanged();
  void reset();
  void addObjectSelectionListener(IObjectSelectionListener* listener);
  
  /*********************************\
   * Implements ILayerSpindizzyMap *
  \*********************************/
  IElementContainer* getElementContainer();
  
  ~LayerSpindizzyMap();
};

#endif
