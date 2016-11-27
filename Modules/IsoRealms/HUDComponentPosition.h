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
#ifndef HUD_COMPONENT_POSITION_H
#define HUD_COMPONENT_POSITION_H

#include <map>
#include <string>

#include <IsoRealms/Collision.h>
#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/ElementType/IElement.h>
#include <IsoRealms/Resources/ElementType/IElementContainer.h>
#include <IsoRealms/Resources/ElementType/PickedElement.h>
#include <IsoRealms/ScreenConfiguration.h>

#include "IComponentSources.h"
#include "IHUDComponentRelation.h"

class HUDComponentPosition:public IElement {
  private:
  enum Handle {
    NONE,
    WEST,
    EAST,
    SOUTH,
    NORTH,
    BOTTOM,
    TOP
  };
    
  static const float EDIT_HANDLE_RADIUS;
  
  IElement* cElement;
  IHUDComponentRelation* cLeftRelation;
  IHUDComponentRelation* cRightRelation;
  IHUDComponentRelation* cTopRelation;
  IHUDComponentRelation* cBottomRelation;
  Handle cSelectedHandle;

  void renderEditingHandle(float, float, Handle);
  void testHandlePick(ElementPickRay*, float, float, Handle);
  
  public:
  HUDComponentPosition(IHUDComponentRelation*, IHUDComponentRelation*, IHUDComponentRelation*, IHUDComponentRelation*);
  
  void setElement(IElement*);
  
  void save(DOMNodeWriter*, IComponentSources*, IResourceLocator*);

  // Editing functions
  void renderSelection();

  /***********************\
   * Implements IElement *
  \***********************/
  IElementType* getElementType();
  bool initElement(IUniverse*, unsigned int);
  void renderStatic();
  void renderRuntime();
  void renderEditing();
  bool renderSelectionHighlight();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void input(SDL_Event&);
  bool inputEditor(SDL_Event&, ILayer*);
  bool isVisualRuntime();
  bool isVisualEditing();
  bool isDynamicRuntime();
  bool isDynamicEditing();
  bool isInteractive();
  void save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
  void initRuntime();
  void staticChanged();
  bool isImplicit();  
  IElementBounds* getBounds();  
  void focusGained(ILayerEditingContext*);
  void focusLost(ILayerEditingContext*);
  void cursorMoved(ILayerEditingContext*, Vertex&, Vertex&);
  void cursorAppeared(ILayerEditingContext*, Vertex&);
  void processCursorMovement(ILayerEditingContext*, Vertex&, Vertex&);
  void processCursorAppearance(ILayerEditingContext*, Vertex&);
  PickedElement* pickElement(Vertex&, Vertex&);
  void reset();

  /************************************\
   * Implements IObjectWithProperties *
  \************************************/
  std::string getTypeName();
  std::vector<IObjectProperty*> getProperties(IComponentContainer*);

  float getWest();
  float getEast();
  float getSouth();
  float getNorth();
  float getTop();
  float getBottom();
  
  virtual ~HUDComponentPosition() {}
};

#endif
