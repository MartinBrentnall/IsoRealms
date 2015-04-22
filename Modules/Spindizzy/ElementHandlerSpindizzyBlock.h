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

#include "ElementSpindizzyBlock.h"
#include "ISpindizzyBlockSet.h"
#include "SpindizzyBlockState.h"

class ElementHandlerSpindizzyBlock:public Element,
                                   public IArgument {
  private:
  ISpindizzyBlockSet* cModuleInterface;
  std::vector<ElementSpindizzyBlock*> cElements;
  std::vector<SpindizzyBlockState*> cClues;

  public:
  ElementHandlerSpindizzyBlock(ISpindizzyBlockSet*);

  void addElement(ElementSpindizzyBlock*);
  void removeElement(ElementSpindizzyBlock*);
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
  
  /***********************\
   * Implements IElement *
  \***********************/
  void renderRuntime();
  void updateRuntime(unsigned int);
  IElementType* getElementType();
  void renderStatic();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
  bool initElement(unsigned int);
  IElementBounds* getBounds();
};

#endif
