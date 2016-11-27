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
#ifndef ELEMENT_HUD_STRING_H
#define ELEMENT_HUD_STRING_H

#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>

#include "HUDComponentPosition.h"
#include "IElementTypeHUDString.h"

class ElementHUDString:public Element {
  private:
  IElementTypeHUDString* cElementType;
  HUDComponentPosition* cElementDimensions;
  
  public:
  ElementHUDString(IElementTypeHUDString*, HUDComponentPosition*);

  /************************************\
   * Implements IObjectWithProperties *
  \************************************/
  std::string getTypeName();
  std::vector<IObjectProperty*> getProperties(IComponentContainer*);
  
  /***********************\
   * Implements IElement *
  \***********************/
  IElementType* getElementType();
  void renderStatic();
  void setDirty();
  IElementBounds* getBounds();
  void renderRuntime();
  bool renderSelectionHighlight();
  void updateRuntime(unsigned int);
  void save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*, BlockLocation&);
};

#endif

