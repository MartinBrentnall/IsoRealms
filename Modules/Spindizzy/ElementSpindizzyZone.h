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
#ifndef ELEMENT_SPINDIZZY_ZONE_H
#define ELEMENT_SPINDIZZY_ZONE_H

#include <cmath>
#include <cstdlib>
#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Collision.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/Boundaries/IBoundaries.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/ElementType/ElementHandler.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/Script/IScriptCall.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "ISpindizzyZoneType.h"
#include "ISpindizzyZoneModule.h"

#include "SpindizzyZoneTheme.h"

/**
 * When performing collision and contains tests and edge cases show up, zones
 * have the following bias:
 * 
 *  - On south/north faces, south zone is preferred over north.
 *  - On west/east faces, West zone is preferred over east.
 *  - On bottom/top faces, bottom zone preferred over top.
 */
class ElementSpindizzyZone:public Element,
                           public IElementContainer,
                           public IElementBounds,
                           public IBoundary {
  private:

  // Interfaces
  ISpindizzyZoneType* cZoneType;
  IElementContainer* cContainer;
  ElementHandler cElementHandler;
    
  // Definition data
  BlockArea* cZoneArea;
  ISpindizzyZoneTheme* cZoneTheme;
  I3DModel* cFlagModel;
  std::map<std::string, IScriptCall*> cScripts;
  std::vector<IArgument*> cArguments;
  
  // Runtime data
  bool cVisited;
  bool cFlagged;
  
  // Edit data
  bool cHasFocus;
  
  public:
  ElementSpindizzyZone(ISpindizzyZoneType*, DOMNodeWrapper*, BlockLocation*, IResourceAccessor*, I3DModelType*, IElementContainer*);
  ElementSpindizzyZone(ISpindizzyZoneType*, BlockArea*, IElementContainer* = nullptr, ISpindizzyZoneTheme* = nullptr);
  
  IElementContainer* getElementContainer();
  void updateFlagModel(I3DModelType*);
  
  void setVisited();
  bool isVisited();
  IColour* getColour(SpindizzyZoneThemeColour*);
  void setFlag(bool);
  void setTheme(ISpindizzyZoneTheme*);
  ISpindizzyZoneTheme* getTheme();
  IScriptCall* getScriptCall(const std::string&);
  void renderPreview(Vertex&, Vertex&, bool);
  void renderEditing(BlockArea&, bool, bool);
  void renderEditing(bool);
  bool intersects(ElementSpindizzyZone*);
  bool intersects(BlockLocation&);
  
  /************************************\
   * Implements IObjectWithProperties *
  \************************************/
  std::string getTypeName();
  std::vector<IObjectProperty*> getProperties(IComponentContainer*);
  
  /***********************\
   * Implements IElement *
  \***********************/
  IElementType* getElementType();
  bool initElement(IUniverse*, unsigned int);
  void renderEditing();
  void renderStatic();
  void renderRuntime();
  bool renderSelectionHighlight();
  void updateEditing(unsigned int);
  void updateRuntime(unsigned int);
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty(IElement*);
  IElementBounds* getBounds();
  void focusGained(ILayerEditingContext*);
  void focusLost(ILayerEditingContext*);
  void cursorAppeared(ILayerEditingContext*, Vertex&);
  void cursorMoved(ILayerEditingContext*, Vertex&, Vertex&);
  PickedElement* pickElement(Vertex&, Vertex&);
  void reset();

  /*****************************\
   * Implements IElementBounds *
  \*****************************/
  float getWest();
  float getEast();
  float getSouth();
  float getNorth();
  float getBottom();
  float getTop();
  
  /********************************\
   * Implements IElementContainer *
  \********************************/
  void addElement(IElement*);
  void removeElement(IElement*);
  void updateElement(IElement*);
  void addArgumentValue(IArgument*);
  void restrictCursor(Vertex&);
  IUniverse* getUniverse();
  
  /************************\
   * Implements IBoundary *
  \************************/
  bool contains(Vertex&);
  bool isEntered(Vertex&, Vertex&);
  bool isExited(Vertex&, Vertex&);
  BlockArea* getCoverage();
  void setArguments();  
  void unsetArguments();
  void setDirty();
};

#endif
