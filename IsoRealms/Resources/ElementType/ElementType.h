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
#ifndef ELEMENT_TYPE_H
#define ELEMENT_TYPE_H

#include <IsoRealms/BlockLocation.h>
#include <IsoRealms/DefaultElementHandler.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IPlugin.h>

#include "IElementType.h"

template<class T1 = IPlugin, class T2 = IElement> class ElementType:public IElementType {
  private:
  T1* cModule;
//  IElementGateway* cElementGateway;

  public:
  ElementType(T1* elementSet) {
    cModule = elementSet;
  }

  void setEditingContext(BlockLocation* blockLocation, IComponentContainer* componentContainer) {
    setEditingContext(blockLocation, componentContainer);
  }
  
  void addElement(T2* element) {
//   TODO    IElementContainer* mElementContainer = cElementGateway->pushElement(element);
//   TODO cModule->addElement(element, mElementContainer);
  }
  
  void removeElement(IElement* element) {
//     cElementGateway->removeElement(element);
  }
  
  void removingElement(IElement* element) {
// TODO    cModule->removingElement(element, elementContainer);
  }
  
  void addingElement(IElement* element) {
// TODO    cModule->addingElement(element, elementContainer);
  }

  /***************************\
   * Implements IElementType *
  \***************************/
  T1* getElementSet() {
    return cModule;
  }
};

#endif
