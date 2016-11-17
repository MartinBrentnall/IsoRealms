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
#ifndef RESOURCE_ELEMENT_SPINDIZZY_ITEM_H
#define RESOURCE_ELEMENT_SPINDIZZY_ITEM_H

#include <cmath>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/Boundaries/Boundaries.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Script/IScriptCall.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "ISpindizzyJewelType.h"
#include "ISpindizzyJewelSet.h"
#include "ElementHandlerItem.h"
#include "ElementSpindizzyItem.h"

class ResourceElementSpindizzyItem:public ISpindizzyJewelType {
  private:
  ISpindizzyJewelSet* cModuleInterface;
    
  I3DModelType* cModelType;
  std::vector<ElementSpindizzyItem*> cContent;
  IBoundaries* cBoundaries;
  ElementSpindizzyItem* cSampleJewel;
  ElementSpindizzyItem* cEditingJewel;

  bool keyDown(SDLKey&, ILayerEditingContext*);

  ElementSpindizzyItem* getElement(IElement*);
  
  public:
  ResourceElementSpindizzyItem(ISpindizzyJewelSet*, DOMNodeWrapper*, IResourceRegistry*);
  
  IBoundaries* getCollectables();
  I3DModelType* getModelType();
  void setModelType(I3DModelType*);
  
  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  ISpindizzyJewelSet* getSpindizzyItemInterface();
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool, bool);
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview(Vertex&);
  void updateEditingPreview(unsigned int);
  bool inputEdit(SDL_Event&, ILayerEditingContext*);
  void updateIcon(unsigned int milliseconds);
  void renderIcon();
  void setDirty(IElement*);
  void destroy(IElement*);
  void removeElement(IElement*);
  Vertex* editorCursorStopped(Vertex*);

  virtual ~ResourceElementSpindizzyItem();
};

#endif
