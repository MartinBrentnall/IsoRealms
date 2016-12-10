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
#include <set>

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
#include "ISpindizzyZoneThemeListener.h"
#include "ISpindizzyZoneType.h"
#include "ElementHandlerZone.h"
#include "ElementSpindizzyZone.h"

class ResourceElementSpindizzyZone:public ISpindizzyZoneThemeListener,
                                   public ISpindizzyZoneType,
                                   public IsoRealmsConstants,
                                   public IArgumentValueCollection {
  private:
  ISpindizzyZoneModule* cModuleInterface;
    
  std::vector<ElementSpindizzyZone*> cContent;
  ElementSpindizzyZone* cSampleZone;
  ElementSpindizzyZone* cEditingZone;
  std::set<ElementSpindizzyZone*> cFocusedZones;
  IColour* cVisitedColour;
  IColour* cUnvisitedColour;
  I3DModelType* cFlagModelType;
  IBoundaries* cBoundaries;
  Vertex* cStartLocation;

  ElementSpindizzyZone* getElement(IElement*);
  
  bool keyDown(SDLKey&, ILayerEditingContext*);
  
  bool intersectsExistingZone(ElementSpindizzyZone*);
  bool intersectsExistingZone(BlockLocation&);

  public:
  ResourceElementSpindizzyZone(ISpindizzyZoneModule*, DOMNodeWrapper*, IResourceRegistry*);

  /******************************************\
   * Implements ISpindizzyZoneThemeListener *
  \******************************************/
  void spindizzyZoneThemeSelected(ISpindizzyZoneTheme*);
  void spindizzyZoneThemeEdited(ISpindizzyZoneTheme*);
  
  /*********************************\
   * Implements ISpindizzyZoneType *
  \*********************************/
  void zoneGainedFocus(ElementSpindizzyZone*);
  void zoneLostFocus(ElementSpindizzyZone*);
  ISpindizzyZoneModule* getSpindizzyZoneInterface();
  bool isOverview();
  IColour* getVisitedColour();
  IColour* getUnvisitedColour();
  IBoundaries* getBoundaries();
  void setZoneArgument(ElementSpindizzyZone*);
  void applyDefaultTheme();
  void setDefaultTheme(ISpindizzyZoneTheme*);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  void loadElement(DOMNodeWrapper*, DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool);
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

  /***************************************\
   * Implements IArgumentValueCollection *
  \***************************************/
  IArgumentValue* getArgumentValue(const std::string&);
  
  virtual ~ResourceElementSpindizzyZone();
};

#endif
