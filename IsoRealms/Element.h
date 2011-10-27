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
#ifndef ELEMENT_H
#define ELEMENT_H

#include "IElement.h"
#include "IElementContainer.h"
#include "IElementFactory.h"
#include "IElementSet.h"

/**
 * 
 */
template<class T1 = IElementSet, class T2 = IElementFactory> class Element:public IElement {
  private:

  /**
   * TODO:
   */
  T2* cElementFactory;

  /**
   * The element container is the entity that is holding the element for use in
   * game.  For example, this may be a Zone or a Map.
   */
  IElementContainer* cElementContainer;

  public:
  Element(T2* elementFactory) {
    cElementFactory = elementFactory;
    cElementContainer = NULL;
  }

  /***********************\
   * Implements IElement *
  \***********************/
  T1* getElementSet() {
    return cElementFactory->getElementSet();
  }

  T2* getElementFactory() {
    return cElementFactory;
  }

  void signalElementDirty() {
    if (cElementContainer) {
      cElementContainer->elementDirty(this);
    }
  }

  void setElementContainer(IElementContainer* elementContainer) {
    cElementContainer = elementContainer;
  }

  std::vector<IDynamicElement*> getDynamicElements() {
    std::vector<IDynamicElement*> mEmptyVector;
    return mEmptyVector;
  }

  std::vector<IDynamicElement*> getDynamicElementsRuntime() {
    std::vector<IDynamicElement*> mEmptyVector;
    return mEmptyVector;
  }

  std::vector<IInteractiveElement*> getInteractiveElements() {
    std::vector<IInteractiveElement*> mEmptyVector;
    return mEmptyVector;
  }

  void save(DOMNodeWriter*, BlockLocation&) {}
  bool initElement(unsigned int, bool) {return true;}
  void removed() {}
  void added() {}
  void setRuntimeContext(IMap*) {}
  void renderStatic() {}
  void renderStaticEditing() {}

  ~Element() {}
};

#endif
