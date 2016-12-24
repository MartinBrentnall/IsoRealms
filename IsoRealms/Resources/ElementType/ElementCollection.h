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
#ifndef ELEMENT_COLLECTION_H
#define ELEMENT_COLLECTION_H

#include <algorithm>
#include <limits>
#include <vector>

#include <IsoRealms/ILayerEditingContext.h>

#include "IElementBounds.h"
#include "PickedElement.h"

template <class T> class ElementCollection : public IElementBounds {
  private:
  std::vector<T*> cElements;
  
  public:
  IElementBounds* getBounds() {
    return this;
  }
  
  std::size_t size() {
    return cElements.size();
  }
  
  bool init(IUniverse* universe, unsigned int pass) {
    bool mSuccess = true;
    for (T* mElement : cElements) {
      if (!mElement->initElement(universe, pass)) {
        mSuccess = false;
      }
    }
    return mSuccess;
  }
  
  T*& operator[](std::size_t index) {
    return cElements[index];
  }
  
  const T*& operator[](std::size_t index) const {
    return cElements[index];
  };
  
  unsigned int getIndex(T* element) {
    return std::find(cElements.begin(), cElements.end(), element) - cElements.begin();
  }
  
  void addElement(T* element) {
    cElements.push_back(element);
  }
  
  void removeElement(T* element) {
    for (std::size_t i = 0; i < cElements.size(); i++) {
      if (cElements[i] == element) {
        cElements.erase(cElements.begin() + i);
      }
    }
  }
  
  bool isEmpty() {
    return cElements.empty();
  }
  
  void renderRuntime() {
    for (T* mElement : cElements) {
      mElement->renderRuntime();
    }    
  }

  void renderEditing() {
    for (T* mElement : cElements) {
      mElement->renderEditing();
    }    
  }

  std::vector<IVisualElement*> getStaticVisuals() {
    std::vector<IVisualElement*> mAllVisuals;
    for (T* mElement : cElements) {
      std::vector<IVisualElement*> mElementVisuals = mElement->getStaticVisuals();
      mAllVisuals.insert(std::end(mAllVisuals), std::begin(mElementVisuals), std::end(mElementVisuals));
    }
    return mAllVisuals;
  }
  
  void updateRuntime(unsigned int milliseconds) {
    for (T* mElement : cElements) {
      mElement->updateRuntime(milliseconds);
    }
  }
  
  void updateEditing(unsigned int milliseconds) {
    for (T* mElement : cElements) {
      mElement->updateEditing(milliseconds);
    }
  }
  
  float getWest() {
    if (cElements.size() == 0) {
      return 0.0f;
    }
    float mValue = std::numeric_limits<float>::max();
    for (T* mElement : cElements) {
      IElementBounds* mBounds = mElement->getBounds();
      mValue = std::min(mBounds->getWest(), mValue);
    }
    return mValue;
  }

  float getEast() {
    if (cElements.size() == 0) {
      return 0.0f;
    }
    float mValue = std::numeric_limits<float>::lowest();
    for (T* mElement : cElements) {
      IElementBounds* mBounds = mElement->getBounds();
      mValue = std::max(mBounds->getEast(), mValue);
    }
    return mValue;
  }
  
  float getSouth() {
    if (cElements.size() == 0) {
      return 0.0f;
    }
    float mValue = std::numeric_limits<float>::max();
    for (T* mElement : cElements) {
      IElementBounds* mBounds = mElement->getBounds();
      mValue = std::min(mBounds->getSouth(), mValue);
    }
    return mValue;
  }

  float getNorth() {
    if (cElements.size() == 0) {
      return 0.0f;
    }
    float mValue = std::numeric_limits<float>::lowest();
    for (T* mElement : cElements) {
      IElementBounds* mBounds = mElement->getBounds();
      mValue = std::max(mBounds->getNorth(), mValue);
    }
    return mValue;
  }

  float getBottom() {
    if (cElements.size() == 0) {
      return 0.0f;
    }
    float mValue = std::numeric_limits<float>::max();
    for (T* mElement : cElements) {
      IElementBounds* mBounds = mElement->getBounds();
      mValue = std::min(mBounds->getBottom(), mValue);
    }
    return mValue;
  }

  float getTop() {
    if (cElements.size() == 0) {
      return 0.0f;
    }
    float mValue = std::numeric_limits<float>::lowest();
    for (T* mElement : cElements) {
      IElementBounds* mBounds = mElement->getBounds();
      mValue = std::max(mBounds->getTop(), mValue);
    }
    return mValue;
  }  

  void cursorMoved(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
    for (T* mElement : cElements) {
      mElement->processCursorMovement(editingContext, start, end);
    }
  }
  
  void cursorAppeared(ILayerEditingContext* editingContext, Vertex& location) {
    for (T* mElement : cElements) {
      mElement->processCursorAppearance(editingContext, location);
    }
  }
  
  PickedElement* pickElement(Vertex& start, Vertex& end) {
    PickedElement* mClosestPickedElement = nullptr;
    for (T* mElement : cElements) {
      PickedElement* mPickedElement = mElement->pickElement(start, end);
      if (mPickedElement != nullptr) {
        if (mClosestPickedElement == nullptr || mPickedElement->getGradient() < mClosestPickedElement->getGradient()) {
          // TODO: Delete Picked Element
          mClosestPickedElement = mPickedElement;
        }
      }
    }
    return mClosestPickedElement;
  }
  
  void reset() {
    for (T* mElement : cElements) {
      mElement->reset();
    }
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator, BlockLocation& location) {
    for (T* mElement : cElements) {
      DOMNodeWriter* mElementBranch = node->addBranch("Element");
      mElement->save(mElementBranch, resourceLocator, location);
    }
  }

  void saveCache(DOMNodeWriter* cache) {
    for (T* mElement : cElements) {
      mElement->saveCache(cache);
    }
  }  
};

#endif
