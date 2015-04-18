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
#ifndef ELEMENT_SPINDIZZY_ITEM_H
#define ELEMENT_SPINDIZZY_ITEM_H

#include <cmath>
#include <cstdlib>
#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Collision.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/Boundaries//IBoundary.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "ISpindizzyJewelType.h"
#include "ISpindizzyJewelSet.h"

class ElementSpindizzyItem:public Element,
                           public IBoundary {
  private:
  ISpindizzyJewelType* cItemType;
  I3DModel* cModel;
  IElementContainer* cContainer;
    
  // Definition data
  BlockLocation cLocation;

  // Runtime data
  bool cCollected;
  Vertex cVertexLocation;

  void collect();

  public:
  ElementSpindizzyItem(ISpindizzyJewelType*, BlockLocation*, I3DModelType*, IElementContainer*);
  
  bool isCollected();
  
  /************************\
   * Implements IBoundary *
  \************************/
  bool contains(Vertex& location);
  bool isEntered(Vertex& start, Vertex& end);
  bool isExited(Vertex& start, Vertex& end);
  void setDirty();
  BlockArea* getCoverage();
  void setArguments();
  void unsetArguments();

  /***********************\
   * Implements IElement *
  \***********************/
  IElementType* getElementType();
  bool initElement(unsigned int);
  void renderStatic();
  void renderRuntime();
  void updateRuntime(unsigned int);
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  IElementBounds* getBounds();
};

#endif
