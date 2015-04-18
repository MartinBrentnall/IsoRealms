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

#include <GL/glew.h>

#include "IDynamicElement.h"
#include "IVisualElement.h"
#include "Resources/ElementType/IElement.h"
#include "Resources/ElementType/IElementHandler.h"

template<class T = IElement> class DefaultElementHandler:public IElementHandler {
  protected:
  std::vector<IElement*> cVisualElementsEditor;
  std::vector<IElement*> cVisualElementsRuntime;
  std::vector<IElement*> cDynamicElementsEditor;
  std::vector<IElement*> cDynamicElementsRuntime;

  public:
  void addElement(IElement* element) {
    if (element->isVisualRuntime()) {
      cVisualElementsRuntime.push_back(element);
    }
    if (element->isVisualEditing()) {
      cVisualElementsEditor.push_back(element);
    }
    if (element->isDynamicEditing()) {
      cDynamicElementsEditor.push_back(element);
    }
    if (element->isDynamicRuntime()) {
      cDynamicElementsRuntime.push_back(element);
    }
    elementAdded(element);
  }
  
  void eraseElement(std::vector<IElement*>& vector, IElement* element) {
    for (unsigned int i = 0; i < vector.size(); i++) {
      if (vector[i] == element) {
        vector.erase(vector.begin() + i);
        return;
      }
    }
  }
  
  void removeElement(IElement* element) {
    eraseElement(cVisualElementsEditor, element);
    eraseElement(cVisualElementsRuntime, element);
    eraseElement(cDynamicElementsEditor, element);
    eraseElement(cDynamicElementsRuntime, element);
    elementRemoved(element);
  }
  
  virtual void elementAdded(IElement* element) {
  }
  
  virtual void elementRemoved(IElement* element) {
  }

  bool isRenderer() {
    return !cVisualElementsRuntime.empty();
  }
  
  virtual void render() {
    // NOTE: Enable this line to see which surfaces are dynamic!
/*    glColor3f(0.3f, 0.3f, 0.3f);*/
    for (unsigned int i = 0; i < cVisualElementsRuntime.size(); i++) {
      cVisualElementsRuntime[i]->renderRuntime();
    }
  }
  
  bool isUpdater() {
    return !cDynamicElementsRuntime.empty();
  }
  
  virtual void update(int ticks) {
    for (unsigned int i = 0; i < cDynamicElementsEditor.size(); i++) {
      cDynamicElementsEditor[i]->updateEditing(ticks);
    }
  }
  
  virtual void updateRuntime(int ticks) {
    for (unsigned int i = 0; i < cDynamicElementsRuntime.size(); i++) {
      cDynamicElementsRuntime[i]->updateRuntime(ticks);
    }
  }
};

#endif
