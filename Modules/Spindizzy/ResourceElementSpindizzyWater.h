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
#ifndef RESOURCE_ELEMENT_SPINDIZZY_WATER_H
#define RESOURCE_ELEMENT_SPINDIZZY_WATER_H

#include <iostream>

#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/IResourceUseListener.h>

#include "ISpindizzyBlockType.h"
#include "ElementSpindizzyWater.h"
#include "ElementHandlerSpindizzyBlock.h"

class ResourceElementSpindizzyWater:public ISpindizzyBlockType,
                                    public IResourceUseListener<ITexture> {
  private:
  ISpindizzyBlockSet* cModuleInterface;
    
  ITexture* cTexture;
  std::vector<ElementSpindizzyWater*> cContent;
  BlockLocation* cStartWaterLocation;
  ElementSpindizzyWater* cSampleWater;

  bool keyDown(SDLKey& key);

  ElementSpindizzyWater* getElement(IElement*);
  
  public:
  ResourceElementSpindizzyWater(ISpindizzyBlockSet*, DOMNodeWrapper*, IResourceRegistry*);
  
  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);

  /**********************************\
   * Implements ISpindizzyBlockType *
  \**********************************/
  ISpindizzyBlockSet* getSpindizzyBlockInterface();
  BlockTypeProperties* getBlockTypeProperties();
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  void configureBlock(DOMNodeWrapper*);

  /***************************\
   * Implements IElementType *
  \***************************/
  void configureElement();
  IElement* getElement();
  void loadElement(DOMNodeWrapper*, DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool);
  void setEditingContext(BlockLocation*, IComponentContainer*);
  bool inputEdit(SDL_Event&, ILayerEditingContext*);
  void renderEditingPreview(Vertex&);
  void updateEditingPreview(unsigned int);
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(IElement*);
  void removeElement(IElement*);
  Vertex* editorCursorStopped(Vertex*);

  /*******************************\
   * Implements ITextureListener *
  \*******************************/
  void resourcePendingDestruction(ITexture*, ITexture*);
  void resourceChanged(ITexture*);
  
  ~ResourceElementSpindizzyWater();
};

#endif
