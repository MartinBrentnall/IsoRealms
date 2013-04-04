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
#ifndef SPINDIZZY_JEWEL_H
#define SPINDIZZY_JEWEL_H

#include <cmath>
#include <cstdlib>
#include <GL/glew.h>

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/Collision.h>
#include <IsoRealms/IDynamicElement.h>
#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/IVisualElement.h>
#include <IsoRealms/Resources/3DModel/I3DModel.h>
#include <IsoRealms/Resources/Collectables/ICollectable.h>
#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "BaseSpindizzyJewelType.h"
#include "ISpindizzyJewelSet.h"

class SpindizzyJewel:public Element<ISpindizzyJewelSet, BaseSpindizzyJewelType>,
                     public ICollectable,
                     public IDynamicElement,
                     public IVisualElement {
  private:

  I3DModel* cModel;
    
  // Definition data
  BlockLocation cLocation;

  // Runtime data
  bool cCollected;
  Vertex cVertexLocation;

  void collect();

  public:
  SpindizzyJewel(BaseSpindizzyJewelType*, BlockLocation*, I3DModelFactory*);
  
  /***************************\
   * Implements ICollectable *
  \***************************/
  bool isCollected(Vertex& start, Vertex& end);
  void setDirty();

  /***********************\
   * Implements IElement *
  \***********************/
  bool initElement(unsigned int);
  void renderStatic();
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IDynamicElement*> getDynamicElementsRuntime();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);

  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(unsigned int);

  /*****************************\
   * Implements IVisualElement *
  \*****************************/
  void render();
};

#endif
