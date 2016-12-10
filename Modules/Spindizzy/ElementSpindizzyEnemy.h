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
#ifndef ELEMENT_SPINDIZZY_ENEMY_H
#define ELEMENT_SPINDIZZY_ENEMY_H

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "IElementSpindizzyDynamic.h"

class ElementSpindizzyEnemy:public IElementSpindizzyDynamic,
                            public IElementBounds {
  private:
  IElementType* cEnemyType;
  IElementContainer* cContainer;
  BlockLocation cStartLocation;
  Vertex* cCurrentLocation;
  I3DModel* cEnemyModel;

  public:
  ElementSpindizzyEnemy(IElementType*, BlockLocation*, I3DModelType*, IElementContainer*);
  ElementSpindizzyEnemy(IElementType*, DOMNodeWrapper*, BlockLocation*, IElementContainer*);

  void setModelType(I3DModelType*, I3DModelType*);

  IElementContainer* getElementContainer();
  
  /***************************************\
   * Implements IElementSpindizzyDynamic *
  \***************************************/
  void reset();
  void processCursorMovement(ILayerEditingContext*, Vertex&, Vertex&);
  void processCursorAppearance(ILayerEditingContext*, Vertex&);
  PickedElement* pickElement(Vertex&, Vertex&);
  
  /************************************\
   * Implements IObjectWithProperties *
  \************************************/
  std::string getTypeName();
  std::vector<IObjectProperty*> getProperties(IComponentContainer*);
  
  /***********************\
   * Implements IElement *
  \***********************/
  IElementType* getElementType();
  void renderEditing();
  void renderStatic();
  void renderRuntime();
  bool renderSelectionHighlight();
  void updateRuntime(unsigned int milliseconds);
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void saveCache(DOMNodeWriter*);
  void setDirty();
  bool initElement(IUniverse*, unsigned int);
  IElementBounds* getBounds();
  
  /*****************************\
   * Implements IElementBounds *
  \*****************************/
  float getWest();
  float getEast();
  float getSouth();
  float getNorth();
  float getBottom();
  float getTop();
};

#endif
