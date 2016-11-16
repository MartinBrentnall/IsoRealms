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
#ifndef ELEMENT_HANDLER_SPINDIZZY_BLOCK_H
#define ELEMENT_HANDLER_SPINDIZZY_BLOCK_H

#include <IsoRealms/Resources/ElementType/Element.h>
#include <IsoRealms/Resources/ElementType/ElementCollection.h>

#include "ISpindizzyBlock.h"
#include "ISpindizzyBlockSet.h"
#include "ISpindizzyElementManager.h"
#include "SpindizzyBlockState.h"

class ElementHandlerSpindizzyBlock:public Element,
                                   public IArgument,
                                   public ISpindizzyElementManager {
  private:
  ISpindizzyBlockSet* cModuleInterface;
  IElementContainer* cContainer;
  ElementCollection<ISpindizzyBlock> cElements;
  std::vector<SpindizzyBlockState*> cClues;

  public:
  ElementHandlerSpindizzyBlock(ISpindizzyBlockSet*, IElementContainer*);

  void addElement(ISpindizzyBlock*);
  void removeElement(ISpindizzyBlock*);
  bool isEmpty();

  /***********************\
   * Scripting Interface *
  \***********************/
  unsigned int getClueCount();
  I3DModel* getClue(unsigned int);

  /************************\
   * Implements IArgument *
  \************************/
  void setValue();
  void unsetValue();
  
  /************************************\
   * Implements IObjectWithProperties *
  \************************************/
  std::string getTypeName();
  std::vector<IObjectProperty*> getProperties();
  
  /***********************\
   * Implements IElement *
  \***********************/
  void renderRuntime();
  void renderEditing();
  void updateRuntime(unsigned int);
  IElementType* getElementType();
  void renderStatic();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
  bool initElement(unsigned int);
  IElementBounds* getBounds();
  bool isSelectable();
  void cursorMoved(ILayerEditingContext*, Vertex&, Vertex&);
  void cursorAppeared(ILayerEditingContext*, Vertex&);
  bool isImplicit();

  /***************************************\
   * Implements ISpindizzyElementManager *
  \***************************************/
  void setArguments();
  void unsetArguments();
  void setDirty(IElement*);
  IElementContainer* getElementContainer();
  PickedElement* pickElement(Vertex&, Vertex&);
};

#endif
