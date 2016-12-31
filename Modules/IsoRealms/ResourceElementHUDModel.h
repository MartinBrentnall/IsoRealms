/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef RESOURCE_ELEMENT_HUD_MODEL_H
#define RESOURCE_ELEMENT_HUD_MODEL_H

#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceRegistry.h>

#include "ElementHUDModel.h"
#include "IElementRelationManager.h"
#include "ResourceElementHUDAbstract.h"

class ResourceElementHUDModel:public ResourceElementHUDAbstract {
  private:
  HUDComponentPosition* cElementDimensions;
  Vertex cModelLocation;
  float cModelScale;
  ICamera* cCamera;
  I3DModel* cModel;

  public:
  ResourceElementHUDModel(IElementRelationManager*, DOMNodeWrapper*, IResourceRegistry*);
    
  /*****************************************\
   * Implements ResourceElementHUDAbstract *
  \*****************************************/
  IElement* createHUDElement(DOMNodeWrapper*, DOMNodeWrapper*, BlockLocation*, IResourceAccessor*, bool, HUDComponentPosition*);
  
  /*******************\
   * Implements TODO *
  \*******************/
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  void configureElement();
  void renderEditingPreview(Vertex&);
  void updateEditingPreview(unsigned int);
  void renderIcon();
  void updateIcon(unsigned int);
  void destroy(IElement*);
  void removeElement(IElement*);
  IEditorCursorAligner* getCursorAligner();
  bool inputEdit(SDL_Event&, ILayerEditingContext*);

  /*****************************\
   * Implements IElementBounds *
  \*****************************/
  float getWest();
  float getEast();
  float getSouth();
  float getNorth();
  float getTop();
  float getBottom();

  virtual ~ResourceElementHUDModel() {}
};

#endif
