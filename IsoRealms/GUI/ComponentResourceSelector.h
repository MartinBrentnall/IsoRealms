/*
 * Copyright 2015 Martin Brentnall
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
#ifndef COMPONENT_RESOURCE_SELECTOR_H
#define COMPONENT_RESOURCE_SELECTOR_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <IsoRealms/Resources/IResourceSelectionListener.h>
#include <IsoRealms/Resources/IResourceSelector.h>

#include "IComponentSelectorListener.h"
#include "ISelector.h"
#include "SelectableComponent.h"

template <class T> class ComponentResourceSelector:public ISelector,
                                                   public IResourceSelectionListener<T> {
  private:
  IComponentSelectorListener<T>* cParent;
  IResourceSelector* cResourceSelector;
  T* cResource;
  
  public:
  ComponentResourceSelector(IComponentSelectorListener<T>* parent, T* resource, IResourceSelector* resourceSelector) {
//    cBorrowedColour = NULL;
    cParent = parent;
    cResourceSelector = resourceSelector;
    cResource = resource;
  }
  
  /************************\
   * Implements ISelector *
  \************************/
  void render(SelectableComponent* component) {
    float mLeft =   component->getLeft();
    float mRight =  component->getRight();
    float mTop =    component->getTop();
    float mBottom = component->getBottom();
    
    glBegin(GL_QUADS);
    glVertex2f(mLeft,  mBottom);
    glVertex2f(mRight, mBottom);
    glVertex2f(mRight, mTop);
    glVertex2f(mLeft,  mTop);
    glEnd();
  }
  
  void selected() {
    cResourceSelector->addResourceSelectionListener(this);
  }
  
  void deselected() {
    cResourceSelector->removeResourceSelectionListener(this);
//     if (cBorrowedColour != NULL) {
//       cResourceSelector->notifyResourceOwned(cBorrowedColour);
//       cBorrowedColour = NULL;
//     }
  }
  
  /********************************************\
   * Implements IResourceSelectionListener<T> *
  \********************************************/
  void resourceSelected(T* resource) {
//     if (cBorrowedColour != NULL) {
//       cResourceSelector->notifyResourceReleased(cBorrowedColour);
//     }
    cResource = resource;
//     cBorrowedColour = colour;
    cParent->selected(this, resource);
  }
};
    
#endif
