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
#ifndef DEFAULT_ELEMENT_HANDLER_H
#define DEFAULT_ELEMENT_HANDLER_H

#include <GL/gl.h>

#include "IDynamicElement.h"
#include "IElement.h"
#include "IElementHandler.h"
#include "IVisualElement.h"

template<class T = IElement> class DefaultElementHandler:public IElementHandler {
  protected:
  std::vector<IVisualElement*> cVisualElements;
  std::vector<IDynamicElement*> cDynamicElements;
  std::vector<IDynamicElement*> cDynamicElementsRuntime;

  public:
  void addElement(IElement* element) {
    std::vector<IVisualElement*> mVisualElements = element->getVisualElements();
    for (unsigned int i = 0; i < mVisualElements.size(); i++) {
      cVisualElements.push_back(mVisualElements[i]);
    }
    std::vector<IDynamicElement*> mDynamicElements = element->getDynamicElements();
    for (unsigned int i = 0; i < mDynamicElements.size(); i++) {
      cDynamicElements.push_back(mDynamicElements[i]);
    }
    std::vector<IDynamicElement*> mDynamicElementsRuntime = element->getDynamicElementsRuntime();
    for (unsigned int i = 0; i < mDynamicElementsRuntime.size(); i++) {
      cDynamicElementsRuntime.push_back(mDynamicElementsRuntime[i]);
    }
    elementAdded(element);
  }
  
  void removeElement(IElement* element) {
    std::vector<IVisualElement*> mVisualElements = element->getVisualElements();
    for (unsigned int i = 0; i < mVisualElements.size(); i++) {
      for (unsigned int j = 0; j < cVisualElements.size(); j++) {
        if (mVisualElements[i] == cVisualElements[j]) {
          cVisualElements.erase(cVisualElements.begin() + j);
        }
      }
    }
    std::vector<IDynamicElement*> mDynamicElements = element->getDynamicElements();
    for (unsigned int i = 0; i < mDynamicElements.size(); i++) {
      for (unsigned int j = 0; j < cDynamicElements.size(); j++) {
        if (mDynamicElements[i] == cDynamicElements[j]) {
          cDynamicElements.erase(cDynamicElements.begin() + j);
        }
      }
    }
    std::vector<IDynamicElement*> mDynamicElementsRuntime = element->getDynamicElementsRuntime();
    for (unsigned int i = 0; i < mDynamicElementsRuntime.size(); i++) {
      for (unsigned int j = 0; j < cDynamicElementsRuntime.size(); j++) {
        if (mDynamicElementsRuntime[i] == cDynamicElementsRuntime[j]) {
          cDynamicElementsRuntime.erase(cDynamicElementsRuntime.begin() + j);
        }
      }
    }
    elementRemoved(element);
  }
  
  virtual void elementAdded(IElement* element) {
  }
  
  virtual void elementRemoved(IElement* element) {
  }

  bool isRenderer() {
    return !cVisualElements.empty();
  }
  
  virtual void render() {
    // NOTE: Enable this line to see which surfaces are dynamic!
/*    glColor3f(0.3f, 0.3f, 0.3f);*/
    for (unsigned int i = 0; i < cVisualElements.size(); i++) {
      cVisualElements[i]->render();
    }
  }
  
  bool isUpdater() {
    return !cDynamicElements.empty();
  }
  
  virtual void update(int ticks) {
    for (unsigned int i = 0; i < cDynamicElements.size(); i++) {
      cDynamicElements[i]->update(ticks);
    }
  }
  
  virtual void updateRuntime(int ticks) {
    for (unsigned int i = 0; i < cDynamicElementsRuntime.size(); i++) {
      cDynamicElementsRuntime[i]->update(ticks);
    }
  }
};

#endif
