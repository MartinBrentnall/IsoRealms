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
#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H

#include "BlockLocation.h"
#include "DefaultElementHandler.h"
#include "IComponentContainer.h"
#include "IElementContainer.h"
#include "IElementFactory.h"
#include "IElementGateway.h"

template<class T1 = IElementSet, class T2 = IElement> class ElementFactory:public IElementFactory {
  private:
  T1* cElementSet;
  IElementGateway* cElementGateway;

  public:
  ElementFactory(T1* elementSet) {
    cElementSet = elementSet;
  }

  void setEditingContext(BlockLocation* blockLocation, IElementGateway* elementGateway, IComponentContainer* componentContainer) {
    setEditingContext(blockLocation, componentContainer);
    cElementGateway = elementGateway;
  }
  
  virtual void setEditingContext(BlockLocation*, IComponentContainer*) = 0;
  
  void addElement(T2* element) {
    IElementContainer* mElementContainer = cElementGateway->pushElement(element);
    cElementSet->addElement(element, mElementContainer);
  }
  
  void removeElement(IElement* element) {
    cElementGateway->removeElement(element);
  }
  
  void removingElement(IElement* element, IElementContainer* elementContainer) {
    cElementSet->removingElement(element, elementContainer);
  }

  /******************************\
   * Implements IElementFactory *
  \******************************/
  T1* getElementSet() {
    return cElementSet;
  }
  
  void registerElement(T2* element, IElementContainer* elementContainer) {
    cElementSet->addElement(element, elementContainer);
  }
};

#endif
