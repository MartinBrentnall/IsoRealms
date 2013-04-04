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
#ifndef SPINDIZZY_JEWEL_TYPE_H
#define SPINDIZZY_JEWEL_TYPE_H

#include <cmath>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IScript.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "BaseSpindizzyJewelType.h"
#include "ISpindizzyJewelSet.h"
#include "SpindizzyJewel.h"

class SpindizzyJewelType:public BaseSpindizzyJewelType,
                         public IsoRealmsConstants {
  private:
  I3DModelFactory* cModelType;
  std::vector<SpindizzyJewel*> cContent;
  ICollectables* cCollectables;
  SpindizzyJewel* cSampleJewel;
  std::vector<IDynamicElement*> cSampleJewelDynamics;
  std::vector<IVisualElement*> cSampleJewelVisuals;
  BlockLocation* cEditingLocation;
  IScript* cJewelCollectedScript;
  IScript* cAllJewelsCollectedScript;

  bool keyDown(SDLKey&);

  public:
  SpindizzyJewelType(ISpindizzyJewelSet*);
  
  ICollectables* getCollectables();
  void jewelCollected();
  void allJewelsCollected();
  
  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  IElementHandler* getElementHandler();
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  bool input(SDL_Event&);
  void updateIcon(int milliseconds);
  void renderIcon();
  void setDirty(IElement*);
  void destroy(IElement*);

  ~SpindizzyJewelType();
};

#endif
