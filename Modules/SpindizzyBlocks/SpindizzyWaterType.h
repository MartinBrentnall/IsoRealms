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
#ifndef SPINDIZZY_WATER_TYPE_H
#define SPINDIZZY_WATER_TYPE_H

#include <iostream>

#include <IsoRealms/Resources/ElementType/IElementType.h>

#include "ISpindizzyBlockType.h"
#include "SpindizzyWater.h"

class SpindizzyWaterType:public ISpindizzyBlockType,
                         public IResourceUseListener<ITexture> {
  private:
  ITexture* cTexture;
  std::vector<SpindizzyWater*> cContent;
  BlockLocation* cEditingLocation;
  BlockLocation* cStartWaterLocation;
  SpindizzyWater* cSampleWater;

  bool keyDown(SDLKey& key);

  public:
  SpindizzyWaterType(ISpindizzyBlockSet*);
  
  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  /**********************************\
   * Implements ISpindizzyBlockType *
  \**********************************/
  void unregisterSurfaces(ISurfaceProcessor*);
  BlockTypeProperties* getBlockTypeProperties();
  void save(DOMNodeWriter*, IResourceLocator*);
  void configureBlock(DOMNodeWrapper*);

  /***************************\
   * Implements IElementType *
  \***************************/
  void configureElement();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  IElementHandler* getElementHandler();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  bool input(SDL_Event&);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(int);
  void destroy(IElement*);

  /*******************************\
   * Implements ITextureListener *
  \*******************************/
  void resourcePendingDestruction(ITexture*, ITexture*);
  void resourceChanged(ITexture*);
  
  ~SpindizzyWaterType();
};

#endif
