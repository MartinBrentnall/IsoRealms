/*
 * Copyright 2016 Martin Brentnall
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
#ifndef RESOURCE_ELEMENT_HUD_ABSTRACT_H
#define RESOURCE_ELEMENT_HUD_ABSTRACT_H

#include <cmath>
#include <GL/glew.h>

#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>

#include "IElementRelationManager.h"
#include "IHUDComponentRelation.h"
#include "HUDComponentPosition.h"

class ResourceElementHUDAbstract:public IElementType {
  private:
  static const std::string X_AXIS_DESCRIPTOR;
  static const std::string Y_AXIS_DESCRIPTOR;
  static const std::string Z_AXIS_DESCRIPTOR;
    
  class SizeRelation:public IHUDComponentRelation {
    private:
    IHUDComponentRelation* cRelation;
    float cOffset;
      
    public:
    SizeRelation(IHUDComponentRelation*, float);
    
    /************************************\
     * Implements IHUDComponentRelation *
    \************************************/
    float getLocation();
    void save(DOMNodeWriter*, const std::string&, IComponentSources*);
    void renderRelation();
  };
  
  IElementRelationManager* cManager;
  
  float getSize(DOMNodeWrapper*, const std::string&);
  
  public:
  ResourceElementHUDAbstract(IElementRelationManager*);
    
  IHUDComponentRelation* getRelation(const std::string&, const std::string&);
  
  /***************************\
   * Implements IElementType *
  \***************************/
  void loadElement(DOMNodeWrapper*, DOMNodeWrapper*, BlockLocation*, IElementContainer*, IResourceAccessor*, bool);
  
  virtual IElement* createHUDElement(DOMNodeWrapper*, DOMNodeWrapper*, BlockLocation*, IResourceAccessor*, bool, HUDComponentPosition*) = 0;
};

#endif
