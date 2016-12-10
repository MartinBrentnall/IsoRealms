/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef ELEMENT_HANDLER_ZONE_H
#define ELEMENT_HANDLER_ZONE_H

#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/ElementType/ElementCollection.h>

#include "ElementSpindizzyZone.h"
#include "IModuleElementHandlerZone.h"

class ElementHandlerZone:public Element,
                         public IArgument {
  private:
  IModuleElementHandlerZone* cModuleInterface;
  bool cSingleZone;
  ElementSpindizzyZone* cZone;
  ElementCollection<ElementSpindizzyZone> cElements;

  public:
  ElementHandlerZone(IModuleElementHandlerZone*);

  void addElement(ElementSpindizzyZone*);
  void removeElement(ElementSpindizzyZone*);
  bool isEmpty();

  /***********************\
   * Scripting Interface *
  \***********************/
  void setSingleZone(bool);
  void setCurrentZone(ElementSpindizzyZone*);

  /************************\
   * Implements IArgument *
  \************************/
  void setValue();
  void unsetValue();
  
  /************************************\
   * Implements IObjectWithProperties *
  \************************************/
  std::string getTypeName();
  std::vector<IObjectProperty*> getProperties(IComponentContainer*);
  
  /***********************\
   * Implements IElement *
  \***********************/
  void renderEditing();
  void renderRuntime();
  bool renderSelectionHighlight();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  IElementType* getElementType();
  void renderStatic();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void saveCache(DOMNodeWriter*);
  void setDirty();
  bool initElement(IUniverse*, unsigned int);
  IElementBounds* getBounds();
  bool isSelectable();
  void cursorMoved(ILayerEditingContext*, Vertex&, Vertex&);
  void cursorAppeared(ILayerEditingContext*, Vertex&);
  PickedElement* pickElement(Vertex&, Vertex&);
  IElementContainer* getElementContainer();
  void reset();
  bool isImplicit();
};

#endif
