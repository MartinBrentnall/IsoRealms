/*
 * Copyright 2015 Martin Brentnall
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
#ifndef RESOURCE_ELEMENT_SPINDIZZY_ZONE_H
#define RESOURCE_ELEMENT_SPINDIZZY_ZONE_H

#include <cmath>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/LuaSupport/ArgumentValueLocal.h>
#include <IsoRealms/Resources/Boundaries/Boundaries.h>
#include <IsoRealms/Resources/Script/IScript.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "ISpindizzyZoneModule.h"
#include "ISpindizzyZoneType.h"
#include "ElementHandlerZone.h"
#include "ElementSpindizzyZone.h"

class ResourceElementSpindizzyZone:public ISpindizzyZoneType,
                                   public IsoRealmsConstants,
                                   public IArgumentValueCollection {
  private:
  ISpindizzyZoneModule* cModuleInterface;
    
  std::vector<ElementSpindizzyZone*> cContent;
  ElementSpindizzyZone* cSampleZone;
  IColour* cVisitedColour;
  IColour* cUnvisitedColour;
  I3DModelType* cFlagModelType;
  IBoundaries* cBoundaries;

  bool keyDown(SDLKey&);

  public:
  ResourceElementSpindizzyZone(ISpindizzyZoneModule*, DOMNodeWrapper*, IResourceRegistry*);

  void applyDefaultTheme();
  
  /*********************************\
   * Implements ISpindizzyZoneType *
  \*********************************/
  ISpindizzyZoneModule* getSpindizzyZoneInterface();
  bool isOverview();
  IColour* getVisitedColour();
  IColour* getUnvisitedColour();
  IBoundaries* getBoundaries();
  void setZoneArgument(ElementSpindizzyZone*);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  void loadElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*);
  void configureElement();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void renderEditingPreview();
  bool inputEdit(SDL_Event&, ILayerEditingContext*);
  void updateIcon(unsigned int milliseconds);
  void renderIcon();
  void setDirty(IElement*);
  void destroy(IElement*);
  Vertex* editorCursorStopped(Vertex*);

  /***************************************\
   * Implements IArgumentValueCollection *
  \***************************************/
  IArgumentValue* getArgumentValue(const std::string&);
  
  virtual ~ResourceElementSpindizzyZone();
};

#endif
